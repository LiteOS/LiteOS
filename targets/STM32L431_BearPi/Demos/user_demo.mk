################################################################################
# this is used for compile the user defined oc demo
################################################################################

#if you have defined multiple demos by yourself, modify this file to compile the chosen demo.


#example for lwm2m
ifeq ($(CONFIG_USER_DEMO), "oc_streetlight_template")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/STM32L431_BearPi/Demos/oc_streetlight_template/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/STM32L431_BearPi/Demos/oc_streetlight_template
	user_demo_defs = -D CONFIG_OC_LWM2M_DEMO_ENABLE=1
	
endif

#example for hello world	 
ifeq ($(CONFIG_USER_DEMO), "hello_world_demo")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/STM32L431_BearPi/Demos/hello_world_demo/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/STM32L431_BearPi/Demos/hello_world_demo
	user_demo_defs = -D CONFIG_HELLO_WORLD_ENABLE=1
	
endif

#example for cloud map 
ifeq ($(CONFIG_USER_DEMO), "oc_cloud_map_demo")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/STM32L431_BearPi/Demos/oc_cloud_map_demo/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/STM32L431_BearPi/Demos/oc_cloud_map_demo
	user_demo_defs = -D CONFIG_OC_LWM2M_CLOUD_MAP_ENABLE=1
	
endif

C_SOURCES += $(user_demo_src)
C_INCLUDES += $(user_demo_inc)
C_DEFS += $(user_demo_defs)