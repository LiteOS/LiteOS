#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "osdepends/osport.h"



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


typedef enum
{
    I2C_IO_MASTER_ORIGIN,
    I2C_IO_SLAVE_ORIGIN,
    I2C_IO_MASTER_MEM,
    I2C_IO_TYPE_INVALID,
}dal_i2c_iotype;



struct i2c_init_s
{
    dal_i2c_idx              i2c_idx; 
    dal_frequence            freq_khz;//speed ,eg: 10(khz),100(khz),400(khz),1000(khz)
    dal_i2c_modetype         mode;//master mode or slave mode
    u16_t                    slave_add;//address when initializes as a slave device
    dal_i2c_slave_add_size   s_add_size;//the size of slave address, you can choose 7bits or 10bits.
};
typedef struct i2c_init_s i2c_init_t;



typedef enum
{
    I2C_SET_SLAVE_ADDRESS,
    I2C_SET_FREQUENCY,
    I2C_SET_IOTYPE,
    I2C_SET_SLAVE_READ_ADD,
    I2C_SET_SLAVE_WRITE_ADD,
    I2C_SET_MEM_ADD,
    I2C_SET_MEM_SIZE,
    I2C_MAX_CMD,
}dal_i2c_cmd;

bool_t uds_i2c_dev_install(const char *name, void *pri);

#ifdef __cplusplus
}
#endif

#endif 
