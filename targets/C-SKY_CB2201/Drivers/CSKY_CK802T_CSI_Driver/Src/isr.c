/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     isr.c
 * @brief    source file for the interrupt server route
 * @version  V1.0
 * @date     25. August 2017
 ******************************************************************************/
#include <drv_common.h>
#include <csi_config.h>
#include "soc.h"
/*20181127 对liteOS宏 进行整理，发现不应该定义CONFIG_KERNEL_NONE，此处代码为去除CONFIG_KERNEL_NONE定义后，针对csi自带的内核文件 进行包含的代码。
#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
*/
extern void dw_usart_irqhandler(int32_t idx);
extern void dw_timer_irqhandler(int32_t idx);
extern void dw_gpio_irqhandler(int32_t idx);
extern void dw_iic_irqhandler(int32_t idx);
extern void ck_rtc_irqhandler(int32_t idx);
extern void dw_spi_irqhandler(int32_t idx);
extern void dw_wdt_irqhandler(int32_t idx);
extern void ck_dma_irqhandler(int32_t idx);
extern void ck_aes_irqhandler(int32_t idx);
extern void ck_sha_irqhandler(int32_t idx);
extern void dw_dmac_irqhandler(int32_t idx);
extern void ck_adc_irqhandler(int32_t idx);
extern void ck_i2s_irqhandler(int32_t idx);
#ifdef CONFIG_KERNEL_FREERTOS
extern void CoretimeIsr(void);
extern void CKPendSVIsr(void);
#endif
extern void systick_handler(void);
extern void xPortSysTickHandler(void);
extern void OSTimeTick(void);

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

/*20181127 对liteOS宏 进行整理，发现不应该定义CONFIG_KERNEL_NONE，此处代码csi自带的内核文件的代码,不是liteos的内核函数代码。
#ifndef CONFIG_KERNEL_NONE
#define CSI_INTRPT_ENTER() csi_kernel_intrpt_enter()
#define CSI_INTRPT_EXIT()  csi_kernel_intrpt_exit()
#else
*/
#define CSI_INTRPT_ENTER()
#define CSI_INTRPT_EXIT()
//#endif

#ifdef CONFIG_HAVE_VIC
#define ATTRIBUTE_ISR __attribute__((isr))
#else
#define ATTRIBUTE_ISR
#endif

//下方CORET_IRQHandler函数已在coretim.c中定义了
//ATTRIBUTE_ISR void CORET_IRQHandler(void)
//{
//#ifndef CONFIG_KERNEL_FREERTOS
//    CSI_INTRPT_ENTER();
//#endif
//
//    readl(0xE000E010);
//
//#if defined(CONFIG_KERNEL_RHINO)
//    systick_handler();
//#elif defined(CONFIG_KERNEL_FREERTOS)
//    xPortSysTickHandler();
//#elif defined(CONFIG_KERNEL_UCOS)
//    OSTimeTick();
//#endif
//
//#ifndef CONFIG_KERNEL_FREERTOS
//    CSI_INTRPT_EXIT();
//#endif
//}

ATTRIBUTE_ISR void USART0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_usart_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void USART1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_usart_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void USART2_IRQHandler(void)
{    CSI_INTRPT_ENTER();
    dw_usart_irqhandler(2);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void USART3_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_usart_irqhandler(3);
    CSI_INTRPT_EXIT();
}
//#include "stdio.h"
ATTRIBUTE_ISR void TIMA0_IRQHandler(void)	//startup.s中被定义
{	//printf("直接中断");//此处没有进入
    CSI_INTRPT_ENTER();
    dw_timer_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void TIMA1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_timer_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void TIMB0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_timer_irqhandler(2);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void TIMB1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_timer_irqhandler(3);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void GPIO0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_gpio_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void GPIO1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_gpio_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void I2C0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    //dw_iic_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void I2C1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
   // dw_iic_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void RTC_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    ck_rtc_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void RTC1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    ck_rtc_irqhandler(1);
    CSI_INTRPT_EXIT();
}

//k_status_t csi_kernel_intrpt_enter(void);
//进入中断时调用，每次执行中断嵌套层数加 1

//k_status_t csi_kernel_intrpt_exit(void);
//退出中断时调用，每次执行中断嵌套层数减 1



ATTRIBUTE_ISR void AES_IRQHandler(void)
{
    CSI_INTRPT_ENTER();	//进入中断，中断嵌套层加一
    ck_aes_irqhandler(0);
    CSI_INTRPT_EXIT();	//退出中断，中断嵌套层减一
}

ATTRIBUTE_ISR void TRNG_IRQHandler(void)
{
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void RSA_IRQHandler(void)
{
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void SPI0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_spi_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void SPI1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_spi_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void WDT_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_wdt_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void DMAC0_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_dmac_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void DMAC1_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    dw_dmac_irqhandler(1);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void ADC_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    //ck_adc_irqhandler(0);
    CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void I2S_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
    //ck_i2s_irqhandler(0);
    CSI_INTRPT_EXIT();
}
ATTRIBUTE_ISR void SHA_IRQHandler(void)
{
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
}


