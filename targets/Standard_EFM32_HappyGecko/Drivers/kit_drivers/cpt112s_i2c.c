/**************************************************************************//**
 * @file
 * @brief helper functions for managing capsense inputs
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

#include "cpt112s_i2c.h"
#include "cpt112s_config.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "em_device.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/******************************************************************************
 * @brief
 * I2C data structure
 * For CPT112S there are 12 capsense outputs, each pin
 * corresponds to a capsenseCurrent bit showing whether a
 * cap-sense button pressed or not, and capsensePrevious
 * bit showing the status of previous cycle.
 *
 * If capsenseCurrent==1 && capsensePrevious==0, detects a "press"
 * If capsenseCurrent==0 && capsensePrevious==1, detects a "release"
 *
 * capsenseCurrent bit-map byte:
 * | N/A | N/A | N/A | N/A| cC11 | cC10| cC9 | cC8 |
 * | cC7 | cC6 | cC5 | cC4 | cC3 | cC2 | cC1 | cC0 |
 *
 * capsensePrevious bit-map byte:
 * | N/A | N/A | N/A | N/A| cP11 | cP10| cP9 | cP8 |
 * | cP7 | cP6 | cP5 | cP4 | cP3 | cP2 | cP1 | cP0 |
 *
 * *cC,cP are abbreviation for capsenseCurrent and capsensePrevious
 *
 *****************************************************************************/

/*******************************************************************************
 ***************************** LOCAL VARIABLES *********************************
 ******************************************************************************/

// GPIO data structure declaration
static uint16_t capsenseCurrent, sliderCurrent;
static uint16_t capsensePrevious, sliderPrevious;

// Buffers
static uint8_t i2c_rxBuffer[CPT112S_I2C_RXBUFFER_SIZE];

// Transmission flags
static volatile bool i2c_rxInProgress;
static volatile bool i2c_startTx;

/*******************************************************************************
 *******************************  PROTOTYPES ***********************************
 ******************************************************************************/

static void setupOscillators(void);
static void setupI2C(void);
static void performI2CTransfer(void);
static void parseI2C(void);

/*******************************************************************************
 ******************************  FUNCTIONS   ***********************************
 ******************************************************************************/

/******************************************************************************
 * @brief Return a bitmask containing the current state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsenseCurrent(void)
{
  return capsenseCurrent;
}

/******************************************************************************
 * @brief Return a bitmask containing the previous state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsensePrevious(void)
{
  return capsensePrevious;
}

/******************************************************************************
 * @brief Return the current slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderCurrent(void)
{
  return sliderCurrent;
}

/******************************************************************************
 * @brief Return the previous slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderPrevious(void)
{
  return sliderPrevious;
}

/******************************************************************************
 * @brief  GPIO data structure initialization
 *****************************************************************************/
void CPT112S_init(void)
{
  setupOscillators();
  setupI2C();

  capsenseCurrent = 0;
  capsensePrevious = 0;
  sliderCurrent = 0xFFFF;
  sliderPrevious = 0xFFFF;
}

/******************************************************************************
 * @brief  parse i2c packet received from slave, update i2c data structure
 *****************************************************************************/
void CPT112S_update(void)
{
  // get previous states of Cap-sense button array
  capsensePrevious = capsenseCurrent;
  sliderPrevious = sliderCurrent;

  while (!GPIO_PinInGet(CS0_I2C_INT_PORT, CS0_I2C_INT_PIN))
  {
    // update current button states
    parseI2C();
  }
}

/*******************************************************************************
 ***************************** LOCAL FUNCTIONS *********************************
 ******************************************************************************/

void parseI2C()
{
  performI2CTransfer();

  if((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_TOUCH_EVENT)
  {
	  sliderCurrent = 0xFFFF;
    capsenseCurrent |= 1 << i2c_rxBuffer[1];
  }
  else if((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_RELEASE_EVENT)
  {
	  sliderCurrent = 0xFFFF;
	  capsenseCurrent &= ~(1 << i2c_rxBuffer[1]);
  }
  else if((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_SLIDER_ACTIVITY)
  {
	  sliderCurrent = (i2c_rxBuffer[1] << 8) | (i2c_rxBuffer[2]);
  }
}

/**************************************************************************//**
 * @brief  Starting oscillators and enabling clocks
 *****************************************************************************/
void setupOscillators(void)
{
  /* Enabling clock to the I2C, GPIO, LE */
  CMU_ClockEnable(cmuClock_I2C0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Starting LFXO and waiting until it is stable */
  CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

  /* Routing the LFXO clock to the RTC */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
}

/**************************************************************************//**
 * @brief  Setup I2C
 *****************************************************************************/
void setupI2C(void)
{
#ifdef I2C_INIT_FAST_MODE
  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_FAST_MODE;
  #else
  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
#endif

#ifdef CS0_SENSOR_EN_PORT
  /* Enable sensor isolation switch for external pull-up resistors */
  GPIO_PinModeSet(CS0_SENSOR_EN_PORT, CS0_SENSOR_EN_PIN, gpioModePushPull, 1);
#endif

  /* Configure SDA/SCL */
  GPIO_PinModeSet(CS0_I2C_SDA_PORT, CS0_I2C_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(CS0_I2C_SCL_PORT, CS0_I2C_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);

  // Configure interrupt pin
  GPIO_PinModeSet(CS0_I2C_INT_PORT, CS0_I2C_INT_PIN, gpioModeInput, 0);

#ifdef CS0_I2C_SDA_LOC
  /* Enable pins */
  I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
  I2C0->ROUTELOC0 = (CS0_I2C_SDA_LOC << _I2C_ROUTELOC0_SDALOC_SHIFT) |
                    (CS0_I2C_SCL_LOC << _I2C_ROUTELOC0_SCLLOC_SHIFT);
#else
  /* Enable pins at location */
  I2C0->ROUTE = I2C_ROUTE_SDAPEN |
                I2C_ROUTE_SCLPEN |
                (CS0_I2C_LOC << _I2C_ROUTE_LOCATION_SHIFT);
#endif

  /* Initializing the I2C */
  I2C_Init(I2C0, &i2cInit);

  /* Setting the status flags and index */
  i2c_rxInProgress = false;
  i2c_startTx = false;

  I2C0->CTRL |= I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;
}

/**************************************************************************//**
 * @brief  Transmitting I2C data. Will busy-wait until the transfer is complete.
 *****************************************************************************/
void performI2CTransfer(void)
{
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Initializing I2C transfer */
  i2cTransfer.addr          = CPT112S_I2C_ADDRESS;
  i2cTransfer.flags         = I2C_FLAG_READ;
  i2cTransfer.buf[0].data   = i2c_rxBuffer;
  i2cTransfer.buf[0].len    = CPT112S_I2C_RXBUFFER_SIZE;
  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress);
}
