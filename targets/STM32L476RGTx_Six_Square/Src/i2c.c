
#include "i2c.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"



I2C_HandleTypeDef s_i2c1;
I2C_HandleTypeDef s_i2c2;
I2C_HandleTypeDef s_i2c3;
I2C_HandleTypeDef i2cx;



void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(i2cx.Instance == I2C1)
	{
	  
		__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
							
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	}
	else if(i2cx.Instance == I2C2)
	{
		
		__HAL_RCC_I2C2_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	}
	else if(i2cx.Instance == I2C3)
	{
		
		__HAL_RCC_I2C3_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
	}
}


/**
* @brief  Initializes the i2c device,include clock/function/gpio/mode/frequency.
* @param  i2c_device_t *device : parameter list
* @retval 1:success
          0:fail
*/
bool_t dal_i2c_open(void *device, s32_t flag)
{
	volatile uint8_t ret;
	i2c_device_t *device_init = (i2c_device_t *)device;

	(void)flag;

	switch (device_init->i2c_idx)
	{
	case DAL_I2C_ID1:
	    i2cx.Instance = I2C1;
	    break;
	case DAL_I2C_ID2:
		i2cx.Instance = I2C2;	
		break;
	case DAL_I2C_ID3:
		i2cx.Instance = I2C3;				
		break;
	default:
		return 0;	
						
	}	
	switch (device_init->freq_khz)
	{
	case DAL_FRE_10KHZ:
		i2cx.Init.Timing = 0x00008BFF;
		break;
	case DAL_FRE_100KHZ:
		i2cx.Init.Timing = 0x00000E14;
		break;
	case DAL_FRE_400KHZ:
		i2cx.Init.Timing = 0x00000004;
		break;
	case DAL_FRE_1000KHZ:
		i2cx.Init.Timing = 0x00000000;
		break;
	default:		
		return 0;	
	}

	i2cx.Init.OwnAddress1     = device_init->slave_add;
	if(device_init->s_add_size)
		i2cx.Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
	else
		i2cx.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2cx.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cx.Init.OwnAddress2     = 0;
	i2cx.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2cx.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cx.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	ret = HAL_I2C_Init(&i2cx);

	if (HAL_I2CEx_ConfigAnalogFilter(&i2cx, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

		/**Configure Digital filter 
		*/
	if (HAL_I2CEx_ConfigDigitalFilter(&i2cx, 0) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	switch (device_init->i2c_idx)
	{
	case 1:						
		s_i2c1 = i2cx;
		break;
	case 2:						
		s_i2c2 = i2cx;
		break;
	case 3:						
		s_i2c3 = i2cx;
		break;
	default:
		return 0;						
	}	
	
	if(ret == HAL_OK)
		return 1;
	else 
		return 0;


     
}


/**
* @brief  deInitialize the i2c device.
* @param  
* @retval 1:success
          0:fail
*/
void dal_i2c_close(void *device)
{

    i2c_device_t *device_deinit = (i2c_device_t *)device;
    
    switch (device_deinit->i2c_idx)
    {
    case DAL_I2C_ID1:
        if(HAL_I2C_DeInit(&s_i2c1) != HAL_OK)
        {
        }
        break;					
    case DAL_I2C_ID2:
        if(HAL_I2C_DeInit(&s_i2c2) != HAL_OK)
        {
        }
        break;
	
    case DAL_I2C_ID3:
        if(HAL_I2C_DeInit(&s_i2c3) != HAL_OK)
        {
        }
        break;	
    default:
        break;
							
    }	
     
	
}




/**
* @brief  set slave address.
* @param  uint16_t slave_add
* @retval 1:success
          0:fail
*/
uint8_t dal_i2c_set_slaveAdd(i2c_device_t *device,uint16_t slave_add)
{
	  
	uint8_t ret;
    i2cx.Init.OwnAddress1 = slave_add;
		
	ret = HAL_I2C_Init(&i2cx);
		
	if (HAL_I2CEx_ConfigAnalogFilter(&i2cx, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
	    _Error_Handler(__FILE__, __LINE__);
	}

		/**Configure Digital filter 
		*/
	if (HAL_I2CEx_ConfigDigitalFilter(&i2cx, 0) != HAL_OK)
	{
	    _Error_Handler(__FILE__, __LINE__);
	}

	switch (device->i2c_idx)
	{
	case 1:						
		s_i2c1 = i2cx;
		break;
	case 2:						
		s_i2c2 = i2cx;
		break;
	case 3:						
		s_i2c3 = i2cx;
		break;
	default:
		return 0;					
	}	

	if(ret == HAL_OK)
		return 1;
	else 
		return 0;
}


/**
* @brief  set frequency.
* @param  Fre_kHz freq_khz
* @retval 1:success
          0:fail
*/
uint8_t dal_i2c_set_frequency(i2c_device_t *device,dal_frequence freq_khz)
{
	uint8_t ret;
	switch (device->freq_khz)
	{
	case DAL_FRE_10KHZ:
		i2cx.Init.Timing = 0x00008BFF;
		break;
	case DAL_FRE_100KHZ:
		i2cx.Init.Timing = 0x00000E14;
		break;
	case DAL_FRE_400KHZ:
		i2cx.Init.Timing = 0x00000004;
		break;
	case DAL_FRE_1000KHZ:
		i2cx.Init.Timing = 0x00000000;
		break;
	default:
		i2cx.Init.Timing = 0x00000E14;
		break;
	}
		
		
	ret = HAL_I2C_Init(&i2cx);
	
	if (HAL_I2CEx_ConfigAnalogFilter(&i2cx, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

		/**Configure Digital filter 
		*/
	if (HAL_I2CEx_ConfigDigitalFilter(&i2cx, 0) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	switch (device->i2c_idx)
	{
	case 1:						
		s_i2c1 = i2cx;
		break;
	case 2:						
		s_i2c2 = i2cx;
		break;
	case 3:						
		s_i2c3 = i2cx;
		break;
	default:
		return 0;				
	}	
	

	if(ret == HAL_OK)
		return 1;
	else 
		return 0;
  
}

/**
* @brief  master device read an amount of data from slave device in polling mode.
* @param  uint16_t slave_add :slave address.
				  uint8_t *data :pointer to data buffer
					uint16_t length :amount of data to be sent
          uint32_t timeout :timeout duration, eg:1000 = 1s 
* @retval 1:success
          0:fail
*/
uint8_t dal_i2c_master_read(i2c_device_t *device,uint16_t slave_add, uint8_t *data, uint16_t length, uint32_t timeout)
{
	switch (device->i2c_idx )
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Master_Receive(&s_i2c1, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Master_Receive(&s_i2c2, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Master_Receive(&s_i2c3, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}
    
	
}


/**
* @brief  master device write an amount of data to slave device in polling mode.
* @param  uint16_t slave_add :slave address.
				  uint8_t *data :pointer to data buffer
					uint16_t length :amount of data to be sent
          uint32_t timeout :timeout duration, eg:1000 = 1s 
* @retval 1:success
          0:fail
*/

uint8_t dal_i2c_master_write(i2c_device_t *device,uint16_t slave_add, uint8_t *data, uint16_t length,uint32_t timeout)
{
	switch (device->i2c_idx )
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Master_Transmit(&s_i2c1, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Master_Transmit(&s_i2c2, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Master_Transmit(&s_i2c3, slave_add, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}
	
   
}


/**
* @brief  slave device read an amount of data from master device in polling mode.
* @param  uint8_t *data :pointer to data buffer
					uint16_t length :amount of data to be sent
          uint32_t timeout :timeout duration, eg:1000 = 1s 
* @retval 1:success
          0:fail
*/
uint8_t dal_i2c_slave_read(i2c_device_t *device,uint8_t *data, uint16_t length, uint32_t timeout)
{
	
	switch (device->i2c_idx )
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Slave_Receive(&s_i2c1, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Slave_Receive(&s_i2c2, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Slave_Receive(&s_i2c3, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}
}


/**
* @brief  slave device write an amount of data to master device in polling mode.
* @param  uint8_t *data :pointer to data buffer
					uint16_t length :amount of data to be sent
          uint32_t timeout :timeout duration, eg:1000 = 1s 
* @retval 1:success
          0:fail
*/
uint8_t dal_i2c_slave_write(i2c_device_t *device,uint8_t *data, uint16_t length, uint32_t timeout)
{
	
	switch (device->i2c_idx )
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Slave_Transmit(&s_i2c1, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Slave_Transmit(&s_i2c2, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Slave_Transmit(&s_i2c3, data, length, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}
}



/**
* @brief  master read an amount of data from memery slave device in polling mode.
* @param  i2c_m_t *i2c_m: parameter list
* @retval 1:success
          0:fail
*/

uint8_t dal_i2c_master_mem_read(i2c_device_t *device,uint16_t slave_add,uint16_t mem_add,dal_i2c_memadd_size mem_add_size,uint8_t *data,uint16_t size,uint32_t timeout)
{
	switch (device->i2c_idx )
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Mem_Read(&s_i2c1, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Mem_Read(&s_i2c2, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Mem_Read(&s_i2c3, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}

}


/**
* @brief  master write an amount of data to memery slave device in polling mode.
* @param  i2c_m_t *i2c_m: parameter list
* @retval 1:success
          0:fail
*/

uint8_t dal_i2c_master_mem_write(i2c_device_t *device,uint16_t slave_add,uint16_t mem_add,dal_i2c_memadd_size mem_add_size,uint8_t *data,uint16_t size,uint32_t timeout)
{
    switch (device->i2c_idx)
	{
	case DAL_I2C_ID1:
		if(HAL_I2C_Mem_Write(&s_i2c1, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID2:
		if(HAL_I2C_Mem_Write(&s_i2c2, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	case DAL_I2C_ID3:
		if(HAL_I2C_Mem_Write(&s_i2c3, slave_add, mem_add, mem_add_size, data, size, timeout) == HAL_OK)
			return 1;
		else
			return 0;
	default:
		return 0;
	}
}



s32_t dal_i2c_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t result = 0;
    i2c_device_t *device = (i2c_device_t *)pri;
    (void)offset;
    
    switch(device->iotype)
    {
    case I2C_IO_MASTER_ORIGIN:
        result = dal_i2c_master_read(device,device->slave_add,buf,len, timeout);
        break;
    case I2C_IO_SLAVE_ORIGIN:
        result = dal_i2c_slave_read(device, buf,len,timeout);
        break;
    case I2C_IO_MASTER_MEM:
        result = dal_i2c_master_mem_read(device,device->slave_add,
            device->mem_add,device->mem_size, buf,len, timeout);
        break;
    default:
        return 0;    
    }

    return result;
    
}

s32_t  dal_i2c_write(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t result = 0;
    i2c_device_t *device = (i2c_device_t *)pri;
    (void)offset;
    switch(device->iotype)
    {
    case I2C_IO_MASTER_ORIGIN:
        result = dal_i2c_master_write(device,device->slave_add,buf,len, timeout);
        break;
    case I2C_IO_SLAVE_ORIGIN:
        result = dal_i2c_slave_write(device, buf,len,timeout);
        break;
    case I2C_IO_MASTER_MEM:
        result = dal_i2c_master_mem_write(device,device->slave_add,
            device->mem_add,device->mem_size, buf,len, timeout);
        break;
    default:
        return 0;    
    }

    return result;
}


bool_t dal_i2c_ioctl(void *pri,u32_t cmd, void *para,s32_t len)
{
    int result = 0;
    switch(cmd)
    {
    case I2C_SET_SLAVE_ADDRESS:
        result = dal_i2c_set_slaveAdd((i2c_device_t *)pri, *(uint16_t *)para);
        break;
     case I2C_SET_FREQUENCY:
        result = dal_i2c_set_frequency((i2c_device_t *)pri, *(dal_frequence *)para);
        break;
     default:
        return 0;
    }

    return result;
}

























