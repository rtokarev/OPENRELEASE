CC = gcc

include ../scripts/cmd.mk

all: mkcramfs sstrip

mkcramfs: LDFLAGS = -lz
mkcramfs: mkcramfs.o

sstrip: CFLAGS = -DELF_DATA=ELFDATA2LSB -DELF_CLASS=ELFCLASS32 -DELF_ARCH=EF_MIPS_ARCH
sstrip: sstrip.o

.PHONY: all clean

clean:
	${RM} mkcramfs
	${RM} mkcramfs.o
	${RM} sstrip.o
	${RM} sstrip
