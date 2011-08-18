CFLAGS += -I${KERNEL_SOURCES}/include
ifdef EXPAND
CFLAGS += -DCONFIG_LG_KERNEL_EXPAND -DCONFIG_LG_XIP
endif
LDFLAGS = -lz

all: mkcramfs

.PHONY: clean

clean:
	rm -rf mkcramfs
