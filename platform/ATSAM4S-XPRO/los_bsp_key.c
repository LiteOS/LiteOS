#include "los_bsp_key.h"

#ifdef ATSAM4S_XPRO
#include "sam4s.h"
#endif

void LOS_EvbKeyInit(void) 
{
#ifdef ATSAM4S_XPRO 
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER =
  PIOA->PIO_IDR  =
  PIOA->PIO_ODR  =
  PIOA->PIO_PER  = (PIO_PA2);             /* Setup PA2 for user button SW0    */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
#endif
    return;
}

uint8_t LOS_EvbGetKeyVal(int KeyNum)
{
#ifdef ATSAM4S_XPRO 
    if (~(PIOA->PIO_PDSR) & (PIO_PA2))
    {
        return LOS_KEY_PRESS;
    }
    else
        return USER_KEY;
#else
    return LOS_GPIO_ERR;
#endif
}

