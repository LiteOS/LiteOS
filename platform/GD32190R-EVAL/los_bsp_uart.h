#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#include <stdio.h>
#include <string.h>

#ifdef GD32F190R8
#include "gd32f1x0.h"
#include "system_gd32f1x0.h"
#include "systick.h"
#endif

extern void LOS_EvbUartInit(void);
extern void LOS_EvbUartReadByte(char* c);
extern void LOS_EvbUartWriteByte(char c);
extern void LOS_EvbUartWriteStr(const char* str);


#endif
