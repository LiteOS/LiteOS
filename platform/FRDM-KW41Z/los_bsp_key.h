#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

/* SW3 -- PTC4 */
#define SW3_PORT   ((PORT_Type *)PORTC_BASE)
#define SW3_GPIO   ((GPIO_Type *)GPIOC_BASE)
#define SW3_PIN    (4U)

/* SW4 -- PTC5 */
#define SW4_PORT   ((PORT_Type *)PORTC_BASE)
#define SW4_GPIO   ((GPIO_Type *)GPIOC_BASE)
#define SW4_PIN    (5U)

#define LOS_KEY_PRESS      (0)

#define USER_KEY   SW4_PIN

extern void LOS_EvbKeyInit(void);

unsigned int LOS_EvbGetKeyVal(int KeyNum);

#endif

