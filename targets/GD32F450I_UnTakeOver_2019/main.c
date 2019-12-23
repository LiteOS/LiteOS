#include "gd32f4xx.h"
#include "gd32f450i_eval.h"
#include "systick.h"
#include <stdio.h>
#include "los_sys.h "
#include "los_typedef.h"
#include "los_task.ph" 


UINT32 g_TskHandle;

void Hardware_Init(void);
void led_init(void);
UINT32 creat_task1();
UINT32 creat_task2();

int main()
{
	Hardware_Init();
	printf("\r\n USART printf example: please press the Tamper key \r\n");
	UINT32 uwRet = 0;
	uwRet = LOS_KernelInit();
	if (uwRet != LOS_OK)
	{
		return LOS_NOK;
	}
	uwRet = creat_task1();
	if (uwRet != LOS_OK)
	{
		return LOS_NOK;
	}
	uwRet = creat_task2();
	if (uwRet != LOS_OK)
	{
		return LOS_NOK;
	}
	LOS_Start();
	while(1)
	{
		gd_eval_led_on(LED1);
	}
}

void Hardware_Init(void)
{
		led_init();
    systick_config();
    gd_eval_com_init(EVAL_COM1);
}

void led_init(void)
{
    gd_eval_led_init(LED1);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
	
    return ch;
}


void task1(void)
{
	int count = 1;
	while(1)
	{
		printf("This is task1, count is %d\r\n", count++);
		LOS_TaskDelay(1000);
	}
}

UINT32 creat_task1()
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task1";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = 0x200;
	
	uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
	if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

void task2(void)
{
int count = 1;
while (1)
  {
        printf("This is task2,count is %d\r\n",count++);
        LOS_TaskDelay(2000);
  }
}
UINT32 creat_task2()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
 
    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "task2";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
    task_init_param.uwStackSize = 0x200;
 
    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
		{
        return uwRet;
		}
    return uwRet;
}