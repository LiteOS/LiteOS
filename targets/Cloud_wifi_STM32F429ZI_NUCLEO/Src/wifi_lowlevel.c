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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "los_hwi.h"
#include "dwt.h"
#include "wifi.h"

/* Defines ------------------------------------------------------------------*/
#define CMD52_WRITE             _BV(31)
#define CMD52_READAFTERWRITE    _BV(27)
#define CMD53_WRITE             _BV(31)
#define CMD53_BLOCKMODE         _BV(27)
#define CMD53_INCREMENTING      _BV(26)

// DMA must be used in high speed mode
#if defined(WIFI_HIGHSPEED) && !defined(WIFI_USEDMA)
#error "DMA must be enabled when SDIO is in high speed mode!"
#endif

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/

// Block size of each functional area, 
// saved in this variable to avoid sending the CMD52 command to read the SDIO register every time
static uint16_t sdio_block_size[2]; 
static uint8_t sdio_func_num = 0; // Total number of functional areas (except no. 0 functional areas)
static uint16_t sdio_rca; // RCA relative address

static SDIO_CmdInitTypeDef sdio_cmd;
static SDIO_DataInitTypeDef sdio_data;
#ifdef WIFI_USEDMA
DMA_HandleTypeDef hdma_sdio;
#endif


/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
static uint8_t WiFi_LowLevel_CheckError(const char *msg_title);
static uint16_t WiFi_LowLevel_GetBlockNum(uint8_t func, uint32_t *psize);
static void WiFi_LowLevel_GPIOInit(void);
static void WiFi_LowLevel_SDIOInit(void);
static void WiFi_LowLevel_SendCMD52(uint8_t func, uint32_t addr, uint8_t data, uint32_t flags);
static void WiFi_LowLevel_SendCMD53(uint8_t func, uint32_t addr, uint16_t count, uint32_t flags);
static void WiFi_LowLevel_SetSDIOBlockSize(uint32_t size);
#ifdef WIFI_FIRMWAREAREA_ADDR
static uint8_t WiFi_LowLevel_VerifyFirmware(void);
#endif
static void WiFi_LowLevel_WaitForResponse(const char *msg_title);

/* Public functions ---------------------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/* Check and clear error flags */
static uint8_t WiFi_LowLevel_CheckError(const char *msg_title)
{
    uint8_t err = 0;
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CCRCFAIL) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CCRCFAIL);
        err++;
        printf("%s: CMD%ld CRC failed!\n", msg_title, SDIO->CMD & SDIO_CMD_CMDINDEX);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CTIMEOUT) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CTIMEOUT);
        err++;
        printf("%s: CMD%ld timeout!\n", msg_title, SDIO->CMD & SDIO_CMD_CMDINDEX);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_DCRCFAIL) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DCRCFAIL);
        err++;
        printf("%s: data CRC failed!\n", msg_title);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_DTIMEOUT) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DTIMEOUT);
        err++;
        printf("%s: data timeout!\n", msg_title);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_STBITERR) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_STBITERR);
        err++;
        printf("%s: start bit error!\n", msg_title);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXUNDERR) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_TXUNDERR);
        err++;
        printf("%s: data underrun!\n", msg_title);
    }
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXOVERR) == SET)
    {
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_RXOVERR);
        err++;
        printf("%s: data overrun!\n", msg_title);
    }
#ifdef WIFI_USEDMA
    if (__HAL_DMA_GET_FLAG(&hdma_sdio, DMA_FLAG_TEIF2_6) == SET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_sdio, DMA_FLAG_TEIF2_6);
        err++;
        printf("%s: DMA transfer error!\n", msg_title);
    }
#endif
    return err;
}

/* Determine which way data should be transmitted */
// Return value: 0 for multi-byte mode, otherwise represents the number of data blocks in block transport mode
// the value of *psize will be adjusted appropriately and may be greater than the original value
static uint16_t WiFi_LowLevel_GetBlockNum(uint8_t func, uint32_t *psize)
{
    uint16_t block_num = 0;
#ifndef WIFI_HIGHSPEED
    if (*psize > 512) // It is not transmitted in chunks until it is greater than 512 bytes
    {
#endif
        // Block transfer mode (DTMODE=0)
        WiFi_LowLevel_SetSDIOBlockSize(sdio_block_size[func]);

        block_num = *psize / sdio_block_size[func];
        if (*psize % sdio_block_size[func] != 0)
            block_num++;
        *psize = block_num * sdio_block_size[func]; // Number of blocks * block size
#ifndef WIFI_HIGHSPEED
    }
    else
    {
        // Multi-byte transmission mode (DTMODE=1, supported only when the frequency of SDIO is lower than 16MHz)
        *psize = (*psize + 3) & ~3; // The WiFi module requires that the number of bytes written must be integer times 4
    }
#endif
    return block_num;
}

/* Get the number of SDIO function areas supported by WiFi module (except no. 0 function areas) */
uint8_t WiFi_LowLevel_GetFunctionNum(void)
{
    return sdio_func_num;
}

/* Initializes all GPIO pins related to the WiFi module */
static void WiFi_LowLevel_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10    ------> SDIO_D2
    PC11    ------> SDIO_D3
    PC12    ------> SDIO_CK
    PD2     ------> SDIO_CMD
    PG2     ------> VCC_CTRL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Driving force ? */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP; 
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
}

/**
  * @brief  Initializes the GPIO,SDIO,Card and so on.
  * @param  None
  * @retval None
  */
void WiFi_LowLevel_Init(void)
{
    // Verify the firmware of flash which is broken
#ifdef WIFI_FIRMWAREAREA_ADDR
	__HAL_RCC_CRC_CLK_ENABLE(); // Enable CRC
    if (!WiFi_LowLevel_VerifyFirmware())
    {
        printf("Error: The firmware stored in flash memory is corrupted!\n");
        while (1);
    }
#endif

    WiFi_LowLevel_GPIOInit();
    WiFi_LowLevel_SDIOInit();
}

/* Receive data and automatically determine which transmission mode to use */
// Size is the number of bytes to receive, and bufsize is the size of the data buffer
// If bufsize=0, only data is read, but not saved to data, at which point the data can be NULL
uint8_t WiFi_LowLevel_ReadData(uint8_t func, uint32_t addr, void *data, uint32_t size, uint32_t bufsize)
{
    uint16_t block_num; // Number of data blocks
#ifdef WIFI_USEDMA
    hdma_sdio.Instance = DMA2_Stream6;
    uint32_t tempData; // Discard data variables
    uint32_t tempAddr;
#else
    uint32_t *p = data;
#endif
    if ((uintptr_t)data & 3){
        // Memory and peripheral addresses must be aligned when DMA transfers more than one byte at a time, 
        // otherwise they will not be transmitted correctly and will not prompt an error
        printf("WiFi_LowLevel_ReadData: data must be 4-byte aligned!\n"); 
    }
    if (size == 0)
    {
        printf("WiFi_LowLevel_ReadData: size cannot be 0!\n");
        return 0;
    }

    block_num = WiFi_LowLevel_GetBlockNum(func, &size);
    if (bufsize > 0 && bufsize < size)
    {
        printf("WiFi_LowLevel_ReadData: a buffer of at least %ld bytes is required! bufsize=%ld\n", size, bufsize);
        return 0;
    }

#ifdef WIFI_USEDMA
    hdma_sdio.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_PFCTRL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio.Init.PeriphBurst = DMA_PBURST_INC4;
    if (bufsize > 0)
    {
        tempAddr = (uint32_t)data;
        hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    }
    else
    {
        // Data discard mode
        tempAddr = (uint32_t)&tempData;
        hdma_sdio.Init.MemInc = DMA_MINC_DISABLE;
    }
    HAL_DMA_Init(&hdma_sdio);
    HAL_DMA_Start(&hdma_sdio, (uint32_t)&SDIO->FIFO, (uint32_t)tempAddr, size / 4);
#endif

    if (block_num)
    {
        sdio_data.TransferMode = SDIO_TRANSFER_MODE_BLOCK;
        WiFi_LowLevel_SendCMD53(func, addr, block_num, CMD53_BLOCKMODE);
    }
    else
    {
        sdio_data.TransferMode = SDIO_TRANSFER_MODE_STREAM;
        WiFi_LowLevel_SendCMD53(func, addr, size, 0);
    }

    sdio_data.DataLength = size;
    sdio_data.DPSM = SDIO_DPSM_ENABLE;
    sdio_data.TransferDir = SDIO_TRANSFER_DIR_TO_SDIO;
    SDIO_ConfigData(SDIO, &sdio_data);

    while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXACT) == RESET); // Wait to start receiving data
#ifdef WIFI_USEDMA
    __SDIO_DMA_ENABLE(SDIO); // Set it to DMA transfer mode
    while (__HAL_DMA_GET_FLAG(&hdma_sdio, DMA_FLAG_TCIF2_6) == RESET
            && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXACT) == SET
            && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXOVERR) == RESET); // Wait for the DMA to finish reading the data without an error
    __HAL_DMA_CLEAR_FLAG(&hdma_sdio, DMA_FLAG_TCIF2_6); 
    HAL_DMA_Abort(&hdma_sdio); 
#else
    while (size && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXACT) == SET && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXOVERR) == RESET)
    {
        if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXDAVL) == SET)
        {
            size -= 4;
            if (bufsize > 0)
                *p++ = SDIO_ReadFIFO(SDIO);
            else
                SDIO_ReadFIFO(SDIO);
        }
    }
#endif
    while ((__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDACT) == SET
            || __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXACT) == SET)
            && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_RXOVERR) == RESET); // Waiting for receiving
    sdio_data.DPSM = SDIO_DPSM_DISABLE;
    SDIO_ConfigData(SDIO, &sdio_data);
    
    // Clear the associated flag bits
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CMDREND | SDIO_FLAG_DATAEND | SDIO_FLAG_DBCKEND);
    return WiFi_LowLevel_CheckError(__func__) == 0;
}

/* Read the SDIO register */
uint8_t WiFi_LowLevel_ReadReg(uint8_t func, uint32_t addr)
{
    WiFi_LowLevel_SendCMD52(func, addr, 0, 0);
    WiFi_LowLevel_WaitForResponse(__func__);
    return SDIO_GetResponse(SDIO, SDIO_RESP1) & 0xff;
}

/* Initialize SDIO peripherals and complete enumeration of WiFi modules */
// SDIO Simplified Specification Version 3.00: 3. SDIO Card Initialization
static void WiFi_LowLevel_SDIOInit(void)
{
    SDIO_InitTypeDef SDIO_InitStruct;
    uint32_t resp;

    /* Peripheral Clock Enable */
    __HAL_RCC_SDIO_CLK_ENABLE();
#ifdef WIFI_USEDMA
    __HAL_RCC_DMA2_CLK_ENABLE();
#endif

    /* Default SDIO peripheral configuration for SD card initialization */
    SDIO_InitStruct.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
    SDIO_InitStruct.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
    SDIO_InitStruct.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
    SDIO_InitStruct.BusWide             = SDIO_BUS_WIDE_1B;
    SDIO_InitStruct.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    SDIO_InitStruct.ClockDiv            = 178;//SDIO_INIT_CLK_DIV;

    /* Set Power State to ON */
    SDIO_PowerState_ON(SDIO);
    /* Initialize SDIO peripheral interface with default configuration */
    SDIO_Init(SDIO, SDIO_InitStruct);

    /* Enable SDIO Clock */
    __SDIO_ENABLE(SDIO);

    __SDIO_OPERATION_ENABLE(SDIO); // Set it to SDIO mode
#ifdef WIFI_USEDMA
    __SDIO_DMA_ENABLE(SDIO); // Set it to DMA transfer mode
#endif
    // You don't need to send CMD0 because the initialization command for SD I/O card is CMD52
    // An I/O only card or the I/O portion of a combo card is NOT reset by CMD0. (See 4.4 Reset for SDIO)
    delayms(10); // Delay prevents CMD5 retransmission
    
    /* Send CMD5: IO_SEND_OP_COND */
    sdio_cmd.Argument = 0;
    sdio_cmd.CmdIndex = 5;
    sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
    sdio_cmd.Response = SDIO_RESPONSE_SHORT; 
    sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
    SDIO_SendCommand(SDIO, &sdio_cmd);
    WiFi_LowLevel_WaitForResponse(__func__);
    printf("RESPCMD%d, RESP1_%08lx\n", SDIO_GetCommandResponse(SDIO), SDIO_GetResponse(SDIO, SDIO_RESP1));

    /* Set VDD Voltage Window: 3.2~3.4V, And send CMD5 again */
    sdio_cmd.Argument = 0x300000;
    SDIO_SendCommand(SDIO, &sdio_cmd);
    WiFi_LowLevel_WaitForResponse(__func__);
    resp = SDIO_GetResponse(SDIO, SDIO_RESP1);
    printf("RESPCMD%d, RESP1_%08lx\n", SDIO_GetCommandResponse(SDIO), resp);
    if (resp & _BV(31))
    {
        // Card is ready to operate after initialization
        sdio_func_num = (resp >> 28) & 7;
        printf("Number of I/O Functions: %d\n", sdio_func_num);
        printf("Memory Present: %d\n", (resp & _BV(27)) != 0);
    }

    /* Get the WiFi module address (CMD3: SEND_RELATIVE_ADDR, Ask the card to publish a new relative address (RCA)) */
    sdio_cmd.Argument = 0;
    sdio_cmd.CmdIndex = 3;
    SDIO_SendCommand(SDIO, &sdio_cmd);
    WiFi_LowLevel_WaitForResponse(__func__);
    sdio_rca = SDIO_GetResponse(SDIO, SDIO_RESP1) >> 16;
    printf("Relative Card Address: 0x%04x\n", sdio_rca);

    /* Select WiFi module (CMD7: SELECT/DESELECT_CARD) */
    sdio_cmd.Argument = sdio_rca << 16;
    sdio_cmd.CmdIndex = 7;
    SDIO_SendCommand(SDIO, &sdio_cmd);
    WiFi_LowLevel_WaitForResponse(__func__);
    printf("Card selected! RESP1_%08lx\n", SDIO_GetResponse(SDIO, SDIO_RESP1));

    /* Increase the clock frequency and set the data timeout to 0.1s */
#ifdef WIFI_HIGHSPEED
    SDIO_InitStruct.ClockDiv = 1;//SDIO_TRANSFER_CLK_DIV; // SDIOCLK/(ClockDiv+2)=24MHz
    sdio_data.DataTimeOut = 2400000;
    printf("SDIO Clock: 24MHz\n");
#else
    SDIO_InitStruct.ClockDiv = 70; // SDIOCLK/(ClockDiv+2)=4MHz
    sdio_data.DataTimeOut = 100000;
    printf("SDIO Clock: 1MHz\n");
#endif
    
    /* The bus width of SDIO peripheral is set to 4 bits */
    SDIO_InitStruct.BusWide = SDIO_BUS_WIDE_4B;
    SDIO_Init(SDIO, SDIO_InitStruct);
    WiFi_LowLevel_WriteReg(0, SDIO_CCCR_BUSIFCTRL, WiFi_LowLevel_ReadReg(0, SDIO_CCCR_BUSIFCTRL) | SDIO_CCCR_BUSIFCTRL_BUSWID_4Bit);
}

static void WiFi_LowLevel_SendCMD52(uint8_t func, uint32_t addr, uint8_t data, uint32_t flags)
{
    sdio_cmd.Argument = (func << 28) | (addr << 9) | data | flags;
    sdio_cmd.CmdIndex = 52;
    sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
    sdio_cmd.Response = SDIO_RESPONSE_SHORT;
    sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
    SDIO_SendCommand(SDIO, &sdio_cmd);
}

static void WiFi_LowLevel_SendCMD53(uint8_t func, uint32_t addr, uint16_t count, uint32_t flags)
{
    // When count=512, 0x1ff and 0x1ff are equal to 0, which conforms to SDIO standard
    sdio_cmd.Argument = (func << 28) | (addr << 9) | (count & 0x1ff) | flags;
    sdio_cmd.CmdIndex = 53;
    sdio_cmd.CPSM = SDIO_CPSM_ENABLE;
    sdio_cmd.Response = SDIO_RESPONSE_SHORT;
    sdio_cmd.WaitForInterrupt = SDIO_WAIT_NO;
    SDIO_SendCommand(SDIO, &sdio_cmd);
}

/* Set the block size of WiFi module function area */
void WiFi_LowLevel_SetBlockSize(uint8_t func, uint32_t size)
{
    sdio_block_size[func] = size;
    WiFi_LowLevel_WriteReg(0, (func << 8) | 0x10, size & 0xff);
    WiFi_LowLevel_WriteReg(0, (func << 8) | 0x11, size >> 8);
}

/* Set the data block size of SDIO peripherals */
static void WiFi_LowLevel_SetSDIOBlockSize(uint32_t size)
{
    switch (size)
    {
    case 1:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_1B;
        break;
    case 2:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_2B;
        break;
    case 4:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_4B;
        break;
    case 8:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_8B;
        break;
    case 16:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_16B;
        break;
    case 32:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_32B;
        break;
    case 64:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_64B;
        break;
    case 128:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_128B;
        break;
    case 256:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_256B;
        break;
    case 512:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;
        break;
    case 1024:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_1024B;
        break;
    case 2048:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_2048B;
        break;
    case 4096:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_4096B;
        break;
    case 8192:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_8192B;
        break;
    case 16384:
        sdio_data.DataBlockSize = SDIO_DATABLOCK_SIZE_16384B;
        break;
    }
}

/* Check that the firmware content saved in Flash is complete */
#ifdef WIFI_FIRMWAREAREA_ADDR
static uint8_t WiFi_LowLevel_VerifyFirmware(void)
{
    uint32_t crc;
    uint32_t len = (WIFI_HELPER_SIZE + WIFI_FIRMWARE_SIZE) / 4 + 3; // Firmware area (including CRC) is 1/4 of the total size
    CRC_HandleTypeDef hcrc;
    
    hcrc.Instance = CRC;
    crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)WIFI_FIRMWAREAREA_ADDR, len);
    return crc == 0;
}
#endif

/* Wait for the SDIO command response */
static void WiFi_LowLevel_WaitForResponse(const char *msg_title)
{
    uint8_t first = 1;
    do
    {
        if (!first)
        {
            SDIO_SendCommand(SDIO, &sdio_cmd); // Resend the command
        }
        else
        {
            first = 0;
        }
        while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDACT) == SET); // Wait for the command to be sent
        WiFi_LowLevel_CheckError(msg_title);
    }
    while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_CMDREND) == RESET);   // If no response is received, try again
    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_CMDREND);
}

/* Sending data automatically determines which transmission mode to use */
// Size is the number of bytes to send, and bufsize is the size of the data buffer
uint8_t WiFi_LowLevel_WriteData(uint8_t func, uint32_t addr, const void *data, uint32_t size, uint32_t bufsize)
{
    uint16_t block_num; 
#ifdef WIFI_USEDMA
    hdma_sdio.Instance = DMA2_Stream6;
#else
    const uint32_t *p = data;
#endif
//    if ((uintptr_t)data & 3)
//        printf("WiFi_LowLevel_WriteData: data must be 4-byte aligned!\n");
    if (size == 0)
    {
        printf("WiFi_LowLevel_WriteData: size cannot be 0!\n");
        return 0;
    }

    block_num = WiFi_LowLevel_GetBlockNum(func, &size);
    if (bufsize < size){
        // A read-only buffer that is out of bounds does not affect data transfer, so this is just a warning
        printf("WiFi_LowLevel_WriteData: a buffer of at least %ld bytes is required! bufsize=%ld\n", size, bufsize);
    }
    if (block_num)
    {
        sdio_data.TransferMode = SDIO_TRANSFER_MODE_BLOCK;
        WiFi_LowLevel_SendCMD53(func, addr, block_num, CMD53_WRITE | CMD53_BLOCKMODE);
    }
    else
    {
        sdio_data.TransferMode = SDIO_TRANSFER_MODE_STREAM;
        WiFi_LowLevel_SendCMD53(func, addr, size, CMD53_WRITE);
    }
    WiFi_LowLevel_WaitForResponse(__func__); // Data must not be sent until CMD53 receives a response

    // Start sending data
#ifdef WIFI_USEDMA
    hdma_sdio.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_PFCTRL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma_sdio.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio.Init.PeriphBurst = DMA_PBURST_INC4;
    HAL_DMA_Init(&hdma_sdio);
    HAL_DMA_Start(&hdma_sdio, (uint32_t)data, (uint32_t)&SDIO->FIFO, size / 4);
#endif

    sdio_data.DataLength = size;
    sdio_data.DPSM = SDIO_DPSM_ENABLE;
    sdio_data.TransferDir = SDIO_TRANSFER_DIR_TO_CARD;
    SDIO_ConfigData(SDIO, &sdio_data);

    while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXACT) == RESET); 
#ifdef WIFI_USEDMA
    __SDIO_DMA_ENABLE(SDIO); 
    while (__HAL_DMA_GET_FLAG(&hdma_sdio, DMA_FLAG_TCIF2_6) == RESET
            && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXACT) == SET
            && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXUNDERR) == RESET); 
    __HAL_DMA_CLEAR_FLAG(&hdma_sdio, DMA_FLAG_TCIF2_6); 
    HAL_DMA_Abort(&hdma_sdio); 
#else
    while (size && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXACT) == SET && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXUNDERR) == RESET)
    {
        size -= 4;
        SDIO_WriteFIFO(SDIO, (uint32_t*)p++); // Enter 4 bytes of data into FIFO
        while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXFIFOF) == SET); // Wait if FIFO is full
    }
#endif
    
    while (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXACT) == SET && __SDIO_GET_FLAG(SDIO, SDIO_FLAG_TXUNDERR) == RESET); 
    sdio_data.DPSM = SDIO_DPSM_DISABLE;
    SDIO_ConfigData(SDIO, &sdio_data);

    __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_DATAEND | SDIO_FLAG_DBCKEND);
    return WiFi_LowLevel_CheckError(__func__) == 0;
}

/* Write the register and return the actual contents of the register after writing */
uint8_t WiFi_LowLevel_WriteReg(uint8_t func, uint32_t addr, uint8_t value)
{
    WiFi_LowLevel_SendCMD52(func, addr, value, CMD52_WRITE | CMD52_READAFTERWRITE);
    WiFi_LowLevel_WaitForResponse(__func__);
    return SDIO_GetResponse(SDIO, SDIO_RESP1) & 0xff;
}


