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

#include <wrap.h>
#include <util.h>

#include <config.h>

#include <RELEASE.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#if PLATFORM == SATURN6

WRAP_DECL(API_STATE_T, API_EME_PreviewDivx, char *pszFilePath, EME_RECT_T rect);
WRAP(API_STATE_T, API_EME_PreviewDivx, char *pszFilePath, EME_RECT_T rect)
{
	say_debug("API_EME_PreviewDivx");

	if (!config.enable_divx_preview)
		return API_ERROR;

	return __real_API_EME_PreviewDivx(pszFilePath, rect);
}

#endif

#if PLATFORM == BCM

DEBUG_STATE_T get_conf_debug_state() {
	if(strcmp(config.debug_state, "RELEASE") == 0) {
		return RELEASE;
	} else if(strcmp(config.debug_state, "DEBUG") == 0) {
		return DEBUG;
	} else if(strcmp(config.debug_state, "EVENT") == 0) {
		return EVENT;
	}

	say_debug("Unknown debug state configured. Using `EVENT' as default.");

	return EVENT;
}

char* get_debug_state_name(DEBUG_STATE_T state) {
	switch (state) {
		case RELEASE:
			return "RELEASE";
		case DEBUG:
			return "DEBUG";
		case EVENT:
			return "EVENT";
		default:
			say_debug("unknown debug state (%u) found");
			return "{unknown}";
	}
}

//WRAP_DECL(int, OSA_MD_GetDbgMode);
//WRAP(int, OSA_MD_GetDbgMode)
//{
//	return __real_OSA_MD_GetDbgMode();
//}

WRAP_DECL(void, OSA_MD_SetDbgMode, DEBUG_STATE_T state);
WRAP(void, OSA_MD_SetDbgMode, DEBUG_STATE_T state)
{
	say_debug("real OSA_MD_SetDbgMode('%s') intercepted.", get_debug_state_name(state));

	DEBUG_STATE_T use_state = get_conf_debug_state();

	say_debug("call real OSA_MD_SetDbgMode('%s') now.", get_debug_state_name(use_state));

	__real_OSA_MD_SetDbgMode(use_state);

	use_state = CALL(DEBUG_STATE_T,OSA_MD_GetDbgMode, void)();

	say_debug("OSA_MD_GetDbgMode() = '%s'", get_debug_state_name(use_state));

}


WRAP_DECL(void, SUMDSVC_CTRL_SetDebugStatus, DEBUG_STATE_T state);
WRAP(void, SUMDSVC_CTRL_SetDebugStatus, DEBUG_STATE_T state)
{
	say_debug("real SUMDSVC_CTRL_SetDebugStatus('%s') intercepted.", get_debug_state_name(state));

	say_debug("call real OSA_MD_SetDbgMode('%s') now.", get_debug_state_name(RELEASE));

	__real_OSA_MD_SetDbgMode(RELEASE);

	DEBUG_STATE_T use_state = get_conf_debug_state();

	say_debug("call real SUMDSVC_CTRL_SetDebugStatus('%s') now.", get_debug_state_name(use_state));

	__real_SUMDSVC_CTRL_SetDebugStatus(use_state);

	say_debug("call real OSA_MD_SetDbgMode('%s') now.", get_debug_state_name(use_state));

	__real_OSA_MD_SetDbgMode(use_state);

	use_state = CALL(DEBUG_STATE_T,SUMDSVC_CTRL_GetDebugStatus, DEBUG_STATE_T*)(&use_state);

	say_debug("SUMDSVC_CTRL_GetDebugStatus() = '%s'", get_debug_state_name(use_state));
}

WRAP_DECL(void, DDI_MICOM_SetDebugStatus, DEBUG_STATE_T state);
WRAP(void, DDI_MICOM_SetDebugStatus, DEBUG_STATE_T state)
{
	say_debug("real DDI_MICOM_SetDebugStatus('%s') intercepted.", get_debug_state_name(state));

	DEBUG_STATE_T use_state = get_conf_debug_state();

	say_debug("call real DDI_MICOM_SetDebugStatus('%s') now.", get_debug_state_name(use_state));

	__real_DDI_MICOM_SetDebugStatus(use_state);

	use_state = CALL(DEBUG_STATE_T,DDI_MICOM_GetDebugStatus, void)();

	say_debug("DDI_MICOM_GetDebugStatus() = '%s'", get_debug_state_name(use_state));
}

WRAP_DECL(void, OSA_MD_GetDebugStatus);
WRAP(void, OSA_MD_GetDebugStatus)
{
	say_debug("real OSA_MD_GetDebugStatus intercepted.");
	say_debug("blocking call to real OSA_MD_GetDebugStatus.");

	DEBUG_STATE_T use_state = get_conf_debug_state();

	say_debug("call real OSA_MD_SetDbgMode('%s') now.", get_debug_state_name(use_state));

	__real_OSA_MD_SetDbgMode(use_state);
}

WRAP_DECL(void, OSA_MD_InitDebugStatus);
WRAP(void, OSA_MD_InitDebugStatus)
{
	say_debug("real OSA_MD_InitDebugStatus intercepted.");
	say_debug("blocking call to real OSA_MD_InitDebugStatus.");

	DEBUG_STATE_T use_state = get_conf_debug_state();

	say_debug("call real SUMDSVC_CTRL_SetDebugStatus('%s') instead.", get_debug_state_name(use_state));
	__real_SUMDSVC_CTRL_SetDebugStatus(use_state);
	say_debug("call real OSA_MD_SetDbgMode('%s') instead.", get_debug_state_name(use_state));
	__real_OSA_MD_SetDbgMode(use_state);
}

void set_debug_state(DEBUG_STATE_T state) {

	say_debug("call real OSA_MD_SetDbgMode('%s')", get_debug_state_name(state));
	__real_OSA_MD_SetDbgMode(state);
	say_debug("call real DDI_MICOM_SetDebugStatus('%s')", get_debug_state_name(state));
	__real_DDI_MICOM_SetDebugStatus(state);
	say_debug("call real SUMDSVC_CTRL_SetDebugStatus('%s')", get_debug_state_name(state));
	__real_SUMDSVC_CTRL_SetDebugStatus(state);

}

#endif

WRAP_DECL(void, API_DDM_POWER_ShutdownSystem, DDI_POWER_OFF_MODE_T powerOffMode);
WRAP(void, API_DDM_POWER_ShutdownSystem, DDI_POWER_OFF_MODE_T powerOffMode)
{
	say_debug("API_DDM_POWER_ShutdownSystem(%u) intercepted.", powerOffMode);

	DEBUG_STATE_T debug_state = CALL(DEBUG_STATE_T,OSA_MD_GetDbgMode, void)();

	say_debug("OSA_MD_GetDbgMode() = '%s'.", get_debug_state_name(debug_state));

	debug_state = CALL(DEBUG_STATE_T,DDI_MICOM_GetDebugStatus, void)();

	say_debug("DDI_MICOM_GetDebugStatus() = '%s'.", get_debug_state_name(debug_state));

	debug_state = CALL(DEBUG_STATE_T,SUMDSVC_CTRL_GetDebugStatus, DEBUG_STATE_T*)(&debug_state);

	say_debug("SUMDSVC_CTRL_GetDebugStatus() = '%s'.", get_debug_state_name(debug_state));

	DEBUG_STATE_T use_state = get_conf_debug_state();

	set_debug_state(use_state);

	if (config.poweroff_script != NULL) {
		say_debug("Execute configured poweroff script `%s' now.", config.poweroff_script);

		if(system(config.poweroff_script)) {
			say_error("Error executing poweroff script `%s'. Ignoring...",
				  config.poweroff_script);
		}

	} else {
		say_debug("No poweroff script configured.");
	}

	say_debug("Call API_DDM_POWER_ShutdownSystem(%u) now.", powerOffMode);

	__real_API_DDM_POWER_ShutdownSystem(powerOffMode);
}


WRAP_DECL(int, UI_SUMODE_IsEnableDbgStatusMenu);
WRAP(int, UI_SUMODE_IsEnableDbgStatusMenu)
{
	say_debug("UI_SUMODE_IsEnableDbgStatusMenu intercepted.");
	return 1;
}
