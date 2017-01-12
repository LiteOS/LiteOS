/*
	here we can include some standard header file
*/
#include <stdio.h>
#include <string.h>
/*
	here include some special hearder file you need
*/
//#include "cmsis_os.h"

/* while use bsp code to start system tick, don't use LOS header */
#define INCLUDE_LOS_HEADER
#ifdef INCLUDE_LOS_HEADER
#include "los_tick.ph"
#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"
#endif

/*****************************************************************************
	global var
 *****************************************************************************/
/* current system Freq , should be set according to the microchip */
const unsigned int sys_clk_freq = 16000000;
/* tick count per second , don't change this */
const unsigned int tick_per_second = 1000;
static unsigned int g_ucycle_per_tick = 0;

/*
	if g_use_ram_vect == 1, we should use sct file STM32F429I-LiteOS.sct
	and we can use LOS_HwiCreate(), LOS_HwiDelete() dynamically regist a irq func
	if g_use_ram_vect == 0, we use default vector table in rom start at address 0x00000000
*/
const unsigned char g_use_ram_vect = 1;

/*****************************************************************************
	LOS function extern 
 *****************************************************************************/
extern void LOS_SetTickSycle(unsigned int);
extern void LOS_TickHandler(void);
extern unsigned int osGetVectorAddr(void);



/*****************************************************************************
 Function    : osTickStart
 Description : Set system tick reqister, and start system tick exception
				Note that This function is called by LOS_Start() kernel.
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
unsigned int osTickStart(void)
{
    unsigned int uwRet = 0;
	
	/* This code section LOS need, so don't change it */
    g_ucycle_per_tick = sys_clk_freq / tick_per_second;
		LOS_SetTickSycle(g_ucycle_per_tick);
	
	/*
		config system tick register and enable system tick exception. 
		this should changed according to the microchip
	*/
#ifdef INCLUDE_LOS_HEADER
    *(volatile UINT32 *)OS_SYSTICK_RELOAD_REG = g_ucycle_per_tick - 1;
    *((volatile UINT8 *)OS_NVIC_EXCPRI_BASE + (((UINT32)(-1) & 0xF) - 4)) = ((7 << 4) & 0xff);
    *(volatile UINT32 *)OS_SYSTICK_CURRENT_REG = 0;
    *(volatile UINT32 *)OS_SYSTICK_CONTROL_REG = (1 << 2) | (1 << 1) | (1 << 0);
		return uwRet;
#else
    uwRet = SysTick_Config(sys_clk_freq/tick_per_second);
    if (uwRet == 1)
    {
        return LOS_ERRNO_TICK_PER_SEC_TOO_SMALL;
    }
		return 0;
#endif
}

/*****************************************************************************
 Function    : SysTick_Handler
 Description : system tick interrupt handler.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void SysTick_Handler(void)
{
	/* 
		LOS need call LOS_TickHandler() in SysTick_Handler, don't change it,
		otherwise, LiteOS will not work.
	*/
	LOS_TickHandler();
	
	/*add your code here */
	
	
	return ;
}

/*****************************************************************************
 Function    : LosAdapIntInit
 Description : config vector table in ram. please see g_use_ram_vect illustration
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
void LosAdapIntInit(void)
{
#if 1
	//this is LiteOS code, just for demo. please use bsp func 
	*(volatile UINT32 *)OS_NVIC_VTOR = osGetVectorAddr();
	*(volatile UINT32 *)OS_NVIC_AIRCR = (0x05FA0000 | OS_NVIC_AIRCR_PRIGROUP << 8);
#else
	SCB->VTOR = osGetVectorAddr();
	NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
#endif
	return ;
}

/*****************************************************************************
 Function    : LosAdapIrpEnable
 Description : external interrupt enable, or status , or others 
				this function is called by LOS_HwiCreate(), so use bsp func to inplemente
 Input       : irqnum: external interrupt number
				prior: priority of this interrupt
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrpEnable(unsigned int irqnum, unsigned short prior)
{
#if 1
    nvicSetIRQ(irqnum);

    nvicSetIrqPRI(irqnum, prior << 4);
#else
    NVIC_EnableIRQ((IRQn_Type)irqnum);

    NVIC_SetPriority((IRQn_Type)irqnum, prior);
#endif
	return ;
}

/*****************************************************************************
 Function    : LosAdapIrqDisable
 Description : external interrupt disable
				this function is called by LOS_HwiDelete(), so use bsp func to inplemente
 Input       : irqnum: external interrupt number
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqDisable(unsigned int irqnum)
{
#if 1
    nvicClrIRQ(irqnum);
#else
	NVIC_DisableIRQ((IRQn_Type)irqnum);
#endif
	return ;
}
