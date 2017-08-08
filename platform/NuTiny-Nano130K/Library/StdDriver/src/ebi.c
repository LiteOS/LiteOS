/****************************************************************************//**
 * @file     ebi.c
 * @version  V0.10
 * $Revision: 3 $
 * $Date: 14/02/05 10:36a $
 * @brief    NANO100 series EBI driver source file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"
#include "ebi.h"


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_EBI_Driver EBI Driver
  @{
*/


/** @addtogroup NANO100_EBI_EXPORTED_FUNCTIONS EBI Exported Functions
  @{
*/

/**
  * @brief  Initialize and enable EBI
  * @param[in]  u32Bank argument is reserved in NANO100 series.
  * @param[in]  u32DataWidth Data bus width. Valid values are:
  *                      - \ref EBI_BUSWIDTH_8BIT
  *                      - \ref EBI_BUSWIDTH_16BIT
  * @param[in]  u32TimingClass Default timing configuration. Valid values are:
  *                      - \ref EBI_TIMING_FASTEST
  *                      - \ref EBI_TIMING_VERYFAST
  *                      - \ref EBI_TIMING_FAST
  *                      - \ref EBI_TIMING_NORMAL
  *                      - \ref EBI_TIMING_SLOW
  *                      - \ref EBI_TIMING_VERYSLOW
  *                      - \ref EBI_TIMING_SLOWEST
  * @param[in]  u32BusMode argument is reserved in NANO100 series.
  * @param[in]  u32CSActiveLevel argument is reserved in NANO100 series.
  * @return none
  */
void EBI_Open(uint32_t u32Bank, uint32_t u32DataWidth, uint32_t u32TimingClass, uint32_t u32BusMode, uint32_t u32CSActiveLevel)
{
    EBI->EBICON = 0;

    if (u32DataWidth == EBI_BUSWIDTH_8BIT)
        EBI->EBICON &= ~EBI_EBICON_ExtBW16_Msk;
    else
        EBI->EBICON |= EBI_EBICON_ExtBW16_Msk;

    EBI->EBICON &= ~(EBI_EBICON_ExttALE_Msk | EBI_EBICON_MCLKDIV_Msk);
    switch (u32TimingClass) {
    case EBI_TIMING_FASTEST:
        EBI->EBICON |= (0 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_1 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = 0;
        break;

    case EBI_TIMING_VERYFAST:
        EBI->EBICON |= (1 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_2 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (4 << EBI_EXTIME_ExttACC_Pos) | (1 << EBI_EXTIME_ExttAHD_Pos) |
                      (2 << EBI_EXTIME_ExtIW2X_Pos) | (2 << EBI_EXTIME_ExtIR2W_Pos) |
                      (2 << EBI_EXTIME_ExtIR2R_Pos);
        break;

    case EBI_TIMING_FAST:
        EBI->EBICON |= (2 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_4 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (8 << EBI_EXTIME_ExttACC_Pos) | (2 << EBI_EXTIME_ExttAHD_Pos) |
                      (4 << EBI_EXTIME_ExtIW2X_Pos) | (4 << EBI_EXTIME_ExtIR2W_Pos) |
                      (4 << EBI_EXTIME_ExtIR2R_Pos);
        break;

    case EBI_TIMING_NORMAL:
        EBI->EBICON |= (3 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_8 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (16 << EBI_EXTIME_ExttACC_Pos) | (3 << EBI_EXTIME_ExttAHD_Pos) |
                      (8 << EBI_EXTIME_ExtIW2X_Pos) | (8 << EBI_EXTIME_ExtIR2W_Pos) |
                      (8 << EBI_EXTIME_ExtIR2R_Pos);
        break;

    case EBI_TIMING_SLOW:
        EBI->EBICON |= (4 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_16 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (20 << EBI_EXTIME_ExttACC_Pos) | (4 << EBI_EXTIME_ExttAHD_Pos) |
                      (12 << EBI_EXTIME_ExtIW2X_Pos) | (12 << EBI_EXTIME_ExtIR2W_Pos) |
                      (12 << EBI_EXTIME_ExtIR2R_Pos);
        break;

    case EBI_TIMING_VERYSLOW:
        EBI->EBICON |= (5 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_32 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (26 << EBI_EXTIME_ExttACC_Pos) | (5 << EBI_EXTIME_ExttAHD_Pos) |
                      (14 << EBI_EXTIME_ExtIW2X_Pos) | (14 << EBI_EXTIME_ExtIR2W_Pos) |
                      (14 << EBI_EXTIME_ExtIR2R_Pos);
        break;

    case EBI_TIMING_SLOWEST:
        EBI->EBICON |= (6 << EBI_EBICON_ExttALE_Pos);
        EBI->EBICON |= (EBI_MCLKDIV_32 << EBI_EBICON_MCLKDIV_Pos);
        EBI->EXTIME = (31 << EBI_EXTIME_ExttACC_Pos) | (7 << EBI_EXTIME_ExttAHD_Pos) |
                      (15 << EBI_EXTIME_ExtIW2X_Pos) | (15 << EBI_EXTIME_ExtIR2W_Pos) |
                      (15 << EBI_EXTIME_ExtIR2R_Pos);
        break;
    }

    EBI->EBICON |= EBI_EBICON_MCLKEN_Msk | EBI_EBICON_ExtEN_Msk;
}

/**
  * @brief  Disable EBI
  * @param[in]  u32Bank argument is reserved in NANO100 series.
  * @return none
  */
void EBI_Close(uint8_t u32Bank)
{
    EBI->EBICON = 0;
    EBI->EXTIME = 0;
}

/**
  * @brief  Set EBI bus timings
  * @param[in]  u32Bank argument is reserved in NANO100 series.
  * @param[in]  u32TimingConfig The new EBI timing settings. This value will be written to EXTIME register.
  * @param[in]  u32MclkDiv Divider for MCLK. Valid values are:
  *                      - \ref EBI_MCLKDIV_1
  *                      - \ref EBI_MCLKDIV_2
  *                      - \ref EBI_MCLKDIV_4
  *                      - \ref EBI_MCLKDIV_8
  *                      - \ref EBI_MCLKDIV_16
  *                      - \ref EBI_MCLKDIV_32
  * @return none
  */
void EBI_SetBusTiming(uint32_t u32Bank, uint32_t u32TimingConfig, uint32_t u32MclkDiv)
{
    EBI->EXTIME = u32TimingConfig;
    EBI->EBICON = (EBI->EBICON & ~EBI_EBICON_MCLKDIV_Msk) | (u32MclkDiv << EBI_EBICON_MCLKDIV_Pos);
}


/*@}*/ /* end of group NANO100_EBI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_EBI_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
