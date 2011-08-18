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

#include <parse_config.h>

#include <log.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void _close_stream(FILE **streamp)
{
	if (*streamp != NULL)
		fclose(*streamp);
}

void _free_pairs(KeyValuePair **pairsp)
{
	if (*pairsp != NULL)
		free(*pairsp);
}

KeyValuePair *parseCfgFile(const char *file)
{
	FILE *stream __attribute__((cleanup(_close_stream))) = NULL;

	if (file == NULL)
		return NULL;

	stream = fopen(file, "r");
	if (stream == NULL) {
		say_error("can't open `%s': %m", file);

		return NULL;
	}

	KeyValuePair *pairs __attribute__((cleanup(_free_pairs))) = NULL;
	unsigned npairs = 0;
	unsigned n = 0;
	size_t len = 0;
	char *line = NULL;

	while (!feof(stream)) {
		if (getline(&line, &len, stream) == -1 ) {
			if (feof(stream))
				break;

			say_error("can't read line from `%s': %m", file);

			return NULL;
		}

		// skip empty lines and comments
		if (line[0] == '\0' || line[0] == '\n' || line[0] == '#')
			continue;

		if (n == npairs) {
			// use here malloc instead of realloc because realloc is broken by ...
			unsigned new_npairs = (1 + npairs) * 2;
			KeyValuePair *tmp = malloc(new_npairs * sizeof(KeyValuePair));

			if (tmp == NULL)
				return NULL;

			if (pairs) {
				memcpy(tmp, pairs, npairs * sizeof(KeyValuePair));
				free(pairs);
			}

			pairs = tmp;
			npairs = new_npairs;
		}

		int r;

		r = sscanf(line, "%s = %s",
			   pairs[n].key, pairs[n].value);
		if (r != 2) {
			say_error("bad line format: `%s'", line);

			return NULL;
		}

		++n;
	}

	free(line);

	pairs[n].key[0] = '\0';

	KeyValuePair *r_pairs = pairs;

	pairs = NULL;

	return r_pairs;
}

KeyValuePair *findKeyValuePair(KeyValuePair *pairs, const char *key)
{
	for (KeyValuePair *pair = pairs; pair != NULL && pair->key[0] != '\0'; ++pair)
		if (strcasecmp(pair->key, key) == 0)
			return pair;

	return NULL;
}

void freeKeyValuePair(KeyValuePair *pairs)
{
	if (pairs)
		free(pairs);
}

unsigned parse_unsigned(char *name __attribute__((unused)), char *str, unsigned defval, int *parserr)
{
	*parserr = 0;

	if (str) {
		unsigned long v;
		char *endptr = NULL;

		v = strtoul(str, &endptr, 10);
		if (*endptr != '\0' || v == ULONG_MAX)
			*parserr = 1;

		defval = v;
	}

	return defval;
}

char *parse_string(char *name __attribute__((unused)), char *str, char *defval, int *parserr)
{
	*parserr = 0;

	if (str == NULL) {
		if (defval != NULL)
			defval = strdup(defval);
	} else {
		if (strcmp(str, "NULL") == 0)
			defval = NULL;
		else
			defval = strdup(str);
	}

	return defval;
}
