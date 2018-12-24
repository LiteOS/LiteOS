
TOP_DIR := .

SUB_DIR := \
	$(TOP_DIR)/components/connectivity/agent_tiny/atiny_lwm2m \
    $(TOP_DIR)/components/connectivity/nb_iot \
    $(TOP_DIR)/components/ota/sota \
    $(TOP_DIR)/components/ota/utility \
    $(TOP_DIR)/osdepends/liteos \
    $(TOP_DIR)/components/net/at_device/gprs_sim900a \
    $(TOP_DIR)/components/net/at_device/nb_bc95 \
    $(TOP_DIR)/components/net/at_device/emtc_bg36 \
    $(TOP_DIR)/components/net/at_device/wifi_esp8266 \
    $(TOP_DIR)/components/net/sal \
    $(TOP_DIR)/components/ota/flag_operate \
    $(TOP_DIR)/components/log
    #$(TOP_DIR)/components/fs/fatfs \
    #$(TOP_DIR)/components/fs/spiffs \
    #$(TOP_DIR)/components/fs/vfs \
    #$(TOP_DIR)/components/lib/libc \


GCOV_INFO_SUFFIX := /gcov/gcov.info 
	
GCOV_INFOS := $(addsuffix $(GCOV_INFO_SUFFIX), $(SUB_DIR))
	
all:
	$(foreach N,$(SUB_DIR),make -C $(N);)
	
clean:
	$(foreach N,$(SUB_DIR),make -C $(N) clean;)
	rm -rf build

run:
	$(foreach N,$(SUB_DIR),make -C $(N) run;)
	
cov:
	$(foreach N,$(SUB_DIR),make -C $(N) cov;)
	mkdir -p build/result
	genhtml -o build/html $(GCOV_INFOS) # --rc genhtml_branch_coverage=1
	$(foreach N,$(SUB_DIR), cp $(N)/Result.htm build/result/Result_$(notdir $(N)).htm;)
	
memcheck:
	$(foreach N,$(SUB_DIR),make -C $(N) memcheck;)
