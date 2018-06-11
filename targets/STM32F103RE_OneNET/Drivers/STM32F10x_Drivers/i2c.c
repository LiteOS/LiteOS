/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	i2c.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2016-11-23
	*
	*	版本： 		V1.0
	*
	*	说明： 		软件IIC总线IO初始化，读写控制
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/
//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "i2c.h"

IIC_INFO iicInfo;


#define I2CDEV_LOOPS_PER_US  5
static inline void DelayUs(uint32_t us)
{
  volatile uint32_t delay;

  for(delay = I2CDEV_LOOPS_PER_US * us; delay > 0; delay--);
}



/*
************************************************************
*	函数名称：	IIC_SpeedCtl
*
*	函数功能：	软件IIC速度控制
*
*	入口参数：	speed：延时参数
*
*	返回参数：	无
*
*	说明：		单位：微秒
************************************************************
*/
void IIC_SpeedCtl(unsigned short speed)
{

	iicInfo.speed = speed;

}

/*
************************************************************
*	函数名称：	IIC_Init
*
*	函数功能：	软件IIC总线IO初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		使用开漏方式，这样可以不用切换IO口的输入输出方向
************************************************************
*/
void IIC_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//打开GPIOB的时钟
	
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;			//开漏，这样不用去切换输出输入方向
	gpioInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);
	
	IIC_SpeedCtl(2);										//IIC速度控制，单位：微秒
	
	SDA_H;													//拉高SDA线，处于空闲状态
	SCL_H;													//拉高SCL线，处于空闲状态

}

/*
************************************************************
*	函数名称：	IIC_Start
*
*	函数功能：	软件IIC开始信号
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void IIC_Start(void)
{
	
	SDA_H;						//拉高SDA线
	SCL_H;						//拉高SCL线
	DelayUs(iicInfo.speed);		//延时，速度控制
	
	SDA_L;						//当SCL线为高时，SDA线一个下降沿代表开始信号
	DelayUs(iicInfo.speed);		//延时，速度控制
	SCL_L;						//钳住SCL线，以便发送数据

}

/*
************************************************************
*	函数名称：	IIC_Stop
*
*	函数功能：	软件IIC停止信号
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void IIC_Stop(void)
{

	SDA_L;						//拉低SDA线
	SCL_L;						//拉低SCL先
	DelayUs(iicInfo.speed);		//延时，速度控制
	
	SCL_H;						//拉高SCL线
	SDA_H;						//拉高SDA线，当SCL线为高时，SDA线一个上升沿代表停止信号
	DelayUs(iicInfo.speed);

}

/*
************************************************************
*	函数名称：	IIC_WaitAck
*
*	函数功能：	软件IIC等待应答
*
*	入口参数：	timeOut：超时时间
*
*	返回参数：	无
*
*	说明：		单位：微秒
************************************************************
*/
_Bool IIC_WaitAck(unsigned int timeOut)
{
	
	
	SDA_H;DelayUs(iicInfo.speed);			//拉高SDA线
	SCL_H;DelayUs(iicInfo.speed);			//拉高SCL线
	
	while(SDA_R)							//如果读到SDA线为1，则等待。应答信号应是0
	{
		if(--timeOut)
		{
			printf("I2C WaitAck TimeOut\r\n");

			IIC_Stop();						//超时未收到应答，则停止总线
			
			return IIC_Err;					//返回失败
		}
		
		DelayUs(iicInfo.speed);
	}
	
	SCL_L;									//拉低SCL线，以便继续收发数据
	
	return IIC_OK;							//返回成功
	
}

/*
************************************************************
*	函数名称：	IIC_Ack
*
*	函数功能：	软件IIC产生一个应答
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		当SDA线为低时，SCL线一个正脉冲代表发送一个应答信号
************************************************************
*/
void IIC_Ack(void)
{
	
	SCL_L;						//拉低SCL线
	SDA_L;						//拉低SDA线
	DelayUs(iicInfo.speed);
	SCL_H;						//拉高SCL线
	DelayUs(iicInfo.speed);
	SCL_L;						//拉低SCL线
	
}

/*
************************************************************
*	函数名称：	IIC_NAck
*
*	函数功能：	软件IIC产生一非个应答
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		当SDA线为高时，SCL线一个正脉冲代表发送一个非应答信号
************************************************************
*/
void IIC_NAck(void)
{
	
	SCL_L;						//拉低SCL线
	SDA_H;						//拉高SDA线
	DelayUs(iicInfo.speed);
	SCL_H;						//拉高SCL线
	DelayUs(iicInfo.speed);
	SCL_L;						//拉低SCL线
	
}

/*
************************************************************
*	函数名称：	IIC_SendByte
*
*	函数功能：	软件IIC发送一个字节
*
*	入口参数：	byte：需要发送的字节
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void IIC_SendByte(unsigned char byte)
{

	unsigned char count = 0;
	
    SCL_L;							//拉低时钟开始数据传输
	
    for(; count < 8; count++)		//循环8次，每次发送一个bit
    {
		if(byte & 0x80)				//发送最高位
			SDA_H;
		else
			SDA_L;
		
		byte <<= 1;					//byte左移1位
		
		DelayUs(iicInfo.speed);
		SCL_H;
		DelayUs(iicInfo.speed);
		SCL_L;
    }

}

/*
************************************************************
*	函数名称：	IIC_RecvByte
*
*	函数功能：	软件IIC接收一个字节
*
*	入口参数：	无
*
*	返回参数：	接收到的字节数据
*
*	说明：		
************************************************************
*/
unsigned char IIC_RecvByte(void)
{
	
	unsigned char count = 0, receive = 0;
	
	SDA_H;							//拉高SDA线，开漏状态下，需线拉高以便读取数据
	
    for(; count < 8; count++ )		//循环8次，每次发送一个bit
	{
		SCL_L;
		DelayUs(iicInfo.speed);
		SCL_H;
		
        receive <<= 1;				//左移一位
		
        if(SDA_R)					//如果SDA线为1，则receive变量自增，每次自增都是对bit0的+1，然后下一次循环会先左移一次
			receive++;
		
		DelayUs(iicInfo.speed);
    }
	
    return receive;
	
}

/*
************************************************************
*	函数名称：	I2C_WriteByte
*
*	函数功能：	软件IIC写一个数据
*
*	入口参数：	slaveAddr：从机地址
*				regAddr：寄存器地址
*				byte：需要写入的数据
*
*	返回参数：	0-写入成功	1-写入失败
*
*	说明：		*byte是缓存写入数据的变量的地址，因为有些寄存器只需要控制下寄存器，并不需要写入值
************************************************************
*/
_Bool I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *byte)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	if(byte)
	{
		IIC_SendByte(*byte);	//发送数据
		if(IIC_WaitAck(5000))	//等待应答
			return IIC_Err;
	}
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

/*
************************************************************
*	函数名称：	I2C_ReadByte
*
*	函数功能：	软件IIC读取一个字节
*
*	入口参数：	slaveAddr：从机地址
*				regAddr：寄存器地址
*				val：需要读取的数据缓存
*
*	返回参数：	0-成功		1-失败
*
*	说明：		val是一个缓存变量的地址
************************************************************
*/
_Bool I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val)
{

	unsigned char addr = 0;

    addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_Start();				//重启信号
	
	IIC_SendByte(addr + 1);		//发送设备地址(读)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	*val = IIC_RecvByte();		//接收
	IIC_NAck();					//产生一个非应答信号，代表读取接收
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

/*
************************************************************
*	函数名称：	I2C_WriteBytes
*
*	函数功能：	软件IIC写多个数据
*
*	入口参数：	slaveAddr：从机地址
*				regAddr：寄存器地址
*				buf：需要写入的数据缓存
*				num：数据长度
*
*	返回参数：	0-写入成功	1-写入失败
*
*	说明：		*buf是一个数组或指向一个缓存区的指针
************************************************************
*/
_Bool I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	while(num--)				//循环写入数据
	{
		IIC_SendByte(*buf);		//发送数据
		if(IIC_WaitAck(5000))	//等待应答
			return IIC_Err;
		
		buf++;					//数据指针偏移到下一个
		
		DelayUs(10);
	}
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

/*
************************************************************
*	函数名称：	I2C_ReadBytes
*
*	函数功能：	软件IIC读多个数据
*
*	入口参数：	slaveAddr：从机地址
*				regAddr：寄存器地址
*				buf：需要读取的数据缓存
*				num：数据长度
*
*	返回参数：	0-写入成功	1-写入失败
*
*	说明：		*buf是一个数组或指向一个缓存区的指针
************************************************************
*/
_Bool I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned short addr = 0;

    addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_Start();				//重启信号
	
	IIC_SendByte(addr + 1);		//发送设备地址(读)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	while(num--)
	{
		*buf = IIC_RecvByte();
		buf++;					//偏移到下一个数据存储地址
		
		if(num == 0)
        {
           IIC_NAck();			//最后一个数据需要回NOACK
        }
        else
        {
          IIC_Ack();			//回应ACK
		}
	}
	
	IIC_Stop();
	
	return IIC_OK;

}
