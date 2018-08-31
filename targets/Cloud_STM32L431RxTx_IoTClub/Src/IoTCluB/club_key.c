/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club key.c
 *Addr: www.iot-club.cn
**/


#include "club_key.h"
#include "club_nbiot.h"

/*  °´¼ü³õÊ¼»¯  */
void club_key_init(){
	UINT32 uvIntSave;
	uvIntSave = LOS_IntLock();
	
	LOS_HwiCreate(EXTI0_IRQn, 2,	0,	F1_Key_IRQHandler,	NULL);
	LOS_HwiCreate(EXTI1_IRQn, 3,	0,	F2_Key_IRQHandler,	NULL);
	LOS_HwiCreate(EXTI2_IRQn, 4,	0,	F3_Key_IRQHandler,	NULL);
	LOS_HwiCreate(EXTI3_IRQn, 5,	0,	F4_Key_IRQHandler,	NULL);
	
	LOS_IntRestore(uvIntSave);
}



void F1_Key_IRQHandler(void){
  /* EXTI line interrupt detected */
  if(__HAL_GPIO_EXTI_GET_IT(KEY_F4) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY_F4);
    CLUB_GPIO_EXTI_Callback(KEY_F4);
  }		
}

void F2_Key_IRQHandler(void){
  /* EXTI line interrupt detected */
  if(__HAL_GPIO_EXTI_GET_IT(KEY_F3) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY_F3);
    CLUB_GPIO_EXTI_Callback(KEY_F3);
  }		
}

void F3_Key_IRQHandler(void){
  /* EXTI line interrupt detected */
  if(__HAL_GPIO_EXTI_GET_IT(KEY_F2) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY_F2);
    CLUB_GPIO_EXTI_Callback(KEY_F2);
  }		
}


void F4_Key_IRQHandler(void){
  /* EXTI line interrupt detected */
  if(__HAL_GPIO_EXTI_GET_IT(KEY_F1) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY_F1);
    CLUB_GPIO_EXTI_Callback(KEY_F1);
  }	
}


void CLUB_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case KEY_F1:
			printf("This is F4\r\n");
			break;
		case KEY_F2:
			printf("This is F3\r\n");
			break;
		case KEY_F3:
			printf("This is F2\r\n");
			break;
		case KEY_F4:
			printf("This is F1\r\n");
			break;
		default:
			break;
	}
	
}




