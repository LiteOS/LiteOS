/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club func.h
 *Addr: www.iot-club.cn
**/

#ifndef __CLUB_KEY__
#define __CLUB_KEY__

#include "sys_init.h"
#include "los_typedef.h"
#include "agent_tiny_demo.h"
#include "stm32l4xx_hal.h"

#define KEY_F1 	GPIO_PIN_3
#define KEY_F2	GPIO_PIN_2
#define KEY_F3	GPIO_PIN_1
#define KEY_F4	GPIO_PIN_0

void club_key_init();

void F1_Key_IRQHandler(void);
void F2_Key_IRQHandler(void);
void F3_Key_IRQHandler(void);
void F4_Key_IRQHandler(void);
void CLUB_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif

