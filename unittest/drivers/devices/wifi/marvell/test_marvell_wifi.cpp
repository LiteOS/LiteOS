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

/* Includes -----------------------------------------------------------------*/
#include "test_marvell_wifi.h"

extern "C"{
#include "wifi.h"

#include "compat-1.3.h"
#include "arc4.h"
#include "md5.h"
#include "sha1.h"
#include "wpa.h"
}

#include <string.h>
#include <stdio.h>
/* Defines ------------------------------------------------------------------*/
#define TEST_SSID       "TP-LINK_4CEAC8"
#define TEST_PASSWORD   "123456789"

typedef enum 
{
    TEST_FLAG_NONE = 0x00,
    TEST_FLAG_GET_MAC,
    TEST_FLAG_SCAN,
    TEST_FLAG_MAC_CONTROL,
    TEST_FLAG_SCAN_SSID,
    TEST_FLAG_SCAN_SSID_FAILED,
    TEST_FLAG_ASSOCIATE,
    
    TEST_FLAG_DISCARD,
    TEST_FLAG_GET_DATA_LENGTH,

    TEST_FLAG_GET_RECEIVED_PACKET,
    TEST_FLAG_WAIT_TIMEOUT,
    TEST_FLAG_SET_WEP,
    TEST_FLAG_KEY_MATERIAL,
    TEST_FLAG_DEAUTHENTICATE,

    TEST_FLAG_SET_KEY_MATERIAL,
    
    TEST_FLAG_JOIN_ADHOCEX,
    TEST_FLAG_JOIN_ADHOC_MAC_CONTROL,
    TEST_FLAG_JOIN_ADHOC_SCAN_SSID,
    TEST_FLAG_JOIN_ADHOC,
    TEST_FLAG_AD_HOC_JOIN,
    TEST_FLAG_MAC_ADDR,
    TEST_FLAG_START_ADHOC,

    TEST_FLAG_CISTPL_VERS_1,
    TEST_FLAG_CISTPL_MANFID,
    TEST_FLAG_CISTPL_FUNCID,
    TEST_FLAG_CISTPL_FUNCE0,
    TEST_FLAG_CISTPL_FUNCE1,
    TEST_FLAG_CISTPL_END,

    TEST_FLAG_START_ADHOCEX,
    TEST_FLAG_START_ADHOC_MAC_CONTROL,

    TEST_FLAG_EVENT,

    TEST_FLAG_RESPONSE_PACKET,
    TEST_FLAG_EAPOL,
    TEST_FLAG_EAPOL_INVALID_FRAME,
    TEST_FLAG_EAPOL_KEYTYPE_AES,
    TEST_FLAG_EAPOL_MSG1_P,
    TEST_FLAG_EAPOL_MSG1_P_RSN,
    TEST_FLAG_EAPOL_MSG1_P_WPA,
    TEST_FLAG_EAPOL_MSG1_G,
    TEST_FLAG_EAPOL_MSG3_WPA,
    TEST_FLAG_EAPOL_MSG3_WPA2
    
}TEST_FLAG;

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static uint8_t s_ucRetRegRead = 0;
static uint8_t s_ucRetRegWrite = 0;
static uint8_t s_ucRetDataRead = 1;
static uint8_t s_ucRetDataWrite = 1;
static uint8_t s_ucFunctionNum = 0;
static uint8_t s_isBoot = 0;
static uint8_t s_cmd_busy_flag = 0;
static uint8_t s_check_mic_flag = 1;
static uint16_t s_prf_err_type = 0;

static uint8_t s_test_flag = TEST_FLAG_NONE;

static uint8_t s_wifi_buffer_command[256]; 
static uint8_t s_wifi_buffer_packet[1792]; 
static uint8_t s_wifi_buffer_rx[2048];

static WiFi_SDIOFrameHeader *rx_header = (WiFi_SDIOFrameHeader *)s_wifi_buffer_rx;
static WiFi_DataRx *rx_packet = (WiFi_DataRx *)s_wifi_buffer_rx;
static WiFi_CommandHeader *rx_cmd = (WiFi_CommandHeader *)s_wifi_buffer_rx;

static WiFi_CommandHeader *tx_cmd = (WiFi_CommandHeader *)s_wifi_buffer_command;

static WiFi_SDIOFrameHeader *tx_header = (WiFi_SDIOFrameHeader *)s_wifi_buffer_packet;
static WiFi_DataRx *tx_packet = (WiFi_DataRx *)s_wifi_buffer_packet;


uint8_t mac[] = {0x00, 0x27, 0x13, 0x7C, 0x1F, 0x54};

WiFi_MIC wifi_mic = { {0}, {0} };

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
extern "C"
{

extern uint32_t sys_now(void);

uint8_t WiFi_LowLevel_GetFunctionNum(void)
{
    return s_ucFunctionNum;
}
void WiFi_LowLevel_Init(void)
{
    
}

static void wifi_response_mac_address(void *data, uint32_t size)
{
    WiFi_Cmd_MACAddr *cmd = (WiFi_Cmd_MACAddr *)s_wifi_buffer_rx;
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_MAC_ADDR;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;
    memcpy(cmd->mac_addr, mac, 6);
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_scan(void *data, uint32_t size)
{
    int i;

    uint16_t ie_size;
    WiFi_CmdResponse_Scan *resp = (WiFi_CmdResponse_Scan *)s_wifi_buffer_rx;
    WiFi_BssDescSet *bss_desc_set;
    IEEEType *ie_params;

//    rx_header->length = tx_cmd->frame_header.length;
    rx_header->length = sizeof(WiFi_CmdResponse_Scan);
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_SCAN;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    resp->buf_size = 0;
    resp->num_of_set = 1;
    bss_desc_set = (WiFi_BssDescSet *)(resp + 1);
    for (i = 0; i < resp->num_of_set; i++)
    {
        memcpy(bss_desc_set->bssid, mac, 6);
        bss_desc_set->rssi = 44;
        bss_desc_set->pkt_time_stamp = 123456;
        bss_desc_set->bcn_interval = 100;
        bss_desc_set->cap_info = 0x0531;
        
        ie_size = 0;
        
        char* ssid = (char*)"TP-LINK_80D8BE";
        ie_params = (IEEEType *)&bss_desc_set->ie_parameters;
        ie_params->header.type = WIFI_MRVLIETYPES_SSIDPARAMSET;
        ie_params->header.length = strlen(ssid);
        memcpy(ie_params->data, ssid, strlen(ssid));
        
        ie_size += TLV_STRUCTLEN(*ie_params);
        
        ie_params = (IEEEType *)TLV_NEXT(ie_params);
        ie_params->header.type = WIFI_MRVLIETYPES_PHYPARAMDSSET;
        ie_params->header.length = 1;
        ie_params->data[0] = 1;

        
        ie_size += TLV_STRUCTLEN(*ie_params);
        
        ie_params = (IEEEType *)TLV_NEXT(ie_params);
        ie_params->header.type = WIFI_MRVLIETYPES_RSNPARAMSET;
        ie_params->header.length = 1;
        ie_params->data[0] = WIFI_SECURITYTYPE_WPA2;
        
        ie_size += TLV_STRUCTLEN(*ie_params);
        
        ie_params = (IEEEType *)TLV_NEXT(ie_params);
        WiFi_Vendor vendor;
        memset(&vendor, 0, sizeof(WiFi_Vendor));
        vendor.oui[0] = 0x00;
        vendor.oui[1] = 0x50;
        vendor.oui[2] = 0xf2;
        vendor.oui_type = 0x01;
        ie_params->header.type = WIFI_MRVLIETYPES_VENDORPARAMSET;
        ie_params->header.length = sizeof(WiFi_Vendor);
        memcpy(ie_params->data, &vendor, ie_params->header.length);
        
        ie_size += TLV_STRUCTLEN(*ie_params);

        bss_desc_set->ie_length = (sizeof(WiFi_BssDescSet) - sizeof(bss_desc_set->ie_length) - sizeof(bss_desc_set->ie_parameters)) + ie_size; 
        rx_header->length += bss_desc_set->ie_length + sizeof(bss_desc_set->ie_length);
        
        bss_desc_set = (WiFi_BssDescSet *)((uint8_t *)bss_desc_set + sizeof(bss_desc_set->ie_length) + bss_desc_set->ie_length);
    }
    
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_mac_control(void *data, uint32_t size)
{
    WiFi_Cmd_MACCtrl *cmd = (WiFi_Cmd_MACCtrl *)s_wifi_buffer_rx;
    WiFi_Cmd_MACCtrl *tx_cmd_mac_control = (WiFi_Cmd_MACCtrl *)tx_cmd;
    rx_header->length = tx_cmd->frame_header.length-1;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_MAC_CONTROL;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;
    cmd->action = tx_cmd_mac_control->action;
    
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_scan_ssid(void *data, uint32_t size)
{
    wifi_response_scan(data, size);
}

static void wifi_response_scan_ssid_failed(void *data, uint32_t size)
{
    WiFi_CmdResponse_Scan *resp = (WiFi_CmdResponse_Scan *)s_wifi_buffer_rx;
    
    rx_header->length = sizeof(WiFi_CmdResponse_Scan);
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_SCAN;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    resp->num_of_set = 0;
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_associate(void *data, uint32_t size)
{ 
    WiFi_CmdResponse_Associate *resp = (WiFi_CmdResponse_Associate *)s_wifi_buffer_rx;
    
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_ASSOCIATE;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    resp->capability = 0;
    rx_header->length = sizeof(WiFi_CmdResponse_Associate);
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_receive_packet(void *data, uint32_t size)
{
    uint8_t buf[] = "1234567890abcdefgh";
    uint16_t len = sizeof(buf);
    rx_header->type = WIFI_SDIOFRAME_DATA;
    rx_packet->rx_packet_length = len;
    memcpy(rx_packet->payload, buf, len);
    rx_header->length = sizeof(WiFi_DataRx) + len;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_set_wep(void *data, uint32_t size)
{
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_SET_WEP;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_key_material(void *data, uint32_t size)
{
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_KEY_MATERIAL;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_deauthenticate(void *data, uint32_t size)
{
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_DEAUTHENTICATE;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_ad_hoc_join(void *data, uint32_t size)
{
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_AD_HOC_JOIN;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = s_cmd_busy_flag ? 0 : tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_start_adhoc(void *data, uint32_t size)
{
    rx_header->length = tx_cmd->frame_header.length;
    rx_header->type = WIFI_SDIOFRAME_COMMAND;
    rx_cmd->cmd_code = 0x8000 | CMD_802_11_AD_HOC_START;
    rx_cmd->size = tx_cmd->size;
    rx_cmd->seq_num = tx_cmd->seq_num;
    rx_cmd->result = tx_cmd->result;

    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_packet(void *data, uint32_t size)
{
    rx_header->length = sizeof(WiFi_DataRx);
    rx_header->type = WIFI_SDIOFRAME_DATA;

    rx_packet->rx_packet_length = 14;
    rx_packet->payload[12] = 0x88; // type == 0x8880
    rx_packet->payload[13] = 0x80;
    
    rx_header->length += rx_packet->rx_packet_length;
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_eapol(void *data, uint32_t size)
{
    rx_header->length = sizeof(WiFi_DataRx);
    rx_header->type = WIFI_SDIOFRAME_DATA;

    WiFi_EAPOLKeyFrame * eapol_key_param = (WiFi_EAPOLKeyFrame*)rx_packet->payload;
    
    rx_packet->rx_packet_length = sizeof(WiFi_EAPOLKeyFrame);
    rx_packet->payload[12] = 0x88; // type == 0x888e
    rx_packet->payload[13] = 0x8e;
    
    if(s_test_flag == TEST_FLAG_EAPOL_INVALID_FRAME){
        eapol_key_param->key_information = htons(0x01);
    }else if(s_test_flag == TEST_FLAG_EAPOL_MSG1_P){
        eapol_key_param->key_information = htons(0x88 | 0x01);
    }else if(s_test_flag == TEST_FLAG_EAPOL_KEYTYPE_AES){
        eapol_key_param->key_information = htons(0x88 | 0x02);
    }else if(s_test_flag == TEST_FLAG_EAPOL_MSG1_G){
        eapol_key_param->key_information = htons(0x380 | 0x01);
    }else if(s_test_flag == TEST_FLAG_EAPOL_MSG3_WPA){
        eapol_key_param->key_information = htons(0x1c8 | 0x01);
    }else if(s_test_flag == TEST_FLAG_EAPOL_MSG3_WPA2){
        eapol_key_param->key_information = htons(0x3c8 | 0x01);
    }

    rx_header->length += rx_packet->rx_packet_length;
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

static void wifi_response_event(void *data, uint32_t size)
{
    rx_header->length = sizeof(WiFi_DataRx);
    rx_header->type = WIFI_SDIOFRAME_EVENT;

    rx_packet->rx_packet_length = 14;
    rx_packet->payload[12] = 0x80; // type == 0x8080
    rx_packet->payload[13] = 0x80;
    
    rx_header->length += rx_packet->rx_packet_length;
    memcpy(data, s_wifi_buffer_rx, rx_header->length);
}

uint8_t WiFi_LowLevel_ReadData(uint8_t func, uint32_t addr, void *data, uint32_t size, uint32_t bufsize)
{
    if(s_test_flag == TEST_FLAG_GET_MAC){
        wifi_response_mac_address(data, size);
    }else if(s_test_flag == TEST_FLAG_SCAN){
        wifi_response_scan(data, size);
    }else if(s_test_flag == TEST_FLAG_SCAN_SSID_FAILED){
        wifi_response_scan_ssid_failed(data, size);
    }else if(s_test_flag == TEST_FLAG_MAC_CONTROL){
        wifi_response_mac_control(data, size);
        s_test_flag = TEST_FLAG_SCAN_SSID;
    }else if(s_test_flag == TEST_FLAG_SCAN_SSID){
        wifi_response_scan_ssid(data, size);
        s_test_flag = TEST_FLAG_ASSOCIATE;
    }else if(s_test_flag == TEST_FLAG_ASSOCIATE){
        wifi_response_associate(data, size);
    }else if(s_test_flag == TEST_FLAG_DISCARD){
        char buf[] = "discard data test.";
        memcpy(data, buf, sizeof(buf));
    }else if(s_test_flag == TEST_FLAG_GET_RECEIVED_PACKET){
        wifi_response_receive_packet(data, size);
    }else if(s_test_flag == TEST_FLAG_SET_WEP){
        wifi_response_set_wep(data, size);
        s_test_flag = TEST_FLAG_MAC_CONTROL;
    }else if(s_test_flag == TEST_FLAG_KEY_MATERIAL){ 
        wifi_response_set_wep(data, size);
    }else if(s_test_flag == TEST_FLAG_SET_KEY_MATERIAL){
        wifi_response_key_material(data, size);
    }else if(s_test_flag == TEST_FLAG_DEAUTHENTICATE){
        wifi_response_deauthenticate(data, size);
    }else if(s_test_flag == TEST_FLAG_JOIN_ADHOCEX){
        wifi_response_set_wep(data, size);
        s_test_flag = TEST_FLAG_JOIN_ADHOC_MAC_CONTROL;
    }else if(s_test_flag == TEST_FLAG_JOIN_ADHOC_MAC_CONTROL){
        wifi_response_mac_control(data, size);
        s_test_flag = TEST_FLAG_JOIN_ADHOC_SCAN_SSID;
    }else if(s_test_flag == TEST_FLAG_JOIN_ADHOC_SCAN_SSID){
        wifi_response_scan_ssid(data, size);
        s_test_flag = TEST_FLAG_JOIN_ADHOC;
    }else if(s_test_flag == TEST_FLAG_JOIN_ADHOC){
        wifi_response_scan_ssid(data, size);
        s_test_flag = TEST_FLAG_AD_HOC_JOIN;
    }else if(s_test_flag == TEST_FLAG_AD_HOC_JOIN){
        wifi_response_ad_hoc_join(data, size);
    }else if(s_test_flag == TEST_FLAG_MAC_ADDR){
        wifi_response_mac_address(data, size);
    }else if(s_test_flag == TEST_FLAG_START_ADHOC){
        wifi_response_start_adhoc(data, size);
    }else if(s_test_flag == TEST_FLAG_START_ADHOCEX){
        wifi_response_set_wep(data, size);
        s_test_flag = TEST_FLAG_START_ADHOC_MAC_CONTROL;
    }else if(s_test_flag == TEST_FLAG_START_ADHOC_MAC_CONTROL){
        wifi_response_mac_control(data, size);
        s_test_flag = TEST_FLAG_START_ADHOC;
    }else if(s_test_flag == TEST_FLAG_EVENT){
        wifi_response_event(data, size);
    }else if(s_test_flag == TEST_FLAG_RESPONSE_PACKET){
        wifi_response_packet(data, size);
    }else if(s_test_flag >= TEST_FLAG_EAPOL){
        wifi_response_eapol(data, size);
    }
    
    return s_ucRetDataRead;
}
uint8_t WiFi_LowLevel_ReadReg(uint8_t func, uint32_t addr)
{
    // Gets the address of the CIS
    if((addr & 0x9) == 0x9){
        return 1;
    }else if(addr >= 0x00010001){
        if(s_test_flag == TEST_FLAG_CISTPL_VERS_1){
            if(addr == 0x10001){
                return CISTPL_VERS_1;
            }else if(addr == 0x10002){
                return 4; // tpl_link
            }else if(addr == 0x10003){
                return 1; // 
            }else if(addr == 0x10004){
                return 2; // 
            }
        }else if(s_test_flag == TEST_FLAG_CISTPL_MANFID){
            if(addr == 0x10001){
                return CISTPL_MANFID;
            }else if(addr == 0x10002){
                return 1;
            }else if(addr == 0x10003){
                return 1;
            }
        }else if(s_test_flag == TEST_FLAG_CISTPL_FUNCID){
            if(addr == 0x10001){
                return CISTPL_FUNCID;
            }else if(addr == 0x10002){
                return 1;
            }else if(addr == 0x10003){
                return 1;
            }
        }else if(s_test_flag == TEST_FLAG_CISTPL_FUNCE0){
            if(addr == 0x10001){
                return CISTPL_FUNCE;
            }else if(addr == 0x10002){
                return 1;
            }else if(addr == 0x10003){
                return 0;
            }
        }else if(s_test_flag == TEST_FLAG_CISTPL_FUNCE1){
            if(addr == 0x10001){
                return CISTPL_FUNCE;
            }else if(addr == 0x10002){
                return 1;
            }else if(addr == 0x10003){
                return 1;
            }
        }else if(s_test_flag == TEST_FLAG_CISTPL_END){
            if(addr == 0x10001){
                return CISTPL_END;
            }else if(addr == 0x10002){
                return 1;
            }else if(addr == 0x10003){
                return 1;
            }
        }else{
            if(addr == 0x10001){
                return 1;
            }else if(addr == 0x10002){
                return 1; // tpl_link
            }else if(addr == 0x10003){
                return 1; // 
            }else if(addr == 0x10004){
                return 1; // 
            }

        }
        return CISTPL_END;
    }else if(addr == SDIO_CCCR_IORDY){
        return SDIO_CCCR_IORDY_IOR1;
    }else if(addr == WIFI_INTSTATUS){
        if(s_test_flag == TEST_FLAG_WAIT_TIMEOUT) return 0;
        return WIFI_INTSTATUS_DNLD | WIFI_INTSTATUS_UPLD;
    }else if(func == 1 && addr == WIFI_SQREADBASEADDR0){
        return 64;
    }else if(func == 1 && addr == WIFI_SQREADBASEADDR1){
        return 0;
    }else if(func == 1 && addr == WIFI_SCRATCHPAD4_0){
        if(s_test_flag == TEST_FLAG_DISCARD) return 10;
        else if(s_test_flag == TEST_FLAG_GET_DATA_LENGTH) return 1;
        return 0xdc;
    }else if(func == 1 && addr == WIFI_SCRATCHPAD4_1){
        if(s_test_flag == TEST_FLAG_DISCARD) return 0;
        else if(s_test_flag == TEST_FLAG_GET_DATA_LENGTH) return 1;
        return 0xfe;
    }

    // wait boot
    if(s_isBoot == 0){
        if(func == 1 && addr == WIFI_SCRATCHPAD4_0){
            return 0xdc;
        }else if(func == 1 && addr == WIFI_SCRATCHPAD4_1){
            s_isBoot = 1;
            return 0xfe;
        }
    }
    return s_ucRetRegRead;
}
void WiFi_LowLevel_SetBlockSize(uint8_t func, uint32_t size)
{

}
uint8_t WiFi_LowLevel_WriteData(uint8_t func, uint32_t addr, const void *data, uint32_t size, uint32_t bufsize)
{
    memcpy(s_wifi_buffer_command, data, size);
    memcpy(s_wifi_buffer_packet, data, size);
    return s_ucRetDataWrite;
}
uint8_t WiFi_LowLevel_WriteReg(uint8_t func, uint32_t addr, uint8_t value)
{
    return s_ucRetRegWrite;
}

void WiFi_AuthenticationCompleteHandler(void)
{
    printf("Authenticated!\n");
}
void WiFi_EventHandler(const WiFi_Event *event)
{
    printf("[Event %d] size=%d", event->event_id, event->header.length);
    if (event->header.length > sizeof(WiFi_Event))
        WiFi_DumpData(event + 1, event->header.length - sizeof(WiFi_Event));
}
void WiFi_PacketHandler(const WiFi_DataRx *data)
{
    if(data == NULL)
        return;
    WiFi_OutputBuffer(data->payload, data->rx_packet_length);
}

void mbedtls_arc4_setup( mbedtls_arc4_context *ctx, const unsigned char *key,
                 unsigned int keylen )
{
}

void mbedtls_md5( const unsigned char *input, size_t ilen, unsigned char output[16] )
{
    if(0 == s_check_mic_flag) return;
    if(s_test_flag >= TEST_FLAG_EAPOL){
        memcpy(output, wifi_mic.MIC, 16);
    }
}

void mbedtls_sha1( const unsigned char *input, size_t ilen, unsigned char output[20] )
{
    if(0 == s_check_mic_flag) return;
    if(s_test_flag >= TEST_FLAG_EAPOL){
        memcpy(output, &wifi_mic, 20);
    }
}

static void wifi_associate_callback(void *arg, void *data, WiFi_Status status)
{
    switch (status)
    {
    case WIFI_STATUS_OK:
        printf("Associated!\n");
        break;
    case WIFI_STATUS_NOTFOUND:
        printf("SSID not found!\n");
        break;
    case WIFI_STATUS_FAIL:
        printf("Association failed!\n");
        break;
    case WIFI_STATUS_INPROGRESS:
        printf("Waiting for authentication!\n");
        break;
    default:
        printf("Unknown error! status=%d\n", status);
    }
}

static void wifi_mac_address_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
    {
        uint8_t mac[6] = {0};
        memcpy(mac, data, 6);
        printf("MAC Addr: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], 
            mac[2], mac[3], mac[4], mac[5]);
    }
    else
        printf("Cannot get MAC address!\n");
}

static void wifi_scan_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
        printf("Scan finished!\n");
    else
        printf("Scan failed!\n");
}

static void wifi_set_wep_callback(void *arg, void *data, WiFi_Status status)
{
    printf("[%s]Test WiFi_SetWEP() ........\n", __func__);
}

static void wifi_deauthenticate_callback(void *arg, void *data, WiFi_Status status)
{
    printf("[%s]Test WiFi_Deauthenticate() ........\n", __func__);
}

static void wifi_adhoc_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
        printf("ADHOC %sed!\n", (char *)arg);
    else
        printf("Cannot %s ADHOC!\n", (char *)arg);
}

static void wifi_join_adhoc_callback(void *arg, void *data, WiFi_Status status)
{
    printf("[%s]Test WiFi_JoinADHOC() ........\n", __func__);
}

static void wifi_start_adhoc(void *arg, void *data, WiFi_Status status)
{
    printf("[%s]Test WiFi_StartADHOC() ........\n", __func__);
}

static void wifi_stop_adhoc(void *arg, void *data, WiFi_Status status)
{
    printf("[%s]Test WiFi_StopADHOC() ........\n", __func__);
}

static void *stub_malloc(size_t size)
{
    return NULL;
}

static uint8_t stub_WiFi_GetBSSID(uint8_t mac_addr[6])
{
    return 0;
}

uint8_t stub_pbkdf2_hmac_sha1(const void *password, uint16_t pwdlen, const void *salt, 
            uint16_t saltlen, uint32_t c, uint8_t *dk, uint16_t dklen)
{
    return 0;
}

static uint8_t stub_PRF(const void *k, uint16_t klen, const char *a, const void *b, 
                uint16_t blen, void *output, uint8_t n)
{
    static int i = 0;
    if(i++ == s_prf_err_type){
        i = 0;
        return 0;
    }
    return 1;
}

}
/* Public functions ---------------------------------------------------------*/

TestMarvellWifi::TestMarvellWifi()
{
    TEST_ADD(TestMarvellWifi::test_WiFi_Init);
    TEST_ADD(TestMarvellWifi::test_WiFi_EAPOL);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetMACAddress);
    TEST_ADD(TestMarvellWifi::test_WiFi_AssociateEx);
    TEST_ADD(TestMarvellWifi::test_WiFi_Scan);

    TEST_ADD(TestMarvellWifi::test_WiFi_Associate);
    TEST_ADD(TestMarvellWifi::test_WiFi_CheckTimeout);
    TEST_ADD(TestMarvellWifi::test_WiFi_Deauthenticate);
    TEST_ADD(TestMarvellWifi::test_WiFi_DiscardData);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetBSSID);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetDataLength);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetPacketBuffer);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetReceivedPacket);
    TEST_ADD(TestMarvellWifi::test_WiFi_GetSecurityType);
    TEST_ADD(TestMarvellWifi::test_WiFi_Input);
    TEST_ADD(TestMarvellWifi::test_WiFi_IsCommandBusy);
    TEST_ADD(TestMarvellWifi::test_WiFi_JoinADHOC);
    TEST_ADD(TestMarvellWifi::test_WiFi_JoinADHOCEx);
    TEST_ADD(TestMarvellWifi::test_WiFi_KeyMaterial);
    TEST_ADD(TestMarvellWifi::test_WiFi_MACAddr);
    TEST_ADD(TestMarvellWifi::test_WiFi_MACControl);
    TEST_ADD(TestMarvellWifi::test_WiFi_ScanSSID);
    TEST_ADD(TestMarvellWifi::test_WiFi_SendCommand);
    TEST_ADD(TestMarvellWifi::test_WiFi_SendPacket);
    TEST_ADD(TestMarvellWifi::test_WiFi_SetWEP);
    TEST_ADD(TestMarvellWifi::test_WiFi_SetWPA);
    TEST_ADD(TestMarvellWifi::test_WiFi_ShowCIS);   // mce48
    TEST_ADD(TestMarvellWifi::test_WiFi_StartADHOC);
    TEST_ADD(TestMarvellWifi::test_WiFi_StartADHOCEx);
    TEST_ADD(TestMarvellWifi::test_WiFi_StopADHOC);
    TEST_ADD(TestMarvellWifi::test_WiFi_TranslateTLV);
    TEST_ADD(TestMarvellWifi::test_WiFi_Wait);
    TEST_ADD(TestMarvellWifi::test_WiFi_WaitForLastTask);
    
    TEST_ADD(TestMarvellWifi::test_WiFi_DumpData);
    TEST_ADD(TestMarvellWifi::test_WiFi_OutputBuffer);

}

TestMarvellWifi::~TestMarvellWifi()
{

}

void TestMarvellWifi::test_WiFi_Associate(void)
{
    int ret = 0;
    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_Associate("abc", WIFI_AUTH_MODE_OPEN, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Associate(...) failed");
    cleanStub(&stub_info);

    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_SCAN_SSID;
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    s_cmd_busy_flag = 1;
    WiFi_Associate("abc", WIFI_AUTH_MODE_OPEN, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Associate(...) failed");
    s_test_flag = TEST_FLAG_SCAN_SSID;
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;
}
void TestMarvellWifi::test_WiFi_AssociateEx(void)
{
    int ret = 0;
    WiFi_Connection conn;
        
    conn.security = WIFI_SECURITYTYPE_WPA; // this support WPA/WPA2
    conn.ssid = (char*)TEST_SSID;
    conn.password = (void*)TEST_PASSWORD; 
    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_AssociateEx(...) failed");
    s_test_flag = TEST_FLAG_MAC_CONTROL;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    
    TestMarvellWifi::test_WiFi_EAPOL(); // authenticate

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_AssociateEx(...) failed");
    cleanStub(&stub_info);
    s_test_flag = TEST_FLAG_MAC_CONTROL;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_AssociateEx(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_MAC_CONTROL;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback
//    WiFi_Input(); // WiFi_ScanSSID_Callback
//    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;

    conn.security = WIFI_SECURITYTYPE_WEP;
    WiFi_WEPKey wep_key;
    memset(&wep_key, 0, sizeof(WiFi_WEPKey));
    char key[] = "embed";
    wep_key.keys[0] = key;
    wep_key.index = 0;
    conn.password = &wep_key;
    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_AssociateEx(...) failed");
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback
    WiFi_Input(); // WiFi_AssocaiateEx_Callback
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_Assocaite_Callback
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

}

void TestMarvellWifi::test_WiFi_CheckTimeout(void)
{
    int ret = 0;
    int timeout = 0;
    while(timeout <= WIFI_DEFAULT_TIMEOUT_CHECK){
        WiFi_CheckTimeout();
        timeout++;
    }
    TEST_ASSERT_MSG((0 == ret), "WiFi_CheckTimeout() failed");
}
void TestMarvellWifi::test_WiFi_Deauthenticate(void)
{
    int ret = 0;
    uint16_t reason = 1;

    stubInfo stub_info;
    setStub((void*)WiFi_GetBSSID, (void*)stub_WiFi_GetBSSID, &stub_info);
    WiFi_Deauthenticate(reason, wifi_deauthenticate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Deauthenticate(...) failed");
    cleanStub(&stub_info);

    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_Deauthenticate(reason, wifi_deauthenticate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Deauthenticate(...) failed");
    cleanStub(&stub_info);
    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_DEAUTHENTICATE;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    
    WiFi_Deauthenticate(reason, wifi_deauthenticate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Deauthenticate(...) failed");

    s_test_flag = TEST_FLAG_DEAUTHENTICATE;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_DiscardData(void)
{
    int ret = 0;
    s_test_flag = TEST_FLAG_DISCARD;
    WiFi_DiscardData();
    TEST_ASSERT_MSG((0 == ret), "WiFi_DiscardData() failed");
    
    s_ucRetDataRead = 0;
    WiFi_DiscardData();
    s_ucRetDataRead = 1;
    TEST_ASSERT_MSG((0 == ret), "WiFi_DiscardData() failed");
    
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_GetBSSID(void)
{
    uint8_t ret = 0;
    uint8_t bssid[6] = {0};
    ret = WiFi_GetBSSID(bssid);
    TEST_ASSERT_MSG((0 == ret), "WiFi_GetBSSID(...) failed");
    printf("bssid: %02X:%02X:%02X:%02X:%02X:%02X\n", bssid[0], bssid[1], 
            bssid[2], bssid[3], bssid[4], bssid[5]);
}
void TestMarvellWifi::test_WiFi_GetDataLength(void)
{
    uint16_t len;
    s_test_flag = TEST_FLAG_GET_DATA_LENGTH;
    len = WiFi_GetDataLength();
    TEST_ASSERT_MSG(((1 | 1<<8) == len), "WiFi_GetDataLength() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_GetMACAddress(void)
{
    int ret = 0;
    WiFi_GetMACAddress(wifi_mac_address_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_GetMACAddress(...) failed");
    s_test_flag = TEST_FLAG_GET_MAC;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_GetMACAddress(wifi_mac_address_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_GetMACAddress(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_GET_MAC;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_GetMACAddress(wifi_mac_address_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_GetMACAddress(...) failed");
    cleanStub(&stub_info);
    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_GET_MAC;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_GetPacketBuffer(void)
{
    uint8_t *packet = NULL;
    packet = WiFi_GetPacketBuffer();
    TEST_ASSERT_MSG((NULL != packet), "WiFi_GetPacketBuffer() failed");
}
void TestMarvellWifi::test_WiFi_GetReceivedPacket(void)
{
    uint8_t *packet = NULL;
    uint16_t len = 0;
    packet = (uint8_t*)WiFi_GetReceivedPacket(&len);
    printf("get received packet length is %d\n", len);
    TEST_ASSERT_MSG((NULL == packet), "WiFi_GetReceivedPacket(...) failed");

    s_test_flag = TEST_FLAG_GET_RECEIVED_PACKET;
    WiFi_Input();
    packet = (uint8_t*)WiFi_GetReceivedPacket(&len);
    printf("get received packet length is %d\n", len);
    TEST_ASSERT_MSG((NULL != packet), "WiFi_GetReceivedPacket(...) failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_GetSecurityType(void)
{
    WiFi_SecurityType type;
    WiFi_SSIDInfo ssid_info;
    memset(&ssid_info, 0, sizeof(WiFi_SSIDInfo));
    
    type = WiFi_GetSecurityType(&ssid_info);
    TEST_ASSERT_MSG((WIFI_SECURITYTYPE_NONE == type), "WiFi_GetSecurityType(...) failed");

    ssid_info.cap_info |= WIFI_CAPABILITY_PRIVACY;
    type = WiFi_GetSecurityType(&ssid_info);
    TEST_ASSERT_MSG((WIFI_SECURITYTYPE_WEP == type), "WiFi_GetSecurityType(...) failed");

    ssid_info.rsn.header.type = 1;
    type = WiFi_GetSecurityType(&ssid_info);
    TEST_ASSERT_MSG((WIFI_SECURITYTYPE_WPA2 == type), "WiFi_GetSecurityType(...) failed");

    ssid_info.rsn.header.type = 0;
    ssid_info.wpa.header.type = 1;
    type = WiFi_GetSecurityType(&ssid_info);
    TEST_ASSERT_MSG((WIFI_SECURITYTYPE_WPA == type), "WiFi_GetSecurityType(...) failed");
}
void TestMarvellWifi::test_WiFi_Init(void)
{
    int ret = 0;
    WiFi_Init();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
}
void TestMarvellWifi::test_WiFi_Input(void)
{
    int ret = 0;
    // event
    s_test_flag = TEST_FLAG_EVENT;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_IsCommandBusy(void)
{
    uint8_t ret = 0;
    ret = WiFi_IsCommandBusy();
    TEST_ASSERT_MSG((0 == ret), "WiFi_IsCommandBusy() failed");
}
void TestMarvellWifi::test_WiFi_JoinADHOC(void)
{
    int ret = 0;
    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_JoinADHOC("abc", wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOC(...) failed");
    cleanStub(&stub_info);

    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_JOIN_ADHOC;
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_SCAN)
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;


    WiFi_JoinADHOC("abc", wifi_associate_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOC(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_JOIN_ADHOC;
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_SCAN)
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;
}
void TestMarvellWifi::test_WiFi_JoinADHOCEx(void)
{
    int ret = 0;
    WiFi_Connection conn;

    conn.security = WIFI_SECURITYTYPE_WPA; // this support WPA/WPA2
    conn.ssid = (char*)TEST_SSID;
    conn.password = (void*)TEST_PASSWORD; 
    WiFi_JoinADHOCEx((const WiFi_Connection*)&conn, 2, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOCEx(...) failed");
    
    WiFi_WEPKey wepkey = {0};
    conn.security = WIFI_SECURITYTYPE_WEP;
    conn.ssid = (char*)"WM-G-MR-09";
    conn.password = (void*)&wepkey;
    wepkey.keys[0] = (char*)"1234567890123";
    wepkey.index = 0;

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_JoinADHOCEx((const WiFi_Connection*)&conn, 2, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOCEx(...) failed");
    cleanStub(&stub_info);
    
    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_JOIN_ADHOCEX;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_802_11_SET_WEP)
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback (CMD_802_11_SCAN)
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    
    WiFi_JoinADHOCEx((const WiFi_Connection*)&conn, 2, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOCEx(...) failed");
    s_test_flag = TEST_FLAG_JOIN_ADHOCEX;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_802_11_SET_WEP)
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback (CMD_802_11_SCAN)
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_JoinADHOCEx((const WiFi_Connection*)&conn, 2, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOCEx(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_JOIN_ADHOCEX;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_802_11_SET_WEP)
//    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_MAC_CONTROL)
//    WiFi_Input(); // WiFi_ScanSSID_Callback
//    WiFi_Input(); // WiFi_JoinADHOC_Callback (CMD_802_11_SCAN)
//    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;

    WiFi_JoinADHOCEx((const WiFi_Connection*)&conn, 2, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_JoinADHOCEx(...) failed");
    
    s_test_flag = TEST_FLAG_JOIN_ADHOCEX;
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_802_11_SET_WEP)
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback (CMD_802_11_SCAN)
    s_cmd_busy_flag = 1;
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_cmd_busy_flag = 0;
    // retry
    WiFi_Input(); // WiFi_AssocaiateEx_Callback(CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_ScanSSID_Callback
    WiFi_Input(); // WiFi_JoinADHOC_Callback (CMD_802_11_SCAN)
    WiFi_Input(); // WiFi_JoinADHOC_Callback(CMD_802_11_AD_HOC_JOIN)
    s_test_flag = TEST_FLAG_NONE;

}
void TestMarvellWifi::test_WiFi_KeyMaterial(void)
{
    int ret = 0;
    uint8_t key[] = {0,1,2,3};
    uint8_t key_count = 1;
    WiFi_CommandAction action = WIFI_ACT_SET;
    MrvlIETypes_KeyParamSet_t keyParam;
    memset(&keyParam, 0, sizeof(MrvlIETypes_KeyParamSet_t));
    memcpy(keyParam.key, key, key_count);

    WiFi_KeyMaterial(action, &keyParam, key_count, NULL, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_KeyMaterial() failed");
    s_test_flag = TEST_FLAG_KEY_MATERIAL;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_MACAddr(void)
{
    int ret = 0;
    WiFi_CommandAction action;

    action = WIFI_ACT_GET;
    WiFi_MACAddr(mac, action, wifi_mac_address_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_MACAddr(...) failed");

    s_test_flag = TEST_FLAG_MAC_ADDR;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    action = WIFI_ACT_SET;
    WiFi_MACAddr(mac, action, wifi_mac_address_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_MACAddr(...) failed");

    s_test_flag = TEST_FLAG_MAC_ADDR;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_MACControl(void)
{
    int ret = 0;
    WiFi_CommandAction action;

    action = WIFI_ACT_GET;
    
    WiFi_MACControl(action, NULL, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_MACControl(...) failed");

    s_test_flag = TEST_FLAG_MAC_CONTROL;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_Scan(void)
{
    int ret = 0;
    WiFi_Scan(wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Scan(...) failed");
    s_test_flag = TEST_FLAG_SCAN;
    // 14 channels, every time 4 channels
    for(int i = 0; i < 4; i++){
        WiFi_Input();
    }
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_Scan(wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Scan(...) failed");
    cleanStub(&stub_info);
    
    // stub_malloc is invalid when memchecks_test_flag = TEST_FLAG_SCAN;
    // 14 channels, every time 4 channels
    for(int i = 0; i < 4; i++){
        WiFi_Input();
    }
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_Scan(wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Scan(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_SCAN_SSID_FAILED;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;
}
void TestMarvellWifi::test_WiFi_ScanSSID(void)
{
    int ret = 0;
    WiFi_SSIDInfo info;
    memset(&info, 0, sizeof(WiFi_SSIDInfo));
    
    WiFi_ScanSSID(TEST_SSID, &info, wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_ScanSSID(...) failed");
    
    s_test_flag = TEST_FLAG_SCAN_SSID_FAILED;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_ScanSSID(TEST_SSID, &info, wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_ScanSSID(...) failed");

    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_SCAN_SSID_FAILED;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_ScanSSID(TEST_SSID, &info, wifi_scan_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_ScanSSID(...) failed");
    cleanStub(&stub_info);
    
    // stub_malloc is invalid when memcheck
    s_cmd_busy_flag = 0;
    s_test_flag = TEST_FLAG_SCAN_SSID_FAILED;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_SendCommand(void)
{
    // 
}
void TestMarvellWifi::test_WiFi_SendPacket(void)
{
    int ret = 0;
    uint8_t data[] = {1,2,3,4,5};
    uint16_t len = sizeof(data);
    
    WiFi_SendPacket(data, len, NULL, NULL, WIFI_DEFAULT_TIMEOUT_DATAACK);
    TEST_ASSERT_MSG((0 == ret), "WiFi_SendPacket(...) failed");
}
void TestMarvellWifi::test_WiFi_SetWEP(void)
{
    int ret = 0;
    WiFi_CommandAction action;
    WiFi_WEPKey wep_key;
    memset(&wep_key, 0, sizeof(WiFi_WEPKey));
    wep_key.keys[0] = (char*)"embed";  // len = 5
    wep_key.index = 0;

    action = WIFI_ACT_ADD;
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    wep_key.keys[0] = (char*)"1234567890123";  // len = 13
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    wep_key.keys[0] = (char*)"1234567890";  // len = 10
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    wep_key.keys[0] = (char*)"abcdefghijklmnopqrstuvwxyz";  // len = 26
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    wep_key.keys[0] = (char*)"maminjie";  // len = 8
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    /////////////////////////////////
    action = WIFI_ACT_REMOVE;
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    s_test_flag = TEST_FLAG_SET_WEP;
    WiFi_Input();
    s_test_flag = TEST_FLAG_NONE;
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");

    action = (WiFi_CommandAction)0xff;
    WiFi_SetWEP(action, &wep_key, wifi_set_wep_callback, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWEP(...) failed");
}
void TestMarvellWifi::test_WiFi_SetWPA(void)
{
    int ret = 0;
    char* ssid = (char*)TEST_SSID;
    char* password = (char*)TEST_PASSWORD; 
    WiFi_SetWPA(ssid, password);
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWPA(...) failed");

    stubInfo stub_info;
    setStub((void*)pbkdf2_hmac_sha1, (void*)stub_pbkdf2_hmac_sha1, &stub_info);
    WiFi_SetWPA(ssid, password);
    TEST_ASSERT_MSG((0 == ret), "WiFi_SetWPA(...) failed");
    cleanStub(&stub_info);
}
void TestMarvellWifi::test_WiFi_ShowCIS(void)
{
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_VERS_1;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_MANFID;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_FUNCID;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_FUNCE0;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_FUNCE1;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_CISTPL_END;
    WiFi_ShowCIS();
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_StartADHOC(void)
{
    int ret = 0;
    WiFi_StartADHOC("abc", 1, wifi_start_adhoc, NULL);
    TEST_ASSERT_MSG((0 == ret), "WiFi_StartADHOC(...) failed");

    s_test_flag = TEST_FLAG_START_ADHOC;
    WiFi_Input(); 
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_StartADHOCEx(void)
{
    int ret = 0;
    WiFi_Connection conn;

    conn.security = WIFI_SECURITYTYPE_WPA; // this support WPA/WPA2
    conn.ssid = (char*)TEST_SSID;
    conn.password = (void*)TEST_PASSWORD; 
    WiFi_StartADHOCEx((const WiFi_Connection*)&conn, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_StartADHOCEx(...) failed");
    
    WiFi_WEPKey wepkey = {0};
    conn.security = WIFI_SECURITYTYPE_WEP;
    conn.ssid = (char*)"WM-G-MR-09";
    conn.password = &wepkey;
    wepkey.keys[0] = (char*)"1234567890123";
    wepkey.index = 0;

    stubInfo stub_info;
    setStub((void*)malloc, (void*)stub_malloc, &stub_info);
    WiFi_StartADHOCEx((const WiFi_Connection*)&conn, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_StartADHOCEx(...) failed");
    cleanStub(&stub_info);
    // stub_malloc is invalid when memcheck
    s_test_flag = TEST_FLAG_START_ADHOCEX;
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_SET_WEP)
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_AD_HOC_START)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    
    WiFi_StartADHOCEx((const WiFi_Connection*)&conn, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_StartADHOCEx(...) failed");
    s_test_flag = TEST_FLAG_START_ADHOCEX;
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_SET_WEP)
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_MAC_CONTROL)
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_AD_HOC_START)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    WiFi_StartADHOCEx((const WiFi_Connection*)&conn, wifi_adhoc_callback, (void*)"start");
    TEST_ASSERT_MSG((0 == ret), "WiFi_StartADHOCEx(...) failed");
    s_cmd_busy_flag = 1;
    s_test_flag = TEST_FLAG_START_ADHOCEX;
    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_SET_WEP)
//    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_MAC_CONTROL)
//    WiFi_Input(); // WiFi_StartADHOCEx_Callback (CMD_802_11_AD_HOC_START)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;
    s_cmd_busy_flag = 0;
}
void TestMarvellWifi::test_WiFi_StopADHOC(void)
{
    WiFi_StopADHOC(wifi_stop_adhoc, NULL);
}
void TestMarvellWifi::test_WiFi_TranslateTLV(void)
{

}
void TestMarvellWifi::test_WiFi_Wait(void)
{
    uint8_t ret = 0;
    ret = WiFi_Wait(WIFI_INTSTATUS_DNLD, 0);
    TEST_ASSERT_MSG((1 == ret), "WiFi_Wait(...) failed");

    s_test_flag = TEST_FLAG_WAIT_TIMEOUT;
    ret = WiFi_Wait(WIFI_INTSTATUS_DNLD, 10);
    TEST_ASSERT_MSG((0 == ret), "WiFi_Wait(...) failed");
    s_test_flag = TEST_FLAG_NONE;
}
void TestMarvellWifi::test_WiFi_WaitForLastTask(void)
{
    int ret = 0;
    WiFi_WaitForLastTask();
    TEST_ASSERT_MSG((0 == ret), "WiFi_WaitForLastTask() failed");
    
    WiFi_GetMACAddress(wifi_mac_address_callback, NULL);
    WiFi_WaitForLastTask();
    TEST_ASSERT_MSG((0 == ret), "WiFi_GetMACAddress(...) failed");
    s_test_flag = TEST_FLAG_GET_MAC;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Input() failed");
    s_test_flag = TEST_FLAG_NONE;

    uint8_t data[] = {1,2,3,4,5};
    uint16_t len = sizeof(data);
    WiFi_SendPacket(data, len, NULL, NULL, WIFI_DEFAULT_TIMEOUT_DATAACK);
    TEST_ASSERT_MSG((0 == ret), "WiFi_SendPacket(...) failed");
}

void TestMarvellWifi::test_WiFi_EAPOL(void)
{
    int ret = 0;
    
    s_test_flag = TEST_FLAG_EAPOL;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_EAPOL_INVALID_FRAME;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");

    s_test_flag = TEST_FLAG_EAPOL_MSG1_P;
    stubInfo stub_info;
    setStub((void*)PRF, (void*)stub_PRF, &stub_info);
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_prf_err_type = 1;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_prf_err_type = 0;
    cleanStub(&stub_info);
    
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input(); // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");

    s_test_flag = TEST_FLAG_EAPOL_MSG3_WPA;
    s_check_mic_flag = 0;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_check_mic_flag = 1;
    
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_SET_KEY_MATERIAL;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (CMD_802_11_KEY_MATERIAL)
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");

    s_test_flag = TEST_FLAG_EAPOL_MSG1_G;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_SET_KEY_MATERIAL;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (CMD_802_11_KEY_MATERIAL)
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    
    s_test_flag = TEST_FLAG_EAPOL_MSG3_WPA2;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_SET_KEY_MATERIAL;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (CMD_802_11_KEY_MATERIAL)
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");

    /////////////////////////////////// KEY TYPE AES //////////////////////////////////
    s_test_flag = TEST_FLAG_EAPOL_KEYTYPE_AES;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input(); // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    
    s_test_flag = TEST_FLAG_EAPOL_MSG3_WPA;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_SET_KEY_MATERIAL;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (CMD_802_11_KEY_MATERIAL)
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");

    s_test_flag = TEST_FLAG_EAPOL_MSG1_G;
    WiFi_Input();
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    s_test_flag = TEST_FLAG_SET_KEY_MATERIAL;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (CMD_802_11_KEY_MATERIAL)
    s_test_flag = TEST_FLAG_RESPONSE_PACKET;
    WiFi_Input();   // WiFi_EAPOLProcess_Callback (WiFi_SendEAPOLResponse)
    TEST_ASSERT_MSG((0 == ret), "WiFi_Init() failed");
    
    s_test_flag = TEST_FLAG_NONE;
}

void TestMarvellWifi::test_WiFi_DumpData(void)
{
    char data[] = {1,2,3,4,5,6,7,8};
    int ret = 0;
    
    WiFi_DumpData(data, sizeof(data));
    TEST_ASSERT_MSG((0 == ret), "WiFi_DumpData(...) failed");
}
void TestMarvellWifi::test_WiFi_OutputBuffer(void)
{
    uint8_t data[] = "marvell 88w8686 sdio wifi unit test WiFi_OutputBuffer()";
    int ret = 0;
    
    WiFi_OutputBuffer(data, sizeof(data));
    TEST_ASSERT_MSG((0 == ret), "WiFi_DumpData(...) failed");
}

/* Private functions --------------------------------------------------------*/

void TestMarvellWifi::setup()
{

}
void TestMarvellWifi::tear_down()
{
    
}

