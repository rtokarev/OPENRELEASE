libopenrelease = libopenrelease/libopenrelease.so

libopenrelease_sources = libopenrelease/config.c \
			 libopenrelease/debug.c \
			 libopenrelease/key_action.c \
			 libopenrelease/libc_wrap.c \
			 libopenrelease/log.c \
			 libopenrelease/mem_patch.c \
			 libopenrelease/openrelease.c \
			 libopenrelease/parse_config.c \
			 libopenrelease/release_wrap.c \
			 libopenrelease/stdio_wrap.c \
			 libopenrelease/symfile.c \
			 libopenrelease/util.c \
			 libopenrelease/version.c \
			 libopenrelease/wrap.c \
			 third_party/daemon.c

libopenrelease_objects = $(patsubst %.c,%.o,${libopenrelease_sources})

${libopenrelease}: LIBS = -ldl -lutil

${libopenrelease}: LDFLAGS = ${LDFLAGS_COMMON} \
			     -shared

${libopenrelease}: ${libopenrelease_objects}
	${CC} ${LDFLAGS} ${libopenrelease_objects} -o $@ ${LIBS}

.PHONY: libopenrelease_clean

libopenrelease_clean:
	${RM} ${libopenrelease_objects}
	${RM} ${libopenrelease}
