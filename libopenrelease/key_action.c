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
#include <uinput.h>

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
	_(sdl_test, ##args)				\
	_(gop_gwin_info_get, ##args)


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
	uinput_send(keyCode);
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

int SDL_TestThread() 
{
	SDL_Surface *screen;
	say_debug("SDL_TestThread: Initializing SDL.");

	putenv("SDL_VIDEODRIVER=lgdtv");
	
	if((SDL_Init(SDL_INIT_VIDEO)==-1)) { 
		say_error("SDL_TestThread: Could not initialize SDL: %s.", SDL_GetError());
		return -1;
	}

	say_debug("SDL_TestThread: SDL initialized.");

	say_debug("SDL_TestThread: pre SDL_SetVideoMode");
	screen=SDL_SetVideoMode(1368, 768, 16, SDL_SWSURFACE ); 
	if ( screen == NULL ){ 
		say_error("SDL_TestThread: Unable to set 1368x768x16 video: %s", SDL_GetError()); 
		exit(1); 
	} 	
	say_debug("SDL_TestThread: SDL_SetVideoMode done");
	
	ShowBMP ("/tmp/sample.bmp", screen, 50, 50);
	SDL_Delay(500);
	int x,y;
	for(x=0;x<1368 ;x++){ 
		for(y=0;y<768;y++){ 

			Uint32 color;
			if (y>0)
			    color = SDL_MapRGB(screen->format, 144, 40, 60);
			if (y>200)
			    color = 0x48a7;
			if (y>400)
			    color = SDL_MapRGBA(screen->format, 144, 40, 60, 100);
			if (y>600)
			    color = 0xC8a7;
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		SDL_UpdateRect(screen, x-1, 0, x+1, y); // fast
	} 	
	ShowBMP ("/tmp/sample.bmp", screen, 640, 480);
	
	int i = 0;
	int loop = 1;
	do {
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{	
			say_debug("SDL_TestThread: SDL_Event #%d, type: ", i++, event.type);
			switch (event.type) 
			{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					say_debug ("SDL_TestThread: SDL_Event: sym: 0x%X, scancode: 0x%X", event.key.keysym.sym, event.key.keysym.scancode);
					if (event.key.keysym.sym == SDLK_RETURN_EXIT) {
						loop = 0;
						say_debug ("SDL_TestThread: SDL_Event: SDLK_RETURN_EXIT");
					}
					break;
			}
		}
	} while (loop);

	say_debug("SDL_TestThread: Quiting SDL.");
	SDL_Quit();
	say_debug("SDL_TestThread: Quiting....");
	return 1;
}

KEY_ACTION_HANDLER_BEGIN(sdl_test)
{
	SDL_Thread *testthread = NULL;
	testthread = SDL_CreateThread(SDL_TestThread, NULL);
	if (testthread == NULL) {
		say_error("SDL_CreateThread failed");
	}
}
KEY_ACTION_HANDLER_END

KEY_ACTION_HANDLER_BEGIN(gop_gwin_info_get)
{
	GOP_HW_Type eGOP_Type = E_GOP4G_0;
	unsigned char u8Wid;
	GopGwinInfo pGInfo[10];
	int pitch;


	for (u8Wid=0; u8Wid<10; u8Wid++ ){
		pitch = 0 ;
		//	short unsigned int MAdp_GOP_GWIN_Info_Get(GOP_HW_Type eGOP_Type, unsigned char u8Wid, GopGwinInfo * pGInfo)
		CALL (short unsigned int, MAdp_GOP_GWIN_Info_Get, GOP_HW_Type, unsigned char, GopGwinInfo *)(eGOP_Type, u8Wid, &pGInfo[u8Wid]);

		say_debug ("\nGOP[%d] info-----------------------------------", u8Wid);
		say_debug ("pGInfo.bUsed=%d", pGInfo[u8Wid].bUsed);
		say_debug ("pGInfo.u32DispHStart=%d", pGInfo[u8Wid].u32DispHStart) ;
		say_debug ("pGInfo.u32DispVStart=%d", pGInfo[u8Wid].u32DispVStart);
		say_debug ("pGInfo.u32DispWidth=%d", pGInfo[u8Wid].u32DispWidth);
		say_debug ("pGInfo.u32DispHeight=%d", pGInfo[u8Wid].u32DispHeight);
		say_debug ("pGInfo.u32DRAMRBlkStart=0x%08X", pGInfo[u8Wid].u32DRAMRBlkStart);
		say_debug ("pGInfo.u32DRAMRBlkHSize=%d", pGInfo[u8Wid].u32DRAMRBlkHSize);
		say_debug ("pGInfo.u32DRAMRBlkVSize=%d", pGInfo[u8Wid].u32DRAMRBlkVSize);
		say_debug ("pGInfo.u32SrcHStart=%d", pGInfo[u8Wid].u32SrcHStart);
		say_debug ("pGInfo.u32SrcVStart=%d", pGInfo[u8Wid].u32SrcVStart);
		say_debug ("pGInfo.u32ColorDepth=%d", pGInfo[u8Wid].u32ColorDepth);
		
		say_debug ("pGInfo.eColorTyped=");
		switch(pGInfo[u8Wid].eColorType){
		case E_GOP_COLOR_RGB555_BLINK:
			say_debug ("E_GOP_COLOR_RGB555_BLINK = E_GOP_COLOR_ARGB1555");
			break ;
		case E_GOP_COLOR_RGB565:
			say_debug ("E_GOP_COLOR_RGB565");
			break ;
		case E_GOP_COLOR_ARGB1555:
			say_debug ("E_GOP_COLOR_ARGB1555");
			break ;
		case E_GOP_COLOR_ARGB4444:
			say_debug ("E_GOP_COLOR_ARGB4444");
			pitch = (((((pGInfo[u8Wid].u32DRAMRBlkHSize) << 1)+15)>>4)<<4) ;
			break ;
		case E_GOP_COLOR_I8:
			say_debug ("E_GOP_COLOR_I8");
			pitch = pGInfo[u8Wid].u32DRAMRBlkHSize ;
			break ;
		case E_GOP_COLOR_ARGB8888:
			say_debug ("E_GOP_COLOR_ARGB8888");
			pitch = (((((pGInfo[u8Wid].u32DRAMRBlkHSize) << 2)+15)>>4)<<4) ;
			break ;
		default:
			say_debug( "unknown %d", pGInfo[u8Wid].eColorType);
			break ;
		}
		
		say_debug("pGInfo.pitch=%d", pitch);
		say_debug("pGInfo.mem size=0x%08X", (pitch*(pGInfo[u8Wid].u32DRAMRBlkVSize)));
		say_debug("pGInfo.u32Enable=%d", pGInfo[u8Wid].u32Enable);		
	}

}
KEY_ACTION_HANDLER_END

