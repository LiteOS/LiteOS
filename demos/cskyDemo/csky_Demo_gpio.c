

#include <stdio.h>
#include "soc.h"
#include "drv_gpio.h"
#include "pin_name.h"
#include "pin.h"
#include "csky_All_Demo.h"
volatile static bool int_flag = 1;

static void gpio_interrupt_handler(int32_t idx)
{
	printf("[info]进入gpio回调函数\n");
    int_flag = 0;
}

void example_pin_gpio_init(void)
{
    drv_pinmux_config(EXAMPLE_GPIO_PIN, EXAMPLE_GPIO_PIN_FUNC);
}

void gpio_falling_edge_interrupt(pin_name_e gpio_pin)
{
    gpio_pin_handle_t *pin = NULL;

    example_pin_gpio_init();

    printf("[info]please change the gpio pin %s from high to low\r\n", EXAMPLE_BOARD_GPIO_PIN_NAME);
	printf("[info]为产生下降沿，请用手指触摸开发板上PA17引脚！\n");
    pin = csi_gpio_pin_initialize(gpio_pin, gpio_interrupt_handler);

    csi_gpio_pin_config_mode(pin, GPIO_MODE_PULLNONE);
    csi_gpio_pin_config_direction(pin, GPIO_DIRECTION_INPUT);
    csi_gpio_pin_set_irq(pin, GPIO_IRQ_MODE_FALLING_EDGE, 1);

    while (int_flag);

    int_flag = 1;
    csi_gpio_pin_uninitialize(pin);
    printf("[success]gpio falling_edge test successfully!!!\n");
    printf("[success]test gpio successfully\n");
}

/*****************************************************************************
test_gpio: main function of the gpio test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_gpio(pin_name_e gpio_pin)
{
    gpio_falling_edge_interrupt(gpio_pin);
    return 0;
}

int task_GPIO(void)
{
	HopeRun_printf("开始GPIO测试............\n");
    return example_gpio(EXAMPLE_GPIO_PIN);
}
