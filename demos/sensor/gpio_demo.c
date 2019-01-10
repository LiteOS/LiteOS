#include "uds/uds_gpio.h"
#include "uds/uds.h"
#include "stdio.h"




uds_gpio_init_t wakeup;
uds_gpio_init_t led;
uds_dev_t device_wakeup;
uds_dev_t device_led;

void user_exit_callback(uint16_t GPIO_PIN)
{
    if(GPIO_PIN == GPIO_PIN_0)
    {
        uint8_t ret = 0,data;
        printf("enter PIN0 IT.\r\n");
        ret = uds_dev_read(device_wakeup,0,&data,1,0);
        if(ret)
            printf("wakeup level is %d \r\n",data);
    }
}

void demo_gpio (void)
{
    s32_t ret;
    u8_t data;
    u32_t cmd;

    wakeup.gpio_port_num = UDS_GPIO_A;
    wakeup.gpio_pin_num = UDS_PIN_0;
    wakeup.gpio_mode = UDS_GPIO_IT_RISING_FALLING;
    wakeup.uds_PreemptionPriority = 0;
    wakeup.uds_SubPriority = 0;


    led.gpio_port_num = UDS_GPIO_C;
    led.gpio_pin_num = UDS_PIN_11;
    led.gpio_mode = UDS_GPIO_OUTPUT_OD;
    led.gpio_pull = UDS_GPIO_NOPULL;

    
    uds_driv_init();
    uds_gpio_dev_install("gpio_wakeup", (void *)&wakeup);
    uds_gpio_dev_install("gpio_led", (void *)&led);

    device_wakeup = uds_dev_open("gpio_wakeup",2);
    if(!device_wakeup)
        while(1);
    device_led = uds_dev_open("gpio_led",2);
    if(!device_led)
        while(1);
    cmd = GPIO_SET_EXITCALLBACK;
    uds_dev_ioctl(device_wakeup,cmd,user_exit_callback,0);
     
    cmd = GPIO_CLEAR_PIN;
    uds_dev_ioctl(device_led,cmd,NULL,0);
    ret = uds_dev_read(device_led,0,&data,1,0);
    if(ret)
        printf("led level is %d \r\n",data);

    osDelay(1000);

    cmd = GPIO_SET_PIN;
    uds_dev_ioctl(device_led,cmd,NULL,0);
    ret = uds_dev_read(device_led,0,&data,1,0);
    if(ret)
        printf("led level is %d \r\n",data);


    while (1)
    {
        cmd = GPIO_TOGGLE_PIN;
        uds_dev_ioctl(device_led,cmd,NULL,0);
        osDelay(1000);
        ret = uds_dev_read(device_wakeup,0,&data,1,0);
        if(ret)
            printf("wakeup level is %d \r\n",data);
        ret = uds_dev_read(device_led,0,&data,1,0);
        if(ret)
            printf("led level is %d \r\n",data);
     }

}

