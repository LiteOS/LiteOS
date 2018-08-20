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
#include "caplesense.h"
#include "em_emu.h"
#include "em_acmp.h"
#include "em_assert.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_core.h"
#include "em_lesense.h"

/* Capacitive sense configuration */
#include "caplesenseconfig.h"

/**************************************************************************//**
 * @brief This vector stores the latest read values from LESENSE
 * @param LESENSE_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelValues[LESENSE_CHANNELS] =
{
/*  Ch0,   Ch1,   Ch2,   Ch3,   Ch4,   Ch5,   Ch6,   Ch7    */
  0, 0, 0, 0, 0, 0, 0, 0,
/*  Ch8,   Ch9,   Ch10,  Ch11,  Ch12,  Ch13,  Ch14,  Ch15   */
  0, 0, 0, 0, 0, 0, 0, 0
};


/**************************************************************************//**
 * @brief  This stores the maximum values seen by a channel
 * @param LESENSE_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelMaxValues[LESENSE_CHANNELS] =
{
/*  Ch0,   Ch1,   Ch2,   Ch3,   Ch4,   Ch5,   Ch6,   Ch7    */
  1, 1, 1, 1, 1, 1, 1, 1,
/*  Ch8,   Ch9,   Ch11,  Ch11,  Ch12,  Ch13,  Ch14,  Ch15   */
  1, 1, 1, 1, 1, 1, 1, 1
};

/**************************************************************************//**
 * @brief  A bit vector which represents the channels to iterate through
 * @param LESENSE_CHANNELS Vector of channels.
 *****************************************************************************/
static const bool channelsInUse[LESENSE_CHANNELS] = LESENSE_CAPSENSE_CH_IN_USE;

/**************************************************************************//**
 * Prototypes
 *****************************************************************************/
void CAPLESENSE_setupCMU(void);
void CAPLESENSE_setupGPIO(void);
void CAPLESENSE_setupACMP(void);


/**************************************************************************//**
 * Local variables
 *****************************************************************************/
/** Callback function for LESENSE interrupts. */
static void (*lesenseScanCb)(void);
/** Callback function for LESENSE interrupts. */
static void (*lesenseChCb)(void);

/** The current channel we are sensing */
static volatile uint8_t currentChannel;



/**************************************************************************//**
 * @brief  Setup the CMU
 *****************************************************************************/
void CAPLESENSE_setupCMU(void)
{
  /* Ensure core frequency has been updated */
  SystemCoreClockUpdate();

  /* Select clock source for HF clock. */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  /* Select clock source for LFA clock. */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  /* Select clock source for LFB clock. */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_Disabled);

  /* Enable HF peripheral clock. */
  CMU_ClockEnable(cmuClock_HFPER, 1);
  /* Enable clock for GPIO. */
  CMU_ClockEnable(cmuClock_GPIO, 1);
  /* Enable clock for ACMP0. */
  CMU_ClockEnable(cmuClock_ACMP0, 1);
  /* Enable clock for ACMP1. */
  CMU_ClockEnable(cmuClock_ACMP1, 1);
  /* Enable CORELE clock. */
  CMU_ClockEnable(cmuClock_CORELE, 1);
  /* Enable clock for LESENSE. */
  CMU_ClockEnable(cmuClock_LESENSE, 1);

  /* Enable clock divider for LESENSE. */
  CMU_ClockDivSet(cmuClock_LESENSE, cmuClkDiv_1);
}


/**************************************************************************//**
 * @brief  Setup the GPIO
 *****************************************************************************/
void CAPLESENSE_setupGPIO(void)
{
  /* Configure the drive strength of the ports for the light sensor. */
  GPIO_DriveModeSet(CAPLESENSE_SLIDER_PORT0, gpioDriveModeStandard);

  /* Initialize the 4 GPIO pins of the touch slider for using them as LESENSE
   * scan channels for capacitive sensing. */
  GPIO_PinModeSet(CAPLESENSE_SLIDER_PORT0, CAPLESENSE_SLIDER0_PIN, gpioModeDisabled, 0);
  GPIO_PinModeSet(CAPLESENSE_SLIDER_PORT0, CAPLESENSE_SLIDER1_PIN, gpioModeDisabled, 0);
  GPIO_PinModeSet(CAPLESENSE_SLIDER_PORT0, CAPLESENSE_SLIDER2_PIN, gpioModeDisabled, 0);
  GPIO_PinModeSet(CAPLESENSE_SLIDER_PORT0, CAPLESENSE_SLIDER3_PIN, gpioModeDisabled, 0);
}


/**************************************************************************//**
 * @brief  Setup the ACMP
 *****************************************************************************/
void CAPLESENSE_setupACMP(void)
{
  /* ACMP capsense configuration constant table. */
  static const ACMP_CapsenseInit_TypeDef initACMP =
  {
    .fullBias                 = false,
    .halfBias                 = false,
    .biasProg                 =                  0x7,
    .warmTime                 = acmpWarmTime512,
    .hysteresisLevel          = acmpHysteresisLevel7,
    .resistor                 = acmpResistor0,
    .lowPowerReferenceEnabled = false,
    .vddLevel                 =                 0x3D,
    .enable                   = false
  };


  /* Configure ACMP locations, ACMP output to pin disabled. */
  ACMP_GPIOSetup(ACMP0, 0, false, false);
  ACMP_GPIOSetup(ACMP1, 0, false, false);

  /* Initialize ACMPs in capacitive sense mode. */
  ACMP_CapsenseInit(ACMP0, &initACMP);
  ACMP_CapsenseInit(ACMP1, &initACMP);

  /* Don't enable ACMP, LESENSE controls it! */
}


/**************************************************************************//**
 * @brief  Setup the LESENSE for capavitive sensing
 * @param sleep If true, go into sleep mode.
 *****************************************************************************/
void CAPLESENSE_setupLESENSE(bool sleep)
{
  uint8_t     i,j;
  static bool init = true;

  /* Array for storing the calibration values. */
  static uint16_t capsenseCalibrateVals[4];

  /* LESENSE channel configuration constant table in sense mode. */
  static const LESENSE_ChAll_TypeDef initChsSense = LESENSE_CAPSENSE_SCAN_CONF_SENSE;
  /* LESENSE channel configuration constant table in sleep mode. */
  static const LESENSE_ChAll_TypeDef initChsSleep = LESENSE_CAPSENSE_SCAN_CONF_SLEEP;
  /* LESENSE central configuration constant table. */
  static const LESENSE_Init_TypeDef  initLESENSE =
  {
    .coreCtrl         =
    {
      .scanStart    = lesenseScanStartPeriodic,
      .prsSel       = lesensePRSCh0,
      .scanConfSel  = lesenseScanConfDirMap,
      .invACMP0     = false,
      .invACMP1     = false,
      .dualSample   = false,
      .storeScanRes = false,
      .bufOverWr    = true,
      .bufTrigLevel = lesenseBufTrigHalf,
      .wakeupOnDMA  = lesenseDMAWakeUpDisable,
      .biasMode     = lesenseBiasModeDutyCycle,
      .debugRun     = false
    },

    .timeCtrl         =
    {
      .startDelay     =          0U
    },

    .perCtrl          =
    {
      .dacCh0Data     = lesenseDACIfData,
      .dacCh0ConvMode = lesenseDACConvModeDisable,
      .dacCh0OutMode  = lesenseDACOutModeDisable,
      .dacCh1Data     = lesenseDACIfData,
      .dacCh1ConvMode = lesenseDACConvModeDisable,
      .dacCh1OutMode  = lesenseDACOutModeDisable,
      .dacPresc       =                        0U,
      .dacRef         = lesenseDACRefBandGap,
      .acmp0Mode      = lesenseACMPModeMuxThres,
      .acmp1Mode      = lesenseACMPModeMuxThres,
      .warmupMode     = lesenseWarmupModeNormal
    },

    .decCtrl          =
    {
      .decInput  = lesenseDecInputSensorSt,
      .chkState  = false,
      .intMap    = true,
      .hystPRS0  = false,
      .hystPRS1  = false,
      .hystPRS2  = false,
      .hystIRQ   = false,
      .prsCount  = true,
      .prsChSel0 = lesensePRSCh0,
      .prsChSel1 = lesensePRSCh1,
      .prsChSel2 = lesensePRSCh2,
      .prsChSel3 = lesensePRSCh3
    }
  };

  /* Only initialize main LESENSE parameters once. */
  if (init)
  {
    /* Initialize LESENSE interface with RESET. */
    LESENSE_Init(&initLESENSE, true);
  }

  /* Different configuration for "sleep" and "sense" modes. */
  if (sleep)
  {
    /* Stop LESENSE before configuration. */
    LESENSE_ScanStop();

    /* Wait until the currently active scan is finished. */
    while (LESENSE_STATUS_SCANACTIVE & LESENSE_StatusGet()) ;

    /* Clear result buffer. */
    LESENSE_ResultBufferClear();

    /* Set scan frequency (in Hz). */
    (void) LESENSE_ScanFreqSet(0U, 4U);

    /* Set clock divisor for LF clock. */
    LESENSE_ClkDivSet(lesenseClkLF, lesenseClkDiv_1);

    /* Configure scan channels. */
    LESENSE_ChannelAllConfig(&initChsSleep);

    /* Restore calibration values. */
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER0_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[0]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER1_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[1]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER2_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[2]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER3_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[3]);

    /* Disable scan complete interrupt. */
    LESENSE_IntDisable(LESENSE_IEN_SCANCOMPLETE);
  }
  else
  {
    /* Stop LESENSE before configuration. */
    LESENSE_ScanStop();

    /* Wait until the currently active scan is finished. */
    while (LESENSE_STATUS_SCANACTIVE & LESENSE_StatusGet()) ;

    /* Clean scan complete interrupt flag. */
    LESENSE_IntClear(LESENSE_IEN_SCANCOMPLETE);

    /* Clear result buffer. */
    LESENSE_ResultBufferClear();

    /* Set scan frequency (in Hz). */
    (void) LESENSE_ScanFreqSet(0U, 64U);

    /* Set clock divisor for LF clock. */
    LESENSE_ClkDivSet(lesenseClkLF, lesenseClkDiv_8);

    /* Configure scan channels. */
    LESENSE_ChannelAllConfig(&initChsSense);

    /* Enable scan complete interrupt. */
    LESENSE_IntEnable(LESENSE_IEN_SCANCOMPLETE);
  }

  /* Enable LESENSE interrupt in NVIC. */
  NVIC_EnableIRQ(LESENSE_IRQn);

  /* Start scanning LESENSE channels. */
  LESENSE_ScanStart();

  /* Run it only once. */
  if (init)
  {
    /* Assuming that the pads are not touched at first, we can use the result as
     * the threshold value to calibrate the capacitive sensing in LESENSE. */
    init = false;

    /* Waiting for buffer to be full. */
    while (!(LESENSE->STATUS & LESENSE_STATUS_BUFHALFFULL)) ;

    /* Read out steady state values from LESENSE for calibration. */
    for (i = 0U, j = 0U; j < LESENSE_CHANNELS; j++)
    {
      if (channelsInUse[j])
      {
        if (i < CAPLESENSE_NUMOF_SLIDERS)
        {
          capsenseCalibrateVals[i] = LESENSE_ScanResultDataBufferGet(j)
                                     - CAPLESENSE_SENSITIVITY_OFFS;
        }
        i++;
      }
    }

    /* Set calibration values. */
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER0_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[0]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER1_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[1]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER2_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[2]);
    LESENSE_ChannelThresSet(CAPLESENSE_SLIDER3_PIN, CAPLESENSE_ACMP_VDD_SCALE, capsenseCalibrateVals[3]);
  }
}


/**************************************************************************//**
 * @brief  LESENSE callback setup
 * @param  scanCb Scan callback
 * @param  chCb Channel callback
 *****************************************************************************/
void CAPLESENSE_setupCallbacks(void (*scanCb)(void), void (*chCb)(void))
{
  lesenseScanCb = scanCb;
  lesenseChCb   = chCb;
}


/**************************************************************************//**
 * @brief  LESENSE interrupt handler
 *****************************************************************************/
void LESENSE_IRQHandler(void)
{
  uint32_t count;


  /* LESENSE scan complete interrupt. */
  if (LESENSE_IF_SCANCOMPLETE & LESENSE_IntGetEnabled())
  {
    LESENSE_IntClear(LESENSE_IF_SCANCOMPLETE);

    /* Iterate trough all channels */
    for (currentChannel = 0; currentChannel < LESENSE_CHANNELS; currentChannel++)
    {
      /* If this channel is not in use, skip to the next one */
      if (!channelsInUse[currentChannel])
      {
        continue;
      }

      /* Read out value from LESENSE buffer */
      count = LESENSE_ScanResultDataGet();

      /* Store value in channelValues */
      channelValues[currentChannel] = count;

      /* Update channelMaxValues */
      if (count > channelMaxValues[currentChannel])
      {
        channelMaxValues[currentChannel] = count;
      }
    }

    /* Call callback function. */
    if (lesenseScanCb != 0x00000000)
    {
      lesenseScanCb();
    }
  }

  /* LESENSE channel interrupt. */
  if (CAPLESENSE_CHANNEL_INT & LESENSE_IntGetEnabled())
  {
    /* Clear flags. */
    LESENSE_IntClear(CAPLESENSE_CHANNEL_INT);

    /* Call callback function. */
    if (lesenseChCb != 0x00000000)
    {
      lesenseChCb();
    }
  }
}


/**************************************************************************//**
 * @brief Get the channelValue for a sensor segment
 * @param capSegment
 * @return channel
 *****************************************************************************/
uint8_t  CAPLESENSE_getSegmentChannel(uint8_t capSegment)
{
  uint8_t channel;

  switch (capSegment)
  {
  case(0):
    channel = SLIDER_PART0_CHANNEL;
    break;
  case(1):
    channel = SLIDER_PART1_CHANNEL;
    break;
  case(2):
    channel = SLIDER_PART2_CHANNEL;
    break;
  default:
    channel = SLIDER_PART3_CHANNEL;
    break;
  }
  return channel;

}


/**************************************************************************//**
 * @brief Get the current channelValue for a channel
 * @param channel The channel.
 * @return The channelValue.
 *****************************************************************************/
uint32_t CAPLESENSE_getVal(uint8_t channel)
{
  return channelValues[channel];
}

/**************************************************************************//**
 * @brief Get the current normalized channelValue for a channel
 * @param channel The channel.
 * @return The channel value in range (0-256).
 *****************************************************************************/
uint32_t CAPLESENSE_getNormalizedVal(uint8_t channel)
{
  uint32_t max = channelMaxValues[channel];
  return (channelValues[channel] << 8) / max;
}



/**************************************************************************//**
 * @brief Get the position of the slider
 * @return The position of the slider if it can be determined,
 *         -1 otherwise.
 *****************************************************************************/
int32_t CAPLESENSE_getSliderPosition(void)
{
  int      i;
  int      minPos = -1;
  uint32_t minVal = 236; /* adjust it */
  /* Values used for interpolation. There is two more which represents the edges.
   * This makes the interpolation code a bit cleaner as we do not have to make special
   * cases for handling them */
  uint32_t interpol[6]      = { 255, 255, 255, 255, 255, 255 };
  uint32_t channelPattern[] = { 0,                        SLIDER_PART0_CHANNEL + 1,
                                SLIDER_PART1_CHANNEL + 1,
                                SLIDER_PART2_CHANNEL + 1,
                                SLIDER_PART3_CHANNEL + 1 };

  /* The calculated slider position. */
  int position;

  /* Iterate through the 4 slider bars and calculate the current value divided by
   * the maximum value multiplied by 256.
   * Note that there is an offset of 1 between channelValues and interpol.
   * This is done to make interpolation easier.
   */
  for (i = 1; i < CAPLESENSE_NUMOF_SLIDERS + 1; i++)
  {
    /* interpol[i] will be in the range 0-256 depending on channelMax */
    interpol[i]  = channelValues[channelPattern[i] - 1] << 8;
    interpol[i] /= channelMaxValues[channelPattern[i] - 1];
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
 * @brief Send the capacative sense system to sleep mode.
 *****************************************************************************/
void CAPLESENSE_Sleep(void)
{
  /* Go to EM2 and wait for the measurement to complete. */
  EMU_EnterEM2(true);
}


/**************************************************************************//**
 * @brief Initializes the capacative sense system without LESENSE.
 * @param sleep If true, go into sleep mode.
 *****************************************************************************/
void CAPLESENSE_Init(bool sleep)
{
  CORE_DECLARE_IRQ_STATE;

  /* Disable interrupts */
  CORE_ENTER_ATOMIC();

  /* Setup CMU. */
  CAPLESENSE_setupCMU();
  /* Setup GPIO. */
  CAPLESENSE_setupGPIO();
  /* Setup ACMP. */
  CAPLESENSE_setupACMP();
  /* Setup LESENSE. */
  CAPLESENSE_setupLESENSE(sleep);

  /* Initialization done, enable interrupts globally. */
  CORE_EXIT_ATOMIC();
}


