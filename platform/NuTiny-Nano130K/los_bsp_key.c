#include "los_bsp_key.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "Nano100Series.h"
#include "gpio.h"



/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init GIOP Key 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{
    //add you code here.
    GPIO_SetMode(PC, BIT12, GPIO_PMD_INPUT);
    
    return ;
}

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    unsigned int KeyVal = 0xFFFFFFFF;
    
    //add you code here.
    //通过这个函数检测按键按下
//	   while(1){
//	      printf("0x%x\n",PC-> PIN);
//			
//		 }
	   KeyVal = (PC-> PIN)&BIT12;
    return KeyVal;//返回0表示按下
}

/*****************************************************************************
 Function    : EXTIxxx_IRQHandler
 Description : EXT xxx IRQ Handler detail,change the function name 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTIxxx_IRQHandler(void)
{
    //add you code here.
    
    return;
}



