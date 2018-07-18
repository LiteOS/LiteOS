/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/*******
 * Notice:
 * The parts of this file refer to https://blog.csdn.net/zlk1214/article/details/76177336
 * Originator: ZLK1214
 *******/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __WIFI_CONFIG_H__
#define __WIFI_CONFIG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include "stm32f429xx.h"

/* Defines ------------------------------------------------------------------*/

#if   defined ( __CC_ARM )  /* ARM Compiler 4/5 */
  #define __PACKED_STRUCT_BEGIN         __packed
  #define __PACKED_STRUCT_END
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler 6 (armclang) */
  #define __PACKED_STRUCT_BEGIN        
  #define __PACKED_STRUCT_END           __PACKED 
#elif defined ( __GNUC__ )  /* GNU Compiler */
  #define __PACKED_STRUCT_BEGIN        
  #define __PACKED_STRUCT_END           __PACKED 
#elif defined ( __ICCARM__ )    /* IAR Compiler */
  #define __PACKED_STRUCT_BEGIN         __PACKED
  #define __PACKED_STRUCT_END            
#elif defined ( __TI_ARM__ )    /* TI ARM Compiler */
  #define __PACKED_STRUCT_BEGIN        
  #define __PACKED_STRUCT_END           __PACKED 
#endif


/* print defines */
#define WIFI_DEBUG_LOG
#ifdef WIFI_DEBUG_LOG
#define WIFI_LOG(fmt, arg...) do { printf("[%s:%d]" fmt "\r\n", __func__, __LINE__, ##arg);} while(0)
#else
#define WIFI_LOG(fmt, arg...)
#endif

#define WIFI_ERROR(fmt, arg...) do { \
                printf(">>WIFI_ERROR<<:[%s:%d]" fmt "\r\n", __func__, __LINE__, ##arg);} while(0)
#define WIFI_WARN(fmt, arg...) do { \
                printf(">>WIFI_WARN<<:[%s:%d]" fmt "\r\n", __func__, __LINE__, ##arg);} while(0)
                

/* options */
#define WIFI_DEFAULT_MAXRETRY           10      // retrans
// timeout (ms)
//#define WIFI_DEFAULT_TIMEOUT_CMDACK     20    // The timeout of command acknowledge
//#define WIFI_DEFAULT_TIMEOUT_CMDRESP    1000  // The timeout of command response
//#define WIFI_DEFAULT_TIMEOUT_DATAACK    20    // The timeout of data acknowledge
#define WIFI_DEFAULT_TIMEOUT_CMDACK     200    // The timeout of command acknowledge
#define WIFI_DEFAULT_TIMEOUT_CMDRESP    2000  // The timeout of command response
#define WIFI_DEFAULT_TIMEOUT_DATAACK    200    // The timeout of data acknowledge

#define WIFI_DEFAULT_TIMEOUT_CHECK      2000
// display
#define WIFI_DISPLAY_PACKET_SIZE    
#define WIFI_DISPLAY_PACKET_RX 
#define WIFI_DISPLAY_PACKET_TX 
#define WIFI_DISPLAY_RESPTIME
#define WIFI_DISPLAY_SCANNED_SSID  // Displays the hot spot information scanned

#define WIFI_HIGHSPEED    // High Speed Mode
#define WIFI_USEDMA       // Use DMA transfer

//#define WIFI_FIRMWAREAREA_ADDR 0x08061000     // The header address of firmware in the inner flash

#ifdef WIFI_FIRMWAREAREA_ADDR
// The storage format: 
//    helperSize(4B) + firmwareSize(4B) + helperContent + firmwareContent + CRC(4B)
#define WIFI_HELPER_SIZE        (*(const uint32_t *)WIFI_FIRMWAREAREA_ADDR)
#define WIFI_FIRMWARE_SIZE      (*(const uint32_t *)(WIFI_FIRMWAREAREA_ADDR + 4))
#define WIFI_HELPER_ADDR        ((const uint8_t *)WIFI_FIRMWAREAREA_ADDR + 8)
#define WIFI_FIRMWARE_ADDR      (WIFI_HELPER_ADDR + WIFI_HELPER_SIZE)
#else
extern const unsigned char firmware_helper_sd[2516];
extern const unsigned char firmware_sd8686[122916];
#define WIFI_HELPER_SIZE    sizeof(firmware_helper_sd)
#define WIFI_FIRMWARE_SIZE  sizeof(firmware_sd8686)
#define WIFI_HELPER_ADDR    firmware_helper_sd
#define WIFI_FIRMWARE_ADDR  firmware_sd8686
#endif

/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __WIFI_CONFIG_H__ */
