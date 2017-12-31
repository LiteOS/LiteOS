#include "los_demo_debug.h"
#include "cm_backtrace.h"

#ifdef LOS_CMBACKTRACE_COMPILE

/******************************************
*
*  函数void fault_test_by_div0(void)用于测试除零异常：
*  1. 当系统处于特权级状态时，只会在语句z = x / y;处触发除零异常。
*  2. 当系统处于非特权级状态时，因0xE000ED14地址只能在特权级状态下访问，
*     所以在非特权级状态下会在语句*SCB_CCR |= (1 << 4);处触发非法访问异常。
*
******************************************/
void fault_test_by_div0(void)
{
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    dprintf("z:%d\n", z);
}

/******************************************
*
*  函数void fault_test_by_unalign(void)用于测试非对齐访问异常：
*  1. 当系统处于特权级状态时，只会在语句p = (int *) 0x03;处触发非对齐访问异常。
*  2. 当系统处于非特权级状态时，因0xE000ED14地址只能在特权级状态下访问，
*     所以在非特权级状态下会在语句*SCB_CCR |= (1 << 3);处触发非法访问异常。
*
******************************************/
void fault_test_by_unalign(void)
{
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    volatile int * p;
    volatile int value;

    *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

    p = (int *) 0x00;
    value = *p;
    dprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x04;
    value = *p;
    dprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x03;
    value = *p;
    dprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);
}

/******************************************
*
*  函数void HardFault_Handler(void)用于异常处理：
*  该函数只在使能CMBACKTRACE功能时使用
*
******************************************/
void HardFault_Handler(void)
{
    asm("mov r0, lr");
    asm("mov r1, sp");
    
    asm("push {lr}");
    asm("bl cm_backtrace_fault");
    asm("pop  {lr}");
    
    while(1)
    {
        ;
    }
}
#endif