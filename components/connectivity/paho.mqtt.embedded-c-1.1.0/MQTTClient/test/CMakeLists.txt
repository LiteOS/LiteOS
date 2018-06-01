PROJECT(mqttcpp-tests)

SET(MQTT_TEST_BROKER_HOST "localhost" CACHE STRING "Hostname of a test MQTT broker to use")
SET(MQTT_TEST_PROXY_PORT "1884" CACHE STRING "Port of the test proxy to use")
SET(MQTT_SSL_HOSTNAME "localhost" CACHE STRING "Hostname of a test SSL MQTT broker to use")
SET(CERTDIR $ENV{TRAVIS_BUILD_DIR}/test/ssl)

ADD_EXECUTABLE(
	testcpp1
	test1.cpp
)

target_compile_definitions(testcpp1 PRIVATE MQTTCLIENT_QOS1=1 MQTTCLIENT_QOS2=1)
target_include_directories(testcpp1 PRIVATE "../src" "../src/linux")
target_link_libraries(testcpp1 MQTTPacketClient  MQTTPacketServer)

ADD_TEST(
	NAME testcpp1
	COMMAND "testcpp1" "--host" ${MQTT_TEST_BROKER_HOST}
)
