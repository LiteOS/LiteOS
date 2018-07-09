/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	sht20.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2016-11-23
	*
	*	版本： 		V1.0
	*
	*	说明： 		温湿度读取
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "sht20.h"
#include "i2c.h"

const int16_t POLYNOMIAL = 0x131;

SHT20_INFO sht20Info;



/*
************************************************************
*	函数名称：	SHT20_reset
*
*	函数功能：	SHT20复位
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SHT20_reset(void)
{
	
    I2C_WriteByte(SHT20_ADDRESS, SHT20_SOFT_RESET, (void *)0);
	
}

/*
************************************************************
*	函数名称：	SHT20_read_user_reg
*
*	函数功能：	SHT20读取用户寄存器
*
*	入口参数：	无
*
*	返回参数：	读取到的用户寄存器的值
*
*	说明：		
************************************************************
*/
unsigned char  SHT20_read_user_reg(void)
{
	
    unsigned char val = 0;
	
    I2C_ReadByte(SHT20_ADDRESS, SHT20_READ_REG, &val);
	
    return val;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CheckCrc
*
*	函数功能：	检查数据正确性
*
*	入口参数：	data：读取到的数据
*				nbrOfBytes：需要校验的数量
*				checksum：读取到的校对比验值
*
*	返回参数：	校验结果
*
*	说明：		0-成功		1-失败
************************************************************
*/
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{
	
    char crc = 0;
    char bit = 0;
    char byteCtr = 0;
	
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CalcTemperatureC
*
*	函数功能：	温度计算
*
*	入口参数：	u16sT：读取到的温度原始数据
*
*	返回参数：	计算后的温度数据
*
*	说明：		
************************************************************
*/
float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [癈] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
	
    return temperatureC;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CalcRH
*
*	函数功能：	湿度计算
*
*	入口参数：	u16sRH：读取到的湿度原始数据
*
*	返回参数：	计算后的湿度数据
*
*	说明：		
************************************************************
*/
float SHT2x_CalcRH(unsigned short u16sRH)
{
	
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
	
    return humidityRH;
	
}

/*
************************************************************
*	函数名称：	SHT2x_MeasureHM
*
*	函数功能：	测量温湿度
*
*	入口参数：	cmd：测量温度还是湿度
*				pMeasurand：不为空则保存为ushort值到此地址
*
*	返回参数：	测量结果
*
*	说明：		
************************************************************
*/
float SHT2x_MeasureHM(unsigned char cmd, unsigned short *pMeasurand)
{
	
    char  checksum = 0;  //checksum
    char  data[2];    //data array for checksum verification
	unsigned char addr = 0;
    unsigned short tmp = 0;
    float t = 0;
	
    addr = SHT20_ADDRESS << 1;
	
	IIC_Start();
	
	IIC_SendByte(addr);
	if(IIC_WaitAck(50000)) //等待应答
		return 0.0;
	
	IIC_SendByte(cmd);
	if(IIC_WaitAck(50000)) //等待应答
		return 0.0;
	
	IIC_Start();
	
	IIC_SendByte(addr + 1);
	while(IIC_WaitAck(50000)) //等待应答
	{
		IIC_Start();
		IIC_SendByte(addr + 1);
	}
	
	osDelay(70);
	
	data[0] = IIC_RecvByte();
	IIC_Ack();
	data[1] = IIC_RecvByte();
	IIC_Ack();
	
	checksum = IIC_RecvByte();
	IIC_NAck();
	
	IIC_Stop();
	
	SHT2x_CheckCrc(data, 2, checksum);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }
	
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }
	
    return t;
	
}

/*
************************************************************
*	函数名称：	SHT20_GetValue
*
*	函数功能：	获取温湿度数据
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		温湿度结果保存在SHT20结构体里
************************************************************
*/
void SHT20_GetValue(void)
{
	
#if 1
	unsigned char val = 0;
	
	IIC_SpeedCtl(5);
	
	SHT20_read_user_reg();
	osDelay(1);
	
	sht20Info.tempreture = SHT2x_MeasureHM(SHT20_Measurement_T_HM, (void *)0);
	osDelay(70);
	
	sht20Info.humidity = SHT2x_MeasureHM(SHT20_Measurement_RH_HM, (void *)0);
	osDelay(25);
	
	SHT20_read_user_reg();
	osDelay(25);
	
	I2C_WriteByte(SHT20_ADDRESS, SHT20_WRITE_REG, &val);
	osDelay(1);
	
	SHT20_read_user_reg();
	osDelay(1);
	
	SHT20_reset();
	osDelay(1);
	
#else
	IIC_SpeedCtl(5);
	sht20Info.tempreture = SHT2x_MeasureHM(SHT20_Measurement_T_HM, (void *)0);

#endif
}


float SHT20_GetTempreture(void)
{
	SHT20_GetValue();
	return sht20Info.tempreture;
}
/*
************************************************************
*	函数名称：	SHT20_test
*
*	函数功能：	SHT20测试
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SHT20_test(void)
{
	SHT20_GetValue();
#if 1
printf("Usart1 Get Data,SHT20 wendu is: \r\n%0.1fC\r\n", sht20Info.tempreture);
printf("Usart1 Get Data,SHT20 shidu is: \r\n%0.1f\r\n", sht20Info.humidity);
#endif
}
