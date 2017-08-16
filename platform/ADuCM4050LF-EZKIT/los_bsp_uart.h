#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#include <stdio.h>
#include <string.h>

#ifdef LOS_ADuCM450LF
#define UART0_TX_PORTP0_MUX (1u<<20)
#define UART0_RX_PORTP0_MUX (1u<<22)
#endif

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(char c);
extern void LOS_EvbUartWriteStr(char* str);


#endif
