
#ifndef _APP_LED_H_
#define _APP_LED_H_

#define LED_NUM 8

extern uint8_t led_init(void);
extern void    led_on(uint8_t num);
extern void    led_off(uint8_t num);
extern void    led_toggle(uint8_t num);

#endif
