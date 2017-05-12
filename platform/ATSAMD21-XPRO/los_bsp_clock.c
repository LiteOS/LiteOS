#include <stdio.h>
#include "samd21.h"

#include "los_bsp_clock.h"

/*************************************************************************************************
 *  功能：设置OSC8M为系统时钟源，并且设置分频系数为1                                             *
 *  参数：                                                               *
 *  返回：                                                                                       *
 *  说明：系统复位后, 默认内部OSC8M为系统时钟源，但分频系统为8，因此系统默认为1MHz               *
 *************************************************************************************************/
void _system_clock_source_setting(void) {
    uint32_t reg = 0;
    
    reg |= SYSCTRL_OSC8M_PRESC_0;
    reg |= SYSCTRL_OSC8M_ONDEMAND;
    reg |= SYSCTRL_OSC8M_ENABLE;
    
    SYSCTRL->OSC8M.reg = reg;
}

/*************************************************************************************************
 *  功能：DFLL参数设置                                                                           *
 *  参数：                                                                                       *
 *  返回：                                                                                       *
 *  说明：参数设置，设置倍频参数为6，以产生48M的系统时钟                                         *
 *************************************************************************************************/
void _system_clock_dfll_setting(void)
{
    uint32_t reg = 0;
    
    /* step 1: DFLL CTRL Setting */
    reg &= ~SYSCTRL_DFLLCTRL_MODE;          /* The DFLL operates in open-loop operation */
    reg &= ~SYSCTRL_DFLLCTRL_ONDEMAND;      /* The oscillator is always on */
    reg &= ~SYSCTRL_DFLLCTRL_CCDIS;
    reg &= ~SYSCTRL_DFLLCTRL_QLDIS;
    reg &= ~SYSCTRL_DFLLCTRL_BPLCKC;
    reg &= ~SYSCTRL_DFLLCTRL_WAITLOCK;
    
    SYSCTRL->DFLLCTRL.reg = reg;
    
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY)) {
        /* Wait for DFLL sync */
    }

    /* step 2: DFLL MUL Setting */
    reg = 0;
    reg |= SYSCTRL_DFLLMUL_MUL(6);          /* 设置倍频系数为6, 使时钟为48MHz */
    SYSCTRL->DFLLMUL.reg = reg;
    
    /* step 3: DFLL VAL Setting */
    reg = 0;
#define NVM_DFLL_COARSE_POS    58 /* DFLL48M Coarse calibration value bit position.*/
#define NVM_DFLL_COARSE_SIZE   6  /* DFLL48M Coarse calibration value bit size.*/

    uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP4)
            + (NVM_DFLL_COARSE_POS / 32))
            >> (NVM_DFLL_COARSE_POS % 32))
            & ((1 << NVM_DFLL_COARSE_SIZE) - 1);
    /* In some revision chip, the coarse calibration value is not correct. */
    if (coarse == 0x3f) {
        coarse = 0x1f;
    }
    uint32_t fine = 512;
    reg = SYSCTRL_DFLLVAL_COARSE(coarse) |
        SYSCTRL_DFLLVAL_FINE(fine);
    SYSCTRL->DFLLVAL.reg = reg;
    
    /* Enable DFLL */
    SYSCTRL->DFLLCTRL.bit.ENABLE = 1;

    /* Write full configuration to DFLL control register */
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ONDEMAND;

    /* Wait state */
    NVMCTRL->CTRLB.bit.RWS = 3;
}

/*************************************************************************************************
 *  功能：GCLK GEN参数设置                                                                       *
 *  参数：                                                                                       *
 *  返回：                                                                                       *
 *  说明：参数设置                                                                               *
 *************************************************************************************************/
void _system_clock_gclkgen_setting(void) {
    uint32_t reg = 0;
    
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {
        /* Wait for synchronization */
    };

    /* Select the correct generator */
    GCLK->GENDIV.bit.ID = GCLK_CLKCTRL_GEN_GCLK0_Val;

    /* Write the new generator configuration */
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {
        /* Wait for synchronization */
    };
    reg = GCLK_GENDIV_ID(GCLK_CLKCTRL_GEN_GCLK0_Val) | GCLK_GENDIV_DIV(0);
    GCLK->GENDIV.reg = reg;

    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {
        /* Wait for synchronization */
    };
    reg = 0;
    reg |= GCLK_GENDIV_ID(GCLK_CLKCTRL_GEN_GCLK0_Val);
    reg &= ~GCLK_GENCTRL_DIVSEL;                /* The generic clock generator equals the clock source divided by GENDIV.DIV */
    reg |= GCLK_GENCTRL_SRC_DFLL48M;
    reg &= ~GCLK_GENCTRL_OE;
    reg &= ~GCLK_GENCTRL_IDC;
    reg &= ~GCLK_GENCTRL_OOV;
    reg &= ~GCLK_GENCTRL_RUNSTDBY;
    reg |= GCLK_GENCTRL_GENEN;
    GCLK->GENCTRL.reg = reg;
}

int SystemClockInit(void) {
    _system_clock_source_setting();
    _system_clock_dfll_setting();
    _system_clock_gclkgen_setting();
    
    return 0;
}
