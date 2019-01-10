#include "uds/uds_timer.h"
#include "uds/uds.h"
#include "stdio.h"

uds_dev_t device_timer1;
u32_t cmd;
uint8_t i;
void user_update_callback(uds_time_x_type timx)
{
    if(timx == UDS_TIME1)
    {
        printf(" TIME1 IT,hello world\r\n");
        i++;
        printf(" i = %d\r\n",i);
        if(i == 10)
        {
          cmd = TIMER_IT_STOP;
          uds_dev_ioctl(device_timer1,cmd,NULL,0);
          printf("demo_timer stop.");
        }
    }
    
}


void demo_timer (void)
{
    
    u32_t cmd;
    uds_timer_init_t tim1;
    tim1.uds_time_x = UDS_TIME1;
    tim1.uds_period = 10000-1;
    tim1.uds_prescaler = (uint32_t)(SystemCoreClock / 10000) - 1;
    tim1.countmode = UDS_UP;
    uds_driv_init();
    uds_timer_dev_install("timer1", (void *)&tim1);
    device_timer1 = uds_dev_open("timer1",2);
    if(!device_timer1)
        while(1);
	
    cmd = TIMER_SET_UPDATE_CALLBACK;
    uds_dev_ioctl(device_timer1,cmd,user_update_callback,0);
	
	
    printf("demo_timer start.\r\n");
    cmd = TIMER_IT_START;
    uds_dev_ioctl(device_timer1,cmd,NULL,0);
    while(1)
    {
        
    }
}