/*
 * Copyright (c) 2011 Teodor Sigaev <teodor@sigaev.ru>
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

/**************************************************************
 *                            Example                         *
 **************************************************************

 BEGIN_CONFIG(TarantulCfg)
	 PARAM_CONFIG(port, int, 4322, parse_int32, print_int32)
	 PARAM_CONFIG(nameint, int, -23, parse_int32, print_int32)
	 PARAM_CONFIG(nameint64, int64_t, 10, parse_int64, print_int64)
	 PARAM_CONFIG(float_field, float, 10e10, parse_float, print_float)
	 PARAM_CONFIG(namestr, char*, "/tmp", parse_string, print_string)
 END_CONFIG

 **************************************************************
 *             end of example of config file                  *
 **************************************************************/

/*
 * Description of macroses:
 *
 * BEGIN_CONFIG( name_of_struct )
 *    begin declaration or definition with pointed name
 * PARAM_CONFIG(field_name, field_type, default_value, func_to_parse, func_to_print)
 *    various information used for declaration/definition/parsing. func_to_parse
 *    is a function which parses C-string and converts it into field's value.
 *    Signature of function:
 *    field_type func_to_parse(char* field_name, char* source_string, field_type default_value, int *error);
 *    void func_to_print(StringBuffer *b, field_type value);
 * END_CONFIG
 *    end declaration or definition
 *
 * Goals:
 *
 * CONFIG_STRUCT
 *    makes structure definition
 * CONFIG_DEFAULTS
 *	  list of default values
 * CONFIG_PARSE
 *    produces static definition of parsing function, signature:
 *    void parse_name_of_struct(char *file, name_of_struct *_cfg)
 *    If file is NULL then parse function puts default values into
 *    config struct
 * CONFIG_CLEAN
 *    undefs all macroses defined in this file
 */

#ifdef CONFIG_CLEAN
/************ cleanup ***************/

#undef BEGIN_CONFIG
#undef PARAM_CONFIG
#undef END_CONFIG
#undef COMMENT_CHAR

#undef CONFIG_STRUCT
#undef CONFIG_DEFAULTS
#undef CONFIG_PARSE
#undef CONFIG_CLEAN

#elif defined CONFIG_STRUCT
/************ definition of struct ***************/

#define BEGIN_CONFIG(cfgname)	\
struct cfgname {

#define PARAM_CONFIG(field, type, default, parser, printer)	\
	type	field;

#define END_CONFIG					\
	int __end_of_struct;			\
};

#elif defined CONFIG_DEFAULTS
/************ definition of struct ***************/

#define BEGIN_CONFIG(cfgname)

#define PARAM_CONFIG(field, type, default, parser, printer)	default,

/* value of __end_of_struct */
#define END_CONFIG		0

#elif defined CONFIG_PARSE
/************ definition of parsing function ***************/

#define BEGIN_CONFIG(cfgname)						\
static void parse_##cfgname(char *file, struct cfgname* _cfg) {			\
	KeyValuePair	*all, *ptr;					\
	int				error;						\
	all = parseCfgFile(file);

#define PARAM_CONFIG(field, type, default, parser, printer)		\
	ptr = findKeyValuePair(all, #field);				\
	(_cfg)->field = parser(#field, (ptr && ptr->value) ? ptr->value : NULL, default, &error); \
	if (error) say_error("bad value format: "#field" = %s", ptr->value);

#define END_CONFIG							\
	freeKeyValuePair(all);					\
}

#else
#pragma warning No action for config
#endif
