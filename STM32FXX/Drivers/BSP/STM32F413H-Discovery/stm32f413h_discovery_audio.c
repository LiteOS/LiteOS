/**
  ******************************************************************************
  * @file    STM32f413h_discovery_audio.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file provides the Audio driver for the STM32F413H-DISCOVERY board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*==============================================================================
                                 User NOTES
                                 
How To use this driver:
-----------------------
   + This driver supports STM32F4xx devices on STM32F413H-DISCOVERY boards.
   + Call the function BSP_AUDIO_OUT_Init(
                                    OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                                                  OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH)
                                    Volume      : Initial volume to be set (0 is min (mute), 100 is max (100%)
                                    AudioFreq   : Audio frequency in Hz (8000, 16000, 22500, 32000...)
                                                  this parameter is relative to the audio file/stream type.
                                   )
      This function configures all the hardware required for the audio application (codec, I2C, I2S, 
      GPIOs, DMA and interrupt if needed). This function returns AUDIO_OK if configuration is OK.
      If the returned value is different from AUDIO_OK or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).
      - OUTPUT_DEVICE_SPEAKER  : only speaker will be set as output for the audio stream.
      - OUTPUT_DEVICE_HEADPHONE: only headphones will be set as output for the audio stream.
      - OUTPUT_DEVICE_BOTH     : both Speaker and Headphone are used as outputs for the audio stream
                                 at the same time.
   + Call the function BSP_AUDIO_OUT_Play(
                                  pBuffer: pointer to the audio data file address
                                  Size   : size of the buffer to be sent in Bytes
                                 )
      to start playing (for the first time) from the audio file/stream.
   + Call the function BSP_AUDIO_OUT_Pause() to pause playing   
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in 
      the STM32F413H_discovery_audio.h file. (refer to the example for more details on the callbacks implementations)
   + To Stop playing, to modify the volume level, the frequency, use the functions: BSP_AUDIO_OUT_SetVolume(), 
      AUDIO_OUT_SetFrequency(), BSP_AUDIO_OUT_SetOutputMode(), BSP_AUDIO_OUT_SetMute() and BSP_AUDIO_OUT_Stop().
   + The driver API and the callback functions are at the end of the STM32F413H_discovery_audio.h file.
 

Driver architecture:
--------------------
   + This driver provides the High Audio Layer: consists of the function API exported in the stm32f413h_discovery_audio.h file
     (BSP_AUDIO_OUT_Init(), BSP_AUDIO_OUT_Play() ...)
   + This driver provide also the Media Access Layer (MAL): which consists of functions allowing to access the media containing/
     providing the audio file/stream. These functions are also included as local functions into
     the stm32f413h_discovery_audio_codec.c file (I2Sx_Out_Init(), I2Sx_Out_DeInit(), I2Sx_In_Init() and I2Sx_In_DeInit())

Known Limitations:
------------------
   1- If the TDM Format used to play in parallel 2 audio Stream (the first Stream is configured in codec SLOT0 and second 
      Stream in SLOT1) the Pause/Resume, volume and mute feature will control the both streams.
   2- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size, 
      File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.
   3- Supports only Stereo audio streaming.
   4- Supports only 16-bits audio data size.
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f413h_discovery_audio.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */ 
  
/** @defgroup STM32F413H_DISCOVERY_AUDIO STM32F413H_DISCOVERY AUDIO
  * @brief This file includes the low layer driver for wm8994 Audio Codec
  *        available on STM32F413H-DISCOVERY board(MB1209).
  * @{
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_Private_Macros STM32F413H DISCOVERY Audio Private macros 
  * @{
  */

#define DFSDM_OVER_SAMPLING(__FREQUENCY__) \
        (__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? 256 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? 256 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? 128 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? 128 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? 64 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? 64  \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? 32 : 25  \

#define DFSDM_CLOCK_DIVIDER(__FREQUENCY__) \
        (__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? 24 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? 48 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? 24 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? 48 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? 24 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? 48  \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? 32 : 72  \
        
#define DFSDM_FILTER_ORDER(__FREQUENCY__) \
        (__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? DFSDM_FILTER_SINC3_ORDER \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? DFSDM_FILTER_SINC3_ORDER \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? DFSDM_FILTER_SINC3_ORDER \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? DFSDM_FILTER_SINC3_ORDER \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? DFSDM_FILTER_SINC4_ORDER \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? DFSDM_FILTER_SINC4_ORDER  \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? DFSDM_FILTER_SINC4_ORDER : DFSDM_FILTER_SINC4_ORDER  \

#define DFSDM_MIC_BIT_SHIFT(__FREQUENCY__) \
        (__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? 5 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? 4 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? 2 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? 2 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? 5 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? 6  \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? 2 : 0  \
        
/* Saturate the record PCM sample */
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))
/**
  * @}
  */ 
  
/** @defgroup STM32F413H_DISCOVERY_AUDIO_Private_Variables STM32F413H DISCOVERY Audio Private Variables
  * @{
  */

AUDIO_DrvTypeDef                *audio_drv;
I2S_HandleTypeDef               haudio_i2s;       /* for Audio_OUT and Audio_IN_analog mic */
I2S_HandleTypeDef               haudio_in_i2sext; /* for Analog mic with full duplex mode  */
AUDIOIN_ContextTypeDef          hAudioIn;

DFSDM_Channel_HandleTypeDef     hAudioInDfsdmChannel[DFSDM_MIC_NUMBER];  /* 5 DFSDM channel handle used for all microphones */
DFSDM_Filter_HandleTypeDef      hAudioInDfsdmFilter[DFSDM_MIC_NUMBER];   /* 5 DFSDM filter handle */
DMA_HandleTypeDef               hDmaDfsdm[DFSDM_MIC_NUMBER];             /* 5 DMA handle used for DFSDM regular conversions */

/* Buffers for right and left samples */
int32_t                         *pScratchBuff[DEFAULT_AUDIO_IN_CHANNEL_NBR];
int32_t                         ScratchSize;

uint32_t                        DmaRecHalfBuffCplt[DFSDM_MIC_NUMBER]  = {0};
uint32_t                        DmaRecBuffCplt[DFSDM_MIC_NUMBER]  = {0};

/* Application Buffer Trigger */
__IO uint32_t                   AppBuffTrigger          = 0;
__IO uint32_t                   AppBuffHalf             = 0;
__IO uint32_t                   MicBuff[DFSDM_MIC_NUMBER] = {0};
__IO uint16_t                   AudioInVolume = DEFAULT_AUDIO_IN_VOLUME; 

/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_Private_Function_Prototypes STM32F413H DISCOVERY Audio Private Prototypes
  * @{
  */
static void I2Sx_In_Init(uint32_t AudioFreq);
static void I2Sx_In_DeInit(void);
static void I2Sx_In_MspInit(void);
static void I2Sx_In_MspDeInit(void);

static void I2Sx_Out_Init(uint32_t AudioFreq);
static void I2Sx_Out_DeInit(void);

static uint8_t DFSDMx_DeInit(void);
static void DFSDMx_ChannelMspInit(void);
static void DFSDMx_ChannelMspDeInit(void);
static void DFSDMx_FilterMspInit(void);
static void DFSDMx_FilterMspDeInit(void);

/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_out_Private_Functions STM32F413H DISCOVERY AUDIO OUT Private Functions
  * @{
  */ 

/**
  * @brief  Configures the audio peripherals.
  * @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       or OUTPUT_DEVICE_BOTH.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note   The I2S PLL input clock must be done in the user application.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{ 
  uint8_t  ret = AUDIO_ERROR;
  uint32_t deviceid = 0x00;
  uint16_t buffer_fake[16] = {0x00};

  I2Sx_Out_DeInit();
  AUDIO_IO_DeInit();
  
  /* PLL clock is set depending on the AudioFreq (44.1 kHz vs 48kHz groups) */
  BSP_AUDIO_OUT_ClockConfig(&haudio_i2s, AudioFreq, NULL);
  
  /* Configure the I2S peripheral */
  haudio_i2s.Instance = AUDIO_OUT_I2Sx;
  if(HAL_I2S_GetState(&haudio_i2s) == HAL_I2S_STATE_RESET)
  {
    /* Initialize the I2S Msp: this __weak function can be rewritten by the application */
    BSP_AUDIO_OUT_MspInit(&haudio_i2s, NULL);
  }
  I2Sx_Out_Init(AudioFreq);

  AUDIO_IO_Init();

  /* wm8994 codec initialization */
  deviceid = wm8994_drv.ReadID(AUDIO_I2C_ADDRESS);

  if(deviceid == WM8994_ID)
  {
    /* Reset the Codec Registers */
    wm8994_drv.Reset(AUDIO_I2C_ADDRESS);
    /* Initialize the audio driver structure */
    audio_drv = &wm8994_drv;
    ret = AUDIO_OK;
  }
  else
  {
    ret = AUDIO_ERROR;
  }

  if(ret == AUDIO_OK)
  {
    /* Send fake I2S data in order to generate MCLK needed by WM8994 to set its registers
     * MCLK is generated only when a data stream is sent on I2S */
    HAL_I2S_Transmit_DMA(&haudio_i2s, buffer_fake, 16);
    /* Initialize the codec internal registers */
    audio_drv->Init(AUDIO_I2C_ADDRESS, OutputDevice, Volume, AudioFreq);
    /* Stop sending fake I2S data */
    HAL_I2S_DMAStop(&haudio_i2s);
  }

  return ret;
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data BYTES.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{
  /* Call the audio Codec Play function */
  if(audio_drv->Play(AUDIO_I2C_ADDRESS, pBuffer, Size) != 0)
  {  
    return AUDIO_ERROR;
  }
  else
  {
    /* Update the Media layer and enable it for play */  
    HAL_I2S_Transmit_DMA(&haudio_i2s, pBuffer, DMA_MAX(Size / AUDIODATA_SIZE));
    
    return AUDIO_OK;
  }
}

/**
  * @brief  Sends n-Bytes on the I2S interface.
  * @param  pData: pointer on data address 
  * @param  Size: number of data to be written
  */
void BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
   HAL_I2S_Transmit_DMA(&haudio_i2s, pData, Size);
}

/**
  * @brief  This function Pauses the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used.
  * @note   When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Pause(void)
{    
  /* Call the Audio Codec Pause/Resume function */
  if(audio_drv->Pause(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    HAL_I2S_DMAPause(&haudio_i2s);
    
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  This function  Resumes the audio file stream.  
  * @note   When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Resume(void)
{    
  /* Call the Media layer pause/resume function */
  /* DMA stream resumed before accessing WM8994 register as WM8994 needs the MCLK to be generated to access its registers
   * MCLK is generated only when a data stream is sent on I2S */
  HAL_I2S_DMAResume(&haudio_i2s);

  /* Call the Audio Codec Pause/Resume function */
  if(audio_drv->Resume(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_SW: for software power off (by writing registers). 
  *                            Then no need to reconfigure the Codec after power on.
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option)
{
  /* Call the Media layer stop function */
  HAL_I2S_DMAStop(&haudio_i2s);

  /* Call Audio Codec Stop function */
  if(audio_drv->Stop(AUDIO_I2C_ADDRESS, Option) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    if(Option == CODEC_PDWN_HW)
    { 
      /* Wait at least 100us */
      HAL_Delay(1);
    }
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  if(audio_drv->SetVolume(AUDIO_I2C_ADDRESS, Volume) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Cmd: Could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{ 
  /* Call the Codec Mute function */
  if(audio_drv->SetMute(AUDIO_I2C_ADDRESS, Cmd) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @param  Output: The audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output)
{
  /* Call the Codec output device function */
  if(audio_drv->SetOutputMode(AUDIO_I2C_ADDRESS, Output) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Updates the audio frequency.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note   This API should be called after the BSP_AUDIO_OUT_Init() to adjust the
  *         audio frequency.
  * @retval None
  */
void BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq)
{ 
  /* PLL clock is set depending by the AudioFreq (44.1khz vs 48khz groups) */ 
  BSP_AUDIO_OUT_ClockConfig(&haudio_i2s, AudioFreq, NULL);

  /* Disable I2S peripheral to allow access to I2S internal registers */
  __HAL_I2S_DISABLE(&haudio_i2s);
    
  /* Update the I2S audio frequency configuration */
  haudio_i2s.Init.AudioFreq = AudioFreq;
  HAL_I2S_Init(&haudio_i2s);
  
  /* Enable I2S peripheral to generate MCLK */
  __HAL_I2S_ENABLE(&haudio_i2s);
}

/**
  * @brief  Deinit the audio peripherals.
  */
void BSP_AUDIO_OUT_DeInit(void)
{
  I2Sx_Out_DeInit();
  /* DeInit the I2S MSP : this __weak function can be rewritten by the application */
  BSP_AUDIO_OUT_MspDeInit(&haudio_i2s, NULL);
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hi2s: I2S handle
  */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the remaining file size and new address offset: This function 
     should be coded by user (its prototype is already declared in STM32F413H_discovery_audio.h) */
  BSP_AUDIO_OUT_TransferComplete_CallBack();
}

/**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  hi2s: I2S handle
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the remaining file size and new address offset: This function 
     should be coded by user (its prototype is already declared in STM32F413H_discovery_audio.h) */
  BSP_AUDIO_OUT_HalfTransfer_CallBack();
}

/**
  * @brief  I2S error callbacks.
  * @param  hi2s: I2S handle
  */
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  BSP_AUDIO_OUT_Error_CallBack();
}

/**
  * @brief  Manages the DMA full Transfer complete event.
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
}

/**
  * @brief  Manages the DMA FIFO error event.
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(void)
{
}

/**
  * @brief  Initializes BSP_AUDIO_OUT MSP.
  * @param  hi2s: I2S handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_AUDIO_OUT_MspInit(I2S_HandleTypeDef *hi2s, void *Params)
{ 
  static DMA_HandleTypeDef hdma_i2s_tx;
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);

  /* Enable I2S clock */
  AUDIO_OUT_I2Sx_CLK_ENABLE();

  /* Enable MCK, SCK, WS, SD and CODEC_INT GPIO clock */
  AUDIO_OUT_I2Sx_MCK_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_SCK_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_SD_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_WS_GPIO_CLK_ENABLE();

  /* CODEC_I2S pins configuration: MCK, SCK, WS and SD pins */
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_MCK_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = AUDIO_OUT_I2Sx_MCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_MCK_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_SCK_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_I2Sx_SCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_SCK_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_WS_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_I2Sx_WS_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_WS_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_SD_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_I2Sx_SD_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_SD_GPIO_PORT, &gpio_init_structure);

  /* Enable the DMA clock */
  AUDIO_OUT_I2Sx_DMAx_CLK_ENABLE();

  if(hi2s->Instance == AUDIO_OUT_I2Sx)
  {
    /* Configure the hdma_i2s_rx handle parameters */
    hdma_i2s_tx.Init.Channel             = AUDIO_OUT_I2Sx_DMAx_CHANNEL;
    hdma_i2s_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_i2s_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_i2s_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_i2s_tx.Init.PeriphDataAlignment = AUDIO_OUT_I2Sx_DMAx_PERIPH_DATA_SIZE;
    hdma_i2s_tx.Init.MemDataAlignment    = AUDIO_OUT_I2Sx_DMAx_MEM_DATA_SIZE;
    hdma_i2s_tx.Init.Mode                = DMA_CIRCULAR;
    hdma_i2s_tx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_i2s_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_i2s_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_i2s_tx.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_i2s_tx.Init.PeriphBurst         = DMA_MBURST_SINGLE;

    hdma_i2s_tx.Instance = AUDIO_OUT_I2Sx_DMAx_STREAM;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hi2s, hdmatx, hdma_i2s_tx);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hdma_i2s_tx);

    /* Configure the DMA Stream */
    HAL_DMA_Init(&hdma_i2s_tx);
  }
  
  /* Enable and set I2Sx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(SPI3_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(SPI3_IRQn);

  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_OUT_I2Sx_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(AUDIO_OUT_I2Sx_DMAx_IRQ);
}

/**
  * @brief  Deinitializes I2S MSP.
  * @param  hi2s: I2S handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_AUDIO_OUT_MspDeInit(I2S_HandleTypeDef *hi2s, void *Params)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  
  /* I2S DMA IRQ Channel deactivation */
  HAL_NVIC_DisableIRQ(AUDIO_OUT_I2Sx_DMAx_IRQ);
  
  if(hi2s->Instance == AUDIO_OUT_I2Sx)
  {
    /* Deinitialize the DMA stream */
    HAL_DMA_DeInit(hi2s->hdmatx);
  }
  
  /* Disable I2S peripheral */
  __HAL_I2S_DISABLE(hi2s);
  
  /* Deactives CODEC_I2S pins MCK, SCK, WS and SD by putting them in input mode */
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_MCK_PIN;
  HAL_GPIO_DeInit(AUDIO_OUT_I2Sx_MCK_GPIO_PORT, gpio_init_structure.Pin);
  
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_SCK_PIN;
  HAL_GPIO_DeInit(AUDIO_OUT_I2Sx_SCK_GPIO_PORT, gpio_init_structure.Pin);
  
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_WS_PIN;
  HAL_GPIO_DeInit(AUDIO_OUT_I2Sx_WS_GPIO_PORT, gpio_init_structure.Pin);
  
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_SD_PIN;
  HAL_GPIO_DeInit(AUDIO_OUT_I2Sx_SD_GPIO_PORT, gpio_init_structure.Pin);
  
  /* Disable I2S clock */
  AUDIO_OUT_I2Sx_CLK_DISABLE();
  
  /* GPIO pins clock and DMA clock can be shut down in the application 
  by surcharging this __weak function */ 
}

/**
  * @brief  Clock Config.
  * @param  hi2s: might be required to set audio peripheral predivider if any.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  * @note   This API is called by BSP_AUDIO_OUT_Init() and BSP_AUDIO_OUT_SetFrequency()
  *         Being __weak it can be overwritten by the application
  */
__weak void BSP_AUDIO_OUT_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t AudioFreq, void *Params)
{ 
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);

  HAL_RCCEx_GetPeriphCLKConfig(&rcc_ex_clk_init_struct);

  /* Set the PLL configuration according to the audio frequency */
  if((AudioFreq == AUDIO_FREQUENCY_11K) || (AudioFreq == AUDIO_FREQUENCY_22K) || (AudioFreq == AUDIO_FREQUENCY_44K))
  {
    /* Configure PLLI2S prescalers */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1  | RCC_PERIPHCLK_PLLI2S);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;    
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 271;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 2;
    
    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);
  }
  else if(AudioFreq == AUDIO_FREQUENCY_96K) /* AUDIO_FREQUENCY_96K */
  {
    /* I2S clock config */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_PLLI2S);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;    
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 344;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 2;

    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);    
  }  
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_48K */
  {
    /* I2S clock config
    PLLI2S_VCO: VCO_344M
    I2S_CLK(first level) = PLLI2S_VCO/PLLI2SR = 344/7 = 49.142 Mhz
    I2S_CLK_x = I2S_CLK(first level)/PLLI2SDIVR = 49.142/1 = 49.142 Mhz */
    rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_PLLI2S;
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 344;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 7;
    
    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);
  }
}

/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @note   This function assumes that the I2S input clock
  *         is already configured and ready to be used.
  * @param  AudioFreq: Audio frequency to be configured for the I2S peripheral.
  */
static void I2Sx_Out_Init(uint32_t AudioFreq)
{
  /* Initialize the hAudioInI2s Instance parameter */
  haudio_i2s.Instance = AUDIO_OUT_I2Sx;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_i2s);
  
  /* I2S peripheral configuration */
  haudio_i2s.Init.AudioFreq = AudioFreq;
  haudio_i2s.Init.ClockSource = I2S_CLOCK_PLL;
  haudio_i2s.Init.CPOL = I2S_CPOL_LOW;
  haudio_i2s.Init.DataFormat = I2S_DATAFORMAT_16B;
  haudio_i2s.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  haudio_i2s.Init.Mode = I2S_MODE_MASTER_TX;
  haudio_i2s.Init.Standard =  I2S_STANDARD_PHILIPS;
  haudio_i2s.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  
  /* Init the I2S */
  HAL_I2S_Init(&haudio_i2s);

 /* Enable I2S block */
  __HAL_I2S_ENABLE(&haudio_i2s);
}

/**
  * @brief  Deinitializes the Audio Codec audio interface (I2S).
  */
static void I2Sx_Out_DeInit(void)
{
  /* Initialize the hAudioInI2s Instance parameter */
  haudio_i2s.Instance = AUDIO_OUT_I2Sx;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_i2s);

  /* DeInit the I2S */
  HAL_I2S_DeInit(&haudio_i2s);
}
 
/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_AUDIO_IN_Private_Functions STM32F413H DISCOVERY AUDIO IN Private functions
  * @{
  */ 

/**
  * @brief  Initializes wave recording.                    
  * @param  AudioFreq: Audio frequency to be configured for the audio in peripheral.
  * @param  BitRes: Audio bit resolution.
  * @param  ChnlNbr: Audio channel number.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr)
{
  return BSP_AUDIO_IN_InitEx(INPUT_DEVICE_DIGITAL_MIC, AudioFreq, BitRes, ChnlNbr);  
}

/**
  * @brief  Initializes wave recording.
  * @param  InputDevice: INPUT_DEVICE_DIGITAL_MICx or INPUT_DEVICE_ANALOG_MIC.                     
  * @param  AudioFreq: Audio frequency to be configured for the audio in peripheral.
  * @param  BitRes: Audio bit resolution.
  * @param  ChnlNbr: Audio channel number.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_InitEx(uint32_t InputDevice, uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr)
{
  uint32_t ret = AUDIO_ERROR;
  uint32_t deviceid =0;
  uint32_t mic_enabled =0;
  uint16_t buffer_fake[16] = {0x00};
  uint32_t i = 0;
  
  /* Store the audio record context */
  hAudioIn.Frequency     = AudioFreq;
  hAudioIn.BitResolution = BitRes;
  hAudioIn.InputDevice = InputDevice; 
  hAudioIn.ChannelNbr = ChnlNbr;
  
  /* Store the total number of microphones enabled */  
  for(i = 0; i < DFSDM_MIC_NUMBER; i ++)
  {
    if(((hAudioIn.InputDevice >> i) & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1)
    {
      mic_enabled++;
    }   
  }
  
  if (InputDevice == INPUT_DEVICE_ANALOG_MIC)
  { 
    InputDevice = INPUT_DEVICE_INPUT_LINE_1;
    /* INPUT_DEVICE_ANALOG_MIC */
    /* Disable I2S */
    I2Sx_In_DeInit();
    
    /* PLL clock is set depending on the AudioFreq (44.1khz vs 48khz groups) */
    BSP_AUDIO_IN_ClockConfig(AudioFreq, NULL); /* Clock config is shared between AUDIO IN and OUT for analog mic */
    
    /* I2S data transfer preparation:
    Prepare the Media to be used for the audio transfer from I2S peripheral to memory */
    haudio_i2s.Instance = AUDIO_IN_I2Sx;
    if(HAL_I2S_GetState(&haudio_i2s) == HAL_I2S_STATE_RESET)
    {
      BSP_AUDIO_OUT_MspInit(&haudio_i2s, NULL); /* Initialize GPIOs for SPI3 Master signals */
      /* Init the I2S MSP: this __weak function can be redefined by the application*/
      BSP_AUDIO_IN_MspInit(NULL);
    }
    
    /* Configure I2S */
    I2Sx_In_Init(AudioFreq);
    
    AUDIO_IO_Init();
    
    /* wm8994 codec initialization */
    deviceid = wm8994_drv.ReadID(AUDIO_I2C_ADDRESS);
    
    if((deviceid) == WM8994_ID)
    {
      /* Reset the Codec Registers */
      wm8994_drv.Reset(AUDIO_I2C_ADDRESS);
      /* Initialize the audio driver structure */
      audio_drv = &wm8994_drv;
      ret = AUDIO_OK;
    }
    else
    {
      ret = AUDIO_ERROR;
    }
    
    if(ret == AUDIO_OK)
    {
      /* Receive fake I2S data in order to generate MCLK needed by WM8994 to set its registers */
      HAL_I2S_Receive_DMA(&haudio_i2s, buffer_fake, 16);
      /* Initialize the codec internal registers */
      audio_drv->Init(AUDIO_I2C_ADDRESS, (OUTPUT_DEVICE_HEADPHONE|InputDevice), 100, AudioFreq);
      /* Stop receiving fake I2S data */
      HAL_I2S_DMAStop(&haudio_i2s);
    }    
  }
  else
  {
    if(hAudioIn.ChannelNbr != mic_enabled)  
    {
      return AUDIO_ERROR;
    }
    else
    {
      /* PLL clock is set depending on the AudioFreq (44.1khz vs 48khz groups) */
      BSP_AUDIO_IN_ClockConfig(AudioFreq, NULL); /* Clock config is shared between AUDIO IN and OUT for analog mic */
      
      /* Init the DFSDM MSP: this __weak function can be redefined by the application*/
      BSP_AUDIO_IN_MspInit(NULL);
      
      /* Default configuration of DFSDM filters and channels */
      ret = BSP_AUDIO_IN_ConfigDigitalMic(hAudioIn.InputDevice, NULL);
    } 
  }
  
  /* Return AUDIO_OK when all operations are correctly done */
  return ret;
}

/**
  * @brief  DeInitializes the audio peripheral.
  */
void BSP_AUDIO_IN_DeInit(void)
{
  if(hAudioIn.InputDevice != INPUT_DEVICE_ANALOG_MIC)
  {    
    /* MSP filters/channels initialization */   
    BSP_AUDIO_IN_MspDeInit(NULL);
    
    DFSDMx_DeInit();
  }
  else
  {
    I2Sx_In_DeInit();
  }
}

/**
  * @brief  Initializes default configuration of the Digital Filter for Sigma-Delta Modulators interface (DFSDM).
  * @param  InputDevice: The microphone to be configured. Can be INPUT_DEVICE_DIGITAL_MIC1..INPUT_DEVICE_DIGITAL_MIC5
  * @note   Channel output Clock Divider and Filter Oversampling are calculated as follow: 
  *         - Clock_Divider = CLK(input DFSDM)/CLK(micro) with
  *           1MHZ < CLK(micro) < 3.2MHZ (TYP 2.4MHZ for MP34DT01TR)
  *         - Oversampling = CLK(input DFSDM)/(Clock_Divider * AudioFreq)
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_ConfigMicDefault(uint32_t InputDevice)
{
  uint32_t i = 0, mic_init[DFSDM_MIC_NUMBER] = {0};
  uint32_t filter_ch = 0, mic_num = 0;
  
  DFSDM_Filter_TypeDef* FilterInstnace[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_MIC1_FILTER, AUDIO_DFSDMx_MIC2_FILTER, AUDIO_DFSDMx_MIC3_FILTER, AUDIO_DFSDMx_MIC4_FILTER, AUDIO_DFSDMx_MIC5_FILTER};
  DFSDM_Channel_TypeDef* ChannelInstnace[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_MIC1_CHANNEL, AUDIO_DFSDMx_MIC2_CHANNEL, AUDIO_DFSDMx_MIC3_CHANNEL, AUDIO_DFSDMx_MIC4_CHANNEL, AUDIO_DFSDMx_MIC5_CHANNEL};
  uint32_t DigitalMicPins[DFSDM_MIC_NUMBER] = {DFSDM_CHANNEL_SAME_CHANNEL_PINS, DFSDM_CHANNEL_SAME_CHANNEL_PINS, DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS, DFSDM_CHANNEL_SAME_CHANNEL_PINS, DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS};
  uint32_t DigitalMicType[DFSDM_MIC_NUMBER] = {DFSDM_CHANNEL_SPI_RISING, DFSDM_CHANNEL_SPI_RISING, DFSDM_CHANNEL_SPI_FALLING, DFSDM_CHANNEL_SPI_RISING, DFSDM_CHANNEL_SPI_FALLING};
  uint32_t Channel4Filter[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_MIC1_CHANNEL_FOR_FILTER, AUDIO_DFSDMx_MIC2_CHANNEL_FOR_FILTER, AUDIO_DFSDMx_MIC3_CHANNEL_FOR_FILTER, AUDIO_DFSDMx_MIC4_CHANNEL_FOR_FILTER, AUDIO_DFSDMx_MIC5_CHANNEL_FOR_FILTER};
  
  for(i = 0; i < hAudioIn.ChannelNbr; i++)
  {  
    if(((InputDevice & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] != 1))
    { 
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC1);   
    }    
    else if(((InputDevice & INPUT_DEVICE_DIGITAL_MIC2) == INPUT_DEVICE_DIGITAL_MIC2) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC2); 
    } 
    else if(((InputDevice & INPUT_DEVICE_DIGITAL_MIC3) == INPUT_DEVICE_DIGITAL_MIC3) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC3); 
    } 
    else if(((InputDevice & INPUT_DEVICE_DIGITAL_MIC4) == INPUT_DEVICE_DIGITAL_MIC4) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)] != 1))
    { 
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC4); 
    } 
    else if(((InputDevice & INPUT_DEVICE_DIGITAL_MIC5) == INPUT_DEVICE_DIGITAL_MIC5) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC5); 
    }
  
    mic_init[mic_num] = 1;
  
    HAL_DFSDM_FilterDeInit(&hAudioInDfsdmFilter[mic_num]);
    /* MIC filters  initialization */
    __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&hAudioInDfsdmFilter[mic_num]); 
    hAudioInDfsdmFilter[mic_num].Instance                          = FilterInstnace[mic_num]; 
    hAudioInDfsdmFilter[mic_num].Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
    hAudioInDfsdmFilter[mic_num].Init.RegularParam.FastMode        = ENABLE;
    hAudioInDfsdmFilter[mic_num].Init.RegularParam.DmaMode         = ENABLE;
    hAudioInDfsdmFilter[mic_num].Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
    hAudioInDfsdmFilter[mic_num].Init.InjectedParam.ScanMode       = DISABLE;
    hAudioInDfsdmFilter[mic_num].Init.InjectedParam.DmaMode        = DISABLE;
    hAudioInDfsdmFilter[mic_num].Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
    hAudioInDfsdmFilter[mic_num].Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
    hAudioInDfsdmFilter[mic_num].Init.FilterParam.SincOrder        = DFSDM_FILTER_ORDER(hAudioIn.Frequency);
    hAudioInDfsdmFilter[mic_num].Init.FilterParam.Oversampling     = DFSDM_OVER_SAMPLING(hAudioIn.Frequency);   
    hAudioInDfsdmFilter[mic_num].Init.FilterParam.IntOversampling  = 1;
    
    if(HAL_OK != HAL_DFSDM_FilterInit(&hAudioInDfsdmFilter[mic_num]))
    {
      return AUDIO_ERROR;
    }

    HAL_DFSDM_ChannelDeInit(&hAudioInDfsdmChannel[mic_num]);
    /* MIC channels initialization */
    __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&hAudioInDfsdmChannel[mic_num]);   
    hAudioInDfsdmChannel[mic_num].Init.OutputClock.Activation   = ENABLE;
    hAudioInDfsdmChannel[mic_num].Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO; 
    hAudioInDfsdmChannel[mic_num].Init.OutputClock.Divider      = DFSDM_CLOCK_DIVIDER(hAudioIn.Frequency); 
    hAudioInDfsdmChannel[mic_num].Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;  
    hAudioInDfsdmChannel[mic_num].Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
    hAudioInDfsdmChannel[mic_num].Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL; 
    hAudioInDfsdmChannel[mic_num].Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
    hAudioInDfsdmChannel[mic_num].Init.Awd.Oversampling         = 10; 
    hAudioInDfsdmChannel[mic_num].Init.Offset                   = 0;
    hAudioInDfsdmChannel[mic_num].Init.RightBitShift            = DFSDM_MIC_BIT_SHIFT(hAudioIn.Frequency);
    hAudioInDfsdmChannel[mic_num].Instance                      = ChannelInstnace[mic_num];
    hAudioInDfsdmChannel[mic_num].Init.Input.Pins               = DigitalMicPins[mic_num]; 
    hAudioInDfsdmChannel[mic_num].Init.SerialInterface.Type     = DigitalMicType[mic_num];
    
    if(HAL_OK != HAL_DFSDM_ChannelInit(&hAudioInDfsdmChannel[mic_num]))
    {
      return AUDIO_ERROR;
    }
    
    filter_ch = Channel4Filter[mic_num];      
    /* Configure injected channel */
    if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&hAudioInDfsdmFilter[mic_num], filter_ch, DFSDM_CONTINUOUS_CONV_ON))
    {
      return AUDIO_ERROR;
    }  
  }
  return AUDIO_OK;
} 

/**
  * @brief  Initializes the Digital Filter for Sigma-Delta Modulators interface (DFSDM).
  * @param  InputDevice: The microphone to be configured. Can be INPUT_DEVICE_DIGITAL_MIC1..INPUT_DEVICE_DIGITAL_MIC5
  * @param  Params : pointer on additional configuration parameters, can be NULL.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
__weak uint8_t BSP_AUDIO_IN_ConfigDigitalMic(uint32_t InputDevice, void *Params)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  
  /* Default configuration of DFSDM filters and channels */
  return(BSP_AUDIO_IN_ConfigMicDefault(InputDevice));
  /* Note: This function can be called at application level and default configuration
           can be ovewritten to fit user's need */
} 

/**
  * @brief  Allocate channel buffer scratch 
  * @param  pScratch : pointer to scratch tables.
  * @param  size: size of scratch buffer
  */
uint8_t BSP_AUDIO_IN_AllocScratch (int32_t *pScratch, uint32_t size)
{ 
  uint32_t idx;
  
  ScratchSize = size / DEFAULT_AUDIO_IN_CHANNEL_NBR;
  
  /* copy scratch pointers */
  for (idx = 0; idx < DEFAULT_AUDIO_IN_CHANNEL_NBR ; idx++)
  {
    pScratchBuff[idx] = (int32_t *)(pScratch + idx * ScratchSize);
  }
  /* Return AUDIO_OK */
  return AUDIO_OK;
}

/**
  * @brief  Starts audio recording.                    
  * @param  pBuf: Main buffer pointer for the recorded data storing
  * @param  size: Current size of the recorded buffer
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_Record(uint16_t *pBuf, uint32_t size)
{  
  hAudioIn.pRecBuf = pBuf;
  hAudioIn.RecSize = size;
  /* Reset Application Buffer Trigger */
  AppBuffTrigger = 0;
  AppBuffHalf = 0;
  
  if (hAudioIn.InputDevice == INPUT_DEVICE_DIGITAL_MIC)
  {    
    /* Call the Media layer start function for MIC1 channel */
    if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)], pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)], ScratchSize))
    {
      return AUDIO_ERROR;
    }
    
    /* Call the Media layer start function for MIC2 channel */
    if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)], pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)], ScratchSize))
    {
      return AUDIO_ERROR;
    }    
  }
  else
  {
    /* Start the process to receive the DMA */
    if (HAL_OK != HAL_I2SEx_TransmitReceive_DMA(&haudio_i2s, pBuf, pBuf, size))
    {
      return AUDIO_ERROR;
    }
  }
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}
  
/**
  * @brief  Starts audio recording.                  
  * @param  pBuf: Main buffer pointer for the recorded data storing
  * @param  size: Current size of the recorded buffer
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_RecordEx(uint32_t *pBuf, uint32_t size)
{
  uint8_t ret = AUDIO_ERROR;
  hAudioIn.RecSize = size;
  uint32_t i = 0;
  uint32_t mic_init[DFSDM_MIC_NUMBER] = {0};
  if (hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    return ret;
  }
  else
  {
    hAudioIn.MultiBuffMode = 1;
    for(i = 0; i < hAudioIn.ChannelNbr; i++)
    {
      if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] != 1))
      {
        /* Call the Media layer start function for MIC1 channel 1 */
        if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)], (int32_t*)pBuf[i], size))
        {
          return AUDIO_ERROR;
        }
        MicBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = i; 
        mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = 1;
      }
      else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC2) == INPUT_DEVICE_DIGITAL_MIC2) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] != 1))
      {
        /* Call the Media layer start function for MIC2 channel 1 */
        if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)], (int32_t*)pBuf[i], size))
        {
          return AUDIO_ERROR;
        }
        MicBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = i; 
        mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = 1;
      }
      else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC3) == INPUT_DEVICE_DIGITAL_MIC3) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)] != 1))
      {
        /* Call the Media layer start function for MIC3 channel 0 */
        if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)], (int32_t*)pBuf[i], size))
        {
          return AUDIO_ERROR;
        }
        MicBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)] = i;
        mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)] = 1;
      }
      else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC4) == INPUT_DEVICE_DIGITAL_MIC4) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)] != 1))
      {
        /* Call the Media layer start function for MIC4 channel 7 */
        if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)], (int32_t*)pBuf[i], size))
        {
          return AUDIO_ERROR;
        }
        MicBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)] = i; 
        mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)] = 1;
      }
      else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC5) == INPUT_DEVICE_DIGITAL_MIC5) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)] != 1))
      {
        /* Call the Media layer start function for MIC5 channel 6 */
        if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)], (int32_t*)pBuf[i], size))
        {
          return AUDIO_ERROR;
        }
        MicBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)] = i; 
        mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)] = 1;
      }
    }
  }
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Initializes the I2S MSP.
  */
static void I2Sx_In_MspInit(void)
{
  static DMA_HandleTypeDef hdma_i2s_rx;
  GPIO_InitTypeDef  gpio_init_structure;  

  /* Enable I2S clock */
  AUDIO_IN_I2Sx_CLK_ENABLE();

  /* Enable MCK GPIO clock, needed by the codec */
  AUDIO_OUT_I2Sx_MCK_GPIO_CLK_ENABLE();

  /* CODEC_I2S pins configuration: MCK pins */
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_MCK_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = AUDIO_OUT_I2Sx_MCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_MCK_GPIO_PORT, &gpio_init_structure);
  
  /* Enable SD GPIO clock */
  AUDIO_IN_I2Sx_EXT_SD_GPIO_CLK_ENABLE();
  /* CODEC_I2S pin configuration: SD pin */
  gpio_init_structure.Pin = AUDIO_IN_I2Sx_EXT_SD_PIN;
  gpio_init_structure.Alternate = AUDIO_IN_I2Sx_EXT_SD_AF;
  HAL_GPIO_Init(AUDIO_IN_I2Sx_EXT_SD_GPIO_PORT, &gpio_init_structure);

  /* Enable the DMA clock */
  AUDIO_IN_I2Sx_DMAx_CLK_ENABLE();
    
  if(haudio_i2s.Instance == AUDIO_IN_I2Sx)
  {
    /* Configure the hdma_i2s_rx handle parameters */
    hdma_i2s_rx.Init.Channel             = AUDIO_IN_I2Sx_DMAx_CHANNEL;
    hdma_i2s_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_i2s_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_i2s_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_i2s_rx.Init.PeriphDataAlignment = AUDIO_IN_I2Sx_DMAx_PERIPH_DATA_SIZE;
    hdma_i2s_rx.Init.MemDataAlignment    = AUDIO_IN_I2Sx_DMAx_MEM_DATA_SIZE;
    hdma_i2s_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_i2s_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_i2s_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_i2s_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_i2s_rx.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_i2s_rx.Init.PeriphBurst         = DMA_MBURST_SINGLE;
    
    hdma_i2s_rx.Instance = AUDIO_IN_I2Sx_DMAx_STREAM;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(&haudio_i2s, hdmarx, hdma_i2s_rx);
    
    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hdma_i2s_rx);
    
    /* Configure the DMA Stream */
    HAL_DMA_Init(&hdma_i2s_rx);
  }
  
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_IN_I2Sx_DMAx_IRQ, AUDIO_IN_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(AUDIO_IN_I2Sx_DMAx_IRQ);	
}

/**
  * @brief  De-Initializes the I2S MSP.
  */
static void I2Sx_In_MspDeInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* I2S DMA IRQ Channel deactivation */
  HAL_NVIC_DisableIRQ(AUDIO_IN_I2Sx_DMAx_IRQ);
  
  if(haudio_i2s.Instance == AUDIO_IN_I2Sx)
  {
    /* Deinitialize the DMA stream */
    HAL_DMA_DeInit(haudio_i2s.hdmarx);
  }
  
  /* Disable I2S peripheral */
  __HAL_I2S_DISABLE(&haudio_i2s);
  
  /* Deactives CODEC_I2S pins MCK by putting them in input mode */
  gpio_init_structure.Pin = AUDIO_OUT_I2Sx_MCK_PIN;
  HAL_GPIO_DeInit(AUDIO_OUT_I2Sx_MCK_GPIO_PORT, gpio_init_structure.Pin);
  
  gpio_init_structure.Pin = AUDIO_IN_I2Sx_EXT_SD_PIN;
  HAL_GPIO_DeInit(AUDIO_IN_I2Sx_EXT_SD_GPIO_PORT, gpio_init_structure.Pin);
  
  /* Disable I2S clock */
  AUDIO_IN_I2Sx_CLK_DISABLE();  
}

/**
  * @brief  Initializes BSP_AUDIO_IN MSP.
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_AUDIO_IN_MspInit(void *Params)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  
  if(hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    I2Sx_In_MspInit();
  }
  else
  {
    /* MSP channels initialization */
    DFSDMx_ChannelMspInit();
    
    /* MSP filters initialization */
    DFSDMx_FilterMspInit();  
  }
}

/**
  * @brief  De-Initializes BSP_AUDIO_IN MSP. 
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_AUDIO_IN_MspDeInit(void *Params)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  
  if(hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    I2Sx_In_MspDeInit();
  }
  else
  {
    /* MSP channels initialization */
    DFSDMx_ChannelMspDeInit();
    
    /* MSP filters initialization */
    DFSDMx_FilterMspDeInit();  
  }
}

/**
  * @brief  Clock Config.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  * @note   This API is called by BSP_AUDIO_OUT_Init() and BSP_AUDIO_OUT_SetFrequency()
  *         Being __weak it can be overwritten by the application
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
__weak uint8_t BSP_AUDIO_IN_ClockConfig(uint32_t AudioFreq, void *Params)
{
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  
  HAL_RCCEx_GetPeriphCLKConfig(&rcc_ex_clk_init_struct);

  /* Set the PLL configuration according to the audio frequency */
  if((AudioFreq == AUDIO_FREQUENCY_11K) || (AudioFreq == AUDIO_FREQUENCY_22K) || (AudioFreq == AUDIO_FREQUENCY_44K))
  {
    /* Configure PLLI2S prescalers */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_DFSDM | RCC_PERIPHCLK_DFSDM2);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.Dfsdm2ClockSelection = RCC_DFSDM2CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 271;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 2;

    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);
  }
  else if(AudioFreq == AUDIO_FREQUENCY_96K)
  {
    /* I2S clock config */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_DFSDM | RCC_PERIPHCLK_DFSDM2);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.Dfsdm2ClockSelection = RCC_DFSDM2CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 344;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 2;

    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);    
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K */
  {
    /* I2S clock config
    PLLI2S_VCO: VCO_344M
    I2S_CLK(first level) = PLLI2S_VCO/PLLI2SR = 344/7 = 49.142 Mhz
    I2S_CLK_x = I2S_CLK(first level)/PLLI2SDIVR = 49.142/1 = 49.142 Mhz */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_DFSDM | RCC_PERIPHCLK_DFSDM2);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.DfsdmClockSelection = RCC_DFSDM1CLKSOURCE_APB2|RCC_DFSDM2CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 344;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 7;

    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);
  }

  if(hAudioIn.InputDevice != INPUT_DEVICE_ANALOG_MIC)
  {
    /* I2S_APB1 selected as DFSDM audio clock source */
    __HAL_RCC_DFSDM1AUDIO_CONFIG(RCC_DFSDM1AUDIOCLKSOURCE_I2SAPB1);
    /* I2S_APB1 selected as DFSDM audio clock source */
    __HAL_RCC_DFSDM2AUDIO_CONFIG(RCC_DFSDM2AUDIOCLKSOURCE_I2SAPB1);
  }

  return AUDIO_OK;
}

/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  uint32_t index, input_device = 0;
  
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC1_FILTER)
  {
    DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = 1;
    input_device = INPUT_DEVICE_DIGITAL_MIC1;    
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC2_FILTER)
  {
    DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = 1;
    input_device = INPUT_DEVICE_DIGITAL_MIC2; 
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC3_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC3; 
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC4_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC4; 
  }  
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC5_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC5;    
  }
  
  if(hAudioIn.MultiBuffMode == 1)
  {
    BSP_AUDIO_IN_TransferComplete_CallBackEx(input_device);
  }
  else
  {
    if((DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] == 1) && (DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] == 1))
    {
      if(AppBuffTrigger >= hAudioIn.RecSize)
        AppBuffTrigger = 0;
      
      for(index = (ScratchSize/2) ; index < ScratchSize; index++)
      {
        hAudioIn.pRecBuf[AppBuffTrigger]     = (uint16_t)(SaturaLH((pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)][index] >> 8), -32760, 32760));
        hAudioIn.pRecBuf[AppBuffTrigger + 1] = (uint16_t)(SaturaLH((pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)][index] >> 8), -32760, 32760));
        AppBuffTrigger += 2;
      }
      DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = DmaRecBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = 0;
    }
    
    /* Update Trigger with Remaining Byte before callback if necessary */
    if(AppBuffTrigger >= hAudioIn.RecSize)
    {
      /* Reset Application Buffer Trigger */
      AppBuffTrigger = 0;
      AppBuffHalf = 0;
      
      /* Call the record update function to get the next buffer to fill and its size (size is ignored) */
      BSP_AUDIO_IN_TransferComplete_CallBack();
    }
    else if((AppBuffTrigger >= hAudioIn.RecSize/2))
    {
      if(AppBuffHalf == 0)
      {
        AppBuffHalf = 1;
        /* Manage the remaining file size and new address offset: This function
        should be coded by user (its prototype is already declared in stm32l476g_eval_audio.h) */
        BSP_AUDIO_IN_HalfTransfer_CallBack();
      }
    }
  }
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hdfsdm_filter : DFSDM filter handle.
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  uint32_t index, input_device = 0;
  
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC1_FILTER)
  {
    DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = 1;
    input_device = INPUT_DEVICE_DIGITAL_MIC1;    
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC2_FILTER)
  {
    DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = 1;
    input_device = INPUT_DEVICE_DIGITAL_MIC2; 
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC3_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC3; 
  }
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC4_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC4; 
  }  
  else if(hdfsdm_filter->Instance == AUDIO_DFSDMx_MIC5_FILTER)
  {
    input_device = INPUT_DEVICE_DIGITAL_MIC5;    
  }
  
  if(hAudioIn.MultiBuffMode == 1)
  {
    BSP_AUDIO_IN_HalfTransfer_CallBackEx(input_device);
  }
  else
  { 
    if((DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] == 1) && (DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] == 1))
    {  
      if(AppBuffTrigger >= hAudioIn.RecSize)
        AppBuffTrigger = 0;
      
      for(index = 0; index < ScratchSize/2; index++)
      {
        hAudioIn.pRecBuf[AppBuffTrigger]     = (int16_t)(SaturaLH((pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)][index] >> 8), -32760, 32760));
        hAudioIn.pRecBuf[AppBuffTrigger + 1] = (int16_t)(SaturaLH((pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)][index] >> 8), -32760, 32760));
        AppBuffTrigger += 2;
      }
      DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] = DmaRecHalfBuffCplt[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] = 0;
    }
    
    
    /* Update Trigger with Remaining Byte before callback if necessary */
    if(AppBuffTrigger >= hAudioIn.RecSize)
    {
      /* Reset Application Buffer Trigger */
      AppBuffTrigger = 0;
      AppBuffHalf = 0;
      
      /* Call the record update function to get the next buffer to fill and its size (size is ignored) */
      BSP_AUDIO_IN_TransferComplete_CallBack();
    }
    else if((AppBuffTrigger >= hAudioIn.RecSize/2))
    {
      if(AppBuffHalf == 0)
      {
        AppBuffHalf = 1;
        /* Manage the remaining file size and new address offset: This function
        should be coded by user */
        BSP_AUDIO_IN_HalfTransfer_CallBack();
      }
    }
  }
}

/**
  * @brief  Half reception complete callback.
  * @param  hi2s : I2S handle.
  */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the remaining file size and new address offset: This function 
     should be coded by user (its prototype is already declared in stm32746g_discovery_audio.h) */
  BSP_AUDIO_IN_HalfTransfer_CallBack();
}

/**
  * @brief  Reception complete callback.
  * @param  hi2s : I2S handle.
  */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Call the record update function to get the next buffer to fill and its size (size is ignored) */
  BSP_AUDIO_IN_TransferComplete_CallBack();
}

/**
  * @brief  Stops audio recording.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_Stop(void)
{  
  AppBuffTrigger = 0;
  AppBuffHalf    = 0;
  
  if (hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    /* Call the Media layer stop function */
    if(HAL_OK != HAL_I2S_DMAStop(&haudio_i2s))
    {
      return AUDIO_ERROR;
    }
    /* Call Audio Codec Stop function */
    if(audio_drv->Stop(AUDIO_I2C_ADDRESS, CODEC_PDWN_HW) != 0)
    {
      return AUDIO_ERROR;
    }
    /* Wait at least 100us */
    HAL_Delay(1);
  }
  else /* InputDevice = Digital Mic */
  {   
    /* Call the Media layer stop function for MIC1 channel */
    if(AUDIO_OK != BSP_AUDIO_IN_PauseEx(INPUT_DEVICE_DIGITAL_MIC1))
    {
      return AUDIO_ERROR;
    }
    
    /* Call the Media layer stop function for MIC2 channel */
    if(AUDIO_OK != BSP_AUDIO_IN_PauseEx(INPUT_DEVICE_DIGITAL_MIC2))
    {
      return AUDIO_ERROR;
    } 
  }
  
  /* Return AUDIO_OK when all operations are correctly done */  
  return AUDIO_OK;
}

/**
  * @brief  Stops audio recording.                 
  * @param  InputDevice: Microphone to be stopped. Can be INPUT_DEVICE_DIGITAL_MIC1 .. INPUT_DEVICE_DIGITAL_MIC5. 
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_StopEx(uint32_t InputDevice)
{
  if((InputDevice < INPUT_DEVICE_DIGITAL_MIC1) || (InputDevice > INPUT_DEVICE_DIGITAL_MIC5))
  {
    return AUDIO_ERROR;
  }
  else
  {
    BSP_AUDIO_IN_PauseEx(InputDevice);      
  }
  
  /* Return AUDIO_OK when all operations are correctly done */  
  return AUDIO_OK;
}

/**
  * @brief  Pauses the audio file stream.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_Pause(void)
{
  if (hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer stop function */
    if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)]))
    {
      return AUDIO_ERROR;
    }
    
    /* Call the Media layer stop function */
    if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)]))
    {
      return AUDIO_ERROR;
    }
  }
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Pauses the audio file stream.
  * @param  InputDevice: Microphone to be paused. Can be INPUT_DEVICE_DIGITAL_MIC1 .. INPUT_DEVICE_DIGITAL_MIC5.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_PauseEx(uint32_t InputDevice)
{
  if((InputDevice < INPUT_DEVICE_DIGITAL_MIC1) || (InputDevice > INPUT_DEVICE_DIGITAL_MIC5))
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer stop function */
    if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&hAudioInDfsdmFilter[POS_VAL(InputDevice)]))
    {
      return AUDIO_ERROR;
    }    
  }
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Resumes the audio file stream.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_Resume(void)
{ 
  if (hAudioIn.InputDevice == INPUT_DEVICE_ANALOG_MIC)
  {
    return AUDIO_ERROR;
  }
  else
  {  
    /* Call the Media layer start function for MIC2 channel */
    if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)], pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)], ScratchSize))
    {
      return AUDIO_ERROR;
    }
    
    /* Call the Media layer start function for MIC1 channel */
    if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)], pScratchBuff[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)], ScratchSize))
    {
      return AUDIO_ERROR;
    }
  } 
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Resumes the audio file stream.
  * @param  pBuf: Main buffer pointer for the recorded data storing
  * @param  InputDevice: Microphone to be paused. Can be INPUT_DEVICE_DIGITAL_MIC1 .. INPUT_DEVICE_DIGITAL_MIC5.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_ResumeEx(uint32_t *pBuf, uint32_t InputDevice)
{
  if((InputDevice < INPUT_DEVICE_DIGITAL_MIC1) || (InputDevice > INPUT_DEVICE_DIGITAL_MIC5))
  {
    return AUDIO_ERROR;
  }
  else
  {  
    /* Call the Media layer stop function */
    if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hAudioInDfsdmFilter[POS_VAL(InputDevice)], (int32_t*)pBuf[MicBuff[POS_VAL(InputDevice)]], hAudioIn.RecSize))
    {
      return AUDIO_ERROR;
    }
  }
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  Controls the audio in volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_IN_SetVolume(uint8_t Volume)
{
  /* Set the Global variable AudioInVolume  */
  AudioInVolume = Volume; 
  
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
  * @brief  User callback when record buffer is filled.
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  User callback when record buffer is filled.
  * @param InputDevice: INPUT_DEVICE_DIGITAL_MIC1 .. INPUT_DEVICE_DIGITAL_MIC5.
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBackEx(uint32_t InputDevice)
{
  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  User callback when record buffer is filled.
  * @param InputDevice: INPUT_DEVICE_DIGITAL_MIC1 .. INPUT_DEVICE_DIGITAL_MIC5.
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBackEx(uint32_t InputDevice)
{
  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Audio IN Error callback function.
  */
__weak void BSP_AUDIO_IN_Error_Callback(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
}

/**
  * @}
  */

/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  De-initializes the Digital Filter for Sigma-Delta Modulators interface (DFSDM).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
static uint8_t DFSDMx_DeInit(void)
{
  for(uint32_t i = 0; i < DFSDM_MIC_NUMBER; i++)
  {
    if(hAudioInDfsdmFilter[i].Instance != NULL)
    {
      if(HAL_OK != HAL_DFSDM_FilterDeInit(&hAudioInDfsdmFilter[i]))
      {
        return AUDIO_ERROR;
      }
      hAudioInDfsdmFilter[i].Instance = NULL;
    }
    if(hAudioInDfsdmChannel[i].Instance != NULL)
    {
      if(HAL_OK != HAL_DFSDM_ChannelDeInit(&hAudioInDfsdmChannel[i]))
      {
        return AUDIO_ERROR;
      }
      hAudioInDfsdmChannel[i].Instance = NULL;
    }
  }
  return AUDIO_OK;
}

/**
  * @brief  Initializes the DFSDM channel MSP.
  */
static void DFSDMx_ChannelMspInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  
  if((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1)
  {
    /* Enable DFSDM clock */
    AUDIO_DFSDMx_MIC1_CLK_ENABLE();
    /* Enable GPIO clock */
    AUDIO_DFSDMx_MIC1_CKOUT_DMIC_GPIO_CLK_ENABLE();
    
    /* DFSDM MIC1 pins configuration: DFSDM_CKOUT, DMIC_DATIN pins -------------*/
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC1_CKOUT_PIN;
    GPIO_InitStruct.Alternate = AUDIO_DFSDMx_MIC1_CKOUT_DMIC_AF;
    HAL_GPIO_Init(AUDIO_DFSDMx_MIC1_CKOUT_DMIC_GPIO_PORT, &GPIO_InitStruct);
    
    AUDIO_DFSDMx_MIC1_DMIC_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC1_DMIC_PIN;
    GPIO_InitStruct.Alternate = AUDIO_DFSDMx_MIC1_DMIC_AF;
    HAL_GPIO_Init(AUDIO_DFSDMx_MIC1_DMIC_GPIO_PORT, &GPIO_InitStruct); 
  }
  
  if(hAudioIn.InputDevice > INPUT_DEVICE_DIGITAL_MIC1)
  {
    /* Enable DFSDM clock */
    AUDIO_DFSDMx_MIC2_5_CLK_ENABLE(); 
    /* Enable GPIO clock */
    AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_GPIO_CLK_ENABLE(); 
    
    /* DFSDM MIC2 pins configuration: DFSDM_CKOUT, DMIC_DATIN pins -------------*/
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC2_5_CKOUT_PIN;
    GPIO_InitStruct.Alternate = AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_AF;
    HAL_GPIO_Init(AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_GPIO_PORT, &GPIO_InitStruct);  
    
    if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC2) == INPUT_DEVICE_DIGITAL_MIC2) ||\
      ((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC3) == INPUT_DEVICE_DIGITAL_MIC3))
    {  
      AUDIO_DFSDMx_MIC23_DMIC_GPIO_CLK_ENABLE();
      GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC23_DMIC_PIN;
      GPIO_InitStruct.Alternate = AUDIO_DFSDMx_MIC23_DMIC_AF;
      HAL_GPIO_Init(AUDIO_DFSDMx_MIC23_DMIC_GPIO_PORT, &GPIO_InitStruct);
    }
    
    if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC4) == INPUT_DEVICE_DIGITAL_MIC4) ||\
      ((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC5) == INPUT_DEVICE_DIGITAL_MIC5))
    {
      
      AUDIO_DFSDMx_MIC45_DMIC_GPIO_CLK_ENABLE();
      GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC45_DMIC_PIN;
      GPIO_InitStruct.Alternate = AUDIO_DFSDMx_MIC45_DMIC_AF;
      HAL_GPIO_Init(AUDIO_DFSDMx_MIC45_DMIC_GPIO_PORT, &GPIO_InitStruct);  
    }
  }
}

/**
  * @brief  DeInitializes the DFSDM channel MSP.
  */
static void DFSDMx_ChannelMspDeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  if((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1)
  {  
    /* DFSDM MIC1 pins configuration: DFSDM_CKOUT, DMIC_DATIN pins -------------*/
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC1_CKOUT_PIN;
    HAL_GPIO_DeInit(AUDIO_DFSDMx_MIC1_CKOUT_DMIC_GPIO_PORT, GPIO_InitStruct.Pin);
    
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC1_DMIC_PIN;
    HAL_GPIO_DeInit(AUDIO_DFSDMx_MIC1_DMIC_GPIO_PORT, GPIO_InitStruct.Pin); 
  }
  
  if(hAudioIn.InputDevice > INPUT_DEVICE_DIGITAL_MIC1)
  {
    /* DFSDM MIC2, MIC3, MIC4 and MIC5 pins configuration: DFSDM_CKOUT pin -----*/
    GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC2_5_CKOUT_PIN;
    HAL_GPIO_DeInit(AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_GPIO_PORT, GPIO_InitStruct.Pin); 
    
    if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC2) == INPUT_DEVICE_DIGITAL_MIC2) ||\
      ((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC3) == INPUT_DEVICE_DIGITAL_MIC3))
    {   
      /* DFSDM MIC2, MIC3 pins configuration:  DMIC_DATIN pin -----*/
      GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC23_DMIC_PIN;
      HAL_GPIO_DeInit(AUDIO_DFSDMx_MIC23_DMIC_GPIO_PORT, GPIO_InitStruct.Pin);
    }
    
    if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC4) == INPUT_DEVICE_DIGITAL_MIC4) ||\
      ((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC5) == INPUT_DEVICE_DIGITAL_MIC5))
    {
      /* DFSDM MIC4, MIC5 pins configuration: DMIC_DATIN pin -----*/   
      GPIO_InitStruct.Pin = AUDIO_DFSDMx_MIC45_DMIC_PIN;
      HAL_GPIO_DeInit(AUDIO_DFSDMx_MIC45_DMIC_GPIO_PORT, GPIO_InitStruct.Pin);
    } 
  }
}

/**
  * @brief  Initializes the DFSDM filter MSP.
  */
static void DFSDMx_FilterMspInit(void)
{
  uint32_t i = 0, mic_num = 0, mic_init[DFSDM_MIC_NUMBER] = {0};
  IRQn_Type AUDIO_DFSDM_DMAx_MIC_IRQHandler[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_DMAx_MIC1_IRQ, AUDIO_DFSDMx_DMAx_MIC2_IRQ, AUDIO_DFSDMx_DMAx_MIC3_IRQ, AUDIO_DFSDMx_DMAx_MIC4_IRQ, AUDIO_DFSDMx_DMAx_MIC5_IRQ};
  DMA_Stream_TypeDef* AUDIO_DFSDMx_DMAx_MIC_STREAM[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_DMAx_MIC1_STREAM, AUDIO_DFSDMx_DMAx_MIC2_STREAM, AUDIO_DFSDMx_DMAx_MIC3_STREAM, AUDIO_DFSDMx_DMAx_MIC4_STREAM, AUDIO_DFSDMx_DMAx_MIC5_STREAM};
  uint32_t AUDIO_DFSDMx_DMAx_MIC_CHANNEL[DFSDM_MIC_NUMBER] = {AUDIO_DFSDMx_DMAx_MIC1_CHANNEL, AUDIO_DFSDMx_DMAx_MIC2_CHANNEL, AUDIO_DFSDMx_DMAx_MIC3_CHANNEL, AUDIO_DFSDMx_DMAx_MIC4_CHANNEL, AUDIO_DFSDMx_DMAx_MIC5_CHANNEL};
  
  /* Enable the DMA clock */
  AUDIO_DFSDMx_DMAx_CLK_ENABLE();
  
  for(i = 0; i < hAudioIn.ChannelNbr; i++)
  {
    if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC1) == INPUT_DEVICE_DIGITAL_MIC1) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC1)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC1);
      mic_init[mic_num] = 1;
    }
    else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC2) == INPUT_DEVICE_DIGITAL_MIC2) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC2)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC2);
      mic_init[mic_num] = 1;
    }  
    else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC3) == INPUT_DEVICE_DIGITAL_MIC3) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC3)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC3);
      mic_init[mic_num] = 1;
    }
    else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC4) == INPUT_DEVICE_DIGITAL_MIC4) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC4)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC4);
      mic_init[mic_num] = 1;
    }
    else if(((hAudioIn.InputDevice & INPUT_DEVICE_DIGITAL_MIC5) == INPUT_DEVICE_DIGITAL_MIC5) && (mic_init[POS_VAL(INPUT_DEVICE_DIGITAL_MIC5)] != 1))
    {
      mic_num = POS_VAL(INPUT_DEVICE_DIGITAL_MIC5);
      mic_init[mic_num] = 1;
    } 
    
    /* Configure the hDmaDfsdm[i] handle parameters */    
    hDmaDfsdm[mic_num].Init.Channel             = AUDIO_DFSDMx_DMAx_MIC_CHANNEL[mic_num]; 
    hDmaDfsdm[mic_num].Instance                 = AUDIO_DFSDMx_DMAx_MIC_STREAM[mic_num];
    hDmaDfsdm[mic_num].Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hDmaDfsdm[mic_num].Init.PeriphInc           = DMA_PINC_DISABLE;
    hDmaDfsdm[mic_num].Init.MemInc              = DMA_MINC_ENABLE;
    hDmaDfsdm[mic_num].Init.PeriphDataAlignment = AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE;
    hDmaDfsdm[mic_num].Init.MemDataAlignment    = AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE;
    hDmaDfsdm[mic_num].Init.Mode                = DMA_CIRCULAR;
    hDmaDfsdm[mic_num].Init.Priority            = DMA_PRIORITY_HIGH;
    hDmaDfsdm[mic_num].Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hDmaDfsdm[mic_num].Init.MemBurst            = DMA_MBURST_SINGLE;
    hDmaDfsdm[mic_num].Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hDmaDfsdm[mic_num].State                    = HAL_DMA_STATE_RESET;  
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(&hAudioInDfsdmFilter[mic_num], hdmaReg, hDmaDfsdm[mic_num]);
    
    /* Reset DMA handle state */
    __HAL_DMA_RESET_HANDLE_STATE(&hDmaDfsdm[mic_num]);
    
    /* Configure the DMA Channel */
    HAL_DMA_Init(&hDmaDfsdm[mic_num]);
    
    /* DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_DFSDM_DMAx_MIC_IRQHandler[mic_num], AUDIO_IN_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_DFSDM_DMAx_MIC_IRQHandler[mic_num]);
  } 
}

/**
  * @brief  DeInitializes the DFSDM filter MSP.
  */
static void DFSDMx_FilterMspDeInit(void)
{
  /* Configure the DMA Channel */
  for(uint32_t i = 0; i < DFSDM_MIC_NUMBER; i++)
  {
    if(hDmaDfsdm[i].Instance != NULL)
    {
      HAL_DMA_DeInit(&hDmaDfsdm[i]); 
    }
  }  
}

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @note   This function assumes that the I2S input clock
  *         is already configured and ready to be used.
  * @param  AudioFreq: Audio frequency to be configured for the I2S peripheral.
  */
static void I2Sx_In_Init(uint32_t AudioFreq)
{
  /* Initialize the hAudioInI2s and haudio_in_i2sext Instance parameters */
  haudio_i2s.Instance = AUDIO_IN_I2Sx;
  haudio_in_i2sext.Instance = I2S3ext;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_i2s);
  __HAL_I2S_DISABLE(&haudio_in_i2sext);

  /* I2S peripheral configuration */
  haudio_i2s.Init.AudioFreq = AudioFreq;
  haudio_i2s.Init.ClockSource = I2S_CLOCK_PLL;
  haudio_i2s.Init.CPOL = I2S_CPOL_LOW;
  haudio_i2s.Init.DataFormat = I2S_DATAFORMAT_16B;
  haudio_i2s.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  haudio_i2s.Init.Mode = I2S_MODE_MASTER_TX;
  haudio_i2s.Init.Standard = I2S_STANDARD_PHILIPS;
  haudio_i2s.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
  /* Init the I2S */
  HAL_I2S_Init(&haudio_i2s);

  /* I2Sext peripheral configuration */
  haudio_in_i2sext.Init.AudioFreq = AudioFreq;
  haudio_in_i2sext.Init.ClockSource = I2S_CLOCK_PLL;
  haudio_in_i2sext.Init.CPOL = I2S_CPOL_HIGH;
  haudio_in_i2sext.Init.DataFormat = I2S_DATAFORMAT_16B;
  haudio_in_i2sext.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  haudio_in_i2sext.Init.Mode = I2S_MODE_SLAVE_RX;
  haudio_in_i2sext.Init.Standard = I2S_STANDARD_PHILIPS;

  /* Init the I2Sext */
  HAL_I2S_Init(&haudio_in_i2sext);

 /* Enable I2S block */
  __HAL_I2S_ENABLE(&haudio_i2s);
  __HAL_I2S_ENABLE(&haudio_in_i2sext);
}

/**
  * @brief  Deinitializes the Audio Codec audio interface (I2S).
  */
static void I2Sx_In_DeInit(void)
{
  /* Initialize the hAudioInI2s Instance parameter */
  haudio_i2s.Instance = AUDIO_IN_I2Sx;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_i2s);

  /* DeInit the I2S */
  HAL_I2S_DeInit(&haudio_i2s);

  /* Initialize the hAudioInI2s Instance parameter */
  haudio_in_i2sext.Instance = I2S3ext;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_in_i2sext);

  /* DeInit the I2S */
  HAL_I2S_DeInit(&haudio_in_i2sext);
}

/**
  * @}
  */ 
  
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
