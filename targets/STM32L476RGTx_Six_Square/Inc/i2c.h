#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"



typedef enum
{
  
  DAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  DAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
 
}dal_i2c_modetype;


typedef enum
{
   DAL_I2C_7BIT_ADD = 0,
   DAL_I2C_10BIT_ADD = 1,

}dal_i2c_slave_add_size;

typedef enum
{
  DAL_I2C_MEMADD_8BITS,
  DAL_I2C_MEMADD_16BITS,
}dal_i2c_memadd_size;

typedef enum
{
  
  DAL_I2C_ID1             = 1,   /*!< I2C1       */
  DAL_I2C_ID2             = 2,   /*!< I2C2        */
  DAL_I2C_ID3             = 3,   /*!< I2C3        */
	
}dal_i2c_idx;

/*当前参数仅适用于系统时钟为4MHz的场合，其他时钟频率请参照cubemx根据系统时钟频率中生成的数值配置iic初始化的timeing系数*/
typedef enum
{
  DAL_FRE_10KHZ = 0,
  DAL_FRE_100KHZ = 1,
  DAL_FRE_400KHZ = 2,
  DAL_FRE_1000KHZ =3,
}dal_frequence;


/* parameter list when master device exchanges data with memory slave device*/

/*
struct i2c_mem_s
{
	uint16_t slave_add; //slave address
	uint16_t mem_add; // memery address
	dal_i2c_memadd_size mem_add_size; // memery address size 
	uint8_t *data;
	uint16_t size;
	uint32_t timeout;
};typedef struct i2c_mem_s i2c_m_t;
*/
struct i2c_init_s
{
	
    dal_i2c_idx  i2c_idx; 
	  dal_frequence freq_khz;//speed ,eg: 10(khz),100(khz),400(khz),1000(khz)
	  dal_i2c_modetype  mode;//master mode or slave mode
	  uint16_t slave_add;//address when initializes as a slave device
	  dal_i2c_slave_add_size s_add_size;//the size of slave address, you can choose 7bits or 10bits.
	

};typedef struct i2c_init_s i2c_init_t;


extern uint8_t dal_i2c_init(i2c_init_t *i2c_init);
extern uint8_t dal_i2c_deInit(i2c_init_t *i2c_init);
extern uint8_t dal_i2c_set_slaveAdd(i2c_init_t *i2c_init,uint16_t slave_add);
extern uint8_t dal_i2c_set_frequency(i2c_init_t *i2c_init,dal_frequence freq_khz);
extern uint8_t dal_i2c_master_read(i2c_init_t *i2c_init,uint16_t slave_add, uint8_t *data, uint16_t length, uint32_t timeout);
extern uint8_t dal_i2c_master_write(i2c_init_t *i2c_init,uint16_t slave_add, uint8_t *data, uint16_t length,uint32_t timeout);
extern uint8_t dal_i2c_slave_read(i2c_init_t *i2c_init,uint8_t *data, uint16_t length, uint32_t timeout);
extern uint8_t dal_i2c_slave_write(i2c_init_t *i2c_init,uint8_t *data, uint16_t length, uint32_t timeout);
extern uint8_t dal_i2c_master_mem_read(i2c_init_t *i2c_init,uint16_t slave_add,uint16_t mem_add,dal_i2c_memadd_size mem_add_size,uint8_t *data,uint16_t size,uint32_t timeout);
extern uint8_t dal_i2c_master_mem_write(i2c_init_t *i2c_init,uint16_t slave_add,uint16_t mem_add,dal_i2c_memadd_size mem_add_size,uint8_t *data,uint16_t size,uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/