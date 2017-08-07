#ifndef __KEY_H
#define __KEY_H	 
#include "MM32L073PF.h"

//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//按键驱动代码	   
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY1    ((GPIOC->IDR & 0x2000) ? 1 : 0)  //输入 //读取按键1  读出口值为1的为1否则为0
#define WK_UP   ((GPIOA->IDR & 0x1) ? 1 : 0)  //输入 //读取按键2 
#define KEY3    ((GPIOB->IDR & 0x400) ? 1 : 0)  //输入 //读取按键3
#define KEY4    ((GPIOB->IDR & 0x800) ? 1 : 0) //输入 //读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

void KEY_Init(void);//IO初始化

#endif /* __KEY_H */
