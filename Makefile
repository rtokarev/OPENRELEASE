CC = mipsel-linux-gcc
cc = mipsel-linux-gcc
AR = mipsel-linux-ar
OBJDUMP = mipsel-linux-objdump
STRIP = utils/sstrip

include scripts/cmd.mk
include scripts/default.mk
include scripts/wraps.mk
-include config.mk

comma = ,

linker = ld-uClibc.so.0

origin_release_libs = ${RELEASE_LIB}/*.a
origin_release_objects = ${RELEASE_LIB}/*.o

CFLAGS = -mips32 \
	 -std=c99 \
         -Wall -Wextra -Werror \
	 -D_GNU_SOURCE \
	 -I. \
	 -Iinclude \
	 -I${KERNEL_SOURCES}/drivers/mstar/include

LDFLAGS_COMMON = -Wl,-EL

ifdef DEBUG
CFLAGS += -ggdb3 -DDEBUG
else
CFLAGS += -O2
endif

all: utils image.cramfs.lzo

utils:
	${MAKE} -C utils

include librelease/librelease.mk
include release/release.mk
include openrelease/openrelease.mk
include scripts/image.mk

#ifeq ($(filter depend clean,${MAKECMDGOALS}),)
# include .depend
#endif

.PHONY: all clean utils

clean: librelease_clean release_clean openrelease_clean image_clean
	${RM} .depend
	${RM} tags
	${MAKE} -C utils clean
