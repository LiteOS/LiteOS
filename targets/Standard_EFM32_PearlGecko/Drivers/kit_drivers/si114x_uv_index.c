/**************************************************************************//**
 * @file
 * @brief Si114x UV Index measurement
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

#include "si114x_functions.h"

/** Si114x register settings. */
#define UV_TASKLIST           0x80              /**< UV task in CHLIST */
#define UV_IRQ_EN             IE_ALS_EVRYSAMPLE /**< UV interrupt enable */
#define SI1146_DEVICE_ID      0x46              /**< Si1146 device ID */


/**************************************************************************//**
 * @brief Si114x PGM toolkit functions uses a void* to pass hardware parameters
   through to the lower level i2c functions. This struct is passed down
   to the low level i2c functions using that pointer.
 *****************************************************************************/
static void * si114x_handle;

/**************************************************************************//**
 * @brief
  *  Reads the UV index measurement data from the
  *  Si114x.
 * @param[out] uvIndex
 *   The UV index read from the sensor
 * @return
 *   Returns 0.
 *****************************************************************************/
int Si114x_MeasureUVIndex(u16 *uvIndex)
{
  u16 data;
  volatile u8 regval;
  int      retval = 0;
  Si114xPsAlsForce(si114x_handle);


  regval = Si114xReadFromRegister(si114x_handle, REG_AUX_DATA0); /*read sample data from si114x */
  data = regval;
  regval = Si114xReadFromRegister(si114x_handle, REG_AUX_DATA1);
  data |= regval << 8;

  //check for saturation after the forced measurement and clear it if found
  //otherwise the next si114x cmd will not be performed.  Also this must be
  //done after reading the AUX_DATA register
  regval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
  while((regval & 0x80) != 0)	   //response == 0x8x means saturation occured
  {	  // Send the NOP Command to clear the error...we cannot use Si114xNop()
	  // because it first checks if REG_RESPONSE < 0 and if so it does not
	  // perform the cmd. Since we have a saturation REG_RESPONSE will be <0
	  Si114xWriteToRegister(si114x_handle, REG_COMMAND, 0x00);
	  regval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);

  }


  /*round to nearest*/
  *uvIndex  = data + 50;
  *uvIndex /= 100;
  /*clear irq*/

  if(*uvIndex > 10)
	  *uvIndex  = data + 50;
  Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, 0xff);
  return retval;
}


/**************************************************************************//**
 * @brief
  *  Set the VIS_RANGE bit of the optical sensor
  *  Si114x.
 * @param[in] vis_range_bit
 *   the boolean value to set the bit to
 * @return
 *   Returns 0.
 *****************************************************************************/
int Si114x_EnableVisRange(int vis_range_bit)
{	s16         retval = 0;
	if( vis_range_bit == 1 )
		retval += Si114xParamSet(si114x_handle, PARAM_ALSVIS_ADC_MISC, RANGE_EN);
	else
		retval += Si114xParamSet(si114x_handle, PARAM_ALSVIS_ADC_MISC, 0);

	return retval;
}



/**************************************************************************//**
 * @brief
  *  Initializes and configures the Si1146 sensor for UV index measurements.
 * @param[in] fullInit
 *   The I2C peripheral to use (not used).
 * @param[in] handle
 *   The si114x handle for programmer's toolkit
 * @return
 *   Returns 0 on success.
 *****************************************************************************/
int Si114x_ConfigureUV(int fullInit, HANDLE *handle)
{
  s16          retval = 0;
  SI114X_CAL_S si114x_cal;

  /* Set handle */
  si114x_handle = *handle;

  /* Note that the Si114xReset() actually performs the following functions: */
  /*     1. Pauses all prior measurements */
  /*     2. Clear  i2c registers that need to be cleared */
  /*     3. Clears irq status to make sure INT* is negated */
  /*     4. Delays 10 ms */
  /*     5. Sends HW Key */
  if (fullInit)
     retval += Si114xReset(si114x_handle);

  /* UV Coefficients */
  si114x_get_calibration(si114x_handle, &si114x_cal, 0);
  si114x_set_ucoef(si114x_handle, 0, &si114x_cal);


  if (fullInit)
  {
     retval += Si114xParamSet(si114x_handle, PARAM_CH_LIST, UV_TASKLIST);
     retval += Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, IE_ALS_EVRYSAMPLE);

     retval += Si114xParamSet(si114x_handle, PARAM_PS_ADC_MISC, 0x24);  /* PS_ADC_MISC to high signal range */
     retval += Si114xParamSet(si114x_handle, PARAM_PS1_ADC_MUX, 0x00);  /* PS1_ADCMUX, PS2_ADCMUX, PS3_ADCMUX to small photodiode */
     retval += Si114xParamSet(si114x_handle, PARAM_PS2_ADC_MUX, 0x00);
     retval += Si114xParamSet(si114x_handle, PARAM_PS3_ADC_MUX, 0x00);
  }
  /* Configure the ALS IR channel for the same settings as PS */
  retval += Si114xParamSet(si114x_handle, PARAM_ALSIR_ADC_MISC, RANGE_EN);
  retval += Si114xParamSet(si114x_handle, PARAM_ALSVIS_ADC_MISC, RANGE_EN);
  /* If nothing went wrong after all of this time, the value */
  /* returned will be 0. Otherwise, it will be some negative */
  /* number */
  return retval;
}
