ifeq ($(LOSCFG_COMPONENTS_AI), y)
COMPONENTS_AI_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/ai/include
endif