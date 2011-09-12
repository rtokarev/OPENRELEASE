/*
 * Copyright (c) 2011 Roman Tokarev <roman.s.tokarev@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *        may be used to endorse or promote products derived from this software
 *        without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio_wrap.h>

#include <config.h>
#include <log.h>

#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pty.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <unistd.h>
#include <utmp.h>


#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


static int mfifo(const char *name, int mode)
{
	if (mkfifo(name, S_IFIFO|S_IRWXU) == -1 && errno != EEXIST)
		return -1;

	return open(name, mode);
}

static int pty_open(int *ptyfd, int *ttyfd)
{

	char buf[64], namebuf[64];
	int i;
	const char *ptymajors = "pqrstuvwxyzabcdefghijklmnoABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char *ptyminors = "0123456789abcdef";
	int num_minors = strlen(ptyminors);
	int num_ptys = strlen(ptymajors) * num_minors;

	if (openpty(ptyfd, ttyfd, NULL, NULL, NULL) == 0)
		return 0;

	/* From dropbear: BSD-style pty code. */
	for (i = 0; i < num_ptys; i++) {
		snprintf(buf, sizeof buf, "/dev/pty%c%c", ptymajors[i / num_minors],
			 ptyminors[i % num_minors]);
		snprintf(namebuf, sizeof namebuf, "/dev/tty%c%c",
		    ptymajors[i / num_minors], ptyminors[i % num_minors]);

		*ptyfd = open(buf, O_RDWR | O_NOCTTY);
		if (*ptyfd < 0) {
			/* Try SCO style naming */
			snprintf(buf, sizeof buf, "/dev/ptyp%d", i);
			snprintf(namebuf, sizeof namebuf, "/dev/ttyp%d", i);
			*ptyfd = open(buf, O_RDWR | O_NOCTTY);
			if (*ptyfd < 0)
				continue;
		}

		/* Open the slave side. */
		*ttyfd = open(namebuf, O_RDWR | O_NOCTTY);
		if (*ttyfd < 0) {
			close(*ptyfd);

			return -1;
		}

		return 0;
	}

	return -1;
}

static void close_fd_(int *fd)
{
	if (*fd != -1)
		(void)close(*fd);
}

int listen_port(char *ipaddr, int port)
{
	int fd __attribute__((cleanup(close_fd_))) = -1,
	    sock = -1;
	int flags;
	struct sockaddr_in sin;

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1) {
		say_error("can't create socket: %m");

		return -1;
	}

	if ((flags = fcntl(fd, F_GETFL, 0) < 0) ||
	    fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		say_error("can't move socket to non-blocking state: %m");

		return -1;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if (ipaddr == NULL || ipaddr[0] == '\0')
		sin.sin_addr.s_addr = INADDR_ANY;
	else if (inet_aton(ipaddr, &sin.sin_addr) == 0) {
		say_error("inet_aton `%s': %m", ipaddr);

		return -1;
	}

	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) != 0) {
		say_error("can't bind to `%s:%d': %m", ipaddr, port);

		return -1;
	}
	if (listen(fd, 1) != 0) {
		say_error("can't listen on `%s:%d': %m", ipaddr, port);

		return -1;
	}

	say_info("listenning on '%s:%d'", ipaddr, port);

	sock = fd;
	fd = -1;

	return sock;
}

static int telnet_client_force_raw(int fd)
{
	static const char iacs_to_send[] __attribute__((aligned(1))) = {
		IAC, DO, TELOPT_ECHO,
		IAC, DO, TELOPT_NAWS,
		IAC, DO, TELOPT_LFLOW,
		IAC, DO, TELOPT_EOR,
		IAC, WILL, TELOPT_ECHO,
		IAC, WILL, TELOPT_SGA
	};
	const char *snd_buf = iacs_to_send;
	char recv_buf[4096];
	ssize_t r;
	size_t l;

	l = sizeof(iacs_to_send);
	while (true) {
		r = write(fd, snd_buf, l);
		if (r <= 0) {
			say_error("can't write to client: %m");

			return -1;
		}
		if ((size_t)r == l)
			break;

		l -= r;
		snd_buf += r;
	}

	r = read(fd, recv_buf, sizeof(recv_buf));
	if (r <= 0) {
		say_error("can't read from client: %m");

		return -1;
	}

	return 0;
}

int stdio_wrap(const char *ifname, const char *ofname)
{
	int master __attribute__((cleanup(close_fd_))) = -1,
	    slave __attribute__((cleanup(close_fd_))) = -1,
	    ifd __attribute__((cleanup(close_fd_))) = -1,
	    ofd __attribute__((cleanup(close_fd_))) = -1,
	    listen_sock __attribute__((cleanup(close_fd_))) = -1,
	    sock = -1;
	fd_set rfd_set;
	struct termios termios;
	int nfds = 0;

	if (ifname &&
	    (ifd = mfifo(ifname, O_RDWR)) == -1)
		say_error("can't open `%s': %m", ifname);

	if (ofname &&
	    (ofd = open(ofname, O_WRONLY | O_CREAT | O_TRUNC)) == -1)
		say_error("can't open `%s': %m", ofname);

	if (config.debug_port != 0 &&
	    (listen_sock = listen_port(config.debug_ipaddr, config.debug_port)) == -1)
		say_error("can't create debug socket");

	if (pty_open(&master, &slave) != 0) {
		say_error("can't open pty: %m");

		return -1;
	}

	switch (fork()) {
	case -1:
		say_error("can't fork: %m");

		return -1;
	case 0:
		close(master);

		(void)login_tty(slave);

		(void)setvbuf(stdout, NULL, _IOLBF, 0);
		(void)setvbuf(stderr, NULL, _IONBF, 0);

		return 0;
	default:
		close(slave);

		if (isatty(STDIN_FILENO)) {
			(void)tcgetattr(STDIN_FILENO, &termios);
			cfmakeraw(&termios);
			(void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
		}

		break;
	}

	FD_ZERO(&rfd_set);
	FD_SET(master, &rfd_set);
	FD_SET(STDIN_FILENO, &rfd_set);
	if (ifd != -1)
		FD_SET(ifd, &rfd_set);
	if (listen_sock != -1)
		FD_SET(listen_sock, &rfd_set);
	nfds = MAX(master, MAX(ifd, listen_sock));

	while (true) {
		fd_set rfd_;
		int n;
		char buf[4096];
		int r;
		int i = -1,
		    o[3] = {-1, -1, -1};

		rfd_ = rfd_set;
		n = select(nfds + 1, &rfd_, NULL, NULL, NULL);
		if (n < 0 && errno != EINTR) {
			say_error("select failed: %m");

			_exit(EXIT_FAILURE);
		}
		for (; n > 0; n--) {
			if (listen_sock != -1 &&
			    FD_ISSET(listen_sock, &rfd_)) {
				if ((sock = accept(listen_sock, NULL, NULL)) == -1) {
					say_error("can't accept connection: %m");

					continue;
				}

				// Force client into `raw' mode
				if (telnet_client_force_raw(sock) != 0) {
					say_error("failed to force client into `raw' mode:"
						  " disconnecting");

					close(sock);

					continue;
				}

				FD_CLR(listen_sock, &rfd_set);
				FD_SET(sock, &rfd_set);
				nfds = MAX(nfds, sock);

				continue;
			} else if (sock != -1 &&
				   FD_ISSET(sock, &rfd_)) {
				i = sock;
				o[0] = master;
			} else if (FD_ISSET(STDIN_FILENO, &rfd_)) {
				i = STDIN_FILENO;
				o[0] = master;
			} else if (ifd != -1 &&
				   FD_ISSET(ifd, &rfd_)) {
				i = ifd;
				o[0] = master;
			} else if (FD_ISSET(master, &rfd_)) {
				i = master;
				o[0] = sock;
				o[1] = STDOUT_FILENO;
				o[2] = ofd;
			} else
				// not reacheable
				assert(0);

			FD_CLR(i, &rfd_);

			if ((r = read(i, buf, sizeof(buf))) <= 0) {
				if (r < 0)
					say_error("couldn't read from %d: %m", i);

				if (i == master)
					_exit(EXIT_FAILURE);

				FD_CLR(i, &rfd_set);

				close(i);

				if (i == sock) {
					sock = -1;
					FD_SET(listen_sock, &rfd_set);
				}

				continue;
			}
			if (buf[r - 1] == '\0')
				// telnet client sent <CR><NUL>
				r -= 1;

			for (unsigned j = 0; j < 3; j++) {
				if (o[j] != -1 &&
				    write(o[j], buf, r) == -1 && errno != EBADF) {
					say_error("couldn't write to %d: %m", o[j]);

					if (o[j] == master)
						_exit(EXIT_FAILURE);

					close(o[j]);

					if (o[j] == sock) {
						sock = -1;
						FD_SET(listen_sock, &rfd_set);
					}
				}
			}
		}
	}

	_exit(EXIT_SUCCESS);
}
