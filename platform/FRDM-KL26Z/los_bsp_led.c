#include "los_bsp_led.h"

#ifdef FRDM_KL25Z
#include "MKL25Z4.h"
#endif

#ifdef FRDM_KL26Z
#include "MKL26Z4.h"
#endif

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)

void BSP_LED_Init(uint8_t led)
{
    SIM_SCGC5 |= SIM_SCGC5_LED1_MASK | SIM_SCGC5_LED2_MASK | SIM_SCGC5_LED3_MASK;
    
    switch(led)
    {
        case LED1: LED1_PORT_PCR  &=    ~(0x07<<8);
                   LED1_PORT_PCR  |=     0x01<<8;                //GPIO mode
                   LED1_GPIO_PDOR |= (1U << LED1_PIN);           //high
                   LED1_GPIO_PDDR |= (1U << LED1_PIN);           //output
            break;
        case LED2: LED2_PORT_PCR  &=    ~(0x07<<8);
                   LED2_PORT_PCR  |=     0x01<<8;                //GPIO mode
                   LED2_GPIO_PDOR |= (1U << LED2_PIN);           //high
                   LED2_GPIO_PDDR |= (1U << LED2_PIN);           //output
            break;
        case LED3: LED3_PORT_PCR  &=    ~(0x07<<8);
                   LED3_PORT_PCR  |=     0x01<<8;                //GPIO mode
                   LED3_GPIO_PDOR |= (1U << LED3_PIN);           //high
                   LED3_GPIO_PDDR |= (1U << LED3_PIN);           //output
            break;
        default:
            break;
    }
}


void BSP_LED_On(uint8_t led)
{
    switch(led)
    {
        case LED1: LED1_GPIO_PDOR &= ~(1U << LED1_PIN);
            break; 
        case LED2: LED2_GPIO_PDOR &= ~(1U << LED2_PIN);
            break;
        case LED3: LED3_GPIO_PDOR &= ~(1U << LED3_PIN);
            break;
        default:
            break;
    }
}


void BSP_LED_Off(uint8_t led)
{
    switch(led)
    {
        case LED1: LED1_GPIO_PDOR |= (1U << LED1_PIN);
            break; 
        case LED2: LED2_GPIO_PDOR |= (1U << LED2_PIN);
            break;
        case LED3: LED3_GPIO_PDOR |= (1U << LED3_PIN);
            break;
        default:
            break;
    }
}
#endif

void LOS_EvbLedInit(void)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
#endif
    return;
}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    switch (index)
    {
        case LOS_LED1:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED1);
            }
            else
            {
                BSP_LED_Off(LED1);
            }
            break;
        }
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED2);
            }
            else
            {
                BSP_LED_Off(LED2);
            }
            break;
        }
        case LOS_LED3:
        {
            if (cmd == LED_ON)
            {
                BSP_LED_On(LED3);
            }
            else
            {
                BSP_LED_Off(LED3);
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    return;
}


