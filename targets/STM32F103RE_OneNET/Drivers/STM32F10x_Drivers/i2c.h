#ifndef _I2C_H_
#define _I2C_H_







#define IIC_OK		0
#define IIC_Err		1



//SDA		PB7
//SCL		PB6
#define SDA_H	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define SDA_L	GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define SDA_R	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define SCL_H	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define SCL_L	GPIO_ResetBits(GPIOB, GPIO_Pin_6)


typedef struct
{

	unsigned short speed;

} IIC_INFO;

extern IIC_INFO iicInfo;




void IIC_Init(void);

void IIC_SpeedCtl(unsigned short speed);

_Bool I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *byte);

_Bool I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val);

_Bool I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);

_Bool I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);

void IIC_Start(void);

void IIC_Stop(void);

_Bool IIC_WaitAck(unsigned int timeOut);

void IIC_Ack(void);

void IIC_NAck(void);

void IIC_SendByte(unsigned char byte);

unsigned char IIC_RecvByte(void);


#endif
