/**
  ******************************************************************************
  * @file    adv7533.c
  * @author  MCD Application Team
  * @brief   This file provides the ADV7533 DSI to HDMI bridge driver 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "adv7533.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup Components
  * @{
  */ 

/** @defgroup ADV7533 ADV7533
  * @brief     This file provides a set of functions needed to drive the 
  *            adv7533 DSI-HDMI bridge.
  * @{
  */
    
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup ADV7533_Private_Constants ADV7533 Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup ADV7533_Exported_Variables
  * @{
  */

AUDIO_DrvTypeDef adv7533_drv = 
{
  adv7533_AudioInit,
  adv7533_DeInit,
  adv7533_ReadID,
  adv7533_Play,
  adv7533_Pause,
  adv7533_Resume,
  adv7533_Stop,  
  adv7533_SetFrequency,
  adv7533_SetVolume, /* Not supported, added for compatibility */
  adv7533_SetMute,  
  adv7533_SetOutputMode, /* Not supported, added for compatibility */ 
  adv7533_Reset /* Not supported, added for compatibility */
};

/**
  * @}
  */
   
/* Exported functions --------------------------------------------------------*/
/** @defgroup ADV7533_Exported_Functions ADV7533 Exported Functions
  * @{
  */

/**
  * @brief  Initializes the ADV7533 bridge.
  * @param  None
  * @retval Status
  */
uint8_t ADV7533_Init(void)
{
  HDMI_IO_Init();

  /* Configure the IC2 address for CEC_DSI interface */
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xE1, ADV7533_CEC_DSI_I2C_ADDR);

  return 0;
}

/**
  * @brief  Power on the ADV7533 bridge.
  * @param  None
  * @retval None
  */
void ADV7533_PowerOn(void)
{
  uint8_t tmp;
  
  /* Power on */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x41);
  tmp &= ~0x40;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x41, tmp);
}

/**
  * @brief  Power off the ADV7533 bridge.
  * @param  None
  * @retval None
  */
void ADV7533_PowerDown(void)
{
   uint8_t tmp;
   
   /* Power down */
   tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x41);
   tmp |= 0x40;
   HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x41, tmp);
}

/**
  * @brief  Configure the DSI-HDMI ADV7533 bridge for video.
  * @param config : pointer to adv7533ConfigTypeDef that contains the
  *                 video configuration parameters
  * @retval None
  */
void ADV7533_Configure(adv7533ConfigTypeDef * config)
{
  uint8_t tmp;
  
  /* Sequence from Section 3 - Quick Start Guide */
  
  /* ADV7533 Power Settings */
  /* Power down */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x41);
  tmp &= ~0x40;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x41, tmp);
  /* HPD Override */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0xD6);
  tmp |= 0x40;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xD6, tmp);
  /* Gate DSI LP Oscillator and DSI Bias Clock Powerdown */
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x03);
  tmp &= ~0x02;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x03, tmp);
  
  /* Fixed registers that must be set on power-up */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x16);
  tmp &= ~0x3E;
  tmp |= 0x20; 
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x16, tmp);
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x9A, 0xE0);
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0xBA);
  tmp &= ~0xF8;
  tmp |= 0x70; 
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xBA, tmp);
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xDE, 0x82);
  
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0xE4); 
  tmp |= 0x40;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xE4, tmp);
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xE5, 0x80);
  
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x15);
  tmp &= ~0x30;
  tmp |= 0x10; 
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x17);
  tmp &= ~0xF0;
  tmp |= 0xD0; 
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x17, tmp);
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x24);
  tmp &= ~0x10;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x24, tmp);
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x57);
  tmp |= 0x01;
  tmp |= 0x10;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x57, tmp);
  
  /* Configure the number of DSI lanes */
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x1C, (config->DSI_LANES << 4));
  
  /* Setup video output mode */
  /* Select HDMI mode */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0xAF);
  tmp |= 0x02;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0xAF, tmp); 
  /* HDMI Output Enable */
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x03);
  tmp |= 0x80;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x03, tmp);

  /* GC packet enable */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x40);
  tmp |= 0x80;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x40, tmp);
  /* Input color depth 24-bit per pixel */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x4C);
  tmp &= ~0x0F;
  tmp |= 0x03;
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x4C, tmp);
  /* Down dither output color depth */
  HDMI_IO_Write(ADV7533_MAIN_I2C_ADDR, 0x49, 0xfc);
  
  /* Internal timing disabled */
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x27);
  tmp &= ~0x80;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x27, tmp);
}

/**
  * @brief  Enable video pattern generation.
  * @param  None
  * @retval None
  */
void ADV7533_PatternEnable(void)
{
  /* Timing generator enable */
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x55, 0x80); /* Color bar */
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x55, 0xA0); /* Color ramp */
  
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x03, 0x89);
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0xAF, 0x16);
}

/**
  * @brief  Disable video pattern generation.
  * @param  none
  * @retval none
  */
void ADV7533_PatternDisable(void)
{
  /* Timing generator enable */
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x55, 0x00);
}

/**
  * @brief Initializes the ADV7533 audio  interface.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param OutputDevice: Not used (for compatiblity only).   
  * @param Volume: Not used (for compatiblity only).   
  * @param AudioFreq: Audio Frequency 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_AudioInit(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume,uint32_t AudioFreq)
{
  uint32_t val = 4096;
  uint8_t  tmp = 0;

  /* Audio data enable*/
  tmp = HDMI_IO_Read(ADV7533_CEC_DSI_I2C_ADDR, 0x05);
  tmp &= ~0x20;
  HDMI_IO_Write(ADV7533_CEC_DSI_I2C_ADDR, 0x05, tmp);

  /* HDMI statup */
  tmp= (uint8_t)((val & 0xF0000)>>16);
  HDMI_IO_Write(DeviceAddr, 0x01, tmp);

  tmp= (uint8_t)((val & 0xFF00)>>8);
  HDMI_IO_Write(DeviceAddr, 0x02, tmp);

  tmp= (uint8_t)((val & 0xFF));
  HDMI_IO_Write(DeviceAddr, 0x03, tmp);

  /* Enable spdif */
  tmp = HDMI_IO_Read(DeviceAddr, 0x0B);
  tmp |= 0x80;
  HDMI_IO_Write(DeviceAddr, 0x0B, tmp);

  /* Enable I2S */
  tmp = HDMI_IO_Read(DeviceAddr, 0x0C);
  tmp |=0x04;
  HDMI_IO_Write(DeviceAddr, 0x0C, tmp);

  /* Set audio sampling frequency */
  adv7533_SetFrequency(DeviceAddr, AudioFreq);

  /* Select SPDIF is 0x10 , I2S=0x00  */
  tmp = HDMI_IO_Read(ADV7533_MAIN_I2C_ADDR, 0x0A);
  tmp &=~ 0x10;
  HDMI_IO_Write(DeviceAddr, 0x0A, tmp);

  /* Set v1P2 enable */
  tmp = HDMI_IO_Read(DeviceAddr, 0xE4);
  tmp |= 0x80;
  HDMI_IO_Write(DeviceAddr, 0xE4, tmp);
 
  return 0;
}

/**
  * @brief  Deinitializes the adv7533
  * @param  None
  * @retval  None
  */
void adv7533_DeInit(void)
{
  /* Deinitialize Audio adv7533 interface */
  AUDIO_IO_DeInit();
}

/**
  * @brief  Get the adv7533 ID.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval The adv7533 ID 
  */
uint32_t adv7533_ReadID(uint16_t DeviceAddr)
{
  uint32_t  tmp = 0;
  
  tmp = HDMI_IO_Read(DeviceAddr, ADV7533_CHIPID_ADDR0);
  tmp = (tmp<<8);
  tmp |= HDMI_IO_Read(DeviceAddr, ADV7533_CHIPID_ADDR1);
  
  return(tmp);
}

/**
  * @brief Pauses playing on the audio hdmi
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_Pause(uint16_t DeviceAddr)
{ 
  return(adv7533_SetMute(DeviceAddr,AUDIO_MUTE_ON));
}       
            
/**
  * @brief Resumes playing on the audio hdmi.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */   
uint32_t adv7533_Resume(uint16_t DeviceAddr)
{ 
  return(adv7533_SetMute(DeviceAddr,AUDIO_MUTE_OFF));
} 

/**
  * @brief Start the audio hdmi play feature.
  * @note  For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.   
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_Play(uint16_t DeviceAddr ,uint16_t* pBuffer  ,uint16_t Size)
{
  return(adv7533_SetMute(DeviceAddr,AUDIO_MUTE_OFF));
}
            
/**
  * @brief Stop playing on the audio hdmi
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_Stop(uint16_t DeviceAddr,uint32_t cmd)
{ 
  return(adv7533_SetMute(DeviceAddr,AUDIO_MUTE_ON));
}               
            
/**
  * @brief Enables or disables the mute feature on the audio hdmi.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
  uint8_t tmp = 0;
  
  tmp = HDMI_IO_Read(DeviceAddr, 0x0D);
  if (Cmd == AUDIO_MUTE_ON)  
  {
    /* enable audio mute*/ 
    tmp |= 0x40;
    HDMI_IO_Write(DeviceAddr, 0x0D, tmp);
  }
  else
  {
    /*audio mute off disable the mute */
    tmp &= ~0x40;
    HDMI_IO_Write(DeviceAddr, 0x0D, tmp);
  }
  return 0;
}

/**
  * @brief Sets output mode.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output : hdmi output.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
  return 0;
}    
            
/**
  * @brief Sets volumee.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Volume : volume value.
  * @retval 0 if correct communication, else wrong communication
  */           
uint32_t adv7533_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
 return 0;
}
            
/**
  * @brief Resets adv7533 registers.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_Reset(uint16_t DeviceAddr)
{
  return 0;
}

/**
  * @brief Sets new frequency.
  * @param DeviceAddr: Device address on communication Bus.
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t adv7533_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq)
{
  uint8_t tmp = 0;

  tmp = HDMI_IO_Read(DeviceAddr, 0x15);
  tmp &= (~0xF0);
  /*  Clock Configurations */
  switch (AudioFreq)
  {
  case  AUDIO_FREQUENCY_32K:
    /* Sampling Frequency =32 KHZ*/
    tmp |= 0x30;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
  case  AUDIO_FREQUENCY_44K: 
    /* Sampling Frequency =44,1 KHZ*/
    tmp |= 0x00;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
    
  case  AUDIO_FREQUENCY_48K: 
    /* Sampling Frequency =48KHZ*/
    tmp |= 0x20;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
    
  case  AUDIO_FREQUENCY_96K: 
    /* Sampling Frequency =96 KHZ*/
    tmp |= 0xA0;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
    
  case  AUDIO_FREQUENCY_88K: 
    /* Sampling Frequency =88,2 KHZ*/
    tmp |= 0x80;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
    
  case  AUDIO_FREQUENCY_176K: 
    /* Sampling Frequency =176,4 KHZ*/
    tmp |= 0xC0;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
    
  case  AUDIO_FREQUENCY_192K: 
    /* Sampling Frequency =192KHZ*/
    tmp |= 0xE0;
    HDMI_IO_Write(DeviceAddr, 0x15, tmp);
    break;
  }
  return 0;
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
