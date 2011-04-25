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

#include <log.h>

#include <config.h>
#include <ir.h>

#include <fcntl.h>
#include <features.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


#ifndef O_LARGEFILE
# define O_LARGEFILE 0x2000
#endif


int __libc_open(const char *pathname, int flags, ...);
int __libc_close(int fd);
int __libc_read(int fd, void *buf, size_t count);

int __real_mount(const char *source, const char *target, const char *filesystemtype,
		 unsigned long mountflags, const void *data);


static int ir_fd = -1;


int open(const char *pathname, int flags, ...)
{
	va_list ap;
	mode_t mode = 0;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	int fd = __libc_open(pathname, flags, mode);

	if (strcmp(pathname, "/dev/ir") == 0)
		ir_fd = fd;

	return fd;
}

int open64(const char *pathname, int flags, ...)
{
	va_list ap;
	mode_t mode = 0;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	return open(pathname, flags, mode);
}

int close(int fd)
{
	if (fd == ir_fd)
		ir_fd = -1;

	return __libc_close(fd);
}

int read(int fd, void *buf, size_t count)
{
	int r = __libc_read(fd, buf, count);

	if (fd == ir_fd)
		return tap_ir_keypress(r, buf);

	return r;
}

int __wrap_mount(const char *source, const char *target, const char *filesystemtype,
		 unsigned long mountflags, const void *data)
{
	say_debug("mount(%s, %s, %s, %lu, 0x%x)", source, target, filesystemtype, mountflags, data);

	if (!config.enable_auto_mount) {
		if (strcmp(target, "/mnt/usb1/Drive1") != 0)
			return -1;

		return 0;
	}

	return __real_mount(source, target, filesystemtype, mountflags, data);
}

