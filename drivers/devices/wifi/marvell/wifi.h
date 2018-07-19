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
#ifndef __WIFI_H__
#define __WIFI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include "wifi_config.h"

/* Defines ------------------------------------------------------------------*/

/* Wi-Fi Register bit value */
#ifndef _BV
#define _BV(n) (1u << (n))
#endif

// 6.9 Card Common Control Registers (CCCR)
#define SDIO_CCCR_IOEN 0x02
#define SDIO_CCCR_IOEN_IOE1 _BV(1)

#define SDIO_CCCR_IORDY 0x03
#define SDIO_CCCR_IORDY_IOR1 _BV(1)

#define SDIO_CCCR_INTEN 0x04
#define SDIO_CCCR_INTEN_IENM _BV(0)
#define SDIO_CCCR_INTEN_IEN1 _BV(1)

#define SDIO_CCCR_BUSIFCTRL 0x07 // Bus Interface Control
#define SDIO_CCCR_BUSIFCTRL_BUSWID_1Bit 0
#define SDIO_CCCR_BUSIFCTRL_BUSWID_4Bit 0x02
#define SDIO_CCCR_BUSIFCTRL_BUSWID_8Bit 0x03

/* Host Control Registers */
#define WIFI_IOPORT0 0x00
#define WIFI_IOPORT1 0x01
#define WIFI_IOPORT2 0x02

#define WIFI_INTMASK 0x04               // Host Interrupt Mask
#define WIFI_INTMASK_HOSTINTMASK 0x0f   // enable/disable SDU to SD host interrupt

#define WIFI_INTSTATUS 0x05             // Host Interrupt Status
#define WIFI_INTSTATUS_ALL 0x0f
#define WIFI_INTSTATUS_OVERFLOW _BV(3)
#define WIFI_INTSTATUS_UNDERFLOW _BV(2)
#define WIFI_INTSTATUS_DNLD _BV(1)      // Download Host Interrupt Status
#define WIFI_INTSTATUS_UPLD _BV(0)      // Upload Host Interrupt Status 

#define WIFI_SQREADBASEADDR0 0x10
#define WIFI_SQREADBASEADDR1 0x11
#define WIFI_SQREADBASEADDR2 0x12
#define WIFI_SQREADBASEADDR3 0x13

/* Card Control Registers */
#define WIFI_CARDSTATUS 0x20            // Card Status
#define WIFI_CARDSTATUS_IOREADY _BV(3)  // I/O Ready Indicator
#define WIFI_CARDSTATUS_CISCARDRDY _BV(2)   // Card Information Structure Card Ready
#define WIFI_CARDSTATUS_UPLDCARDRDY _BV(1)  // Upload Card Ready (CMD53 read/write command will clear it)
#define WIFI_CARDSTATUS_DNLDCARDRDY _BV(0)  // Download Card Ready

#define WIFI_SCRATCHPAD4_0 0x34
#define WIFI_SCRATCHPAD4_1 0x35

/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/

/* Wi-Fi module enum type */
// Authentication Type to be used to authenticate with AP
typedef enum
{
    WIFI_AUTH_MODE_OPEN = 0x00,
    WIFI_AUTH_MODE_SHARED = 0x01,
    WIFI_AUTH_MODE_NETWORK_EAP = 0x80
} WiFi_AuthenticationType;

// BSS type
typedef enum
{
    WIFI_BSS_INFRASTRUCTURE = 0x01,
    WIFI_BSS_INDEPENDENT = 0x02,
    WIFI_BSS_ANY = 0x03
} WiFi_BSSType;

// Action
typedef enum
{
    WIFI_ACT_GET = 0,
    WIFI_ACT_SET = 1,
    WIFI_ACT_ADD = 2,
    WIFI_ACT_BITWISE_SET = 2,
    WIFI_ACT_BITWISE_CLR = 3,
    WIFI_ACT_REMOVE = 4
} WiFi_CommandAction;

// WiFi Command List
typedef enum
{
    /* Reset and Initialization */
    CMD_802_11_RESET        = 0x0005, // Resets the WLAN device
    CMD_802_11_SNMP_MIB     = 0x0016, // Gets/sets the SNMP MIB
    CMD_802_11_MAC_ADDR     = 0x004D, // WLAN MAC address
    CMD_MAC_MULTICAST_ADR   = 0x0010, // Gets/sets MAC multicast filter table
    CMD_GSPI_BUS_CONFIG     = 0x005A, // Gets/sets the G-SPI Bus mode and time delay betweenhost address write and data read
    CMD_802_11_LDO_CONFIG   = 0x0096, // Gets/sets core voltage source
    CMD_FUNC_INIT           = 0x00A9, // Initialize data structures and hardware blocks and activate threads
    CMD_FUNC_SHUTDOWN       = 0x00AA, // Release resources deactivate hardware blocks and threads, and re-initialize SDIO driver
    CMD_FUNC_IF_CTRL        = 0x00AB, // Power up or down host interfaces used by specific function
    /* MAC/Baseband/RF Control  */
    CMD_MAC_CONTROL             = 0x0028, // Controls hardware MAC
    CMD_802_11_RADIO_CONTROL    = 0x001C, // Controls the radio chip
    CMD_802_11_RF_ANTENNA       = 0x0020, // Gets/sets the Tx and Rx antenna mode
    CMD_802_11_RF_TX_POWER      = 0x001E, // Gets/sets radio transmit power
    CMD_802_11_RF_CHANNEL       = 0x001D, // Gets/sets RF channel
    /* Register and Memory Access */
    CMD_BBP_REG_ACCESS  = 0x001A, // Peeks and pokes baseband processor hardware register
    CMD_RF_REG_ACCESS   = 0x001B, // Peeks and pokes RF hardware register
    CMD_MAC_REG_ACCESS  = 0x0019, // Peeks and pokes MAC hardware register
    CMD_EEPROM_ACCESS   = 0x0059, // Retrieves the EEPROM data
    CMD_PMIC_REG_ACCESS = 0x00AC, // Pokes Marvell power management device registers
    /* RF Calibration Data */
    CMD_CFG_DATA    = 0x008F, // Download new optimized register settings or calibrating data
    /* Status Information */
    CMD_GET_HW_SPEC     = 0x0003, // Gets hardware specifications
    CMD_802_11_GET_LOG  = 0x000B, // Gets the WLAN log
    CMD_GET_TSF         = 0x0080, // Retrieves the current TSF value
    CMD_TX_PKT_STATS    = 0x008D, // Reports the current Tx packet status
    CMD_802_11_AVG_RATE_INFO    = 0x00A3, // Gets Tx or Rx last successful rate and average rate
    CMD_802_11_RSSI_INFO        = 0x00A4, // Gets received signal strength
    /* LED Control */
    CMD_802_11_LED_CONTROL  = 0x004E, // LED Control
    /* Scan */
    CMD_802_11_SCAN             = 0x0006, // Starts the scan process
    CMD_802_11_BG_SCAN_CONFIG   = 0x006B, // Gets/sets background scan configuration
    CMD_802_11_BG_SCAN_QUERY    = 0x006C, // Gets background scan results
    /* Network Start/Stop/Join */
    CMD_802_11_ASSOCIATE        = 0x0012, // Initiate an association with the AP
    CMD_802_11_AD_HOC_START     = 0x002b, // Starts an Ad-Hoc network
    CMD_802_11_AD_HOC_JOIN      = 0x002c, // Join an Ad-Hoc network
    CMD_802_11_AD_HOC_STOP      = 0x0040, // Stops Ad-Hoc Network
    CMD_802_11_IBSS_COALESCING_STATUS = 0x0083, // Configures IBSS coalescing settings
    /* Security */
    CMD_802_11_DEAUTHENTICATE   = 0x0024, // Starts de-authentication process with the AP
    CMD_802_11_SET_WEP          = 0x0013, // Configures the WEP keys
    CMD_802_11_KEY_MATERIAL     = 0x005E, // Gets/sets key material used to do Tx encryption or Rx decryption
    CMD_802_11_CRYPTO           = 0x0078, // Provides the AES ciher engine API between firmware and driver
    /* Rate Adaptation*/
    CMD_802_11_RATE_ADAPT_RATESET = 0x0076, // Gets/sets transmit data rate
    CMD_TX_RATE_QUERY             = 0x007F, // Reports the current Tx rate
    /* Transmit Power Control */
    CMD_802_11_TPC_CFG              = 0x0072, // Controls TPC functionality
    CMD_802_11_POWER_ADAPT_CFG_EXT  = 0x0073, // Configures power adaptation related settings
    /* Event Subscription */
    CMD_802_11_SUBSCRIBE_EVENT = 0x0075, // Subscribe to events and set thresholds
    /* Power Management */
    CMD_802_11_PS_MODE      = 0x0021, // Gets/sets power save mode
    CMD_802_11_SLEEP_PARAMS = 0x0066, // Gets/sets sleep parameters
    CMD_802_11_HOST_SLEEP_CFG   = 0x0043, // Configures the wakeup semantics of the host driver
    CMD_802_11_WAKEUP_CONFIRM   = 0x0044, // Sends a host awake event
    CMD_802_11_INACTIVITY_TIMEOUT_EXT = 0x008A, // Gets/sets inactivity timeout enhancement value
    CMD_802_11_FW_WAKE_METHOD   = 0x0074, // Firmware wake method
    CMD_802_11_AUTO_TX          = 0x0082, // Configures auto transmit settings
    CMD_802_11_HOST_SLEEP_ACTIVATE = 0x0045, // Sets the host to sleep
    CMD_SDIO_GPIO_INT_CONFIG    = 0x0088, // Configures SDIO/GPIO host interface interrupt
    CMD_802_11_DEEP_SLEEP       = 0x003E, // Initiates deep sleep mode
    CMD_802_11_SLEEP_PERIOD     = 0x0068, // Sleep period command
    /* Bluetooth Coexistence */
    CMD_802_11_BCA_CONFIG_TIMESHARE = 0x0069, // Configures the BCA timeshare interval and percentage
    /* WMM */
    CMD_WMM_GET_STATUS      = 0x0071, // Retrieves the current WMM state
    CMD_WMM_ADDTS_REQ       = 0x006E, // Sends an ADDTS frame to the associated AP
    CMD_WMM_DELTS_REQ       = 0x006F, // Sends a DELTS to the associated AP
    CMD_WMM_QUEUE_CONFIG    = 0x0070, // Gets,sets,or defaults the configuration of a WMM queue
    CMD_WMM_QUEUE_STATS     = 0x0081, // Manages the WMM AC queues statistics collection
    CMD_WMM_QUEUE_GET_TS_STATS = 0x005D, // Retrieves current state of a given traffic stream(TSID) from the firmware
    /* Wi-Fi Protected Setup*/
    CMD_WPS_ADHOC_REGISTRAR_CONFIG = 0x0084, // Configures WPS settings
    /* 802.11d */
    CMD_802_11D_DOMAIN_INFO = 0x005B, // Gets/sets 802.11d domain information
    CMD_802_11_RGN_CODE     = 0x004C, // Gets/sets region code stored in the EEPROM
    /* 802.11h */
    CMD_802_11_MEASUREMENT_REQUEST  = 0x0062, // Sends measurement request
    CMD_802_11_GET_MEASUREMENT_REPORT = 0x0063, // Gets measurement response report frame
    CMD_802_11_CHAN_SW_ANN          = 0x0061, // Broadcasts a channel switch announcement
    CMD_802_11_TPC_INFO             = 0x005F, // Gets TPC information
    CMD_802_11_TPC_ADAPT_REQ        = 0x0060, // Requests TPC report
    /* Memory Efficient Filtering */
    CMD_MEF_CFG = 0x009A, // Sets up firmware filtering options
    /* Small Debug Print */
    CMD_DBGS_CFG = 0x008B, // Configures smalldebug print facility
    CMD_GET_MEM = 0x008C // Retriene firmware vitals or trace memory     
} WiFi_CommandList;

// WiFi Command response status code
typedef enum
{
    CMD_STATUS_SUCCESS      = 0x0000, // No error
    CMD_STATUS_ERROR        = 0x0001, // Command failed
    CMD_STATUS_UNSUPPORTED  = 0x0002 // Command is not supported
} WiFi_CommandResult;

// WiFi Key Type
typedef enum
{
    WIFI_KEYTYPE_WEP = 0,
    WIFI_KEYTYPE_TKIP = 1,
    WIFI_KEYTYPE_AES = 2
} WiFi_KeyType;

// Table 45: IEEE 802.11 Standard IE Translated to Marvell IE
typedef enum
{
    WIFI_MRVLIETYPES_SSIDPARAMSET   = 0x0000,
    WIFI_MRVLIETYPES_RATESPARAMSET  = 0x0001,
    WIFI_MRVLIETYPES_PHYPARAMDSSET  = 0x0003,
    WIFI_MRVLIETYPES_CFPARAMSET     = 0x0004,
    WIFI_MRVLIETYPES_IBSSPARAMSET   = 0x0006,
    WIFI_MRVLIETYPES_RSNPARAMSET    = 0x0030,
    WIFI_MRVLIETYPES_VENDORPARAMSET = 0x00dd,
    WIFI_MRVLIETYPES_KEYPARAMSET    = 0x0100,
    WIFI_MRVLIETYPES_CHANLISTPARAMSET = 0x0101,
    WIFI_MRVLIETYPES_TSFTIMESTAMP   = 0x0113,
    WIFI_MRVLIETYPES_AUTHTYPE       = 0x011f
} WiFi_MrvlIETypes;

// SDIO Frame Type
typedef enum
{
    WIFI_SDIOFRAME_DATA = 0x00,
    WIFI_SDIOFRAME_COMMAND = 0x01,
    WIFI_SDIOFRAME_EVENT = 0x03
} WiFi_SDIOFrameType;

// 16.5 SDIO Card Metaformat
typedef enum
{
    CISTPL_NULL = 0x00, // Null tuple
    CISTPL_VERS_1 = 0x15, // Level 1 version/product-information
    CISTPL_MANFID = 0x20, // Manufacturer Identification String Tuple
    CISTPL_FUNCID = 0x21, // Function Identification Tuple
    CISTPL_FUNCE = 0x22, // Function Extensions
    CISTPL_END = 0xff // The End-of-chain Tuple
} WiFi_SDIOTupleCode;

// Security Type
typedef enum
{
    WIFI_SECURITYTYPE_NONE = 0,
    WIFI_SECURITYTYPE_WEP = 1,
    WIFI_SECURITYTYPE_WPA = 2,
    WIFI_SECURITYTYPE_WPA2 = 3
} WiFi_SecurityType;

// Callback Status
typedef enum
{
    WIFI_STATUS_OK = 0,         // Received successful response
    WIFI_STATUS_FAIL = 1,       // Failed to complete the requested operation (eg: associate AP failed)
    WIFI_STATUS_BUSY = 2,       // The previous operation has not been completed
    WIFI_STATUS_NORESP = 3,     // No response was received after several retransmission
    WIFI_STATUS_MEM = 4,        // Out of Memory
    WIFI_STATUS_INVALID = 5,    // Invalid paramete
    WIFI_STATUS_NOTFOUND = 6,   // Target not found (eg: AP)
    WIFI_STATUS_INPROGRESS = 7  // The command is executed successfully, but subsequent operations are required
} WiFi_Status;

// WEP Key Type
typedef enum
{
    WIFI_WEPKEYTYPE_40BIT = 1,
    WIFI_WEPKEYTYPE_104BIT = 2
} WiFi_WEPKeyType;

/* Callback function */
/* data : NULL , no response */
typedef void (*WiFi_Callback)(void *arg, void *data, WiFi_Status status); 

/* WiFi Command bit */
// Capability information
#define WIFI_CAPABILITY_ESS                 _BV(0)
#define WIFI_CAPABILITY_IBSS                _BV(1)
#define WIFI_CAPABILITY_CF_POLLABLE         _BV(2)
#define WIFI_CAPABILITY_CF_POLL_REQUEST     _BV(3)
#define WIFI_CAPABILITY_PRIVACY             _BV(4)
#define WIFI_CAPABILITY_SHORT_PREAMBLE      _BV(5)
#define WIFI_CAPABILITY_PBCC                _BV(6)
#define WIFI_CAPABILITY_CHANNEL_AGILITY     _BV(7)
#define WIFI_CAPABILITY_SPECTRUM_MGMT       _BV(8)
#define WIFI_CAPABILITY_SHORT_SLOT          _BV(10)
#define WIFI_CAPABILITY_DSSS_OFDM           _BV(13)

#define WIFI_KEYINFO_KEYENABLED             _BV(2)
#define WIFI_KEYINFO_UNICASTKEY             _BV(1)
#define WIFI_KEYINFO_MULTICASTKEY           _BV(0)

#define WIFI_MACCTRL_RX                     _BV(0)
#define WIFI_MACCTRL_TX                     _BV(1) 
#define WIFI_MACCTRL_LOOPBACK               _BV(2)
#define WIFI_MACCTRL_WEP                    _BV(3)
#define WIFI_MACCTRL_ETHERNET2              _BV(4)
#define WIFI_MACCTRL_PROMISCUOUS            _BV(7)
#define WIFI_MACCTRL_ALLMULTICAST           _BV(8)
#define WIFI_MACCTRL_ENFORCEPROTECTION      _BV(10) // strict protection
#define WIFI_MACCTRL_ADHOCGPROTECTIONMODE   _BV(13) // 802.11g protection mode


/* Macros -------------------------------------------------------------------*/
// Given the sizeof the structure, sizeof(TLV), the sizeof the data domain is calculated, 
// which is generally used to assign a value to header.length
// For example, define an MrvlIETypes_CfParamSet_t param variable, 
//          assign param.header.length=TLV_PAYLOADLEN(param)
#define TLV_PAYLOADLEN(tlv) (sizeof(tlv) - sizeof((tlv).header))

// Given the size of the data domain, find the size of the entire structure
// eg: define a large buffer, and then define a pointer p to an IEEEType * that points to the buffer
//     After the buffer receives the data, it is obvious that the actual sizeof the received IEEEType data cannot be sizeof(IEEEType), 
//     because the length of the data is defined as 1 when defining the IEEEType structure body
// You should use TLV_STRUCTLEN(*p)
#define TLV_STRUCTLEN(tlv) (sizeof((tlv).header) + (tlv).header.length)

// Given the address and size of the TLV, find the address of the next TLV
#define TLV_NEXT(tlv) ((uint8_t *)(tlv) + TLV_STRUCTLEN(*(tlv)))

// byte-order Conversion functions
#ifndef htons
#define htons(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#endif
#ifndef ntohs
#define ntohs htons
#endif

#define WiFi_GetCommandCode(data) (((data) == NULL) ? 0 : (((const WiFi_CommandHeader *)(data))->cmd_code & 0x7fff))
#define WiFi_IsCommandResponse(data) ((data) != NULL && ((const WiFi_CommandHeader *)(data))->cmd_code & 0x8000)

/* TLV (Tag Length Value) of IEEE IE Type Format */
typedef __PACKED_STRUCT 
{
    uint8_t type;
    uint8_t length; // payload size
} IEEEHeader;

// information element parameter
// The base of IEEETypes_* type
typedef __PACKED_STRUCT
{
    IEEEHeader header;
    uint8_t data[1];
} IEEEType;

typedef __PACKED_STRUCT
{
    IEEEHeader header;
    uint8_t channel;
} IEEETypes_DsParamSet_t;

typedef __PACKED_STRUCT
{
    IEEEHeader header;
    uint16_t atim_window;
} IEEETypes_IbssParamSet_t;

/* TLV (Tag Length Value) of MrvllEType Format */
typedef __PACKED_STRUCT
{
    uint16_t type;
    uint16_t length;
} MrvlIEHeader;

// The base of MrvlIETypes_* type
typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t data[1];
} MrvlIEType;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint16_t auth_type;
} MrvlIETypes_AuthType_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t count;
    uint8_t period;
    uint16_t max_duration;
    uint16_t duration_remaining;
} MrvlIETypes_CfParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    __PACKED_STRUCT
    {
        uint8_t band_config_type;
        uint8_t chan_number;
        uint8_t scan_type;
        uint16_t min_scan_time;
        uint16_t max_scan_time;
    } channels[1];
} MrvlIETypes_ChanListParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint16_t key_type_id;
    uint16_t key_info;
    uint16_t key_len;
    uint8_t key[32];
} MrvlIETypes_KeyParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t channel;
} MrvlIETypes_PhyParamDSSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t rates[14];
} MrvlIETypes_RatesParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t rsn[64];
} MrvlIETypes_RsnParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t ssid[32];
} MrvlIETypes_SSIDParamSet_t;

typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint64_t tsf_table[1];
} MrvlIETypes_TsfTimestamp_t;

// the max size of this struct is 256 bytes
typedef __PACKED_STRUCT
{
    MrvlIEHeader header;
    uint8_t vendor[64]; // 64 enough
} MrvlIETypes_VendorParamSet_t;

/* WiFi command and data frame type */
// Header of all frame
typedef __PACKED_STRUCT
{
    uint16_t length; // include itself
    uint16_t type;
} WiFi_SDIOFrameHeader;

// Header of Command frame
typedef __PACKED_STRUCT
{
    WiFi_SDIOFrameHeader frame_header;
    uint16_t cmd_code;
    uint16_t size;
    uint16_t seq_num;
    uint16_t result;
} WiFi_CommandHeader;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint8_t bssid[6]; // MAC address
    uint8_t ssid[32];
    uint8_t bss_type;
    uint16_t bcn_period;
    uint8_t dtim_period; // Specify DTIM period (TBTTs)
    uint8_t timestamp[8];
    uint8_t start_ts[8]; // Starting timestamp
    IEEETypes_DsParamSet_t ds_param_set; // IEEE DS parameter set element
    uint32_t reserved1;
    IEEETypes_IbssParamSet_t ibss_param_set; // IEEE IBSS parameter set
    uint32_t reserved2;
    uint16_t cap_info;
    uint8_t data_rates[14];
    uint32_t reserved3;
} WiFi_Cmd_ADHOCJoin;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint8_t ssid[32];
    uint8_t bss_type;
    uint16_t bcn_period;
    uint8_t reserved1;
    IEEETypes_IbssParamSet_t ibss_param_set; // ATIM window length in TU
    uint32_t reserved2;
    IEEETypes_DsParamSet_t ds_param_set; // The channel for ad-hoc network
    uint8_t reserved3[6];
    uint16_t cap_info; // Capability information
    uint8_t data_rate[14];
} WiFi_Cmd_ADHOCStart;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint8_t peer_sta_addr[6];
    uint16_t reason_code; // Reason code defined in IEEE 802.11 specification section 7.3.1.7 to indicate de-authentication reason
} WiFi_Cmd_Deauthenticate;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t action;
    MrvlIETypes_KeyParamSet_t keys;
} WiFi_Cmd_KeyMaterial;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t action;
    uint8_t mac_addr[6];
} WiFi_Cmd_MACAddr;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t action;
    uint16_t reserved;
} WiFi_Cmd_MACCtrl;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t action;
    uint16_t tx_key_index; // Key set being used for transmit (0~3)
    uint8_t wep_types[4]; // use 40 or 104 bits
    uint8_t keys[4][16];
} WiFi_Cmd_SetWEP;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint8_t peer_sta_addr[6]; // Peer MAC address
    uint16_t cap_info; // Capability information
    uint16_t listen_interval; // Listen interval
    uint16_t bcn_period; // Beacon period
    uint8_t dtim_period; // DTIM period
} WiFi_CmdRequest_Associate;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint8_t bss_type;
    uint8_t bss_id[6];
} WiFi_CmdRequest_Scan;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t capability;
    uint16_t status_code;
    uint16_t association_id;
    IEEEType ie_buffer;
} WiFi_CmdResponse_Associate;

typedef __PACKED_STRUCT
{
    WiFi_CommandHeader header;
    uint16_t buf_size;
    uint8_t num_of_set;
} WiFi_CmdResponse_Scan;

// WiFi Rx Data Frame
// Table 2: Fields in Receive Packet Descriptor
typedef __PACKED_STRUCT
{
    WiFi_SDIOFrameHeader header;
    uint16_t reserved1;
    int8_t snr; // Signal to noise ratio for this packet (dB)
    uint8_t reserved2;
    uint16_t rx_packet_length; // Number of bytes in the payload
    uint8_t nf; // Noise floor for this packet (dBm). Noise floor is always negative. The absolute value is passed.
    uint8_t rx_rate; // Rate at which this packet is received
    uint32_t rx_packet_offset; // Offset from the start of the packet to the beginning of the payload data packet
    uint32_t reserved3;
    uint8_t priority; // Specifies the user priority of received packet
    uint8_t reserved4[3];
    uint8_t payload[1]; // link layer
} WiFi_DataRx;

// WiFi Tx Data Frame
// Table 3: Fields in Transmit Packet Descriptor
typedef __PACKED_STRUCT
{
    WiFi_SDIOFrameHeader header;
    uint32_t reserved1;
    uint32_t tx_control; // See 3.2.1 Per-Packet Settings
    uint32_t tx_packet_offset; // Offset of the beginning of the payload data packet (802.3 or 802.11 frames) from the beginning of the packet (bytes)
    uint16_t tx_packet_length; // Number of bytes in the payload data frame
    uint16_t tx_dest_addr_high; // Destination MAC address bytes 4 to 5
    uint32_t tx_dest_addr_low; // Destination MAC address bytes 0 to 3
    uint8_t priority; // Specifies the user priority of transmit packet
    uint8_t flags;
    uint8_t pkt_delay_2ms; // Amount of time the packet has been queued in the driver layer for WMM implementations
    uint8_t reserved2;
    uint8_t payload[1]; // link layer
} WiFi_DataTx;

// EAPOL (EAP(Extensible Authentication Protocol) OVER LAN)
// https://www.vocal.com/secure-communication/eapol-extensible-authentication-protocol-over-lan/
typedef __PACKED_STRUCT
{
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
    uint8_t version;
    uint8_t packet_type;
    uint16_t packet_body_length; // big endian

    /* packet body */
    // 802.11-2016.pdf: Figure 12-32 EAPOL-Key frame
    // http://etutorials.org/Networking/802.11+security.+wi-fi+protected+access+and+802.11i/Part+II+The+Design+of+Wi-Fi+Security/Chapter+10.+WPA+and+RSN+Key+Hierarchy/Details+of+Key+Derivation+for+WPA/
    uint8_t descriptor_type;
    uint16_t key_information;
    uint16_t key_length;
    uint8_t key_replay_counter[8];
    uint8_t key_nonce[32];
    uint8_t key_iv[16];
    uint8_t key_rsc[8]; // receive sequence counter
    uint8_t reserved[8]; // not used in WPA
    uint8_t key_mic[16];
    uint16_t key_data_length;
    uint8_t key_data[1];
} WiFi_EAPOLKeyFrame;

// WiFi EVENT
typedef __PACKED_STRUCT
{
    WiFi_SDIOFrameHeader header;
    uint32_t event_id; // Enumerated identifier for the event
    uint16_t reason_code; // IEEE Reason Code as described in the 802.11 standard
    uint8_t mac_addr[6]; // Peer STA Address
} WiFi_Event;


typedef __PACKED_STRUCT
{
    uint16_t ie_length; // Total information element length (不含sizeof(ie_length))
    uint8_t bssid[6]; // BSSID
    uint8_t rssi; // RSSI value as received from peer

    // Probe Response/Beacon Payload
    uint64_t pkt_time_stamp; // Timestamp
    uint16_t bcn_interval; // Beacon interval
    uint16_t cap_info; // Capabilities information
    IEEEType ie_parameters; // some IEEEType data
} WiFi_BssDescSet;

typedef struct
{
    WiFi_SecurityType security;
    char *ssid;
    void *password;
} WiFi_Connection;

typedef __PACKED_STRUCT
{
    uint8_t TK[16];
    uint8_t TKIPTxMICKey[8];
    uint8_t TKIPRxMICKey[8];
} WiFi_GTK;

typedef __PACKED_STRUCT
{
    uint8_t type;
    uint8_t length;
    uint8_t oui[3];
    uint8_t data_type;
    uint8_t data[1];
} WiFi_KDE;

typedef __PACKED_STRUCT
{
    uint8_t MIC[16];
    uint8_t RESERVED[4]; // for sha1
} WiFi_MIC;

typedef __PACKED_STRUCT
{
    uint8_t KCK[16];
    uint8_t KEK[16];
    uint8_t TK[16];
    uint8_t TKIPTxMICKey[8];
    uint8_t TKIPRxMICKey[8];
} WiFi_PTK;

typedef __PACKED_STRUCT
{
    uint8_t MAC[2][6];
    uint8_t nonce[2][32];
} WiFi_PTKB;

// WiFi AP info
typedef struct
{
    MrvlIETypes_SSIDParamSet_t ssid;
    uint8_t mac_addr[6];
    uint16_t cap_info;
    uint16_t bcn_period;
    uint8_t channel;
    MrvlIETypes_RatesParamSet_t rates;
    MrvlIETypes_RsnParamSet_t rsn;
    MrvlIETypes_VendorParamSet_t wpa;
    MrvlIETypes_VendorParamSet_t wwm;
    MrvlIETypes_VendorParamSet_t wps;
} WiFi_SSIDInfo;

typedef struct
{
    WiFi_Callback callback;
    void *arg;
    uint8_t busy;
    uint8_t ready; // unused by data buffer
    uint8_t retry; // unused by data buffer
    uint32_t start_time;
    uint32_t timeout;
} WiFi_TxBuffer;

typedef __PACKED_STRUCT
{
    uint8_t oui[3];
    uint8_t oui_type;
    uint16_t version;
    uint8_t multicast_oui[4];
    uint16_t unicast_num;
    uint8_t unicast_oui[1][4];  // suppose unicast_num=1
    uint16_t auth_num;          // when unicast_num=1, the field is valid
    uint8_t auth_oui[1][4];
} WiFi_Vendor;

typedef struct
{
    char *keys[4];
    uint8_t index; // 0~3
} WiFi_WEPKey;


/* Functions API ------------------------------------------------------------*/

/* WiFi lowlevel function */
uint8_t WiFi_LowLevel_GetFunctionNum(void);
void WiFi_LowLevel_Init(void);
uint8_t WiFi_LowLevel_ReadData(uint8_t func, uint32_t addr, void *data, uint32_t size, uint32_t bufsize);
uint8_t WiFi_LowLevel_ReadReg(uint8_t func, uint32_t addr);
void WiFi_LowLevel_SetBlockSize(uint8_t func, uint32_t size);
uint8_t WiFi_LowLevel_WriteData(uint8_t func, uint32_t addr, const void *data, uint32_t size, uint32_t bufsize);
uint8_t WiFi_LowLevel_WriteReg(uint8_t func, uint32_t addr, uint8_t value);

/* WiFi major function */
void WiFi_Associate(const char *ssid, WiFi_AuthenticationType auth_type, WiFi_Callback callback, void *arg);
void WiFi_AssociateEx(const WiFi_Connection *conn, WiFi_AuthenticationType auth_type, int8_t max_retry, WiFi_Callback callback, void *arg);
void WiFi_CheckTimeout(void);
void WiFi_Deauthenticate(uint16_t reason, WiFi_Callback callback, void *arg);
void WiFi_DiscardData(void);
uint8_t WiFi_GetBSSID(uint8_t mac_addr[6]);
uint16_t WiFi_GetDataLength(void);
void WiFi_GetMACAddress(WiFi_Callback callback, void *arg);
uint8_t *WiFi_GetPacketBuffer(void);
const uint8_t *WiFi_GetReceivedPacket(uint16_t *len);
WiFi_SecurityType WiFi_GetSecurityType(const WiFi_SSIDInfo *info);
void WiFi_Init(void);
void WiFi_Input(void);
uint8_t WiFi_IsCommandBusy(void);
void WiFi_JoinADHOC(const char *ssid, WiFi_Callback callback, void *arg);
void WiFi_JoinADHOCEx(const WiFi_Connection *conn, int8_t max_retry, WiFi_Callback callback, void *arg);
void WiFi_KeyMaterial(WiFi_CommandAction action, MrvlIETypes_KeyParamSet_t *key, uint8_t key_count, WiFi_Callback callback, void *arg);
void WiFi_MACAddr(const uint8_t newaddr[6], WiFi_CommandAction action, WiFi_Callback callback, void *arg);
void WiFi_MACControl(uint16_t action, WiFi_Callback callback, void *arg);
void WiFi_Scan(WiFi_Callback callback, void *arg);
void WiFi_ScanSSID(const char *ssid, WiFi_SSIDInfo *info, WiFi_Callback callback, void *arg);
void WiFi_SendCommand(uint16_t code, const void *data, uint16_t size, WiFi_Callback callback, void *arg, uint32_t timeout, uint8_t max_retry);
void WiFi_SendPacket(void *data, uint16_t size, WiFi_Callback callback, void *arg, uint32_t timeout);
void WiFi_SetWEP(WiFi_CommandAction action, const WiFi_WEPKey *key, WiFi_Callback callback, void *arg);
void WiFi_SetWPA(const char *ssid, const char *password);
void WiFi_ShowCIS(void);
void WiFi_StartADHOC(const char *ssid, uint16_t cap_info, WiFi_Callback callback, void *arg);
void WiFi_StartADHOCEx(const WiFi_Connection *conn, WiFi_Callback callback, void *arg);
void WiFi_StopADHOC(WiFi_Callback callback, void *arg);
uint8_t WiFi_TranslateTLV(MrvlIEType *mrvlie_tlv, const IEEEType *ieee_tlv, uint16_t mrvlie_payload_size);
uint8_t WiFi_Wait(uint8_t status, uint32_t timeout);
void WiFi_WaitForLastTask(void);

/* Callback function interface */
void WiFi_AuthenticationCompleteHandler(void);
void WiFi_EventHandler(const WiFi_Event *event);
void WiFi_PacketHandler(const WiFi_DataRx *data);

/* format output */
void WiFi_DumpData(const void *data, uint32_t len);
void WiFi_OutputBuffer(const uint8_t *buffer, int length);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __WIFI_H__ */

