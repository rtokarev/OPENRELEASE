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

#define KEYS(_, args...)				\
	_(K_PAGE_UP,		0x00, ##args)		\
	_(K_PAGE_DOWN,		0x01, ##args)		\
	_(K_VOL_UP,		0x02, ##args)		\
	_(K_VOL_DOWN,		0x03, ##args)		\
/*	_(K_UNKNOWN_X04,	0x04, ##args) */	\
/*	_(K_UNKNOWN_X05,	0x05, ##args) */	\
	_(K_RIGHT,		0x06, ##args)		\
	_(K_LEFT,		0x07, ##args)		\
	_(K_POWER,		0x08, ##args)		\
	_(K_MUTE,		0x09, ##args)		\
/*	_(K_UNKNOWN_X0A,	0x0a, ##args) */	\
	_(K_INPUT,		0x0b, ##args)		\
/*	_(K_UNKNOWN_X0C,	0x0c, ##args) */	\
/*	_(K_UNKNOWN_X0D,	0x0d, ##args) */	\
/*	_(K_UNKNOWN_X0E,	0x0e, ##args) */	\
/*	_(K_UNKNOWN_X0F,	0x0f, ##args) */	\
	_(K_0,			0x10, ##args)		\
	_(K_1,			0x11, ##args)		\
	_(K_2,			0x12, ##args)		\
	_(K_3,			0x13, ##args)		\
	_(K_4,			0x14, ##args)		\
	_(K_5,			0x15, ##args)		\
	_(K_6,			0x16, ##args)		\
	_(K_7,			0x17, ##args)		\
	_(K_8,			0x18, ##args)		\
	_(K_9,			0x19, ##args)		\
	_(K_Q_VIEW,		0x1a, ##args)		\
/*	_(K_UNKNOWN_X1B,	0x1b, ##args) */	\
/*	_(K_UNKNOWN_X1C,	0x1c, ##args) */	\
/*	_(K_UNKNOWN_X1D,	0x1d, ##args) */	\
	_(K_FAV_MARK,		0x1e, ##args)		\
/*	_(K_UNKNOWN_X1F,	0x1f, ##args) */	\
	_(K_TEXT,		0x20, ##args)		\
	_(K_T_OPT,		0x21, ##args)		\
/*	_(K_UNKNOWN_X22,	0x22, ##args) */	\
/*	_(K_UNKNOWN_X23,	0x23, ##args) */	\
/*	_(K_UNKNOWN_X24,	0x24, ##args) */	\
/*	_(K_UNKNOWN_X25,	0x25, ##args) */	\
/*	_(K_UNKNOWN_X26,	0x26, ##args) */	\
/*	_(K_UNKNOWN_X27,	0x27, ##args) */	\
	_(K_RETURN_EXIT,	0x28, ##args)		\
/*	_(K_UNKNOWN_X29,	0x29, ##args) */	\
/*	_(K_UNKNOWN_X2A,	0x2a, ##args) */	\
/*	_(K_UNKNOWN_X2B,	0x2b, ##args) */	\
/*	_(K_UNKNOWN_X2C,	0x2c, ##args) */	\
/*	_(K_UNKNOWN_X2D,	0x2d, ##args) */	\
/*	_(K_UNKNOWN_X2E,	0x2e, ##args) */	\
/*	_(K_UNKNOWN_X2F,	0x2f, ##args) */	\
	_(K_AV_MODE,		0x30, ##args)		\
/*	_(K_UNKNOWN_X31,	0x31, ##args) */	\
/*	_(K_UNKNOWN_X32,	0x32, ##args) */	\
/*	_(K_UNKNOWN_X33,	0x33, ##args) */	\
/*	_(K_UNKNOWN_X34,	0x34, ##args) */	\
/*	_(K_UNKNOWN_X35,	0x35, ##args) */	\
/*	_(K_UNKNOWN_X36,	0x36, ##args) */	\
/*	_(K_UNKNOWN_X37,	0x37, ##args) */	\
/*	_(K_UNKNOWN_X38,	0x38, ##args) */	\
	_(K_SUBTITLE,		0x39, ##args)		\
/*	_(K_UNKNOWN_X3A,	0x3a, ##args) */	\
/*	_(K_UNKNOWN_X3B,	0x3b, ##args) */	\
/*	_(K_UNKNOWN_X3C,	0x3c, ##args) */	\
/*	_(K_UNKNOWN_X3D,	0x3d, ##args) */	\
/*	_(K_UNKNOWN_X3E,	0x3e, ##args) */	\
/*	_(K_UNKNOWN_X3F,	0x3f, ##args) */	\
	_(K_UP,			0x40, ##args)		\
	_(K_DOWN,		0x41, ##args)		\
/*	_(K_UNKNOWN_X42,	0x42, ##args) */	\
	_(K_MENU,		0x43, ##args)		\
	_(K_OK,			0x44, ##args)		\
	_(K_Q_MENU,		0x45, ##args)		\
/*	_(K_UNKNOWN_X46,	0x46, ##args) */	\
/*	_(K_UNKNOWN_X47,	0x47, ##args) */	\
/*	_(K_UNKNOWN_X48,	0x48, ##args) */	\
/*	_(K_UNKNOWN_X49,	0x49, ##args) */	\
/*	_(K_UNKNOWN_X4A,	0x4a, ##args) */	\
/*	_(K_UNKNOWN_X4B,	0x4b, ##args) */	\
/*	_(K_UNKNOWN_X4C,	0x4c, ##args) */	\
/*	_(K_UNKNOWN_X4D,	0x4d, ##args) */	\
/*	_(K_UNKNOWN_X4E,	0x4e, ##args) */	\
/*	_(K_UNKNOWN_X4F,	0x4f, ##args) */	\
/*	_(K_UNKNOWN_X50,	0x50, ##args) */	\
/*	_(K_UNKNOWN_X51,	0x51, ##args) */	\
/*	_(K_UNKNOWN_X52,	0x52, ##args) */	\
	_(K_LIST,		0x53, ##args)		\
/*	_(K_UNKNOWN_X54,	0x54, ##args) */	\
/*	_(K_UNKNOWN_X55,	0x55, ##args) */	\
/*	_(K_UNKNOWN_X56,	0x56, ##args) */	\
/*	_(K_UNKNOWN_X57,	0x57, ##args) */	\
	_(K_WIDGETS,            0x58, ##args)		\
	_(K_NETCAST,		0x59, ##args)		\
	_(K_INPUT_AV1,		0x5a, ##args)		\
	_(K_EXIT,		0x5b, ##args)		\
/*	_(K_UNKNOWN_X5C,	0x5c, ##args) */	\
/*	_(K_UNKNOWN_X5D,	0x5d, ##args) */	\
/*	_(K_UNKNOWN_X5E,	0x5e, ##args) */	\
/*	_(K_UNKNOWN_X5F,	0x5f, ##args) */	\
/*	_(K_UNKNOWN_X60,	0x60, ##args) */	\
	_(K_BLUE_KEY,		0x61, ##args)		\
/*	_(K_UNKNOWN_X62,	0x62, ##args) */	\
	_(K_YELLOW_KEY,		0x63, ##args)		\
/*	_(K_UNKNOWN_X64,	0x64, ##args) */	\
	_(K_FREEZE,		0x65, ##args)		\
/*	_(K_UNKNOWN_X66,	0x66, ##args) */	\
/*	_(K_UNKNOWN_X67,	0x67, ##args) */	\
/*	_(K_UNKNOWN_X68,	0x68, ##args) */	\
/*	_(K_UNKNOWN_X69,	0x69, ##args) */	\
/*	_(K_UNKNOWN_X6A,	0x6a, ##args) */	\
/*	_(K_UNKNOWN_X6B,	0x6b, ##args) */	\
/*	_(K_UNKNOWN_X6C,	0x6c, ##args) */	\
/*	_(K_UNKNOWN_X6D,	0x6d, ##args) */	\
/*	_(K_UNKNOWN_X6E,	0x6e, ##args) */	\
/*	_(K_UNKNOWN_X6F,	0x6f, ##args) */	\
/*	_(K_UNKNOWN_X70,	0x70, ##args) */	\
	_(K_GREEN_KEY,		0x71, ##args)		\
	_(K_RED_KEY,		0x72, ##args)		\
/*	_(K_UNKNOWN_X73,	0x73, ##args) */	\
/*	_(K_UNKNOWN_X74,	0x74, ##args) */	\
/*	_(K_UNKNOWN_X75,	0x75, ##args) */	\
	_(K_RATIO_4_3,          0x76, ##args)		\
	_(K_RATIO_16_9,         0x77, ##args)		\
/*	_(K_UNKNOWN_X78,	0x78, ##args) */	\
	_(K_RATIO,		0x79, ##args)		\
/*	_(K_UNKNOWN_X7A,	0x7a, ##args) */	\
/*	_(K_UNKNOWN_X7B,	0x7b, ##args) */	\
/*	_(K_UNKNOWN_X7C,	0x7c, ##args) */	\
/*	_(K_UNKNOWN_X7D,	0x7d, ##args) */	\
	_(K_SIMPLINK,		0x7e, ##args)		\
/*	_(K_UNKNOWN_X7F,	0x7f, ##args) */	\
/*	_(K_UNKNOWN_X80,	0x80, ##args) */	\
/*	_(K_UNKNOWN_X81,	0x81, ##args) */	\
/*	_(K_UNKNOWN_X82,	0x82, ##args) */	\
/*	_(K_UNKNOWN_X83,	0x83, ##args) */	\
/*	_(K_UNKNOWN_X84,	0x84, ##args) */	\
/*	_(K_UNKNOWN_X85,	0x85, ##args) */	\
/*	_(K_UNKNOWN_X86,	0x86, ##args) */	\
/*	_(K_UNKNOWN_X87,	0x87, ##args) */	\
/*	_(K_UNKNOWN_X88,	0x88, ##args) */	\
/*	_(K_UNKNOWN_X89,	0x89, ##args) */	\
/*	_(K_UNKNOWN_X8A,	0x8a, ##args) */	\
/*	_(K_UNKNOWN_X8B,	0x8b, ##args) */	\
/*	_(K_UNKNOWN_X8C,	0x8c, ##args) */	\
/*	_(K_UNKNOWN_X8D,	0x8d, ##args) */	\
	_(K_FORWARD,		0x8e, ##args)		\
	_(K_BACKWARD,		0x8f, ##args)		\
/*	_(K_UNKNOWN_X90,	0x90, ##args) */	\
	_(K_AD,			0x91, ##args)		\
/*	_(K_UNKNOWN_X92,	0x92, ##args) */	\
/*	_(K_UNKNOWN_X93,	0x93, ##args) */	\
/*	_(K_UNKNOWN_X94,	0x94, ##args) */	\
	_(K_ENERGY_SAVING,	0x95, ##args)		\
/*	_(K_UNKNOWN_X96,	0x96, ##args) */	\
/*	_(K_UNKNOWN_X97,	0x97, ##args) */	\
/*	_(K_UNKNOWN_X98,	0x98, ##args) */	\
/*	_(K_UNKNOWN_X99,	0x99, ##args) */	\
/*	_(K_UNKNOWN_X9A,	0x9a, ##args) */	\
/*	_(K_UNKNOWN_X9B,	0x9b, ##args) */	\
/*	_(K_UNKNOWN_X9C,	0x9c, ##args) */	\
/*	_(K_UNKNOWN_X9D,	0x9d, ##args) */	\
	_(K_LIVETV,             0x9e, ##args)		\
	_(K_APP,                0x9f, ##args)		\
/*	_(K_UNKNOWN_XA0,	0xa0, ##args) */	\
/*	_(K_UNKNOWN_XA1,	0xa1, ##args) */	\
/*	_(K_UNKNOWN_XA2,	0xa2, ##args) */	\
/*	_(K_UNKNOWN_XA3,	0xa3, ##args) */	\
/*	_(K_UNKNOWN_XA4,	0xa4, ##args) */	\
/*	_(K_UNKNOWN_XA5,	0xa5, ##args) */	\
/*	_(K_UNKNOWN_XA6,	0xa6, ##args) */	\
/*	_(K_UNKNOWN_XA7,	0xa7, ##args) */	\
/*	_(K_UNKNOWN_XA8,	0xa8, ##args) */	\
/*	_(K_UNKNOWN_XA9,	0xa9, ##args) */	\
	_(K_INFO,		0xaa, ##args)		\
	_(K_GUIDE,              0xab, ##args)		\
/*	_(K_UNKNOWN_XAC,	0xac, ##args) */	\
/*	_(K_UNKNOWN_XAD,	0xad, ##args) */	\
/*	_(K_UNKNOWN_XAE,	0xae, ##args) */	\
	_(K_RATIO_ZOOM,         0xaf, ##args)		\
	_(K_PLAY,		0xb0, ##args)		\
	_(K_STOP,		0xb1, ##args)		\
/*	_(K_UNKNOWN_XB2,	0xb2, ##args) */	\
/*	_(K_UNKNOWN_XB3,	0xb3, ##args) */	\
/*	_(K_UNKNOWN_XB4,	0xb4, ##args) */	\
/*	_(K_UNKNOWN_XB5,	0xb5, ##args) */	\
/*	_(K_UNKNOWN_XB6,	0xb6, ##args) */	\
/*	_(K_UNKNOWN_XB7,	0xb7, ##args) */	\
/*	_(K_UNKNOWN_XB8,	0xb8, ##args) */	\
/*	_(K_UNKNOWN_XB9,	0xb9, ##args) */	\
	_(K_PAUSE,		0xba, ##args)		\
/*	_(K_UNKNOWN_XBB,	0xbb, ##args) */	\
/*	_(K_UNKNOWN_XBC,	0xbc, ##args) */	\
	_(K_REC,                0xbd, ##args)		\
/*	_(K_UNKNOWN_XBE,	0xbe, ##args) */	\
	_(K_INPUT_COMPONENT1,   0xbf, ##args)		\
/*	_(K_UNKNOWN_XC0,	0xc0, ##args) */	\
/*	_(K_UNKNOWN_XC1,	0xc1, ##args) */	\
/*	_(K_UNKNOWN_XC2,	0xc2, ##args) */	\
/*	_(K_UNKNOWN_XC3,	0xc3, ##args) */	\
/*	_(K_UNKNOWN_XC4,	0xc4, ##args) */	\
/*	_(K_UNKNOWN_XC5,	0xc5, ##args) */	\
/*	_(K_UNKNOWN_XC6,	0xc6, ##args) */	\
/*	_(K_UNKNOWN_XC7,	0xc7, ##args) */	\
/*	_(K_UNKNOWN_XC8,	0xc8, ##args) */	\
/*	_(K_UNKNOWN_XC9,	0xc9, ##args) */	\
/*	_(K_UNKNOWN_XCA,	0xca, ##args) */	\
/*	_(K_UNKNOWN_XCB,	0xcb, ##args) */	\
	_(K_INPUT_HDMI2,        0xcc, ##args)		\
/*	_(K_UNKNOWN_XCD,	0xcd, ##args) */	\
	_(K_INPUT_HDMI1,	0xce, ##args)		\
/*	_(K_UNKNOWN_XCF,	0xcf, ##args) */	\
	_(K_INPUT_AV2,          0xd0, ##args)		\
/*	_(K_UNKNOWN_XD1,	0xd1, ##args) */	\
/*	_(K_UNKNOWN_XD2,	0xd2, ##args) */	\
/*	_(K_UNKNOWN_XD3,	0xd3, ##args) */	\
	_(K_INPUT_COMPONENT2,   0xd4, ##args)		\
	_(K_INPUT_RGB,          0xd5, ##args)		\
	_(K_TV,                 0xd6, ##args)		\
/*	_(K_UNKNOWN_XD7,	0xd7, ##args) */	\
/*	_(K_UNKNOWN_XD8,	0xd8, ##args) */	\
/*	_(K_UNKNOWN_XD9,	0xd9, ##args) */	\
/*	_(K_UNKNOWN_XDA,	0xda, ##args) */	\
/*	_(K_UNKNOWN_XDB,	0xdb, ##args) */	\
	_(K_3D,                 0xdc, ##args)		\
/*	_(K_UNKNOWN_XDD,	0xdd, ##args) */	\
/*	_(K_UNKNOWN_XDE,	0xde, ##args) */	\
/*	_(K_UNKNOWN_XDF,	0xdf, ##args) */	\
	_(K_BRIGHT_DOWN,        0xe0, ##args)		\
	_(K_BRIGHT_UP,          0xe1, ##args)		\
/*	_(K_UNKNOWN_XE2,	0xe2, ##args) */	\
/*	_(K_UNKNOWN_XE3,	0xe3, ##args) */	\
/*	_(K_UNKNOWN_XE4,	0xe4, ##args) */	\
/*	_(K_UNKNOWN_XE5,	0xe5, ##args) */	\
/*	_(K_UNKNOWN_XE6,	0xe6, ##args) */	\
/*	_(K_UNKNOWN_XE7,	0xe7, ##args) */	\
/*	_(K_UNKNOWN_XE8,	0xe8, ##args) */	\
/*	_(K_UNKNOWN_XE9,	0xe9, ##args) */	\
/*	_(K_UNKNOWN_XEA,	0xea, ##args) */	\
/*	_(K_UNKNOWN_XEB,	0xeb, ##args) */	\
/*	_(K_UNKNOWN_XEC,	0xec, ##args) */	\
/*	_(K_UNKNOWN_XED,	0xed, ##args) */	\
/*	_(K_UNKNOWN_XEE,	0xee, ##args) */	\
/*	_(K_UNKNOWN_XEF,	0xef, ##args) */	\
	_(K_TV_RAD,		0xf0, ##args)		\
/*	_(K_UNKNOWN_XF1,	0xf1, ##args) */	\
/*	_(K_UNKNOWN_XF2,	0xf2, ##args) */	\
/*	_(K_UNKNOWN_XF3,	0xf3, ##args) */	\
/*	_(K_UNKNOWN_XF4,	0xf4, ##args) */	\
/*	_(K_UNKNOWN_XF5,	0xf5, ##args) */	\
/*	_(K_UNKNOWN_XF6,	0xf6, ##args) */	\
/*	_(K_UNKNOWN_XF7,	0xf7, ##args) */	\
/*	_(K_UNKNOWN_XF8,	0xf8, ##args) */	\
	_(K_TILT,		0xf9, ##args)		\
	_(K_IN_STOP,		0xfa, ##args)		\
	_(K_IN_START,		0xfb, ##args)		\
	_(K_P_CHECK,		0xfc, ##args)		\
	_(K_S_CHECK,		0xfd, ##args)		\
	_(K_POWERONLY,		0xfe, ##args)		\
	_(K_EZ_ADJUST,		0xff, ##args)

#endif
