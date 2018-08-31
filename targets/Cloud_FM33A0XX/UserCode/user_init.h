#ifndef	__USER_INIT_H__
#define __USER_INIT_H__

#include "define_all.h"  


typedef int (*ConditionHook)(void);

extern void Init_SysTick(void);
extern void TicksDelay(uint32_t ClkNum);
extern void TicksDelayMs(uint32_t ms , ConditionHook Hook);
extern void TicksDelayUs(uint32_t us);
extern unsigned char CheckSysReg( __IO uint32_t *RegAddr, uint32_t Value );
extern unsigned char CheckNvicIrqEn( IRQn_Type IRQn );

extern void AnalogIO( GPIOx_Type* GPIOx, uint32_t PinNum );
extern void InputtIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type );
extern void OutputIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type );
extern void AltFunIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type );
extern void CloseeIO( GPIOx_Type* GPIOx, uint32_t PinNum );
extern void Close_None_GPIO_80pin(void);
extern void Close_AllIO_GPIO_80pin( void );

extern void Init_SysClk_Gen(void);
extern void Clk_Adj_Proc( uint08 ClkMode );
extern void Init_Pad_Io(void);
extern void IWDT_Init(void);

extern void Init_IO(void);
extern void Init_System(void);

extern void LED0_Flash(uint08 Times);

#endif
