# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

# Since LiteOS needs to link every single libs, and the build system needs to record
# every target names. Meanwhile $(LITEOS_LIBDEP) also needs to be updated after the build,
# $(LITEOS_LIBDEP) is exported afterwards.
BASELIB_FILE = $(OUT)/lib/libs.list
LIBDEP_FILE  = $(OUT)/lib/libdep.mk

define add_lib_to_baselib_file
    $(if $(1), @echo -l$(1) >> $(BASELIB_FILE))
endef

define update_from_baselib_file
    $(eval LITEOS_BASELIB += $(shell cat $(BASELIB_FILE)))
    $(HIDE) $(RM) $(BASELIB_FILE)
    $(HIDE) echo "LITEOS_LIBDEP := " $(LITEOS_BASELIB) > $(LIBDEP_FILE)
endef