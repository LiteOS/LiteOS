#include "uds/uds_gpio.h"
#include "uds/uds.h"
#include "stdio.h"




uds_gpio_init_t wakeup;
uds_gpio_init_t led;
uds_gpio_init_t test_inout_pin;

uds_dev_t device_wakeup;
uds_dev_t device_led;
uds_dev_t device_test_inout_pin;

void user_exit_callback(uint16_t GPIO_PIN)
{
    u32_t cmd;
    u8_t data_read,data_write;
    if(GPIO_PIN == GPIO_PIN_0)
    {
        uint8_t ret = 0,data;
        printf("enter PIN0 IT.\r\n");
        ret = uds_dev_read(device_wakeup,0,&data,1,0);
        if(ret)
            printf("wakeup level is %d \r\n",data);

        cmd = UDS_GPIO_SET_OUTPUT;
        uds_dev_ioctl(device_test_inout_pin,cmd,NULL,0);

        data_write = 1;
        uds_dev_write(device_test_inout_pin,0,&data_write,1,0);
    }
}

void demo_gpio (void)
{
    s32_t ret;
    u8_t data_write,data_read;
    u32_t cmd;

    wakeup.gpio_port_num = UDS_GPIO_A;
    wakeup.gpio_pin_num = UDS_PIN_0;
    wakeup.gpio_mode = UDS_GPIO_IT_RISING_FALLING;
    wakeup.gpio_speed = UDS_GPIO_SPEED_HTGH;
    wakeup.uds_PreemptionPriority = 0;
    wakeup.uds_SubPriority = 0;


    led.gpio_port_num = UDS_GPIO_C;
    led.gpio_pin_num = UDS_PIN_11;
    led.gpio_mode = UDS_GPIO_OUTPUT_OD;
    led.gpio_speed = UDS_GPIO_SPEED_HTGH;
    led.gpio_pull = UDS_GPIO_NOPULL;

    test_inout_pin.gpio_port_num = UDS_GPIO_B;
    test_inout_pin.gpio_pin_num = UDS_PIN_11;
    test_inout_pin.gpio_mode = UDS_GPIO_INPUT;
    test_inout_pin.gpio_speed = UDS_GPIO_SPEED_HTGH;
    test_inout_pin.gpio_pull = UDS_GPIO_NOPULL;
    
    uds_driv_init();
    uds_gpio_dev_install("gpio_wakeup", (void *)&wakeup);
    uds_gpio_dev_install("gpio_led", (void *)&led);
    uds_gpio_dev_install("gpio_test_io", (void *)&test_inout_pin);


    device_wakeup = uds_dev_open("gpio_wakeup",2);
    if(!device_wakeup)
        while(1);

    device_led = uds_dev_open("gpio_led",2);
    if(!device_led)
        while(1);

    device_test_inout_pin = uds_dev_open("gpio_test_io",2);
    if(!device_test_inout_pin)
        while(1);

    
    cmd = UDS_GPIO_SET_EXITCALLBACK;
    uds_dev_ioctl(device_wakeup,cmd,user_exit_callback,0);
     
    printf("GPIO demo starts.\r\n\n");
    
    /*test led pin and wake up pin*/
    data_write = 0;
    uds_dev_write(device_led,0,&data_write,1,0);

    ret = uds_dev_read(device_led,0,&data_read,1,0);
    if(ret)
        printf("led level is %d \r\n",data_read);

    osDelay(1000);

    data_write = 1;
    uds_dev_write(device_led,0,&data_write,1,0);

    ret = uds_dev_read(device_led,0,&data_read,1,0);
    if(ret)
        printf("led level is %d \r\n\n",data_read);

    while (1)
    {
        cmd = UDS_GPIO_TOGGLE_PIN;
        uds_dev_ioctl(device_led,cmd,NULL,0);
        osDelay(1000);
        ret = uds_dev_read(device_wakeup,0,&data_read,1,0);
        if(ret)
            printf("wakeup level is %d \r\n",data_read);
        ret = uds_dev_read(device_led,0,&data_read,1,0);
        if(ret)
            printf("led level is %d \r\n",data_read);
        ret = uds_dev_read(device_test_inout_pin,0,&data_read,1,0);
        if(ret)
            printf("testio level is %d \r\n\n",data_read);
     }

}

