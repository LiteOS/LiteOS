#include "uds/uds_timer.h"
#include "uds/uds.h"
#include "stdio.h"

uds_driv_t device_timer1;
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
    printf("demo_timer start.\r\n");
	cmd = TIMER_IT_START;
    los_dev_ioctl(device_timer1,cmd,NULL,0);
    while(1)
	{
		
	}
}