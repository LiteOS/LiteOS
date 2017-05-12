#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#include <stdio.h>
#include <string.h>

/* LPUART -- PTC6 & PTC7 */
#define LPUART_DEBUG_PORT       ((PORT_Type *)PORTC_BASE)
#define LPUART_DEBUG_GPIO       ((GPIO_Type *)GPIOC_BASE)
#define LPUART_DEBUG_RX_PIN     (6U)
#define LPUART_DEBUG_TX_PIN     (7U)

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(const char c);
extern void LOS_EvbUartWriteStr(const char* str);


#endif
