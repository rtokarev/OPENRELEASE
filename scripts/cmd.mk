HOSTCC = gcc
CC = ${TOOLCHAIN}gcc
cc = ${TOOLCHAIN}gcc
AR = ${TOOLCHAIN}ar
OBJDUMP = ${TOOLCHAIN}objdump

MAKE = make

CP = cp
LN = ln
RM = rm -rf
MKDIR = mkdir
CHMOD = chmod
TAR = tar

ALIGN = scripts/align
VERSION_FORMAT = scripts/version_format
SYMGEN = TOOLCHAIN=${TOOLCHAIN} scripts/symgen
SSTRIP = utils/sstrip/sstrip
MKCRAMFS = utils/mkcramfs/mkcramfs
LZOPACK = ${LG_TOOLS}/lzopack
MKEPK = ${LG_TOOLS}/mkepk

ifeq (${VERBOSE},)
$(eval override HOSTCC = @/bin/echo -e "\tHOSTCC" $$@; $(value HOSTCC))
$(eval override CC = @/bin/echo -e "\tCC" $$@; $(value CC))
$(eval override AR = @/bin/echo -e "\tAR" $$@; $(value AR))

$(eval override MAKE = @${MAKE} -s $$$ 1>/dev/null 2>/dev/null)

$(eval override CP = @${CP})
$(eval override LN = @${LN})
$(eval override RM = @${RM})
$(eval override MKDIR = @${MKDIR})
$(eval override CHMOD = @${CHMOD})
$(eval override TAR = @${TAR})

$(eval override ALIGN = @${ALIGN} $$$ 1>/dev/null 2>/dev/null)
$(eval override SYMGEN = @$(value SYMGEN) $$$ 1>/dev/null 2>/dev/null)
$(eval override MKCRAMFS = @${MKCRAMFS} $$$ 1>/dev/null 2>/dev/null)
$(eval override LZOPACK = @$(value LZOPACK) $$$ 1>/dev/null 2>/dev/null)
$(eval override MKEPK = @$(value MKEPK) $$$ 1>/dev/null 2>/dev/null)
endif

