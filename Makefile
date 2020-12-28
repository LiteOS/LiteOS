LITEOSTOPDIR = $(CURDIR)

export LITEOSTOPDIR

-include $(LITEOSTOPDIR)/config.mk

RM = rm -rf
MAKE = make
__LIBS = libs

LITEOS_TARGET = Huawei_LiteOS
LITEOS_LIBS_TARGET = libs_target

.PHONY: all lib clean cleanall $(LITEOS_TARGET) debug release help

all: $(OUT) $(BUILD) $(LITEOS_TARGET)
lib: $(OUT) $(BUILD) $(LITEOS_LIBS_TARGET)

debug:
	$(HIDE)echo "=============== make a debug version  ==============="
	$(HIDE) $(MAKE) all

release:
ifeq ($(PLATFORM),)
	$(HIDE)echo "=============== make a release version for platform $(PLATFORM) ==============="
	$(HIDE)$(SCRIPTS_PATH)/mklibversion.sh $(PLATFORM)
else
	$(HIDE)echo "================make a release version for all platform ==============="
	$(HIDE)$(SCRIPTS_PATH)/mklibversion.sh
endif

#-----need move when make version-----#
##### make lib #####
$(__LIBS): $(OUT) $(CXX_INCLUDE)
$(LITEOS_TARGET): $(__LIBS) LITEOS_BUILD

$(OUT): $(LITEOS_MENUCONFIG_H)
	$(HIDE)mkdir -p $(OUT)/lib

$(BUILD):
	$(HIDE)mkdir -p $(BUILD)

$(LITEOS_LIBS_TARGET): $(__LIBS)
	$(HIDE)for dir in $(LIB_SUBDIRS); \
		do $(MAKE) -C $$dir all || exit 1; \
	done
	$(HIDE)echo "=============== make lib done  ==============="

include tools/menuconfig/Makefile.kconfig

$(LITEOS_MENUCONFIG_H):
ifneq ($(LITEOS_PLATFORM_MENUCONFIG_H), $(wildcard $(LITEOS_PLATFORM_MENUCONFIG_H)))
	$(HIDE)+make savemenuconfig
endif

LITEOS_BUILD: $(LITEOS_MENUCONFIG_H)

LITEOS_BUILD:
	$(HIDE)echo $(LOSCFG_ENTRY_SRC)

	$(HIDE)for dir in $(LITEOS_SUBDIRS); \
	do $(MAKE) -C $$dir all || \
	if [ "$$?" != "0" ]; then \
	echo "########################################################################################################"; \
	echo "########                      LiteOS build failed!                                              ########"; \
	echo "########################################################################################################"; \
	exit 1; \
	fi;\
	done

$(LITEOS_TARGET):
ifeq ($(OS), Linux)
	$(call update_from_baselib_file)
endif
	$(LD) $(LITEOS_LDFLAGS) $(LITEOS_TABLES_LDFLAGS) $(LITEOS_DYNLDFLAGS) -Map=$(OUT)/$@.map -o $(OUT)/$@.elf --start-group $(LITEOS_BASELIB) --end-group
	$(OBJCOPY) -O binary $(OUT)/$@.elf $(OUT)/$@.bin
	$(OBJDUMP) -t $(OUT)/$@.elf |sort >$(OUT)/$@.sym.sorted
	$(OBJDUMP) -d $(OUT)/$@.elf >$(OUT)/$@.asm
	$(SIZE) $(OUT)/$@.elf
	$(HIDE)echo "########################################################################################################"
	$(HIDE)echo "########                      LiteOS build successfully!                                        ########"
	$(HIDE)echo "########################################################################################################"

clean:
	$(HIDE)for dir in $(LITEOS_SUBDIRS); \
		do make -C $$dir clean|| exit 1; \
	done
	$(HIDE)$(RM) $(__OBJS) $(LITEOS_TARGET) $(OUT) $(BUILD) $(LITEOS_MENUCONFIG_H) *.bak *~
	$(HIDE)$(RM) -rf $(LITEOS_PLATFORM_MENUCONFIG_H)
	$(HIDE)echo "clean $(LITEOS_PLATFORM) finish"

cleanall:
	$(HIDE)rm -rf $(LITEOSTOPDIR)/out
	$(HIDE)echo "clean all"
