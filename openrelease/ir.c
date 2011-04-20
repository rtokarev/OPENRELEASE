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

#include <ir.h>

#include <config.h>
#include <debug.h>
#include <log.h>
#include <parse_config.h>

#include <mdrv_ir_io.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define SCREEN_MUTE_ON		"kd 00 01\n"
#define SCREEN_MUTE_OFF		"kd 00 00\n"
#define VOLUME_MUTE_ON		"ke 00 00\n"
#define VOLUME_MUTE_OFF		"ke 00 01\n"
#define OSD_SELECT_ON		"kl 00 01\n"
#define OSD_SELECT_OFF		"kl 00 00\n"
#define CONTROL_LOCK_ON		"km 00 01\n"
#define CONTROL_LOCK_OFF	"km 00 00\n"

#define IR_READ_PARAM_DUMP(level, param)			\
	say(level, "IR_READ_PARAM:\n"				\
	    "\tu32Timeout = %u\n"				\
	    "\tu8KeyType = %u\n"				\
	    "\tu8KeyValue = %u\n"				\
	    "\tu8IRType = %u\n"					\
	    "\tu8IRValue = %u\n"				\
	    "\tu8TVLinkData = [%u %u %u %u %u %u %u %u %u %u]",	\
	    (param)->u32Timeout, (param)->u8KeyType,		\
	    (param)->u8KeyValue, (param)->u8IRType,		\
	    (param)->u8IRValue,					\
	    (param)->u8TVLinkData[0], (param)->u8TVLinkData[1],	\
	    (param)->u8TVLinkData[2], (param)->u8TVLinkData[3],	\
	    (param)->u8TVLinkData[4], (param)->u8TVLinkData[5],	\
	    (param)->u8TVLinkData[6], (param)->u8TVLinkData[7],	\
	    (param)->u8TVLinkData[8], (param)->u8TVLinkData[9]);

#define IR_KEY_TO_ACTION_NAME(name, code, _) _(name)
#define IR_KEY_ACTION_HANDLER(name, code) \
	int (* name##_action_handler)(IR_READ_PARAM_T *param) = ir_action_default;
#define IR_KEY_INIT_ACTION(name, code) \
	name##_action_handler = get_action_handler(keymap.name);
#define IR_KEY_DO_ACTION(name, code, param)					\
	if (((param)->u8IRType != 0 && (param)->u8IRValue == code) ||		\
	    ((param)->u8KeyType != 0 && (param)->u8KeyValue == code)) {		\
		say_debug("IR_KEY_DO_ACTION("#name", "#code")");		\
										\
		return name##_action_handler(param);				\
	}

#define IR_ACTION_KEY_PRESSED_HANDLER(name, code)		\
	static int ir_action_##name(IR_READ_PARAM_T *param) {	\
		param->u8IRValue = code;			\
								\
		return 0;					\
	}
#define IR_ACTION_NAME_TO_HANDLER(name)		\
	if (strcmp(#name, action_name) == 0)	\
		return ir_action_##name;

#define ACTION_HANDLERS(ir_keys) ir_keys(IR_KEY_ACTION_HANDLER)
#define INIT_ACTIONS(ir_keys) ir_keys(IR_KEY_INIT_ACTION)
#define ACTION_DO(ir_keys, param) ir_keys(IR_KEY_DO_ACTION, param)
#define KEY_PRESSED_ACTIONS(ir_keys, _) ir_keys(IR_KEY_TO_ACTION_NAME, _)
#define KEY_PRESSED_ACTION_HANDLERS(ir_keys) ir_keys(IR_ACTION_KEY_PRESSED_HANDLER)
#define GET_ACTION_HANDLER(actions) actions(IR_ACTION_NAME_TO_HANDLER)

#define IR_ACTION_DECL(name) \
	static int ir_action_##name(IR_READ_PARAM_T *param);
#define IR_ACTION_BEGIN(name)					\
	static int ir_action_##name(IR_READ_PARAM_T *param)	\
	{							\
		say_debug("ir_action_"#name);			\
		IR_READ_PARAM_DUMP(debug_level, param);		\
								\
		if (poweroff)					\
			return -1;

#define IR_ACTION_END		\
		usleep(500000);	\
				\
		return -1;	\
	}


#define CONFIG_STRUCT
#include <keymap_tmpl.h>

#define IR_ACTIONS(_)			\
	_(default)			\
	KEY_PRESSED_ACTIONS(IR_KEYS, _) \
	_(screen_mute)			\
	_(soft_poweroff)		\
	_(osd_select)


IR_ACTION_DECL(default);


struct keymap keymap = {
#define CONFIG_DEFAULTS
#include <keymap_tmpl.h>
};

ACTION_HANDLERS(IR_KEYS);

static bool poweroff = false;


#define CONFIG_PARSE
#include <keymap_tmpl.h>

IR_ACTION_BEGIN(default)
{
	return 0;
}
IR_ACTION_END

KEY_PRESSED_ACTION_HANDLERS(IR_KEYS);

IR_ACTION_BEGIN(screen_mute)
{
	static bool screen_muted = false;
	const char *cmd;

	if (!screen_muted)
		cmd = SCREEN_MUTE_ON;
	else
		cmd = SCREEN_MUTE_OFF;

	if (debug_write(cmd) == -1)
		return 0;

	screen_muted = !screen_muted;
}
IR_ACTION_END

static int ir_action_soft_poweroff(IR_READ_PARAM_T *param)
{
	const char *cmd;

	say_debug("ir_action_poweroff");
	IR_READ_PARAM_DUMP(debug_level, param);

	if (!poweroff)
		cmd = VOLUME_MUTE_ON
		      SCREEN_MUTE_ON
		      CONTROL_LOCK_ON;
	else
		cmd = VOLUME_MUTE_OFF
		      SCREEN_MUTE_OFF
		      CONTROL_LOCK_OFF;

	if (debug_write(cmd) == -1)
		return 0;

	poweroff = !poweroff;

	usleep(500000);

	return -1;
}

IR_ACTION_BEGIN(osd_select)
{
	static bool osd_on = true;
	const char *cmd;

	if (osd_on)
		cmd = OSD_SELECT_OFF;
	else
		cmd = OSD_SELECT_ON;

	if (debug_write(cmd) == -1)
		return 0;

	osd_on = !osd_on;
}
IR_ACTION_END

void *get_action_handler(const char *action_name)
{
	GET_ACTION_HANDLER(IR_ACTIONS);

	say_error("unknown action handler: `%s', replace with default", action_name);

	return ir_action_default;
}

void tap_ir_init(void)
{
	parse_keymap(config.keymap, &keymap);

	INIT_ACTIONS(IR_KEYS);
}

int tap_ir_keypress(int r, void *buf)
{
	IR_READ_PARAM_T *param = (IR_READ_PARAM_T *)buf;

	if (r == -1)
		return -1;

	IR_READ_PARAM_DUMP(debug_level, param);

	ACTION_DO(IR_KEYS, param);

	say_error("unknown ir code");
	IR_READ_PARAM_DUMP(error_level, param);

	return ir_action_default(param);
}
