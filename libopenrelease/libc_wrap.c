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

#include <libc_wrap.h>

#include <config.h>
#include <log.h>

#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#if PLATFORM == SATURN6 || PLATFORM == BCM

void __uClibc_main(int (*main)(int, char **, char **), int argc, char **argv,
		   void (*app_init)(void), void (*app_fini)(void),
		   void (*rtld_fini)(void), void *stack_end)
{
	void (* __real___uClibc_main)(int (*main)(int, char **, char **), int argc, char **argv,
				      void (*app_init)(void), void (*app_fini)(void),
				      void (*rtld_fini)(void), void *stack_end);

	__real___uClibc_main = dlsym(RTLD_NEXT, "__uClibc_main");
	if (__real___uClibc_main == NULL) {
		fprintf(stderr, "symbol lookup error: undefined symbol: __uClibc_main");

		_exit(EXIT_FAILURE);
	}

	__real_main = main;

	__real___uClibc_main(__wrap_main, argc, argv,
			     app_init, app_fini,
			     rtld_fini, stack_end);
}

#elif PLATFORM == SATURN7

int __libc_start_main(int (* main)(int, char **, char **), int argc, char **ubp_av,
		      void (* init)(void), void (* fini)(void),
		      void (* rtld_fini)(void), void *stack_end)
{
	int (* __real___libc_start_main)(int (* main)(int, char **, char **), int argc, char **ubp_av,
					 void (* init)(void), void (* fini)(void),
					 void (* rtld_fini)(void), void *stack_end);

	__real___libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
	if (__real___libc_start_main == NULL) {
		fprintf(stderr, "symbol lookup error: undefined symbol: __libc_start_main");

		_exit(EXIT_FAILURE);
	}

	__real_main = main;

	return __real___libc_start_main(__wrap_main, argc, ubp_av,
					init, fini,
					rtld_fini, stack_end);
}

#endif

int mount(const char *source, const char *target, const char *filesystemtype,
	  unsigned long mountflags, const void *data)
{
	static int (* real_mount)(const char *, const char *, const char *,
				    unsigned long, const void *) = NULL;

	say_debug("mount(%s, %s, %s, %lu, 0x%x)", source, target, filesystemtype, mountflags, data);

	if (!config.enable_auto_mount) {
		if (strcmp(target, "/mnt/usb1/Drive1") != 0)
			return -1;

		return 0;
	}

	if (real_mount == NULL)
		real_mount = dlsym(RTLD_NEXT, "mount");
	if (real_mount == NULL) {
		say_error("symbol lookup error: undefined symbol: mount");

		return -1;
	}

	say_debug("real_mount = %p", real_mount);

	return real_mount(source, target, filesystemtype, mountflags, data);
}

