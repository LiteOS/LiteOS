#include "uds/uds.h"
#include "uds_gpio.h"
#include "stdio.h"

extern void free(void *ptr);
extern void *malloc(size_t size);



struct gpio_device_s
{
    uds_gpio_init_t        gpio_init;
    uds_gpio_cmd           iotype;


};
typedef struct gpio_device_s gpio_device_t;



GPIO_InitTypeDef GPIO_InitStruct;



/**
  * @brief  set an output pin to be high level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval -1:input parameters error 
  * @retval  0:success
  */
s32_t uds_gpio_set_output_pin(gpio_device_t *device)
{
    switch (device->gpio_init.gpio_port_num)
    {
        case UDS_GPIO_A:
            HAL_GPIO_WritePin(GPIOA, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_B:
            HAL_GPIO_WritePin(GPIOB, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_C:
            HAL_GPIO_WritePin(GPIOC, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_D:
            HAL_GPIO_WritePin(GPIOD, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_E:
            HAL_GPIO_WritePin(GPIOE, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_F:
            HAL_GPIO_WritePin(GPIOF, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_G:
            HAL_GPIO_WritePin(GPIOG, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        case UDS_GPIO_H:
            HAL_GPIO_WritePin(GPIOH, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_SET);
            break;
        default:
            return -1;
     }
    return 0;
 
}




/**
  * @brief  set an output pin to be low level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval -1:input parameters error 
  * @retval  1:success
  */
s32_t uds_gpio_clear_output_pin(gpio_device_t *device)
{
    switch (device->gpio_init.gpio_port_num)
    {
        case UDS_GPIO_A:
            HAL_GPIO_WritePin(GPIOA, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_B:
            HAL_GPIO_WritePin(GPIOB, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_C:
            HAL_GPIO_WritePin(GPIOC, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_D:
            HAL_GPIO_WritePin(GPIOD, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_E:
            HAL_GPIO_WritePin(GPIOE, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_F:
            HAL_GPIO_WritePin(GPIOF, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_G:
            HAL_GPIO_WritePin(GPIOG, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        case UDS_GPIO_H:
            HAL_GPIO_WritePin(GPIOH, (1<<(device->gpio_init.gpio_pin_num)), GPIO_PIN_RESET);
            break;
        default:
            return -1;
    }  
    return 0;
}

/**
  * @brief  toggle pin level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval -1:input parameters error 
  * @retval 1:success
  */
s32_t uds_gpio_toggle_output_pin(gpio_device_t *device)
{
    switch (device->gpio_init.gpio_port_num)
    {
        case UDS_GPIO_A:
            HAL_GPIO_TogglePin(GPIOA, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_B:
            HAL_GPIO_TogglePin(GPIOB, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_C:
            HAL_GPIO_TogglePin(GPIOC, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_D:
            HAL_GPIO_TogglePin(GPIOD, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_E:
            HAL_GPIO_TogglePin(GPIOE, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_F:
            HAL_GPIO_TogglePin(GPIOF, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_G:
            HAL_GPIO_TogglePin(GPIOG, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_H:
            HAL_GPIO_TogglePin(GPIOH, (1<<(device->gpio_init.gpio_pin_num)));
            break;
        default:
            return -1;
    }  
    return 0;
}

/**
  * @brief  read the level of the input pin.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:low level
  * @retval 1:high level
  * @retval -1:input parameters error
  */
s32_t uds_gpio_read_input_pin(gpio_device_t *device, u8_t *data)
{
   // s32_t ret;
    switch (device->gpio_init.gpio_port_num)
    {
        case 0:
           *data = HAL_GPIO_ReadPin(GPIOA, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 1:
           *data = HAL_GPIO_ReadPin(GPIOB, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 2:
           *data = HAL_GPIO_ReadPin(GPIOC, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 3:
           *data = HAL_GPIO_ReadPin(GPIOD, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 4:
           *data = HAL_GPIO_ReadPin(GPIOE, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 5:
           *data = HAL_GPIO_ReadPin(GPIOF, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 6:
           *data = HAL_GPIO_ReadPin(GPIOG, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 7:
           *data = HAL_GPIO_ReadPin(GPIOH, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        default:
           return -1;
    }
	
}

/**
  * @brief  Read the specified input port pin.
  * @param  GPIOx: where x can be (A..H) to select the GPIO peripheral for STM32L4 family
  * @param  GPIO_Pin: specifies the port bit to read.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadOutputPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState bitstatus;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->ODR & GPIO_Pin) != 0x00u)
    {
        bitstatus = GPIO_PIN_SET;
    }
    else
    {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}
/**
  * @brief  read the level of the output pin.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:low level
  * @retval 1:high level
  * @retval -1:input parameters error
  */
s32_t uds_gpio_read_output_pin(gpio_device_t *device,u8_t *data)
{
   // s32_t ret;
    switch (device->gpio_init.gpio_port_num)
    {
        case 0:
           *data = HAL_GPIO_ReadOutputPin(GPIOA, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 1:
           *data = HAL_GPIO_ReadOutputPin(GPIOB, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 2:
           *data = HAL_GPIO_ReadOutputPin(GPIOC, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 3:
           *data = HAL_GPIO_ReadOutputPin(GPIOD, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 4:
           *data = HAL_GPIO_ReadOutputPin(GPIOE, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 5:
           *data = HAL_GPIO_ReadOutputPin(GPIOF, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 6:
           *data = HAL_GPIO_ReadOutputPin(GPIOG, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        case 7:
           *data = HAL_GPIO_ReadOutputPin(GPIOH, (1<<(device->gpio_init.gpio_pin_num)));
           return 0;
        default:
           return -1;
    }
	
}



static bool_t uds_gpio_open(void *pri, s32_t flag)
{
    return true;
}


/**
* @brief  deInitialize the i2c device.
* @param  
* @retval 1:success
          0:fail
*/
static void uds_gpio_close(void *pri)
{
    
}


/**
  * @brief  initialize the GPIO peripheral according to the specified parameters in the gpio.
  * @param  gpio_init_t *gpio : initialize paremeters list.
    * @retval -1:input parameters error 
    * @retval   0:success
                    
*/


static bool_t uds_gpio_init(void *pri)
{
    volatile s32_t ret;
    uint32_t mode;
    gpio_device_t *device_init = (gpio_device_t *)pri;

    

    switch(device_init->gpio_init.gpio_mode)
    {
        case UDS_GPIO_INPUT:
            mode = GPIO_MODE_INPUT;
            break;
        case UDS_GPIO_OUTPUT_PP:
            mode = GPIO_MODE_OUTPUT_PP;
            break;
        case UDS_GPIO_OUTPUT_OD:
            mode = GPIO_MODE_OUTPUT_OD;
            break;
        case UDS_GPIO_IT_FALLING:
            mode = GPIO_MODE_IT_FALLING;
            break;
        case UDS_GPIO_IT_RISING:
            mode = GPIO_MODE_IT_RISING;
            break;
        case UDS_GPIO_IT_RISING_FALLING:
            mode = GPIO_MODE_IT_RISING_FALLING;
            break;
        default:
            return -1;
    
    }
    switch (device_init->gpio_init.gpio_port_num)
    {
        
        case UDS_GPIO_A:
            __HAL_RCC_GPIOA_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            break;
        case UDS_GPIO_B:
            __HAL_RCC_GPIOB_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            break;
      case UDS_GPIO_C:
            __HAL_RCC_GPIOC_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            break;
        case UDS_GPIO_D:
            __HAL_RCC_GPIOD_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
            break;
        case UDS_GPIO_E:
            __HAL_RCC_GPIOE_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
            break;
        case UDS_GPIO_F:
            __HAL_RCC_GPIOF_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
            break;
        case UDS_GPIO_G:
            __HAL_RCC_GPIOG_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
            break;
        case UDS_GPIO_H:
            __HAL_RCC_GPIOH_CLK_ENABLE();
    
            GPIO_InitStruct.Pin = 1<<(device_init->gpio_init.gpio_pin_num);
            GPIO_InitStruct.Mode = mode;
            GPIO_InitStruct.Pull = device_init->gpio_init.gpio_pull;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
            HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
            break;
        default:
            return -1;
              
    }
    if(device_init->gpio_init.gpio_mode >= 3)
        {
            switch (device_init->gpio_init.gpio_pin_num)
            {
                case 0:
                    HAL_NVIC_SetPriority(EXTI0_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
                    break;
              case 1:
                    HAL_NVIC_SetPriority(EXTI1_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
                    break;
                case 2:
                    HAL_NVIC_SetPriority(EXTI2_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
                    break;
                case 3:
                    HAL_NVIC_SetPriority(EXTI3_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
                    break;
                case 4:
                    HAL_NVIC_SetPriority(EXTI4_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    HAL_NVIC_SetPriority(EXTI9_5_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
                    break;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    HAL_NVIC_SetPriority(EXTI15_10_IRQn, device_init->gpio_init.uds_PreemptionPriority, device_init->gpio_init.uds_SubPriority);
                    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
                    break;
                default:
                    return -1;
            }
        }
        return 1;
}



static void uds_gpio_deinit(void *pri)
{
    gpio_device_t *device_deinit = (gpio_device_t *)pri;
    switch (device_deinit->gpio_init.gpio_port_num)
    {
        case UDS_GPIO_A:
            HAL_GPIO_DeInit(GPIOA, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_B:
            HAL_GPIO_DeInit(GPIOB, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_C:
            HAL_GPIO_DeInit(GPIOC, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_D:
            HAL_GPIO_DeInit(GPIOD, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_E:
            HAL_GPIO_DeInit(GPIOE, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_F:
            HAL_GPIO_DeInit(GPIOF, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_G:
            HAL_GPIO_DeInit(GPIOG, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        case UDS_GPIO_H:
            HAL_GPIO_DeInit(GPIOH, (1<<(device_deinit->gpio_init.gpio_pin_num)));
            break;
        default:
            break;
    }
    free(device_deinit);
}


static s32_t uds_gpio_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t result = 0;
    gpio_device_t *device = (gpio_device_t *)pri;
    (void)offset;
    switch(device->gpio_init.gpio_mode)
    {
        case UDS_GPIO_INPUT:
        case UDS_GPIO_IT_RISING:
        case UDS_GPIO_IT_FALLING:
        case UDS_GPIO_IT_RISING_FALLING:
            result = uds_gpio_read_input_pin(device,buf);
            break;
        case UDS_GPIO_OUTPUT_PP:
        case UDS_GPIO_OUTPUT_OD:
            result = uds_gpio_read_output_pin(device,buf);
            break;
        default:
            break;
    }
    return result;
}

static s32_t  uds_gpio_write(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    
    return true;
    /*
    s32_t result = 0;
    gpio_device_t *device = (gpio_device_t *)pri;
    (void)offset;
    if(buf[0])
    {
        result = uds_gpio_set_output_pin(device);
    }
    else
    {
        result = uds_gpio_clear_output_pin(device);
    }
    return result;
    */
}





static bool_t uds_gpio_ioctl(void *pri,u32_t cmd, void *para,s32_t len)
{
    bool_t result = true;
    switch(cmd)
    {
        case GPIO_TOGGLE_PIN:
            result = uds_gpio_toggle_output_pin((gpio_device_t *)pri);
            break;
        case GPIO_SET_PIN:
            result = uds_gpio_set_output_pin((gpio_device_t *)pri);
            break;
        case GPIO_CLEAR_PIN:
            result = uds_gpio_clear_output_pin((gpio_device_t *)pri);
            break;
        default:
            break;
    }
    return result;
}


static uds_driv_op_t uds_opt = {
    .open     = uds_gpio_open,
    .close    = uds_gpio_close,
    .read     = uds_gpio_read,
    .write    = uds_gpio_write,
    .ioctl    = uds_gpio_ioctl,
    .init     = uds_gpio_init,
    .deinit   = uds_gpio_deinit,
};

bool_t uds_gpio_dev_install(const char *name, void *pri)
{
    gpio_device_t *device = (gpio_device_t *)malloc(sizeof(gpio_device_t));
    memset(device, 0, sizeof(gpio_device_t));

    memcpy(&device->gpio_init,pri,sizeof(uds_gpio_init_t));
    if(!uds_driv_register(name,&uds_opt,pri,0))
    {
        return -1;
    }
    return 0;
}





extern  gpio_device_t wakeup;


void uds_exti0_callback(void)
{
    uint8_t ret = 0,data;
    printf("enter PIN0 IT.\r\n");
    
    ret = uds_gpio_read(&wakeup,0,&data,0,0);
    if(!ret)
        printf("wakeup level is %d \r\n",data);
    
 
}


void uds_exti1_callback(void)
{
   
 
}
void uds_exti2_callback(void)
{
   
 
}
void uds_exti3_callback(void)
{
   
 
}
void uds_exti4_callback(void)
{
   
 
}
void uds_exti5_callback(void)
{
   
 
}


void uds_exti6_callback(void)
{
   
 
}
void uds_exti7_callback(void)
{
   
 
}
void uds_exti8_callback(void)
{
   
 
}
void uds_exti9_callback(void)
{
   
 
}
void uds_exti10_callback(void)
{
   
 
}


void uds_exti11_callback(void)
{
   
 
}
void uds_exti12_callback(void)
{
   
 
}
void uds_exti13_callback(void)
{
   
 
}
void uds_exti14_callback(void)
{
   
 
}
void uds_exti15_callback(void)
{
   
 
}


void EXTI0_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    uds_exti0_callback();
  }
  
}


void EXTI1_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
            uds_exti1_callback();
        }
}


void EXTI2_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
            uds_exti2_callback();
        }
}
void EXTI3_IRQHandler(void)
{

    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
            uds_exti3_callback();
        }
        
}
void EXTI4_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
            uds_exti4_callback();
        }
}
void EXTI9_5_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
            uds_exti5_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
            uds_exti6_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
            uds_exti7_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
            uds_exti8_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
            uds_exti9_callback();
        }
}
void EXTI15_10_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
            uds_exti10_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
            uds_exti11_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
            uds_exti12_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
            uds_exti13_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
            uds_exti14_callback();
        }
        else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
        {
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
            uds_exti15_callback();
        }
}



