#include "ds18b20.h"

void delay_us(uint32_t micro_sec)
{
    uint32_t tick_want;
    uint32_t tick_old;
    uint32_t tick_now;
    uint32_t cnt = 0;
    uint32_t reload;
       
    reload = SysTick->LOAD;                
    tick_want = micro_sec * (SystemCoreClock / 1000000);
    
    tick_old = SysTick->VAL;             /* 刚进入时的计数器值 */

    while (1)
    {
        tick_now = SysTick->VAL;    
        if (tick_now != tick_old)
        {    
            /* SYSTICK是一个递减的计数器 */    
            if (tick_now < tick_old)
            {
                cnt += tick_old - tick_now;    
            }
            /* 重新装载递减 */
            else
            {
                cnt += reload - tick_now + tick_old;    
            }        
            tick_old = tick_now;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (cnt >= tick_want)
            {
                break;
            }
        }  
    }
}

void ds18b20_init(void)
{
    rcu_periph_clock_enable(DQ_GPIO_CLK);

    gpio_mode_set(DQ_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DQ_GPIO_PIN);
    gpio_output_options_set(DQ_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DQ_GPIO_PIN);
    
    DQ_PIN_SET;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_Reset
*	功能说明: 复位DS18B20。 拉低DQ为低，持续最少480us，然后等待
*	形    参: 无
*	返 回 值: 0 失败； 1 表示成功
*********************************************************************************************************
*/
uint8_t DS18B20_Reset(void)
{
    
    uint8_t retry = 0;
    uint16_t k;

    /* 复位，如果失败则返回0 */
    for (retry = 0; retry < 3; retry++){
        DQ_PIN_RESET;
        delay_us(520);  /* 延迟 520uS， 要求这个延迟大于 480us */
        DQ_PIN_SET;				/* 释放DQ */

        delay_us(15);	/* 等待15us */

        /* 检测DQ电平是否为低 */
        for (k = 0; k < 10; k++)
        {
            if (IS_DQ_LOW)
			{
				break;
			}
			delay_us(10);	/* 等待65us */
		}
		if (k >= 10)
		{
			continue;		/* 失败 */
		}

		/* 等待DS18B20释放DQ */
		for (k = 0; k < 30; k++)
		{
			if (!IS_DQ_LOW)
			{
				break;
			}
			delay_us(10);	/* 等待65us */
		}
		if (k >= 30)
		{
			continue;		/* 失败 */
		}

		break;
	}

	delay_us(5);

	if (retry >= 1)
	{
		return 0;
	}

	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_WriteByte
*	功能说明: 向DS18B20写入1字节数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DS18B20_WriteByte(uint8_t _val)
{
	/*
		写数据时序, 见DS18B20 page 16
	*/
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		DQ_PIN_RESET;
		delay_us(2);

		if (_val & 0x01)
		{
			DQ_PIN_SET;
		}
		else
		{
			DQ_PIN_RESET;
		}
		delay_us(60);
		DQ_PIN_SET;
		delay_us(2);
		_val >>= 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadByte
*	功能说明: 向DS18B20读取1字节数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t DS18B20_ReadByte(void)
{
	/*
		写数据时序, 见DS18B20 page 16
	*/
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		read >>= 1;

		DQ_PIN_RESET;
		delay_us(3);
		DQ_PIN_SET;
		delay_us(3);

		if (IS_DQ_LOW)
		{
			;
		}
		else
		{
			read |= 0x80;
		}
		delay_us(60);
	}

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempReg
*	功能说明: 读温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempReg(void)
{
	uint8_t temp1, temp2;

	/* 总线复位 */
	if (DS18B20_Reset() == 0)
	{
		return 0;
	}		

	DS18B20_WriteByte(0xcc);	/* 发命令 */
	DS18B20_WriteByte(0x44);	/* 发转换命令 */

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0xcc);	/* 发命令 */
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* 读温度值低字节 */
	temp2 = DS18B20_ReadByte();	/* 读温度值高字节 */

	return ((temp2 << 8) | temp1);	/* 返回16位寄存器值 */
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadID
*	功能说明: 读DS18B20的ROM ID， 总线上必须只有1个芯片
*	形    参: _id 存储ID
*	返 回 值: 0 表示失败， 1表示检测到正确ID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* 总线复位 */
	if (DS18B20_Reset() == 0)
	{
		return 0;
	}

	DS18B20_WriteByte(0x33);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_ReadByte();
	}

	DS18B20_Reset();		/* 总线复位 */
	
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempByID
*	功能说明: 读指定ID的温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempByID(uint8_t *_id)
{
	uint8_t temp1, temp2;
	uint8_t i;

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}
	DS18B20_WriteByte(0x44);	/* 发转换命令 */

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}	
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* 读温度值低字节 */
	temp2 = DS18B20_ReadByte();	/* 读温度值高字节 */

	return ((temp2 << 8) | temp1);	/* 返回16位寄存器值 */
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
