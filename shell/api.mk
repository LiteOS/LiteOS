# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

ifeq ($(LOSCFG_SHELL), y)
    ifeq ($(LOSCFG_SHELL_FULL_CAP), y)
        LITEOS_SHELL_INCLUDE  += -I $(LITEOSTOPDIR)/shell/full/include
    endif
endif
