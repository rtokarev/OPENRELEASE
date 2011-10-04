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

/* 
  Written by Eugene Vorobev <jenya.vv@gmail.com>
  LGDTV OSD video driver implementation.
  1368x768x16 resolution.
  Hardware acceleration not implemented yet.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
 
 
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_LGDTV_video.h"
#include "SDL_LGDTV_events_c.h"

#include "SDL_LGDTV_video.h"
#include "SDL_LGDTV_events_c.h"
#include "SDL_LGDTV_mouse_c.h"

#include "libopenrelease.h"
#include "RELEASE.h"

// POSD0_ADR
#define FB_ADDR 0x0AC0A000UL
// 1368x768x2
#define FB_MMAP_SIZE 0x201000UL

static void *VFB;
unsigned int fb_addr = FB_ADDR;
int fd;

//extern char *GetFrameBufferAddress(void);
//extern void VideoInit(int, int, int);
//extern void VideoClose(void);
//extern void UpdateSurface(char *, int, int);
//extern int y_SetHWAlpha(SDL_Surface *, Uint8);
//extern int y_SetHWColorKey(SDL_Surface *, Uint32);
//extern int y_HWSWAccelBlit(SDL_Surface *, SDL_Rect *, SDL_Surface *, SDL_Rect *);
//extern int y_HWAccelBlit(SDL_Surface *, SDL_Rect *, SDL_Surface *, SDL_Rect *);
//extern int y_HWFillRect(SDL_Surface *, SDL_Rect *, Uint32);
//extern int y_AllocHWSurface(SDL_Surface *);
//extern void y_FreeHWSurface(SDL_Surface *);

static int LGDTV_Available(void);
static SDL_VideoDevice *LGDTV_CreateDevice(int devindex);

/* Initialization/Query functions */
static int LGDTV_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **LGDTV_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *LGDTV_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int LGDTV_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void LGDTV_VideoQuit(_THIS);

/* Hardware surface functions */
static int LGDTV_AllocHWSurface(_THIS, SDL_Surface *surface);
static int LGDTV_LockHWSurface(_THIS, SDL_Surface *surface);
static void LGDTV_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void LGDTV_FreeHWSurface(_THIS, SDL_Surface *surface);

/* etc. */
static void LGDTV_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

static int LGDTV_HWFillRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color);
static int LGDTV_CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst);
static int LGDTV_HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
static int LGDTV_HWSWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
static int LGDTV_SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key);
static int LGDTV_SetHWAlpha(_THIS, SDL_Surface *surface, Uint8 value);

/* LGDTV driver bootstrap functions */

VideoBootStrap LGDTV_bootstrap = {
	"lgdtv", "LGDTV OSD Framebuffer",
	LGDTV_Available, LGDTV_CreateDevice
};


static int LGDTV_Available(void)
{
	return 1;
}

static void LGDTV_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *LGDTV_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->VideoInit = LGDTV_VideoInit;
	device->ListModes = LGDTV_ListModes;
	device->SetVideoMode = LGDTV_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = LGDTV_SetColors;
	device->UpdateRects = LGDTV_UpdateRects;
	device->VideoQuit = LGDTV_VideoQuit;
//	device->AllocHWSurface = LGDTV_AllocHWSurface;
	device->AllocHWSurface = NULL;
//	device->CheckHWBlit = LGDTV_CheckHWBlit;
	device->CheckHWBlit = NULL;
//	device->FillHWRect = LGDTV_HWFillRect;
	device->FillHWRect = NULL;
//	device->SetHWColorKey = LGDTV_SetHWColorKey;
	device->SetHWColorKey = NULL;
//	device->SetHWAlpha = LGDTV_SetHWAlpha;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = LGDTV_LockHWSurface;
	device->UnlockHWSurface = LGDTV_UnlockHWSurface;
	device->FlipHWSurface = NULL;
//	device->FreeHWSurface = LGDTV_FreeHWSurface;
	device->FreeHWSurface = NULL;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = LGDTV_InitOSKeymap;
//	device->InitOSKeymap = NULL;
	device->PumpEvents = LGDTV_PumpEvents;
//	device->PumpEvents = NULL;

	device->free = LGDTV_DeleteDevice;

	return device;
}


int LGDTV_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	/* ARGB1555 */
	vformat->Amask = LGDTV_AMASK;
	vformat->Rmask = LGDTV_RMASK;
	vformat->Gmask = LGDTV_GMASK;
	vformat->Bmask = LGDTV_BMASK;

	vformat->BitsPerPixel = LGDTV_BPP;
	vformat->BytesPerPixel = LGDTV_BPP/8;
	vformat->alpha = 2;

	this->info.wm_available = 0;
	this->info.hw_available = 0;
	this->info.blit_hw      = 0;
	this->info.blit_hw_CC   = 0;
	this->info.blit_hw_A    = 0;
	this->info.blit_sw      = 1;
	this->info.blit_sw_CC   = 1;
	this->info.blit_sw_A    = 1;
	this->info.blit_fill    = 1;
	this->info.current_w    = LGDTV_WIDTH;
	this->info.current_h    = LGDTV_HEIGHT;

	LGDTV_OpenInputDevices(this);
	
	/* We're done! */
	return(0);
}

SDL_Rect **LGDTV_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	static const SDL_Rect modes[] = {
		{  0, 0, LGDTV_WIDTH, LGDTV_HEIGHT },
	};
	static const SDL_Rect *modelist[] = {
		&modes[0],
		NULL
	};

	switch (format->BitsPerPixel) {
		case LGDTV_BPP:
			return((SDL_Rect **)modelist);
		default:
			return(NULL);
	}

}

void print_GFXOSD_VOSD_INFO_T(GFXOSD_VOSD_INFO_T * info)
{
	printf("GFXOSD_VOSD_INFO_T.bEnable: %d\n", info->bEnable);
	printf("GFXOSD_VOSD_INFO_T.osdId: %d\n", info->osdId);
	printf("GFXOSD_VOSD_INFO_T.viewX: %d\n", info->viewX);
	printf("GFXOSD_VOSD_INFO_T.viewY: %d\n", info->viewY);
	printf("GFXOSD_VOSD_INFO_T.viewWidth: %d\n", info->viewWidth);
	printf("GFXOSD_VOSD_INFO_T.viewHeight: %d\n", info->viewWidth);
	printf("GFXOSD_VOSD_INFO_T.dispX: %d\n", info->dispX);
	printf("GFXOSD_VOSD_INFO_T.dispY: %d\n", info->dispY);
	printf("GFXOSD_VOSD_INFO_T.dispWidth: %d\n", info->dispWidth);
	printf("GFXOSD_VOSD_INFO_T.dispHeight: %d\n", info->dispHeight);
	printf("GFXOSD_VOSD_INFO_T.format: %d\n", info->format);
	printf("GFXOSD_VOSD_INFO_T.pxlDepth: %d\n", info->pxlDepth);
	printf("GFXOSD_VOSD_INFO_T.width: %d\n", info->width);
	printf("GFXOSD_VOSD_INFO_T.height: %d\n", info->height);
	printf("GFXOSD_VOSD_INFO_T.stride: %d\n", info->stride);
	printf("GFXOSD_VOSD_INFO_T.alphaValue: %d\n", info->alphaValue);
	printf("GFXOSD_VOSD_INFO_T.pAddr: 0x%x\n", info->pAddr);
}

SDL_Surface *LGDTV_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	printf("LGDTV_SetVideoMode: begin\n");
	if (width != LGDTV_WIDTH || height != LGDTV_HEIGHT || LGDTV_BPP != 16) {
		printf("LGDTV_SetVideoMode: Only 1366x768x16 mode supported\n");
		SDL_SetError("Only 1366x768x16 mode supported");
		return(NULL);
	}
	
	GFXOSD_VOSD_INFO_T GfxVOsdInfo;
	GFXOSD_DEF_T _gGFXOSDDef;

	this->hidden->vosdId = GFXOSD_VOSD_0;

	printf("LGDTV_SetVideoMode: DDI_GFXOSD_GetVirtualOSDInfo[GFXOSD_VOSD_0]:\n");

//	DTV_STATUS_T DDI_GFXOSD_GetVirtualOSDInfo(GFXOSD_VOSD_ID_T vosdId, GFXOSD_VOSD_INFO_T * pGfxVosdInfo)
	CALL (DTV_STATUS_T, DDI_GFXOSD_GetVirtualOSDInfo, GFXOSD_VOSD_ID_T, GFXOSD_VOSD_INFO_T *)(this->hidden->vosdId, &GfxVOsdInfo);
	print_GFXOSD_VOSD_INFO_T(&GfxVOsdInfo);

	VFB = GfxVOsdInfo.pAddr;
	
	printf("LGDTV_SetVideoMode: pre set buffer\n");
	this->hidden->buffer = VFB;
	if ( ! this->hidden->buffer) {
		printf("LGDTV_SetVideoMode: ! this->hidden->buffer\n");
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(0);
	}
	printf("LGDTV_SetVideoMode: pre memset\n");
	SDL_memset(this->hidden->buffer, 0, LGDTV_WIDTH * LGDTV_HEIGHT * (LGDTV_BPP / 8));

	printf("LGDTV_SetVideoMode: /* Allocate the new pixel format for the screen */\n");

	if ( ! SDL_ReallocFormat(current, LGDTV_BPP, LGDTV_RMASK, LGDTV_GMASK, LGDTV_BMASK, LGDTV_AMASK) ) {
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	printf("LGDTV_SetVideoMode: /* Set up the new mode framebuffer */\n");
	current->flags = flags | SDL_FULLSCREEN;
	this->hidden->w = current->w = LGDTV_WIDTH;
	this->hidden->h = current->h = LGDTV_HEIGHT;
	this->hidden->bpp = LGDTV_BPP;
	current->pitch = current->w * (LGDTV_BPP / 8);
	current->pixels = this->hidden->buffer;
	this->UpdateRects = LGDTV_UpdateRects;

	printf("LGDTV_SetVideoMode: /* We're done */\n");
	return(current);
}

/* We don't actually allow hardware surfaces other than the main one */
static int LGDTV_AllocHWSurface(_THIS, SDL_Surface *surface)
{
//	return y_AllocHWSurface(surface);
}

static void LGDTV_FreeHWSurface(_THIS, SDL_Surface *surface)
{
//	y_FreeHWSurface(surface);
}

/* We need to wait for vertical retrace on page flipped displays */
static int LGDTV_LockHWSurface(_THIS, SDL_Surface *surface)
{
//	void DDI_GFXOSD_Lock(void)
	printf("SDL: DDI_GFXOSD_Lock begin\n");
	CALL (void, DDI_GFXOSD_Lock, void)();
	printf("SDL: DDI_GFXOSD_Lock end\n");
	return(0);
}

static void LGDTV_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
//	void DDI_GFXOSD_UnLock(void)
	printf("SDL: DDI_GFXOSD_UnLock begin\n");
	CALL (void, DDI_GFXOSD_UnLock, void)();
	printf("SDL: DDI_GFXOSD_UnLock end\n");
	return;
}

static void LGDTV_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	DTV_STATUS_T ret;
	__BOOLEAN bImmediate = 1;
    int i;
	for (i = 0; i < numrects; i++) {
//	DTV_STATUS_T DDI_GFXOSD_UpdateScreen(GFXOSD_VOSD_ID_T vosdId, __UINT32 x, __UINT32 y, __UINT32 width, __UINT32 height, __BOOLEAN bImmediate)
//		printf ("SDL DRIVER: LGDTV_UpdateRects begin (x: %d, y: %d, h: %d, w: %d, i: %d)\n", rects[i].x, rects[i].y, rects[i].w, rects[i].h, bImmediate);
		CALL (DTV_STATUS_T, DDI_GFXOSD_UpdateScreen, GFXOSD_VOSD_ID_T, __UINT32, __UINT32, __UINT32, __UINT32, __BOOLEAN)(this->hidden->vosdId, rects[i].x, rects[i].y, rects[i].w, rects[i].h, bImmediate);
//		printf ("SDL DRIVER: LGDTV_UpdateRects end\n");
		if (ret != API_OK)
				printf ("SDL DRIVER: LGDTV_UpdateRects: DDI_GFXOSD_UpdateScreen error: %d (x: %d, y: %d, h: %d, w: %d, i: %d)\n)", ret, rects[i].x, rects[i].y, rects[i].w, rects[i].h, bImmediate);
	}
}

int LGDTV_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	/* do nothing of note. */
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/

//void LGDTV_CloseEventDevices(void);

void LGDTV_VideoQuit(_THIS)
{
	SDL_memset(this->hidden->buffer, 0, this->hidden->w * this->hidden->h * (this->hidden->bpp / 8));
    
	//munmap(VFB, FB_MMAP_SIZE);
    close(fd);	
	
	this->screen->pixels = NULL;
	this->hidden->buffer = NULL;
	//LGDTV_CloseInputDevices();
}

int LGDTV_HWFillRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color)
{
//	return y_HWFillRect(dst, rect, color);
}

int LGDTV_CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst)
{
	if (src->format->BitsPerPixel != this->hidden->bpp || dst->format->BitsPerPixel != this->hidden->bpp)
		return(-1);

	if ((src->flags & SDL_HWSURFACE) != SDL_HWSURFACE && (dst->flags & SDL_HWSURFACE) != SDL_HWSURFACE)
	{
		return(-1);
	}

	src->flags |= SDL_HWACCEL;

	if ((src->flags & SDL_SRCCOLORKEY ) == SDL_SRCCOLORKEY)
	{
		src->flags &= ~SDL_HWACCEL;
	}

	if ((src->flags & SDL_SRCALPHA) == SDL_SRCALPHA)
	{

	}

	if ((src->flags & SDL_HWSURFACE) == SDL_HWSURFACE && (dst->flags & SDL_HWSURFACE) == SDL_HWSURFACE)
	{
		src->map->hw_blit = LGDTV_HWAccelBlit;
		return 1;
	}

	else if ((src->flags & SDL_SWSURFACE) == SDL_SWSURFACE && (dst->flags & SDL_HWSURFACE) == SDL_HWSURFACE)
	{
		src->map->hw_blit = LGDTV_HWSWAccelBlit;
		return 1;
	}

	return (-1);
}

int LGDTV_HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
//	return y_HWAccelBlit(src, srcrect, dst, dstrect);
}

int LGDTV_HWSWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
//	return y_HWSWAccelBlit(src, srcrect, dst, dstrect);
}

int LGDTV_SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key)
{
//	return y_SetHWColorKey(surface, key);
}

int LGDTV_SetHWAlpha(_THIS, SDL_Surface *surface, Uint8 value)
{
//	return y_SetHWAlpha(surface, value);
}
