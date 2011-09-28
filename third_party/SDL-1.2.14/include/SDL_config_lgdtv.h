#ifndef _SDL_config_lgdtv_h
#define _SDL_config_lgdtv_h

#include "SDL_platform.h"

/* C datatypes */
#define SDL_HAS_64BIT_TYPE      1

/* Endianness */
#define SDL_BYTEORDER		1234

/* Useful headers */
#define HAVE_ALLOCA_H		1
#define HAVE_SYS_TYPES_H	1
#define HAVE_STDIO_H		1
#define STDC_HEADERS		1
#define HAVE_STRING_H		1
#define HAVE_INTTYPES_H		1
#define HAVE_STDINT_H		1
#define HAVE_CTYPE_H		1
#define HAVE_MATH_H		1
#define HAVE_SIGNAL_H		1

/* C library functions */
#define HAVE_MALLOC		1
#define HAVE_CALLOC		1
#define HAVE_REALLOC		1
#define HAVE_FREE		1
#define HAVE_ALLOCA		1
#define HAVE_GETENV		1
#define HAVE_PUTENV		1
#define HAVE_UNSETENV		1
#define HAVE_QSORT		1
#define HAVE_ABS		1
#define HAVE_BCOPY		1
#define HAVE_MEMSET		1
#define HAVE_MEMCPY		1
#define HAVE_MEMMOVE		1
#define HAVE_MEMCMP		1
#define HAVE_STRLEN		1
#define HAVE_STRLCAT		1
#define HAVE_STRDUP		1
#define HAVE_STRCHR		1
#define HAVE_STRRCHR		1
#define HAVE_STRSTR		1
#define HAVE_STRTOL		1
#define HAVE_STRTOUL		1
#define HAVE_STRTOLL		1
#define HAVE_STRTOULL		1
#define HAVE_STRTOD		1
#define HAVE_ATOI		1
#define HAVE_ATOF		1
#define HAVE_STRCMP		1
#define HAVE_STRNCMP		1
#define HAVE_STRCASECMP		1
#define HAVE_STRNCASECMP	1
#define HAVE_SSCANF		1
#define HAVE_SNPRINTF		1
#define HAVE_VSNPRINTF		1
#define HAVE_SIGACTION		1
#define HAVE_SETJMP		1
#define HAVE_NANOSLEEP		1

/* Enable the dummy audio driver (src/audio/dummy/\*.c) */
#define SDL_AUDIO_DRIVER_DUMMY	1

/* Enable the linux cdrom driver (src/cdrom/linux/\*.c) */
#define SDL_CDROM_LINUX		1

/* Enable the linux joystick driver (src/joystick/linux/\*.c) */
#define SDL_JOYSTICK_LINUX	1
#define SDL_INPUT_LINUXEV	1

/* Enable the dlopen shared object loader (src/loadso/dlopen/\*.c) */
#define SDL_LOADSO_DLOPEN	1

/* Enable the pthread thread support (src/thread/pthread/\*.c) */
#define SDL_THREAD_PTHREAD	1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX	0
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP	1

/* Enable the unix timer support (src/timer/unix/\*.c) */
#define SDL_TIMER_UNIX		1

/* Enable the lgdtv video driver (src/video/lgdtv/\*.c) */
#define SDL_VIDEO_DRIVER_LGDTV	1

#endif /* _SDL_config_lgdtv_h */
