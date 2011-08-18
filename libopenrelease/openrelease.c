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

#include <config.h>
#include <debug.h>
#include <key_action.h>
#include <libc_wrap.h>
#include <log.h>
#include <mem_patch.h>
#include <symfile.h>
#include <version.h>
#include <wrap.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int (* __real_main)(int, char **, char **) = NULL;


int daemonize(int nochdir, int noclose);


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

	setvbuf(stdout, (char *)NULL, _IOLBF, 0);
	setvbuf(stderr, (char *)NULL, _IONBF, 0);

	return 0;
}

void usage(void)
{
	printf("Usage: OPENRELEASE [OPENRELEASE OPTIONS] -- [RELEASE OPTIONS]\n"
	       "\n"
	       "[OPENRELEASE OPTIONS]:\n"
	       "  -c FILE               config file\n"
	       "  -d                    become daemon\n"
	       "  -h                    print this message and exit\n"
	       "  -l FILE               log file\n"
	       "  -v                    increments the current Verbose level by one.\n"
	       "                        Multiple -v options are allowed\n"
	       "  -V                    print program version and exit\n"
	      );
}

int __wrap_main(int argc, char *argv[], char *envp[])
{
	int c;
	char *config_file = NULL;
	char *log_file = NULL;
	bool daemon = false;

	while ((c = getopt(argc, argv, "c:dhl:vV")) != -1) {
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
			++verbose;

			break;
		case 'V':
			printf("%s\n", openrelease_version());

			_exit(EXIT_SUCCESS);
		case '?':
			usage();

			_exit(EXIT_FAILURE);
		default:
			say_error("unmatched option: -%c", c);

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

	mmaps_init();

	symfile_load(config.symfile);
	wrap_init();

	debug_init();
	key_action_init();

	say_info("dive into RELEASE");

	unsetenv("LD_PRELOAD");

	argc = 1;
	if (optind != argc) {
		memmove(&argv[1], &argv[optind], (optind - argc) * sizeof(argv[0]));
		argc += optind - argc;
	}
	argv[argc] = NULL;

	__real_main(argc, argv, envp);

	_exit(EXIT_SUCCESS);

	return 0;
}
