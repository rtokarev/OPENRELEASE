#ifndef _KEYS_H_
#define _KEYS_H_
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

#if PLATFORM == SATURN6
#define PLATFORM_KEYS(_, args...)             \
	_(K_GUIDE,		0xa9, ##args)
#elif PLATFORM == SATURN7 || PLATFORM == BCM
#define PLATFORM_KEYS(_, args...)	      \
	_(K_GUIDE,		0xab, ##args)
#endif

#define KEYS(_, args...)		      \
	_(K_ENERGY_SAVING,	0x95, ##args) \
	_(K_POWER,		0x08, ##args) \
	_(K_INPUT,		0x0b, ##args) \
	_(K_RATIO,		0x79, ##args) \
	_(K_TV_RAD,		0xf0, ##args) \
	_(K_Q_MENU,		0x45, ##args) \
	_(K_MENU,		0x43, ##args) \
	_(K_UP,			0x40, ##args) \
	_(K_DOWN,		0x41, ##args) \
	_(K_LEFT,		0x07, ##args) \
	_(K_RIGHT,		0x06, ##args) \
	_(K_OK,			0x44, ##args) \
	_(K_RETURN_EXIT,	0x28, ##args) \
	_(K_INFO,		0xaa, ##args) \
	_(K_AV_MODE,		0x30, ##args) \
	_(K_VOL_UP,		0x02, ##args) \
	_(K_VOL_DOWN,		0x03, ##args) \
	_(K_FAV_MARK,		0x1e, ##args) \
	_(K_MUTE,		0x09, ##args) \
	_(K_PAGE_UP,		0x00, ##args) \
	_(K_PAGE_DOWN,		0x01, ##args) \
	_(K_0,			0x10, ##args) \
	_(K_1,			0x11, ##args) \
	_(K_2,			0x12, ##args) \
	_(K_3,			0x13, ##args) \
	_(K_4,			0x14, ##args) \
	_(K_5,			0x15, ##args) \
	_(K_6,			0x16, ##args) \
	_(K_7,			0x17, ##args) \
	_(K_8,			0x18, ##args) \
	_(K_9,			0x19, ##args) \
	_(K_LIST,		0x53, ##args) \
	_(K_Q_VIEW,		0x1a, ##args) \
	_(K_RED_KEY,		0x72, ##args) \
	_(K_GREEN_KEY,		0x71, ##args) \
	_(K_YELLOW_KEY,		0x63, ##args) \
	_(K_BLUE_KEY,		0x61, ##args) \
	_(K_TEXT,		0x20, ##args) \
	_(K_T_OPT,		0x21, ##args) \
	_(K_SUBTITLE,		0x39, ##args) \
	_(K_SIMPLINK,		0x7e, ##args) \
	_(K_FREEZE,		0x65, ##args) \
	_(K_NETCAST,		0x59, ##args) \
	_(K_EXIT,		0x5b, ##args) \
	_(K_STOP,		0xb1, ##args) \
	_(K_PLAY,		0xb0, ##args) \
	_(K_PAUSE,		0xba, ##args) \
	_(K_FORWARD,		0x8f, ##args) \
	_(K_BACKWARD,		0xbe, ##args) \
	_(K_AD,			0x91, ##args) \
	_(K_TILT,		0xf9, ##args) \
	_(K_IN_STOP,		0xfa, ##args) \
	_(K_IN_START,		0xfb, ##args) \
	_(K_P_CHECK,		0xfc, ##args) \
	_(K_S_CHECK,		0xfd, ##args) \
	_(K_POWERONLY,		0xfe, ##args) \
	_(K_EZ_ADJUST,		0xff, ##args) \
	_(K_INPUT_AV1,		0x5a, ##args) \
	_(K_INPUT_HDMI,		0xce, ##args) \
					      \
	PLATFORM_KEYS(_, ##args)

#endif
