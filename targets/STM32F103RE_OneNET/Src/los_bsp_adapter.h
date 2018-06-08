#ifndef _LOS_BSP_ADAPTER_H
#define _LOS_BSP_ADAPTER_H

#include <stdio.h>
#include <string.h>
#include "los_typedef.h" //OneNET


extern void LOS_EvbSetup(void);
extern void LOS_EvbTrace(const char *str);
extern void LOS_DelayUs( UINT32	num );



#endif
