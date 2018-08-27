#ifndef	__PRINT_H__
#define __PRINT_H__

//#include "define_all.h"  


/*	
	0 = 关闭打印
	1 = 普通打印
	2 = 带文件名，行号打印	
*/	
#define __DEBUG_PRINT__ 1


#define PRINT_UART		UART1
#define PRINT_BAUD		9600
#define PRINT_DATABIT	Eight8Bit
#define PRINT_PARITY	NONE
#define PRINT_STOPBIT	OneBit


#if( __DEBUG_PRINT__ == 0 )
	#define dbgPrintf(format,...)
	
#elif( __DEBUG_PRINT__ == 1 )
	#define dbgPrintf(format,...) printf(format"\r\n", ##__VA_ARGS__)  
	
#elif( __DEBUG_PRINT__ == 2 )
	#define dbgPrintf(format,...) printf("File: "__FILE__", Line: %05d: "format"\r\n", __LINE__, ##__VA_ARGS__)  
	
#else  
	#define dbgPrintf(format,...)
	
#endif  


#define LPTIMER_TC	( * ( ( volatile uint32_t * )0x40013404 ) )	//LPTIM->LPTCNT 
	
extern void Init_PrintUartx(void);
extern void Init_lptimer_for_runtime_stats(void);

#endif
