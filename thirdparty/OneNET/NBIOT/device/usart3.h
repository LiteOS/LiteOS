#ifndef USART3_H_
#define USART3_H_

#include <stdint.h>
#include "los_bsp_led.h"

#define US3_RX_BUFSIZE    512
#define US3_TX_BUFSIZE    512

#define US3_POOL_NUM      4
#define US3_POOL_BUFSIZE  US3_RX_BUFSIZE

typedef struct {
  uint8_t buf[US3_POOL_BUFSIZE];
} us3_pool_t;


void SendCmd(char* cmd, char *result, uint16_t timeout, uint16_t retry);
void SendData(char* cmd, char *result, uint16_t len, uint16_t timeout, uint16_t retry);
void USART3_Init(void);
void netif_rx(uint8_t*buf,uint16_t *read);

#endif

