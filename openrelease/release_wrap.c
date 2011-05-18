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
#include <config.h>
#include <RELEASE.h>
#include <time.h>

API_STATE_T __real_API_EME_DecodePreviewImageUsingTask(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp);

API_STATE_T __wrap_API_EME_DecodePreviewImageUsingTask(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp)
{
	API_STATE_T retval;
	say_debug ("__wrap_API_EME_DecodePreviewImageUsingTask:");
	say_debug ("pszFilePath: %s", pszFilePath);
	say_debug ("x: %d, y: %d, w: %d, h: %d", x, y, width, height);
	say_debug ("bDisplayImageDirectly: %d", bDisplayImageDirectly);
	say_debug ("timeStamp: %d", timeStamp);

	say_debug ("starting __real_API_EME_DecodePreviewImageUsingTask...");
	retval = __real_API_EME_DecodePreviewImageUsingTask(pszFilePath, x, y, width, height, bDisplayImageDirectly, timeStamp);
	say_debug ("retval: %s", retval);
	return retval;	
}

API_STATE_T __real_API_EME_PreviewDivx(char * pszFilePath, EME_RECT_T rect);

API_STATE_T __wrap_API_EME_PreviewDivx(char * pszFilePath, EME_RECT_T rect)
{
	API_STATE_T retval;
	__UINT32 timeStamp;
	
	say_debug ("__wrap_API_EME_PreviewDivx:");
	say_debug ("pszFilePath: %s", pszFilePath);
	say_debug ("x: %d, y: %d, w: %d, h: %d", rect.x, rect.y, rect.w, rect.h);
	if (!config.enable_divx_preview) {
	    timeStamp = time (NULL);
		say_debug ("starting __wrap_API_EME_DecodePreviewImageUsingTask...");
		retval = __wrap_API_EME_DecodePreviewImageUsingTask("/mnt/usb1/Drive1/preview.jpg", rect.x, rect.y, rect.w, rect.h, 1, timeStamp);
		return retval;
	}		
	
	say_debug ("starting __real_API_EME_PreviewDivx...");
	retval = __real_API_EME_PreviewDivx(pszFilePath, rect);
	say_debug ("retval: %s", retval);
	return retval;
}
