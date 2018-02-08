/*!
    \file  gd32f1x0_gpio.h
    \brief definitions for the gpio
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_GPIO_H
#define GD32F1X0_GPIO_H

#include "gd32f1x0.h"

/* GPIOx(x=A,B,C,D,F) definitions */
#define GPIOA                     (GPIO_BASE + 0x00000000)
#define GPIOB                     (GPIO_BASE + 0x00000400)
#define GPIOC                     (GPIO_BASE + 0x00000800)
#define GPIOD                     (GPIO_BASE + 0x00000C00)
#define GPIOF                     (GPIO_BASE + 0x00001400)

/* registers definitions */
#define GPIO_CTL(GPIO)            REG32(GPIO + 0x00)    /*!< GPIO port control register */
#define GPIO_OMODE(GPIO)          REG32(GPIO + 0x04)    /*!< GPIO port output mode register */
#define GPIO_OSPD(GPIO)           REG32(GPIO + 0x08)    /*!< GPIO port output speed register */
#define GPIO_PUD(GPIO)            REG32(GPIO + 0x0C)    /*!< GPIO port pull-up/down register */
#define GPIO_ISTAT(GPIO)          REG32(GPIO + 0x10)    /*!< GPIO port input status register */
#define GPIO_OCTL(GPIO)           REG32(GPIO + 0x14)    /*!< GPIO port output control register */
#define GPIO_BOP(GPIO)            REG32(GPIO + 0x18)    /*!< GPIO port bit operation register */
#define GPIO_LOCK(GPIO)           REG32(GPIO + 0x1C)    /*!< GPIO port configuration lock register */
#define GPIO_AFSEL0(GPIO)         REG32(GPIO + 0x20)    /*!< GPIO alternate function selected register 0 */
#define GPIO_AFSEL1(GPIO)         REG32(GPIO + 0x24)    /*!< GPIO alternate function selected register 1 */
#define GPIO_BC(GPIO)             REG32(GPIO + 0x28)    /*!< GPIO bit clear register */
#define GPIO_TG(GPIO)             REG32(GPIO + 0x2C)    /*!< GPIO port bit toggle register */

/* bits definitions */
/* GPIO_CTL */
#define GPIO_CTL_CTL0             BITS(0,1)             /*!< pin 0 configuration bits */ 
#define GPIO_CTL_CTL1             BITS(2,3)             /*!< pin 1 configuration bits */
#define GPIO_CTL_CTL2             BITS(4,5)             /*!< pin 2 configuration bits */
#define GPIO_CTL_CTL3             BITS(6,7)             /*!< pin 3 configuration bits */
#define GPIO_CTL_CTL4             BITS(8,9)             /*!< pin 4 configuration bits */
#define GPIO_CTL_CTL5             BITS(10,11)           /*!< pin 5 configuration bits */
#define GPIO_CTL_CTL6             BITS(12,13)           /*!< pin 6 configuration bits */
#define GPIO_CTL_CTL7             BITS(14,15)           /*!< pin 7 configuration bits */
#define GPIO_CTL_CTL8             BITS(16,17)           /*!< pin 8 configuration bits */
#define GPIO_CTL_CTL9             BITS(18,19)           /*!< pin 9 configuration bits */
#define GPIO_CTL_CTL10            BITS(20,21)           /*!< pin 10 configuration bits */
#define GPIO_CTL_CTL11            BITS(22,23)           /*!< pin 11 configuration bits */
#define GPIO_CTL_CTL12            BITS(24,25)           /*!< pin 12 configuration bits */
#define GPIO_CTL_CTL13            BITS(26,27)           /*!< pin 13 configuration bits */
#define GPIO_CTL_CTL14            BITS(28,29)           /*!< pin 14 configuration bits */
#define GPIO_CTL_CTL15            BITS(30,31)           /*!< pin 15 configuration bits */

/* GPIO_OMODE */
#define GPIO_OMODE_OM0            BIT(0)                /*!< pin 0 output mode bit */
#define GPIO_OMODE_OM1            BIT(1)                /*!< pin 1 output mode bit */
#define GPIO_OMODE_OM2            BIT(2)                /*!< pin 2 output mode bit */
#define GPIO_OMODE_OM3            BIT(3)                /*!< pin 3 output mode bit */
#define GPIO_OMODE_OM4            BIT(4)                /*!< pin 4 output mode bit */
#define GPIO_OMODE_OM5            BIT(5)                /*!< pin 5 output mode bit */
#define GPIO_OMODE_OM6            BIT(6)                /*!< pin 6 output mode bit */
#define GPIO_OMODE_OM7            BIT(7)                /*!< pin 7 output mode bit */
#define GPIO_OMODE_OM8            BIT(8)                /*!< pin 8 output mode bit */
#define GPIO_OMODE_OM9            BIT(9)                /*!< pin 9 output mode bit */
#define GPIO_OMODE_OM10           BIT(10)               /*!< pin 10 output mode bit */
#define GPIO_OMODE_OM11           BIT(11)               /*!< pin 11 output mode bit */
#define GPIO_OMODE_OM12           BIT(12)               /*!< pin 12 output mode bit */
#define GPIO_OMODE_OM13           BIT(13)               /*!< pin 13 output mode bit */
#define GPIO_OMODE_OM14           BIT(14)               /*!< pin 14 output mode bit */
#define GPIO_OMODE_OM15           BIT(15)               /*!< pin 15 output mode bit */

/* GPIO_OSPD */
#define GPIO_OSPD_OSPD0           BITS(0,1)             /*!< pin 0 output max speed bits */
#define GPIO_OSPD_OSPD1           BITS(2,3)             /*!< pin 1 output max speed bits */
#define GPIO_OSPD_OSPD2           BITS(4,5)             /*!< pin 2 output max speed bits */
#define GPIO_OSPD_OSPD3           BITS(6,7)             /*!< pin 3 output max speed bits */
#define GPIO_OSPD_OSPD4           BITS(8,9)             /*!< pin 4 output max speed bits */
#define GPIO_OSPD_OSPD5           BITS(10,11)           /*!< pin 5 output max speed bits */
#define GPIO_OSPD_OSPD6           BITS(12,13)           /*!< pin 6 output max speed bits */
#define GPIO_OSPD_OSPD7           BITS(14,15)           /*!< pin 7 output max speed bits */
#define GPIO_OSPD_OSPD8           BITS(16,17)           /*!< pin 8 output max speed bits */
#define GPIO_OSPD_OSPD9           BITS(18,19)           /*!< pin 9 output max speed bits */
#define GPIO_OSPD_OSPD10          BITS(20,21)           /*!< pin 10 output max speed bits */
#define GPIO_OSPD_OSPD11          BITS(22,23)           /*!< pin 11 output max speed bits */
#define GPIO_OSPD_OSPD12          BITS(24,25)           /*!< pin 12 output max speed bits */
#define GPIO_OSPD_OSPD13          BITS(26,27)           /*!< pin 13 output max speed bits */
#define GPIO_OSPD_OSPD14          BITS(28,29)           /*!< pin 14 output max speed bits */
#define GPIO_OSPD_OSPD15          BITS(30,31)           /*!< pin 15 output max speed bits */

/* GPIO_PUD */
#define GPIO_PUD_PUD0             BITS(0,1)             /*!< pin 0 pull-up or pull-down bits */
#define GPIO_PUD_PUD1             BITS(2,3)             /*!< pin 1 pull-up or pull-down bits */
#define GPIO_PUD_PUD2             BITS(4,5)             /*!< pin 2 pull-up or pull-down bits */
#define GPIO_PUD_PUD3             BITS(6,7)             /*!< pin 3 pull-up or pull-down bits */
#define GPIO_PUD_PUD4             BITS(8,9)             /*!< pin 4 pull-up or pull-down bits */
#define GPIO_PUD_PUD5             BITS(10,11)           /*!< pin 5 pull-up or pull-down bits */
#define GPIO_PUD_PUD6             BITS(12,13)           /*!< pin 6 pull-up or pull-down bits */
#define GPIO_PUD_PUD7             BITS(14,15)           /*!< pin 7 pull-up or pull-down bits */
#define GPIO_PUD_PUD8             BITS(16,17)           /*!< pin 8 pull-up or pull-down bits */
#define GPIO_PUD_PUD9             BITS(18,19)           /*!< pin 9 pull-up or pull-down bits */
#define GPIO_PUD_PUD10            BITS(20,21)           /*!< pin 10 pull-up or pull-down bits */
#define GPIO_PUD_PUD11            BITS(22,23)           /*!< pin 11 pull-up or pull-down bits */
#define GPIO_PUD_PUD12            BITS(24,25)           /*!< pin 12 pull-up or pull-down bits */
#define GPIO_PUD_PUD13            BITS(26,27)           /*!< pin 13 pull-up or pull-down bits */
#define GPIO_PUD_PUD14            BITS(28,29)           /*!< pin 14 pull-up or pull-down bits */
#define GPIO_PUD_PUD15            BITS(30,31)           /*!< pin 15 pull-up or pull-down bits */

/* GPIO_ISTAT */
#define GPIO_ISTAT_ISTAT0         BIT(0)                /*!< pin 0 input data */
#define GPIO_ISTAT_ISTAT1         BIT(1)                /*!< pin 1 input data */
#define GPIO_ISTAT_ISTAT2         BIT(2)                /*!< pin 2 input data */
#define GPIO_ISTAT_ISTAT3         BIT(3)                /*!< pin 3 input data */
#define GPIO_ISTAT_ISTAT4         BIT(4)                /*!< pin 4 input data */
#define GPIO_ISTAT_ISTAT5         BIT(5)                /*!< pin 5 input data */
#define GPIO_ISTAT_ISTAT6         BIT(6)                /*!< pin 6 input data */
#define GPIO_ISTAT_ISTAT7         BIT(7)                /*!< pin 7 input data */
#define GPIO_ISTAT_ISTAT8         BIT(8)                /*!< pin 8 input data */
#define GPIO_ISTAT_ISTAT9         BIT(9)                /*!< pin 9 input data */
#define GPIO_ISTAT_ISTAT10        BIT(10)               /*!< pin 10 input data */
#define GPIO_ISTAT_ISTAT11        BIT(11)               /*!< pin 11 input data */
#define GPIO_ISTAT_ISTAT12        BIT(12)               /*!< pin 12 input data */
#define GPIO_ISTAT_ISTAT13        BIT(13)               /*!< pin 13 input data */
#define GPIO_ISTAT_ISTAT14        BIT(14)               /*!< pin 14 input data */
#define GPIO_ISTAT_ISTAT15        BIT(15)               /*!< pin 15 input data */

/* GPIO_OCTL */
#define GPIO_OCTL_OCTL0           BIT(0)                /*!< pin 0 output data */
#define GPIO_OCTL_OCTL1           BIT(1)                /*!< pin 1 output data */
#define GPIO_OCTL_OCTL2           BIT(2)                /*!< pin 2 output data */
#define GPIO_OCTL_OCTL3           BIT(3)                /*!< pin 3 output data */
#define GPIO_OCTL_OCTL4           BIT(4)                /*!< pin 4 output data */
#define GPIO_OCTL_OCTL5           BIT(5)                /*!< pin 5 output data */
#define GPIO_OCTL_OCTL6           BIT(6)                /*!< pin 6 output data */
#define GPIO_OCTL_OCTL7           BIT(7)                /*!< pin 7 output data */
#define GPIO_OCTL_OCTL8           BIT(8)                /*!< pin 8 output data */
#define GPIO_OCTL_OCTL9           BIT(9)                /*!< pin 9 output data */
#define GPIO_OCTL_OCTL10          BIT(10)               /*!< pin 10 output data */
#define GPIO_OCTL_OCTL11          BIT(11)               /*!< pin 11 output data */
#define GPIO_OCTL_OCTL12          BIT(12)               /*!< pin 12 output data */
#define GPIO_OCTL_OCTL13          BIT(13)               /*!< pin 13 output data */
#define GPIO_OCTL_OCTL14          BIT(14)               /*!< pin 14 output data */
#define GPIO_OCTL_OCTL15          BIT(15)               /*!< pin 15 output data */

/* GPIO_BOP */
#define GPIO_BOP_BOP0             BIT(0)                /*!< pin 0 set bit */
#define GPIO_BOP_BOP1             BIT(1)                /*!< pin 1 set bit */
#define GPIO_BOP_BOP2             BIT(2)                /*!< pin 2 set bit */
#define GPIO_BOP_BOP3             BIT(3)                /*!< pin 3 set bit */
#define GPIO_BOP_BOP4             BIT(4)                /*!< pin 4 set bit */
#define GPIO_BOP_BOP5             BIT(5)                /*!< pin 5 set bit */
#define GPIO_BOP_BOP6             BIT(6)                /*!< pin 6 set bit */
#define GPIO_BOP_BOP7             BIT(7)                /*!< pin 7 set bit */
#define GPIO_BOP_BOP8             BIT(8)                /*!< pin 8 set bit */
#define GPIO_BOP_BOP9             BIT(9)                /*!< pin 9 set bit */
#define GPIO_BOP_BOP10            BIT(10)               /*!< pin 10 set bit */
#define GPIO_BOP_BOP11            BIT(11)               /*!< pin 11 set bit */
#define GPIO_BOP_BOP12            BIT(12)               /*!< pin 12 set bit */
#define GPIO_BOP_BOP13            BIT(13)               /*!< pin 13 set bit */
#define GPIO_BOP_BOP14            BIT(14)               /*!< pin 14 set bit */
#define GPIO_BOP_BOP15            BIT(15)               /*!< pin 15 set bit */
#define GPIO_BOP_CR0              BIT(16)               /*!< pin 0 reset bit */
#define GPIO_BOP_CR1              BIT(17)               /*!< pin 1 reset bit */
#define GPIO_BOP_CR2              BIT(18)               /*!< pin 2 reset bit */
#define GPIO_BOP_CR3              BIT(19)               /*!< pin 3 reset bit */
#define GPIO_BOP_CR4              BIT(20)               /*!< pin 4 reset bit */
#define GPIO_BOP_CR5              BIT(21)               /*!< pin 5 reset bit */
#define GPIO_BOP_CR6              BIT(22)               /*!< pin 6 reset bit */
#define GPIO_BOP_CR7              BIT(23)               /*!< pin 7 reset bit */
#define GPIO_BOP_CR8              BIT(24)               /*!< pin 8 reset bit */
#define GPIO_BOP_CR9              BIT(25)               /*!< pin 9 reset bit */
#define GPIO_BOP_CR10             BIT(26)               /*!< pin 10 reset bit */
#define GPIO_BOP_CR11             BIT(27)               /*!< pin 11 reset bit */
#define GPIO_BOP_CR12             BIT(28)               /*!< pin 12 reset bit */
#define GPIO_BOP_CR13             BIT(29)               /*!< pin 13 reset bit */
#define GPIO_BOP_CR14             BIT(30)               /*!< pin 14 reset bit */
#define GPIO_BOP_CR15             BIT(31)               /*!< pin 15 reset bit */

/* GPIO_LOCK */
#define GPIO_LOCK_LK0             BIT(0)                /*!< pin 0 lock bit */
#define GPIO_LOCK_LK1             BIT(1)                /*!< pin 1 lock bit */
#define GPIO_LOCK_LK2             BIT(2)                /*!< pin 2 lock bit */
#define GPIO_LOCK_LK3             BIT(3)                /*!< pin 3 lock bit */
#define GPIO_LOCK_LK4             BIT(4)                /*!< pin 4 lock bit */
#define GPIO_LOCK_LK5             BIT(5)                /*!< pin 5 lock bit */
#define GPIO_LOCK_LK6             BIT(6)                /*!< pin 6 lock bit */
#define GPIO_LOCK_LK7             BIT(7)                /*!< pin 7 lock bit */
#define GPIO_LOCK_LK8             BIT(8)                /*!< pin 8 lock bit */
#define GPIO_LOCK_LK9             BIT(9)                /*!< pin 9 lock bit */
#define GPIO_LOCK_LK10            BIT(10)               /*!< pin 10 lock bit */
#define GPIO_LOCK_LK11            BIT(11)               /*!< pin 11 lock bit */
#define GPIO_LOCK_LK12            BIT(12)               /*!< pin 12 lock bit */
#define GPIO_LOCK_LK13            BIT(13)               /*!< pin 13 lock bit */
#define GPIO_LOCK_LK14            BIT(14)               /*!< pin 14 lock bit */
#define GPIO_LOCK_LK15            BIT(15)               /*!< pin 15 lock bit */
#define GPIO_LOCK_LKK             BIT(16)               /*!< pin lock key */

/* GPIO_AFSEL0 */
#define GPIO_AFSEL0_SEL0          BITS(0,3)             /*!< pin 0 alternate function selected */
#define GPIO_AFSEL0_SEL1          BITS(4,7)             /*!< pin 1 alternate function selected */
#define GPIO_AFSEL0_SEL2          BITS(8,11)            /*!< pin 2 alternate function selected */
#define GPIO_AFSEL0_SEL3          BITS(12,15)           /*!< pin 3 alternate function selected */
#define GPIO_AFSEL0_SEL4          BITS(16,19)           /*!< pin 4 alternate function selected */
#define GPIO_AFSEL0_SEL5          BITS(20,23)           /*!< pin 5 alternate function selected */
#define GPIO_AFSEL0_SEL6          BITS(24,27)           /*!< pin 6 alternate function selected */
#define GPIO_AFSEL0_SEL7          BITS(28,31)           /*!< pin 7 alternate function selected */
//
/* GPIO_AFSEL1 */
#define GPIO_AFSEL1_SEL8          BITS(0,3)             /*!< pin 8 alternate function selected */
#define GPIO_AFSEL1_SEL9          BITS(4,7)             /*!< pin 9 alternate function selected */
#define GPIO_AFSEL1_SEL10         BITS(8,11)            /*!< pin 10 alternate function selected */
#define GPIO_AFSEL1_SEL11         BITS(12,15)           /*!< pin 11 alternate function selected */
#define GPIO_AFSEL1_SEL12         BITS(16,19)           /*!< pin 12 alternate function selected */
#define GPIO_AFSEL1_SEL13         BITS(20,23)           /*!< pin 13 alternate function selected */
#define GPIO_AFSEL1_SEL14         BITS(24,27)           /*!< pin 14 alternate function selected */
#define GPIO_AFSEL1_SEL15         BITS(28,31)           /*!< pin 15 alternate function selected */

/* GPIO_BC */
#define GPIO_BC_CR0               BIT(0)                /*!< pin 0 clear bit */
#define GPIO_BC_CR1               BIT(1)                /*!< pin 1 clear bit */
#define GPIO_BC_CR2               BIT(2)                /*!< pin 2 clear bit */
#define GPIO_BC_CR3               BIT(3)                /*!< pin 3 clear bit */
#define GPIO_BC_CR4               BIT(4)                /*!< pin 4 clear bit */
#define GPIO_BC_CR5               BIT(5)                /*!< pin 5 clear bit */
#define GPIO_BC_CR6               BIT(6)                /*!< pin 6 clear bit */
#define GPIO_BC_CR7               BIT(7)                /*!< pin 7 clear bit */
#define GPIO_BC_CR8               BIT(8)                /*!< pin 8 clear bit */
#define GPIO_BC_CR9               BIT(9)                /*!< pin 9 clear bit */
#define GPIO_BC_CR10              BIT(10)               /*!< pin 10 clear bit */
#define GPIO_BC_CR11              BIT(11)               /*!< pin 11 clear bit */
#define GPIO_BC_CR12              BIT(12)               /*!< pin 12 clear bit */
#define GPIO_BC_CR13              BIT(13)               /*!< pin 13 clear bit */
#define GPIO_BC_CR14              BIT(14)               /*!< pin 14 clear bit */
#define GPIO_BC_CR15              BIT(15)               /*!< pin 15 clear bit */

#ifdef GD32F170_190
/* GPIO_TG */
#define GPIO_TG_TG0               BIT(0)                /*!< pin 0 toggle bit */
#define GPIO_TG_TG1               BIT(1)                /*!< pin 1 toggle bit */
#define GPIO_TG_TG2               BIT(2)                /*!< pin 2 toggle bit */
#define GPIO_TG_TG3               BIT(3)                /*!< pin 3 toggle bit */
#define GPIO_TG_TG4               BIT(4)                /*!< pin 4 toggle bit */
#define GPIO_TG_TG5               BIT(5)                /*!< pin 5 toggle bit */
#define GPIO_TG_TG6               BIT(6)                /*!< pin 6 toggle bit */
#define GPIO_TG_TG7               BIT(7)                /*!< pin 7 toggle bit */
#define GPIO_TG_TG8               BIT(8)                /*!< pin 8 toggle bit */
#define GPIO_TG_TG9               BIT(9)                /*!< pin 9 toggle bit */
#define GPIO_TG_TG10              BIT(10)               /*!< pin 10 toggle bit */
#define GPIO_TG_TG11              BIT(11)               /*!< pin 11 toggle bit */
#define GPIO_TG_TG12              BIT(12)               /*!< pin 12 toggle bit */
#define GPIO_TG_TG13              BIT(13)               /*!< pin 13 toggle bit */
#define GPIO_TG_TG14              BIT(14)               /*!< pin 14 toggle bit */
#define GPIO_TG_TG15              BIT(15)               /*!< pin 15 toggle bit */

#endif /* GD32F170_190 */

/* constants definitions */
typedef FlagStatus bit_status;

/* output mode definitions */
#define CTL_CLTR(regval)          (BITS(0,1) & ((regval) << 0))
#define GPIO_MODE_INPUT           CTL_CLTR(0)           /*!< input mode */
#define GPIO_MODE_OUTPUT          CTL_CLTR(1)           /*!< output mode */
#define GPIO_MODE_AF              CTL_CLTR(2)           /*!< alternate functions mode */
#define GPIO_MODE_ANALOG          CTL_CLTR(3)           /*!< analog mode */

/* pull up pull down definitions */
#define PUD_PUPD(regval)          (BITS(0,1) & ((regval) << 0))
#define GPIO_PUPD_NONE            PUD_PUPD(0)           /*!< without weak pull-up and pull-down resistors */
#define GPIO_PUPD_PULLUP          PUD_PUPD(1)           /*!< with weak pull-up resistors */
#define GPIO_PUPD_PULLDOWN        PUD_PUPD(2)           /*!< with weak pull-down resistors */

/* gpio pin definitions */
#define GPIO_PIN_0                BIT(0)                /*!< GPIO pin 0 */
#define GPIO_PIN_1                BIT(1)                /*!< GPIO pin 1 */
#define GPIO_PIN_2                BIT(2)                /*!< GPIO pin 2 */
#define GPIO_PIN_3                BIT(3)                /*!< GPIO pin 3 */
#define GPIO_PIN_4                BIT(4)                /*!< GPIO pin 4 */
#define GPIO_PIN_5                BIT(5)                /*!< GPIO pin 5 */
#define GPIO_PIN_6                BIT(6)                /*!< GPIO pin 6 */
#define GPIO_PIN_7                BIT(7)                /*!< GPIO pin 7 */
#define GPIO_PIN_8                BIT(8)                /*!< GPIO pin 8 */
#define GPIO_PIN_9                BIT(9)                /*!< GPIO pin 9 */
#define GPIO_PIN_10               BIT(10)               /*!< GPIO pin 10 */
#define GPIO_PIN_11               BIT(11)               /*!< GPIO pin 11 */
#define GPIO_PIN_12               BIT(12)               /*!< GPIO pin 12 */
#define GPIO_PIN_13               BIT(13)               /*!< GPIO pin 13 */
#define GPIO_PIN_14               BIT(14)               /*!< GPIO pin 14 */
#define GPIO_PIN_15               BIT(15)               /*!< GPIO pin 15 */
#define GPIO_PIN_ALL              ((uint16_t)(0xFFFF))  /*!< GPIO pin all */

/* gpio ctlr values */
#define GPIO_MODE_SET(n, mode)    ((mode) << (2 * (n)))
#define GPIO_MODE_MASK(n)         (0x3 << (2 * (n)))

/* gpio pull up pull down values */
#define GPIO_PUPD_SET(n, pupd)    ((pupd) << (2 * (n)))
#define GPIO_PUPD_MASK(n)         (0x3 << (2 * (n)))

/* gpio output speed values */
#define GPIO_OSPEED_SET(n, speed) ((speed) << (2 * (n)))
#define GPIO_OSPEED_MASK(n)       (0x3 << (2 * (n)))

/* gpio output type */
#define GPIO_OTYPE_PP             ((uint8_t)(0x00))     /*!< push pull mode */
#define GPIO_OTYPE_OD             ((uint8_t)(0x01))     /*!< open drain mode */

/* gpio output speed */
#define OSPD_OSPD(regval)         (BITS(0,1) & ((regval) << 0))
#define GPIO_OSPEED_2MHZ          OSPD_OSPD(0)          /*!< output max speed 2M */
#define GPIO_OSPEED_10MHZ         OSPD_OSPD(1)          /*!< output max speed 10M */
#define GPIO_OSPEED_50MHZ         OSPD_OSPD(3)          /*!< output max speed 50M */

/* gpio alternate function values */
#define GPIO_AFR_SET(n, af)       ((af) << ((n) * 4))
#define GPIO_AFR_MASK(n)          (0xf << ((n) * 4))
 
/* gpio alternate function */
#define AF(regval)                (BITS(0,3) & ((regval) << 0)) 
#define GPIO_AF_0                  AF(0)                /*!< alternate function selected 0 */
#define GPIO_AF_1                  AF(1)                /*!< alternate function selected 1 */
#define GPIO_AF_2                  AF(2)                /*!< alternate function selected 2 */
#define GPIO_AF_3                  AF(3)                /*!< alternate function selected 3 */
#define GPIO_AF_4                  AF(4)                /*!< alternate function selected 4 */
#define GPIO_AF_5                  AF(5)                /*!< alternate function selected 5 */
#define GPIO_AF_6                  AF(6)                /*!< alternate function selected 6 */
#define GPIO_AF_7                  AF(7)                /*!< alternate function selected 7 */
#define GPIO_AF_8                  AF(8)                /*!< alternate function selected 8 */
#define GPIO_AF_9                  AF(9)                /*!< alternate function selected 9 */
#define GPIO_AF_10                 AF(10)               /*!< alternate function selected 10 */
#define GPIO_AF_11                 AF(11)               /*!< alternate function selected 11 */

/* function declarations */

/* reset gpio port */
void gpio_deinit(uint32_t port);
/* set gpio mode */
void gpio_mode_set(uint32_t port, uint8_t mode, uint8_t pull_up_down,uint16_t pin);
/* set gpio output type and speed */
void gpio_output_options_set(uint32_t port, uint8_t otype, uint8_t speed,uint16_t pin);

/* set gpio pin bit */
void gpio_bit_set(uint32_t port, uint16_t pin);
/* reset gpio pin bit */
void gpio_bit_reset(uint32_t port, uint16_t pin);
/* write data to the specified gpio pin */
void gpio_bit_write(uint32_t port, uint16_t pin, bit_status bit_value);
/* write data to the specified gpio port */
void gpio_port_write(uint32_t port, uint16_t data);

/* get gpio pin input status */
FlagStatus gpio_input_bit_get(uint32_t port, uint16_t pin);
/* get gpio port input status */
uint16_t gpio_input_port_get(uint32_t port);
/* get gpio pin output status */
FlagStatus gpio_output_bit_get(uint32_t port, uint16_t pin);
/* get gpio port output status */
uint16_t gpio_output_port_get(uint32_t port);

/* set gpio alternate function */
void gpio_af_set(uint32_t port, uint8_t alt_func_num, uint16_t pin);
/* lock gpio pin bit */
void gpio_pin_lock(uint32_t port, uint16_t pin);

#ifdef GD32F170_190

/* toggle gpio pin status */
void gpio_bit_toggle(uint32_t port, uint16_t pin);
/* toggle gpio port status */
void gpio_port_toggle(uint32_t port, uint16_t pin);

#endif /* GD32F170_190 */

#endif /* GD32F1X0_GPIO_H */
