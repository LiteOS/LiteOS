/**************************************************************************//**
 * @file
 * @brief Kit specific board defines for the CPT007B demo
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef CPT007B_CONFIG_H
#define CPT007B_CONFIG_H

/////////////////////////////////////////////////////////////////////////////
// Project Configurations
/////////////////////////////////////////////////////////////////////////////
// The Demo mode determines the total number of total capsense buttons,
// the pins on the STK that correspond to the pins on the CPT device
// and so on. 

// 0 - STK works with CPT007B board with GPIO mode capsense buttons
// 1 - STK works with CPT112S board with I2C mode capsense buttons
#ifndef CPT_DEMO_MODE
#define CPT_DEMO_MODE             0

#if CPT_DEMO_MODE
#define TOTAL_CAPSENSE_PIN        12
#define CS0_I2C_LOC               1
#define CS0_I2C_INT_PORT          gpioPortD
#define CS0_I2C_INT_PIN           1
#define CS0_I2C_SDA_PORT          gpioPortD
#define CS0_I2C_SDA_PIN           7
#define CS0_I2C_SCL_PORT          gpioPortD
#define CS0_I2C_SCL_PIN           6
#else
#define TOTAL_CAPSENSE_PIN        7
#define CS0_0_PORT                gpioPortD
#define CS0_0_PIN                 1
#define CS0_1_PORT                gpioPortD
#define CS0_1_PIN                 3
#define CS0_2_PORT                gpioPortD
#define CS0_2_PIN                 6
#define CS0_3_PORT                gpioPortD
#define CS0_3_PIN                 7
#define CS0_4_PORT                gpioPortC
#define CS0_4_PIN                 5
#define CS0_5_PORT                gpioPortC
#define CS0_5_PIN                 12
#define CS0_6_PORT                gpioPortE
#define CS0_6_PIN                 2

#define CS0_0_PRESENT             0x01
#define CS0_1_PRESENT             0x02
#define CS0_2_PRESENT             0x04
#define CS0_3_PRESENT             0x08
#define CS0_4_PRESENT             0x10
#define CS0_5_PRESENT             0x20
#define CS0_6_PRESENT             0x40
#endif


#endif /* CPT_DEMO_MODE */
#endif /* CPT007B_CONFIG_H */
