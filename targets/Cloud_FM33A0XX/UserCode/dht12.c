#include "dht12.h"
void DHT12_Rst(void)	   
{                 
	//DHT12_DQ_MODE_OUTPUT(); 	 
	//OutputIO(GPIOA, GPIO_Pin_10,0);
	GPIOA->INEN &= (~(1 << 10));
	GPIOA->FCR &= 0XFFCFFFFF;
	GPIOA->FCR |= 0X00100000;
	DHT12_DQ_WRITE(0); 	 
	//HAL_Delay(20);    	 
	TicksDelayMs(20, NULL);//????
	DHT12_DQ_WRITE(1); 
	//LOS_DelayUs(30);  
	TicksDelayUs(30);
}

UINT8 DHT12_Check(void) 	   
{   
	UINT8 retry=0;
	
	//InputtIO(GPIOA, GPIO_Pin_10,0);
	GPIOA->INEN |= (1 << 10);
	GPIOA->ODEN |= (1 << 10);
	GPIOA->FCR &= 0XFFCFFFFF;
	
//	if(DHT12_DQ_READ()){
//		return 1;
//	}else{
//		return 0;
//	}
	while (DHT12_DQ_READ()&& (retry<100)) 
	{
		retry++;
		//LOS_DelayUs(1);
		TicksDelayUs(1);
	};	 
	if(retry>=100){
		return 1;
	}
	else retry=0;
	while (!DHT12_DQ_READ()&&(retry<100)) 
	{
		retry++;
		//LOS_DelayUs(1);
		TicksDelayUs(1);
	};
	if(retry>=100){
		return 1;	    
	}
	return 0;
}

UINT8 DHT12_Read_Bit(void) 			 
{
 	UINT8 retry=0;
	while(DHT12_DQ_READ()&&retry<100) 
	{
		retry++;
		//LOS_DelayUs(1);
		TicksDelayUs(1);
	}
	retry=0;
	while(!DHT12_DQ_READ()&&retry<100) 
	{
		retry++;
		//LOS_DelayUs(1);
		TicksDelayUs(1);
	}
	//LOS_DelayUs(40); 
	TicksDelayUs(40);
	if(DHT12_DQ_READ())return 1;
	else return 0;		   
}

UINT8 DHT12_Read_Byte(void)    
{        
	UINT8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT12_Read_Bit();
    }						    
    return dat;
}

/*TEMP: -20~60бу*/
/*HUMIDITY: 20%~95%*/
/* return:  0 init ok , !0 not ok*/

UINT8 DHT12_Read_Data(INT8 *temp,UINT8 *humi)    
{        
 	UINT8 buf[5]={0x00};
	UINT8 i;
	DHT12_Rst();
	if(DHT12_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT12_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
            ((buf[3]&0x80) == 0x80)?(*temp=-buf[2]):(*temp=buf[2]);		
		}
	}else return 1;
	return 0;	    
}
 	 
UINT8 DHT12_Init(void)
{	
	RCC_PERCLK_SetableEx( PDCCLK, ENABLE );		//IO?????????
	OutputIO(GPIOA, GPIO_Pin_10,0);
	DHT12_DQ_WRITE(1);
    DHT12_Rst();
	return DHT12_Check();
}
