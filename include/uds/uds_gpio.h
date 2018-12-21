#ifndef __uds_gpio_H
#define __uds_gpio_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Includes ------------------------------------------------------------------*/
#include "osdepends/osport.h"


typedef enum
{
    UDS_GPIO_A =0,
    UDS_GPIO_B ,
    UDS_GPIO_C ,
    UDS_GPIO_D ,
    UDS_GPIO_E ,
    UDS_GPIO_F ,
    UDS_GPIO_G ,
    UDS_GPIO_H ,
}uds_gpio_port_type;

typedef enum
{
    UDS_PIN_0 =0,
    UDS_PIN_1 ,
    UDS_PIN_2 ,
    UDS_PIN_3 ,
    UDS_PIN_4 ,
    UDS_PIN_5 ,
    UDS_PIN_6 ,
    UDS_PIN_7 ,
    UDS_PIN_8 ,
    UDS_PIN_9 ,
    UDS_PIN_10 ,
    UDS_PIN_11 ,
    UDS_PIN_12 ,
    UDS_PIN_13 ,
    UDS_PIN_14 ,
    UDS_PIN_15 ,
}uds_gpio_pin_type;




typedef enum
{
    UDS_GPIO_INPUT = 0,
    UDS_GPIO_OUTPUT_PP,
    UDS_GPIO_OUTPUT_OD ,
    UDS_GPIO_IT_RISING ,
    UDS_GPIO_IT_FALLING ,
    UDS_GPIO_IT_RISING_FALLING ,
}uds_gpio_mode_type;



typedef enum
{
    UDS_GPIO_NOPULL = 0,
    UDS_GPIO_PULLUP,
    UDS_GPIO_PULLDOWN,
}uds_gpio_pull_type;





typedef struct gpio_init_s
{
    uds_gpio_port_type gpio_port_num;
    uds_gpio_pin_type gpio_pin_num;
    uds_gpio_mode_type gpio_mode;
    uds_gpio_pull_type gpio_pull;
    uint8_t uds_SubPriority;//if chooses the interrupt mode, set this value.
    uint8_t uds_PreemptionPriority;//if chooses the interrupt mode, set this value.
}uds_gpio_init_t;



typedef enum
{
    GPIO_TOGGLE_PIN,
    GPIO_SET_PIN,
    GPIO_CLEAR_PIN,
    GPIO_SET_EXITCALLBACK,
    GPIO_MAX_CMD,
}uds_gpio_cmd;
/*
struct gpio_device_s
{
    uds_gpio_port_type gpio_port_num;
    uds_gpio_pin_type gpio_pin_num;
    uds_gpio_mode_type gpio_mode;
    uds_gpio_pull_type gpio_pull;
    uint8_t uds_SubPriority;//if chooses the interrupt mode, set this value.
    uint8_t uds_PreemptionPriority;//if chooses the interrupt mode, set this value.
};
typedef struct gpio_device_s gpio_device_t;
*/

//extern bool_t uds_gpio_open(void *device, s32_t flag);
//extern void uds_gpio_close(void *device);
//extern bool_t uds_gpio_ioctl(void *pri,u32_t cmd, void *para,s32_t len);
//extern s32_t uds_gpio_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
//extern s32_t  uds_gpio_write(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);

bool_t uds_gpio_dev_install(const char *name, void *pri);




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

