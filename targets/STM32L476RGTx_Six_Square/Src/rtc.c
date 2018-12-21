#include "uds/uds_rtc.h"
#include "uds/uds.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_rtc.h"
#include "stm32l4xx_hal_rtc_ex.h"
#include "string.h"

struct rtc_device_s
{
    rtc_init_t               		rtc_init;

};
typedef struct rtc_device_s rtc_device_t;

RTC_HandleTypeDef s_rtc0;
RTC_HandleTypeDef *rtcx;

extern void free(void *ptr);
extern void *malloc(size_t size);



void stringTOtime(char* string,RTC_TimeTypeDef* sTime,	RTC_DateTypeDef *sDate)
{
	uint8_t year=0,month=0,day=0,hour=0,minitue=0,second=0;
	//2018-11-12 12:13:14
	if(strlen(string)<19)return;
	
	year = (string[0]-'0')*1000+(string[1]-'0')*100+(string[2]-'0')*10+(string[3]-'0')-2000;//2018 -2000
	month= (string[5]-'0')*10+(string[6]-'0');//11
	day	 = (string[8]-'0')*10+(string[9]-'0');
	hour = (string[11]-'0')*10+(string[12]-'0');
	minitue=(string[14]-'0')*10+(string[15]-'0');
	second=(string[17]-'0')*10+(string[18]-'0');
	
	sTime->Hours=hour;
	sTime->Minutes=minitue;
	sTime->Seconds=second;
	sTime->DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
	sTime->StoreOperation=RTC_STOREOPERATION_RESET;
	
	sDate->Year=year;
	sDate->Month=month;
	sDate->Date=day;
	//sDate->WeekDay=
	
}

void stringTOalarm(char* string,RTC_AlarmTypeDef* sAlarm)
{
	uint8_t hour=0,minitue=0,second=0;
	//2018-11-12 12:13:14
	if(strlen(string)<19)return;

	hour = (string[11]-'0')*10+(string[12]-'0');
	minitue=(string[14]-'0')*10+(string[15]-'0');
	second=(string[17]-'0')*10+(string[18]-'0');
	
	sAlarm->AlarmTime.Hours=hour;
	sAlarm->AlarmTime.Minutes=minitue;
	sAlarm->AlarmTime.Seconds=second;
  sAlarm->AlarmTime.SubSeconds = 0;
  sAlarm->AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm->AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm->AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES;//
  sAlarm->AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL; //×ÓÃë ÆÁ±Î È«²¿
  sAlarm->AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm->AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;//1;
  sAlarm->Alarm = RTC_ALARM_A;
	
}


void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}


/**
* @brief  set TIME
* @param  
* @retval 1:success
          0:fail
*/
static s32_t uds_rtc_set_time(rtc_device_t *device,char* string)
{
 
    //uint8_t ret;
    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }
		RTC_TimeTypeDef sTime={0};	
		RTC_DateTypeDef sDate={0};
		stringTOtime(string,&sTime,&sDate);
		
	if (HAL_RTC_SetTime(rtcx, &sTime,RTC_FORMAT_BIN) == HAL_OK)
        return UDS_OK;
    else 
        return -UDS_ERROR;
}

/**
* @brief  set Date
* @param  
* @retval 1:success
          0:fail
*/
static s32_t uds_rtc_set_date(rtc_device_t *device,char* string)
{
 
    //uint8_t ret;

    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }

		RTC_TimeTypeDef sTime={0};	
		RTC_DateTypeDef sDate={0};
		stringTOtime(string,&sTime,&sDate);

	if (HAL_RTC_SetDate(rtcx, &sDate,RTC_FORMAT_BIN) == HAL_OK)
        return UDS_OK;
    else 
        return -UDS_ERROR;
}

/**
* @brief  set ALARM
* @param  
* @retval 1:success
          0:fail
*/
static s32_t uds_rtc_set_alarm(rtc_device_t *device,char* string)
{
 
    //uint8_t ret;

    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }

	RTC_AlarmTypeDef sAlarm;
	stringTOalarm(string,&sAlarm);
		
	if (HAL_RTC_SetAlarm_IT(rtcx, &sAlarm,RTC_FORMAT_BIN) == HAL_OK)
        return UDS_OK;
    else 
        return -UDS_ERROR;
}

/**
* @brief  set WKUP
* @param  
* @retval 1:success
          0:fail
*/
static s32_t uds_rtc_set_wakeup_timer(rtc_device_t *device,uint8_t WakeUpCounter)
{
 
    //uint8_t ret;

    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }


	if (HAL_RTCEx_SetWakeUpTimer_IT(rtcx, WakeUpCounter, RTC_WAKEUPCLOCK_RTCCLK_DIV16) == HAL_OK)
        return UDS_OK;
  else 
        return -UDS_ERROR;
}



/**
* @brief  get time
* @param  
* @retval 1:success
          0:fail
*/
static s32_t uds_rtc_get_time(rtc_device_t *device,char* buff,s32_t len)
{
 
    //uint8_t ret;

    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }
		if(len<9)return -UDS_ERROR;
		
		//11:22:33
	RTC_TimeTypeDef sTime={0};
	if (HAL_RTC_GetTime(rtcx, &sTime,RTC_FORMAT_BIN) != HAL_OK)return -UDS_ERROR;
	sprintf(buff,"%d:%d:%d",sTime.Hours,sTime.Minutes,sTime.Seconds);
	return UDS_OK;
}
/**
* @brief  get date
* @param  
* @retval 1:success
          0:fail
*/

static s32_t uds_rtc_get_date(rtc_device_t *device,char* buff,s32_t len)
{
 
    //uint8_t ret;

    switch (device->rtc_init.rtc_idx)
    {
    case 0:
        rtcx = &s_rtc0;
        break;
    default:
        return -UDS_ERROR;
    }
		
		if(len<10)return  -UDS_ERROR;
		//2018-11-12
	RTC_DateTypeDef sDate={0};

	if (HAL_RTC_GetDate(rtcx, &sDate,RTC_FORMAT_BIN) != HAL_OK)
        return -UDS_ERROR;
		sprintf(buff,"%d-%d-%d",sDate.Year,sDate.Month,sDate.Date);
	return 0;
}

static s32_t uds_rtc_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t result = 0;
    //rtc_device_t *device = (rtc_device_t *)pri;
    (void)offset;
	(void)timeout;
	
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
    if (HAL_RTC_GetTime(rtcx, &sTime,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_RTC_GetDate(rtcx, &sDate,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if(len<=20)
		result=-UDS_ERROR;
	else
		sprintf((char*)buf,"%d-%d-%d %d:%d:%d",sDate.Year+2000,sDate.Month,sDate.Date,sTime.Hours,sTime.Minutes,sTime.Seconds);
    return result;
    
}

static s32_t  uds_rtc_write(void *pri,u32_t offset,u8_t *string,s32_t len,u32_t timeout)
{
    s32_t result = 0;
   // rtc_device_t *device = (rtc_device_t *)pri;
   (void)offset;
	 (void)timeout;
		RTC_TimeTypeDef sTime={0};	
		RTC_DateTypeDef sDate={0};
		
		if(len<19)return -UDS_ERROR;
		stringTOtime((char*)string,&sTime,&sDate);

	if (HAL_RTC_SetTime(rtcx, &sTime,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_RTC_SetDate(rtcx, &sDate,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

    return result;
}



static bool_t uds_rtc_ioctl(void *pri,u32_t cmd, void *para,s32_t len)
{
    bool_t result = true;
    rtc_device_t *device = (rtc_device_t *)pri; 
    switch(cmd)
    {
    case RTC_SET_TIME:
        result = uds_rtc_set_time(device,(char*)para);
        break;
     case RTC_SET_DATE:
        result = uds_rtc_set_date(device,(char*)para);
        break;
     case RTC_SET_ALARM:
        result = uds_rtc_set_alarm(device,(char*)para);
        break;
     case RTC_SET_WKUP_TIME:
				if(len!=1)return -UDS_ERROR;
        result = uds_rtc_set_wakeup_timer(device,(int)para);
        break;
     case RTC_GET_TIME:
        result = uds_rtc_get_time(device,(char*)para,len);
        break;
     case RTC_GET_DATE:
        result = uds_rtc_get_date(device,(char*)para,len);
        break;
     default:
        break;
    }
    return result;
}


/**
* @brief  Initializes the rtc device
* @param  rtc_device_t *device : parameter list
* @retval 1:success
          0:fail
*/
static bool_t uds_rtc_open(void *pri, s32_t flag)
{
    return UDS_OK;
}


/**
* @brief  deInitialize the rtc device.
* @param  
* @retval 1:success
          0:fail
*/
static void uds_rtc_close(void *pri)
{
    
}


static bool_t uds_rtc_init(void *pri)
{
    HAL_StatusTypeDef ret;
    rtc_device_t *device = (rtc_device_t *)pri;
    
    switch (device->rtc_init.rtc_idx)
    {
    case UDS_RTC_ID0:
        rtcx = &s_rtc0;
        rtcx->Instance = RTC;
        break;
   
    default:
        return -UDS_ERROR;
    }

    switch (device->rtc_init.rtc_clk)
    {
    case UDS_RTC_LSE_32768Hz:
        rtcx->Init.AsynchPrediv = 127;
		rtcx->Init.SynchPrediv	= 255;
        break;
    case UDS_RTC_LSI_32KHz:
        rtcx->Init.AsynchPrediv = 127;
		rtcx->Init.SynchPrediv	= 249;
        break;
    case UDS_RTC_HSE_1MHz:
        rtcx->Init.AsynchPrediv = 124;
		rtcx->Init.SynchPrediv	= 7999;
        break;
    case UDS_RTC_LSI_37KHz:
        rtcx->Init.AsynchPrediv = 124;
		rtcx->Init.SynchPrediv	= 295;
        break;
    default:
        rtcx->Init.AsynchPrediv = 127;
		rtcx->Init.SynchPrediv	= 255;
        break;
    }

	switch (device->rtc_init.rtc_hour_format)
    {
    case UDS_RTC_Hour_Format_24:
        rtcx->Init.HourFormat  		= RTC_HOURFORMAT_24;
        break;
    case UDS_RTC_Hour_Format_12:
        rtcx->Init.HourFormat  		= RTC_HOURFORMAT_12;
        break;
    default:
        rtcx->Init.HourFormat  		= RTC_HOURFORMAT_24;
        break;
    }

	switch (device->rtc_init.rtc_output)
    {
    case UDS_RTC_OUTPUT_DISABLE:
        rtcx->Init.OutPut  		= RTC_OUTPUT_DISABLE;
        break;
    case UDS_RTC_OUTPUT_ALARMA:
        rtcx->Init.OutPut  		= RTC_OUTPUT_ALARMA;
        break;
    case UDS_RTC_OUTPUT_ALARMB:
        rtcx->Init.OutPut  		= RTC_OUTPUT_ALARMB;
        break;
    case UDS_RTC_OUTPUT_WAKEUP:
        rtcx->Init.OutPut  		= RTC_OUTPUT_WAKEUP;
        break;
    default:
        rtcx->Init.OutPut  		= RTC_OUTPUT_DISABLE;
        break;
    }


	switch (device->rtc_init.rtc_output_remap)
    {
    case UDS_RTC_OUTPUT_REMAP_NONE:
        rtcx->Init.OutPutRemap  		= RTC_OUTPUT_REMAP_NONE;
        break;
    case UDS_RTC_OUTPUT_REMAP_POS1:
        rtcx->Init.OutPutRemap  		= RTC_OUTPUT_REMAP_POS1;
        break;
    default:
        rtcx->Init.OutPutRemap  		= RTC_OUTPUT_REMAP_NONE;
        break;
    }


	switch (device->rtc_init.rtc_output_polarity)
    {
    case UDS_RTC_OUTPUT_POLARITY_HIGH:
        rtcx->Init.OutPutPolarity  		= RTC_OUTPUT_POLARITY_HIGH;
        break;
    case UDS_RTC_OUTPUT_POLARITY_LOW:
        rtcx->Init.OutPutPolarity  		= RTC_OUTPUT_POLARITY_LOW;
        break;
    default:
        rtcx->Init.OutPutPolarity  		= RTC_OUTPUT_POLARITY_HIGH;
        break;
    }

	switch (device->rtc_init.rtc_output_type)
    {
    case UDS_RTC_OUTPUT_TYPE_OPENDRAIN:
        rtcx->Init.OutPutType  		= RTC_OUTPUT_TYPE_OPENDRAIN;
        break;
    case UDS_RTC_OUTPUT_TYPE_PUSHPULL:
        rtcx->Init.OutPutType  		= RTC_OUTPUT_TYPE_PUSHPULL;
        break;
    default:
        rtcx->Init.OutPutType  		= RTC_OUTPUT_TYPE_OPENDRAIN;
        break;
    }
	

    ret = HAL_RTC_Init(rtcx);
    if(ret == HAL_OK)
        return UDS_OK;
    else 
        return -UDS_ERROR;

}


static void uds_rtc_deinit(void *pri)
{
    rtc_device_t *device = (rtc_device_t *)pri;

    switch (device->rtc_init.rtc_idx)
    {
    case UDS_RTC_ID0:
        if(HAL_RTC_DeInit(&s_rtc0) != HAL_OK)
        {
        }
        break;

    default:
        break;
    }

    free(device);
}

static uds_driv_op_t uds_opt = {
    .open     = uds_rtc_open,
    .read     = uds_rtc_read,
    .write    = uds_rtc_write,
    .close    = uds_rtc_close,
    .ioctl    = uds_rtc_ioctl,
    .init     = uds_rtc_init,
    .deinit   = uds_rtc_deinit,
};


bool_t uds_rtc_dev_install(const char *name, void *pri)
{
    rtc_device_t *device = (rtc_device_t *)malloc(sizeof(rtc_device_t));
    memset(device, 0, sizeof(rtc_device_t));
    memcpy(&device->rtc_init,pri,sizeof(rtc_init_t));
	
	
    if(!uds_driv_register(name,&uds_opt,device,0))
    {
        return -UDS_ERROR;
    }
    return UDS_OK;
}











