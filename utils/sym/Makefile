CC = ${TOOLCHAIN}gcc

CFLAGS += -std=c99 -I../../include -D_GNU_SOURCE

all: sym

sym: ../../libopenrelease/log.c ../../libopenrelease/symfile.c

.PHONY: clean

clean:
	rm -f sym
