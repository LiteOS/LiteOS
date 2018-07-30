
TOP_DIR := .

SUB_DIR := \
	$(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client \
	$(TOP_DIR)/components/connectivity/at_frame \
	$(TOP_DIR)/components/connectivity/nb_iot \
	$(TOP_DIR)/drivers/devices/gprs \
	$(TOP_DIR)/drivers/devices/wifi/esp8266 \
	$(TOP_DIR)/drivers/devices/wifi/esp8266_anylink_spi \
	$(TOP_DIR)/drivers/devices/nb \
#	$(TOP_DIR)/components/connectivity/fota/ota \
#	$(TOP_DIR)/components/connectivity/fota/utility \
#	$(TOP_DIR)/components/connectivity/fota/fota_package_storage_device \

GCOV_INFO_SUFFIX := /gcov/gcov.info 
	
GCOV_INFOS := $(addsuffix $(GCOV_INFO_SUFFIX), $(SUB_DIR))
	
all:
	$(foreach N,$(SUB_DIR),make -C $(N);)
	
clean:
	$(foreach N,$(SUB_DIR),make -C $(N) clean;)
	rm -rf gcov

run:
	$(foreach N,$(SUB_DIR),make -C $(N) run;)
	
cov:
	$(foreach N,$(SUB_DIR),make -C $(N) cov;)
	mkdir -p gcov
	genhtml -o gcov/html $(GCOV_INFOS) --rc genhtml_branch_coverage=1
	
memcheck:
	$(foreach N,$(SUB_DIR),make -C $(N) memcheck;)