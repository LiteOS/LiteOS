/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * @file riscv_hal_stubs.c
 * @author Microsemi SoC Products Group
 * @brief RISC-V soft processor CoreRISCV_AXI4 Function stubs.
 * The functions below will only be linked with the application code if the user
 * does not provide an implementation for these functions. These functions are
 * defined with weak linking so that they can be overridden by a function with
 * same prototype in the user's application code.
 *
 * SVN $Revision: 9187 $
 * SVN $Date: 2017-05-13 13:31:28 +0530 (Sat, 13 May 2017) $
 */
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((weak)) void handle_m_ext_interrupt()
{
    _exit(10);
}

__attribute__((weak)) void Software_IRQHandler(void)
{
    _exit(10);
}

__attribute__((weak)) void SysTick_Handler(void)
{
    ;
}

__attribute__((weak))  uint8_t Invalid_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_1_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_2_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_3_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_4_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_5_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_6_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_7_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_8_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_9_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_10_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_11_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_12_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_13_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_14_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_15_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_16_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_17_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_18_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_19_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_20_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_21_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_22_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_23_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_24_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_25_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_26_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_27_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_28_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_29_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_30_IRQHandler(void)
{
    return(0);
}

__attribute__((weak))  uint8_t External_31_IRQHandler(void)
{
    return(0);
}

#ifdef __cplusplus
}
#endif
