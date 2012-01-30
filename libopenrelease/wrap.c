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

#include <functions2wrap.h>
#include <log.h>
#include <mem_patch.h>
#include <util.h>

#include <stdio.h>


#define FUNCTION2WRAP_MEMBER(foo_name)		\
	{					\
		.name = #foo_name,		\
		.origin_addr = NULL,		\
		.real_addr = __real_##foo_name,	\
		.wrap_addr = __wrap_##foo_name	\
	},

#define INIT_FUNCTIONS2WRAP(varname, list)	\
	struct function2wrap varname[] = { list(FUNCTION2WRAP_MEMBER) { .name = NULL } };

#define DECL_REAL_FOO(foo_name) \
	void __real_##foo_name(void);
#define DECL_WRAP_FOO(foo_name) \
	void __wrap_##foo_name(void);

#define DECL_REAL(list)	\
	list(DECL_REAL_FOO)
#define DECL_WRAP(list) \
	list(DECL_WRAP_FOO)


struct function2wrap {
	const char *name;
	void *origin_addr;
	void *real_addr;
	void *wrap_addr;
};


DECL_REAL(FUNCTIONS2WRAP)
DECL_WRAP(FUNCTIONS2WRAP)


INIT_FUNCTIONS2WRAP(functions2wrap, FUNCTIONS2WRAP);


DECL_REAL(FUNCTIONS2WRAP)

static void patch_origin_foo(void *origin_addr, void *wrap_addr, void *real_addr)
{
	int *origin_instr_set = (int *)origin_addr;

	int *real_instr_set = (int *)real_addr;

	say_debug("patch real foo");
	/* load address of an origin function into t9 */
	/* lui t9,hi(origin_instr_set) (001111 00000 11001 0000000000000000) */
	vmem_patch(&real_instr_set[0],
		   0x3C190000 | ((int)origin_instr_set >> 16));
	/* ori t9,t9,lo(origin_instr_set) (001101 11001 11001 0000000000000000) */
	vmem_patch(&real_instr_set[1],
		   0x37390000 | ((int)origin_instr_set & 0x0000ffff));
	/* copy the first 4 instructions of an origin function to a real one */
	for (int i = 0; i < 4; ++i)
		vmem_patch(&real_instr_set[2 + i],
			   origin_instr_set[i]);
	/* load address of the forth instruction of an origin function into t0 */
	/* lui t0,hi(origin_instr_set + 4) (001111 00000 01000 0000000000000000) */
	vmem_patch(&real_instr_set[6],
		   0x3C080000 | ((int)(origin_instr_set + 4) >> 16));
	/* ori t0,t0,lo(origin_instr_set + 4) (001101 01000 01000 0000000000000000) */
	vmem_patch(&real_instr_set[7],
		   0x35080000 | ((int)(origin_instr_set + 4) & 0x0000ffff));
	/* jump to the forth instruction of an origin function*/
	/* jr t0 (000000 01000 000000000000000 001000) */
	vmem_patch(&real_instr_set[8],
		   0x1000008);
	/* nop */
	vmem_patch(&real_instr_set[9],
		   0x0);

	say_debug("patch origin foo");
	/* replace the first 4 instructions of an origin function */
	/* load address of a wrapper function into t9 */
	/* lui t9,hi(&__wrap_foo) (001111 00000 11001 0000000000000000) */
	vmem_patch(&origin_instr_set[0],
		   0x3C190000 | ((int)wrap_addr >> 16));
	/* ori t9,t9,lo(&__wrap_foo) (001101 11001 11001 0000000000000000) */
	vmem_patch(&origin_instr_set[1],
		   0x37390000 | ((int)wrap_addr & 0x0000ffff));
	/* jump to a wrapper function */
	/* jr t9 (000000 11001 000000000000000 001000) */
	vmem_patch(&origin_instr_set[2],
		   0x3200008);
	/* nop */
	vmem_patch(&origin_instr_set[3],
		   0x0);
}

void wrap_init(void)
{
	struct function2wrap *foo;

	say_debug("wrap_init");

	for (foo = functions2wrap; foo->name != NULL; ++foo) {
		foo->origin_addr = sym2addr(foo->name);
		if (foo->origin_addr == NULL) {
			say_error("can't patch `%s': function address is unknown", foo->name);

			continue;
		}

		patch_origin_foo(foo->origin_addr, foo->wrap_addr, foo->real_addr);
		say_debug("`%s' = %p function was successfully patched: wrap = %p, real = %p",
			  foo->name, foo->origin_addr, foo->wrap_addr, foo->real_addr);
	}
}
