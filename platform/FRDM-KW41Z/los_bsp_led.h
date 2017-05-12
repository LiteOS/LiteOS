#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

/* LED RED -- PTC1 */
#define LED_RED_PORT    ((PORT_Type *)PORTC_BASE)
#define LED_RED_GPIO    ((GPIO_Type *)GPIOC_BASE)
#define LED_RED_PIN     (1U)

/* LED GREEN -- PTA19 */
#define LED_GREEN_PORT  ((PORT_Type *)PORTA_BASE)
#define LED_GREEN_GPIO  ((GPIO_Type *)GPIOA_BASE)
#define LED_GREEN_PIN   (19U)

/* LED BLUE -- PTA18 */
#define LED_BLUE_PORT   ((PORT_Type *)PORTA_BASE)
#define LED_BLUE_GPIO   ((GPIO_Type *)GPIOA_BASE)
#define LED_BLUE_PIN    (18U)

/* LED3 -- TPB0 */
#define LED3_PORT    ((PORT_Type *)PORTB_BASE)
#define LED3_GPIO    ((GPIO_Type *)GPIOB_BASE)
#define LED3_PIN     (0U)


#define LED_ON          (1)
#define LED_OFF         (0)
#define LOS_LED1        (LED_RED_PIN)
#define LOS_LED2        (LED_GREEN_PIN)
#define LOS_LED3        (LED_BLUE_PIN)
#define LOS_LED4        (LED3_PIN)
#define LOS_LEDX        (4)


void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);
#endif
