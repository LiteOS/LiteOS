#ifndef __CSKY_DEMO_ALL_H_
#define __CSKY_DEMO_ALL_H_


int task_Timer(void);
int task_SPI_EEPROM(void);
int task_AES(void);
int task_WatchDog(void);
int task_RTC(void);
int task_DMA(void);
int task_EFLASH(void);
int task_Usart(void);
int task_PWM(void);
int task_RSA(void);
int task_SHA(void);
int task_CRC(void);
int task_TRNG(void);
int task_GPIO(void);
int task_IIC(void);
int task_IIS(void);
int task_ADC(void);
#define TRACE_CMH_1 (printf("%s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
#define HopeRun_printf (printf("\n\n[%s]--->[%d]",__FUNCTION__, __LINE__ ), printf) 
#define SHOW_PRECOMPILE_INFO	0	//用于屏蔽和显示预编译时的提示信息
#endif