DATE = $(shell date +%Y%m%d)
EPK_VERSION_HEX := $(shell ${VERSION_FORMAT} ${EPK_VERSION})
PAK_VERSION_HEX := $(shell ${VERSION_FORMAT} ${PAK_VERSION})

ifeq (${PLATFORM_NAME}, DVB-SATURN6)
lgapp.epk: lgapp.pak
	@echo "\tEPK lgapp.epk"
	@if [ "${EPK_VERSION_HEX}" = "" ]; \
	then \
		echo "\t\tbad EPK_VERSION format: ${EPK_VERSION}"; \
		exit 1; \
	fi
	${MKEPK} -m ${EPK_VERSION_HEX} ${PLATFORM_ID} $@ $^

lgapp.pak: lgapp.cramfs.lzo
	@echo "\tPAK lgapp.pak"
	@if [ "${PAK_VERSION_HEX}" = "" ]; \
	then \
		echo "\t\tbad PAK_VERSION format: ${PAK_VERSION}"; \
		exit 1; \
	fi
	${MKEPK} -c $@ $^ lgapp ${PLATFORM_NAME} ${PAK_VERSION_HEX} ${DATE} RELEASE
endif

lgapp.cramfs.lzo: lgapp.cramfs
	@echo "\tLZOPACK lgapp.cramfs.lzo"
	${LZOPACK} -9 $^ $@
	${ALIGN} $@

lgapp.cramfs: lgapp
	@echo "\tMKCRAMFS lgapp.cramfs"
	${MKCRAMFS} lgapp $@

lgapp.tar.gz: lgapp
	@echo "\tCREATE lgapp.tar.gz"
	${TAR} czf $@ lgapp

lgapp: ${release} ${RELEASE} ${sym}
	@echo "\tCREATE lgapp"
	${RM} lgapp
	${MAKE} release_install

.PHONY: lgapp_clean

lgapp_clean:
	${RM} lgapp
	${RM} lgapp.cramfs lgapp.cramfs.lzo
	${RM} lgapp.pak lgapp.epk
	${RM} lgapp.tar.gz
