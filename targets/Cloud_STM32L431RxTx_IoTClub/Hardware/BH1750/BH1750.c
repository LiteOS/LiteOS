/********************************************************************************
    * 文件名称 ：BH1750.c
    * 作     者：物联网俱乐部
    * 版     本：V1.0
    * 编写日期 ：2018-4-1
    * 功     能：EVB_M/H 光照强度扩展板驱动
*********************************************************************************
    * 说    明 ：本例程配套物联网俱乐部EVB_M1开发板使用
    *
    * 淘     宝：https://shop128001708.taobao.com/
    * 论     坛：bbs.iot-club.cn
*********************************************************************************/

#include "BH1750.h"

float result_lx=0;
uint8_t BUF[2]={0};
uint16_t result=0;


static void delayus(uint16_t time)
{
	uint8_t i;

  while(time)
  {
	  for (i = 0; i < 10; i++)
    {

    }
    time--;
  }
}


/**
  * 函数功能: 配置I2C总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void I2C_InitGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
#if defined(USE_NB_NEUL95)
  /* 打开GPIO时钟 */
	I2C_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH ;
  HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
#endif
	
#if defined(USE_ESP8266)
  /* 打开GPIO时钟 */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(I2C_GPIO_SDA_PORT, &GPIO_InitStruct);
	HAL_GPIO_Init(I2C_GPIO_SCL_PORT, &GPIO_InitStruct);
#endif
	
	I2C_SCL(HIGH); 
	I2C_SDA(HIGH); 
}

/**
  * 函数功能: CPU发起I2C总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	SDA_OUT();
	I2C_SDA(HIGH); 
	I2C_SCL(HIGH); 
	delayus(4);
	I2C_SDA(LOW); 
	delayus(4);
	I2C_SCL(LOW); 
}

/**
  * 函数功能: CPU发起I2C总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	SDA_OUT();
	I2C_SCL(LOW); 
	I2C_SDA(LOW); 
	delayus(4);
	I2C_SCL(HIGH); 	
	I2C_SDA(HIGH); 
	delayus(4);
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t I2C_WaitAck(void)
{
  uint8_t ucErrTime=0;
	SDA_IN(); 
	I2C_SDA(HIGH); 	/* CPU释放SDA总线 */
	delayus(4);
	I2C_SCL(HIGH); 	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	delayus(4);
	while (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	
	I2C_SCL(LOW); 
	return 0;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Ack(void)
{

	I2C_SCL(LOW); 
	SDA_OUT(); 
	I2C_SDA(LOW); 	/* CPU驱动SDA = 0 */
	delayus(2);
	I2C_SCL(HIGH); 	/* CPU产生1个时钟 */
	delayus(2);
	I2C_SCL(LOW); 
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_NAck(void)
{
	
	I2C_SCL(LOW); 
	SDA_OUT();
	I2C_SDA(HIGH); 	/* CPU驱动SDA = 1 */
	delayus(2);
	I2C_SCL(HIGH); 	/* CPU产生1个时钟 */
	delayus(2);
	I2C_SCL(LOW); 
}

/**
  * 函数功能: CPU向I2C总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_SendByte(uint8_t Byte)
{
  uint8_t t;  
      
    SDA_OUT();                                                      //sda???  
    I2C_SCL(LOW);                                                   //??????????  
    for (t = 0; t < 8; t++)  
    {  
        I2C_SDA((Byte & 0x80) >> 7);  
        Byte <<= 1;  
        delayus(2);  
        I2C_SCL(HIGH);  
        delayus(2);  
        I2C_SCL(LOW);  
        delayus(2);  
    } 
}


/**
  * 函数功能: CPU从I2C总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i;
	uint8_t value;
  SDA_IN();
	/* 读到第1个bit为数据的bit7 */
	value = 0;
for (i = 0; i < 8; i++)  
    {  
        I2C_SCL(LOW);  
        delayus(2);  
        I2C_SCL(HIGH);  
        value <<= 1;  
        if (I2C_SDA_READ() ) value++;  
        delayus(1);  
    }  
		if (!ack)
        I2C_NAck();//发送nACK
    else
        I2C_Ack(); //发送ACK  
	return value;
}


/**
  * 函数功能: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
  * 输入参数: _Address：设备的I2C总线地址
  * 返 回 值: 返回值 0 表示正确， 返回1表示未探测到
  * 说    明：在访问I2C设备前，请先调用 I2C_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
  */
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_InitGPIO();		/* 配置GPIO */	
	I2C_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_SendByte(_Address | I2C_WR);
	ucAck = I2C_WaitAck();	/* 检测设备的ACK应答 */
	I2C_Stop();			/* 发送停止信号 */
	return ucAck;
}

void Cmd_Write_BH1750(uint8_t cmd)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(BH1750_Addr+0);   //发送设备地址+写信号
	while(I2C_WaitAck());
	delayus(100);
    I2C_SendByte(cmd);    //内部寄存器地址
	while(I2C_WaitAck());
	delayus(100);
    I2C_Stop();                   //发送停止信号
	LOS_TaskDelay(5);
}
void Init_BH1750(void)
{
	I2C_InitGPIO();	
	Cmd_Write_BH1750(0x01);
}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_CON);
}
void Read_BH1750(void)
{   uint8_t i;	
    I2C_Start();                          //起始信号
    I2C_SendByte(BH1750_Addr+1);         //发送设备地址+读信号
	while(I2C_WaitAck());
	for (i=0; i<3; i++)                      
    {     
        if (i ==3)
        {
        BUF[i]=I2C_ReadByte(0); 
                   
        }
        else
        {	BUF[i]=I2C_ReadByte(1);  //发送ACK	
         
       }
   }
	
	 //发送NACK

    I2C_Stop();                          //停止信号
    LOS_TaskDelay(5);
}
float Convert_BH1750(void)
{
	Start_BH1750();
	LOS_TaskDelay(180);
	Read_BH1750();
	result=BUF[0];
	result=(result<<8)+BUF[1];  //合成数据，即光照数据	
	result_lx=(float)(result/1.2);
  return result_lx;
}

