# Copyright (c) 2013 The F9 Microkernel Project. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

objs := $(all-y)
deps := $(objs:%.o=%.o.d)
build-utils := $(foreach u,$(build-util-bin),$(out)/util/$(u))
host-utils := $(foreach u,$(host-util-bin),$(out)/util/$(u))

# Create output directories if necessary
_dir_create := $(foreach d,$(dirs),$(shell [ -d $(out)/$(d) ] || \
	    mkdir -p $(out)/$(d)))
_dir_y_create := $(foreach d,$(dirs-y),$(shell [ -d $(out)/$(d) ] || \
	    mkdir -p $(out)/$(d)))

symmap_obj-list = $(symmap_obj-list-y)

# Decrease verbosity unless you pass V=1
quiet = $(if $(V),,@echo '  $(2)' $(subst $(out)/,,$@) ; )$(cmd_$(1))
silent = $(if $(V),,1>/dev/null)

# commands to build all targets
cmd_obj_to_bin = $(OBJCOPY) -O binary $< $@
cmd_elf_to_list = $(OBJDUMP) -S $< > $@
cmd_elf = $(LD) $(LDFLAGS) $(objs) -o $@ \
	-L platform -T $(CHIP)/$(LITEOS_LD_FILE) $(LIBGCC)
cmd_strip = $(STRIP) $< -o $@
cmd_elf_relink = $(LD) $(LDFLAGS) $(objs) $(symmap_obj-list) -o $@ \
	-L platform -T $(CHIP)/$(LITEOS_LD_FILE) $(LIBGCC) \
	-Map $(out)/$*.map
cmd_c_to_o = $(CC) $(CFLAGS) -c $< -o $@
cmd_c_to_o_user = $(CC) $(CFLAGS_INCLUDE_USER) $(CFLAGS) $(CFLAGS_USER) -MMD -MF $@.d -c $< -o $@
cmd_c_to_build = $(BUILDCC) $(BUILD_CFLAGS) $(BUILD_LDFLAGS) \
	        			$< -o $@
cmd_bin = cat $^ > $@

# commands to build Kconfig
KCONFIG := external/kconfig
MCONF_INPUT = $(out_host)/Config.in
cmd_kconfig_prepare = \
	mkdir -p $(out_host) $(out_host)/lxdialog && \
	$(call init_mconf, $(MCONF_INPUT))
cmd_kconfig = $(MAKE) --no-print-directory -C $(KCONFIG) -f Makefile.f9 mconf \
		obj=$(shell pwd)/$(out_host) \
		CC="$(BUILDCC)" HOSTCC="$(BUILDCC)"
cmd_mconf = $(out_host)/mconf $(MCONF_INPUT)

.PHONY: bare
bare: $(out)/$(PROJECT).bin

test:
	@echo $(deps)

$(out)/%.bin: $(out)/%.elf.bin $(bin-list)
	$(call quiet,bin,CAT    )

$(out)/%.elf.bin: $(out)/%.elf
	$(call quiet,obj_to_bin,OBJCOPY)

$(out)/%.list: $(out)/%.elf
	$(call quiet,elf_to_list,OBJDUMP)

$(out)/%.elf: $(out)/liteos_nosym.elf $(symmap_obj-list)
	$(call quiet,elf_relink,LD     )
	$(SIZE) $(out)/liteos.elf

$(out)/liteos_nosym.elf: $(objs)
	$(call quiet,elf,LD     )

$(out)/user/%.o:user/%.c
	$(call quiet,c_to_o_user,CC     )

$(out)/%.o:%.c
	$(call quiet,c_to_o,CC     )

$(out)/%.o:%.S
	$(call quiet,c_to_o,AS     )

$(build-utils): $(out)/%:%.c
	$(call quiet,c_to_build,BUILDCC)

.PHONY: flash
flash: $(out)/$(PROJECT).bin
	$(call platform-flash-command, $<)

.PHONY: clean
clean:
	-rm -rf $(out)

.PHONY: distclean
distclean: clean
	-rm -rf $(out_host)
	-rm -f $(CONFIG) $(CONFIG).old include/autoconf.h

# FIXME: validate the target machine and check its availability
.PHONY: qemu
qemu: $(out)/$(PROJECT).bin
	-killall -q qemu-system-arm
	$(QEMU_DIR)qemu-system-arm -M stm32-p103 -kernel $(out)/$(PROJECT).bin -serial stdio -semihosting

$(out_host)/mconf:
	$(call quiet,kconfig,CONFIG )
$(MCONF_INPUT): $(KCONFIG_FILES)
	$(call quiet,kconfig_prepare,PREPARE)

.PHONY: config
config: $(MCONF_INPUT) $(out_host)/mconf
	$(call quiet,mconf,EVAL   )

.SECONDARY:

-include $(deps)
