
#ifndef _APP_INTERRUPT_H_
#define _APP_INTERRUPT_H_

extern volatile uint32_t dwSysTicks;
extern volatile uint32_t dwLedTick;
extern volatile uint32_t dwLedTime;


extern void SysTick_Handler(void);

#endif

// end file
