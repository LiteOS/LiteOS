#include "los_hwi.h"
#include "los_typedef.h"
#define HWI_NUM_INT50 50
void uart_irqhandle(int irq,void *dev)
{
	printf("\n int the func uart_irqhandle \n");
}
void hwi_test()
{
	int a = 1;
	UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
	LOS_HwiCreate(HWI_NUM_INT50, 0,0,uart_irqhandle,NULL);//创建中断
	hal_interrupt_unmask(HWI_NUM_INT50);
	LOS_IntRestore(uvIntSave);
	hal_interrupt_mask(HWI_NUM_INT50);	
}