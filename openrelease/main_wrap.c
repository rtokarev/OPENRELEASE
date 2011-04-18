/*
 * Copyright (c) 2010 Roman Tokarev <roman.s.tokarev@gmail.com>
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

#include <config.h>
#include <debug.h>
#include <ir.h>
#include <log.h>
#include <version.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int daemonize(int nochdir, int noclose);

int __real_main(int argc, char *argv[]);


static int mfifo(const char *name, int mode)
{
	if (mkfifo(name, S_IFIFO|S_IRWXU) == -1 && errno != EEXIST)
		return -1;

	return open(name, mode);
}

static void _close_fd(int *fd)
{
	if (*fd != -1)
		(void)close(*fd);
}

static int stdio_init(const char *ifname, const char *ofname)
{
	int ifd __attribute__((cleanup(_close_fd))) = -1,
	    ofd __attribute__((cleanup(_close_fd))) = -1;

	if (ifname &&
	    (ifd = mfifo(ifname, O_RDWR)) == -1) {
		say_error("can't open `%s': %m", ifname);

		return -1;
	}

	if (ofname &&
	    (ofd = open(ofname, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
		say_error("can't open `%s': %m", ofname);

		return -1;
	}

	if (ifd != -1 &&
	    dup2(ifd, STDIN_FILENO) == -1)
		return -1;

	if (ofd != -1 &&
	    (dup2(ofd, STDOUT_FILENO) == -1 ||
	     dup2(ofd, STDERR_FILENO) == -1))
		return -1;

	return 0;
}

void usage(void)
{
	printf("Usage: OPENRELEASE [OPTIONS]\n"
	       "  -c FILE               config file\n"
	       "  -d                    become daemon\n"
	       "  -h                    print this message and exit\n"
	       "  -l FILE               log file\n"
	       "  -v                    print program version and exit\n"
	      );
}

int __wrap_main(int argc, char *argv[])
{
	int c;
	char *config_file = NULL;
	char *log_file = NULL;
	bool daemon = false;

	while ((c = getopt(argc, argv, "c:dhl:v")) != -1) {
		switch (c) {
		case 'c':
			config_file = strdup(optarg);

			break;
		case 'd':
			daemon = true;

			break;
		case 'h':
			usage();

			_exit(EXIT_SUCCESS);
		case 'l':
			log_file = strdup(optarg);

			break;
		case 'v':
			printf("%s\n", openrelease_version());

			_exit(EXIT_SUCCESS);
		default:
			usage();

			_exit(EXIT_FAILURE);
		}
	}

	create_log(log_file);

	if (daemon && getppid() != 1) {
		if (daemonize(1, 0) == -1) {
			say_error("can't daemonize");

			_exit(EXIT_FAILURE);
		}

		execvp(argv[0], argv);
	}

	config_init(config_file);

	if (stdio_init(config.input, config.output) == -1) {
		say_error("stdio_init failed");

		_exit(EXIT_FAILURE);
	}

	debug_init();
	tap_ir_init();

	say("dive into RELEASE");

	__real_main(argc, argv);

	_exit(EXIT_SUCCESS);

	return 0;
}
