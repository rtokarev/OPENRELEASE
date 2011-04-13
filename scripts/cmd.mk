MAKE = make

CP = cp
LN = ln
RM = rm -rf
MKDIR = mkdir
CHMOD = chmod

ALIGN = scripts/align
MKCRAMFS = utils/mkcramfs
LZOPACK = lzopack

ifeq ($(VERBOSE),)
$(eval override CC = @/bin/echo -e "\tCC" $$@; $(CC))
$(eval override AR = @/bin/echo -e "\tAR" $$@; $(AR))

$(eval override MAKE = @${MAKE} -s)

$(eval override CP = @${CP})
$(eval override LN = @${LN})
$(eval override RM = @${RM})
$(eval override MKDIR = @${MKDIR})
$(eval override CHMOD = @${CHMOD})

$(eval override STRIP = @${STRIP})
$(eval override ALIGN = @${ALIGN} $$$ 1>/dev/null 2>/dev/null)
$(eval override MKCRAMFS = @${MKCRAMFS} $$$ 1>/dev/null 2>/dev/null)
$(eval override LZOPACK = @${LZOPACK} $$$ 1>/dev/null 2>/dev/null)
endif

