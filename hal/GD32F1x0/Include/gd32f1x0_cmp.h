/*!
    \file  gd32f1x0_cmp.h
    \brief definitions for the cmp
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_CMP_H
#define GD32F1X0_CMP_H

#include "gd32f1x0.h"   

/* CMP definitions */
#define CMP                                      CMP_BASE                       /*!< CMP base address */  

/* registers definitions */
#define CMP_CS                                   REG32((CMP) + 0x00)            /*!< CMP control and status register */

/* CMP_CS bits definitions */
#define CMP_CS_CMP0EN                            BIT(0)                         /*!< comparator channel0 enable  */
#define CMP_CS_CMP0S                             BIT(1)                         /*!< comparator channel0 switch */
#define CMP_CS_CMP0M                             BITS(2,3)                      /*!< comparator channel0 mode */
#define CMP_CS_CMP0MSEL                          BITS(4,6)                      /*!< COMP0_M input selection */
#define CMP_CS_CMP0OSEL                          BITS(8,10)                     /*!< comparator channel0 output selection */
#define CMP_CS_CMP0PL                            BIT(11)                        /*!< polarity of comparator channel0 output */
#define CMP_CS_CMP0HST                           BITS(12,13)                    /*!< comparator channel0 hysteresis */
#define CMP_CS_CMP0O                             BIT(14)                        /*!< comparator channel0 output */
#define CMP_CS_CMP0LK                            BIT(15)                        /*!< comparator channel0 lock */
#define CMP_CS_CMP1EN                            BIT(16)                        /*!< comparator channel1 enable */
#define CMP_CS_CMP1M                             BITS(18,19)                    /*!< comparator channel1 mode */
#define CMP_CS_CMP1MSEL                          BITS(20,22)                    /*!< CMP1_M input selection */
#define CMP_CS_WNDEN                             BIT(23)                        /*!< window mode enable */
#define CMP_CS_CMP1OSEL                          BITS(24,26)                    /*!< comparator channel1 output selection */
#define CMP_CS_CMP1PL                            BIT(27)                        /*!< polarity of comparator channel1 output */
#define CMP_CS_CMP1HST                           BITS(28,29)                    /*!< comparator channel1 hysteresis */
#define CMP_CS_CMP1O                             BIT(30)                        /*!< comparator channel1 output */
#define CMP_CS_CMP1LK                            BIT(31)                        /*!< comparator channel1 lock */

/* consts definitions */
/* operating mode */
typedef enum{
    CMP_HIGHSPEED = 0,                                                          /*!< high speed mode */
    CMP_MIDDLESPEED,                                                            /*!< medium speed mode */
    CMP_LOWSPEED,                                                               /*!< low speed mode */
    CMP_VERYLOWSPEED                                                            /*!< very-low speed mode */
}operating_mode_enum;

/* inverting input */
typedef enum{
    CMP_1_4VREFINT = 0,                                                         /*!< VREFINT /4 input */
    CMP_1_2VREFINT,                                                             /*!< VREFINT /2 input */
    CMP_3_4VREFINT,                                                             /*!< VREFINT *3/4 input */
    CMP_VREFINT,                                                                /*!< VREFINT input */
    CMP_DAC0,                                                                   /*!< PA4 (DAC0) input */
#ifdef GD32F170_190
    CMP_DAC1,                                                                   /*!< DAC1 input */
#else
    CMP_PA5,                                                                    /*!< PA5 input */
#endif
    CMP_PA0                                                                     /*!< PA0 input */
}inverting_input_enum;

/* hysteresis */
typedef enum{
    CMP_HYSTERESIS_NO = 0,                                                      /*!< output no hysteresis */
    CMP_HYSTERESIS_LOW,                                                         /*!< output low hysteresis */
    CMP_HYSTERESIS_MIDDLE,                                                      /*!< output middle hysteresis */
    CMP_HYSTERESIS_HIGH                                                         /*!< output high hysteresis */
}cmp_hysteresis_enum;

/* output */  
typedef enum{
    CMP_OUTPUT_NONE = 0,                                                        /*!< output no selection */
    CMP_OUTPUT_TIM0BKIN,                                                        /*!< timer 0 break input */
    CMP_OUTPUT_TIM0IC0,                                                         /*!< timer 0 channel0 input capture */
    CMP_OUTPUT_TIM0OCREFCLR,                                                    /*!< timer 0 OCREF_CLR input */
    CMP_OUTPUT_TIM1IC3,                                                         /*!< timer 1 channel3 input capture */
    CMP_OUTPUT_TIM1OCREFCLR,                                                    /*!< timer 1 OCREF_CLR input */
    CMP_OUTPUT_TIM2IC0,                                                         /*!< timer 2 channel0 input capture */
    CMP_OUTPUT_TIM2OCREFCLR                                                     /*!< timer 2 OCREF_CLR input */
}cmp_output_enum;

/* comparator channel0 mode */
#define CS_CMP0M(regval)                         (BITS(2,3) & ((regval) << 2))

#define CS_CMP0M_HIGHSPEED                       CS_CMP0M(0)                    /*!< channel0 mode high speed */
#define CS_CMP0M_MIDDLESPEED                     CS_CMP0M(1)                    /*!< channel0 mode middle speed */
#define CS_CMP0M_LOWSPEED                        CS_CMP0M(2)                    /*!< channel0 mode low speed */
#define CS_CMP0M_VERYLOWSPEED                    CS_CMP0M(3)                    /*!< channel0 mode very low speed */

/* comparator channel0 inverting input */
#define CS_CMP0MSEL(regval)                      (BITS(4,6) & ((regval) << 4))

#define CS_CMP0MSEL_1_4VREFINT                   CS_CMP0MSEL(0)                 /*!< channel0 inverting input 1/4 Vrefint */
#define CS_CMP0MSEL_1_2VREFINT                   CS_CMP0MSEL(1)                 /*!< channel0 inverting input 1/2 Vrefint */
#define CS_CMP0MSEL_3_4VREFINT                   CS_CMP0MSEL(2)                 /*!< channel0 inverting input 3/4 Vrefint */
#define CS_CMP0MSEL_VREFINT                      CS_CMP0MSEL(3)                 /*!< channel0 inverting input Vrefint */
#define CS_CMP0MSEL_DAC0                         CS_CMP0MSEL(4)                 /*!< channel0 inverting input DAC0*/
#define CS_CMP0MSEL_PA5                          CS_CMP0MSEL(5)                 /*!< channel0 inverting input PA5*/
#define CS_CMP0MSEL_PA0                          CS_CMP0MSEL(6)                 /*!< channel0 inverting input PA0*/

/* comparator channel0 output */
#define CS_CMP0OSEL(regval)                      (BITS(8,10) & ((regval) << 8))

#define CS_CMP0OSEL_OUTPUT_NONE                  CS_CMP0OSEL(0)                 /*!< channel0 output none  */
#define CS_CMP0OSEL_OUTPUT_TIM0BKIN              CS_CMP0OSEL(1)                 /*!< channel0 output timer 0 break input */
#define CS_CMP0OSEL_OUTPUT_TIM0IC0               CS_CMP0OSEL(2)                 /*!< channel0 output timer 0 channel 0 input capture */
#define CS_CMP0OSEL_OUTPUT_TIM0OCREFCLR          CS_CMP0OSEL(3)                 /*!< channel0 output timer 0 ocrefclear input */ 
#define CS_CMP0OSEL_OUTPUT_TIM1IC3               CS_CMP0OSEL(4)                 /*!< channel0 output timer 1 channel 3 input capture */
#define CS_CMP0OSEL_OUTPUT_TIM1OCREFCLR          CS_CMP0OSEL(5)                 /*!< channel0 output timer 1 ocrefclear input */
#define CS_CMP0OSEL_OUTPUT_TIM2IC0               CS_CMP0OSEL(6)                 /*!< channel0 output timer 2 channle 0 input capture */
#define CS_CMP0OSEL_OUTPUT_TIM2OCREFCLR          CS_CMP0OSEL(7)                 /*!< channel0 output timer 2 ocrefclear input */

/* comparator channel0 hysteresis */
#define CS_CMP0HST(regval)                       (BITS(12,13) & ((regval) << 12))
#define CS_CMP0HST_HYSTERESIS_NO                 CS_CMP0HST(0)                  /*!< channel0 output no hysteresis */
#define CS_CMP0HST_HYSTERESIS_LOW                CS_CMP0HST(1)                  /*!< channel0 output low hysteresis */
#define CS_CMP0HST_HYSTERESIS_MIDDLE             CS_CMP0HST(2)                  /*!< channel0 output middle hysteresis */
#define CS_CMP0HST_HYSTERESIS_HIGH               CS_CMP0HST(3)                  /*!< channel0 output high hysteresis */

/* comparator channel1 mode */
#define CS_CMP1M(regval)                         (BITS(18,19) & ((regval) << 18))

#define CS_CMP1M_HIGHSPEED                       CS_CMP1M(0)                    /*!< channel1 mode high speed */
#define CS_CMP1M_MIDDLESPEED                     CS_CMP1M(1)                    /*!< channel1 mode middle speed */
#define CS_CMP1M_LOWSPEED                        CS_CMP1M(2)                    /*!< channel1 mode low speed */
#define CS_CMP1M_VERYLOWSPEED                    CS_CMP1M(3)                    /*!< channel1 mode very low speed */

/* comparator channel1 inverting input */
#define CS_CMP1MSEL(regval)                      (BITS(20,22) & ((regval) << 20))

#define CS_CMP1MSEL_1_4VREFINT                   CS_CMP1MSEL(0)                 /*!< channel1 inverting input 1/4 Vrefint */
#define CS_CMP1MSEL_1_2VREFINT                   CS_CMP1MSEL(1)                 /*!< channel1 inverting input 1/2 Vrefint */
#define CS_CMP1MSEL_3_4VREFINT                   CS_CMP1MSEL(2)                 /*!< channel1 inverting input 3/4 Vrefint */
#define CS_CMP1MSEL_VREFINT                      CS_CMP1MSEL(3)                 /*!< channel1 inverting input Vrefint */
#define CS_CMP1MSEL_DAC0                         CS_CMP1MSEL(4)                 /*!< channel1 inverting input DAC0*/
#ifdef GD32F170_190
#define CS_CMP1MSEL_PA5                          CS_CMP1MSEL(5)                 /*!< channel1 inverting input PA5*/
#else
#define CS_CMP1MSEL_DAC1                         CS_CMP1MSEL(5)                 /*!< channel1 inverting input DAC1*/
#endif
#define CS_CMP1MSEL_PA0                          CS_CMP1MSEL(6)                 /*!< channel1 inverting input PA0*/

/* comparator channel1 output */
#define CS_CMP1OSEL(regval)                      (BITS(24,26) & ((regval) << 24))

#define CS_CMP1OSEL_OUTPUT_NONE                  CS_CMP1OSEL(0)                 /*!< channel1 output none  */
#define CS_CMP1OSEL_OUTPUT_TIM1BKIN              CS_CMP1OSEL(1)                 /*!< channel1 output timer 0 break input */
#define CS_CMP1OSEL_OUTPUT_TIM1IC0               CS_CMP1OSEL(2)                 /*!< channel1 output timer 0 channel 0 input capture */
#define CS_CMP1OSEL_OUTPUT_TIM1OCREFCLR          CS_CMP1OSEL(3)                 /*!< channel1 output timer 0 ocrefclear input */ 
#define CS_CMP1OSEL_OUTPUT_TIM2IC3               CS_CMP1OSEL(4)                 /*!< channel1 output timer 1 channel 3 input capture */
#define CS_CMP1OSEL_OUTPUT_TIM2OCREFCLR          CS_CMP1OSEL(5)                 /*!< channel1 output timer 1 ocrefclear input */
#define CS_CMP1OSEL_OUTPUT_TIM3IC0               CS_CMP1OSEL(6)                 /*!< channel1 output timer 2 channle 0 input capture */
#define CS_CMP1OSEL_OUTPUT_TIM3OCREFCLR          CS_CMP1OSEL(7)                 /*!< channel1 output timer 2 ocrefclear input */

/* comparator channel1 hysteresis */
#define CS_CMP1HST(regval)                       (BITS(28,29) & ((regval) << 28))
#define CS_CMP1HST_HSTHYSTERESIS_NO              CS_CMP1HST(0)                  /*!< channel1 output no hysteresis */
#define CS_CMP1HST_HYSTERESIS_LOW                CS_CMP1HST(1)                  /*!< channel1 output low hysteresis */
#define CS_CMP1HST_HYSTERESIS_MIDDLE             CS_CMP1HST(2)                  /*!< channel1 output middle hysteresis */
#define CS_CMP1HST_HYSTERESIS_HIGH               CS_CMP1HST(3)                  /*!< channel1 output high hysteresis */
/* comparator channel definitions */
#define CMP_CHANNEL_CMP0                        ((uint32_t)0x00000000)          /*!< comparator channel0 */
#define CMP_CHANNEL_CMP1                        ((uint32_t)0x00000010)          /*!< comparator channel1 */

/* comparator output level */
#define CMP_OUTPUTLEVEL_HIGH                    ((uint32_t)0x00000001)          /*!< comparator output high */
#define CMP_OUTPUTLEVEL_LOW                     ((uint32_t)0x00000000)          /*!< comparator output low */

/* output polarity of comparator */
#define CMP_OUTPUT_POLARITY_INVERTED            ((uint32_t)0x00000001)          /*!< output is inverted */
#define CMP_OUTPUT_POLARITY_NOINVERTED          ((uint32_t)0x00000000)          /*!< output is not inverted */

/* function declarations */

/* initialization functions */
/* CMP deinit */
void cmp_deinit(void);
/* CMP mode init */
void cmp_mode_init(uint32_t cmp_channal, operating_mode_enum cmp_operating_mode, inverting_input_enum cmp_inverting_input, cmp_hysteresis_enum output_hysteresis);
/* CMP output init */
void cmp_output_init(uint32_t cmp_channal, cmp_output_enum cmp_output_slection, uint32_t cmp_output_polarity);
/* CMP output polarity init */
void cmp_output_polarity_init(uint32_t cmp_channal, uint32_t cmp_output_polarity);

/* enable functions */
/* enable CMP channel */
void cmp_channel_enable(uint32_t cmp_channal);
/* disable CMP channel */
void cmp_channel_disable(uint32_t cmp_channal);
/* enable CMP switch */
void cmp_channel0_switch_enable(void);
/* disable CMP switch */
void cmp_channel0_switch_disable(void);
/* enable the window mode */
void cmp_window_enable(void);
/* disable the window mode */
void cmp_window_disable(void);
/* lock the CMP */
void cmp_lock_enable(uint32_t cmp_channal);
/* unlock the CMP */
void cmp_lock_disable(uint32_t cmp_channal);

/* output functions */
/* get output level */
uint32_t cmp_output_level(uint32_t cmp_channal);

#endif /* GD32F1X0_CMP_H */
