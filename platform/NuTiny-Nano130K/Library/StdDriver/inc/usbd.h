/******************************************************************************
 * @file     usbd.h
 * @brief    NANO100 series USB driver header file
 * @version  2.0.0
 * @date     20, September, 2014
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __USBD_H__
#define __USBD_H__


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_USBD_Driver USBD Driver
  @{
*/

/** @addtogroup NANO100_USBD_EXPORTED_STRUCTS USBD Exported Structs
  @{
*/
typedef struct s_usbd_info {
    uint8_t *gu8DevDesc;                /*!< Device descriptor */
    uint8_t *gu8ConfigDesc;             /*!< Config descriptor */
    uint8_t **gu8StringDesc;            /*!< Pointer for USB String Descriptor pointers */
    uint8_t **gu8HidReportDesc;         /*!< Pointer for HID Report descriptor */
    uint32_t *gu32HidReportSize;        /*!< Pointer for HID Report descriptor Size */
    uint32_t *gu32ConfigHidDescIdx; 	/*!< Pointer for HID Descriptor start index */	
} S_USBD_INFO_T;

/// @cond HIDDEN_SYMBOLS
extern S_USBD_INFO_T gsInfo;
/// @endcond /* HIDDEN_SYMBOLS */

/*@}*/ /* end of group NANO100_USBD_EXPORTED_STRUCTS */

/** @addtogroup NANO100_USBD_EXPORTED_CONSTANTS USBD Exported Constants
  @{
*/
#define USBD_BUF_BASE   (USBD_BASE+0x100)

#define USBD_MAX_EP     8

#define EP0     0       /*!< Endpoint 0 */
#define EP1     1       /*!< Endpoint 1 */
#define EP2     2       /*!< Endpoint 2 */
#define EP3     3       /*!< Endpoint 3 */
#define EP4     4       /*!< Endpoint 4 */
#define EP5     5       /*!< Endpoint 5 */
#define EP6     6       /*!< Endpoint 6 */
#define EP7     7       /*!< Endpoint 7 */

/// @cond HIDDEN_SYMBOLS
extern volatile uint32_t g_usbd_UsbConfig;

/*!<USB Request Type */
#define REQ_STANDARD        0x00
#define REQ_CLASS           0x20
#define REQ_VENDOR          0x40

/*!<USB Standard Request */
#define GET_STATUS          0x00
#define CLEAR_FEATURE       0x01
#define SET_FEATURE         0x03
#define SET_ADDRESS         0x05
#define GET_DESCRIPTOR      0x06
#define SET_DESCRIPTOR      0x07
#define GET_CONFIGURATION   0x08
#define SET_CONFIGURATION   0x09
#define GET_INTERFACE       0x0A
#define SET_INTERFACE       0x0B
#define SYNC_FRAME          0x0C

/*!<USB Descriptor Type */
#define DESC_DEVICE         0x01
#define DESC_CONFIG         0x02
#define DESC_STRING         0x03
#define DESC_INTERFACE      0x04
#define DESC_ENDPOINT       0x05
#define DESC_QUALIFIER      0x06
#define DESC_OTHERSPEED     0x07

/*!<USB HID Descriptor Type */
#define DESC_HID            0x21
#define DESC_HID_RPT        0x22

/*!<USB Descriptor Length */
#define LEN_DEVICE          18
#define LEN_CONFIG          9
#define LEN_INTERFACE       9
#define LEN_ENDPOINT        7
#define LEN_HID             9
#define LEN_CCID            0x36

/*!<USB Endpoint Type */
#define EP_ISO              0x01
#define EP_BULK             0x02
#define EP_INT              0x03

#define EP_INPUT            0x80
#define EP_OUTPUT           0x00

/*!<USB Feature Selector */
#define FEATURE_DEVICE_REMOTE_WAKEUP    0x01
#define FEATURE_ENDPOINT_HALT           0x00

/// @endcond

#define USBD_WAKEUP_EN          USBD_CTL_WAKEUP_EN_Msk      /*!< USB Wake-up Enable */
#define USBD_DRVSE0             USBD_CTL_DRVSE0_Msk         /*!< Drive SE0 */

#define USBD_DPPU_EN            USBD_CTL_DPPU_EN_Msk        /*!< USB D+ Pull-up Enable */
#define USBD_PWRDN              USBD_CTL_PWRDB_Msk          /*!< PHY Turn-On */
#define USBD_PHY_EN             USBD_CTL_PHY_EN_Msk         /*!< PHY Enable */
#define USBD_USB_EN             USBD_CTL_USB_EN_Msk         /*!< USB Enable */

#define USBD_INT_BUS            USBD_INTEN_BUSEVT_IE_Msk    /*!< USB Bus Event Interrupt */
#define USBD_INT_USB            USBD_INTEN_USBEVT_IE_Msk    /*!< USB usb Event Interrupt */
#define USBD_INT_FLDET          USBD_INTEN_FLDET_IE_Msk     /*!< USB Float Detect Interrupt */
#define USBD_INT_WAKEUP         USBD_INTEN_WAKEUP_IE_Msk    /*!< USB Wake-up Interrupt */

#define USBD_INTSTS_WAKEUP      USBD_INTSTS_WKEUP_STS_Msk   /*!< USB Wakeup Interrupt Status */
#define USBD_INTSTS_FLDET       USBD_INTSTS_FLD_STS_Msk     /*!< USB Float Detect Interrupt Status */
#define USBD_INTSTS_BUS         USBD_INTSTS_BUS_STS_Msk     /*!< USB Bus Event Interrupt Status */
#define USBD_INTSTS_USB         USBD_INTSTS_USB_STS_Msk     /*!< USB usb Event Interrupt Status */
#define USBD_INTSTS_SETUP       USBD_INTSTS_SETUP_Msk       /*!< USB Setup Event */
#define USBD_INTSTS_EP0         USBD_INTSTS_EPEVT0_Msk      /*!< USB Endpoint 0 Event */
#define USBD_INTSTS_EP1         USBD_INTSTS_EPEVT1_Msk      /*!< USB Endpoint 1 Event */
#define USBD_INTSTS_EP2         USBD_INTSTS_EPEVT2_Msk      /*!< USB Endpoint 2 Event */
#define USBD_INTSTS_EP3         USBD_INTSTS_EPEVT3_Msk      /*!< USB Endpoint 3 Event */
#define USBD_INTSTS_EP4         USBD_INTSTS_EPEVT4_Msk      /*!< USB Endpoint 4 Event */
#define USBD_INTSTS_EP5         USBD_INTSTS_EPEVT5_Msk      /*!< USB Endpoint 5 Event */
#define USBD_INTSTS_EP6         USBD_INTSTS_EPEVT6_Msk      /*!< USB Endpoint 6 Event */
#define USBD_INTSTS_EP7         USBD_INTSTS_EPEVT7_Msk      /*!< USB Endpoint 7 Event */

#define USBD_STATE_USBRST       USBD_BUSSTS_USBRST_Msk      /*!< USB Bus Reset */
#define USBD_STATE_SUSPEND      USBD_BUSSTS_SUSPEND_Msk     /*!< USB Bus Suspend */
#define USBD_STATE_RESUME       USBD_BUSSTS_RESUME_Msk      /*!< USB Bus Resume */
#define USBD_STATE_TIMEOUT      USBD_BUSSTS_TIMEOUT_Msk     /*!< USB Bus Timeout */

#define USBD_CFG_SSTALL         USBD_CFG_SSTALL_Msk         /*!< Set Stall */
#define USBD_CFG_CSTALL         USBD_CFG_CSTALL_Msk         /*!< Clear Stall */

#define USBD_CFG_EPMODE_DISABLE (0ul << USBD_CFG_EPMODE_Pos)/*!< Endpoint Disable */
#define USBD_CFG_EPMODE_OUT     (1ul << USBD_CFG_EPMODE_Pos)/*!< Out Endpoint */
#define USBD_CFG_EPMODE_IN      (2ul << USBD_CFG_EPMODE_Pos)/*!< In Endpoint */
#define USBD_CFG_TYPE_ISO       (1ul << USBD_CFG_ISOCH_Pos) /*!< Isochronous */


/*@}*/ /* end of group NANO100_USBD_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_USBD_EXPORTED_FUNCTIONS USBD Exported Functions
  @{
*/

/**
  * @brief      Compare two input numbers and return maximum one.
  *
  * @param[in]  a   First number to be compared.
  * @param[in]  b   Second number to be compared.
  *
  * @return     Maximum value between a and b.
  *
  * @details    If a > b, then return a. Otherwise, return b.
  */
#define Maximum(a,b)                ((a)>(b) ? (a) : (b))


/**
  * @brief      Compare two input numbers and return minimum one
  *
  * @param[in]  a   First number to be compared
  * @param[in]  b   Second number to be compared
  *
  * @return     Minimum value between a and b
  *
  * @details    If a < b, then return a. Otherwise, return b.
  */
#define Minimum(a,b)                ((a)<(b) ? (a) : (b))

/**
  * @brief      Enable USBD engine
  * @param      None
  * @retval     None
  */
#define USBD_ENABLE_USB()           ((uint32_t)(USBD->CTL |= 0xF))

/**
  * @brief      Disable USBD engine
  * @param      None
  * @retval     None
  */
#define USBD_DISABLE_USB()          ((uint32_t)(USBD->CTL &= ~USBD_USB_EN))

/**
  * @brief      Enable USBD PHY
  * @param      None
  * @retval     None
  */
#define USBD_ENABLE_PHY()           ((uint32_t)(USBD->CTL |= USBD_PHY_EN))

/**
  * @brief      Disable USBD PHY
  * @param      None
  * @retval     None
  */
#define USBD_DISABLE_PHY()          ((uint32_t)(USBD->CTL &= ~USBD_PHY_EN))

/**
  * @brief      Force USB PHY Transceiver to Drive SE0
  * @param      None
  * @retval     None
  */
#define USBD_SET_SE0()              ((uint32_t)(USBD->CTL |= USBD_DRVSE0))

/**
  * @brief      Release SE0
  * @param      None
  * @retval     None
  */
#define USBD_CLR_SE0()              ((uint32_t)(USBD->CTL &= ~USBD_DRVSE0))

/**
  * @brief      Set USBD address
  * @param[in]  addr    host assign address number
  * @retval     None
  */
#define USBD_SET_ADDR(addr)         (USBD->FADDR = (addr))

/**
  * @brief      Get USBD address
  * @param      None
  * @retval     USBD address
  */
#define USBD_GET_ADDR()             ((uint32_t)(USBD->FADDR))

/**
  * @brief      Enable USBD interrupt
  * @param[in]  intr    interrupt mask
  * @retval     None
  */
#define USBD_ENABLE_INT(intr)       (USBD->INTEN |= (intr))

/**
  * @brief      Get USBD interrupt flag
  * @param      None
  * @retval     interrupt status
  */
#define USBD_GET_INT_FLAG()         ((uint32_t)(USBD->INTSTS))

/**
  * @brief      Clear USBD interrupt 
  * @param[in]  flag    interrupt flag      
  * @retval     None
  */
#define USBD_CLR_INT_FLAG(flag)     (USBD->INTSTS = flag)

/**
  * @brief      Get USBD Endpoint status
  * @param      None
  * @retval     endpoint status
  */
#define USBD_GET_EP_FLAG()          ((uint32_t)(USBD->EPSTS))

/**
  * @brief      Get USBD bus state
  * @param      None
  * @retval     bus status
  */
#define USBD_GET_BUS_STATE()        ((uint32_t)(USBD->BUSSTS & 0xf))

/**
  * @brief      check cable connect state
  * @param      None
  * @retval     connect / disconnect
  */
#define USBD_IS_ATTACHED()          ((uint32_t)(USBD->BUSSTS & USBD_BUSSTS_FLDET_Msk))

/**
  * @brief      Stop USB endpoint transaction
  * @param[in]  ep endpoint
  * @retval     None
  */
#define USBD_STOP_TRANSACTION(ep)   (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) |= USBD_CFG_CLRRDY_Msk)

/**
  * @brief      Set USB data1 token
  * @param[in]  ep endpoint
  * @retval     None
  */
#define USBD_SET_DATA1(ep)          (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) |= USBD_CFG_DSQ_SYNC_Msk)

/**
  * @brief      Set USB data0 token
  * @param[in]  ep endpoint
  * @retval     None
  */
#define USBD_SET_DATA0(ep)          (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) &= (~USBD_CFG_DSQ_SYNC_Msk))

/**
  * @brief      Set USB payload size (IN data)
  * @param[in]  ep endpoint
  * @param[in]  size IN transfer length
  * @retval     None
  */
#define USBD_SET_PAYLOAD_LEN(ep, size)  (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].MXPLD + (uint32_t)((ep) << 4))) = (size))

/**
  * @brief      Get USB payload size (OUT data)
  * @param[in]  ep endpoint
  * @retval     received data length
  */
#define USBD_GET_PAYLOAD_LEN(ep)        ((uint32_t)*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].MXPLD + (uint32_t)((ep) << 4))))

/**
  * @brief      config endpoint
  * @param[in]  ep endpoint
  * @param[in]  config config value
  * @retval     None
  */
#define USBD_CONFIG_EP(ep, config)      (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) = (config))

/**
  * @brief      Set buffer for USB endpoint
  * @param[in]  ep endpoint
  * @param[in]  offset buffer offset
  * @retval     None
  */
#define USBD_SET_EP_BUF_ADDR(ep, offset)    (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].BUFSEG + (uint32_t)((ep) << 4))) = (offset))

/**
  * @brief      Get buffer for USB endpoint
  * @param[in]  ep endpoint
  * @retval     buffer offset
  */
#define USBD_GET_EP_BUF_ADDR(ep)        ((uint32_t)*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].BUFSEG + (uint32_t)((ep) << 4))))

/**
  * @brief       Set USB endpoint stall state
  *
  * @param[in]   ep  The USB endpoint ID. 
  *
  * @return      None
  *
  * @details     Set USB endpoint stall state for the specified endpoint ID. Endpoint will respond STALL token automatically.
  *
  */
#define USBD_SET_EP_STALL(ep)        (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) |= USBD_CFG_SSTALL_Msk)

/**
  * @brief       Clear USB endpoint stall state
  *
  * @param[in]   ep  The USB endpoint ID. 
  *
  * @return      None
  *
  * @details     Clear USB endpoint stall state for the specified endpoint ID. Endpoint will respond ACK/NAK token.
  */
#define USBD_CLR_EP_STALL(ep)        (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) &= ~USBD_CFG_SSTALL_Msk)

/**
  * @brief       Get USB endpoint stall state
  *
  * @param[in]   ep  The USB endpoint ID.
  *
  * @retval      0      USB endpoint is not stalled.
  * @retval      Others USB endpoint is stalled.
  *
  * @details     Get USB endpoint stall state of the specified endpoint ID.
  *
  */
#define USBD_GET_EP_STALL(ep)        (*((__IO uint32_t *) ((uint32_t)&USBD->EP[0].CFG + (uint32_t)((ep) << 4))) & USBD_CFG_SSTALL_Msk)

/**
  * @brief      To support byte access between USB SRAM and system SRAM
  *
  * @param[in]  dest Destination pointer.
  *
  * @param[in]  src  Source pointer.
  *
  * @param[in]  size Byte count.
  *
  * @return     None
  *
  * @details    This function will copy the number of data specified by size and src parameters to the address specified by dest parameter.
  *
  */
static __INLINE void USBD_MemCopy(uint8_t *dest, uint8_t *src, int32_t size)
{
    while (size--) *dest++ = *src++;
}


/**
 * @brief       Set USB endpoint stall state
 *
  * @param[in]   epnum  USB endpoint number
 * @return      None
 *
 * @details     Set USB endpoint stall state, endpoint will return STALL token.
 */
static __INLINE void USBD_SetStall(uint8_t epnum)
{
    uint32_t u32CfgAddr;
    uint32_t u32Cfg;
    int i;

    for (i=0; i<USBD_MAX_EP; i++) {
        u32CfgAddr = (uint32_t)(i << 4) + (uint32_t)&USBD->EP[0].CFG; /* USBD_CFG0 */
        u32Cfg = *((__IO uint32_t *) (u32CfgAddr));

        if((u32Cfg & 0xf) == epnum) {
            *((__IO uint32_t *) (u32CfgAddr)) = (u32Cfg | USBD_CFG_SSTALL);
            break;
        }
    }
}

/**
 * @brief       Clear USB endpoint stall state
 *
  * @param[in]   epnum  USB endpoint number
 * @return      None
 *
 * @details     Clear USB endpoint stall state, endpoint will return ACK/NAK token.
 */
static __INLINE void USBD_ClearStall(uint8_t epnum)
{
    uint32_t u32CfgAddr;
    uint32_t u32Cfg;
    int i;

    for (i=0; i<USBD_MAX_EP; i++) {
        u32CfgAddr = (uint32_t)(i << 4) + (uint32_t)&USBD->EP[0].CFG; /* USBD_CFG0 */
        u32Cfg = *((__IO uint32_t *) (u32CfgAddr));

        if((u32Cfg & 0xf) == epnum) {
            *((__IO uint32_t *) (u32CfgAddr)) = (u32Cfg & ~USBD_CFG_SSTALL);
            break;
        }
    }
}

/**
 * @brief       Get USB endpoint stall state
 *
  * @param[in]   epnum  USB endpoint number
 * @retval      0 USB endpoint is not stalled.
 * @retval      non-0 USB endpoint is stalled.
 *
 * @details     Get USB endpoint stall state.
 */
static __INLINE uint32_t USBD_GetStall(uint8_t epnum)
{
    uint32_t u32CfgAddr;
    uint32_t u32Cfg;
    int i;

    for (i=0; i<USBD_MAX_EP; i++) {
        u32CfgAddr = (uint32_t)(i << 4) + (uint32_t)&USBD->EP[0].CFG; /* USBD_CFG0 */
        u32Cfg = *((__IO uint32_t *) (u32CfgAddr));

        if((u32Cfg & 0xf) == epnum)
            break;
    }
    return (u32Cfg & USBD_CFG_SSTALL);
}


/*--------------------------------------------------------------------*/
extern volatile uint8_t g_usbd_RemoteWakeupEn;

typedef void (*VENDOR_REQ)(void); /*!<USB Vendor request callback function */

typedef void (*CLASS_REQ)(void); /*!<USB Class request callback function */

typedef void (*SET_INTERFACE_REQ)(uint32_t u32AltInterface); /*!<USB Standard request "Set Interface" callback function */

/*--------------------------------------------------------------------*/
void USBD_Open(S_USBD_INFO_T *param, CLASS_REQ pfnClassReq, SET_INTERFACE_REQ pfnSetInterface);
void USBD_Start(void);
void USBD_GetSetupPacket(uint8_t *buf);
void USBD_ProcessSetupPacket(void);
void USBD_StandardRequest(void);
void USBD_PrepareCtrlIn(uint8_t *pu8Buf, uint32_t u32Size);
void USBD_CtrlIn(void);
void USBD_PrepareCtrlOut(uint8_t *pu8Buf, uint32_t u32Size);
void USBD_CtrlOut(void);
void USBD_SwReset(void);
void USBD_SetVendorRequest(VENDOR_REQ pfnVendorReq);
void USBD_LockEpStall(uint32_t u32EpBitmap);


/*@}*/ /* end of group NANO100_USBD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_USBD_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */


#endif //__USBD_H__

/*** (C) COPYRIGHT 2013~2014 Nuvoton Technology Corp. ***/
