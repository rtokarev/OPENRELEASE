include scripts/default.mk
-include config.mk
include scripts/cmd.mk

origin_release_libs = ${RELEASE_LIB}/*.a
origin_release_objects = ${RELEASE_LIB}/*.o

VERSION = $(shell git describe)
VERSION_BASE = $(shell git describe | sed -e 's/-.*//')

CFLAGS = -mips32 \
	 -std=c99 \
	 -fPIC \
         -Wall -Wextra -Werror \
	 -D_GNU_SOURCE \
	 -I. \
	 -Iinclude \
	 -include platform.h \
	 -DOPENRELEASE_VERSION=\"${VERSION}\" \
	 -DPLATFORM=${PLATFORM}

LDFLAGS_COMMON = -Wl,-EL

ifeq (${DEBUG},1)
CFLAGS += -O0 -ggdb3 -DDEBUG
else
CFLAGS += -O2
endif

all: libopenrelease

include utils/utils.mk
include release/release.mk
include libopenrelease/libopenrelease.mk
include scripts/lgapp.mk

libopenrelease: ${libopenrelease}
release: sstrip lgapp.tar.gz

#ifeq ($(filter depend clean,${MAKECMDGOALS}),)
# include .depend
#endif

.PHONY: all release libopenrelease clean

clean: utils_clean release_clean libopenrelease_clean lgapp_clean
	${RM} .depend
