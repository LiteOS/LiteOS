ifeq ($(OS), Linux)
OBJOUT := $(BUILD)$(dir $(subst $(LITEOSTOPDIR),,$(shell pwd)))$(MODULE_NAME)
LOCAL_PWD := $(shell pwd)
else
OUT = $(LITEOSTOPDIR)/out/$(LITEOS_PLATFORM)
BUILD = $(OUT)/obj
OBJOUT := $(BUILD)$(dir $(subst $(LITEOSTOPDIR),,$(CURDIR)))$(MODULE_NAME)
endif

LIBA := $(OUT)/lib/lib$(MODULE_NAME).a
LIB := $(LIBA)


# create a separate list of objects per source type

LOCAL_CSRCS     := $(filter %.c,$(LOCAL_SRCS))
LOCAL_CPPSRCS   := $(filter %.cpp,$(LOCAL_SRCS))
LOCAL_ASMSRCS   := $(filter %.s,$(LOCAL_SRCS))
LOCAL_ASMSRCS2  := $(filter %.S,$(LOCAL_SRCS))

LOCAL_COBJS    := $(patsubst %.c,$(OBJOUT)/%.o,$(LOCAL_CSRCS))
LOCAL_CPPOBJS  := $(patsubst %.cpp,$(OBJOUT)/%.o,$(LOCAL_CPPSRCS))
LOCAL_ASMOBJS  := $(patsubst %.s,$(OBJOUT)/%.o,$(LOCAL_ASMSRCS))
LOCAL_ASMOBJS2 := $(patsubst %.S,$(OBJOUT)/%.o,$(LOCAL_ASMSRCS2))

LOCAL_OBJS   := $(LOCAL_COBJS) $(LOCAL_CPPOBJS) $(LOCAL_ASMOBJS) $(LOCAL_ASMOBJS2)

LOCAL_CGCH   := $(patsubst %.h,%.h.gch,$(LOCAL_CHS))
LOCAL_CPPGCH := $(patsubst %.h,%.h.gch,$(LOCAL_CPPHS))

all : $(LIB)

SUB_MODULE_BUILD: $(MODULE_y)
	$(HIDE) for dir in $(MODULE_y); 		\
		do $(MAKE) -C $$dir all || exit 1; 	\
	done

$(LOCAL_COBJS): $(OBJOUT)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(LITEOS_CFLAGS) $(LOCAL_CFLAGS) -c $< -o $@

$(LOCAL_CPPOBJS): $(OBJOUT)/%.o: %.cpp
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(GPP) $(LOCAL_FLAGS) $(LITEOS_CXXFLAGS) $(LOCAL_CPPFLAGS) -c $< -o $@

$(LOCAL_ASMOBJS2): $(OBJOUT)/%.o: %.S
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LITEOS_CPU_TYPE), riscv)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(LITEOS_ASFLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(LITEOS_CFLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
endif

$(LOCAL_ASMOBJS): $(OBJOUT)/%.o: %.s
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(LITEOS_ASFLAGS) $(LOCAL_ASFLAGS) -c $< -o $@

$(LOCAL_CGCH): %.h.gch : %.h
	$(HIDE)$(CC) $(LOCAL_FLAGS)$(LITEOS_CFLAGS) $(LOCAL_CFLAGS) $> $^

$(LOCAL_CPPGCH): %.h.gch : %.h
	$(HIDE)$(GPP) $(LOCAL_FLAGS) $(LITEOS_CXXFLAGS) $(LOCAL_CPPFLAGS) -x c++-header $> $^

LOCAL_GCH := $(LOCAL_CGCH) $(LOCAL_CPPGCH)

$(LOCAL_OBJS): $(LOCAL_GCH)

$(LIBA): $(LOCAL_OBJS) SUB_MODULE_BUILD
	$(HIDE)$(OBJ_MKDIR)
	$(call add_lib_to_baselib_file,$(MODULE_NAME))

ifeq ($(OS), Linux)
	$(HIDE)$(AR) $(ARFLAGS) $@ $(LOCAL_OBJS)
else
ifeq ($(LOCAL_MODULES),)
	$(HIDE)$(AR) $(ARFLAGS) $@ $(LOCAL_OBJS)
else
	$(HIDE)for i in $(LOCAL_MODULES); do \
		pushd $(OBJOUT)/$$i 1>/dev/null; \
		$(AR) $(ARFLAGS) $@ *.o;\
		popd 1>/dev/null;\
	done
endif
endif

clean:
	$(HIDE)$(RM) $(LIB) $(OBJOUT) $(LOCAL_GCH) *.bak *~

.PHONY: all clean
