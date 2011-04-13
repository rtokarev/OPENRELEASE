ifndef STATIC
librelease = librelease/librelease.so

${librelease}: LDWRAP = $(patsubst %,-Wl${comma}--wrap=%,${WRAPS})
${librelease}: LDFLAGS = -shared \
			 -Wl,-EL \
			 -Wl,--undefined=main \
			 ${LDWRAP}

${librelease}: ${origin_release_libs} ${origin_release_objects}
	${CC} ${LDFLAGS} -Wl,--start-group ${origin_release_libs} -Wl,--end-group ${origin_release_objects} -o $@

endif

librelease_clean:
	${RM} ${librelease}
