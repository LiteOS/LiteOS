#ifndef __timer_H
#define __timer_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32l4xx_hal.h"

#include "osdepends/osport.h"

typedef enum
{
    UDS_TIME1 = 1,//advance timer
    UDS_TIME2 = 2,//general timer group 1
    UDS_TIME3 = 3,//general timer group 1
    UDS_TIME4 = 4,//general timer group 1
    UDS_TIME5 = 5,//general timer group 1
    UDS_TIME6 = 6,//basic timer
    UDS_TIME7 = 7,//basic timer
    UDS_TIME8 = 8,//advance timer
    UDS_TIME15 = 15 ,//general timer group 2
    UDS_TIME16 = 16,//general timer group 2
    UDS_TIME17 = 17,//general timer group 2
    

}uds_time_x_type;

typedef enum
{
    UDS_UP = 1,
    UDS_DOWN,


}uds_time_countmode;

typedef enum
{
    TIMER_IT_START,
    TIMER_IT_STOP,
}uds_timer_cmd_t;

typedef struct time_counter_init_s
{
    uds_time_x_type uds_time_x;
    uint32_t uds_period;//0~65535
    uint32_t uds_prescaler;//0~65535 the timint time = (prescaler+1)* (period+1)/timeclk
    uint32_t uds_clockdivision;//time clock frequency.
    uds_time_countmode countmode;
    uint8_t it_SubPriority;//if chooses the interrupt mode, set this value.
    uint8_t it_PreemptionPriority;//if chooses the interrupt mode, set this value.
    

}uds_timer_init_t;

/*
typedef enum
{
    
      
}uds_timer_cmd;
*/
bool_t uds_timer_dev_install(const char *name, void *pri);

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
