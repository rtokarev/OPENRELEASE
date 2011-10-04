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

#ifndef _SDL_LGDTV_video_h
#define _SDL_LGDTV_video_h

#include <libopenrelease.h>

#include "../SDL_sysvideo.h"

#define LGDTV_INPUT_DEVICE  "/dev/input/event"

#define LGDTV_MAX_INPUTS    32

#define LGDTV_WIDTH			1368
#define LGDTV_HEIGHT		768
#define LGDTV_BPP			16
#define LGDTV_AMASK			0x8000		// 1000 0000 0000 0000
#define LGDTV_RMASK			0x7C00		// 0111 1100 0000 0000
#define LGDTV_GMASK			0x03E0		// 0000 0011 1110 0000
#define LGDTV_BMASK			0x001F		// 0000 0000 0001 1111


/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this


/* Private display data */

struct SDL_PrivateVideoData {
    int w, h;
    int bpp;
    void *buffer;
    int input_fd[LGDTV_MAX_INPUTS];
    GFXOSD_VOSD_ID_T vosdId;
};

/* Old variable names */
#define input_fd            (this->hidden->input_fd)

#endif /* _SDL_nullvideo_h */
