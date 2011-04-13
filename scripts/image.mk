image.cramfs.lzo: image.cramfs
	@echo "\tLZOPACK image.cramfs.lzo"
	${LZOPACK} -9 $^ $@
	${ALIGN} $@

ifndef STATIC
image.cramfs: NEEDED_LIBS = $(shell ${OBJDUMP} -p ${release} ${openrelease} | \
			      grep NEEDED | sort | uniq | awk '{print $$2}')

image.cramfs: ${release} ${RELEASE} ${openrelease} ${OPENRELEASE}
	@echo "\tMKCRAMFS image.cramfs"
	${RM} image
	${MAKE} release_install
	${MAKE} openrelease_install
	${MKDIR} -p image/lib
	${CP} $(patsubst %,`${cc} -print-file-name=%`,${NEEDED_LIBS}) image/lib
	${CP} `${cc} -print-file-name=${linker}` image/lib
	${STRIP} image/lib/*
	${CHMOD} +t image/lib/*
	${CHMOD} -t image/lib/${linker}
	${MKCRAMFS} image $@
else
ifeq (${STATIC}, OPENRELEASE)
image.cramfs: ${openrelease}
	@echo "\tMKCRAMFS image.cramfs"
	${MAKE} openrelease_install
else
ifneq (${STATIC}, RELEASE)
.PHONY: image.cramfs
image.cramfs:
	@echo "possible values for STATIC are RELEASE/OPENRELEASE"
	@exit 1
else
image.cramfs: ${release}
	@echo "\tMKCRAMFS image.cramfs"
	${MAKE} release_install
endif
endif
	${MKCRAMFS} image $@
endif

image_clean:
	${RM} image
	${RM} image.cramfs image.cramfs.lzo
