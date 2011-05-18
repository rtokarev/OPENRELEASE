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
#include <RELEASE.h>


int __real_main(int argc, char *argv[]);
int __real_mount(const char *source, const char *target, const char *filesystemtype,
		 unsigned long mountflags, const void *data);
API_STATE_T __real_API_EME_DecodePreviewImageUsingTask(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp);
API_STATE_T __real_API_EME_PreviewDivx(char * pszFilePath, EME_RECT_T rect);

int __wrap_main(int argc, char *argv[])
{
	argv[0] = "RELEASE";

	return __real_main(argc, argv);
}

int __wrap_mount(const char *source, const char *target, const char *filesystemtype,
		 unsigned long mountflags, const void *data)
{
	return __real_mount(source, target, filesystemtype, mountflags, data);
}

API_STATE_T __wrap_API_EME_DecodePreviewImageUsingTask(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp)
{
	return __real_API_EME_DecodePreviewImageUsingTask(pszFilePath, x, y, width, height, bDisplayImageDirectly, timeStamp);
}

API_STATE_T __wrap_API_EME_PreviewDivx(char * pszFilePath, EME_RECT_T rect)
{
	return __real_API_EME_PreviewDivx(pszFilePath, rect);
}
