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

openrelease_sources = openrelease/debug.c \
		      openrelease/ir.c \
		      openrelease/libc_wrap.c \
		      openrelease/log.c \
		      openrelease/main_wrap.c \
		      openrelease/parse_config.c \
		      openrelease/release_wrap.c \
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
	${CP} ${RELEASE_CFG} image/bin/openrelease.cfg
ifndef DEBUG
	${STRIP} image/bin/openrelease
else
	${MKDIR} -p image/src
	${CP} ${openrelease_sources} image/src/
endif
	${CHMOD} +t image/bin/openrelease
	${MKDIR} -p image/cfg
	${CP} ${OPENRELEASE_CONFIG} image/cfg/; fi
	${CP} ${OPENRELEASE_KEYMAP} image/cfg/; fi

openrelease_clean:
	${RM} ${openrelease_objects}
	${RM} ${openrelease}
