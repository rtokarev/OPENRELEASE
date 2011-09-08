release = release/release
sym = release/release.sym
linker = ld-uClibc.so.0

RELEASE = release/RELEASE

${release}: LIBS = -ldl -lm -lstdc++ -lpthread -lrt

${release}: LDFLAGS = ${LDFLAGS_COMMON} \
		      -Wl,-dynamic-linker=../lib/${linker} \
		      -Wl,--start-group \
		      	${origin_release_libs} \
		      -Wl,--end-group \
		      ${LIBS}

${release}: ${origin_release_libs} ${origin_release_objects}
	${CC} ${LDFLAGS} ${origin_release_objects} -o $@

${sym}: ${release}
	@echo "\tSYMGEN ${sym}"
	${SYMGEN} $^ > $@

.PHONY: release_install release_clean

RELEASE_NEEDED_LIBS = $(shell ${OBJDUMP} -p ${release} | \
			grep NEEDED | sort | uniq | awk '{print $$2}')
RELEASE_NEEDED_LIBS_SRC = $(patsubst %,`${cc} -print-file-name=%`,${RELEASE_NEEDED_LIBS})
RELEASE_NEEDED_LIBS_DST = $(patsubst %,lgapp/lib/%,${RELEASE_NEEDED_LIBS})

.PHONY: release_needed_libs_install release_linker_install

release_needed_libs_install:
	${MKDIR} -p lgapp/lib
	${CP} ${RELEASE_NEEDED_LIBS_SRC} lgapp/lib
	${CHMOD} +t ${RELEASE_NEEDED_LIBS_DST}
ifeq (${DEBUG},0)
	${SSTRIP} ${RELEASE_NEEDED_LIBS_DST}
endif

release_linker_install:
	${MKDIR} -p lgapp/lib
	${CP} `${cc} -print-file-name=${linker}` lgapp/lib
ifeq (${DEBUG},0)
	${SSTRIP} lgapp/lib/${linker}
endif

release_install: release_needed_libs_install release_linker_install

release_install: ${release} ${RELEASE} ${sym}
	${MKDIR} -p lgapp/bin
	${CP} ${RELEASE} lgapp/
	${CP} ${release} lgapp/bin/RELEASE
	${CP} ${sym} lgapp/bin/RELEASE.sym
	${CHMOD} +t lgapp/bin/RELEASE
ifeq (${DEBUG},0)
	${SSTRIP} lgapp/bin/RELEASE
endif

release_clean:
	${RM} ${release} ${sym}
