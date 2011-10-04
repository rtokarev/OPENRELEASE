/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include "SDL_config.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"
#include "SDL_LGDTV_video.h"
#include "SDL_LGDTV_events_c.h"
#include "SDL_LGDTV_keys.h"


static SDLKey keymap[KEY_MAX] = { [0 ... KEY_MAX-1] = SDLK_UNKNOWN };


static int LGDTV_HandleInput(_THIS, int fd)
{
	printf ("SDL: LGDTV_HandleInput start\n");
	int i, nread;
	struct input_event event[64];
	SDL_keysym keysym;

	nread = read(fd, event, sizeof(struct input_event) * 64);

	for ( i = 0; i < nread / sizeof(struct input_event); i++ ) {
		if ( (event[i].type == EV_KEY) && (event[i].value < 2) ) {
/*
			switch ( event[i].code ) {

				case BTN_LEFT:
				case BTN_MIDDLE:
				case BTN_RIGHT:
					SDL_PrivateMouseButton(event[i].value ? SDL_PRESSED : SDL_RELEASED,
							keymap[event[i].code], 0, 0);
					break;
					
				default:
*/
					keysym.scancode = event[i].code;
					keysym.sym = keymap[event[i].code];
					keysym.mod = KMOD_NONE;
					SDL_PrivateKeyboard(event[i].value ? SDL_PRESSED : SDL_RELEASED,
							&keysym);
/*
					break;
			}
*/			
		} else if ( event[i].type == EV_REL ) {
			SDL_PrivateMouseMotion(0, 1,
					event[i].code ? 0 : event[i].value,
					event[i].code ? event[i].value : 0);
		}
	}

	printf ("SDL: LGDTV_HandleInput end\n");
	return(i > 0);
}


void LGDTV_OpenInputDevices(_THIS)
{	
	int i, cap;
	char device[FILENAME_MAX];

	for ( i = 0; i < LGDTV_MAX_INPUTS; i++ ) {
		sprintf(device, "%s%i", LGDTV_INPUT_DEVICE, i);
		if ( (input_fd[i] = open(device, O_RDONLY)) >= 0 ) {
			if ( (ioctl(input_fd[i], EVIOCGRAB, 1) < 0) ||			/* Failed to grab the input device*/
			     (ioctl(input_fd[i], EVIOCGBIT(0, EV_MAX), &cap) < 0) ||	/* Failed to get the device capabilities */
			    !(((cap >> EV_KEY) & 1) || ((cap >> EV_REL) & 1)) ||	/* Does not have EV_KEY nor EV_REL event types */
			     ((cap >> EV_ABS) & 1) ) {					/* It has EV_ABS event type (Joystick) */
				ioctl(input_fd[i], EVIOCGRAB, 0);
				close(input_fd[i]);
				input_fd[i] = -1;
			}
		}
	}
}

void LGDTV_CloseInputDevices(_THIS)
{
	printf("SDL: LGDTV_CloseInputDevices begin\n");
	int i;

	for ( i = 0; i < LGDTV_MAX_INPUTS; i++ ) {
		printf("SDL: LGDTV_CloseInputDevices: input_fd[%d]=%d\n", i, input_fd[i]);
		if ( input_fd[i] >= 0 ) {
			printf("SDL: LGDTV_CloseInputDevices: ioctl\n");
			ioctl(input_fd[i], EVIOCGRAB, 0);
			printf("SDL: LGDTV_CloseInputDevices: close\n");
			
			close(input_fd[i]);
			input_fd[i] = -1;
		}
	}
	printf("SDL: LGDTV_CloseInputDevices end\n");
}

void LGDTV_InitOSKeymap(_THIS)
{
	keymap[K_ENERGY_SAVING] = SDLK_ENERGY_SAVING;
	keymap[K_POWER] = SDLK_POWER;
	keymap[K_INPUT] = SDLK_INPUT;
	keymap[K_RATIO] = SDLK_RATIO;
	keymap[K_TV_RAD] = SDLK_TV_RAD;
	keymap[K_Q_MENU] = SDLK_Q_MENU;
	keymap[K_MENU] = SDLK_MENU;
	keymap[K_GUIDE] = SDLK_GUIDE;
	keymap[K_UP] = SDLK_UP;
	keymap[K_DOWN] = SDLK_DOWN;
	keymap[K_LEFT] = SDLK_LEFT;
	keymap[K_RIGHT] = SDLK_RIGHT;
	keymap[K_OK] = SDLK_OK;
	keymap[K_RETURN_EXIT] = SDLK_RETURN_EXIT;
	keymap[K_INFO] = SDLK_INFO;
	keymap[K_AV_MODE] = SDLK_AV_MODE;
	keymap[K_VOL_UP] = SDLK_VOL_UP;
	keymap[K_VOL_DOWN] = SDLK_VOL_DOWN;
	keymap[K_FAV_MARK] = SDLK_FAV_MARK;
	keymap[K_MUTE] = SDLK_MUTE;
	keymap[K_PAGE_UP] = SDLK_PAGEUP;
	keymap[K_PAGE_DOWN] = SDLK_PAGEDOWN;
	keymap[K_0] = SDLK_0;
	keymap[K_1] = SDLK_1;
	keymap[K_2] = SDLK_2;
	keymap[K_3] = SDLK_3;
	keymap[K_4] = SDLK_4;
	keymap[K_5] = SDLK_5;
	keymap[K_6] = SDLK_6;
	keymap[K_7] = SDLK_7;
	keymap[K_8] = SDLK_8;
	keymap[K_9] = SDLK_9;
	keymap[K_LIST] = SDLK_LIST;
	keymap[K_Q_VIEW] = SDLK_Q_VIEW;
	keymap[K_RED_KEY] = SDLK_RED_KEY;
	keymap[K_GREEN_KEY] = SDLK_GREEN_KEY;
	keymap[K_YELLOW_KEY] = SDLK_YELLOW_KEY;
	keymap[K_BLUE_KEY] = SDLK_BLUE_KEY;
	keymap[K_TEXT] = SDLK_TEXT;
	keymap[K_T_OPT] = SDLK_T_OPT;
	keymap[K_SUBTITLE] = SDLK_SUBTITLE;
	keymap[K_SIMPLINK] = SDLK_SIMPLINK;
	keymap[K_FREEZE] = SDLK_FREEZE;
	keymap[K_NETCAST] = SDLK_NETCAST;
	keymap[K_EXIT] = SDLK_EXIT;
	keymap[K_STOP] = SDLK_STOP;
	keymap[K_PLAY] = SDLK_PLAY;
	keymap[K_PAUSE] = SDLK_PAUSE;
	keymap[K_FORWARD] = SDLK_FORWARD;
	keymap[K_BACKWARD] = SDLK_BACKWARD;
	keymap[K_AD] = SDLK_AD;
	keymap[K_TILT] = SDLK_TILT;
	keymap[K_IN_STOP] = SDLK_IN_STOP;
	keymap[K_IN_START] = SDLK_IN_START;
	keymap[K_P_CHECK] = SDLK_P_CHECK;
	keymap[K_S_CHECK] = SDLK_S_CHECK;
	keymap[K_POWERONLY] = SDLK_POWERONLY;
	keymap[K_EZ_ADJUST] = SDLK_EZ_ADJUST;
}

void LGDTV_PumpEvents(_THIS)
{
	fd_set fdset;
	int i, max_fd, posted;
	static struct timeval zero = { 0, 0 };

	do {
		FD_ZERO(&fdset);
		max_fd = 0;
		for ( i = 0; i < LGDTV_MAX_INPUTS; i++ ) {
			if (input_fd[i] >= 0) {
				FD_SET(input_fd[i], &fdset);
				if ( max_fd < input_fd[i] ) {
					max_fd = input_fd[i];
				}
			}
		}
		posted = 0;
		if ( select(max_fd+1, &fdset, NULL, NULL, &zero) > 0 ) {
			for ( i = 0; i < LGDTV_MAX_INPUTS; i++ ) {
				if ( input_fd[i] >= 0 ) {
					if ( FD_ISSET(input_fd[i], &fdset) ) {
						posted += LGDTV_HandleInput(this, input_fd[i]);
					}
				}
			}
		}
	} while ( posted );
}
