/*
 * Copyright (c) 2011 Eugene Vorobev <jenya.vv@gmail.com>
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

#include <log.h>

#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>


void *load_plugin(const char *sofile)
{
	void * plugin = dlopen(sofile, RTLD_LAZY);
	const char *error = (char *)dlerror();

	if (plugin == NULL ) {
		say_error("load_plugin: Failed loading %s: %s\n", sofile, error);
	} else {
		say_debug("load_plugin: Success loading %s\n", sofile);
	}

	return (plugin);
}

int start_plugin(void * plugin)
{
	void *(*plugin_main)(void*)=NULL;

	plugin_main = dlsym(plugin, "plugin_main");
	const char *error = (char *)dlerror();

	if (error != NULL) {
        say_error("start_plugin: dlsym error: %s\n", error);
        return 0;
    }

	pthread_t plugin_thread;
	int rc = pthread_create(&plugin_thread, NULL, plugin_main, NULL);
	if (rc) {
		say_error("start_plugin: pthread_create error: %d\n", rc);
		return 0;
	}

	return 1;
}
