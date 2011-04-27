openrelease = openrelease/openrelease
OPENRELEASE = openrelease/OPENRELEASE

${openrelease}: LIBS = ${librelease} \
		       -ldl -lm -lstdc++ -lpthread -lrt
${openrelease}: LDWRAP = $(patsubst %,-Wl${comma}--wrap=%,${WRAPS}) \
			 -Wl,--wrap=main
ifdef STATIC
${openrelease}: LDFLAGS += ${LDFLAGS_COMMON} \
			   -static \
			   -Wl,--start-group \
			   ${origin_release_libs} \
			   ${LIBS} \
			   -Wl,--end-group \
			   ${LDWRAP}
else
${openrelease}: LDFLAGS += ${LDFLAGS_COMMON} \
			   -Wl,-dynamic-linker=lib/${linker} \
			   ${LIBS} \
			   ${LDWRAP}
endif

openrelease_sources = openrelease/config.c \
		      openrelease/debug.c \
		      openrelease/ir.c \
		      openrelease/libc_wrap.c \
		      openrelease/log.c \
		      openrelease/main_wrap.c \
		      openrelease/parse_config.c \
		      openrelease/release_wrap.c \
		      openrelease/version.c \
		      third_party/daemon.c

openrelease_objects = $(patsubst %.c,%.o,${openrelease_sources})
ifdef STATIC
${openrelease}: openrelease_objects += ${origin_release_objects}
endif

${openrelease}: ${openrelease_objects} ${librelease}
	${CC} ${openrelease_objects} ${LDFLAGS} -o $@

openrelease_install: ${openrelease} ${OPENRELEASE}
ifdef STATIC
	${RM} image
endif
	${MKDIR} -p image/bin
	${CP} ${OPENRELEASE} image/
ifdef STATIC
	${LN} -s OPENRELEASE image/RELEASE
endif
	${CP} ${openrelease} image/bin/openrelease
ifndef DEBUG
	${STRIP} image/bin/openrelease
else
	${MKDIR} -p image/src
	${CP} ${openrelease_sources} image/src/
endif
	${CHMOD} +t image/bin/openrelease
ifdef OPENRELEASE_CONFIG
	${MKDIR} -p image/etc
	${CP} ${OPENRELEASE_CONFIG} image/etc/
endif
ifdef OPENRELEASE_KEYMAP
	${MKDIR} -p image/etc
	${CP} ${OPENRELEASE_KEYMAP} image/etc/
endif

openrelease_clean:
	${RM} ${openrelease_objects}
	${RM} ${openrelease}
