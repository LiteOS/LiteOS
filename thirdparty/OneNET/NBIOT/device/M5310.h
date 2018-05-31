#ifndef __NBIOT_HEADER__
#define __NBIOT_HEADER__

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

uint32_t ip_SendData(char * buf,char *ptr, uint32_t len);
char hexstring_to_hex(unsigned char *outstr,unsigned char *instr,int len);

void M5310_Power_Init(void);
void netdev_init(void);

#endif
