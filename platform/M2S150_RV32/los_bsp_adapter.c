/*
    here we can include some standard header file
*/
#include <stdio.h>
#include <string.h>
/*
    here include some special hearder file you need
*/
//#include "cmsis_os.h"

#include "los_bsp_adapter.h"
#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

#ifdef LOS_M2S150_RV32
#include "riscv_hal.h"
#endif

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
/* current system default HZ , should be set according to the microchip */
const unsigned int sys_clk_freq = 83000000;

/* tick count per second , don't change this */
const unsigned int tick_per_second = 1000;
static unsigned int g_ucycle_per_tick = 0;

/*
    if g_use_ram_vect == 1, we should use sct file STM32F429I-LiteOS.sct
    and we can use LOS_HwiCreate(), LOS_HwiDelete() dynamically regist a irq func
    if g_use_ram_vect == 0, we use default vector table in rom start at address 0x00000000
*/
const unsigned char g_use_ram_vect = 0;

typedef struct
{
	uint32_t val_low;
	uint32_t val_high;
}riscv_machine_timer_t;

static volatile riscv_machine_timer_t *mtime = (riscv_machine_timer_t *)0x4400BFF8;

static volatile riscv_machine_timer_t *mtimecmp = (riscv_machine_timer_t *)0x44004000;
/*****************************************************************************
    LOS function extern
 *****************************************************************************/
extern void LOS_SetTickSycle(unsigned int);
extern void LOS_TickHandler(void);
extern unsigned int osGetVectorAddr(void);

/*-----------------------------------------------------------*/

/* Sets the next timer interrupt
 * Reads previous timer compare register, and adds tickrate */
static void prvSetNextTimerInterrupt(void)
{
	uint64_t time;

	time = mtime->val_low;
	time |= ((uint64_t)mtime->val_high << 32);

	time += g_ucycle_per_tick;

	mtimecmp->val_low = (uint32_t)(time & 0xFFFFFFFF);
	mtimecmp->val_high = (uint32_t)((time >> 32) & 0xFFFFFFFF);

	/* Enable timer interrupt */
	__asm volatile("csrs mie,%0"::"r"(0x80));
}
/*****************************************************************************
 Function    : osTickStart
 Description : Set system tick reqister, and start system tick exception
              Note that This function is called by LOS_Start() In LiteOS kernel.
              so, please don't init system tick before LiteOS kernel Inited
              in your own system.
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
        Set system tick relaod register valude, current register valude and start
        system tick exception.
        Note: here can be replaced by some function , for example in Stm32 bsp
        you can just call SysTick_Config(sys_clk_freq/tick_per_second);
    */
#ifdef LOS_M2S150_RV32
    uwRet = (unsigned int)SysTick_Config(g_ucycle_per_tick);
#endif

    return uwRet;
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
    
    return;
}

/*****************************************************************************
 Function    : LosAdapIntInit
 Description : config vector table in ram. please see g_use_ram_vect illustration
               this is used for arm cortex-M3/M4
 Input       : None
 Output      : None
 Return      : LOS_OK
 *****************************************************************************/
void LosAdapIntInit(void)
{
    /*Nothing for RISC-V as of now*/
    return;
}

/*****************************************************************************
 Function    : LosAdapIrpEnable
 Description : external interrupt enable, and set priority
               this function is called by LOS_HwiCreate(),
               so here can use bsp func to inplemente it 
               Note : if don't use LOS_HwiCreate(), leave it empty
 Input       : irqnum: external interrupt number
               prior: priority of this interrupt
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrpEnable(unsigned int irqnum, unsigned short prior)
{
    /*
        enable irq 
    */
    PLIC_EnableIRQ((IRQn_Type)irqnum);
    /*
        set irq priority , 
    */
    PLIC_SetPriority((IRQn_Type)irqnum, 1);     //only on priority for RISC-V
    return;
}

/*****************************************************************************
 Function    : LosAdapIrqDisable
 Description : external interrupt disable
               this function is called by LOS_HwiDelete(), so use bsp func
               to inplemente it
               Note : if don't use LOS_HwiDelete(), leave it empty
 Input       : irqnum: external interrupt number
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqDisable(unsigned int irqnum)
{
    PLIC_DisableIRQ((IRQn_Type) irqnum);
}

/*****************************************************************************
 Function    : LOS_EvbSetup
 Description : Init device on the board
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbSetup(void)
{
    LOS_EvbUartInit();
    LOS_EvbLedInit();
    LOS_EvbKeyInit();
    return;
}

/*****************************************************************************
 Function    : LOS_EvbTrace
 Description : Bsp printf function
 Input       : const char *str
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbTrace(const char *str)
{
    LOS_EvbUartWriteStr(str);
    return;
}
