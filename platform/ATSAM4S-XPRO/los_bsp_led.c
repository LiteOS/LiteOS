#include "los_bsp_led.h"         

#ifdef ATSAM4S_XPRO
#include "sam4s.h"
#endif


void LOS_EvbLedInit(void) 
{
#ifdef ATSAM4S_XPRO
    PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

    PMC->PMC_PCER0 = (1UL << ID_PIOC);      /* enable PIOC clock                */

    PIOC->PIO_PER  =
    PIOC->PIO_OER  =
    PIOC->PIO_PUDR =
    PIOC->PIO_OWER = (PIO_PC23);            /* Setup PC23 for LEDs              */
    
    LOS_EvbLedControl(0, LED_OFF);

    PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
#endif
    return ;
}

void LOS_EvbLedControl(int index, int cmd)
{
#ifdef ATSAM4S_XPRO
    if (cmd == LED_ON)
    {
        PIOC->PIO_CODR = PIO_PC23; /*led1 on */
    }
    else
    {
        PIOC->PIO_SODR = PIO_PC23; /*led1 off */
    }
#endif
    return ;
}
