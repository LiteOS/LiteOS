/*************************************************
* 文件名称 ：main.c
* 作     者：物联网俱乐部
* 版     本：V1.0
* 编写日期 ：2018-4-1
* 功     能：主函数，任务创建与调度
*********************************************************************
* 说    明 ：本例程配套物联网俱乐部EVB_M1开发板使用
*
* 淘     宝：https://shop128001708.taobao.com/
* 论     坛：bbs.iot-club.cn
*********************************************************************************/
#ifndef __BH1750_H__
#define __BH1750_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define I2C_OWN_ADDRESS                            0x0A

#define I2C_WR	        0		/* 写控制bit */
#define I2C_RD	        1		/* 读控制bit */

#if defined(USE_NB_NEUL95)
#define I2C_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2C_GPIO_PORT                       GPIOC   
#define I2C_SCL_PIN                         GPIO_PIN_11
#define I2C_SDA_PIN                         GPIO_PIN_10

/* 高低电平*/  
#define HIGH    1  
#define LOW     0 


#define I2C_SCL(n) (n?HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_RESET)) //SCL  
#define I2C_SDA(n) (n?HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_RESET)) //SDA 
#define I2C_SDA_READ()               HAL_GPIO_ReadPin(I2C_GPIO_PORT,I2C_SDA_PIN)

#define SDA_IN()  {GPIOC->MODER&=~(3<<(10*2));GPIOC->MODER|=0<<10*2;}       
#define SDA_OUT() {GPIOC->MODER&=~(3<<(10*2));GPIOC->MODER|=1<<10*2;}  

#endif

#if defined(USE_ESP8266)
#define I2C_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2C_GPIO_SCL_PORT                       GPIOC
#define I2C_GPIO_SDA_PORT                       GPIOA
#define I2C_SCL_PIN                         GPIO_PIN_12
#define I2C_SDA_PIN                         GPIO_PIN_15

/* 高低电平*/  
#define HIGH    1
#define LOW     0


#define I2C_SCL(n) (n?HAL_GPIO_WritePin(I2C_GPIO_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_GPIO_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_RESET)) //SCL
#define I2C_SDA(n) (n?HAL_GPIO_WritePin(I2C_GPIO_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_GPIO_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_RESET)) //SDA
#define I2C_SDA_READ()               HAL_GPIO_ReadPin(I2C_GPIO_SDA_PORT,I2C_SDA_PIN)

#define SDA_IN()  {GPIOA->MODER&=~((unsigned int)3<<(15*2));GPIOA->MODER|=0<<15*2;}       
#define SDA_OUT() {GPIOA->MODER&=~((unsigned int)3<<(15*2));GPIOA->MODER|=1<<15*2;}

#endif

#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07




void    I2C_Start(void);
void    I2C_Stop(void);
uint8_t I2C_WaitAck(void);
void    I2C_Ack(void);
void    I2C_NAck(void);
void    I2C_SendByte(uint8_t Byte);
uint8_t I2C_ReadByte(uint8_t  ack);
uint8_t I2C_CheckDevice(uint8_t _Address);


void Init_BH1750(void);								  //IO初始化，
void Start_BH1750(void);			//上电，设置清除数据寄存器
void Read_BH1750(void);                               //连续的读取内部寄存器数据
float Convert_BH1750(void);
#endif





