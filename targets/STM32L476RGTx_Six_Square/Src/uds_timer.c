#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_tim_ex.h"
#include "uds/uds.h"
#include "uds/uds_timer.h"
#include "stdio.h"

extern void free(void *ptr);
extern void *malloc(size_t size);


struct timer_device_s
{
    uds_timer_init_t        uds_timer_init;
    uds_timer_cmd_t         iotype;
    


};
typedef struct timer_device_s timer_device_t;


TIM_HandleTypeDef s_time1;
TIM_HandleTypeDef s_time2;
TIM_HandleTypeDef s_time3;
TIM_HandleTypeDef s_time4;
TIM_HandleTypeDef s_time5;
TIM_HandleTypeDef s_time6;
TIM_HandleTypeDef s_time7;
TIM_HandleTypeDef s_time8;
TIM_HandleTypeDef s_time15;
TIM_HandleTypeDef s_time16;
TIM_HandleTypeDef s_time17;
TIM_HandleTypeDef *time_x;

uint8_t it_SubPriority_temp;
uint8_t it_PreemptionPriority_temp;



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *timex)
{
    /* Set TIMx instance */
    if(timex->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    }
    else if(timex->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if(timex->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
    else if(timex->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
    else if(timex->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM5_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
    else if(timex->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
    }
    else if(timex->Instance == TIM7)
    {
        __HAL_RCC_TIM7_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM7_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
    else if(timex->Instance == TIM8)
    {
        __HAL_RCC_TIM8_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM8_UP_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    }
    else if(timex->Instance == TIM15)
    {
        __HAL_RCC_TIM15_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    }
    else if(timex->Instance == TIM16)
    {
        __HAL_RCC_TIM16_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    }
    else if(timex->Instance == TIM17)
    {
        __HAL_RCC_TIM17_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, it_PreemptionPriority_temp, it_SubPriority_temp);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
    }
}

static bool_t uds_timer_open(void *pri, s32_t flag)
{
    return UDS_OK;
}


/**
* @brief  deInitialize the i2c device.
* @param  
* @retval 1:success
          0:fail
*/
static void uds_timer_close(void *pri)
{

}


/**
  * @brief  initialize the GPIO peripheral according to the specified parameters in the gpio.
  * @param  gpio_init_t *gpio : initialize paremeters list.
    * @retval -1:input parameters error 
    * @retval   0:success
*/

/*

    Tout= ((arr+1)*(psc+1))/Tclk；

　　其中：

　　　　Tclk：TIM3 的输入时钟频率（单位为 Mhz）。

　　　　Tout：TIM3 溢出时间（单位为 s）。
*/

static bool_t uds_timer_baseinit(void *pri)
{
    volatile s32_t ret;
   
    timer_device_t *device_init = (timer_device_t *)pri;

    switch(device_init->uds_timer_init.uds_time_x)
    {
        case UDS_TIME1:
            time_x = &s_time1;
            time_x->Instance = TIM1;
            break;
        case UDS_TIME2:
            time_x = &s_time2;
            time_x->Instance = TIM2;
            break;
        case UDS_TIME3:
            time_x = &s_time3;
            time_x->Instance = TIM3;
            break;
        case UDS_TIME4:
            time_x = &s_time4;
            time_x->Instance = TIM4;
            break;
        case UDS_TIME5:
            time_x = &s_time5;
            time_x->Instance = TIM5;
            break;
        case UDS_TIME6:
            time_x = &s_time6;
            time_x->Instance = TIM6;
            break;
        case UDS_TIME7:
            time_x = &s_time7;
            time_x->Instance = TIM7;
            break;
        case UDS_TIME8:
            time_x = &s_time8;
            time_x->Instance = TIM8;
            break;
        case UDS_TIME15:
            time_x = &s_time15;
            time_x->Instance = TIM15;
            break;
        case UDS_TIME16:
            time_x = &s_time16;
            time_x->Instance = TIM16;
            break;
        case UDS_TIME17:
            time_x = &s_time17;
            time_x->Instance = TIM17;
            break;
        default:
            return -UDS_ERROR;

    } 
    if(time_x->Init.CounterMode == UDS_UP)
    {
        time_x->Init.CounterMode = TIM_COUNTERMODE_UP;
    }
    else
    {
        time_x->Init.CounterMode = TIM_COUNTERMODE_DOWN;
    }
    time_x->Init.Period            = device_init->uds_timer_init.uds_period;
    time_x->Init.Prescaler         = device_init->uds_timer_init.uds_prescaler;
    time_x->Init.ClockDivision     = device_init->uds_timer_init.uds_clockdivision;
    time_x->Init.RepetitionCounter = 0;
    it_PreemptionPriority_temp = device_init->uds_timer_init.it_PreemptionPriority;
    it_SubPriority_temp = device_init->uds_timer_init.it_SubPriority;

    if (HAL_TIM_Base_Init(time_x) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
    /*
    if (HAL_TIM_Base_Start_IT(time_x) != HAL_OK)
    {
        
        Error_Handler();
    }
    */
    return UDS_OK;
}

static void uds_timer_basedeinit(void *pri)
{
    timer_device_t *device_deinit = (timer_device_t *)pri;
    switch(device_deinit->uds_timer_init.uds_time_x)
    {
        case UDS_TIME1:
            HAL_TIM_Base_DeInit(&s_time1);
            break;
        case UDS_TIME2:
            HAL_TIM_Base_DeInit(&s_time2);
            break;
        case UDS_TIME3:
            HAL_TIM_Base_DeInit(&s_time3);
            break;
        case UDS_TIME4:
            HAL_TIM_Base_DeInit(&s_time4);
            break;
        case UDS_TIME5:
            HAL_TIM_Base_DeInit(&s_time5);
            break;
        case UDS_TIME6:
            HAL_TIM_Base_DeInit(&s_time6);
            break;
        case UDS_TIME7:
            HAL_TIM_Base_DeInit(&s_time7);
            break;
        case UDS_TIME8:
            HAL_TIM_Base_DeInit(&s_time8);
            break;
        case UDS_TIME15:
            HAL_TIM_Base_DeInit(&s_time15);
            break;
        case UDS_TIME16:
            HAL_TIM_Base_DeInit(&s_time16);
            break;
        case UDS_TIME17:
            HAL_TIM_Base_DeInit(&s_time17);
            break;
        default:
            break;
    } 
    free(device_deinit);
}




s32_t uds_start_it(void *pri)
{
    timer_device_t *device = (timer_device_t *)pri;
    uint8_t ret;
    switch(device->uds_timer_init.uds_time_x)
    {
        case UDS_TIME1:
            ret = HAL_TIM_Base_Start_IT(&s_time1);
            break;
        case UDS_TIME2:
            ret = HAL_TIM_Base_Start_IT(&s_time2);
            break;
        case UDS_TIME3:
            ret = HAL_TIM_Base_Start_IT(&s_time3);
            break;
        case UDS_TIME4:
            ret = HAL_TIM_Base_Start_IT(&s_time4);
            break;
        case UDS_TIME5:
            ret = HAL_TIM_Base_Start_IT(&s_time5);
            break;
        case UDS_TIME6:
            ret = HAL_TIM_Base_Start_IT(&s_time6);
            break;
        case UDS_TIME7:
            ret = HAL_TIM_Base_Start_IT(&s_time7);
            break;
        case UDS_TIME8:
            ret = HAL_TIM_Base_Start_IT(&s_time8);
            break;
        case UDS_TIME15:
            ret = HAL_TIM_Base_Start_IT(&s_time15);
            break;
        case UDS_TIME16:
            ret = HAL_TIM_Base_Start_IT(&s_time16);
            break;
        case UDS_TIME17:
            ret = HAL_TIM_Base_Start_IT(&s_time17);
            break;
        default:
            return -UDS_ERROR;
    } 
    if(ret == HAL_OK)
    {
        return UDS_OK;
    }
    else 
    {
        return -UDS_ERROR;
    }

}

s32_t uds_stop_it(void *pri)
{
    timer_device_t *device = (timer_device_t *)pri;
    uint8_t ret;
    switch(device->uds_timer_init.uds_time_x)
    {
        case UDS_TIME1:
            ret = HAL_TIM_Base_Stop_IT(&s_time1);
            break;
        case UDS_TIME2:
            ret = HAL_TIM_Base_Stop_IT(&s_time2);
            break;
        case UDS_TIME3:
            ret = HAL_TIM_Base_Stop_IT(&s_time3);
            break;
        case UDS_TIME4:
            ret = HAL_TIM_Base_Stop_IT(&s_time4);
            break;
        case UDS_TIME5:
            ret = HAL_TIM_Base_Stop_IT(&s_time5);
            break;
        case UDS_TIME6:
            ret = HAL_TIM_Base_Stop_IT(&s_time6);
            break;
        case UDS_TIME7:
            ret = HAL_TIM_Base_Stop_IT(&s_time7);
            break;
        case UDS_TIME8:
            ret = HAL_TIM_Base_Stop_IT(&s_time8);
            break;
        case UDS_TIME15:
            ret = HAL_TIM_Base_Stop_IT(&s_time15);
            break;
        case UDS_TIME16:
            ret = HAL_TIM_Base_Stop_IT(&s_time16);
            break;
        case UDS_TIME17:
            ret = HAL_TIM_Base_Stop_IT(&s_time17);
            break;
        default:
            return -UDS_ERROR;
    } 

    if(ret == HAL_OK)
    {
        return UDS_OK;
    }
    else 
    {
        return -UDS_ERROR;
    }

}

static s32_t uds_timer_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    return true;
}

static s32_t  uds_timer_write(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    return true;
}


 typedef void (*timer_callback_fun)(uds_time_x_type);
timer_callback_fun timer_user_callback;

s32_t uds_set_update_callback(timer_device_t *device,timer_callback_fun para)
{
    timer_user_callback = para;
    return UDS_OK;
}

static bool_t uds_timer_ioctl(void *pri,u32_t cmd, void *para,s32_t len)
{
    bool_t result = true;
	timer_device_t *device = (timer_device_t *)pri; 
    switch(cmd)
    {
        case TIMER_IT_START:
            result = uds_start_it(device);
            break;
        case TIMER_IT_STOP:
            result = uds_stop_it(device);
            break;
        case TIMER_SET_UPDATE_CALLBACK:
            result = uds_set_update_callback(device,(timer_callback_fun)para);
            break;
        default:
            result = -UDS_ERROR;
            break;
    }
    return result;
    
}


static uds_driv_op_t uds_opt = {
    .open     = uds_timer_open,
    .close    = uds_timer_close,
    .read     = uds_timer_read,
    .write    = uds_timer_write,
    .ioctl    = uds_timer_ioctl,
    .init     = uds_timer_baseinit,
    .deinit   = uds_timer_basedeinit,
};

bool_t uds_timer_dev_install(const char *name, void *pri)
{
    timer_device_t *device = (timer_device_t *)malloc(sizeof(timer_device_t));
    memset(device, 0, sizeof(timer_device_t));

    memcpy(&device->uds_timer_init,pri,sizeof(uds_timer_init_t));
    if(!uds_driv_register(name,&uds_opt,device,0))
    {
        return -UDS_ERROR;
    }
    return UDS_OK;
}












void time_update_callback(TIM_HandleTypeDef timhandle, uds_time_x_type timx)
{

	if(__HAL_TIM_GET_FLAG(&timhandle, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_IT_SOURCE(&timhandle, TIM_IT_UPDATE) != RESET)
		{
			__HAL_TIM_CLEAR_IT(&timhandle, TIM_IT_UPDATE);
			timer_user_callback(timx);
		}
	}


}

void TIM1_UP_TIM16_IRQHandler(void)
{
	if( s_time1.Instance!= 0)
	{
		time_update_callback(s_time1,UDS_TIME1);
	}
	else
	{
		time_update_callback(s_time16,UDS_TIME16);
	}
    
}
void TIM1_BRK_TIM15_IRQHandler(void)
{
    time_update_callback(s_time15,UDS_TIME15);
}
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
    time_update_callback(s_time17,UDS_TIME17);
}

void TIM2_IRQHandler(void)
{
    time_update_callback(s_time2,UDS_TIME2);
}
void TIM3_IRQHandler(void)
{
    time_update_callback(s_time3,UDS_TIME3);
}
void TIM4_IRQHandler(void)
{
    time_update_callback(s_time4,UDS_TIME4);
}
void TIM5_IRQHandler(void)
{
    time_update_callback(s_time5,UDS_TIME5);
}
void TIM6_IRQHandler(void)
{
    time_update_callback(s_time6,UDS_TIME6);
}
void TIM7_IRQHandler(void)
{
    time_update_callback(s_time7,UDS_TIME7);
}
void TIM8_UP_IRQHandler(void)
{
    time_update_callback(s_time8,UDS_TIME8);
}

