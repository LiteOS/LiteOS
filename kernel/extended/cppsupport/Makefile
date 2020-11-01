# A Makefile for cppsupport

include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/kernel/base/include -I $(LITEOSTOPDIR)/kernel/extended/include

LOCAL_FLAGS := $(LITEOS_CFLAGS_INTERWORK) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
