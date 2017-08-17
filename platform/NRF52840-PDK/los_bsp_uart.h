#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#include <stdio.h>
#include <string.h>

#ifdef LOS_NRF52840

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5
#define HWFC           false

#endif

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(const char c);
extern void LOS_EvbUartWriteStr(const char* str);


#endif
