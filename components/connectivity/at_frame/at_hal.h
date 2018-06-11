#ifndef __AT_HAL_H__
#define __AT_HAL_H__

#include "los_queue.h"
#include "los_mux.h"
#include "los_task.h"
#include "los_sem.h"
#include "atiny_adapter.h"
#include "stm32f4xx_hal.h"

void at_transmit(uint8_t * cmd, int32_t len,int flag);
void at_usart_config(void);
int read_resp(uint8_t * buf);
//declear in device drivers
extern at_config at_user_conf;
#endif