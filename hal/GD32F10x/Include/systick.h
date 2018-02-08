/**
  ******************************************************************************
  * @brief   The header file of the SysTick configuration.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYS_TICK_H
#define SYS_TICK_H

/* Exported functions ------------------------------------------------------- */
void SysTick_Configuration(void);
void Delay_1ms(uint32_t nTime);
void TimingDelay_Decrement(void);

#endif /* SYS_TICK_H */

