/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.c
*	版    本 : V1.0
*	说    明 : 用gpio模拟i2c总线, 适用于STM32F4系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/

#include "gpio_i2c.h"

/*
	安富莱STM32-V5开发板 i2c总线GPIO:
 		PH4/I2C2_SCL
 		PH5/I2C2_SDA
*/
/* 定义读写SCL和SDA的宏 */
#define GPIO_I2C_SCL_1          gpio_bit_set(GPIO_I2C_SCL_PORT, GPIO_I2C_SCL_PIN)           /* SCL = 1 */
#define GPIO_I2C_SCL_0          gpio_bit_reset(GPIO_I2C_SCL_PORT, GPIO_I2C_SCL_PIN)         /* SCL = 0 */

#define GPIO_I2C_SDA_1          gpio_bit_set(GPIO_I2C_SDA_PORT, GPIO_I2C_SDA_PIN)           /* SDA = 1 */
#define GPIO_I2C_SDA_0          gpio_bit_reset(GPIO_I2C_SDA_PORT, GPIO_I2C_SDA_PIN)         /* SDA = 0 */
     
#define GPIO_I2C_SCL_READ       gpio_input_bit_get(GPIO_I2C_SCL_PORT, GPIO_I2C_SCL_PIN)     /* 读SCL口线状态 */     
#define GPIO_I2C_SDA_READ       gpio_input_bit_get(GPIO_I2C_SDA_PORT, GPIO_I2C_SDA_PIN)     /* 读SDA口线状态 */


/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_init(void)
{
    rcu_periph_clock_enable(GPIO_I2C_SCL_PORT_CLK);
    rcu_periph_clock_enable(GPIO_I2C_SDA_PORT_CLK);
    
    gpio_mode_set(GPIO_I2C_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_I2C_SCL_PIN);
    gpio_output_options_set(GPIO_I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_I2C_SCL_PIN);
    
    gpio_mode_set(GPIO_I2C_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_I2C_SDA_PIN);
    gpio_output_options_set(GPIO_I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_I2C_SDA_PIN);

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    gpio_i2c_stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void gpio_i2c_delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	GPIO_I2C_SDA_1;
	GPIO_I2C_SCL_1;
	
    gpio_i2c_delay();
	
    GPIO_I2C_SDA_0;
    
	gpio_i2c_delay();
    
	GPIO_I2C_SCL_0;
    
	gpio_i2c_delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	GPIO_I2C_SDA_0;
	GPIO_I2C_SCL_1;
    
	gpio_i2c_delay();
    
	GPIO_I2C_SDA_1;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_sendbyte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			GPIO_I2C_SDA_1;
		}
		else
		{
			GPIO_I2C_SDA_0;
		}
		gpio_i2c_delay();
		GPIO_I2C_SCL_1;
		gpio_i2c_delay();
		GPIO_I2C_SCL_0;
		if (i == 7)
		{
			 GPIO_I2C_SDA_1; // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		gpio_i2c_delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t gpio_i2c_readbyte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		GPIO_I2C_SCL_1;
		gpio_i2c_delay();
		if (GPIO_I2C_SDA_READ)
		{
			value++;
		}
		GPIO_I2C_SCL_0;
		gpio_i2c_delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t gpio_i2c_waitack(void)
{
	uint8_t re;

	GPIO_I2C_SDA_1;	/* CPU释放SDA总线 */
	gpio_i2c_delay();
	GPIO_I2C_SCL_1;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	gpio_i2c_delay();
	if (GPIO_I2C_SDA_READ)	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	GPIO_I2C_SCL_0;
	gpio_i2c_delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_ack(void)
{
	GPIO_I2C_SDA_0;	/* CPU驱动SDA = 0 */
	gpio_i2c_delay();
	GPIO_I2C_SCL_1;	/* CPU产生1个时钟 */
	gpio_i2c_delay();
	GPIO_I2C_SCL_0;
	gpio_i2c_delay();
	GPIO_I2C_SDA_1;	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void gpio_i2c_nack(void)
{
	GPIO_I2C_SDA_1;	/* CPU驱动SDA = 1 */
	gpio_i2c_delay();
	GPIO_I2C_SCL_1;	/* CPU产生1个时钟 */
	gpio_i2c_delay();
	GPIO_I2C_SCL_0;
	gpio_i2c_delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t gpio_i2c_checkdevice(uint8_t _Address)
{
	uint8_t ucAck;

	if (GPIO_I2C_SDA_READ && GPIO_I2C_SCL_READ)
	{
		gpio_i2c_start();		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		gpio_i2c_sendbyte(_Address | I2C_WR);
		ucAck = gpio_i2c_waitack();	/* 检测设备的ACK应答 */

		gpio_i2c_stop();			/* 发送停止信号 */

		return ucAck;
	}
	return 1;	/* I2C总线异常 */
}
