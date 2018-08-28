/***************************************************************************//**
 * @file
 * @brief capsense configuration parameters.
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __CAPSENSCONFIG_H
#define __CAPSENSCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

#define ACMP_CAPSENSE                           ACMP1
#define ACMP_CAPSENSE_CLKEN                     CMU_HFPERCLKEN0_ACMP1
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP1
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP1OUT

#define ACMP_CHANNELS           8             /**< Number of channels for the Analog Comparator */

#define BUTTON_CHANNEL          5             /**< Button channel */

#define SLIDER_PART0_CHANNEL    0             /**< Touch slider channel Part 0 */
#define SLIDER_PART1_CHANNEL    1             /**< Touch slider channel Part 1 */
#define SLIDER_PART2_CHANNEL    2             /**< Touch slider channel Part 2 */
#define SLIDER_PART3_CHANNEL    3             /**< Touch slider channel Part 3 */


#define CAPSENSE_CH_IN_USE  { true, true, true, true, false, true, false, false }

#ifdef __cplusplus
}
#endif
#endif
