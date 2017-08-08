/******************************************************************************
 * @file     usbd.c
 * @brief    NANO100 series USBD driver Sample file
 * @version  2.0.0
 * @date     20, September, 2014
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/*!<Includes */
#include <string.h>
#include "nano100Series.h"

/*--------------------------------------------------------------------------*/
/* Global variables for Control Pipe */
uint8_t g_usbd_SetupPacket[8] = {0};
volatile uint8_t g_usbd_RemoteWakeupEn = 0; /*!< Remote wake up function enable flag */

/**
 * @cond HIDDEN_SYMBOLS
 */
static volatile uint8_t *g_usbd_CtrlInPointer = 0;
static volatile uint32_t g_usbd_CtrlInSize = 0;
static volatile uint8_t *g_usbd_CtrlOutPointer = 0;
static volatile uint32_t g_usbd_CtrlOutSize = 0;
static volatile uint32_t g_usbd_CtrlOutSizeLimit = 0;
static volatile uint32_t g_usbd_UsbAddr = 0;
static volatile uint32_t g_usbd_CtrlMaxPktSize = 8;
static volatile uint32_t g_usbd_UsbAltInterface = 0;
volatile uint32_t g_usbd_UsbConfig = 0;
/**
 * @endcond
 */

S_USBD_INFO_T *g_usbd_sInfo;

VENDOR_REQ g_usbd_pfnVendorRequest = NULL;
CLASS_REQ g_usbd_pfnClassRequest = NULL;
SET_INTERFACE_REQ g_usbd_pfnSetInterface = NULL;
uint32_t g_u32EpStallLock = 0;       /*!< Bit map flag to lock specified EP when SET_FEATURE */

/**
  * @brief  USBD Initial, Enable clock and reset USB.
  * @param[in]   param               Descriptor
  * @param[in]   pfnClassReq         Class Request Callback Function
  * @param[in]   pfnSetInterface     SetInterface Request Callback Function
  * @retval None.
  */
void USBD_Open(S_USBD_INFO_T *param, CLASS_REQ pfnClassReq, SET_INTERFACE_REQ pfnSetInterface)
{
    g_usbd_sInfo = param;
    g_usbd_pfnClassRequest = pfnClassReq;
    g_usbd_pfnSetInterface = pfnSetInterface;

    /* get EP0 maximum packet size */
    g_usbd_CtrlMaxPktSize = g_usbd_sInfo->gu8DevDesc[7];

    /* Initial USB engine */
    USBD->CTL = 0x29f;
    USBD->PDMA |= USBD_PDMA_BYTEM_Msk;
    /* Force SE0, and then clear it to connect*/
    USBD_SET_SE0();
}

/**
 * @brief       USBD Start
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to start transfer
 */
void USBD_Start(void)
{
    /* Enable USB-related interrupts. */
    USBD_ENABLE_INT(USBD_INT_BUS | USBD_INT_USB | USBD_INT_FLDET | USBD_INT_WAKEUP);
    CLK_SysTickDelay(100000);
    USBD_CLR_SE0();
}

/**
 * @brief       Get Setup Packet
 *
 * @param[in]   buf Buffer pointer to store setup packet
 *
 * @return      None
 *
 * @details     This function is used to get Setup packet.
 */
void USBD_GetSetupPacket(uint8_t *buf)
{
    USBD_MemCopy(buf, g_usbd_SetupPacket, 8);
}

/**
 * @brief       Process Setup Packet
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to process Setup packet.
 */
void USBD_ProcessSetupPacket(void)
{
    // Setup packet process
    USBD_MemCopy(g_usbd_SetupPacket, (uint8_t *)USBD_BUF_BASE, 8);

    switch (g_usbd_SetupPacket[0] & 0x60) { /* request type */
    case REQ_STANDARD: { // Standard
        USBD_StandardRequest();
        break;
    }
    case REQ_CLASS: { // Class
        if (g_usbd_pfnClassRequest != NULL) {
            g_usbd_pfnClassRequest();
        }
        break;
    }
    case REQ_VENDOR: { // Vendor
        if (g_usbd_pfnVendorRequest != NULL) {
            g_usbd_pfnVendorRequest();
        }
        break;
    }
    default: { // reserved
        /* Setup error, stall the device */
        USBD_SET_EP_STALL(EP0);
        USBD_SET_EP_STALL(EP1);
        break;
    }
    }
}

/**
 * @brief       Get Descriptor request
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to process GetDescriptor request.
 */
void USBD_GetDescriptor(void)
{
    uint32_t u32Len;

    u32Len = 0;
    u32Len = g_usbd_SetupPacket[7];
    u32Len <<= 8;
    u32Len += g_usbd_SetupPacket[6];

    switch (g_usbd_SetupPacket[3]) {
    // Get Device Descriptor
    case DESC_DEVICE: {
        u32Len = Minimum(u32Len, LEN_DEVICE);
        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8DevDesc, u32Len);
        USBD_PrepareCtrlOut(0,0);
        break;
    }
    // Get Configuration Descriptor
    case DESC_CONFIG: {
        uint32_t u32TotalLen;

        u32TotalLen = g_usbd_sInfo->gu8ConfigDesc[3];
        u32TotalLen = g_usbd_sInfo->gu8ConfigDesc[2] + (u32TotalLen << 8);

        u32Len = Minimum(u32Len, u32TotalLen);
        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8ConfigDesc, u32Len);
        USBD_PrepareCtrlOut(0,0);
        break;
    }
    // Get HID Descriptor
    case DESC_HID: {
        /* CV3.0 HID Class Descriptor Test,
           Need to indicate index of the HID Descriptor within gu8ConfigDescriptor, specifically HID Composite device. */
        uint32_t u32ConfigDescOffset;   // u32ConfigDescOffset is configuration descriptor offset (HID descriptor start index)
        u32Len = Minimum(u32Len, LEN_HID);
        u32ConfigDescOffset = g_usbd_sInfo->gu32ConfigHidDescIdx[g_usbd_SetupPacket[4]];
        USBD_PrepareCtrlIn((uint8_t *)&g_usbd_sInfo->gu8ConfigDesc[u32ConfigDescOffset], u32Len);
        USBD_PrepareCtrlOut(0,0);
        break;
    }
    // Get Report Descriptor
    case DESC_HID_RPT: {
        u32Len = Minimum(u32Len, g_usbd_sInfo->gu32HidReportSize[g_usbd_SetupPacket[4]]);
        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8HidReportDesc[g_usbd_SetupPacket[4]], u32Len);
        USBD_PrepareCtrlOut(0,0);
        break;
    }
    // Get String Descriptor
    case DESC_STRING: {
        // Get String Descriptor
        if(g_usbd_SetupPacket[2] < 4) {
            u32Len = Minimum(u32Len, g_usbd_sInfo->gu8StringDesc[g_usbd_SetupPacket[2]][0]);
            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8StringDesc[g_usbd_SetupPacket[2]], u32Len);
            USBD_PrepareCtrlOut(0, 0);
        } else {
            // Not support. Reply STALL.
            USBD_SET_EP_STALL(EP0);
            USBD_SET_EP_STALL(EP1);
        }
        break;
    }
    default:
        // Not support. Reply STALL.
        USBD_SET_EP_STALL(EP0);
        USBD_SET_EP_STALL(EP1);
        break;
    }
}

/**
 * @brief       Process USB standard request
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to process USB Standard Request.
 */
void USBD_StandardRequest(void)
{
    /* clear global variables for new request */
    g_usbd_CtrlInPointer = 0;
    g_usbd_CtrlInSize = 0;

    if (g_usbd_SetupPacket[0] & 0x80) { /* request data transfer direction */
        // Device to host
        switch (g_usbd_SetupPacket[1]) {
        case GET_CONFIGURATION: {
            // Return current configuration setting
            /* Data stage */
            M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)) = g_usbd_UsbConfig;
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 1);
            /* Status stage */
            USBD_PrepareCtrlOut(0,0);
            break;
        }
        case GET_DESCRIPTOR: {
            USBD_GetDescriptor();
            break;
        }
        case GET_INTERFACE: {
            // Return current interface setting
            /* Data stage */
            M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)) = g_usbd_UsbAltInterface;
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 1);
            /* Status stage */
            USBD_PrepareCtrlOut(0,0);
            break;
        }
        case GET_STATUS: {
            // Device
            if(g_usbd_SetupPacket[0] == 0x80) {
                uint8_t u8Tmp;

                u8Tmp = 0;
                if(g_usbd_sInfo->gu8ConfigDesc[7] & 0x40) u8Tmp |= 1; // Self-Powered/Bus-Powered.
                if(g_usbd_sInfo->gu8ConfigDesc[7] & 0x20) u8Tmp |= (g_usbd_RemoteWakeupEn << 1); // Remote wake up

                M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)) = u8Tmp;
            }
            // Interface
            else if (g_usbd_SetupPacket[0] == 0x81)
                M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)) = 0;
            // Endpoint
            else if (g_usbd_SetupPacket[0] == 0x82) {
                uint8_t ep = g_usbd_SetupPacket[4] & 0xF;
                M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)) = USBD_GetStall(ep)? 1 : 0;
            }

            M8(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0) + 1) = 0;
            /* Data stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 2);
            /* Status stage */
            USBD_PrepareCtrlOut(0,0);
            break;
        }
        default: {
            /* Setup error, stall the device */
            USBD_SET_EP_STALL(EP0);
            USBD_SET_EP_STALL(EP1);
            break;
        }
        }
    } else {
        // Host to device
        switch (g_usbd_SetupPacket[1]) {
        case CLEAR_FEATURE: {
            if(g_usbd_SetupPacket[2] == FEATURE_ENDPOINT_HALT) {

                int32_t epNum, i;

                /* EP number stall is not allow to be clear in MSC class "Error Recovery Test".
                   a flag: g_u32EpStallLock is added to support it */
                epNum = g_usbd_SetupPacket[4] & 0xF;
                for(i = 0; i < USBD_MAX_EP; i++) {
                    if(((USBD->EP[i].CFG & 0xF) == epNum) && ((g_u32EpStallLock & (1 << i)) == 0))
                        USBD->EP[i].CFG &= ~(USBD_CFG_SSTALL_Msk | USBD_CFG_DSQ_SYNC_Msk);
                }
            } else if(g_usbd_SetupPacket[2] == FEATURE_DEVICE_REMOTE_WAKEUP)
                g_usbd_RemoteWakeupEn = 0;
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        case SET_ADDRESS: {
            g_usbd_UsbAddr = g_usbd_SetupPacket[2];

            // DATA IN for end of setup
            /* Status Stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        case SET_CONFIGURATION: {
            g_usbd_UsbConfig = g_usbd_SetupPacket[2];

            if (g_usbd_UsbConfig == 0) {
                int volatile i;
                /* Reset PID DATA0 */
                for (i=2; i<USBD_MAX_EP; i++)
                    USBD->EP[i].CFG &= ~USBD_CFG_DSQ_SYNC_Msk;
            }
            // DATA IN for end of setup
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);

            break;
        }
        case SET_FEATURE: {
            if(g_usbd_SetupPacket[2] == FEATURE_ENDPOINT_HALT)
                USBD_SetStall(g_usbd_SetupPacket[4] & 0xF);
            else if(g_usbd_SetupPacket[2] == FEATURE_DEVICE_REMOTE_WAKEUP)
                g_usbd_RemoteWakeupEn = 1;
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        case SET_INTERFACE: {
            g_usbd_UsbAltInterface = g_usbd_SetupPacket[2];
            if (g_usbd_pfnSetInterface != NULL)
                g_usbd_pfnSetInterface(g_usbd_UsbAltInterface);
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        default: {
            /* Setup error, stall the device */
            USBD_SET_EP_STALL(EP0);
            USBD_SET_EP_STALL(EP1);
            break;
        }
        }
    }
}

/**
 * @brief       Prepare Control IN transaction
 *
 * @param[in]   pu8Buf      Control IN data pointer
 * @param[in]   u32Size     IN transfer size
 *
 * @return      None
 *
 * @details     This function is used to prepare Control IN transfer
 */
void USBD_PrepareCtrlIn(uint8_t *pu8Buf, uint32_t u32Size)
{
    if(u32Size > g_usbd_CtrlMaxPktSize) {
        // Data size > MXPLD
        g_usbd_CtrlInPointer = pu8Buf + g_usbd_CtrlMaxPktSize;
        g_usbd_CtrlInSize = u32Size - g_usbd_CtrlMaxPktSize;
        USBD_SET_DATA1(EP0);
        USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), pu8Buf, g_usbd_CtrlMaxPktSize);
        USBD_SET_PAYLOAD_LEN(EP0, g_usbd_CtrlMaxPktSize);
    } else {
        // Data size <= MXPLD
        g_usbd_CtrlInPointer = 0;
        g_usbd_CtrlInSize = 0;
        USBD_SET_DATA1(EP0);
        USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), pu8Buf, u32Size);
        USBD_SET_PAYLOAD_LEN(EP0, u32Size);
    }
}

/**
 * @brief       Start Control IN transfer
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to start Control IN
 */
void USBD_CtrlIn(void)
{
    if(g_usbd_CtrlInSize) {
        // Process remained data
        if(g_usbd_CtrlInSize > g_usbd_CtrlMaxPktSize) {
            // Data size > MXPLD
            USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), (uint8_t *)g_usbd_CtrlInPointer, g_usbd_CtrlMaxPktSize);
            USBD_SET_PAYLOAD_LEN(EP0, g_usbd_CtrlMaxPktSize);
            g_usbd_CtrlInPointer += g_usbd_CtrlMaxPktSize;
            g_usbd_CtrlInSize -= g_usbd_CtrlMaxPktSize;
        } else {
            // Data size <= MXPLD
            USBD_MemCopy((uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0), (uint8_t *)g_usbd_CtrlInPointer, g_usbd_CtrlInSize);
            USBD_SET_PAYLOAD_LEN(EP0, g_usbd_CtrlInSize);
            g_usbd_CtrlInPointer = 0;
            g_usbd_CtrlInSize = 0;
        }
    } else {
        // In ACK for Set address
        if((g_usbd_SetupPacket[0] == REQ_STANDARD) && (g_usbd_SetupPacket[1] == SET_ADDRESS)) {
            if((USBD_GET_ADDR() != g_usbd_UsbAddr) && (USBD_GET_ADDR() == 0)) {
                USBD_SET_ADDR(g_usbd_UsbAddr);
            }
        }

        // No more data for IN token
        USBD_SET_PAYLOAD_LEN(EP0, 0);
    }
}

/**
 * @brief       Prepare Control OUT transaction
 *
 * @param[in]   pu8Buf      Control OUT data pointer
 * @param[in]   u32Size     OUT transfer size
 *
 * @return      None
 *
 * @details     This function is used to prepare Control OUT transfer
 */
void USBD_PrepareCtrlOut(uint8_t *pu8Buf, uint32_t u32Size)
{
    g_usbd_CtrlOutPointer = pu8Buf;
    g_usbd_CtrlOutSize = 0;
    g_usbd_CtrlOutSizeLimit = u32Size;
    USBD_SET_PAYLOAD_LEN(EP1, g_usbd_CtrlMaxPktSize);
}

/**
 * @brief       Start Control OUT transfer
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to start Control OUT
 */
void USBD_CtrlOut(void)
{
    uint32_t u32Size;

    if(g_usbd_CtrlOutSize < g_usbd_CtrlOutSizeLimit) {
        u32Size = USBD_GET_PAYLOAD_LEN(EP1);
        USBD_MemCopy((uint8_t *)g_usbd_CtrlOutPointer, (uint8_t *)USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP1), u32Size);
        g_usbd_CtrlOutPointer += u32Size;
        g_usbd_CtrlOutSize += u32Size;
    }
}

/**
 * @brief       Clear all software flags
 *
 * @param       None
 *
 * @return      None
 *
 * @details     This function is used to clear all software control flag
 */
void USBD_SwReset(void)
{
    int i;

    // Reset all variables for protocol
    g_usbd_CtrlInPointer = 0;
    g_usbd_CtrlInSize = 0;
    g_usbd_CtrlOutPointer = 0;
    g_usbd_CtrlOutSize = 0;
    g_usbd_CtrlOutSizeLimit = 0;
    memset(g_usbd_SetupPacket, 0, 8);

    /* Reset PID DATA0 */
    for (i=0; i<USBD_MAX_EP; i++)
        USBD->EP[i].CFG &= ~USBD_CFG_DSQ_SYNC_Msk;

    // Reset USB device address
    USBD_SET_ADDR(0);
}


/**
 * @brief       USBD Set Vendor Request
 *
 * @param[in]   pfnVendorReq         Vendor Request Callback Function
 *
 * @return      None
 *
 * @details     This function is used to set USBD vendor request callback function
 */
void USBD_SetVendorRequest(VENDOR_REQ pfnVendorReq)
{
    g_usbd_pfnVendorRequest = pfnVendorReq;
}


void USBD_LockEpStall(uint32_t u32EpBitmap)
{
    g_u32EpStallLock = u32EpBitmap;
}



