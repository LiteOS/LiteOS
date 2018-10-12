/**************************************************************************//**
 * @file
 * @brief Capacitive sense driver
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

/* EM header files */
#include "em_device.h"

/* Drivers */
#include "em_acmp.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "capsense.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** The current channel we are sensing. */
static volatile uint8_t currentChannel;
/** Flag for measurement completion. */
static volatile bool measurementComplete;

#if defined(CAPSENSE_CH_IN_USE)
/**************************************************************************//**
 * @brief
 *   A bit vector which represents the channels to iterate through
 *
 * @note
 *   This api is deprecated and new application should define
 *   CAPSENSE_CHANNELS instead of CAPSENSE_CH_IN_USE.
 *
 * @param ACMP_CHANNELS
 *   Vector of channels.
 *****************************************************************************/
static const bool channelsInUse[ACMP_CHANNELS] = CAPSENSE_CH_IN_USE;
#elif defined(CAPSENSE_CHANNELS)
/**************************************************************************//**
 * @brief
 *   An array of channels that the capsense driver should iterate through.
 *
 * @param CAPSENSE_CHANNELS
 *   Initializer list that contains all the channels that the application
 *   would like to use for capsense.
 *
 * @param ACMP_CHANNELS
 *   The number of channels in the array
 *****************************************************************************/
static const ACMP_Channel_TypeDef channelList[ACMP_CHANNELS] = CAPSENSE_CHANNELS;
#endif

/**************************************************************************//**
 * @brief The NUM_SLIDER_CHANNELS specifies how many of the ACMP_CHANNELS
 *        are used for a touch slider
 *****************************************************************************/
#if !defined(NUM_SLIDER_CHANNELS)
#define NUM_SLIDER_CHANNELS 4
#endif

/**************************************************************************//**
 * @brief This vector stores the latest read values from the ACMP
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelValues[ACMP_CHANNELS] = {0};

/**************************************************************************//**
 * @brief  This stores the maximum values seen by a channel
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelMaxValues[ACMP_CHANNELS] = {0};

/** @endcond */

/**************************************************************************//**
 * @brief
 *   TIMER0 interrupt handler.
 *
 * @detail
 *   When TIMER0 expires the number of pulses on TIMER1 is inserted into
 *   channelValues. If this values is bigger than what is recorded in
 *   channelMaxValues, channelMaxValues is updated.
 *   Finally, the next ACMP channel is selected.
 *****************************************************************************/
void TIMER0_IRQHandler(void)
{
  uint32_t count;

  /* Stop timers */
  TIMER0->CMD = TIMER_CMD_STOP;
  TIMER1->CMD = TIMER_CMD_STOP;

  /* Clear interrupt flag */
  TIMER0->IFC = TIMER_IFC_OF;

  /* Read out value of TIMER1 */
  count = TIMER1->CNT;

  /* Store value in channelValues */
  channelValues[currentChannel] = count;

  /* Update channelMaxValues */
  if (count > channelMaxValues[currentChannel])
    channelMaxValues[currentChannel] = count;

  measurementComplete = true;
}

/**************************************************************************//**
 * @brief Get the current channelValue for a channel
 * @param channel The channel.
 * @return The channelValue.
 *****************************************************************************/
uint32_t CAPSENSE_getVal(uint8_t channel)
{
  return channelValues[channel];
}

/**************************************************************************//**
 * @brief Get the current normalized channelValue for a channel
 * @param channel The channel.
 * @return The channel value in range (0-256).
 *****************************************************************************/
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel)
{
  uint32_t max = channelMaxValues[channel];
  return (channelValues[channel] << 8) / max;
}

/**************************************************************************//**
 * @brief Get the state of the Gecko Button
 * @param channel The channel.
 * @return true if the button is "pressed"
 *         false otherwise.
 *****************************************************************************/
bool CAPSENSE_getPressed(uint8_t channel)
{
  uint32_t treshold;
  /* Treshold is set to 12.5% below the maximum value */
  /* This calculation is performed in two steps because channelMaxValues is
   * volatile. */
  treshold  = channelMaxValues[channel];
  treshold -= channelMaxValues[channel] >> 3;

  if (channelValues[channel] < treshold)
  {
    return true;
  }
  return false;
}

/**************************************************************************//**
 * @brief Get the position of the slider
 * @return The position of the slider if it can be determined,
 *         -1 otherwise.
 *****************************************************************************/
int32_t CAPSENSE_getSliderPosition(void)
{
  int      i;
  int      minPos = -1;
  uint32_t minVal = 224; /* 0.875 * 256 */
  /* Values used for interpolation. There is two more which represents the edges.
   * This makes the interpolation code a bit cleaner as we do not have to make special
   * cases for handling them */
  uint32_t interpol[(NUM_SLIDER_CHANNELS+2)];
  for (i = 0; i < (NUM_SLIDER_CHANNELS+2); i++)
  {
    interpol[i] = 255;
  }

  /* The calculated slider position. */
  int position;

  /* Iterate through the slider bars and calculate the current value divided by
   * the maximum value multiplied by 256.
   * Note that there is an offset of 1 between channelValues and interpol.
   * This is done to make interpolation easier.
   */
  for (i = 1; i < (NUM_SLIDER_CHANNELS+1); i++)
  {
    /* interpol[i] will be in the range 0-256 depending on channelMax */
    interpol[i]  = channelValues[i - 1] << 8;
    interpol[i] /= channelMaxValues[i - 1];
    /* Find the minimum value and position */
    if (interpol[i] < minVal)
    {
      minVal = interpol[i];
      minPos = i;
    }
  }
  /* Check if the slider has not been touched */
  if (minPos == -1)
    return -1;

  /* Start position. Shift by 4 to get additional resolution. */
  /* Because of the interpol trick earlier we have to substract one to offset that effect */
  position = (minPos - 1) << 4;

  /* Interpolate with pad to the left */
  position -= ((256 - interpol[minPos - 1]) << 3)
              / (256 - interpol[minPos]);

  /* Interpolate with pad to the right */
  position += ((256 - interpol[minPos + 1]) << 3)
              / (256 - interpol[minPos]);

  return position;
}

/**************************************************************************//**
 * @brief
 *   Start a capsense measurement of a specific channel and waits for
 *   it to complete.
 *****************************************************************************/
static void CAPSENSE_Measure(ACMP_Channel_TypeDef channel)
{
  /* Set up this channel in the ACMP. */
  ACMP_CapsenseChannelSet(ACMP_CAPSENSE, channel);

  /* Reset timers */
  TIMER0->CNT = 0;
  TIMER1->CNT = 0;

  measurementComplete = false;

  /* Start timers */
  TIMER0->CMD = TIMER_CMD_START;
  TIMER1->CMD = TIMER_CMD_START;

  /* Wait for measurement to complete */
  while ( measurementComplete == false )
  {
    EMU_EnterEM1();
  }
}

/**************************************************************************//**
 * @brief
 *   This function iterates through all the capsensors and reads and
 *   initiates a reading. Uses EM1 while waiting for the result from
 *   each sensor.
 *****************************************************************************/
void CAPSENSE_Sense(void)
{
  /* Use the default STK capacative sensing setup and enable it */
  ACMP_Enable(ACMP_CAPSENSE);

#if defined(CAPSENSE_CHANNELS)
  /* Iterate through only the channels in the channelList */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++)
  {
    CAPSENSE_Measure(channelList[currentChannel]);
  }
#else
  /* Iterate through all channels and check which channel is in use */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++)
  {
    /* If this channel is not in use, skip to the next one */
    if (!channelsInUse[currentChannel])
    {
      continue;
    }

    CAPSENSE_Measure((ACMP_Channel_TypeDef) currentChannel);
  }
#endif

  /* Disable ACMP while not sensing to reduce power consumption */
  ACMP_Disable(ACMP_CAPSENSE);
}

/**************************************************************************//**
 * @brief
 *   Initializes the capacitive sense system.
 *
 * @detail
 *   Capacitive sensing uses two timers: TIMER0 and TIMER1 as well as ACMP.
 *   ACMP is set up in cap-sense (oscillator mode).
 *   TIMER1 counts the number of pulses generated by ACMP_CAPSENSE.
 *   When TIMER0 expires it generates an interrupt.
 *   The number of pulses counted by TIMER1 is then stored in channelValues
 *****************************************************************************/
void CAPSENSE_Init(void)
{
  /* Use the default STK capacative sensing setup */
  ACMP_CapsenseInit_TypeDef capsenseInit = ACMP_CAPSENSE_INIT_DEFAULT;

  /* Enable TIMER0, TIMER1, ACMP_CAPSENSE and PRS clock */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);
#if defined(ACMP_CAPSENSE_CMUCLOCK)
  CMU_ClockEnable(ACMP_CAPSENSE_CMUCLOCK, true);
#else
  CMU->HFPERCLKEN0 |= ACMP_CAPSENSE_CLKEN;
#endif
  CMU_ClockEnable(cmuClock_PRS, true);

  /* Initialize TIMER0 - Prescaler 2^9, top value 10, interrupt on overflow */
  TIMER0->CTRL = TIMER_CTRL_PRESC_DIV512;
  TIMER0->TOP  = 10;
  TIMER0->IEN  = TIMER_IEN_OF;
  TIMER0->CNT  = 0;

  /* Initialize TIMER1 - Prescaler 2^10, clock source CC1, top value 0xFFFF */
  TIMER1->CTRL = TIMER_CTRL_PRESC_DIV1024 | TIMER_CTRL_CLKSEL_CC1;
  TIMER1->TOP  = 0xFFFF;

  /*Set up TIMER1 CC1 to trigger on PRS channel 0 */
  TIMER1->CC[1].CTRL = TIMER_CC_CTRL_MODE_INPUTCAPTURE /* Input capture      */
                       | TIMER_CC_CTRL_PRSSEL_PRSCH0   /* PRS channel 0      */
                       | TIMER_CC_CTRL_INSEL_PRS       /* PRS input selected */
                       | TIMER_CC_CTRL_ICEVCTRL_RISING /* PRS on rising edge */
                       | TIMER_CC_CTRL_ICEDGE_BOTH;    /* PRS on rising edge */

  /*Set up PRS channel 0 to trigger on ACMP1 output*/
  PRS->CH[0].CTRL = PRS_CH_CTRL_EDSEL_POSEDGE      /* Posedge triggers action */
                    | PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE      /* PRS source */
                    | PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE;     /* PRS source */

  /* Set up ACMP1 in capsense mode */
  ACMP_CapsenseInit(ACMP_CAPSENSE, &capsenseInit);

  /* Enable TIMER0 interrupt */
  NVIC_EnableIRQ(TIMER0_IRQn);
}
