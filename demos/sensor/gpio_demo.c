#include "dal_gpio.h"
#include "stdio.h"

dal_gpio_init_t wakeup;
dal_gpio_init_t led;

/*
void dal_exti0_callback(void)
{
	uint8_t ret = 1;
	ret = dal_gpio_read_input_pin(&wakeup);
  printf("wakeup level is %d \r\n",ret);
  printf("hello world.\r\n");
 
}
*/


void demo_gpio (void)
{
	volatile uint8_t ret;
 
	wakeup.gpio_port_num = DAL_GPIO_A;
	wakeup.gpio_pin_num = DAL_PIN_0;
	wakeup.gpio_mode = DAL_GPIO_IT_RISING_FALLING;
	wakeup.dal_PreemptionPriority = 0;
	wakeup.dal_SubPriority = 0;
	
	
	led.gpio_port_num = DAL_GPIO_C;
	led.gpio_pin_num = DAL_PIN_11;
	led.gpio_mode = DAL_GPIO_OUTPUT_OD;
	led.gpio_pull = DAL_GPIO_NOPULL;

  dal_gpio_init(&wakeup);
	dal_gpio_init(&led);
	
	dal_gpio_clear_output_pin(&led);
	ret = dal_gpio_read_output_pin(&led);
	printf("led level is %d \r\n",ret);
	
	osDelay(1000);
	
	dal_gpio_set_output_pin(&led);
	ret = dal_gpio_read_output_pin(&led);
	printf("led level is %d \r\n",ret);
	
	while (1)
  {
			dal_gpio_toggle_output_pin(&led);
		  osDelay(1000);
		  ret = dal_gpio_read_input_pin(&wakeup);
	    printf("wakeup level is %d \r\n",ret);
			ret = dal_gpio_read_output_pin(&led);
	    printf("led level is %d \r\n",ret);
  }

}