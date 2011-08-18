mkcramfs:
	@echo "\tMAKE mkcramfs"
ifneq (${PLATFORM_NAME}, DVB-SATURN6)
	${MAKE} -C utils/mkcramfs KERNEL_SOURCES=${KERNEL_SOURCES} EXPAND=1
else
	${MAKE} -C utils/mkcramfs KERNEL_SOURCES=${KERNEL_SOURCES}
endif

sstrip:
	@echo "\tMAKE sstrip"
	${MAKE} -C utils/sstrip

.PHONY: mkcramfs sstrip utils_clean

utils_clean:
	${MAKE} -C utils/sstrip clean
	${MAKE} -C utils/mkcramfs clean
