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
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wpa.h"
#include "wifi.h"

#ifndef WIFI_FIRMWAREAREA_ADDR 
#include "sd8686.c"
#include "helper_sd.c"
#endif

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
typedef enum
{
    WIFI_RX_FLAG_NONE = 0x00,
    WIFI_RX_FLAG_DATA = 0x01
}WiFi_RxFlag;
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static uint8_t      wifi_buffer_command[256]; 
static uint8_t      wifi_buffer_packet[1792]; 
static uint8_t      wifi_buffer_rx[2048];
static uint8_t      wifi_psk[32]; // preshared-key
static uint8_t      wifi_snonce[32];
static uint32_t     wifi_port;
static uint32_t     wifi_input_time = 0; 
static WiFi_GTK     wifi_gtk;
static WiFi_PTK     wifi_ptk;
static WiFi_SSIDInfo wifi_ssid_info = {0}; 
static WiFi_TxBuffer wifi_tx_command = {0}; 
static WiFi_TxBuffer wifi_tx_packet = {0}; 

static uint8_t wifi_rx_flag = WIFI_RX_FLAG_NONE;
/* Extern variables ---------------------------------------------------------*/

extern uint32_t sys_now(void);

/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
static void WiFi_Associate_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_AssociateEx_Callback(void *arg, void *data, WiFi_Status status);
static uint8_t WiFi_CheckCommandBusy(WiFi_Callback callback, void *arg);
static uint8_t WiFi_CheckMIC(WiFi_EAPOLKeyFrame *packet, uint16_t len);
static uint8_t WiFi_CheckTxBufferRetry(WiFi_TxBuffer *tbuf, void *data);
static void WiFi_Deauthenticate_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_DownloadHelper(void);
static void WiFi_DownloadReal(void);
static void WiFi_DownloadFirmware(void);
static void WiFi_EAPOLProcess(WiFi_DataRx *data);
static void WiFi_EAPOLProcess_Callback(void *arg, void *data, WiFi_Status status);
static uint8_t WiFi_ExtractGTK(const WiFi_EAPOLKeyFrame *packet_rx);
static void WiFi_GetMACAddress_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_JoinADHOC_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_JoinADHOCEx_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_TxBufferComplete(WiFi_TxBuffer *tbuf, void *data, WiFi_Status status);
static void WiFi_Scan_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_ScanSSID_Callback(void *arg, void *data, WiFi_Status status);
static void WiFi_SendEAPOLResponse(const WiFi_EAPOLKeyFrame *packet_rx, uint16_t key_info, 
                        const void *key_data, uint16_t key_data_len, WiFi_Callback callback, void *arg);
static void WiFi_SetKeyMaterial(WiFi_KeyType key_type, uint8_t key_num, WiFi_Callback callback, void *arg);
static void WiFi_StartADHOCEx_Callback(void *arg, void *data, WiFi_Status status);

/* Public functions ---------------------------------------------------------*/
void WiFi_DumpData(const void *data, uint32_t len)
{
    uint32_t i = 0;
    const uint8_t *p = data;
    while (len--){
        printf("0x%02X ", *p++);
        if(++i%20 == 0) printf("\n");
    }
    printf("\n");
}

void WiFi_OutputBuffer(const uint8_t *buffer, int length)
{
    int i;

    if (length == 0) printf("\n");
    if (buffer == NULL) return;

    i = 0;
    while (i < length)
    {
        uint8_t array[16];
        int j;

        memcpy(array, buffer+i, 16);
        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            printf("%02X ", array[j]);
            if (j%4 == 3) printf(" ");
        }
        if (length > 16)
        {
            while (j < 16)
            {
                printf("   ");
                if (j%4 == 3) printf(" ");
                j++;
            }
        }
        printf(" ");
        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            if ((array[j] >= 0x20 && array[j] <= 0x7f))
                printf("%c", array[j]);
            else
                printf(".");
        }
        printf("\n");
        i += 16;
    }
    printf("\n");
}


/* Associate AP */
// The mac_addr parameter, used to receive the MAC address of the hotspot, can be NULL, 
// but cannot point to a local variable
void WiFi_Associate(const char *ssid, WiFi_AuthenticationType auth_type, WiFi_Callback callback, void *arg)
{
    void **p;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;
    
    // The last member is not a pointer, but the actual data
    p = malloc(2 * sizeof(void *) + sizeof(WiFi_AuthenticationType)); 
    if (p == NULL)
    {
        printf("WiFi_Associate: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;
    *(WiFi_AuthenticationType *)(p + 2) = auth_type;

    WiFi_ScanSSID(ssid, &wifi_ssid_info, WiFi_Associate_Callback, p);
}

static void WiFi_Associate_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;    // arg block
    void *app_arg = p[0];       // user arguments
    WiFi_Callback app_callback = (WiFi_Callback)p[1]; // user callback
    WiFi_AuthenticationType *pauth = (WiFi_AuthenticationType *)(p + 2);

    uint16_t cmd_size;
    WiFi_CmdRequest_Associate *cmd;
    WiFi_CmdResponse_Associate *resp;
    WiFi_SecurityType security;
    MrvlIETypes_PhyParamDSSet_t *ds;
    MrvlIETypes_CfParamSet_t *cf;
    MrvlIETypes_AuthType_t *auth;
    MrvlIETypes_VendorParamSet_t *vendor;
    MrvlIETypes_RsnParamSet_t *rsn;

    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_Associate error!\n");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    security = WiFi_GetSecurityType(&wifi_ssid_info);
    switch (WiFi_GetCommandCode(data))
    {
    case CMD_802_11_SCAN:
        // The WiFi_ScanSSID command is executed
        cmd = (WiFi_CmdRequest_Associate *)wifi_buffer_command;
        memcpy(cmd->peer_sta_addr, wifi_ssid_info.mac_addr, sizeof(wifi_ssid_info.mac_addr));
        cmd->cap_info = wifi_ssid_info.cap_info;
        cmd->listen_interval = 10;
        cmd->bcn_period = wifi_ssid_info.bcn_period;
        cmd->dtim_period = 1;
        memcpy(cmd + 1, &wifi_ssid_info.ssid, TLV_STRUCTLEN(wifi_ssid_info.ssid));

        ds = (MrvlIETypes_PhyParamDSSet_t *)((uint8_t *)(cmd + 1) + TLV_STRUCTLEN(wifi_ssid_info.ssid));
        ds->header.type = WIFI_MRVLIETYPES_PHYPARAMDSSET;
        ds->header.length = 1;
        ds->channel = wifi_ssid_info.channel;

        cf = (MrvlIETypes_CfParamSet_t *)(ds + 1);
        memset(cf, 0, sizeof(MrvlIETypes_CfParamSet_t));
        cf->header.type = WIFI_MRVLIETYPES_CFPARAMSET;
        cf->header.length = TLV_PAYLOADLEN(*cf);

        memcpy(cf + 1, &wifi_ssid_info.rates, TLV_STRUCTLEN(wifi_ssid_info.rates));
        auth = (MrvlIETypes_AuthType_t *)((uint8_t *)(cf + 1) + TLV_STRUCTLEN(wifi_ssid_info.rates));
        auth->header.type = WIFI_MRVLIETYPES_AUTHTYPE;
        auth->header.length = TLV_PAYLOADLEN(*auth);
        auth->auth_type = *pauth;

        cmd_size = (uint8_t *)(auth + 1) - wifi_buffer_command;
        if (security == WIFI_SECURITYTYPE_WPA)
        {
            // The WPA network must add Vendor parameters to the command to successfully connect
            vendor = (MrvlIETypes_VendorParamSet_t *)(auth + 1);
            memcpy(vendor, &wifi_ssid_info.wpa, TLV_STRUCTLEN(wifi_ssid_info.wpa));
            cmd_size += TLV_STRUCTLEN(wifi_ssid_info.wpa);
        }
        else if (security == WIFI_SECURITYTYPE_WPA2)
        {
            // The WPA2 network must add RSN parameters to the command to successfully connect
            rsn = (MrvlIETypes_RsnParamSet_t *)(auth + 1);
            memcpy(rsn, &wifi_ssid_info.rsn, TLV_STRUCTLEN(wifi_ssid_info.rsn));
            cmd_size += TLV_STRUCTLEN(wifi_ssid_info.rsn);
        }

        WiFi_SendCommand(CMD_802_11_ASSOCIATE, wifi_buffer_command, cmd_size, WiFi_Associate_Callback, 
                        arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
        // Keep arg memory until the association is successful
        break;
    case CMD_802_11_ASSOCIATE:
        // The associated command executes and receives a response
        // Now you need to check if the association is successful
        free(arg);
        resp = (WiFi_CmdResponse_Associate *)data;
//        printf("capability=0x%04x, status_code=0x%04x, aid=0x%04x\n", 
//                resp->capability, resp->status_code, resp->association_id);
        if (app_callback)
        {
            if (resp->association_id == 0xffff){
                // Association failure (check the resp->capability and resp->status_code values 
                // in the callback function data to see why)
                app_callback(app_arg, data, WIFI_STATUS_FAIL); 
            }else if (security == WIFI_SECURITYTYPE_WPA || security == WIFI_SECURITYTYPE_WPA2){
                app_callback(app_arg, data, WIFI_STATUS_INPROGRESS); // Waiting for the certification
            }else{
                app_callback(app_arg, data, WIFI_STATUS_OK); // Associated with success
            }
        }
        break;
    }
}

/* Associate a hotspot and enter a password */
// When connecting a WPA type hotspot, the security member simply assigns WIFI_SECURITYTYPE_WPA
// without specifying the WPA version number
void WiFi_AssociateEx(const WiFi_Connection *conn, WiFi_AuthenticationType auth_type, 
                            int8_t max_retry, WiFi_Callback callback, void *arg)
{
    int8_t *pmax_retry;
    uint16_t ssid_len;
    void **p;
    WiFi_AuthenticationType *pauth;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    ssid_len = strlen(conn->ssid);
    p = malloc(2 * sizeof(void *) + sizeof(WiFi_AuthenticationType) + sizeof(int8_t) + ssid_len + 1);
    if (p == NULL)
    {
        WIFI_ERROR("malloc failed!");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;
    pauth = (WiFi_AuthenticationType *)(p + 2);
    *pauth = auth_type;
    pmax_retry = (int8_t *)(pauth + 1);
    // The maximum number of attempts to reconnect, -1 is an infinite number of attempts, and 0 is no retry
    *pmax_retry = max_retry; 
    memcpy(pmax_retry + 1, conn->ssid, ssid_len + 1);

    if (conn->security == WIFI_SECURITYTYPE_WEP)
    {
        WiFi_SetWEP(WIFI_ACT_ADD, conn->password, WiFi_AssociateEx_Callback, p);
    }
    else
    {
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, WiFi_AssociateEx_Callback, p);
        if (conn->security == WIFI_SECURITYTYPE_WPA || conn->security == WIFI_SECURITYTYPE_WPA2)
            WiFi_SetWPA(conn->ssid, conn->password);
    }
}

static void WiFi_AssociateEx_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    WiFi_AuthenticationType *pauth = (WiFi_AuthenticationType *)(p + 2);
    int8_t *pmax_retry = (int8_t *)(pauth + 1);

    char *ssid = (char *)(pmax_retry + 1);
    uint16_t cmd_code = WiFi_GetCommandCode(data);

    if (cmd_code == CMD_802_11_ASSOCIATE || cmd_code == CMD_802_11_SCAN)
    {
        if (cmd_code == CMD_802_11_ASSOCIATE && (status == WIFI_STATUS_OK || status == WIFI_STATUS_INPROGRESS))
        {
            free(arg);
            if (app_callback)
                app_callback(app_arg, data, status);
            return;
        }
        else
        {
            if (*pmax_retry != 0)
            {
                if (*pmax_retry != -1)
                    (*pmax_retry)--;
                cmd_code = CMD_MAC_CONTROL;
                status = WIFI_STATUS_OK;
            }
        }
    }

    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_AssociateEx error!\n");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    switch (cmd_code)
    {
    case CMD_802_11_SET_WEP:
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_WEP | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, 
                        WiFi_AssociateEx_Callback, arg);
        break;
    case CMD_MAC_CONTROL:
        WiFi_Associate(ssid, *pauth, WiFi_AssociateEx_Callback, arg);
    }
}

/* If a new command is requested before the previous command is finished, 
 * the callback function is called directly to report an error */
static uint8_t WiFi_CheckCommandBusy(WiFi_Callback callback, void *arg)
{
    // Before sending a new command, you must ensure that the previous command has been sent and received a response
    // See 4.2 Protocol: The command exchange protocol is serialized, the host driver must wait until
    // it has received a command response for the current command request before it can send the next command request.
    if (WiFi_IsCommandBusy())
    {
        printf("Warning: The previous command is in progress!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_BUSY);
        return 1;
    }

    WiFi_WaitForLastTask(); // Wait for the previous data frame to receive confirmation
    return 0;
}

/* Verify that the MIC value in the EAPOL frame is correct */
static uint8_t WiFi_CheckMIC(WiFi_EAPOLKeyFrame *packet, uint16_t len)
{
    uint8_t ret;
    WiFi_KeyType key_type = (WiFi_KeyType)(ntohs(packet->key_information) & 0x07);
    WiFi_MIC mic[2];

    memcpy(mic[0].MIC, packet->key_mic, sizeof(packet->key_mic));
    memset(packet->key_mic, 0, sizeof(packet->key_mic));
    if (key_type == WIFI_KEYTYPE_TKIP)
        ret = hmac_md5(wifi_ptk.KCK, sizeof(wifi_ptk.KCK), &packet->version, len - 14, mic[1].MIC);
    else if (key_type == WIFI_KEYTYPE_AES)
        ret = hmac_sha1(wifi_ptk.KCK, sizeof(wifi_ptk.KCK), &packet->version, len - 14, mic[1].MIC);
    else
        return 0;

    if (!ret)
    {
        printf("WiFi_CheckMIC: out of memory!\n");
        return 0;
    }
    return memcmp(mic[0].MIC, mic[1].MIC, sizeof(mic[1].MIC)) == 0;
}

/* Data frames, command frames send timeout processing */
void WiFi_CheckTimeout(void)
{
    WiFi_CommandHeader *cmd = (WiFi_CommandHeader *)wifi_buffer_command;

    // Check the INTSTATUS register regularly to avoid program die if no SDIOIT interrupt is detected
    if (sys_now() > wifi_input_time + WIFI_DEFAULT_TIMEOUT_CHECK)
        WiFi_Input();

    // Data parameters in the callback function: failed-send failed data, success-receive response
    // When sending data frames, data is always the content sent
    if (WiFi_CheckTxBufferRetry(&wifi_tx_command, wifi_buffer_command))
    {
        WiFi_SendCommand(0, NULL, 0, wifi_tx_command.callback, wifi_tx_command.arg, 
                        wifi_tx_command.timeout, wifi_tx_command.retry - 1);
        printf("WiFi Command 0x%04x Timeout! Resend...\n", cmd->cmd_code);
    }
    WiFi_CheckTxBufferRetry(&wifi_tx_packet, wifi_buffer_packet);
}

/* Check if the send buffer needs to be retransmitted */
// Data is the data that needs to be passed to the callback function when an error is reported
static uint8_t WiFi_CheckTxBufferRetry(WiFi_TxBuffer *tbuf, void *data)
{
    if (tbuf->busy && sys_now() > tbuf->start_time + tbuf->timeout) 
    {
        if (tbuf->retry != 0)
        {
            tbuf->busy = 0;
            return 1;
        }
        // Report an error to the callback function over the maximum retry
        WiFi_TxBufferComplete(tbuf, data, WIFI_STATUS_NORESP); 
    }
    return 0;
}

/* Disconnect from the hot spot */
void WiFi_Deauthenticate(uint16_t reason, WiFi_Callback callback, void *arg)
{
    uint8_t ret;
    void **p;
    WiFi_Cmd_Deauthenticate *cmd = (WiFi_Cmd_Deauthenticate *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    ret = WiFi_GetBSSID(cmd->peer_sta_addr);
    if (!ret)
    {
        printf("WiFi_Deauthenticate: WiFi is not connected!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_FAIL);
        return;
    }

    p = malloc(2 * sizeof(void *));
    if (p == NULL)
    {
        printf("WiFi_Deauthenticate: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;

    cmd->reason_code = reason;
    WiFi_SendCommand(CMD_802_11_DEAUTHENTICATE, wifi_buffer_command, sizeof(WiFi_Cmd_Deauthenticate), 
                    WiFi_Deauthenticate_Callback, p, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

static void WiFi_Deauthenticate_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    free(arg);

    if (status == WIFI_STATUS_OK)
        memset(wifi_ssid_info.mac_addr, 0, 6);
    else
        printf("WiFi_Deauthenticate failed!\n");

    if (app_callback)
        app_callback(app_arg, data, status);
}

static void WiFi_DownloadHelper(void)
{
    uint8_t helper_buf[64];
    const uint8_t *data;
    uint16_t curr;
    uint32_t len;

    // Download the helper firmware
    data = WIFI_HELPER_ADDR;
    len = WIFI_HELPER_SIZE;
    while (len)
    {
        // Download up to 60 bytes of firmware at a time
        curr = (len > 60) ? 60 : len;
        memcpy(helper_buf, &curr, 4); // The first 4 bytes are the amount of data actually downloaded
        memcpy(helper_buf + 4, data, curr);
        
        if (len != WIFI_HELPER_SIZE) // There is no waiting until the data is sent for the first time
            WiFi_Wait(WIFI_INTSTATUS_DNLD, 0);
        WiFi_LowLevel_WriteData(1, wifi_port, helper_buf, sizeof(helper_buf), sizeof(helper_buf));
        len -= curr;
        data += curr;
    }
    
    // End with an empty packet
    memset(helper_buf, 0, 4);
    WiFi_LowLevel_WriteData(1, wifi_port, helper_buf, sizeof(helper_buf), sizeof(helper_buf)); 

}

static void WiFi_DownloadReal(void)
{
    uint8_t tmp_buffer[512];
    const uint8_t *data;
    uint16_t curr;
    uint32_t len;
    
    // Download the real firmware
    data = WIFI_FIRMWARE_ADDR; // This address must be 4-byte aligned, or it will cause a transfer error
    len = WIFI_FIRMWARE_SIZE;
    while (len)
    {
        // Gets the number of bytes that should be downloaded this time
        // Each time n>=curr bytes of data can be sent, only one CMD53 command can be sent, 
        // and the WiFi module only recognizes the prior curr bytes of data
        WiFi_Wait(WIFI_INTSTATUS_DNLD, 0); // You must wait for Download Ready before sending
        while ((curr = WiFi_LowLevel_ReadReg(1, WIFI_SQREADBASEADDR0) | (WiFi_LowLevel_ReadReg(1, WIFI_SQREADBASEADDR1) << 8)) == 0);
        //printf("Required: %d bytes, Remaining: %d bytes\n", curr, len);

        if (curr & 1)
        {
            // If curr is odd (such as 17), it indicates that CRC validation error occurred at the receiving end 
            // and the previous content should be retransmitted (omitted in this part of the code).
            printf("Error: an odd size is invalid!\n");
            while (1);
        }
        if (curr > len)
            curr = len;

        /* Avoid the firmware header address not on the boundary address 
         * Omits adding compiler pseudo-instructions to the firmware array, 
         * eg: __attribute__((aligned))
         */
        memcpy(tmp_buffer, data, curr);

        // Send firmware data
        // You cannot use multi-byte transport mode in high speed mode, 
        // you can only use block transport mode, so the buffer should be large enough
#ifdef WIFI_HIGHSPEED 
        if (len < 32) // Len is the buffer residual size
        {
            WiFi_LowLevel_WriteData(1, wifi_port, tmp_buffer, curr, sizeof(tmp_buffer));
        }
        else
#endif
            WiFi_LowLevel_WriteData(1, wifi_port, tmp_buffer, curr, len);

        len -= curr;
        data += curr;
    }

}

/* download firmware */
static void WiFi_DownloadFirmware(void)
{
    WiFi_DownloadHelper();
    WiFi_DownloadReal();
    
    // Wait for Firmware to start
    WiFi_Wait(WIFI_INTSTATUS_DNLD, 0);
    while (WiFi_GetDataLength() != 0xfedc);
    printf("Firmware is successfully downloaded!\n");
}

/* Discard received data or commands that have not been processed due to a program error */
void WiFi_DiscardData(void)
{
    uint16_t len = WiFi_GetDataLength();
    uint8_t ret = WiFi_LowLevel_ReadData(1, wifi_port, wifi_buffer_rx, len, sizeof(wifi_buffer_rx));
    if (ret)
    {
        printf("Discarded %d bytes!\n", len);
        WiFi_DumpData(wifi_buffer_rx, len);
    }
    else
        printf("Discarding %d bytes of data failed!\n", len);
}

/* Handle EAPOL authentication frames */
static void WiFi_EAPOLProcess(WiFi_DataRx *data)
{
    uint8_t i, ret;
    uint8_t random_b[10];
    uint16_t key_info;
    uint16_t key_data_len;
    uint32_t random_k[16];
    WiFi_EAPOLKeyFrame *packet_rx = (WiFi_EAPOLKeyFrame *)data->payload;
    WiFi_EAPOLKeyFrame *packet_tx;
    WiFi_KDE *kde;
    WiFi_KeyType key_type;
    WiFi_PTKB ptkb;

    key_info = ntohs(packet_rx->key_information);
    key_type = (WiFi_KeyType)(key_info & 0x07);
    if (key_type != WIFI_KEYTYPE_TKIP && key_type != WIFI_KEYTYPE_AES)
    {
        printf("Unsupported key descriptor version: %d\n", key_type);
        return;
    }

    // Removes bits that are independent of the first six parameters of the EAPOL-Key frame notation
    switch (key_info & 0x23c8) 
    {
    case 0x88:
        /* 4-way handshake Message 1: EAPOL-Key(0,0,1,0,P,0,...), P=1 */
        printf("Message 1 received!\n");

        /* Generate SNonce */
        // PRF-256(Random number, "Init Counter", Local MAC Address || Time)
        srand(sys_now());
        for (i = 0; i < sizeof(random_k) / sizeof(uint32_t); i++)
            random_k[i] = rand();
        memcpy(random_b, packet_rx->dest, sizeof(packet_rx->dest));
        *(uint32_t *)(random_b + sizeof(packet_rx->dest)) = sys_now();
        ret = PRF(random_k, sizeof(random_k), "Init Counter", random_b, sizeof(random_b), wifi_snonce, sizeof(wifi_snonce)); // PRF-256
        if (!ret)
        {
            // In the case of insufficient memory, 
            // you only need to increase the Heap_Size of the heap space in the STM32 startup file (.s) to solve the problem
            printf("PRF: out of memory!\n"); 
            break;
        }

        /* Generate PTK */
        // Smaller MAC addresses are in the front, bigger ones are in the back
        if (memcmp(packet_rx->dest, packet_rx->src, sizeof(packet_rx->src)) < 0)
            memcpy(ptkb.MAC, packet_rx->dest, sizeof(ptkb.MAC));
        else
        {
            memcpy(ptkb.MAC[0], packet_rx->src, sizeof(packet_rx->src));
            memcpy(ptkb.MAC[1], packet_rx->dest, sizeof(packet_rx->dest));
        }
        // Smaller random Numbers come first, bigger Numbers come last
        if (memcmp(packet_rx->key_nonce, wifi_snonce, sizeof(wifi_snonce)) < 0)
        {
            memcpy(ptkb.nonce[0], packet_rx->key_nonce, sizeof(packet_rx->key_nonce));
            memcpy(ptkb.nonce[1], wifi_snonce, sizeof(wifi_snonce));
        }
        else
        {
            memcpy(ptkb.nonce[0], wifi_snonce, sizeof(wifi_snonce));
            memcpy(ptkb.nonce[1], packet_rx->key_nonce, sizeof(packet_rx->key_nonce));
        }
        // wifi_psk is generated when the password is set
        ret = PRF(wifi_psk, sizeof(wifi_psk), "Pairwise key expansion", &ptkb, sizeof(ptkb), &wifi_ptk, sizeof(wifi_ptk)); // PRF-512
        if (!ret)
        {
            printf("PRF: out of memory!\n");
            break;
        }

        /* Add Key Data information in Message 2 to be sent */
        // https://community.arubanetworks.com/t5/Technology-Blog/A-closer-look-at-WiFi-Security-IE-Information-Elements/ba-p/198867
        // Hotspots using WPA must have WPA IE information items, and there must be no RSN IE information items
        // Hotspots that use WPA2 must have RSN IE information items, possibly WPA IE information items
        packet_tx = (WiFi_EAPOLKeyFrame *)WiFi_GetPacketBuffer();
        kde = (WiFi_KDE *)packet_tx->key_data;
        if (wifi_ssid_info.rsn.header.type)
        {
            // The router provides RSN IE information items that can be copied directly
            //printf("RSN IE copied!\n");
            kde->type = WIFI_MRVLIETYPES_RSNPARAMSET;
            kde->length = wifi_ssid_info.rsn.header.length;
            memcpy(kde->oui, wifi_ssid_info.rsn.rsn, wifi_ssid_info.rsn.header.length);
            key_data_len = sizeof(kde->type) + sizeof(kde->length) + kde->length;
        }
        else if (wifi_ssid_info.wpa.header.type)
        {
            // The router provides a WPA IE information item that can be copied directly
            //printf("WPA IE copied!\n");
            kde->type = WIFI_MRVLIETYPES_VENDORPARAMSET;
            kde->length = wifi_ssid_info.wpa.header.length;
            memcpy(kde->oui, wifi_ssid_info.wpa.vendor, wifi_ssid_info.wpa.header.length);
            key_data_len = sizeof(kde->type) + sizeof(kde->length) + kde->length;
        }
        else
        {
            printf("IE not copied!\n");
            key_data_len = 0;
        }

        /* Send Message 2: EAPOL-Key(0,1,0,0,P,0,...) */
        WiFi_SendEAPOLResponse(packet_rx, 0x108 | key_type, NULL, key_data_len, WiFi_EAPOLProcess_Callback, (void *)2);
        break;
    case 0x1c8: // WPA MSG3
    case 0x3c8: // WPA2 MSG3
        /* 4-way handshake Message 3: EAPOL-Key(1,1,1,1,P,0,...) */
        printf("Message 3 received!\n");
        if (!WiFi_CheckMIC(packet_rx, data->rx_packet_length)) // prevents undetected modification of message 3
        {
            printf("Message 3 is corrupted!\n");
            break;
        }
        // If the command send buffer is occupied, the Msg3 discarded this time will not respond and wait for the next Msg3
        if (WiFi_IsCommandBusy()) 
            break;

        // When WPA is certified, PTK is only sent to the firmware
        // For WPA2 authentication, you need to get GTK and send PTK and GTK to the firmware
        ret = WiFi_ExtractGTK(packet_rx);
        WiFi_SetKeyMaterial(key_type, ret ? 2 : 1, WiFi_EAPOLProcess_Callback, (void *)(ret ? 2 : 3));

        /* Send Message 4: EAPOL-Key(1,1,0,0,P,0,...) */
        WiFi_SendEAPOLResponse(packet_rx, 0x308 | key_type, NULL, 0, WiFi_EAPOLProcess_Callback, (void *)4);
        break;
    case 0x380:
        /* Group key handshake Message 1: EAPOL-Key(1,1,1,0,G,0,...), G=0 */
        printf("Group key handshake!\n");
        if (!WiFi_CheckMIC(packet_rx, data->rx_packet_length))
            break;
        if (WiFi_IsCommandBusy())
            break;

        /* Send the new GTK to the firmware */
        if (!WiFi_ExtractGTK(packet_rx))
        {
            printf("Extracting GTK failed!\n");
            break;
        }
        // Send both PTK and GTK at the same time, not just GTK, otherwise the key in the firmware cannot be updated 
        WiFi_SetKeyMaterial(key_type, 2, WiFi_EAPOLProcess_Callback, (void *)1); 

        /* Send Message 2: EAPOL-Key(1,1,0,0,G,0,...) */
        WiFi_SendEAPOLResponse(packet_rx, 0x300 | key_type, NULL, 0, WiFi_EAPOLProcess_Callback, (void *)2);
        break;
    default:
        printf("Unhandled EAPOL frame! key_info=0x%04x\n", key_info);
        WiFi_DumpData(packet_rx, data->rx_packet_length);
    }
}

static void WiFi_EAPOLProcess_Callback(void *arg, void *data, WiFi_Status status)
{
    WiFi_SDIOFrameHeader *header = (WiFi_SDIOFrameHeader *)data;
    if (status == WIFI_STATUS_OK)
    {
        if (header->type == WIFI_SDIOFRAME_COMMAND)
        {
            switch ((uint32_t)arg)
            {
            case 1:
                printf("GTK set!\n");
                break;
            case 2:
                printf("PTK & GTK set!\n");
                break;
            case 3:
                printf("PTK set!\n");
                break;
            }
        }
        else if (header->type == WIFI_SDIOFRAME_DATA)
        {
            printf("Message %d sent!\n", (int)arg);
            if ((uint32_t)arg == 4){
                // With PTK, you can send broadcast frames, so it's appropriate to call callback here
                WiFi_AuthenticationCompleteHandler();
            }
        }
    }
}

/* Decrypt key_data data with KEK key and extract GTK key */
static uint8_t WiFi_ExtractGTK(const WiFi_EAPOLKeyFrame *packet_rx)
{
    uint16_t key_info = ntohs(packet_rx->key_information);
    uint16_t key_len = ntohs(packet_rx->key_length);
    uint16_t keydata_len = ntohs(packet_rx->key_data_length);
    WiFi_KDE *kde;
    WiFi_KeyType key_type = (WiFi_KeyType)(key_info & 0x07);

    // Decrypt the key_data field
    kde = (WiFi_KDE *)wifi_buffer_command;
    if (key_type == WIFI_KEYTYPE_TKIP)
        ARC4_decrypt_keydata(wifi_ptk.KEK, packet_rx->key_iv, packet_rx->key_data, keydata_len, wifi_buffer_command);
    else if (key_type == WIFI_KEYTYPE_AES)
        keydata_len = AES_unwrap(wifi_ptk.KEK, packet_rx->key_data, keydata_len, wifi_buffer_command);
    else
        return 0;

    if (keydata_len == key_len)
    {
        // If the authentication type is WPA, the decrypted keydata content is GTK
        memcpy(&wifi_gtk, wifi_buffer_command, keydata_len);
        return 1;
    }
    else
    {
        // If the authentication type is WPA2, the decrypted keydata content is some KDE structured data, 
        // with GTK in one of the KDE
        while (kde->length != 0) 
        {
            if (kde->type == 0xdd && kde->data_type == 1 && kde->length - 6 == key_len) // GTK KDE
            {
                memcpy(wifi_gtk.TK, kde->data + 2, key_len);
                return 1;
            }
            kde = (WiFi_KDE *)((uint8_t *)kde + kde->length + 2);
            // Ensure that the key->length falls within the valid data area
            if (((uint8_t *)kde - wifi_buffer_command) >= keydata_len - 1) 
                break;
        }
    }

    return 0;
}

/* Gets the MAC address of the connected hotspot */
uint8_t WiFi_GetBSSID(uint8_t mac_addr[6])
{
    uint8_t i;
    memcpy(mac_addr, wifi_ssid_info.mac_addr, 6);
    for (i = 0; i < 6; i++)
    {
        if (mac_addr[i] != 0)
            break;
    }
    return i != 6; // The return value indicates whether the MAC address is not all 0
}

/* Gets the size of the data to be received */
uint16_t WiFi_GetDataLength(void)
{
    // Read the low 16 bits of the Scratch pad 4 register
    return WiFi_LowLevel_ReadReg(1, WIFI_SCRATCHPAD4_0) | (WiFi_LowLevel_ReadReg(1, WIFI_SCRATCHPAD4_1) << 8);
}

/* Get the MAC address */
// callback cannot be NULL
void WiFi_GetMACAddress(WiFi_Callback callback, void *arg)
{
    void **p;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    p = malloc(2 * sizeof(void *));
    if (p == NULL)
    {
        printf("WiFi_GetMACAddress: malloc failed!\n");
        if(callback){
            callback(arg, NULL, WIFI_STATUS_MEM);
        }
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;

    WiFi_MACAddr(NULL, WIFI_ACT_GET, WiFi_GetMACAddress_Callback, p);
}

static void WiFi_GetMACAddress_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    WiFi_Cmd_MACAddr *cmd = (WiFi_Cmd_MACAddr *)data;
    free(arg);

    if (status == WIFI_STATUS_OK)
        app_callback(app_arg, cmd->mac_addr, status);
    else
    {
        printf("WiFi_GetMACAddress error!\n");
        app_callback(app_arg, NULL, status);
    }
}

/* Request to send new data frames */
uint8_t *WiFi_GetPacketBuffer(void)
{
    WiFi_DataTx *data = (WiFi_DataTx *)wifi_buffer_packet;
    WiFi_WaitForLastTask(); // You must ensure that the buffer is not occupied before you use it
    return data->payload;
}

const uint8_t *WiFi_GetReceivedPacket(uint16_t *len)
{
    if((wifi_rx_flag & WIFI_RX_FLAG_DATA) == WIFI_RX_FLAG_DATA)
    {
        wifi_rx_flag &= ~WIFI_RX_FLAG_DATA;
        WiFi_DataRx *data = (WiFi_DataRx *)wifi_buffer_rx; 
        if (data->header.type == WIFI_SDIOFRAME_DATA)
        {
            *len = data->rx_packet_length;
            return data->payload;
        }
    }
    return NULL;
}


/**
  * @brief  Get the AP Security Type. 
  * @param  info pointer to a WiFi_SSIDInfo structure that contains
  *         the SSID information for the specified AP.  
  * @retval Security Type
  */
WiFi_SecurityType WiFi_GetSecurityType(const WiFi_SSIDInfo *info)
{
    if (info->cap_info & WIFI_CAPABILITY_PRIVACY)
    {
        if (info->rsn.header.type)
            return WIFI_SECURITYTYPE_WPA2;
        else if (info->wpa.header.type)
            return WIFI_SECURITYTYPE_WPA;
        else
            return WIFI_SECURITYTYPE_WEP;
    }
    else
        return WIFI_SECURITYTYPE_NONE;
}

/**
  * @brief  Initializes the SDIO Wi-Fi module.
  * @param  None
  * @retval None
  */
void WiFi_Init(void)
{
    // Initialize the low level driver port
    WiFi_LowLevel_Init();
    WiFi_ShowCIS();

    // Initializes the Function 1 through of Function 0 (CIA)
    WiFi_LowLevel_WriteReg(0, SDIO_CCCR_IOEN, SDIO_CCCR_IOEN_IOE1); // IOE1=1 (Enable Function)
    while ((WiFi_LowLevel_ReadReg(0, SDIO_CCCR_IORDY) & SDIO_CCCR_IORDY_IOR1) == 0); // Wait IOR1=1 (I/O Function Ready)
    WiFi_LowLevel_WriteReg(0, SDIO_CCCR_INTEN, SDIO_CCCR_INTEN_IENM | SDIO_CCCR_INTEN_IEN1); // Open SDIO interrupt request

    // enable SDU to SD host interrupt of Function 1
    // Using interrupt flag bits to determine whether there is data to read is more reliable than card status bit
    WiFi_LowLevel_WriteReg(1, WIFI_INTMASK, WIFI_INTMASK_HOSTINTMASK); 
                                                                        
    // download firmware
    wifi_port = WiFi_LowLevel_ReadReg(1, WIFI_IOPORT0) | (WiFi_LowLevel_ReadReg(1, WIFI_IOPORT1) << 8) 
                    | (WiFi_LowLevel_ReadReg(1, WIFI_IOPORT2) << 16);
    WiFi_LowLevel_SetBlockSize(1, 32);
    WiFi_DownloadFirmware();
    WiFi_LowLevel_SetBlockSize(1, 256);
}

void WiFi_Input(void)
{
    uint8_t ret, status;
    uint16_t len;
    WiFi_SDIOFrameHeader *rx_header = (WiFi_SDIOFrameHeader *)wifi_buffer_rx;
    WiFi_DataRx *rx_packet = (WiFi_DataRx *)wifi_buffer_rx;
    WiFi_CommandHeader *rx_cmd = (WiFi_CommandHeader *)wifi_buffer_rx;
    WiFi_CommandHeader *tx_cmd = (WiFi_CommandHeader *)wifi_buffer_command;

    wifi_input_time = sys_now();
    status = WiFi_LowLevel_ReadReg(1, WIFI_INTSTATUS); // Gets the interrupt token bit that needs to be processed
    if (status == 0)
        return;
    // These flag bits must be cleared before they can be processed to avoid removing new interrupts from the process
    WiFi_LowLevel_WriteReg(1, WIFI_INTSTATUS, WIFI_INTSTATUS_ALL & ~status); 

    if (status & WIFI_INTSTATUS_DNLD)
    {
        // Command frame received confirmation
        if (wifi_tx_command.busy && wifi_tx_command.ready == 0)
        {
#ifdef WIFI_DISPLAY_RESPTIME
            printf("CMD 0x%04x ACK at %ldms\n", tx_cmd->cmd_code, sys_now() - wifi_tx_command.start_time);
#endif
            wifi_tx_command.ready = 1;
        }

        // Data frame sent successfully and received confirmation
        if (wifi_tx_packet.busy)
        {
#ifdef WIFI_DISPLAY_RESPTIME
            printf("Packet ACK at %ldms\n", sys_now() - wifi_tx_packet.start_time);
#endif
            WiFi_TxBufferComplete(&wifi_tx_packet, wifi_buffer_packet, WIFI_STATUS_OK);
        }
    }

    if (status & WIFI_INTSTATUS_UPLD)
    {
        len = WiFi_GetDataLength();
        ret = WiFi_LowLevel_ReadData(1, wifi_port, wifi_buffer_rx, len, sizeof(wifi_buffer_rx));
        if (ret)
        {
            switch (rx_header->type)
            {
            case WIFI_SDIOFRAME_DATA:
                // Receive Ethernet data frames
                if (rx_packet->rx_packet_length >= 14 && rx_packet->payload[12] == 0x88 && rx_packet->payload[13] == 0x8e){
                    WiFi_EAPOLProcess(rx_packet); // Handles EAPOL authentication frames of type 0x888e
                }else{
                    wifi_rx_flag |= WIFI_RX_FLAG_DATA;
                    WiFi_PacketHandler(rx_packet);
                }
                break;
            case WIFI_SDIOFRAME_COMMAND:
                // Receive the command response frame
                if (rx_cmd->seq_num == tx_cmd->seq_num) // The serial number is consistent
                {
#ifdef WIFI_DISPLAY_RESPTIME
                    printf("CMDRESP 0x%04x at %ldms\n", rx_cmd->cmd_code, sys_now() - wifi_tx_command.start_time);
#endif
                    WiFi_TxBufferComplete(&wifi_tx_command, wifi_buffer_rx, WIFI_STATUS_OK);
                }
                break;
            case WIFI_SDIOFRAME_EVENT:
                // Received event frame
                WiFi_EventHandler((WiFi_Event *)wifi_buffer_rx); // Invokes the event processing callback function
            }
        }
    }
}

/* Before sending a command frame, you must ensure that the command send buffer is empty */
uint8_t WiFi_IsCommandBusy(void)
{
    return wifi_tx_command.busy;
}

/* Join ad-hoc */
void WiFi_JoinADHOC(const char *ssid, WiFi_Callback callback, void *arg)
{
    void **p;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    p = malloc(2 * sizeof(void *));
    if (p == NULL)
    {
        printf("WiFi_JoinADHOC: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;

    WiFi_ScanSSID(ssid, &wifi_ssid_info, WiFi_JoinADHOC_Callback, p);
}

static void WiFi_JoinADHOC_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    WiFi_Cmd_ADHOCJoin *cmd;

    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_JoinADHOC error!\n");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    switch (WiFi_GetCommandCode(data))
    {
    case CMD_802_11_SCAN:
        cmd = (WiFi_Cmd_ADHOCJoin *)wifi_buffer_command;
        memcpy(cmd->bssid, wifi_ssid_info.mac_addr, sizeof(cmd->bssid));
        strncpy((char *)cmd->ssid, (char *)wifi_ssid_info.ssid.ssid, sizeof(cmd->ssid)); // Strncpy fills the unused area with 0
        cmd->bss_type = WIFI_BSS_ANY; // recommended for use when joining Ad-Hoc networks
        cmd->bcn_period = wifi_ssid_info.bcn_period;
        cmd->dtim_period = 1;
        memset(cmd->timestamp, 0, sizeof(cmd->timestamp) + sizeof(cmd->start_ts));
        cmd->ds_param_set.header.type = WIFI_MRVLIETYPES_PHYPARAMDSSET;
        cmd->ds_param_set.header.length = TLV_PAYLOADLEN(cmd->ds_param_set);
        cmd->ds_param_set.channel = wifi_ssid_info.channel;
        cmd->reserved1 = 0;
        cmd->ibss_param_set.header.type = WIFI_MRVLIETYPES_IBSSPARAMSET;
        cmd->ibss_param_set.header.length = TLV_PAYLOADLEN(cmd->ibss_param_set);
        cmd->ibss_param_set.atim_window = 0;
        cmd->reserved2 = 0;
        cmd->cap_info = wifi_ssid_info.cap_info;
        memcpy(cmd->data_rates, wifi_ssid_info.rates.rates, sizeof(cmd->data_rates));
        cmd->reserved3 = 0;
        WiFi_SendCommand(CMD_802_11_AD_HOC_JOIN, wifi_buffer_command, sizeof(WiFi_Cmd_ADHOCJoin), 
                            WiFi_JoinADHOC_Callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
        break;
    case CMD_802_11_AD_HOC_JOIN:
        free(arg);
        cmd = (WiFi_Cmd_ADHOCJoin *)data;
        if (app_callback)
            app_callback(app_arg, data, (cmd->header.result == 0) ? WIFI_STATUS_OK : WIFI_STATUS_FAIL);
        break;
    }
}

/* Join ad-hoc networks with passwords */
void WiFi_JoinADHOCEx(const WiFi_Connection *conn, int8_t max_retry, WiFi_Callback callback, void *arg)
{
    int8_t *pmax_retry;
    uint16_t ssid_len;
    void **p;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    if (conn->security == WIFI_SECURITYTYPE_WPA || conn->security == WIFI_SECURITYTYPE_WPA2)
    {
        // 88W8686 can connect to the WPA2 authentication type Ad-Hoc hotspot created by the computer 
        // and complete the EAPOL authentication, 
        // but it will also create a WEP - type Ad-Hoc hotspot of the same name, making the communication impossible
        printf("WiFi_JoinADHOCEx: WPA is not supported!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_INVALID);
        return;
    }

    ssid_len = strlen(conn->ssid);
    p = malloc(2 * sizeof(void *) + sizeof(int8_t) + ssid_len + 1);
    if (p == NULL)
    {
        printf("WiFi_JoinADHOCEx: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;
    pmax_retry = (int8_t *)(p + 2);
    *pmax_retry = max_retry;
    memcpy(pmax_retry + 1, conn->ssid, ssid_len + 1);

    if (conn->security == WIFI_SECURITYTYPE_WEP)
        WiFi_SetWEP(WIFI_ACT_ADD, conn->password, WiFi_JoinADHOCEx_Callback, p);
    else
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, WiFi_JoinADHOCEx_Callback, p);
}

static void WiFi_JoinADHOCEx_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    int8_t *pmax_retry = (int8_t *)(p + 2);
    char *ssid = (char *)(pmax_retry + 1);
    uint16_t cmd_code = WiFi_GetCommandCode(data);

    if (cmd_code == CMD_802_11_AD_HOC_JOIN || cmd_code == CMD_802_11_SCAN)
    {
        if (cmd_code == CMD_802_11_AD_HOC_JOIN && status == WIFI_STATUS_OK)
        {
            free(arg);
            if (app_callback)
                app_callback(app_arg, data, status);
            return;
        }
        else
        {
            if (*pmax_retry != 0)
            {
                if (*pmax_retry != -1)
                    (*pmax_retry)--;
                cmd_code = CMD_MAC_CONTROL;
                status = WIFI_STATUS_OK;
            }
        }
    }

    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_JoinADHOCEx error!\n");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    switch (cmd_code)
    {
    case CMD_802_11_SET_WEP:
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_WEP | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, 
                        WiFi_JoinADHOCEx_Callback, arg);
        break;
    case CMD_MAC_CONTROL:
        WiFi_JoinADHOC(ssid, WiFi_JoinADHOCEx_Callback, arg);
    }
}

/* Gets or sets the WPA key */
void WiFi_KeyMaterial(WiFi_CommandAction action, MrvlIETypes_KeyParamSet_t *key, uint8_t key_count, 
                            WiFi_Callback callback, void *arg)
{
    uint8_t i;
    WiFi_Cmd_KeyMaterial *cmd = (WiFi_Cmd_KeyMaterial *)wifi_buffer_command;
    MrvlIETypes_KeyParamSet_t *pkey = &cmd->keys;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    cmd->action = action;
    if (action == WIFI_ACT_SET)
    {
        for (i = 0; i < key_count; i++)
        {
            key[i].header.type = WIFI_MRVLIETYPES_KEYPARAMSET;
            key[i].header.length = sizeof(MrvlIETypes_KeyParamSet_t) - sizeof(key[i].header) - sizeof(key[i].key) + key->key_len;
            memcpy(pkey, key + i, TLV_STRUCTLEN(key[i]));
            pkey = (MrvlIETypes_KeyParamSet_t *)TLV_NEXT(pkey);
        }
    }
    WiFi_SendCommand(CMD_802_11_KEY_MATERIAL, wifi_buffer_command, (uint8_t *)pkey - wifi_buffer_command, 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Gets or sets the MAC address */
void WiFi_MACAddr(const uint8_t newaddr[6], WiFi_CommandAction action, WiFi_Callback callback, void *arg)
{
    WiFi_Cmd_MACAddr *cmd = (WiFi_Cmd_MACAddr *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;
    cmd->action = action;
    if (action == WIFI_ACT_SET)
        memcpy(cmd->mac_addr, newaddr, 6);
    else
        memset(cmd->mac_addr, 0, 6);
    WiFi_SendCommand(CMD_802_11_MAC_ADDR, wifi_buffer_command, sizeof(WiFi_Cmd_MACAddr), 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Configure the MAC */
void WiFi_MACControl(uint16_t action, WiFi_Callback callback, void *arg)
{
    WiFi_Cmd_MACCtrl *cmd = (WiFi_Cmd_MACCtrl *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;
    cmd->action = action;
    cmd->reserved = 0;
    WiFi_SendCommand(CMD_MAC_CONTROL, wifi_buffer_command, sizeof(WiFi_Cmd_MACCtrl), 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Scan all hotspots (display only) */
void WiFi_Scan(WiFi_Callback callback, void *arg)
{
    uint8_t i;
    void **p;
    WiFi_CmdRequest_Scan *cmd = (WiFi_CmdRequest_Scan *)wifi_buffer_command; // The command to send
    // The +1 here refers to the forward sizeof(pointer type) address cells, not just the forward one
    MrvlIETypes_ChanListParamSet_t *chanlist = (MrvlIETypes_ChanListParamSet_t *)(cmd + 1); 
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    p = malloc(2 * sizeof(void *));
    if (p == NULL)
    {
        printf("WiFi_Scan: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;

    cmd->bss_type = WIFI_BSS_ANY;
    memset(cmd->bss_id, 0, sizeof(cmd->bss_id));

    // The basic parameters of the channel
    chanlist->header.type = WIFI_MRVLIETYPES_CHANLISTPARAMSET;
    chanlist->header.length = 4 * sizeof(chanlist->channels);
    for (i = 0; i < 4; i++) // Scan the first 4 channels first (i is the index, I +1 is the channel number)
    {
        chanlist->channels[i].band_config_type = 0; // 2.4GHz band, 20MHz channel width (2.4 - 2.4835GHz)
        chanlist->channels[i].chan_number = i + 1;  
        chanlist->channels[i].scan_type = 0;
        chanlist->channels[i].min_scan_time = 0;
        chanlist->channels[i].max_scan_time = 100;
    }

    WiFi_SendCommand(CMD_802_11_SCAN, wifi_buffer_command, sizeof(WiFi_CmdRequest_Scan) + TLV_STRUCTLEN(*chanlist), 
                    WiFi_Scan_Callback, p, 3000, WIFI_DEFAULT_MAXRETRY);
}

static void WiFi_Scan_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];

    uint8_t i, j, n;
    WiFi_CmdRequest_Scan *cmd = (WiFi_CmdRequest_Scan *)wifi_buffer_command;
    MrvlIETypes_ChanListParamSet_t *chanlist = (MrvlIETypes_ChanListParamSet_t *)(cmd + 1);

    uint8_t ssid[33], channel = 0;
    uint16_t ie_size;
    WiFi_CmdResponse_Scan *resp = (WiFi_CmdResponse_Scan *)data;
    WiFi_BssDescSet *bss_desc_set;
    WiFi_SecurityType security;
    WiFi_Vendor *vendor;
    IEEEType *ie_params;
    //MrvlIETypes_TsfTimestamp_t *tft_table;

    if (status != WIFI_STATUS_OK)
    {
        WIFI_LOG("WiFi_Scan error!");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    // Send the command to scan the next four channels
    j = chanlist->channels[0].chan_number + 4;
    if (j < 14)
    {
        if (j == 13)
            n = 2;
        else
            n = 4;

        chanlist->header.length = n * sizeof(chanlist->channels);
        for (i = 0; i < n; i++)
            chanlist->channels[i].chan_number = i + j;
        WiFi_SendCommand(CMD_802_11_SCAN, wifi_buffer_command, sizeof(WiFi_CmdRequest_Scan) + TLV_STRUCTLEN(*chanlist), 
                        WiFi_Scan_Callback, arg, 3000, WIFI_DEFAULT_MAXRETRY);
    }
    else
        n = 0;

    // Num_of_set is the number of hot spots
    if (resp->num_of_set > 0)
    {
        bss_desc_set = (WiFi_BssDescSet *)(resp + 1);
        for (i = 0; i < resp->num_of_set; i++)
        {
            security = WIFI_SECURITYTYPE_WEP;
            ie_params = &bss_desc_set->ie_parameters;
            ie_size = bss_desc_set->ie_length - (sizeof(WiFi_BssDescSet) - sizeof(bss_desc_set->ie_length) - sizeof(bss_desc_set->ie_parameters));
            while (ie_size > 0)
            {
                switch (ie_params->header.type)
                {
                case WIFI_MRVLIETYPES_SSIDPARAMSET:
                    memcpy(ssid, ie_params->data, ie_params->header.length);
                    ssid[ie_params->header.length] = '\0';
                    break;
                case WIFI_MRVLIETYPES_PHYPARAMDSSET:
                    channel = ie_params->data[0];
                    break;
                case WIFI_MRVLIETYPES_RSNPARAMSET:
                    security = WIFI_SECURITYTYPE_WPA2;
                    break;
                case WIFI_MRVLIETYPES_VENDORPARAMSET:
                    if (security != WIFI_SECURITYTYPE_WPA2)
                    {
                        vendor = (WiFi_Vendor *)ie_params->data;
                        if (vendor->oui[0] == 0x00 && vendor->oui[1] == 0x50 && vendor->oui[2] == 0xf2 && vendor->oui_type == 0x01)
                            security = WIFI_SECURITYTYPE_WPA;
                    }
                    break;
                }
                ie_size -= TLV_STRUCTLEN(*ie_params);
                ie_params = (IEEEType *)TLV_NEXT(ie_params);
            }
            if ((bss_desc_set->cap_info & WIFI_CAPABILITY_PRIVACY) == 0)
                security = WIFI_SECURITYTYPE_NONE;
            
#ifdef WIFI_DISPLAY_SCANNED_SSID
            printf("SSID '%s', ", ssid); // Hot name
            printf("MAC %02X:%02X:%02X:%02X:%02X:%02X, ", 
                            bss_desc_set->bssid[0], bss_desc_set->bssid[1],
                            bss_desc_set->bssid[2], bss_desc_set->bssid[3], 
                            bss_desc_set->bssid[4], bss_desc_set->bssid[5]); // MAC
            printf("RSSI %d, Channel %d\n", bss_desc_set->rssi, channel); // Signal strength and channel number
            //printf("  Timestamp %lld, Beacon Interval %d\n", bss_desc_set->pkt_time_stamp, bss_desc_set->bcn_interval);

            printf("  Capability: 0x%04x (Security: ", bss_desc_set->cap_info);
            switch (security)
            {
            case WIFI_SECURITYTYPE_NONE:
                printf("Unsecured");
                break;
            case WIFI_SECURITYTYPE_WEP:
                printf("WEP");
                break;
            case WIFI_SECURITYTYPE_WPA:
                printf("WPA");
                break;
            case WIFI_SECURITYTYPE_WPA2:
                printf("WPA2");
                break;
            }

            printf(", Mode: ");
            if (bss_desc_set->cap_info & WIFI_CAPABILITY_IBSS)
                printf("Ad-Hoc");
            else
                printf("Infrastructure");
            printf(")\n");
#endif // WIFI_DISPLAY_SCANNED_SSID

            // Go to the next hot spot
            bss_desc_set = (WiFi_BssDescSet *)((uint8_t *)bss_desc_set + sizeof(bss_desc_set->ie_length) + bss_desc_set->ie_length);
        }

        // resp->buf_size is the total size of bss_desc_set
        // so tft_table == buffer + sizeof(WiFi_CmdResponse_Scan) + resp->buf_size
        /*
        tft_table = (MrvlIETypes_TsfTimestamp_t *)bss_desc_set;
        if (tft_table->header.type == WIFI_MRVLIETYPES_TSFTIMESTAMP && tft_table->header.length == resp->num_of_set * sizeof(uint64_t))
        {
          printf("Timestamps: ");
          for (i = 0; i < resp->num_of_set; i++)
            printf("%lld ", tft_table->tsf_table[i]);
          printf("\n");
        }
        */

        // The TSF timestamp table is the end of the entire data and there is no Channel/band table behind it
        //if (((uint8_t *)tft_table - (uint8_t *)data) + TLV_STRUCTLEN(*tft_table) == resp->header.frame_header.length)
        //  printf("data end!\n");
    }

    // Call the callback function when the scan is complete
    if (n == 0)
    {
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
    }
}

/* Gets the details of the hotspot with the specified name */
void WiFi_ScanSSID(const char *ssid, WiFi_SSIDInfo *info, WiFi_Callback callback, void *arg)
{
    uint8_t i;
    void **p;
    MrvlIETypes_ChanListParamSet_t *chan_list;
    WiFi_CmdRequest_Scan *cmd = (WiFi_CmdRequest_Scan *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    p = malloc(3 * sizeof(void *));
    if (p == NULL)
    {
        printf("WiFi_ScanSSID: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;
    p[2] = info;
    memset(info, 0, sizeof(WiFi_SSIDInfo)); 

    cmd->bss_type = WIFI_BSS_ANY;
    memset(cmd->bss_id, 0, sizeof(cmd->bss_id));

    // Assign value to the info->ssid member
    info->ssid.header.type = WIFI_MRVLIETYPES_SSIDPARAMSET;
    info->ssid.header.length = strlen(ssid);
    memcpy(info->ssid.ssid, ssid, info->ssid.header.length);
    // Copy the info->ssid into the command content to be sent
    memcpy(cmd + 1, &info->ssid, TLV_STRUCTLEN(info->ssid)); 

    chan_list = (MrvlIETypes_ChanListParamSet_t *)((uint8_t *)(cmd + 1) + TLV_STRUCTLEN(info->ssid));
    chan_list->header.type = WIFI_MRVLIETYPES_CHANLISTPARAMSET;
    chan_list->header.length = 14 * sizeof(chan_list->channels); // Scan 14 channels at a time
    for (i = 0; i < 14; i++) 
    {
        chan_list->channels[i].band_config_type = 0;
        chan_list->channels[i].chan_number = i + 1;
        chan_list->channels[i].scan_type = 0;
        chan_list->channels[i].min_scan_time = 0;
        chan_list->channels[i].max_scan_time = 100;
    }

    WiFi_SendCommand(CMD_802_11_SCAN, wifi_buffer_command, ((uint8_t *)chan_list - wifi_buffer_command) + TLV_STRUCTLEN(*chan_list), 
                    WiFi_ScanSSID_Callback, p, 3000, WIFI_DEFAULT_MAXRETRY);
}

static void WiFi_ScanSSID_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    WiFi_SSIDInfo *info = p[2];

    uint16_t ie_size;
    WiFi_CmdResponse_Scan *resp = (WiFi_CmdResponse_Scan *)data;
    WiFi_BssDescSet *bss_desc_set = (WiFi_BssDescSet *)(resp + 1);
    IEEEType *ie_params;
    WiFi_Vendor *vendor;

    free(arg);
    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_ScanSSID error!\n");
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    if (resp->num_of_set == 0)
    {
        // The specified AP hotspot was not found, at which point all the members in the info structure except the ssid members were 0
        // The content in the resp is lost when it reaches the num_of_set member
        printf("No SSID!\n");
        if (app_callback)
            app_callback(app_arg, data, WIFI_STATUS_NOTFOUND);
        return;
    }

    // The bss_desc_set is subject to the first information item scanned
    memcpy(info->mac_addr, bss_desc_set->bssid, sizeof(info->mac_addr));
    info->cap_info = bss_desc_set->cap_info;
    info->bcn_period = bss_desc_set->bcn_interval;

    // If info->xxx.header.type=0, 
    // it indicates that there is no information of this item (except for the SSID structure, 
    // because the type of SSID =WIFI_MRVLIETYPES_SSIDPARAMSET=0).
    ie_params = &bss_desc_set->ie_parameters;
    // The total size of all IEEE_Type data
    ie_size = bss_desc_set->ie_length - (sizeof(WiFi_BssDescSet) - sizeof(bss_desc_set->ie_length) - sizeof(bss_desc_set->ie_parameters)); 
    while (ie_size > 0)
    {
        switch (ie_params->header.type)
        {
        case WIFI_MRVLIETYPES_RATESPARAMSET:
            // rate
            WiFi_TranslateTLV((MrvlIEType *)&info->rates, ie_params, sizeof(info->rates.rates));
            break;
        case WIFI_MRVLIETYPES_PHYPARAMDSSET:
            info->channel = ie_params->data[0];
            break;
        case WIFI_MRVLIETYPES_RSNPARAMSET:
            // Usually there is only one RSN information (related to WPA2)
            WiFi_TranslateTLV((MrvlIEType *)&info->rsn, ie_params, sizeof(info->rsn.rsn));
            break;
        case WIFI_MRVLIETYPES_VENDORPARAMSET:
            // There are usually multiple VENDOR information (wpa-related)
            vendor = (WiFi_Vendor *)ie_params->data;
            if (vendor->oui[0] == 0x00 && vendor->oui[1] == 0x50 && vendor->oui[2] == 0xf2)
            {
                switch (vendor->oui_type)
                {
                case 0x01:
                    // wpa_oui
                    WiFi_TranslateTLV((MrvlIEType *)&info->wpa, ie_params, sizeof(info->wpa.vendor));
                    break;
                case 0x02:
                    // wmm_oui
                    if (ie_params->header.length == 24) // Legal size
                        WiFi_TranslateTLV((MrvlIEType *)&info->wwm, ie_params, sizeof(info->wwm.vendor));
                    break;
                case 0x04:
                    // wps_oui
                    WiFi_TranslateTLV((MrvlIEType *)&info->wps, ie_params, sizeof(info->wps.vendor));
                    break;
                }
            }
            break;
        }

        // go to the next TLV
        ie_size -= TLV_STRUCTLEN(*ie_params);
        ie_params = (IEEEType *)TLV_NEXT(ie_params);
    }

    if (app_callback)
        app_callback(app_arg, data, status);
}

/* Send the WiFi command and call the callback function when a response or timeout is received */
// size=0 means that data contains complete command data, and code and size can be obtained directly 
// from data (for resending)
// retry can be 0(call the callback function directly the first time you fail, no more retries), 
// but timeout cannot be 0(otherwise you will be mistaken for a timeout and call the callback function before receiving the response)
//
// Only non-blocking WiFi commands can be executed in an no-operating system environment, 
// and the results of the command execution can be notified by the callback function 
// (the callback function should be guaranteed to be invoked and called only once).
// 
// If there is a operating system, a task to execute the WiFi in blocking the way command, 
// you can add a send in the function before the command block waiting for the result 
// indicates that the command channel availability of semaphores code 0 and 1
// When command channel is available, the semaphore value is 1 and awaken one waiting to send command task, 
// after sending command continue to block waiting for the response, 
// the callback function and (success or failure) according to the results of the command execution decision function return values
void WiFi_SendCommand(uint16_t code, const void *data, uint16_t size, 
                            WiFi_Callback callback, void *arg, uint32_t timeout, uint8_t max_retry)
{
    static uint16_t seq_num = 0;
    WiFi_CommandHeader *cmdhdr = (WiFi_CommandHeader *)wifi_buffer_command;

    // You must make sure that the previous command has been sent before you send the command
    if (WiFi_CheckCommandBusy(callback, arg)) 
        return;

    // Direct command: WiFi_SendCommand(0, data, 0, ...)
    // Populate the command header and send the command: WiFi_SendCommand(code, data, size, ...)
    // Retry the last command: WiFi_SendCommand(0, NULL, 0, ...)
    if (data != NULL && data != wifi_buffer_command){
        // The command content to be sent is copied to the buffer to be reissued when an error occurs
        memmove(wifi_buffer_command, data, (size != 0) ? size : cmdhdr->frame_header.length); 
    }

    if (size != 0)
    {
        cmdhdr->frame_header.length = size;
        cmdhdr->frame_header.type = WIFI_SDIOFRAME_COMMAND;
        cmdhdr->cmd_code = code;
        cmdhdr->size = size - sizeof(WiFi_SDIOFrameHeader); // The command size includes the command header, but not the SDIO frame header
        cmdhdr->seq_num = seq_num++;
        cmdhdr->result = 0;
    }
    else
        size = cmdhdr->frame_header.length; // Do not fill in cmdhdr when reissuing a command
    WiFi_LowLevel_WriteData(1, wifi_port, wifi_buffer_command, size, sizeof(wifi_buffer_command));
    // The WriteData function has a low probability of error, so I won't judge its return value for simplicity
    // Even if something goes wrong (such as a CRC validation error), 
    // the WiFi_CheckTimeout function repasses the command because the command response is not received

    wifi_tx_command.arg = arg;
    wifi_tx_command.busy = 1;
    wifi_tx_command.callback = callback;
    wifi_tx_command.ready = 0;
    wifi_tx_command.retry = max_retry;
    wifi_tx_command.start_time = sys_now();
    wifi_tx_command.timeout = timeout;
}

/* Send an EAPOL response frame */
static void WiFi_SendEAPOLResponse(const WiFi_EAPOLKeyFrame *packet_rx, uint16_t key_info, const void *key_data, 
                                    uint16_t key_data_len, WiFi_Callback callback, void *arg)
{
    uint8_t ret;
    uint16_t len;
    WiFi_EAPOLKeyFrame *packet_tx = (WiFi_EAPOLKeyFrame *)WiFi_GetPacketBuffer();
    WiFi_KeyType key_type = (WiFi_KeyType)(key_info & 0x07);
    WiFi_MIC mic;

    memcpy(packet_tx->dest, packet_rx->src, 6); // Dest MAC address
    memcpy(packet_tx->src, packet_rx->dest, 6); // The source MAC address
    packet_tx->type = htons(0x888e); // Big end order 0x888e: 802.1X Authentication
    packet_tx->version = packet_rx->version; // Protocol version number (MIC is calculated from this field)
    packet_tx->packet_type = packet_rx->packet_type; // Usually 3: Key
    packet_tx->packet_body_length = packet_tx->key_data - &packet_tx->descriptor_type + key_data_len;
    packet_tx->packet_body_length = htons(packet_tx->packet_body_length);

    packet_tx->descriptor_type = packet_rx->descriptor_type; // Descriptor type
    packet_tx->key_information = htons(key_info);
    packet_tx->key_length = packet_rx->key_length;
    memcpy(packet_tx->key_replay_counter, packet_rx->key_replay_counter, sizeof(packet_rx->key_replay_counter));
    memcpy(packet_tx->key_nonce, wifi_snonce, sizeof(wifi_snonce));
    memset(packet_tx->key_iv, 0, sizeof(packet_tx->key_iv) + sizeof(packet_tx->key_rsc) + sizeof(packet_tx->reserved));
    packet_tx->key_data_length = htons(key_data_len);
    if (key_data)
        memcpy(packet_tx->key_data, key_data, key_data_len);

    // The MIC was obtained by KCK on the EAPOL frame to be sent 
    // (the Ethernet frame took off two MAC address fields and the rest of the type/length=0x888e field)
    len = sizeof(WiFi_EAPOLKeyFrame) - sizeof(packet_tx->key_data) + key_data_len; // Total frame length
    memset(packet_tx->key_mic, 0, sizeof(packet_tx->key_mic)); // clear the MIC field
    if (key_type == WIFI_KEYTYPE_TKIP)
        ret = hmac_md5(wifi_ptk.KCK, sizeof(wifi_ptk.KCK), &packet_tx->version, len - 14, mic.MIC); // Calculate MIC
    else if (key_type == WIFI_KEYTYPE_AES)
        ret = hmac_sha1(wifi_ptk.KCK, sizeof(wifi_ptk.KCK), &packet_tx->version, len - 14, mic.MIC);
    else
    {
        printf("WiFi_SendEAPOLResponse: unsupported key type!\n");
        if (callback)
            callback(arg, packet_tx, WIFI_STATUS_INVALID);
        return;
    }
    if (!ret)
    {
        printf("WiFi_SendEAPOLResponse: out of memory!\n");
        if (callback)
            callback(arg, packet_tx, WIFI_STATUS_MEM);
        return;
    }

    memcpy(packet_tx->key_mic, mic.MIC, sizeof(mic.MIC)); // Put the calculated results on the MIC field
    WiFi_SendPacket(packet_tx, len, callback, arg, WIFI_DEFAULT_TIMEOUT_DATAACK);
}

/* Send data frame */
// data--->WiFi_DataTx.payload
// size=0 means that the last frame is sent again, when the data parameter is ignored
void WiFi_SendPacket(void *data, uint16_t size, WiFi_Callback callback, void *arg, uint32_t timeout)
{
    WiFi_DataTx *packet = (WiFi_DataTx *)wifi_buffer_packet;

    // Send new data frames: WiFi_SendPacket(data, size, ...)
    // Retransmit the previous data frame: WiFi_SendPacket(NULL, 0, ...)
    if (size == 0)
        data = packet->payload;

    WiFi_WaitForLastTask();
    if (data != packet->payload)
        memmove(packet->payload, data, size); 

    if (size != 0)
    {
        // For details on sending packets, refer to Chapter 3: Data Path for the Firmware Specification PDF
        packet->header.length = sizeof(WiFi_DataTx) - sizeof(packet->payload) + size;
        packet->header.type = WIFI_SDIOFRAME_DATA;

        packet->reserved1 = 0;
        packet->tx_control = 0; // Please refer to 3.2.1 Per-Packet Settings Settings for the format of control information
        packet->tx_packet_offset = sizeof(WiFi_DataTx) - sizeof(packet->payload) - sizeof(packet->header); // Do not include SDIOFrameHeader
        packet->tx_packet_length = size;
        memcpy((void *)&packet->tx_dest_addr_high, packet->payload, 6);
        packet->priority = 0;
        packet->flags = 0;
        packet->pkt_delay_2ms = 0;
        packet->reserved2 = 0;
    }
    WiFi_LowLevel_WriteData(1, wifi_port, wifi_buffer_packet, packet->header.length, sizeof(wifi_buffer_packet));

    // Next, you need to wait for the Download Ready position 1 to indicate that the data frame was sent successfully
    wifi_tx_packet.arg = arg;
    wifi_tx_packet.busy = 1;
    wifi_tx_packet.callback = callback;
    wifi_tx_packet.retry = 0;
    wifi_tx_packet.start_time = sys_now();
    wifi_tx_packet.timeout = timeout;
}

/* Send the PTK and GTK keys to the firmware for use */
static void WiFi_SetKeyMaterial(WiFi_KeyType key_type, uint8_t key_num, WiFi_Callback callback, void *arg)
{
    MrvlIETypes_KeyParamSet_t keys[2];
    uint16_t key_len;
    if (key_type == WIFI_KEYTYPE_TKIP)
        key_len = sizeof(wifi_ptk.TK) + sizeof(wifi_ptk.TKIPTxMICKey) + sizeof(wifi_ptk.TKIPRxMICKey);
    else if (key_type == WIFI_KEYTYPE_AES)
        key_len = sizeof(wifi_ptk.TK);
    else
    {
        if (callback)
            callback(arg, NULL, WIFI_STATUS_INVALID);
        return;
    }

    keys[0].key_type_id = key_type;
    keys[0].key_info = WIFI_KEYINFO_KEYENABLED | WIFI_KEYINFO_UNICASTKEY; // Unicast key
    keys[0].key_len = key_len;
    memcpy(keys[0].key, wifi_ptk.TK, sizeof(wifi_ptk.TK));
    if (key_type == WIFI_KEYTYPE_TKIP)
    {
        // The order of the key used for MIC validation in the firmware is just the opposite of the order in which the PRF function is generated
        memcpy(keys[0].key + sizeof(wifi_ptk.TK), wifi_ptk.TKIPRxMICKey, sizeof(wifi_ptk.TKIPRxMICKey));
        memcpy(keys[0].key + sizeof(wifi_ptk.TK) + sizeof(wifi_ptk.TKIPRxMICKey), wifi_ptk.TKIPTxMICKey, sizeof(wifi_ptk.TKIPTxMICKey));
    }

    if (key_num == 2)
    {
        keys[1].key_type_id = key_type;
        keys[1].key_info = WIFI_KEYINFO_KEYENABLED | WIFI_KEYINFO_MULTICASTKEY; // Multicast, broadcast key
        keys[1].key_len = key_len;
        memcpy(keys[1].key, wifi_gtk.TK, sizeof(wifi_gtk.TK));
        if (key_type == WIFI_KEYTYPE_TKIP)
        {
            memcpy(keys[1].key + sizeof(wifi_gtk.TK), wifi_gtk.TKIPRxMICKey, sizeof(wifi_gtk.TKIPRxMICKey));
            memcpy(keys[1].key + sizeof(wifi_gtk.TK) + sizeof(wifi_gtk.TKIPRxMICKey), wifi_gtk.TKIPTxMICKey, sizeof(wifi_gtk.TKIPTxMICKey));
        }
    }

    WiFi_KeyMaterial(WIFI_ACT_SET, keys, key_num, callback, arg);
}

/* Set the WEP key (length must be 5 or 13 characters) */
// action: WIFI_ACT_ADD / WIFI_ACT_REMOVE, The parameter key can be set to NULL when the key is removed
void WiFi_SetWEP(WiFi_CommandAction action, const WiFi_WEPKey *key, WiFi_Callback callback, void *arg)
{
    uint8_t i, j, len;
    uint16_t cmd_size;
    uint32_t temp;
    WiFi_Cmd_SetWEP *cmd = (WiFi_Cmd_SetWEP *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    cmd->action = action;
    cmd->tx_key_index = key->index;
    if (action == WIFI_ACT_ADD)
    {
        memset(cmd->wep_types, 0, sizeof(cmd->wep_types) + sizeof(cmd->keys));
        for (i = 0; i < 4; i++)
        {
            if (key->keys[i] == NULL)
                continue;

            len = strlen(key->keys[i]);
            if (len == 5 || len == 13)
            {
                // 5 or 13 ASCII key characters
                if (len == 5)
                    cmd->wep_types[i] = WIFI_WEPKEYTYPE_40BIT;
                else if (len == 13)
                    cmd->wep_types[i] = WIFI_WEPKEYTYPE_104BIT;
                memcpy(cmd->keys[i], key->keys[i], len);
            }
            else if (len == 10 || len == 26)
            {
                // 10 or 26 hexadecimal key characters
                if (len == 10)
                    cmd->wep_types[i] = WIFI_WEPKEYTYPE_40BIT;
                else if (len == 26)
                    cmd->wep_types[i] = WIFI_WEPKEYTYPE_104BIT;

                for (j = 0; j < len; j += 2)
                {
                    if (!isxdigit(key->keys[i][j]) || !isxdigit(key->keys[i][j + 1]))
                    {
                        printf("WiFi_SetWEP: The hex key %d contains invalid characters!\n", i);
                        if (callback)
                            callback(arg, NULL, WIFI_STATUS_INVALID);
                        return;
                    }

                    sscanf(key->keys[i] + j, "%02lx", &temp);
                    cmd->keys[i][j / 2] = temp;
                }
            }
            else
            {
                printf("WiFi_SetWEP: The length of key %d is invalid!\n", i);
                if (callback)
                    callback(arg, NULL, WIFI_STATUS_INVALID);
                return;
            }
        }
        cmd_size = sizeof(WiFi_Cmd_SetWEP);
    }
    else if (action == WIFI_ACT_REMOVE)
        cmd_size = cmd->wep_types - wifi_buffer_command;
    else
    {
        if (callback)
            callback(arg, NULL, WIFI_STATUS_INVALID);
        return;
    }
    WiFi_SendCommand(CMD_802_11_SET_WEP, wifi_buffer_command, cmd_size, 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Set the WPA password and generate PSK */
void WiFi_SetWPA(const char *ssid, const char *password)
{
    uint8_t ret = pbkdf2_hmac_sha1(password, strlen(password), ssid, strlen(ssid), 4096, wifi_psk, sizeof(wifi_psk));
    if (!ret)
        printf("WiFi_SetWPA: out of memory!\n");
}

/* Display WiFi module information */
void WiFi_ShowCIS(void)
{
    uint8_t data[255];
    uint8_t func, i, n, len;
    uint8_t tpl_code, tpl_link; // 16.2 Basic Tuple Format and Tuple Chain Structure
    uint32_t cis_ptr;

    n = WiFi_LowLevel_GetFunctionNum();
    for (func = 0; func <= n; func++)
    {
        // Gets the address of the CIS
        cis_ptr = (func << 8) | 0x9;
        cis_ptr  = WiFi_LowLevel_ReadReg(0, cis_ptr) | (WiFi_LowLevel_ReadReg(0, cis_ptr + 1) << 8) 
                    | (WiFi_LowLevel_ReadReg(0, cis_ptr + 2) << 16);
        printf("[CIS] func=%d, ptr=0x%08lx\n", func, cis_ptr);

        // Iterate through the CIS until the tail node
        while ((tpl_code = WiFi_LowLevel_ReadReg(0, cis_ptr++)) != CISTPL_END)
        {
            if (tpl_code == CISTPL_NULL)
                continue;

            tpl_link = WiFi_LowLevel_ReadReg(0, cis_ptr++); // The size of the data at this node
            for (i = 0; i < tpl_link; i++)
                data[i] = WiFi_LowLevel_ReadReg(0, cis_ptr + i);

            switch (tpl_code)
            {
            case CISTPL_VERS_1:
                printf("Product Information:");
                for (i = 2; data[i] != 0xff; i += len + 1)
                {
                    // Iterate through all the strings
                    len = strlen((char *)data + i);
                    if (len != 0)
                        printf(" %s", data + i);
                }
                printf("\n");
                break;
            case CISTPL_MANFID:
                // 16.6 CISTPL_MANFID: Manufacturer Identification String Tuple
                printf("Manufacturer Code: 0x%04x\n", *(uint16_t *)data); // TPLMID_MANF
                printf("Manufacturer Information: 0x%04x\n", *(uint16_t *)(data + 2)); // TPLMID_CARD
                break;
            case CISTPL_FUNCID:
                // 16.7.1 CISTPL_FUNCID: Function Identification Tuple
                printf("Card Function Code: 0x%02x\n", data[0]); // TPLFID_FUNCTION
                printf("System Initialization Bit Mask: 0x%02x\n", data[1]); // TPLFID_SYSINIT
                break;
            case CISTPL_FUNCE:
                // 16.7.2 CISTPL_FUNCE: Function Extension Tuple
                if (data[0] == 0)
                {
                    // 16.7.3 CISTPL_FUNCE Tuple for Function 0 (Extended Data 00h)
                    printf("Maximum Block Size: %d\n", *(uint16_t *)(data + 1));
                    printf("Maximum Transfer Rate Code: 0x%02x\n", data[3]);
                }
                else
                {
                    // 16.7.4 CISTPL_FUNCE Tuple for Function 1-7 (Extended Data 01h)
                    printf("Maximum Block Size: %d\n", *(uint16_t *)(data + 0x0c)); // TPLFE_MAX_BLK_SIZE
                }
                break;
            default:
                printf("[CIS Tuple 0x%02x] addr=0x%08lx size=%d\n", tpl_code, cis_ptr - 2, tpl_link);
                WiFi_DumpData(data, tpl_link);
            }
            // When TPL_LINK is 0xff, the current node is the tail node
            if (tpl_link == 0xff)
                break; 
            cis_ptr += tpl_link;
        }
    }
}

/* Create an ad-hoc WiFi hotspot */
// When creating a hotspot with a WEP password, cap_info=WIFI_CAPABILITY_PRIVACY
// When creating a cryptoless hotspot, cap_info=0
void WiFi_StartADHOC(const char *ssid, uint16_t cap_info, WiFi_Callback callback, void *arg)
{
    WiFi_Cmd_ADHOCStart *cmd = (WiFi_Cmd_ADHOCStart *)wifi_buffer_command;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;
    strncpy((char *)cmd->ssid, ssid, sizeof(cmd->ssid));
    cmd->bss_type = WIFI_BSS_INDEPENDENT;
    cmd->bcn_period = 100;
    cmd->reserved1 = 0;
    cmd->ibss_param_set.header.type = WIFI_MRVLIETYPES_IBSSPARAMSET;
    cmd->ibss_param_set.header.length = TLV_PAYLOADLEN(cmd->ibss_param_set);
    cmd->ibss_param_set.atim_window = 0;
    cmd->reserved2 = 0;
    cmd->ds_param_set.header.type = WIFI_MRVLIETYPES_PHYPARAMDSSET;
    cmd->ds_param_set.header.length = TLV_PAYLOADLEN(cmd->ds_param_set);
    cmd->ds_param_set.channel = 1;
    memset(cmd->reserved3, 0, sizeof(cmd->reserved3));
    cmd->cap_info = WIFI_CAPABILITY_IBSS | cap_info;
    memset(cmd->data_rate, 0, sizeof(cmd->data_rate));
    *(uint32_t *)cmd->data_rate = 0x968b8482;
    WiFi_SendCommand(CMD_802_11_AD_HOC_START, wifi_buffer_command, sizeof(WiFi_Cmd_ADHOCStart), 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Create an ad-hoc WiFi hotspot and set the password */
// The conn->mac_addr member is ignored
void WiFi_StartADHOCEx(const WiFi_Connection *conn, WiFi_Callback callback, void *arg)
{
    uint16_t ssid_len;
    void **p;
    WiFi_SecurityType *psecurity;
    if (WiFi_CheckCommandBusy(callback, arg))
        return;

    if (conn->security == WIFI_SECURITYTYPE_WPA || conn->security == WIFI_SECURITYTYPE_WPA2)
    {
        printf("WiFi_StartADHOCEx: WPA is not supported!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_INVALID);
        return;
    }

    ssid_len = strlen(conn->ssid);
    p = malloc(2 * sizeof(void *) + sizeof(WiFi_SecurityType) + ssid_len + 1);
    if (p == NULL)
    {
        printf("WiFi_StartADHOCEx: malloc failed!\n");
        if (callback)
            callback(arg, NULL, WIFI_STATUS_MEM);
        return;
    }
    p[0] = arg;
    p[1] = (void*)callback;
    psecurity = (WiFi_SecurityType *)(p + 2);
    *psecurity = conn->security;
    memcpy(psecurity + 1, conn->ssid, ssid_len + 1);

    if (conn->security == WIFI_SECURITYTYPE_WEP)
        WiFi_SetWEP(WIFI_ACT_ADD, conn->password, WiFi_StartADHOCEx_Callback, p);
    else
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, WiFi_StartADHOCEx_Callback, p);
}

static void WiFi_StartADHOCEx_Callback(void *arg, void *data, WiFi_Status status)
{
    void **p = (void **)arg;
    void *app_arg = p[0];
    WiFi_Callback app_callback = (WiFi_Callback)p[1];
    WiFi_SecurityType *psecurity = (WiFi_SecurityType *)(p + 2);
    char *ssid = (char *)(psecurity + 1);

    if (status != WIFI_STATUS_OK)
    {
        printf("WiFi_StartADHOCEx error!\n");
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        return;
    }

    switch (WiFi_GetCommandCode(data))
    {
    case CMD_802_11_SET_WEP:
        WiFi_MACControl(WIFI_MACCTRL_ETHERNET2 | WIFI_MACCTRL_WEP | WIFI_MACCTRL_TX | WIFI_MACCTRL_RX, 
                        WiFi_StartADHOCEx_Callback, arg);
        break;
    case CMD_MAC_CONTROL:
        if (*psecurity == WIFI_SECURITYTYPE_NONE)
            WiFi_StartADHOC(ssid, 0, WiFi_StartADHOCEx_Callback, arg);
        else
            WiFi_StartADHOC(ssid, WIFI_CAPABILITY_PRIVACY, WiFi_StartADHOCEx_Callback, arg);
        break;
    case CMD_802_11_AD_HOC_START:
        free(arg);
        if (app_callback)
            app_callback(app_arg, data, status);
        break;
    }
}

void WiFi_StopADHOC(WiFi_Callback callback, void *arg)
{
    WiFi_SendCommand(CMD_802_11_AD_HOC_STOP, wifi_buffer_command, sizeof(WiFi_CommandHeader), 
                    callback, arg, WIFI_DEFAULT_TIMEOUT_CMDRESP, WIFI_DEFAULT_MAXRETRY);
}

/* Release the send buffer and call the callback function */
static void WiFi_TxBufferComplete(WiFi_TxBuffer *tbuf, void *data, WiFi_Status status)
{
    if (tbuf->busy)
    {
        tbuf->busy = 0;
        if (tbuf->callback)
            tbuf->callback(tbuf->arg, data, status); 
    }
}

/* Convert IEEE TLV into MrvlIE TLV */
uint8_t WiFi_TranslateTLV(MrvlIEType *mrvlie_tlv, const IEEEType *ieee_tlv, uint16_t mrvlie_payload_size)
{
    mrvlie_tlv->header.type = ieee_tlv->header.type;
    if (ieee_tlv->header.length > mrvlie_payload_size){
        // If the source data size exceeds the maximum buffer capacity, the remaining data is discarded
        mrvlie_tlv->header.length = mrvlie_payload_size; 
    }else{
        mrvlie_tlv->header.length = ieee_tlv->header.length;
    }
    memset(mrvlie_tlv->data, 0, mrvlie_payload_size); 
    memcpy(mrvlie_tlv->data, ieee_tlv->data, mrvlie_tlv->header.length); // Copy the data
    // The return value indicates whether the buffer size is sufficient
    return mrvlie_tlv->header.length == ieee_tlv->header.length; 
}

/* Within the specified timeout period, wait for the specified card status location bit 
 * and clear the corresponding interrupt flag bit */
// Return 1 on success
uint8_t WiFi_Wait(uint8_t status, uint32_t timeout)
{
    if (timeout != 0)
        timeout += sys_now();

    while ((WiFi_LowLevel_ReadReg(1, WIFI_INTSTATUS) & status) != status)
    {
        if (timeout != 0 && sys_now() > timeout)
        {
            printf("WiFi_Wait(0x%02x): timeout!\n", status);
            return 0;
        }
    }

    // Clear the corresponding interrupt flag bit
    // The bits that do not need to be cleared must be 1
    WiFi_LowLevel_WriteReg(1, WIFI_INTSTATUS, WIFI_INTSTATUS_ALL & ~status); 
    // Can't remove the SDIOIT bit! Otherwise it may cause the position to never be placed
    return 1;
}

/* Wait for the previously sent command frame or data frame to receive confirmation */
void WiFi_WaitForLastTask(void)
{
    int32_t remaining;
#ifdef WIFI_DISPLAY_RESPTIME
    WiFi_CommandHeader *tx_cmd = (WiFi_CommandHeader *)wifi_buffer_command;
#endif

    // Before executing the CMD53 command to send data, 
    // you must wait for the previously sent data to be confirmed by DNLDRDY
    // Note: CMDBUSY=DATABUSY=1 and CMDRDY=0 are impossible
    while ((wifi_tx_command.busy && wifi_tx_command.ready == 0) || wifi_tx_packet.busy)
    {
        if (wifi_tx_command.busy && wifi_tx_command.ready == 0)
        {
            remaining = wifi_tx_command.start_time + WIFI_DEFAULT_TIMEOUT_CMDACK - sys_now() + 1;
            if (remaining > 0)
            {
                if (WiFi_Wait(WIFI_INTSTATUS_DNLD, remaining))
                {
#ifdef WIFI_DISPLAY_RESPTIME
                    printf("-- CMD 0x%04x ACK at %ldms\n", tx_cmd->cmd_code, sys_now() - wifi_tx_command.start_time);
#endif
                }
            }
            wifi_tx_command.ready = 1;
            
            // Now that the command either timeout or has been confirmed, 
            // it is not responsible for repassing the command
        }

        if (wifi_tx_packet.busy)
        {
            // Just wait for the download ready position bit, no matter the new data frame (upload ready)

            // The remaining time +1 (less than or equal to 0 indicates the timeout)
            remaining = wifi_tx_packet.start_time + wifi_tx_packet.timeout - sys_now() + 1; 
            // Wait for the flag position within the remaining time +1 and clear the interrupt position
            if (remaining > 0 && WiFi_Wait(WIFI_INTSTATUS_DNLD, remaining)) 
            {
#ifdef WIFI_DISPLAY_RESPTIME
                printf("-- Packet ACK at %ldms\n", sys_now() - wifi_tx_packet.start_time);
#endif
                // If DNLDRDY is set, the data frame is sent successfully, 
                // the busy is reset and the corresponding callback function is called
                WiFi_TxBufferComplete(&wifi_tx_packet, wifi_buffer_packet, WIFI_STATUS_OK); 

                // If a new frame is sent in the callback function, then busy is still equal to 1 
                // and you need to keep waiting, so you can't write break here
            }
            else
            {
                // Notifies the callback function of timeout
                WiFi_CheckTxBufferRetry(&wifi_tx_packet, wifi_buffer_packet); 
            }
        }
    }
}

/* Private functions --------------------------------------------------------*/

