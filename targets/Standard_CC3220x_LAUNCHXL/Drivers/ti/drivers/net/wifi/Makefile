
include ../../../../../imports.mak

# Find subdirectories
RELEASE-CCS-SUBDIRS   = $(wildcard ./ccs/rtos/.)
RELEASE-GCC-SUBDIRS   = $(wildcard ./gcc/rtos/.)
RELEASE-IAR-SUBDIRS   = $(wildcard ./iar/rtos/.)
RELEASE-CCS-SUBDIRS   += $(wildcard ./ccs/nortos/.)
RELEASE-GCC-SUBDIRS   += $(wildcard ./gcc/nortos/.)
RELEASE-IAR-SUBDIRS   += $(wildcard ./iar/nortos/.)



RELEASE-SUBDIRS =
ifneq ($(CCS_ARMCOMPILER),)
	RELEASE-SUBDIRS += $(RELEASE-CCS-SUBDIRS)
endif
ifneq ($(GCC_ARMCOMPILER),)
	RELEASE-SUBDIRS += $(RELEASE-GCC-SUBDIRS)
endif
ifneq ($(IAR_ARMCOMPILER),)
	RELEASE-SUBDIRS += $(RELEASE-IAR-SUBDIRS)
endif

.PHONY: release $(RELEASE-SUBDIRS)

# rules for building
all: release

release: $(RELEASE-SUBDIRS)

$(RELEASE-SUBDIRS):
	@ $(MAKE) -C $@ -f makefile.defs

CLEAN-RELEASE-SUBDIRS = $(addprefix clean-,$(RELEASE-SUBDIRS))

.PHONY: clean-release $(CLEAN-RELEASE-SUBDIRS)

# rules for cleaning
clean: clean-release

clean-release: $(CLEAN-RELEASE-SUBDIRS)

$(CLEAN-RELEASE-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) -f makefile.defs clean
