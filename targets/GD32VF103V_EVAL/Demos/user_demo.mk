################################################################################
# this is used for compile the user defined oc demo
################################################################################

#if you have defined multiple demos by yourself, modify this file to compile the chosen demo.

#example for hello world	 
ifeq ($(CONFIG_USER_DEMO), "hello_world_demo")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/hello_world_demo/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/hello_world_demo
	user_demo_defs = -D CONFIG_HELLO_WORLD_ENABLE=1
	
endif

#example for oceanlink
ifeq ($(CONFIG_USER_DEMO), "oc_oceanlink_demo")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/oc_oceanlink_demo/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/oc_oceanlink_demo
	user_demo_defs = -D CONFIG_OC_OCEANLINK_ENABLE=1
	
endif

#example for oc develop
ifeq ($(CONFIG_USER_DEMO), "oc_develop_demo")	
	user_demo_src  = ${wildcard $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/oc_develop_demo/*.c}
	user_demo_inc = -I $(TOP_DIR)/targets/GD32VF103V_EVAL/Demos/oc_develop_demo
	user_demo_defs = -D CONFIG_OC_DEVELOP_ENABLE=1
	
endif

C_SOURCES += $(user_demo_src)
C_INCLUDES += $(user_demo_inc)
C_DEFS += $(user_demo_defs)
