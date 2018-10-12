/**************************************************************************//**
 * @file
 * @brief EFM32GG_DK3750, Touch panel driver
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


#include <stdlib.h>
#include <string.h>
#include "em_device.h"
#include "em_adc.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "touch.h"
#include "calibrate.h"
#include "bsp.h"
#ifndef TOUCH_WITHOUT_STORE
#include "i2cspm.h"
#include "eeprom.h"
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define CALIBRATION_MAGIC_NUMBER     0xCA71B4A7
#define CALIBRATION_EEPROM_OFFSET    0x80
/** @endcond */

/* ADC inputs connected to touchpad */
#define ADC_X                     adcSingleInpCh1 /**< Input channel 1 */
#define ADC_Y                     adcSingleInpCh4 /**< Input channel 4 */

/* touchpad PIOs */
#define TOUCH_X1_PORT             gpioPortD     /**< Port D */
#define TOUCH_X1_PIN              5             /**< Pin 5  */
#define TOUCH_X2_PORT             gpioPortD     /**< Port D */
#define TOUCH_X2_PIN              4             /**< Pin 4  */
#define TOUCH_Y1_PORT             gpioPortD     /**< Port D */
#define TOUCH_Y1_PIN              3             /**< Pin 3  */
#define TOUCH_Y2_PORT             gpioPortD     /**< Port D */
#define TOUCH_Y2_PIN              1             /**< Pin 1  */

/** touch driver states */
typedef enum
{ TOUCH_INIT,
  TOUCH_CHECK_PRESS,
  TOUCH_MEASURE_X,
  TOUCH_MEASURE_Y } TOUCH_State_TypeDef;

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

static volatile TOUCH_State_TypeDef touch_state = TOUCH_INIT;
static ADC_InitSingle_TypeDef       sInit       = ADC_INITSINGLE_DEFAULT;
volatile TOUCH_Pos_TypeDef          newpos;
volatile TOUCH_Pos_TypeDef          current_pos;
static TOUCH_Pos_TypeDef            pos;

static void                         (*upcall)(TOUCH_Pos_TypeDef *) = 0;

uint32_t                            touch_ignore_move;
/** @endcond */

/** Calibration table */
MATRIX calibrationMatrix = { 103800, 2048, -8184704, -384, 102144, -16424640, 287650 };

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#ifndef TOUCH_WITHOUT_STORE
/**************************************************************************//**
 * @brief
 *	calculate simple checksum over POINT table
 *
 * @param[in] magic
 *	Magic number
 *
 * @param[in] data
 *	Data to be added in the checksum.
 *
 * @param[in] len
 *	NSize of data.
 *
 * @return
 *	Returns calculated checksum.
 *****************************************************************************/
static uint32_t touch_CountChecksum(uint32_t magic, uint32_t *data, uint32_t len)
{
  unsigned long checksum = magic;

  while (len--)
  {
    if (checksum & 0x80000000)
    {
      checksum <<= 1;
      checksum  |= 1;
    }
    else checksum <<= 1;
    checksum += *data;
    data++;
  }
  return(checksum);
}

/** function loads calibration table from EEPROM, validate it and if OK uses it */
static void touch_LoadCalibration(void)
{
  I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;
  uint32_t         temp, checksum;
  int              count;
  MATRIX           new_matrix;

#if !defined( BSP_STK )
  BSP_PeripheralAccess(BSP_I2C, true);
#endif

  /* Initialize I2C driver, using standard rate. Devices on DK itself */
  /* supports fast mode, but in case some slower devices are added on */
  /* prototype board, we use standard mode. */
  I2CSPM_Init(&i2cInit);
  count  = EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET, (uint8_t*) &temp, sizeof(temp));
  count += EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET + 4, (uint8_t*) &new_matrix, sizeof(new_matrix));
  if (count == sizeof(new_matrix) + 4)
  {
    if (temp == CALIBRATION_MAGIC_NUMBER)
    {
      checksum = touch_CountChecksum(temp, (uint32_t*) &new_matrix, sizeof(new_matrix) / 4);
      count    = EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET + 4 + sizeof(new_matrix), (uint8_t*) &temp, sizeof(temp));
      if (temp == checksum)
      {                                      /* looks like calibration table is valid */
        ADC_IntDisable(ADC0, ADC_IF_SINGLE); /* we need to disable ADC interrupt to avoid current_pos structure update for a while */
        memcpy(&calibrationMatrix, &new_matrix, sizeof(calibrationMatrix));
        ADC_IntEnable(ADC0, ADC_IF_SINGLE);
      }
    }
  }
}

/** function stores calibration table together with magic number and checksum */
static void touch_StoreCalibration(void)
{
  int      count;
  uint32_t temp = CALIBRATION_MAGIC_NUMBER, checksum;
  checksum = touch_CountChecksum(CALIBRATION_MAGIC_NUMBER, (uint32_t*) &calibrationMatrix, sizeof(calibrationMatrix) / 4);
  count    = EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET, (uint8_t*) &temp, sizeof(temp));
  count   += EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET + 4, (uint8_t*) &calibrationMatrix, sizeof(calibrationMatrix));
  count   += EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET + 4 + sizeof(calibrationMatrix), (uint8_t*) &checksum, sizeof(checksum));
}
#endif
/** @endcond */

/***************************************************************************//**
 * @brief
 *	Convert ADC readings into XY position
 *
 * @param[in] pos
 *	Pointer to position structure
 ******************************************************************************/
void TOUCH_RecalculatePosition(volatile TOUCH_Pos_TypeDef *pos)
{
  POINT old_pos, new_pos;

  if (pos->pen)
  {
    old_pos.x = pos->adcx;
    old_pos.y = pos->adcy;
    if (getDisplayPoint(&new_pos, &old_pos, &calibrationMatrix) == OK)
    {
      if (new_pos.x >= 0) pos->x = new_pos.x;
      else pos->x = 0;
      if (new_pos.y >= 0) pos->y = new_pos.y;
      else pos->y = 0;
    }
  }
  else
  {
    pos->x = 0;
    pos->y = 0;
  }
}

/***************************************************************************//**
 * @brief
 *	Function that calls registered upcall
 ******************************************************************************/
void TOUCH_CallUpcall(void)
{
  if (upcall)
    upcall((TOUCH_Pos_TypeDef*) &current_pos);
}

/***************************************************************************//**
 * @brief
 *	Check if cursor state changed (down or move)
 *
 * @return
 *	1 if the cursor state has changed
 ******************************************************************************/
int TOUCH_StateChanged(void)
{
  int result = 0;
  int diff, a, b;
  if (newpos.pen && !current_pos.pen) result = 1; /* pen down */
  a    = current_pos.x;
  b    = newpos.x;
  diff = a - b;
  if (abs(diff) > (int) touch_ignore_move) result = 1; /* move in X axis */
  a    = current_pos.y;
  b    = newpos.y;
  diff = a - b;
  if (abs(diff) > (int) touch_ignore_move) result = 1; /* move in Y axis */
  return result;
}

/***************************************************************************//**
 * @brief
 *	Interrupt handler is executed with frequency ~28Hz when panel is not pressed
 *	and with frequency ~140Hz when panel is pressed - this will give ~50 readings per second
 ******************************************************************************/
void ADC0_IRQHandler(void)
{
  switch (touch_state)
  {
  case TOUCH_INIT:   /* enter this state if touch panel is not pressed */
    GPIO_PinModeSet(TOUCH_Y1_PORT, TOUCH_Y1_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(TOUCH_Y2_PORT, TOUCH_Y2_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(TOUCH_X1_PORT, TOUCH_X1_PIN, gpioModeInputPullFilter , 0);
    GPIO_PinModeSet(TOUCH_X2_PORT, TOUCH_X2_PIN, gpioModeInput, 0);
    sInit.input      = ADC_Y;
    sInit.reference  = adcRefVDD;
    sInit.resolution = adcResOVS;
    sInit.acqTime    = adcAcqTime128;               /* used to slow down */
    if(GPIO_PinInGet(TOUCH_X2_PORT, TOUCH_X2_PIN))
    {
      touch_state = TOUCH_MEASURE_Y;
      GPIO_PinModeSet(TOUCH_X1_PORT, TOUCH_X1_PIN, gpioModePushPull, 1);
      GPIO_PinModeSet(TOUCH_X2_PORT, TOUCH_X2_PIN, gpioModePushPull, 0);
      GPIO_PinModeSet(TOUCH_Y1_PORT, TOUCH_Y1_PIN, gpioModeInput, 0);
      GPIO_PinModeSet(TOUCH_Y2_PORT, TOUCH_Y2_PIN, gpioModeInput, 0);
      sInit.input   = ADC_X;
      sInit.acqTime = adcAcqTime16;                  /* pressed, so speed-up */
    }
    ADC_InitSingle(ADC0, &sInit);
    break;
  case TOUCH_CHECK_PRESS:   /* checks if touch panel is still pressed */
    if( GPIO_PinInGet(TOUCH_X2_PORT, TOUCH_X2_PIN) )
    {
      touch_state = TOUCH_MEASURE_Y;
      GPIO_PinModeSet(TOUCH_X1_PORT, TOUCH_X1_PIN, gpioModePushPull, 1);
      GPIO_PinModeSet(TOUCH_X2_PORT, TOUCH_X2_PIN, gpioModePushPull, 0);
      GPIO_PinModeSet(TOUCH_Y1_PORT, TOUCH_Y1_PIN, gpioModeInput, 0);
      GPIO_PinModeSet(TOUCH_Y2_PORT, TOUCH_Y2_PIN, gpioModeInput, 0);
      sInit.input   = ADC_X;
      sInit.acqTime = adcAcqTime16;                  /* pressed, so speed-up */
      ADC_InitSingle(ADC0, &sInit);
      current_pos.pen = newpos.pen;
      TOUCH_RecalculatePosition(&newpos);
      if (newpos.pen)
      {
        int call_upcall = TOUCH_StateChanged();
        if (call_upcall)
        {
          current_pos.x = newpos.x;
          current_pos.y = newpos.y;
        }
        current_pos.adcx = newpos.adcx;
        current_pos.adcy = newpos.adcy;
        current_pos.pen  = 1;
        if (call_upcall) TOUCH_CallUpcall();
      }
      newpos.pen = 1;
    }
    else
    {
      touch_state     = TOUCH_INIT;
      newpos.pen      = 0;
      current_pos.pen = 0;
      TOUCH_CallUpcall();
    }
    break;
  case TOUCH_MEASURE_Y:                                             /* touch panel pressed, measure Y position */
    newpos.adcy = (ADC_DataSingleGet(ADC0) + 31) >> 6;              /* reduce ADC resolution to 10-bits */
    GPIO_PinModeSet(TOUCH_Y1_PORT, TOUCH_Y1_PIN, gpioModePushPull, 0);                 /* to avoid overflow in calibration routines */
    GPIO_PinModeSet(TOUCH_Y2_PORT, TOUCH_Y2_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(TOUCH_X1_PORT, TOUCH_X1_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TOUCH_X2_PORT, TOUCH_X2_PIN, gpioModeInput, 0);
    sInit.input = ADC_Y;
    ADC_InitSingle(ADC0, &sInit);
    touch_state = TOUCH_MEASURE_X;
    break;
  case TOUCH_MEASURE_X:   /* touch panel pressed, measure X position */
    newpos.adcx = (ADC_DataSingleGet(ADC0) + 31) >> 6;
    GPIO_PinModeSet(TOUCH_Y1_PORT, TOUCH_Y1_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(TOUCH_Y2_PORT, TOUCH_Y2_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(TOUCH_X1_PORT, TOUCH_X1_PIN, gpioModeInputPullFilter , 0);
    GPIO_PinModeSet(TOUCH_X2_PORT, TOUCH_X2_PIN, gpioModeInput, 0);
    sInit.input = ADC_Y;
    ADC_InitSingle(ADC0, &sInit);
    touch_state = TOUCH_CHECK_PRESS;
    break;
  default: touch_state = TOUCH_INIT;
  }
  ADC_IntClear(ADC0, ADC_IF_SINGLE);
  ADC_Start(ADC0, adcStartSingle);
}

/***************************************************************************//**
 * @brief
 *   Check status of the touch panel.
 *
 * @return
 *   0 if panel is idle (not touched).
 ******************************************************************************/
int TOUCH_IsBusy(void)
{
  if( (touch_state == TOUCH_INIT) )
  {
    return GPIO_PinInGet(TOUCH_X2_PORT, TOUCH_X2_PIN);
  }

  if( (touch_state == TOUCH_CHECK_PRESS) )
  {
    return TOUCH_BUSY_CHECK;
  }

  return(TOUCH_BUSY_SCAN);
}

/***************************************************************************//**
 * @brief
 *	Initialize touch panel driver
 *
 * @param config
 *	Driver configuration data.
 ******************************************************************************/
void TOUCH_Init(TOUCH_Config_TypeDef *config)
{
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
#ifndef TOUCH_WITHOUT_STORE
  touch_LoadCalibration();
#endif
  CMU_ClockEnable(cmuClock_ADC0, true);
  ADC_IntDisable(ADC0, _ADC_IF_MASK);
  init.prescale     = ADC_PrescaleCalc(config->frequency, 0);
  touch_ignore_move = config->ignore;
  init.ovsRateSel   = config->oversampling;
  ADC_Init(ADC0, &init);
  BSP_PeripheralAccess(BSP_TOUCH, true);
  sInit.input      = ADC_Y;
  sInit.reference  = adcRefVDD;
  sInit.resolution = adcResOVS;
  ADC_InitSingle(ADC0, &sInit);
  ADC_IntClear(ADC0, _ADC_IF_MASK);
  touch_state = TOUCH_INIT;
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);
  ADC_Start(ADC0, adcStartSingle);
}

/***************************************************************************//**
 * @brief
 *	Returns current touch position and state
 *
 * @return
 *	Current touch position and state
 ******************************************************************************/
TOUCH_Pos_TypeDef *TOUCH_GetPos(void)
{
  ADC_IntDisable(ADC0, ADC_IF_SINGLE); /* we need to disable ADC interrupt to avoid current_pos structure update for a while */
  pos.pen  = current_pos.pen;
  pos.x    = current_pos.x;
  pos.y    = current_pos.y;
  pos.adcx = current_pos.adcx;
  pos.adcy = current_pos.adcy;
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);

  return &pos;
}

/***************************************************************************//**
 * @brief
 *	Register upcall which will be call every position or state change
 *
 * @param[in] new_upcall
 *	Pointer to upcall function
 ******************************************************************************/
void TOUCH_RegisterUpcall(TOUCH_Upcall_TypeDef *new_upcall)
{
  upcall = new_upcall;
}


/***************************************************************************//**
 * @brief Set calibration table
 *
 * @param[in] displayPtr
 *	Table of display points
 *
 * @param[in] screenPtr
 *	Table of adc values reflecting display points
 *
 * @return
 *	Returns OK if calibration table is set.
 ******************************************************************************/
int TOUCH_CalibrationTable(POINT * displayPtr, POINT * screenPtr)
{
  int result;
  result = setCalibrationMatrix(displayPtr, screenPtr, &calibrationMatrix);
#ifndef TOUCH_WITHOUT_STORE
  if (result == OK)
    touch_StoreCalibration();
#endif
  return(result);
}
