# Makefile to build the LGDTV SDL library

CC     = mipsel-linux-uclibc-gcc
AR     = mipsel-linux-uclibc-ar
RANLIB = mipsel-linux-uclibc-ranlib
OPENRELEASE_PATH = ../../
#CFLAGS = -Winline -mabi=32 -mips32 -mtune=mips32 -O3 -fPIC -fstrength-reduce -fthread-jumps -fexpensive-optimizations -fomit-frame-pointer -frename-registers -pipe -G 0 -ffast-math -msoft-float -I./include -I${OPENRELEASE_PATH}/include
CFLAGS =  -O2 -fPIC -I./include -I${OPENRELEASE_PATH}/include

SOURCES = \
	src/*.c \
	src/audio/*.c \
	src/cdrom/*.c \
	src/cpuinfo/*.c \
	src/events/*.c \
	src/file/*.c \
	src/joystick/*.c \
	src/stdlib/*.c \
	src/thread/*.c \
	src/timer/*.c \
	src/video/*.c \
	src/audio/dummy/*.c \
	src/video/lgdtv/*.c \
	src/joystick/linux/*.c \
	src/cdrom/linux/*.c \
	src/thread/pthread/*.c \
	src/timer/unix/*.c \
	src/loadso/dlopen/*.c \

MAIN = src/main/dummy/SDL_dummy_main.o
OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.c,\.o,g')

all: libSDLmain.a libSDL.a libSDL.so

libSDLmain.a: $(MAIN)
	$(AR) crv $@ $^
	$(RANLIB) $@

libSDL.a: $(OBJECTS)
	$(AR) crv $@ $^
	$(RANLIB) $@

libSDL.so: $(OBJECTS)
	$(CC) -o $@ -shared $? -ldl -lm -lpthread

clean:
	rm -f libSDLmain.a libSDL.a libSDL.so $(MAIN) $(OBJECTS)
