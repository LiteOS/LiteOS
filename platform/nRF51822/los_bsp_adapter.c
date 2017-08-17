/*
	here we can include some standard header file
*/
#include <stdio.h>
#include <string.h>
#include "los_bsp_adapter.h"
#include "los_bsp_key.h"
#include "los_bsp_led.h"
#include "los_bsp_uart.h"


/* while use bsp code to start system tick, don't use LOS header */
//#define INCLUDE_LOS_HEADER
#ifdef INCLUDE_LOS_HEADER
#include "los_tick.ph"
#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"
#endif

/******************************************************************************
	here include some special hearder file you need
******************************************************************************/
#ifdef BOARD_NRF51822
#include "app_timer.h"

const nrf_drv_timer_t TIMER_TICK = NRF_DRV_TIMER_INSTANCE(1);
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
const unsigned char g_use_ram_vect = 0;

/*****************************************************************************
	LOS function extern 
 *****************************************************************************/
extern void LOS_SetTickSycle(unsigned int);
extern void LOS_TickHandler(void);
extern unsigned int osGetVectorAddr(void);


#ifdef BOARD_NRF51822
/**
 * @brief Handler for timer events.
 */
void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE1:
					  LOS_TickHandler();
				    LOS_AppTickHandler();
            break;
        default:
            //Do nothing.
            break;
    }    
}
#endif

/**
 * @brief Function for main application entry.
 */
int time1_start(void)
{
#ifdef BOARD_NRF51822
    uint32_t time_ms = 1; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
    
    //Configure TIMER init.
    err_code = nrf_drv_timer_init(&TIMER_TICK, NULL, timer_event_handler);
    APP_ERROR_CHECK(err_code);
    
    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_TICK, time_ms);
    
    nrf_drv_timer_extended_compare(&TIMER_TICK, NRF_TIMER_CC_CHANNEL1, 
			time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_TICK);
	  
	  return 0;
#else
	while(1);
#endif
	 
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
	  time1_start();
	
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
	
    /*add your systick handler code here */
	  
    return ;
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
    /* 
        1:ReLoad vector table address at ram . if do nothing , vector table is
        located in rom 0x00000000
        2:set nvic irq priority group
  
        Note: here can be replaced by some function , for example in Stm32 bsp
        you can just call SCB->VTOR = osGetVectorAddr(); and
        NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
    */
    if (g_use_ram_vect)
    {
    }

    return ;
}

/*****************************************************************************
 Function    : LosAdaqIrpEnable
 Description : external interrupt enable, and set priority 
               this function is called by LOS_HwiCreate(), 
               so here can use bsp func to inplemente it 
               Note : if don't use LOS_HwiCreate(), leave it empty
 Input       : irqnum: external interrupt number
               prior: priority of this interrupt
 Output      : None
 Return      : None
 *****************************************************************************/
void LosAdapIrqEnable(unsigned int irqnum, unsigned short prior)
{
    /*
        enable irq , for example in stm32 bsp you can use 
        NVIC_EnableIRQ((IRQn_Type)irqnum);
    */
    //nvicSetIRQ(irqnum);
    /*
        set irq priority , for example in stm32 bsp you can use 
        NVIC_SetPriority((IRQn_Type)irqnum, prior);
    */
    return ;
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
    /* 
        disable irq, for example in stm32 bsp you can use 
        NVIC_DisableIRQ((IRQn_Type)irqnum);
    */
    return ;
}

 
/*****************************************************************************
 Function    : LOS_EvbSetup
 Description : enable the device on the dev baord
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbSetup(void)
{
	LOS_EvbUartInit();
	LOS_EvbLedInit();
	LOS_EvbKeyInit();
	return ;
}


void LOS_EvbTrace(const char *str)
{
	LOS_EvbUartWriteStr(str);
		
	return ;
}


