 SIMPLELINK_SDK_INSTALL_DIR ?= $(abspath ../../../)

include $(SIMPLELINK_SDK_INSTALL_DIR)/imports.mak

OBJS = button.obj \
	checkbox.obj \
	circle.obj \
	context.obj \
	display.obj \
	fontcm12.obj \
	fontcm12b.obj \
	fontcm12i.obj \
	fontcm14.obj \
	fontcm14b.obj \
	fontcm14i.obj \
	fontcm16.obj \
	fontcm16b.obj \
	fontcm16i.obj \
	fontcm18.obj \
	fontcm18b.obj \
	fontcm18i.obj \
	fontcm20.obj \
	fontcm20b.obj \
	fontcm20i.obj \
	fontcm22.obj \
	fontcm22b.obj \
	fontcm22i.obj \
	fontcm24.obj \
	fontcm24b.obj \
	fontcm24i.obj \
	fontcm26.obj \
	fontcm26b.obj \
	fontcm26i.obj \
	fontcm28.obj \
	fontcm28b.obj \
	fontcm28i.obj \
	fontcm30.obj \
	fontcm30b.obj \
	fontcm30i.obj \
	fontcm32.obj \
	fontcm32b.obj \
	fontcm32i.obj \
	fontcm34.obj \
	fontcm34b.obj \
	fontcm34i.obj \
	fontcm36.obj \
	fontcm36b.obj \
	fontcm36i.obj \
	fontcm38.obj \
	fontcm38b.obj \
	fontcm38i.obj \
	fontcm40.obj \
	fontcm40b.obj \
	fontcm40i.obj \
	fontcm42.obj \
	fontcm42b.obj \
	fontcm42i.obj \
	fontcm44.obj \
	fontcm44b.obj \
	fontcm44i.obj \
	fontcm46.obj \
	fontcm46b.obj \
	fontcm46i.obj \
	fontcm48.obj \
	fontcm48b.obj \
	fontcm48i.obj \
	fontcmsc12.obj \
	fontcmsc14.obj \
	fontcmsc16.obj \
	fontcmsc18.obj \
	fontcmsc20.obj \
	fontcmsc22.obj \
	fontcmsc24.obj \
	fontcmsc26.obj \
	fontcmsc28.obj \
	fontcmsc30.obj \
	fontcmsc32.obj \
	fontcmsc34.obj \
	fontcmsc36.obj \
	fontcmsc38.obj \
	fontcmsc40.obj \
	fontcmsc42.obj \
	fontcmsc44.obj \
	fontcmsc46.obj \
	fontcmsc48.obj \
	fontcmss12.obj \
	fontcmss12b.obj \
	fontcmss12i.obj \
	fontcmss14.obj \
	fontcmss14b.obj \
	fontcmss14i.obj \
	fontcmss16.obj \
	fontcmss16b.obj \
	fontcmss16i.obj \
	fontcmss18.obj \
	fontcmss18b.obj \
	fontcmss18i.obj \
	fontcmss20.obj \
	fontcmss20b.obj \
	fontcmss20i.obj \
	fontcmss22.obj \
	fontcmss22b.obj \
	fontcmss22i.obj \
	fontcmss24.obj \
	fontcmss24b.obj \
	fontcmss24i.obj \
	fontcmss26.obj \
	fontcmss26b.obj \
	fontcmss26i.obj \
	fontcmss28.obj \
	fontcmss28b.obj \
	fontcmss28i.obj \
	fontcmss30.obj \
	fontcmss30b.obj \
	fontcmss30i.obj \
	fontcmss32.obj \
	fontcmss32b.obj \
	fontcmss32i.obj \
	fontcmss34.obj \
	fontcmss34b.obj \
	fontcmss34i.obj \
	fontcmss36.obj \
	fontcmss36b.obj \
	fontcmss36i.obj \
	fontcmss38.obj \
	fontcmss38b.obj \
	fontcmss38i.obj \
	fontcmss40.obj \
	fontcmss40b.obj \
	fontcmss40i.obj \
	fontcmss42.obj \
	fontcmss42b.obj \
	fontcmss42i.obj \
	fontcmss44.obj \
	fontcmss44b.obj \
	fontcmss44i.obj \
	fontcmss46.obj \
	fontcmss46b.obj \
	fontcmss46i.obj \
	fontcmss48.obj \
	fontcmss48b.obj \
	fontcmss48i.obj \
	fontcmtt12.obj \
	fontcmtt14.obj \
	fontcmtt16.obj \
	fontcmtt18.obj \
	fontcmtt20.obj \
	fontcmtt22.obj \
	fontcmtt24.obj \
	fontcmtt26.obj \
	fontcmtt28.obj \
	fontcmtt30.obj \
	fontcmtt32.obj \
	fontcmtt34.obj \
	fontcmtt36.obj \
	fontcmtt38.obj \
	fontcmtt40.obj \
	fontcmtt42.obj \
	fontcmtt44.obj \
	fontcmtt46.obj \
	fontcmtt48.obj \
	fontfixed6x8.obj \
	image.obj \
	imageButton.obj \
	line.obj \
	radioButton.obj \
	rectangle.obj \
	string.obj

INCS = -I$(SIMPLELINK_SDK_INSTALL_DIR)/source

CCS_LIB_DIRS = $(wildcard lib/ccs/*)
GCC_LIB_DIRS = $(wildcard lib/gcc/*)
IAR_LIB_DIRS = $(wildcard lib/iar/*)

LIB_DIRS =
ifneq ($(CCS_ARMCOMPILER),)
	LIB_DIRS += $(CCS_LIB_DIRS)
endif

ifneq ($(GCC_ARMCOMPILER),)
	LIB_DIRS += $(GCC_LIB_DIRS)
endif

ifneq ($(IAR_ARMCOMPILER),)
	LIB_DIRS += $(IAR_LIB_DIRS)
endif

export INCS
export OBJS

.PHONY: build-libs $(LIB_DIRS)

# BUILD GOALS
all: build-libs

build-libs: $(LIB_DIRS)

$(LIB_DIRS):
	@ $(MAKE) -C $@


CLEAN_DIRS = $(addprefix clean-,$(LIB_DIRS))

.PHONY: clean-libs $(CLEAN_DIRS)

# CLEAN GOALS
clean: clean-libs

clean-libs: $(CLEAN_DIRS)

$(CLEAN_DIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
