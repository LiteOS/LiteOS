#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#include <stdio.h>
#include <string.h>

#define RX_PIN_NUMBER  22
#define TX_PIN_NUMBER  21
#define CTS_PIN_NUMBER 40
#define RTS_PIN_NUMBER 40

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(const char c);
extern void LOS_EvbUartWriteStr(const char* str);
extern void LOS_EvbUartPrintf(char* fmt, ...);

#endif
