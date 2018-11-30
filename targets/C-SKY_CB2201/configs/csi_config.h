#ifndef __CSI_CONFIG_H__
#define __CSI_CONFIG_H__


//#define CONFIG_KERNEL_NONE 1  //表示无系统（裸奔），20181127

#define CONFIG_STARTUP_IRQ 1	//表示在startup.s中使用底层非接管中断（在startup.s文件中，无法使用target_config.h中的宏，不得已在此处定义一个控制接管的宏）20181128

#define CONFIG_CHIP_SL02 1

#define CONFIG_HAVE_VIC 1

#define CONFIG_ARCH_INTERRUPTSTACK 4096

#define CONFIG_NUTTXMM_NONE 1


#define CONFIG_SYSTEM_SECURE 1	//启动TEE安全模式，芯片进入安全模式,20181127

#define CONFIG_CHIP_CH2201 1	

//#define CONFIG_Audio_Codec 1 //本工程不存在Audio Codec音频编码器的驱动文件和驱动设备。不可打开该宏。20181129

#define CONFIG_YMC2HQFNR1A_CB2201 1//针对开发板 “CSKY 赤焰剑”，可以打开该宏（控制PWM中led引脚等）

#endif
