release_epk = dynRELEASE.${RELEASE_VERSION}.epk

DATE = $(shell date +%Y%m%d)
EPK_VERSION := $(shell ${VERSION_FORMAT} ${RELEASE_VERSION})
PAK_VERSION := $(shell ${VERSION_FORMAT} ${VERSION_BASE})

ifeq (${PLATFORM_NAME}, DVB-SATURN6)
${release_epk}: lgapp.pak
	@echo "\tEPK lgapp.epk"
	@if [ "${EPK_VERSION}" = "" ]; \
	then \
		echo "\t\tbad RELEASE_VERSION format: ${RELEASE_VERSION}"; \
		exit 1; \
	fi
	${MKEPK} -m ${EPK_VERSION} ${PLATFORM_ID} $@ $^

lgapp.pak: lgapp.cramfs.lzo
	@echo "\tPAK lgapp.pak"
	@if [ "${PAK_VERSION}" = "" ]; \
	then \
		echo "\t\tbad VERSION_BASE format: ${VERSION_BASE}"; \
		exit 1; \
	fi
	${MKEPK} -c $@ $^ lgapp ${PLATFORM_NAME} ${PAK_VERSION} ${DATE} RELEASE
endif

lgapp.cramfs.lzo: lgapp.cramfs
	@echo "\tLZOPACK lgapp.cramfs.lzo"
	${LZOPACK} -9 $^ $@
	${ALIGN} $@

lgapp.cramfs: ${release} ${RELEASE} ${sym}
	@echo "\tMKCRAMFS lgapp.cramfs"
	${RM} lgapp
	${MAKE} release_install
	${MKCRAMFS} lgapp $@

lgapp.tar.gz: ${release} ${RELEASE} ${sym}
	@echo "\tCREATE lgapp.tar.gz"
	${RM} lgapp
	${MAKE} release_install
	${TAR} czf $@ lgapp

.PHONY: lgapp_clean

lgapp_clean:
	${RM} lgapp
	${RM} lgapp.cramfs lgapp.cramfs.lzo
	${RM} lgapp.pak lgapp.epk
	${RM} lgapp.tar.gz
