release = release/release
RELEASE = release/RELEASE

${release}: LIBS = ${librelease} \
		   -ldl -lm -lstdc++ -lpthread -lrt

${release}: LDWRAP = $(patsubst %,-Wl${comma}--wrap=%,${WRAPS})

ifdef STATIC
${release}: LDFLAGS = -static \
		      -Wl,-EL \
		      -Wl,--start-group \
		      ${origin_release_libs} \
		      ${LIBS} \
		      -Wl,--end-group \
		      ${LDWRAP}
else
${release}: LDFLAGS = -Wl,-EL \
		      -Wl,-dynamic-linker=lib/${linker} \
		      ${LIBS} \
	              ${LDWRAP}
endif

release_sources = release/release_wrap_stub.c

release_objects = $(patsubst %.c,%.o,${release_sources})
ifdef STATIC
${release}: release_objects += ${origin_release_objects}
endif

${release}: ${release_objects} ${librelease}
	${CC} ${release_objects} ${LDFLAGS} -o $@

release_install: ${release} ${RELEASE}
ifdef STATIC
	${RM} image
endif
	${MKDIR} -p image/bin
	${CP} ${RELEASE} image/
	${CP} ${release} image/bin/release
	${CP} ${RELEASE_CFG} image/bin/release.cfg
ifndef DEBUG
	${STRIP} image/bin/release
else
	${MKDIR} image/src
	${CP} ${release_sources} image/src/
endif
	${CHMOD} +t image/bin/release

release_clean:
	${RM} ${release_objects}
	${RM} ${release}
