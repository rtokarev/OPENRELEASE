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

#include <key_action.h>

#include <config.h>
#include <debug.h>
#include <keys.h>
#include <log.h>
#include <parse_config.h>
#include <util.h>
#include <wrap.h>

#include <SDL.h>
#include <SDL_getenv.h>

#include <RELEASE.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define ACTION_KEY_CODE_PREFIX "K_X"

#define SCREEN_MUTE_ON		"kd 00 01\n"
#define SCREEN_MUTE_OFF		"kd 00 00\n"
#define VOLUME_MUTE_ON		"ke 00 00\n"
#define VOLUME_MUTE_OFF		"ke 00 01\n"
#define OSD_SELECT_ON		"kl 00 01\n"
#define OSD_SELECT_OFF		"kl 00 00\n"
#define CONTROL_LOCK_ON		"km 00 01\n"
#define CONTROL_LOCK_OFF	"km 00 00\n"

#define KEY_NAME_TO_ACTION_NAME(n, k, _, args...) _(n, ##args)
#define KEY_ACTION_DEFAULT(n, k)			\
	struct key_action n##_action = {		\
		.action_name = #n,			\
		.key_name = #n,				\
		.key = k,				\
		.handler = key_action_default_handler	\
	};
#define KEY_ACTION_INIT(n, k)					\
	n##_action.handler = key_action_find_handler(keymap.n);	\
	n##_action.action_name = keymap.n;
#define KEY_ACTION_DO(n, k)					\
	case k:							\
		return n##_action.handler(&n##_action);

#define KEY_ACTION_KEY_PRESS_HANDLER(n, k)						\
	static void key_action_##n##_handler(struct key_action *a) {			\
		say_debug("%s (0x%x) -> %s", a->key_name, a->key, a->action_name);	\
											\
		return __real__MICOM_ProcessSingleKey(k, 0);				\
	}
#define KEY_ACTION_FIND_HANDLER(n, s)	\
	if (strcmp(#n, s) == 0)		\
		return key_action_##n##_handler;
#define KEY_ACTION_HANDLER_DECL(n) \
	static void key_action_##n##_handler(struct key_action *a);

#define KEY_PRESS_ACTION_NAMES(keys, _, args...) keys(KEY_NAME_TO_ACTION_NAME, _, ##args)
#define KEY_ACTIONS_DEFAULT(keys) keys(KEY_ACTION_DEFAULT)
#define KEY_ACTIONS_INIT(keys) keys(KEY_ACTION_INIT)
#define KEY_ACTIONS_DO(keys, k)					\
	switch (k) {						\
	keys(KEY_ACTION_DO)					\
								\
	default:						\
		say_error("unknown ir code: 0x%x", k);		\
								\
		return __real__MICOM_ProcessSingleKey(k, 0);	\
	}
#define KEY_ACTIONS_PRESS_HANDLERS(keys) keys(KEY_ACTION_KEY_PRESS_HANDLER)
#define KEY_ACTIONS_FIND_HANDLER(actions, s) actions(KEY_ACTION_FIND_HANDLER, s)
#define KEY_ACTIONS_HANDLER_DECLS(actions) actions(KEY_ACTION_HANDLER_DECL)

#define KEY_ACTION_HANDLER_BEGIN(n)							\
	static void key_action_##n##_handler(struct key_action *a)			\
	{										\
		if (poweroff)								\
			return;								\
											\
		say_debug("%s (0x%x) -> %s", a->key_name, a->key, a->action_name);

#define KEY_ACTION_HANDLER_END	\
		return;		\
	}


#define CONFIG_STRUCT
#include <keymap_tmpl.h>

#define ACTIONS(_, args...)			\
	_(default, ##args)			\
	KEY_PRESS_ACTION_NAMES(KEYS, _, ##args)	\
	_(key_code, ##args)			\
	_(screen_mute, ##args)			\
	_(soft_poweroff, ##args)		\
	_(osd_select, ##args)			\
	_(sdl_test, ##args)


struct key_action;

typedef void (* action_handler_t)(struct key_action *action);

struct key_action {
	char *action_name;
	char *key_name;
	unsigned key;
	action_handler_t handler;
};


WRAP_DECL(void, _MICOM_ProcessSingleKey, __UINT8 keyCode, __UINT32 keyType)

KEY_ACTIONS_HANDLER_DECLS(ACTIONS);

static void key_action_key_code_handler(struct key_action *action);


struct keymap keymap = {
#define CONFIG_DEFAULTS
#include <keymap_tmpl.h>
};

KEY_ACTIONS_DEFAULT(KEYS);

static bool poweroff = false;


#define CONFIG_PARSE
#include <keymap_tmpl.h>

static void *key_action_find_handler(const char *action_name)
{
	KEY_ACTIONS_FIND_HANDLER(ACTIONS, action_name);

	// No suitable handler has been found among defined ones.

	// Try to interpret this actions as a key code action.
	if (strncmp(action_name, ACTION_KEY_CODE_PREFIX, sizeof(ACTION_KEY_CODE_PREFIX) - 1) == 0) {
		unsigned key;
		char *endptr = NULL;

		key = strtol(action_name + sizeof(ACTION_KEY_CODE_PREFIX) - 1, &endptr, 16);
		if (endptr == NULL || *endptr == '\0')
			return key_action_key_code_handler;
	}

	say_error("unknown action handler: `%s', replace it with default one", action_name);

	return key_action_default_handler;
}

void key_action_init(void)
{
	parse_keymap(config.keymap, &keymap);

	KEY_ACTIONS_INIT(KEYS);
}

WRAP(void, _MICOM_ProcessSingleKey, __UINT8 keyCode, __UINT32 keyType __attribute__((unused)))
{
	KEY_ACTIONS_DO(KEYS, keyCode);
}

KEY_ACTION_HANDLER_BEGIN(default)
{
	return __real__MICOM_ProcessSingleKey(a->key, 0);
}
KEY_ACTION_HANDLER_END

KEY_ACTIONS_PRESS_HANDLERS(KEYS);

KEY_ACTION_HANDLER_BEGIN(key_code)
{
	unsigned key;

	key = strtol(a->action_name + sizeof(ACTION_KEY_CODE_PREFIX) - 1, NULL, 16);

	return __real__MICOM_ProcessSingleKey(key, 0);
}
KEY_ACTION_HANDLER_END

KEY_ACTION_HANDLER_BEGIN(screen_mute)
{
	static bool screen_muted = false;
	const char *cmd;

	if (!screen_muted)
		cmd = SCREEN_MUTE_ON;
	else
		cmd = SCREEN_MUTE_OFF;

	if (debug_write(cmd) == -1)
		return;

	screen_muted = !screen_muted;
}
KEY_ACTION_HANDLER_END

static void key_action_soft_poweroff_handler(struct key_action *a)
{
	const char *cmd;

	say_debug("%s (0x%x) -> %s", a->key_name, a->key, a->action_name);

	if (!poweroff)
		cmd = VOLUME_MUTE_ON
		      SCREEN_MUTE_ON;
	else
		cmd = VOLUME_MUTE_OFF
		      SCREEN_MUTE_OFF;

	if (debug_write(cmd) == -1)
		return;

	poweroff = !poweroff;

	CALL(DTV_STATUS_T, DDI_MICOM_CancelPowerOff, void)();

	return;
}

KEY_ACTION_HANDLER_BEGIN(osd_select)
{
	static bool osd_on = true;
	const char *cmd;

	if (osd_on)
		cmd = OSD_SELECT_OFF;
	else
		cmd = OSD_SELECT_ON;

	if (debug_write(cmd) == -1)
		return;

	osd_on = !osd_on;
}
KEY_ACTION_HANDLER_END

void ShowBMP(char *file, SDL_Surface *screen, int x, int y)
{
	SDL_Surface *image;
	SDL_Rect dest;

	image = SDL_LoadBMP(file);
	if ( image == NULL ) {
		say_error("Couldn't load %s: %s\n", file, SDL_GetError());
		return;
	}
	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;
	SDL_BlitSurface(image, NULL, screen, &dest);
	SDL_UpdateRects(screen, 1, &dest);
}

/*-------------------------------------*/
/* Simple SDL benchmark program
 *
 * Matthew J. Gelhaus
 * October 19, 2005
 *
 */


int VIDEOX;
int VIDEOY;


static unsigned char spriteData[] = {
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,
0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


static int getInfo(SDL_Surface *surface)
{
	say_debug("SDL_BENCHMARK: Pitch = %d", surface->pitch);

	say_debug("SDL_BENCHMARK: Hardware surfaces avail  = %d", SDL_GetVideoInfo()->hw_available);
	say_debug("SDL_BENCHMARK: Window manager avail     = %d", SDL_GetVideoInfo()->wm_available);
	say_debug("SDL_BENCHMARK: Blitter hardware         = %d", SDL_GetVideoInfo()->blit_hw);
	say_debug("SDL_BENCHMARK: Colorkey blit hardware   = %d", SDL_GetVideoInfo()->blit_hw_CC);
	say_debug("SDL_BENCHMARK: Alpha blit hardware      = %d", SDL_GetVideoInfo()->blit_hw_A);
	say_debug("SDL_BENCHMARK: Software->Hardware accel = %d", SDL_GetVideoInfo()->blit_sw);
	if (SDL_GetVideoInfo()->hw_available == 1)
		say_debug("SDL_BENCHMARK: Video memory             = %d", SDL_GetVideoInfo()->video_mem);
	else
		say_debug("SDL_BENCHMARK: Video memory             = N/A");

	return SDL_GetVideoInfo()->hw_available;
}

static int doSlowPoints(SDL_Surface *surface)
{
	int numFrames = 2;
	int k = 0;
	int x, y;
	__UINT16 *bufp;

	if (SDL_LockSurface(surface) <0) return k;
	for (k=0; k<numFrames; k++)
	{
		for(x=0;x<VIDEOX ;x++){
			for(y=0;y<VIDEOY;y++){
				bufp = (__UINT16 *)surface->pixels + y*(VIDEOX*2)/2 + x;
				*bufp = (__UINT16)(32768-(k+1)*16);
			}
			SDL_UpdateRect(surface, x, 0, 1, VIDEOY);
		}
	}
	SDL_UnlockSurface(surface);
	return k;
}

static int doFastPoints(SDL_Surface *surface)
{
	int numFrames = 32;
	int k = 0;
	int x, y;

__UINT16 *bufp;

	if (SDL_LockSurface(surface) <0) return k;
	for (k=0; k<numFrames; k++)
	{
		for(x=0;x<VIDEOX ;x++){
			for(y=0;y<VIDEOY;y++){
				bufp = (__UINT16 *)surface->pixels + y*(VIDEOX*2)/2 + x;
				*bufp = (__UINT16)(255-k*16-k);
			}
		}
		SDL_UpdateRect(surface, 0, 0, VIDEOX, VIDEOY);
	}
	SDL_UnlockSurface(surface);
	return k;
}

static int doRects(SDL_Surface *surface)
{
	int numRects = 1024;

	int k = 0;

	SDL_Rect rect;

	if (SDL_LockSurface(surface) <0) return k;
	rect.x = 0;
	rect.y = 0;
	for (k=0; k<numRects; k++)
	{
		rect.x++;
		if (rect.x == VIDEOX -6) rect.x = 0;
		rect.y++;
		if (rect.y == VIDEOY -6) rect.y = 0;
		rect.w = (VIDEOX -rect.x)/2;
		rect.h = (VIDEOY -rect.y)/2;

		SDL_FillRect(surface, &rect, k % 16535);
		SDL_UpdateRect(surface, rect.x, rect.y, rect.w, rect.h);
	}
	SDL_UnlockSurface(surface);
	return k;
}


static int doBlitter(SDL_Surface *surface)
{
	int numBlits = 4096;

	int i,j;
	int ofs;
	int minx, miny;
	SDL_Rect rect, orect;
	SDL_Color colors[3];
	colors[0].r = 0;
	colors[0].g = 0;
	colors[0].b = 0;
	colors[1].r = 255;
	colors[1].g = 255;
	colors[1].b = 0;
	colors[2].r = 255;
	colors[2].g = 0;
	colors[2].b = 0;
	SDL_SetColors(surface, colors, 0, 3);

	if (SDL_LockSurface(surface) <0) return 0;
	rect.x = 0;
	rect.y = 0;
	rect.w = VIDEOX;
	rect.h = VIDEOY;
	SDL_FillRect(surface, &rect, 0);
	for (i=0; i<32; i++)
		for (j=0; j<32; j++)
		{
			ofs = j*surface->pitch+i;
			//((unsigned char *)surface->pixels)[ofs] = (unsigned char)(rand()*255/RAND_MAX);
			((__UINT16 *)surface->pixels)[ofs] = spriteData[j*32+i]*20000;
		}
	SDL_UpdateRect(surface, 0, 0, 32, 32);
	SDL_UnlockSurface(surface);

	rect.x = 32;
	rect.y = 32;
	rect.w = 32;
	rect.h = 32;
	orect.x = 0;
	orect.y = 0;
	orect.w = 32;
	orect.h = 32;

	rect.x = 0;
	rect.y = 0;
	for (i=0; i<numBlits; i++)
	{
		orect.x = rect.x;
		orect.y = rect.y;
		rect.x++;
		if (rect.x >= VIDEOX -32) rect.x=0;
		rect.y++;
		if (rect.y >= VIDEOY -32) rect.y=0;
		minx = (orect.x < rect.x) ? orect.x : rect.x;
		miny = (orect.y < rect.y) ? orect.y : rect.y;
		SDL_BlitSurface(surface, &orect, surface, &rect);
		SDL_UpdateRect(surface, minx, miny, 32, 32);
	}
	return i;
}

int benchmark(SDL_Surface *screen)
{
    time_t starttime;
  int i, state=0;
  int hw_avail=0;
  double results[4];

    for (i=0; i<4; i++)
      results[i] = -1.0;

	VIDEOX = 1366;  VIDEOY = 768;

	say_debug("SDL_BENCHMARK: Mode = %dx%d, software", VIDEOX, VIDEOY);


    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    starttime = 1;
    state = 0;

    hw_avail = getInfo(screen);
    say_debug("SDL_BENCHMARK: ");

    // Slow points test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Slow points test");
    state = doSlowPoints(screen);
    results[0] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[0]);

    // Fast points test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Fast points test");
//    for (i=0; i<2; i++)
       state = doFastPoints(screen);
    results[1] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[1]);

    // Rect fill test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Rect fill test");
    state = doRects(screen);
    results[2] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[2]);

    // Blitter test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: 32x32 Blitter test");
    state = doBlitter(screen);
    results[3] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[3]);
 

  // Output results
  //
  say_debug("SDL_BENCHMARK: Slow points (frames/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[0]);
  say_debug("SDL_BENCHMARK: Fast points (frames/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[1]);
  say_debug("SDL_BENCHMARK:    Rect fill (rects/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[2]);
  say_debug("SDL_BENCHMARK:  32x32 blits (blits/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[3]);
  say_debug("SDL_BENCHMARK: ");


  return 0;
}
/*-------------------------------------*/


KEY_ACTION_HANDLER_BEGIN(sdl_test)
{
	//Uint32 startclock = 0;
	//DTV_STATUS_T ret;
	say_debug("sdl_test start");
	SDL_Surface *screen;

	putenv("SDL_VIDEODRIVER=lgdtv");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		say_error("sdl_test: Unable to init SDL: %s", SDL_GetError());
		return;
	}
	screen = SDL_SetVideoMode(1368, 768, 16, SDL_SWSURFACE);
	if (screen == NULL)
	{
		say_error("sdl_test: Unable to set up video: %s", SDL_GetError());
		return;
	}

	say_debug("sdl_test: sdl init done");
	/*
	startclock = SDL_GetTicks();
	ShowBMP ("/tmp/sample.bmp", screen, 50, 50);
	say_debug("sdl_test: bmp#1 done. time: %d ms", SDL_GetTicks()-startclock);
	SDL_Delay(5000);
	int x, y;
	say_debug("sdl_test: begin fill loop");
	startclock = SDL_GetTicks();
	Uint16 color;
	Uint16 *bufp;
	for(x=0;x<1366 ;x++){
		//say_debug("sdl_test: begin loop  x: %d", x);
		for(y=0;y<768;y++){ 
			color = SDL_MapRGB(screen->format, x/5, 200, y/3);
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
			//CALL(DTV_STATUS_T, DDI_GFXOSD_WritePixel, GFXOSD_VOSD_ID_T, __UINT32, __UINT32, __UINT32)(0, x, y, color);
			//if (ret != API_OK)
			//	say_error ("DDI_GFXOSD_WritePixel error: %d (x: %d, y: %d", ret, x, y);
		}
	}
	say_debug("sdl_test: fill done. time: %d ms", SDL_GetTicks()-startclock);
	startclock = SDL_GetTicks();
	SDL_UpdateRect(screen, 0, 0, 1366, 768); // fast
	say_debug("sdl_test: updated. time: %d ms", SDL_GetTicks()-startclock);

	SDL_Delay(5000);
	startclock = SDL_GetTicks();
	ShowBMP ("/tmp/sample.bmp", screen, 640, 480);
	say_debug("sdl_test: bmp#2 done. time: %d ms", SDL_GetTicks()-startclock);

	SDL_Delay(5000);
	*/
	benchmark(screen);
	say_debug("sdl_test: quiting...");
	SDL_Quit();
	say_debug("sdl_test: quit");
}
KEY_ACTION_HANDLER_END



