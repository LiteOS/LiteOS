#include "los_bsp_key.h"
#include "samd21.h"
#include <stdio.h>
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/




/*****************************************************************************
 Function    : LOS_EvbKeyInit
 Description : Init GIOP Key
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbKeyInit(void)
{   
    //下面是设置数字口10，也就是PORTA[18]为中断口， 对应EIC[2]，在上升沿触发
    PM->APBAMASK.reg |= 0x01<<6;         // EIC APB Clock Enable
    GCLK->CLKCTRL.reg = (0x1<<14) |0x5;  // Generic Clock EIC enable
  REG_EIC_INTENSET |= 0x0004;            // EXTINT2 Enable
    REG_EIC_CONFIG0 |= 0x00000900;       // EXTINT2是上升沿触发中断
    PORT->Group[0].PINCFG[18].reg = 0x1; // PORTA[18]复用使能
    PORT->Group[0].PMUX[9].reg = 0x00;   // PORTA[18]的复用设置是A，也就是EIC[2]
    REG_EIC_CTRL = 0x02;                 // EIC Enable
    (*(RwReg  *)0xE000E100) = 0x0010;    // NVIC Controller, Enable EIC
    return;
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
    
    return KeyVal;
}

/*****************************************************************************
 Function    : EXTIxxx_IRQHandler
 Description : EXT xxx IRQ Handler detail,change the function name 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void EXTInt2_IRQHandler(void)
{
    //add you code here.
    REG_EIC_INTFLAG = 0x0004; // Clear EIC[2]
    printf("The Key is preesed\n");
    return;
}



