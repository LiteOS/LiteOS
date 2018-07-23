
#include <stdio.h>
#include "los_typedef.h"
#include "los_task.h"

#include "fsl_device_registers.h"
#include "fsl_usart.h"
#include "board.h"
#include "pin_mux.h"

#define BOARD_UART          USART0
#define BOARD_UART_BAUDRATE 115200
#define BOARD_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_Flexcomm0)

void uart_init(void)
{
    uint32_t srcFreq = 0;
    usart_config_t usart_config;
    USART_GetDefaultConfig(&usart_config);
    srcFreq = BOARD_UART_CLK_FREQ;
    usart_config.baudRate_Bps = BOARD_UART_BAUDRATE;
    USART_Init(BOARD_UART, &usart_config, srcFreq);
    return ;
}
int uart_read(unsigned char *buf,int len,int timeout)
{
    int ret = 0;
    ret = USART_ReadBlockingTimeout(BOARD_UART,buf,len,timeout);
    return ret;
}
int uart_write(unsigned char *buf,int len)
{
    USART_WriteBlocking(BOARD_UART,buf,len);
    return len;
}
void TaskDelay(int time) //unit:TICKS
{
    LOS_TaskDelay(time);
    return;
}

UINT32 creat_maintask(const char *taskname,TSK_ENTRY_FUNC fnTask,UINT32 stackisize,UINT32 uwArg,UINT16 prior)
{
    UINT32 uwRet = LOS_OK;
	UINT32  handle;
    TSK_INIT_PARAM_S task_init_param;
	
    task_init_param.uwArg = uwArg;
    task_init_param.usTaskPrio = prior;
    task_init_param.pcName =(char *) taskname;
    task_init_param.pfnTaskEntry = fnTask;
    task_init_param.uwStackSize = stackisize;
    uwRet = LOS_TaskCreate(&handle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}



int fputc(int ch, FILE *f)
{
    int ret = 0;
    ret =uart_write((unsigned char *)&ch,1);
    return ret;
}

int fgetc(FILE *f)
{
    char ch;
    //uart_read((unsigned char *)&ch,1,0xFFFFFF);
    return ch;
}
volatile int gRamSize = 0;
volatile int gRamMax = 0;

VOID osTaskMemUsedInc(UINT32 uwUsedSize)
{
    gRamSize += uwUsedSize;
    if(gRamSize > gRamMax)
    {
        gRamMax = gRamSize;
    }
}
VOID osTaskMemUsedDec(UINT32 uwUsedSize)
{
    gRamSize -= uwUsedSize;
}

