#include "i2c.h"
#include "stdio.h"
#define SHT20_IIC_ADDR    0x80
#define SHT20_READ_ADDR   0X81
#define SHT20_WRITE_ADDR  0x80
#define SHT20_MEASURE_TEMP_CMD      0xE3
#define SHT20_MEASURE_RH_CMD        0xE5
#define SHT20_SOFT_RESET_CMD        0xFE




//******************************************************************************
// fn : SHT20_Convert
//
// brief : 将原始的AD值转成相应的温度值与湿度值
//
// param : value-> ad值
//         isTemp-> >0 温度值，=0 湿度值
//
// return : 直观的温度值
float SHT20_Convert(uint16_t value,uint8_t isTemp)
{
  float tmp = 0.0;
  if(isTemp)
  {
    tmp = -46.85 + (175.72* value)/(1 << 16);
  }
  else
  {
    tmp = -6 + (125.0 *value)/(1<<16);
  }
  return tmp;
}
 i2c_init_t i2c1;
void demo_sht21_iic(void)
{
	 uint8_t ret,cmd;
	 uint16_t data_raw_temp,data_raw_rh;
	 volatile float temp =0 ;
	 uint8_t pDATA[3] = {0,0,0};
	 
	i2c1.i2c_idx = 1;
	i2c1.mode = DAL_I2C_MODE_MASTER;
	i2c1.freq_khz = DAL_FRE_100KHZ;//10khz		
	i2c1.s_add_size = DAL_I2C_7BIT_ADD;

	ret = dal_i2c_init(&i2c1);
		if( !ret)
			while(1); 
	  
	//cmd = SHT20_SOFT_RESET_CMD;
	//ret = dal_i2c_master_write(&i2c1,SHT20_WRITE_ADDR,&cmd,1,10000);
	//if(ret == 0)
//		while(1);
//	HAL_Delay(15);

 while (1)
  {
		
		
		cmd = SHT20_MEASURE_TEMP_CMD;
		ret = dal_i2c_master_write(&i2c1,SHT20_WRITE_ADDR,&cmd,1,10000);
		if(ret == 0)
			while(1);
		ret = dal_i2c_master_read(&i2c1,SHT20_READ_ADDR,pDATA,3,10000);
		if(ret == 0)
			while(1);
		data_raw_temp = pDATA[0];
		data_raw_temp <<= 8;
		data_raw_temp += (pDATA[1] & 0xfc);
		temp = SHT20_Convert(data_raw_temp,1);
		printf("temp = %.1f \r\n",temp);
		temp = 0;
		
		cmd = SHT20_MEASURE_RH_CMD;
   
		ret = dal_i2c_master_write(&i2c1,SHT20_WRITE_ADDR,&cmd,1,10000);
			if(ret == 0)
				while(1);
     ret = dal_i2c_master_read(&i2c1,SHT20_READ_ADDR,pDATA,3,10000);
		if(ret == 0)
			while(1);
  	data_raw_rh = pDATA[0];
		data_raw_rh <<= 8;
		data_raw_rh += (pDATA[1] & 0xf0);
		temp = SHT20_Convert(data_raw_rh,0);
		printf("rh = %.1f \r\n",temp);
		temp = 0;
		
		
  }

}
