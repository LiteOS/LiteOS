#ifndef	__DEFINEALL_H__
#define __DEFINEALL_H__

//定义常量, 常数

//系统时钟默认使用RCHF
#define RCHFCLKCFG	8	//8, 16, 24, 32MHz

//define_all.h中RCHFCLKCFG控制系统时钟
#if( RCHFCLKCFG == 8 )//8.0MHz
#define clkmode   1
#define SYSCLKdef RCC_RCHFCON_FSEL_8MHZ//RCHF中心频率8MHz
#elif( RCHFCLKCFG == 16 )//16.0MHz
#define clkmode   2
#define SYSCLKdef RCC_RCHFCON_FSEL_16MHZ//RCHF中心频率16MHz
#elif( RCHFCLKCFG == 24 )//24.0MHz
#define clkmode   3
#define SYSCLKdef RCC_RCHFCON_FSEL_24MHZ//RCHF中心频率24MHz
#elif( RCHFCLKCFG == 32 )//32.0MHz
#define clkmode   4
#define SYSCLKdef RCC_RCHFCON_FSEL_32MHZ//RCHF中心频率32MHz
#endif

/*变量类型定义*/
typedef union
{
  unsigned char B08;
  struct
  {
    unsigned char bit0:1;
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
  }Bit;
}B08_Bit;
#define	uint08 uint8_t
#define	uint16 uint16_t
#define uint32 uint32_t
#define	int08 int8_t		
#define	int16 int16_t
#define	int32 int32_t

/*功能IO宏定义*/
#if 0 //suzhen
#define LED0_GPIO		GPIOB
#define LED0_PIN		GPIO_Pin_6
#endif 

#if 1 //suzhen
#define LED0_GPIO		GPIOA
#define LED0_PIN		GPIO_Pin_4
#endif


#define KEY0_GPIO		GPIOB
#define KEY0_PIN		GPIO_Pin_0

/*操作宏定义*/
#define LED0_ON			GPIO_ResetBits(LED0_GPIO, LED0_PIN)
#define LED0_OFF		GPIO_SetBits(LED0_GPIO, LED0_PIN)
#define LED0_TOG		GPIO_ToggleBits(LED0_GPIO, LED0_PIN)

#define KEY0_P 			(SET == GPIO_ReadInputDataBit(KEY0_GPIO, KEY0_PIN))
#define KEY0_N 			(RESET == GPIO_ReadInputDataBit(KEY0_GPIO, KEY0_PIN))

#define EA_OFF	((__get_PRIMASK()&0x00000001) == 0x00000001)
#define EA_ON	((__get_PRIMASK()&0x00000001) == 0x00000000)


/* GPIO配置函数参数宏定义 */
//IO输入口配置 
//type 0 = 普通 
//type 1 = 上拉
#define IN_NORMAL	0
#define IN_PULLUP	1

//IO输出口配置 
//type 0 = 普通 
//type 1 = OD
#define OUT_PUSHPULL	0
#define OUT_OPENDRAIN	1

//IO数字特殊功能口 
//type 0 = 普通 
//type 1 = OD (OD功能仅部分特殊功能支持)
//type 2 = 普通+上拉 
//type 3 = OD+上拉
#define ALTFUN_NORMAL			0
#define ALTFUN_OPENDRAIN		1
#define ALTFUN_PULLUP			2
#define ALTFUN_OPENDRAIN_PULLUP	3


/*include*/
#include "FM33A0XX.h"
#include "fm33a0xx_include_all.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bintohex.h"
#include "user_init.h"
#include "print.h"  //suzhen

//pzh - add liteos  
//2018.6.14

#include "los_base.h"  
#include "los_config.h"
#include "los_sys.h"
#include "los_typedef.h"
#include "los_task.ph"  
#include "los_inspect_entry.h"  
#include "los_api_dynamic_mem.h"
#include "los_api_event.h"  
#include "los_api_interrupt.h"
#include "los_api_list.h"
#include "los_api_msgqueue.h"
#include "los_api_mutex.h"
#include "los_api_sem.h"
#include "los_api_static_mem.h"
#include "los_api_systick.h"
#include "los_api_task.h"
#include "los_api_timer.h"

#endif



