
TOP_DIR := .

SUB_DIR := \
	$(TOP_DIR)/components/connectivity/agent_tiny/firmware_update \
	$(TOP_DIR)/components/connectivity/agent_tiny/mqtt_client \
	$(TOP_DIR)/components/connectivity/agent_tiny/osadapter \
	$(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client \
	$(TOP_DIR)/components/connectivity/at_frame \
	$(TOP_DIR)/components/connectivity/sota \
	$(TOP_DIR)/components/connectivity/nb_iot \
	$(TOP_DIR)/components/connectivity/paho.mqtt.embedded-c-1.1.0/MQTTClient-C \
	$(TOP_DIR)/drivers/devices/gprs \
	$(TOP_DIR)/drivers/devices/wifi/esp8266 \
	$(TOP_DIR)/drivers/devices/nb \
	$(TOP_DIR)/components/fota/ota \
	$(TOP_DIR)/components/fota/utility \
	$(TOP_DIR)/components/fota/fota_package_storage_device
#	$(TOP_DIR)/drivers/devices/wifi/esp8266_anylink_spi \
#	$(TOP_DIR)/drivers/devices/wifi/mavell \



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
	genhtml -o build/html $(GCOV_INFOS) --rc genhtml_branch_coverage=1
	$(foreach N,$(SUB_DIR), cp $(N)/Result.htm build/result/Result_$(notdir $(N)).htm;)
	
memcheck:
	$(foreach N,$(SUB_DIR),make -C $(N) memcheck;)
