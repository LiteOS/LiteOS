/**
  ******************************************************************************
  * @file    stm32f413h_discovery_audio.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32f413h_discovery_audio.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F413H_DISCOVERY_AUDIO_H
#define __STM32F413H_DISCOVERY_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include audio component Driver */
#include "../Components/wm8994/wm8994.h"
#include "stm32f413h_discovery.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32F413H_DISCOVERY_AUDIO
  * @{
  */

/** @defgroup STM32F413H_DISCOVERY_AUDIO_Exported_Types STM32F413H DISCOVERY Audio Exported Types
  * @{
  */
  
typedef struct
{
  uint32_t               Frequency;      /* Record Frequency */
  uint32_t               BitResolution;  /* Record bit resolution */
  uint32_t               ChannelNbr;     /* Record Channel Number */
  uint16_t               *pRecBuf;       /* Pointer to record user buffer */
  uint32_t               RecSize;        /* Size to record in mono, double size to record in stereo */
  uint32_t               InputDevice;    /* Audio Input Device */
  uint32_t               MultiBuffMode;  /* Multi buffer mode selection */
}AUDIOIN_ContextTypeDef;
   
/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_Exported_Constants  STM32F413H DISCOVERY Audio Exported Constants
  * @{
  */

/*------------------------------------------------------------------------------
                        AUDIO OUT CONFIGURATION
------------------------------------------------------------------------------*/
/* SPI Configuration defines */
#define AUDIO_OUT_I2Sx                           SPI2
#define AUDIO_OUT_I2Sx_CLK_ENABLE()              __HAL_RCC_SPI2_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_CLK_DISABLE()             __HAL_RCC_SPI2_CLK_DISABLE()

#define AUDIO_OUT_I2Sx_MCK_PIN                   GPIO_PIN_3
#define AUDIO_OUT_I2Sx_MCK_GPIO_PORT             GPIOA
#define AUDIO_OUT_I2Sx_MCK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_MCK_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_MCK_AF                    GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_SCK_PIN                   GPIO_PIN_3
#define AUDIO_OUT_I2Sx_SCK_GPIO_PORT             GPIOD
#define AUDIO_OUT_I2Sx_SCK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_SCK_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_SCK_AF                    GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_WS_PIN                    GPIO_PIN_9
#define AUDIO_OUT_I2Sx_WS_GPIO_PORT              GPIOB
#define AUDIO_OUT_I2Sx_WS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_WS_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_WS_AF                     GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_SD_PIN                    GPIO_PIN_3
#define AUDIO_OUT_I2Sx_SD_GPIO_PORT              GPIOC
#define AUDIO_OUT_I2Sx_SD_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_SD_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_SD_AF                     GPIO_AF5_SPI2

/* I2S DMA Stream Tx definitions */
#define AUDIO_OUT_I2Sx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA1_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_DMAx_CLK_DISABLE()        __HAL_RCC_DMA1_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_DMAx_STREAM               DMA1_Stream4
#define AUDIO_OUT_I2Sx_DMAx_CHANNEL              DMA_CHANNEL_0
#define AUDIO_OUT_I2Sx_DMAx_IRQ                  DMA1_Stream4_IRQn
#define AUDIO_OUT_I2Sx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_I2Sx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define DMA_MAX_SIZE                             0xFFFF
   
#define AUDIO_OUT_I2Sx_DMAx_IRQHandler           DMA1_Stream4_IRQHandler
  
   /* Select the interrupt preemption priority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO                    ((uint32_t)0x0E)   /* Select the preemption priority level(0 is the highest) */


/*------------------------------------------------------------------------------
                        AUDIO IN CONFIGURATION
------------------------------------------------------------------------------*/
/* DFSDM Configuration defines */
#define AUDIO_DFSDMx_MIC1_CHANNEL                      DFSDM1_Channel1    /* MP34DT01TR U1 microphone on PCB top side */
#define AUDIO_DFSDMx_MIC2_CHANNEL                      DFSDM2_Channel1    /* MP34DT01TR U2 microphone on PCB top side */
#define AUDIO_DFSDMx_MIC3_CHANNEL                      DFSDM2_Channel0    /* MP34DT01TR U3 microphone on PCB top side */
#define AUDIO_DFSDMx_MIC4_CHANNEL                      DFSDM2_Channel7    /* MP34DT01TR U4 microphone on PCB top side */
#define AUDIO_DFSDMx_MIC5_CHANNEL                      DFSDM2_Channel6    /* MP34DT01TR U5 microphone on PCB top side */

#define AUDIO_DFSDMx_MIC1_CHANNEL_FOR_FILTER           DFSDM_CHANNEL_1
#define AUDIO_DFSDMx_MIC2_CHANNEL_FOR_FILTER           DFSDM_CHANNEL_1
#define AUDIO_DFSDMx_MIC3_CHANNEL_FOR_FILTER           DFSDM_CHANNEL_0
#define AUDIO_DFSDMx_MIC4_CHANNEL_FOR_FILTER           DFSDM_CHANNEL_7
#define AUDIO_DFSDMx_MIC5_CHANNEL_FOR_FILTER           DFSDM_CHANNEL_6

#define AUDIO_DFSDMx_MIC1_FILTER                       DFSDM1_Filter0     /* Common MIC1 filter for MP34DT01TR (U1) microphone input */
#define AUDIO_DFSDMx_MIC2_FILTER                       DFSDM2_Filter0     /* Common MIC2 filter for MP34DT01TR(U2) microphone input */
#define AUDIO_DFSDMx_MIC3_FILTER                       DFSDM2_Filter1     /* Common MIC2 filter for MP34DT01TR(U3) microphone input */
#define AUDIO_DFSDMx_MIC4_FILTER                       DFSDM2_Filter2     /* Common MIC2 filter for MP34DT01TR(U4) microphone input */
#define AUDIO_DFSDMx_MIC5_FILTER                       DFSDM2_Filter3     /* Common MIC2 filter for MP34DT01TR(U5) microphone input */

#define AUDIO_DFSDMx_MIC2_5_CLK_ENABLE()               __HAL_RCC_DFSDM2_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC1_CLK_ENABLE()                 __HAL_RCC_DFSDM1_CLK_ENABLE()
   
#define AUDIO_DFSDMx_MIC1_CKOUT_PIN                          GPIO_PIN_8
#define AUDIO_DFSDMx_MIC1_CKOUT_DMIC_GPIO_PORT               GPIOA
#define AUDIO_DFSDMx_MIC1_CKOUT_DMIC_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC1_CKOUT_DMIC_AF                      GPIO_AF6_DFSDM1
   
#define AUDIO_DFSDMx_MIC1_DMIC_PIN                           GPIO_PIN_6
#define AUDIO_DFSDMx_MIC1_DMIC_GPIO_PORT                     GPIOD
#define AUDIO_DFSDMx_MIC1_DMIC_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC1_DMIC_AF                            GPIO_AF6_DFSDM1
   
#define AUDIO_DFSDMx_MIC2_5_CKOUT_PIN                        GPIO_PIN_2
#define AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_GPIO_PORT             GPIOD
#define AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC2_5_CKOUT_DMIC_AF                    GPIO_AF3_DFSDM2
   
#define AUDIO_DFSDMx_MIC23_DMIC_PIN                           GPIO_PIN_7
#define AUDIO_DFSDMx_MIC23_DMIC_GPIO_PORT                     GPIOA
#define AUDIO_DFSDMx_MIC23_DMIC_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC23_DMIC_AF                            GPIO_AF7_DFSDM2  
   
#define AUDIO_DFSDMx_MIC45_DMIC_PIN                           GPIO_PIN_7
#define AUDIO_DFSDMx_MIC45_DMIC_GPIO_PORT                     GPIOB
#define AUDIO_DFSDMx_MIC45_DMIC_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_DFSDMx_MIC45_DMIC_AF                            GPIO_AF6_DFSDM2 

/* DFSDM DMA Right and Left channels definitions */
#define AUDIO_DFSDMx_DMAx_CLK_ENABLE()                 __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_DFSDMx_DMAx_MIC1_STREAM                  DMA2_Stream6
#define AUDIO_DFSDMx_DMAx_MIC1_CHANNEL                 DMA_CHANNEL_3
#define AUDIO_DFSDMx_DMAx_MIC2_STREAM                  DMA2_Stream0
#define AUDIO_DFSDMx_DMAx_MIC2_CHANNEL                 DMA_CHANNEL_8
#define AUDIO_DFSDMx_DMAx_MIC3_STREAM                  DMA2_Stream1
#define AUDIO_DFSDMx_DMAx_MIC3_CHANNEL                 DMA_CHANNEL_8
#define AUDIO_DFSDMx_DMAx_MIC4_STREAM                  DMA2_Stream2
#define AUDIO_DFSDMx_DMAx_MIC4_CHANNEL                 DMA_CHANNEL_8
#define AUDIO_DFSDMx_DMAx_MIC5_STREAM                  DMA2_Stream7
#define AUDIO_DFSDMx_DMAx_MIC5_CHANNEL                 DMA_CHANNEL_8
#define AUDIO_DFSDMx_DMAx_MIC1_IRQ                     DMA2_Stream6_IRQn
#define AUDIO_DFSDMx_DMAx_MIC2_IRQ                     DMA2_Stream0_IRQn
#define AUDIO_DFSDMx_DMAx_MIC3_IRQ                     DMA2_Stream1_IRQn
#define AUDIO_DFSDMx_DMAx_MIC4_IRQ                     DMA2_Stream2_IRQn
#define AUDIO_DFSDMx_DMAx_MIC5_IRQ                     DMA2_Stream7_IRQn

#define AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE             DMA_PDATAALIGN_WORD
#define AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE                DMA_MDATAALIGN_WORD
   
#define AUDIO_DFSDM_DMAx_MIC1_IRQHandler               DMA2_Stream6_IRQHandler
#define AUDIO_DFSDM_DMAx_MIC2_IRQHandler               DMA2_Stream0_IRQHandler
#define AUDIO_DFSDM_DMAx_MIC3_IRQHandler               DMA2_Stream1_IRQHandler
#define AUDIO_DFSDM_DMAx_MIC4_IRQHandler               DMA2_Stream2_IRQHandler
#define AUDIO_DFSDM_DMAx_MIC5_IRQHandler               DMA2_Stream7_IRQHandler


/* Select the interrupt preemption priority and subpriority for the IT/DMA interrupt */
#define AUDIO_IN_IRQ_PREPRIO                0x0F   /* Select the preemption priority level(0 is the highest) */

/* HW defines for Analog mic configuration */
#define AUDIO_IN_I2Sx                           SPI3
#define AUDIO_IN_I2Sx_CLK_ENABLE()              __HAL_RCC_SPI3_CLK_ENABLE()
#define AUDIO_IN_I2Sx_CLK_DISABLE()             __HAL_RCC_SPI3_CLK_DISABLE()

#define AUDIO_IN_I2Sx_EXT_SD_PIN                GPIO_PIN_4
#define AUDIO_IN_I2Sx_EXT_SD_GPIO_PORT          GPIOB
#define AUDIO_IN_I2Sx_EXT_SD_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_IN_I2Sx_EXT_SD_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_IN_I2Sx_EXT_SD_AF                 GPIO_AF7_SPI3

#define AUDIO_IN_CODEC_INT_PIN                  GPIO_PIN_2
#define AUDIO_IN_CODEC_INT_GPIO_PORT            GPIOG
#define AUDIO_IN_CODEC_INT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_IN_CODEC_INT_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOG_CLK_DISABLE()
#define AUDIO_IN_CODEC_INT_IRQ                  EXTI2_IRQn

/* I2S DMA Stream Rx definitions */
#define AUDIO_IN_I2Sx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA1_CLK_ENABLE()
#define AUDIO_IN_I2Sx_DMAx_CLK_DISABLE()        __HAL_RCC_DMA1_CLK_DISABLE()
#define AUDIO_IN_I2Sx_DMAx_STREAM               DMA1_Stream0
#define AUDIO_IN_I2Sx_DMAx_CHANNEL              DMA_CHANNEL_3
#define AUDIO_IN_I2Sx_DMAx_IRQ                  DMA1_Stream0_IRQn
#define AUDIO_IN_I2Sx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_IN_I2Sx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
   
#define AUDIO_IN_I2Sx_DMAx_IRQHandler           DMA1_Stream0_IRQHandler
#define AUDIO_IN_I2Sx_DMAx_IRQ                  DMA1_Stream0_IRQn

/*------------------------------------------------------------------------------
             CONFIGURATION: Audio Driver Configuration parameters
------------------------------------------------------------------------------*/

#define AUDIODATA_SIZE                      2   /* 16-bits audio data size */

/* Audio status definition */     
#define AUDIO_OK                            ((uint8_t)0)
#define AUDIO_ERROR                         ((uint8_t)1)
#define AUDIO_TIMEOUT                       ((uint8_t)2)

/* Audio out parameters */
#define DEFAULT_AUDIO_OUT_FREQ              I2S_AUDIOFREQ_48K
#define DEFAULT_AUDIO_OUT_BIT_RESOLUTION    ((uint8_t)16)
#define DEFAULT_AUDIO_OUT_CHANNEL_NBR       ((uint8_t)2) /* Mono = 1, Stereo = 2 */
#define DEFAULT_AUDIO_OUT_VOLUME            ((uint16_t)64)

/* Audio in parameters */
#define DEFAULT_AUDIO_IN_FREQ               I2S_AUDIOFREQ_16K
#define DEFAULT_AUDIO_IN_BIT_RESOLUTION     ((uint8_t)16)
#define DEFAULT_AUDIO_IN_CHANNEL_NBR        ((uint8_t)2) /* Mono = 1, Stereo = 2 */
#define DEFAULT_AUDIO_IN_VOLUME             ((uint16_t)64)
   
/*------------------------------------------------------------------------------
                            OUTPUT DEVICES definition
------------------------------------------------------------------------------*/

/* Alias on existing output devices to adapt for 2 headphones output */
#define OUTPUT_DEVICE_HEADPHONE1 OUTPUT_DEVICE_HEADPHONE
#define OUTPUT_DEVICE_HEADPHONE2 OUTPUT_DEVICE_SPEAKER /* Headphone2 is connected to Speaker output of the wm8994 */

/*------------------------------------------------------------------------------
                           INPUT DEVICES definition
------------------------------------------------------------------------------*/
/* Analog microphone input from 3.5 audio jack connector */    
#define INPUT_DEVICE_ANALOG_MIC        ((uint32_t)0x00000001)     
/* MP34DT01TR digital microphone on PCB top side */
#define INPUT_DEVICE_DIGITAL_MIC1      ((uint32_t)0x00000010)
#define INPUT_DEVICE_DIGITAL_MIC2      ((uint32_t)0x00000020)
#define INPUT_DEVICE_DIGITAL_MIC3      ((uint32_t)0x00000040)
#define INPUT_DEVICE_DIGITAL_MIC4      ((uint32_t)0x00000080)
#define INPUT_DEVICE_DIGITAL_MIC5      ((uint32_t)0x00000100)
#define INPUT_DEVICE_DIGITAL_MIC       ((uint32_t)(INPUT_DEVICE_DIGITAL_MIC1 | INPUT_DEVICE_DIGITAL_MIC2)) 
#define INPUT_DEVICE_DIGITAL_MIC_ALL   ((uint32_t)(INPUT_DEVICE_DIGITAL_MIC1 | INPUT_DEVICE_DIGITAL_MIC2 |\
                                                   INPUT_DEVICE_DIGITAL_MIC3 | INPUT_DEVICE_DIGITAL_MIC4 |\
                                                   INPUT_DEVICE_DIGITAL_MIC5)) 

#define DFSDM_MIC_NUMBER               (5)

/**
  * @}
  */
 
/** @defgroup STM32F413H_DISCOVERY_AUDIO_Exported_Variables  STM32F413H DISCOVERY Audio Exported Variables
  * @{
  */
extern __IO uint16_t AudioInVolume;
 /**
  * @}
  */
   
/** @defgroup STM32F413H_DISCOVERY_AUDIO_Exported_Macros  STM32F413H DISCOVERY Audio Exported Macros
  * @{
  */
#define DMA_MAX(x)           (((x) <= DMA_MAX_SIZE)? (x):DMA_MAX_SIZE)
#define POS_VAL(VAL)         (POSITION_VAL(VAL) - 4) 
/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_OUT_Exported_Functions  STM32F413H DISCOVERY Audio OUT Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size);
void    BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size);
uint8_t BSP_AUDIO_OUT_Pause(void);
uint8_t BSP_AUDIO_OUT_Resume(void);
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option);
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume);
void    BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq);
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd);
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output);
void    BSP_AUDIO_OUT_DeInit(void);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function is called when the requested data has been completely transferred.*/
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void);

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void  BSP_AUDIO_OUT_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t AudioFreq, void *Params);
void  BSP_AUDIO_OUT_MspInit(I2S_HandleTypeDef *hi2s, void *Params);
void  BSP_AUDIO_OUT_MspDeInit(I2S_HandleTypeDef *hi2s, void *Params);

/**
  * @}
  */ 

/** @defgroup STM32F413H_DISCOVERY_AUDIO_IN_Exported_Functions  STM32F413H DISCOVERY AudioIN Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t BSP_AUDIO_IN_InitEx(uint32_t InputDevice, uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t BSP_AUDIO_IN_ConfigMicDefault(uint32_t InputDevice);
uint8_t BSP_AUDIO_IN_ConfigDigitalMic(uint32_t InputDevice, void *Params);    
uint8_t BSP_AUDIO_IN_AllocScratch (int32_t *pScratch, uint32_t size);
uint8_t BSP_AUDIO_IN_Record(uint16_t *pBuf, uint32_t Size);
uint8_t BSP_AUDIO_IN_RecordEx(uint32_t *pBuf, uint32_t Size);
uint8_t BSP_AUDIO_IN_SetFrequency(uint32_t AudioFreq);
uint8_t BSP_AUDIO_IN_Stop(void);
uint8_t BSP_AUDIO_IN_StopEx(uint32_t InputDevice);
uint8_t BSP_AUDIO_IN_Pause(void);
uint8_t BSP_AUDIO_IN_PauseEx(uint32_t InputDevice);
uint8_t BSP_AUDIO_IN_Resume(void);
uint8_t BSP_AUDIO_IN_ResumeEx(uint32_t *pBuf, uint32_t InputDevice);
uint8_t BSP_AUDIO_IN_SetVolume(uint8_t Volume);
void    BSP_AUDIO_IN_DeInit(void);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void    BSP_AUDIO_IN_TransferComplete_CallBack(void);
void    BSP_AUDIO_IN_HalfTransfer_CallBack(void);
void    BSP_AUDIO_IN_TransferComplete_CallBackEx(uint32_t InputDevice);
void    BSP_AUDIO_IN_HalfTransfer_CallBackEx(uint32_t InputDevice);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_IN_Error_Callback(void);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
uint8_t BSP_AUDIO_IN_ClockConfig(uint32_t AudioFreq, void *Params);
void    BSP_AUDIO_IN_MspInit(void *Params);
void    BSP_AUDIO_IN_MspDeInit(void *Params);

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

#ifdef __cplusplus
}
#endif

#endif /* __STM32F413H_DISCOVERY_AUDIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
