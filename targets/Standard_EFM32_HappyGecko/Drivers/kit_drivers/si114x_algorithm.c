/**************************************************************************//**
 * @brief Configures and calculates gestures from Si1147
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


#include "si114x_algorithm.h"
#include "si114x_functions.h"
#include "si1147_i2c.h"

#define LED1I                 0xb
#define LED2I                 0xb
#define LED3I                 0xb
#define UV_TASKLIST           0x80
#define GESTURE_TASKLIST      0x7
#define HOVER_TASKLIST        0x1
#define UV_IRQ_EN             IE_ALS_EVRYSAMPLE
#define GESTURE_IRQ           (IE_ALS_EVRYSAMPLE | IE_PS1_EVRYSAMPLE | IE_PS2_EVRYSAMPLE | IE_PS3_EVRYSAMPLE)
#define MEASRATE_FAST         320
#define MEASRATE_SLOW         32000
#define PS_HOVER_THRESHOLD    295
#define PS_THRESHOLD          275
#define SI1147_DEVICE_ID      0x47

/*
   The Si114x PGM toolkit functions uses a void* to pass hardware parameters
   through to the lower level i2c functions. The struct below is passed down
   to the low level i2c functions using that pointer.
*/
static si114x_i2c_t si114x_i2c;
static si114x_i2c_t *si114x_handle = &si114x_i2c;


/* Function Prototypes */
static gesture_t ProcessSi1147Samples(Si114x_Sample_TypeDef *samples);

/**************************************************************************//**
 * @brief
  *  Detects whether Si1147 is on the i2c bus
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @return
 *   Returns 1 on success. Otherwise returns 0.
 *****************************************************************************/
int Si1147_Detect_Device(I2C_TypeDef *i2c, uint8_t addr)
{
  uint8_t data;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;
  data                = Si114xReadFromRegister(si114x_handle, 0);

  if (data == SI1147_DEVICE_ID)
  {
    return 1;
  }
  return 0;
}

/**************************************************************************//**
 * @brief
  *  Enables or disables the Si1147 interrupt pin
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] enable
 *   Set to 1 to enable the interrupt output pin or set to 0 to disable the pin.
 * @return
 *   Returns error codes based on the I2CDRV.
 *****************************************************************************/
int Si1147_SetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int enable)
{
  int retval;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;
  if (enable)
  {
    retval = Si114xWriteToRegister(si114x_handle, REG_INT_CFG, ICG_INTOE);
  }
  else
  {
    retval = Si114xWriteToRegister(si114x_handle, REG_INT_CFG, 0);
  }
  return retval;
}

/**************************************************************************//**
 * @brief
  *  Get the enable status of the Si1147 interrupt pin
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] enable
 *   Will be set to 1 if the interrupt output pin is enabled
 *   or set to 0 if disabled.
 * @return
 *   Returns 0.
 *****************************************************************************/
int Si1147_GetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int *enable)
{
  int retval = 0;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;
  *enable             = Si114xReadFromRegister(si114x_handle, REG_INT_CFG);
  return retval;
}

/**************************************************************************//**
 * @brief
  *  Reads the UV measurement data and checks for object in proximity to the
  *  Si1147.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[out] uvIndex
 *   The UV index read from the sensor
 * @param[out] objectDetect
 *   Returns true if an object is detected in front of the sensor, otherwise
 *   false.
 * @return
 *   Returns 0.
 *****************************************************************************/

int Si1147_MeasureUVAndObjectPresent(I2C_TypeDef *i2c, uint8_t addr, uint16_t *uvIndex, int *objectDetect)
{
  uint16_t data;
  int      retval = 0;
  int      gestureMode;
  *objectDetect       = 0;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;
  Si1147_GetInterruptOutputEnable(i2c,addr,&gestureMode);
  if ( !gestureMode ) /* Force only if not already running swipe detection. */
  {
    Si114xPsAlsForce(si114x_handle);
    while ((Si114xReadFromRegister(si114x_handle, REG_IRQ_STATUS) & 1) == 0)
    ;                                                           /*wait for measurement data */
  }
  data  = Si114xReadFromRegister(si114x_handle, REG_AUX_DATA0); /*read sample data from si114x */
  data |= Si114xReadFromRegister(si114x_handle, REG_AUX_DATA1) << 8;
  /*round to nearest*/
  *uvIndex  = data + 50;
  *uvIndex /= 100;

  if ( !gestureMode ) /* Check for object only if not already running swipe detection. */
  {
    data  = Si114xReadFromRegister(si114x_handle, REG_PS1_DATA0); /*read sample data from si114x */
    data |= Si114xReadFromRegister(si114x_handle, REG_PS1_DATA1) << 8;
    if (data > PS_HOVER_THRESHOLD)
    {
      *objectDetect = 1;
    }
  }
  /*clear irq*/
  Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, 0xff);

  return retval;
}

/**************************************************************************//**
 * @brief
  *  Reads the PS measurement data from the Si1147.
 * @param[in] si114x_handle
 *   Contains i2c peripheral information. See definition of si114x_i2c_t.
 * @param[out] sample
 *   The sample data read from the sensor.
 *****************************************************************************/
static void readPSData(HANDLE si114x_handle, Si114x_Sample_TypeDef *sample)
{
  /*read sample data from si114x */
  sample->ps1  = Si114xReadFromRegister(si114x_handle, REG_PS1_DATA0);
  sample->ps1 |= Si114xReadFromRegister(si114x_handle, REG_PS1_DATA1) << 8;

  sample->ps2  = Si114xReadFromRegister(si114x_handle, REG_PS2_DATA0);
  sample->ps2 |= Si114xReadFromRegister(si114x_handle, REG_PS2_DATA1) << 8;

  sample->ps3  = Si114xReadFromRegister(si114x_handle, REG_PS3_DATA0);
  sample->ps3 |= Si114xReadFromRegister(si114x_handle, REG_PS3_DATA1) << 8;
}


/**************************************************************************//**
 * @brief
  *  Reads new measurement data and processes a new sample. This function
  *  should be called every time an interrupt for a new sample is received.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] timestamp
 *   The timestamp for when the sample interrupt was received.
 * @return
 *   Returns the type of gesture detected (as defined by gesture_t).
 *****************************************************************************/
gesture_t Si1147_NewSample(I2C_TypeDef *i2c, uint8_t addr, uint32_t timestamp)
{
  Si114x_Sample_TypeDef sample;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;
  sample.timestamp    = timestamp;
  /*read sample data from si114x */
  readPSData(si114x_handle, &sample);
  /*clear irq*/
  Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, Si114xReadFromRegister(si114x_handle, REG_IRQ_STATUS));
  /*look for gestures */
  return ProcessSi1147Samples(&sample);
}


/**************************************************************************//**
 * @brief
  *  Implements the algorithm for detecting gestures on the sensor STK.
  *  Should be called with new sample data every time an interrupt is
  *  received.
 * @param[in] samples
 *   New sample data received from the sensor.
 * @return
 *   Returns the type of gesture detected (as defined by gesture_t).
 *****************************************************************************/
static gesture_t ProcessSi1147Samples(Si114x_Sample_TypeDef *samples)
{
  u16        ps[3];

  static u32 ps_entry_time[3] = { 0, 0, 0 };
  static u32 ps_exit_time[3]  = { 0, 0, 0 };

  static u8  ps_state[3] = { 0, 0, 0 };

  u8         array_counter;
  u32 diff_x ;
  u32 diff_y1 ;
  u32 diff_y2 ;
  u32 ps_time[3] ;
  u32 ps_avg;
  gesture_t  ret = NONE;  /*gesture result return value */
  /*save new samples into ps array */
  ps[0] = samples->ps1;
  ps[1] = samples->ps2;
  ps[2] = samples->ps3;

  /* Check state of all three measurements */
  for (array_counter = 0; array_counter < 3; array_counter++)
  {
    /* If measurement higher than the ps_threshold value, */
    /*   record the time of entry and change the state to look for the exit time */
    if (ps[array_counter] >= PS_THRESHOLD)
    {
      if (ps_state[array_counter] == 0)
      {
        ps_state[array_counter]      = 1;
        ps_entry_time[array_counter] = samples->timestamp;
      }
    }
    else
    {
      if (ps_state[array_counter] == 1)
      {
        ps_state[array_counter]     = 0;
        ps_exit_time[array_counter] = samples->timestamp;
      }
    }
  }

  /* If there is no object in front of the board, look at history to see if a gesture occured */
  if ((ps[0] < PS_THRESHOLD) && (ps[1] < PS_THRESHOLD) && (ps[2] < PS_THRESHOLD))
  {
    /* If the ps_max values are high enough and there exit entry and exit times, */
    /*   then begin processing gestures */
    if ((ps_entry_time[0] != 0) && (ps_entry_time[1] != 0) && (ps_entry_time[2] != 0)
        && (ps_exit_time[0] != 0) && (ps_exit_time[1] != 0) && (ps_exit_time[2] != 0))
    {
      /* Make sure no timestamps overflowed, indicated possibility if any of them are close to overflowing */
      if ((ps_exit_time[0] > 0xFC000000L) || (ps_exit_time[1] > 0xFC000000L) || (ps_exit_time[2] > 0xFC000000L)
          || (ps_entry_time[0] > 0xFC000000L) || (ps_entry_time[1] > 0xFC000000L) || (ps_entry_time[2] > 0xFC000000L))
      {         /* If any of them are close to overflowing, overflow them all so they all have the same reference */
        ps_exit_time[0] += 0x1FFFFFFFL;
        ps_exit_time[1] += 0x1FFFFFFFL;
        ps_exit_time[2] += 0x1FFFFFFFL;

        ps_entry_time[0] += 0x1FFFFFFFL;
        ps_entry_time[1] += 0x1FFFFFFFL;
        ps_entry_time[2] += 0x1FFFFFFFL;
      }

      /* Calculate the midpoint (between entry and exit times) of each waveform */
      /*  the order of these midpoints helps determine the gesture */
      ps_time[0] = (ps_exit_time[0] - ps_entry_time[0]) / 2;
      ps_time[0] = ps_time[0] + ps_entry_time[0];

      ps_time[1] = (ps_exit_time[1] - ps_entry_time[1]) / 2;
      ps_time[1] = ps_time[1] + ps_entry_time[1];

      ps_time[2] = (ps_exit_time[2] - ps_entry_time[2]) / 2;
      ps_time[2] = ps_time[2] + ps_entry_time[2];

      /* The diff_x and diff_y values help determine a gesture by comparing the */
      /*  LED measurements that are on a single axis */
      if (ps_time[1] > ps_time[2])
      {
        diff_x = ps_time[1] - ps_time[2];
      }
      else
      {
        diff_x = ps_time[2] - ps_time[1];
      }
      if( ps_time[0] > ps_time[1] )
      {
        diff_y1 = ps_time[0] - ps_time[1];
      }
	  else
      {
        diff_y1 = ps_time[1] - ps_time[0];
      }

      if( ps_time[0] > ps_time[2] )
      {
        diff_y2 = ps_time[0] - ps_time[2];
      }
	  else
      {
        diff_y2 = ps_time[2] - ps_time[0];
      }


      /* Take the average of all three midpoints to make a comparison point for each midpoint */
      ps_avg = (u32) ps_time[0] + (u32) ps_time[1] + (u32) ps_time[2];
      ps_avg = ps_avg / 3;

      if ((ps_exit_time[0] - ps_entry_time[0]) > 10 || (ps_exit_time[1] - ps_entry_time[1]) > 10 || (ps_exit_time[2] - ps_entry_time[2]) > 10)
      {
        if( ( (ps_time[0] < ps_time[1]) &&  (diff_y1 > diff_x) ) || ( (ps_time[0] <= ps_time[2]) && (diff_y2 > diff_x) ) )
        {           /* An up gesture occured if the bottom LED had its midpoint first */
          ret = UP;
        }
        else if  ( ( (ps_time[0] < ps_time[1]) &&  (diff_y1 > diff_x) ) || ( (ps_time[0] > ps_time[2]) && (diff_y2 > diff_x) ) )
        {           /* A down gesture occured if the bottom LED had its midpoint last */
          ret = DOWN;
        }
        else if((ps_time[0] < ps_time[1]) && (ps_time[2] < ps_time[1]) && (diff_x > ((diff_y1+diff_y2)/2)))
        {           /* A left gesture occured if the left LED had its midpoint last */
          ret = LEFT;
        }
        else if( (ps_time[0] < ps_time[2]) && (ps_time[1] < ps_time[2])  && (diff_x > ((diff_y1+diff_y2)/2)))
        {           /* A right gesture occured if the right LED had midpoint later than the right LED */
          ret = RIGHT;
        }
      }
    }
    for (array_counter = 0; array_counter < 3; array_counter++)
    {
      ps_exit_time[array_counter]  = 0;
      ps_entry_time[array_counter] = 0;
    }
  }

  return ret;
}


/**************************************************************************//**
 * @brief
  *  Initializes and configures the Si1147 sensor.
 * @param[in] i2c
 *   The I2C peripheral to use (not used).
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] lowpower
 *   Set to 1 to initialize without autonomous mode (low power) or set to 0
 *   to enable autonomous mode.
 * @return
 *   Returns 0 on success.
 *****************************************************************************/
int Si1147_ConfigureDetection(I2C_TypeDef *i2c, uint8_t addr, int lowpower)
{
  s16          retval = 0;
  SI114X_CAL_S si114x_cal;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = i2c;


  /* Note that the Si114xReset() actually performs the following functions: */
  /*     1. Pauses all prior measurements */
  /*     2. Clear  i2c registers that need to be cleared */
  /*     3. Clears irq status to make sure INT* is negated */
  /*     4. Delays 10 ms */
  /*     5. Sends HW Key */
  retval += Si114xReset(si114x_handle);

  retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED21, (LED1I << 4) + LED2I);
  retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED3, LED3I);



  /* UV Coefficients */
  si114x_get_calibration(si114x_handle, &si114x_cal, 0);
  si114x_set_ucoef(si114x_handle, 0, &si114x_cal);



  retval += Si114xParamSet(si114x_handle, PARAM_CH_LIST, GESTURE_TASKLIST | UV_TASKLIST);

  retval += Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, GESTURE_IRQ);

  retval += Si114xParamSet(si114x_handle, PARAM_PS_ADC_MISC, 0x24);  /* PS_ADC_MISC to high signal range */
  retval += Si114xParamSet(si114x_handle, PARAM_PS1_ADC_MUX, 0x00);  /* PS1_ADCMUX, PS2_ADCMUX, PS3_ADCMUX to small photodiode */
  retval += Si114xParamSet(si114x_handle, PARAM_PS2_ADC_MUX, 0x00);
  retval += Si114xParamSet(si114x_handle, PARAM_PS3_ADC_MUX, 0x00);

  /* Configure the ALS IR channel for the same settings as PS */

  retval += Si114xParamSet(si114x_handle, PARAM_ALSIR_ADC_MISC, RANGE_EN);
  retval += Si114xParamSet(si114x_handle, PARAM_ALSVIS_ADC_MISC, RANGE_EN);

  if (!lowpower)
  {
    /* Set up how often the device wakes up to make measurements (10ms) */
    retval += Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE_MSB, (MEASRATE_FAST & 0xff00) >> 8);
    retval += Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE_LSB, MEASRATE_FAST & 0x00ff);
    /* Enable Autonomous Operation */
    retval += Si114xPsAlsAuto(si114x_handle);

  }

  /* If nothing went wrong after all of this time, the value */
  /* returned will be 0. Otherwise, it will be some negative */
  /* number */
  return retval;
}
