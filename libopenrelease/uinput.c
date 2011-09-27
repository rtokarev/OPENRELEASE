/*
 * Copyright (c) 2011 Eugene Vorobev <jenya.vv@gmail.com>
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

#include <uinput.h>
#include <log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

static int fd;
 
int uinput_init(void) 
{
	struct uinput_user_dev uidev;
	int i;

	fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);

	if(fd < 0) {
		say_error("uinput open error: %s", strerror(errno));
		return(-1);
	}

	if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
		say_error("uinput ioctl error: %s", strerror(errno));
		return(-1);
	}

	for (i=0; i<0xff; i++) {
		if(ioctl(fd, UI_SET_KEYBIT, i) < 0) {
			say_error("uinput ioctl UI_SET_KEYBIT %d error: %s", i, strerror(errno));
			return(-1);
		}
	}

	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "OPENRELEASE remote");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;

	if(write(fd, &uidev, sizeof(uidev)) < 0) {
		say_error("uinput write uidev error: %s", strerror(errno));
		return(-1);
	}

	if(ioctl(fd, UI_DEV_CREATE) < 0) {
		say_error("uinput ioctl UI_DEV_CREATE error: %s", strerror(errno));
		return(-1);
	}

	return 1;
}

int uinput_send(unsigned key) 
{
	struct input_event ev;

	memset(&ev, 0, sizeof(ev));
	ev.type = EV_KEY;
	ev.code = key;
	ev.value = 1; // SDL_PRESSED

	say_debug ("uinput send key pressed: %d", key);
	if(write(fd, &ev, sizeof(struct input_event)) < 0) {
		say_error("uinput write key %d pressed error: %s", key, strerror(errno));
		return(-1);
	}

	memset(&ev, 0, sizeof(ev));
	ev.type = EV_KEY;
	ev.code = key;
	ev.value = 0; // SDL_RELEASED

	say_debug ("uinput send key released: %d", key);
	if(write(fd, &ev, sizeof(struct input_event)) < 0) {
	say_error("uinput write key %d released error: %s", key, strerror(errno));
		return(-1);
	}

	return 1;
}
