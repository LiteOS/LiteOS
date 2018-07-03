/********************************************************************
 * M8266WIFIDrv.h
 * .Description
 *     Header file of M8266WIFI Dirver Layer
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 * .Author:	wzuo(wzuo@anylinkin.com, 1521340710@qq.com)
 * .History
 *     - V1.1.6-M  20180208
 ********************************************************************/
#ifndef _M8266WIFI_H_
#define _M8266WIFI_H_

//#include "ht32f1655_56.h"

/*
//These for TMS320F28335 CSS
typedef unsigned          char uint8_t;
typedef unsigned int  	  uint16_t;
typedef unsigned long  	  uint32_t;
typedef   signed          char sint8_t;

typedef uint32_t  		  u32;
typedef uint16_t 		  u16;
typedef uint8_t  		  u8;
typedef sint8_t           s8;

*/

typedef unsigned          char uint8_t;
typedef unsigned short    int  uint16_t;
typedef unsigned          int  uint32_t;
typedef   signed          char sint8_t;

typedef uint32_t  				u32;
typedef uint16_t 					u16;
typedef uint8_t  					u8;
typedef sint8_t           s8;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   1. APIs releated to SPI Host I/F Function Configuration and Performance Test // 
//       - M8266HostIf_SPI_Select                                        4.1.1    //
//       - M8266WIFI_SPI_Interface_Communication_OK                      4.1.2    //
//       - M8266WIFI_SPI_Interface_Communication_Stress_Test             4.1.3    //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266HostIf_SPI_Select                                                          *
 * Description                                                                     *
 *  1.To specify the SPI used by providing the base address of SPI used            *
 *  2.Called when Initial M8266WIFI Module before perform SPI read/write           *
 * Parameter(s):                                                                   *
 *  1. spi_base_addr: the base address of used                                     *
 *     you may use M8266WIFI_INTERFACE_SPI defined in brd_cfg.h as example         *
 *  2. spi_clock: the spi_clock you set in HZ                                      *
 *     e.g.  10500000 for 10.5MHz                                                  *
 *  3. status when error                                                           *
 * Return:                                                                         *
 *   1. success                                                                    *
 *   0. failure                                                                    *
 ***********************************************************************************/
u8 M8266HostIf_SPI_Select(uint32_t spi_base_addr, uint32_t spi_clock, u16* status);

void M8266HostIf_SPI_DMA_Select(uint32_t spi_tx_dam_base_addr, uint32_t spi_tx_dam_stream_base_addr, 
																uint32_t spi_rx_dam_base_addr, uint32_t spi_rx_dam_stream_base_addr);

/***********************************************************************************
 * M8266WIFI_SPI_Interface_Communication_OK                                        *
 * Description                                                                     *
 *  1. To write a byte and then read out from M8266WIFI module SPI registers       *
 *     to check whether the logical fundamentai SPI communication (read/write)     *
 *     is stablly OK under the clock and interconnection                           *
 *  2. Called during the initialization of M8266WIFI Module                        *
 *     DONOT CALL IT AFTER MODULE HAS STARTED THE NORMAL OPERATION                 *
 *  3. You may repeat calling it in your debug code for speed stablility test      *
 *     during your stress performance test to your produc                          * 
 * Parameter(s):                                                                   *
 *  1. byte:  a pointer to the byte read out during test                           *
 *     - normally the data should be 0x45 during test.                             *
 *       if it is 0x45, this function will return 1 for success as well            *
 *     - If readout other value, it may indicating the fundamental SPI             *
 *       communication is not OK. e.g. 0xFF may indicates that your spi            *
 *       port has problem such incoreect interconnection or initialization         *
 *     - user NULL if you don't expect this data                                   * 
 * Return:                                                                         *
 *   1. success                                                                    *
 *   0. failure                                                                    *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Interface_Communication_OK(u8* byte);	

/***********************************************************************************
 * M8266WIFI_SPI_Interface_Communication_Stress_Test                               *
 * Description                                                                     *
 *  1. To perform a batch of byte write and then read out from M8266WIFI module    *
 *     SPI register to check whether the high-speed and bulk fundamentai SPI       *
 *     communication (read/write) is stablly OK under the clock                    *
 *  2. Called during the initialization of M8266WIFI Module                        *
 *     DONOT CALL IT AFTER MODULE HAS STARTED THE NORMAL OPERATION                 *
 *  3. You may call it in your debug code for speed stablility test                *
 *     during your stress performance test to your produc                          * 
 * Parameter(s):                                                                   *
 *  1. max_times:  the max write-read times used for the stress test               *
 *     - And the test data to be written will be nimber byte of data               *
 *       from 0 to  max_times                                                      *
 * Return:                                                                         *
 *  1. success times of write-read-verify                                          *
 ***********************************************************************************/
u32 M8266WIFI_SPI_Interface_Communication_Stress_Test(u32 max_times);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   2. APIs releated to Module WIFI fundamental Query and Configurations via SPI //
//       - M8266WIFI_SPI_Get_Opmode                                      4.2.1    //
//       - M8266WIFI_SPI_Set_Opmode                                      4.2.2    //
//                                                                                //
//       - M8266WIFI_SPI_STA_Connect_Ap                                  4.2.3    //
//       - M8266WIFI_SPI_Get_STA_Connection_Status                       4.2.4    //
//       - M8266WIFI_SPI_Get_STA_IP_Addr                                 4.2.5    //
//       - M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI                 4.2.6    //
//       - M8266WIFI_SPI_STA_Scan_Signals                                4.2.7    //
//       - M8266WIFI_SPI_Set_STA_Hostname                                4.2.8    //
//       - M8266WIFI_SPI_Get_STA_Hostname                                4.2.9    //
//       - M8266WIFI_SPI_Query_STA_Param                                 4.2.10   //
//                                                                                //
//       - M8266WIFI_SPI_Config_AP                                  		 4.2.11   //
//       - M8266WIFI_SPI_Query_AP_Param                             		 4.2.12   //
//       - M8266WIFI_SPI_Config_AP_Param                             		 4.2.13   //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266WIFI_SPI_Get_Opmode                                                        *
 * .Description:                                                                   *
 *     To get the op_mode(STA, AP, or STA+AP) of M8266WIFI module via SPI          *
 * .Parameter(s)                                                                   *
 *     1. op_mode : pointer to the op_mode returned                                *
 *        =1, 			STA mode                                                       *
 *        =2, 			AP mode                                                        *
 *        =3, 			STA+AP mode                                                    *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_Opmode(u8* op_mode, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Set_Opmode                                                        *
 * .Description:                                                                   *
 *     To set op_mode(STA, AP, or STA+AP) of M8266WIFI module via SPI              *
 * .Parameter(s)                                                                   *
 *     1. op_mode : the op_mode to set                                             *
 *        =1, 			set to STA mode                                                *
 *        =2, 			set to AP mode                                                 *
 *        =3, 			set to STA+AP mode                                             *
 *        =others, 	set to STA+AP mode                                             *
 *     2. saved   : to save into flash the opmode or not                           *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Set_Opmode(u8 op_mode, u8 saved, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_STA_Connect_Ap                                                    *
 * .Description:                                                                   *
 *     To connect the M8266WIFI STA to an AP or router via SPI                     *
 * .Parameter(s)                                                                   *
 *     1. ssid    : the ssid of AP connected to. Max len = 32 Bytes                *
 *     2. password: the passowrd of AP connecting to. Max len = 64 Bytes           *
 *     3. saved   : to save the ssid and password into flash the opmode or not     *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     4. timeout_in_s: max time in seconds waiting for being connected            *
 *     5. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_STA_Connect_Ap(u8 ssid[32], u8 password[64], u8 saved, u8 timeout_in_s, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Get_STA_Connection_Status                                         *
 * .Description:                                                                   *
 *     To get the connecting status of M8266WIFI STA to AP or routers via SPI      *
 * .Parameter(s)                                                                   *
 *     1. connection_status :  the connection status returned                      *
 *          =0, if station is in idle                                              *
 *          =1, if station is connecting to an AP                                  *
 *          =2, if station is to connect to an AP but with an wrong password       *
 *          =3, if station is to connect to an AP which is not found               *
 *          =4, if station failed to connect to the AP                             *
 *          =5, if station is connected to the AP and has got the IP successfully  *
 *          =255, if the module is in AP-only mode                                 *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_STA_Connection_Status(u8* connection_status, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Get_STA_IP_Addr                                                   *
 * .Description:                                                                   *
 *     To get ip address of M8266WIFI STA via SPI                                  *
 * .Parameter(s)                                                                   *
 *     1. sta_ip :  the sta ip address returned if successful                      *
 *                  "0.0.0.0" returned if in AP-only mode or ip not achieved       *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_STA_IP_Addr(char* sta_ip , u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI                                   *
 * .Description:                                                                   *
 *     To get current AP info (SSID and RSSI) M8266WIFI STA connected to           *
 * .Parameter(s)                                                                   *
 *     1. ssid    : the current SSID returned which m8266wifi sta connected to     *
 *     2. rssi    : the rssi of current connected ssid. 31 if error                *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(char* ssid, s8* rssi, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_STA_Scan_Signals                                                  *
 * .Description:                                                                   *
 *     To perform a scanning procedure, the scanned signals sorted by rssi         *
 * .Parameter(s)                                                                   *
 *     1. scanned_signals : the return signals after scanning                      *
 *     2. max_signals     : max counts of signals to scan                          *
 *     3. channel         : channel to scan. USE 0xFF to scan all channles         *
 *     4. timeout_in_s    : time out in seconds                                    *
 *     5. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *    !=0, signal numbers scanned succesfully                                      *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 struct ScannedSigs{
	char ssid[32];
	u8   channel;
	u8   authmode;
	s8   rssi;
};
u8 M8266WIFI_SPI_STA_Scan_Signals(struct ScannedSigs scanned_signals[], u8 max_signals, 
																  u8 channel, u8 timeout_in_s, u16* status);

/**********************************************************************************
 * M8266WIFI_SPI_Get_STA_Hostname								                                  *
 * .Description:																																	*
 *     To get the hostname of the STA via SPI                                     *
 *     Note: Will return failure if module in AP-Only Mode                        *
 * .Parameter(s)																																	*
 *     1. hostbame 		: the hostame to get  		       														*
 *     2. status  		: pointer to the status upon failure												*
 * .Return value:																																	*
 *     =1, success																																*
 *     =0, has error(s)																														*
 **********************************************************************************/
u8 M8266WIFI_SPI_Get_STA_Hostname(char hostname[28+1], u16* status);

/**********************************************************************************
 * M8266WIFI_SPI_Set_STA_Hostname								                                  *
 * .Description:																																	*
 *     To set up the hostname of the STA via SPI                                  *
 *     Note: Will return failure if module in AP-Only Mode                        *
 * .Parameter(s)																																	*
 *     1. hostbame 		: the hostame to setup		       														*
 *     2. status  		: pointer to the status upon failure												*
 * .Return value:																																	*
 *     =1, success																																*
 *     =0, has error(s)																														*
 **********************************************************************************/
u8 M8266WIFI_SPI_Set_STA_Hostname(char hostname[28+1], u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Query_STA_Param                                                   *
 * .Description:                                                                   *
 *     To query a parameter of the module STA via SPI                              *
 * .Parameter(s)                                                                   *
 *     1. param_type : the parame type to set, enum of STA_PARAM_TYPE      				 *
 *                      STA_PARAM_TYPE_SSID				= 0,                             *
 *                      STA_PARAM_TYPE_PASSWORD		= 1,                             *
 *                      STA_PARAM_TYPE_CHANNEL		= 2,                             *
 *                      STA_PARAM_TYPE_HOSTNAME		= 3,                             *
 *                      STA_PARAM_TYPE_RSSI				= 6,                             *
 *                      STA_PARAM_TYPE_IP_ADDR		= 7,                             *
 *     2. param      : pointer to the param value returned                         *
 *     3. param_len  : pointer to length the param, unit in bytes                  *
 *     4. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
typedef enum{
	STA_PARAM_TYPE_SSID				= 0,
	STA_PARAM_TYPE_PASSWORD		= 1,
	STA_PARAM_TYPE_CHANNEL		= 2,
	STA_PARAM_TYPE_HOSTNAME		= 3,
	STA_PARAM_TYPE_USE_BSSID	= 4,
	STA_PARAM_TYPE_BSSID			= 5,
	STA_PARAM_TYPE_RSSI				= 6,
	STA_PARAM_TYPE_IP_ADDR		= 7,
	
}STA_PARAM_TYPE;
u8 M8266WIFI_SPI_Query_STA_Param(STA_PARAM_TYPE param_type, u8* param, u8* param_len, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Config_AP                                                         *
 * .Description:                                                                   *
 *     To config the module AP via SPI                                             *
 * .Parameter(s)                                                                   *
 *     1. ssid    : the ssid of AP, Max Size=13 Bytes                              *
 *     2. password: the passowrd of AP, Max Size=13 Bytes                          *
 *     3. enc     : authorisation mode, valid value is 0~4                         *
 *                  = 0, OPEN                                                      *
 *                  = 1, WEP                                                       *
 *                  = 2, WPA_PSK                                                   *
 *                  = 3, WPA2_PSK                                                  *
 *                  = 4, WPA_WPA2_PSK                                              *
 *                  = others, OPEN                                                 *
 *     4. channel: the channel of AP, valid value is 1~13.                         *
 *        - Value other than 1~13 will set to channel 6                            *
 *     5. saved   : to save the ssid and password into flash the opmode or not     *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     6. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Config_AP(u8 ssid[13+1], u8 password[13+1], u8 enc, u8 channel, u8 saved, u16* status);

typedef enum{
	AP_PARAM_TYPE_SSID 						= 0,
	AP_PARAM_TYPE_PASSWORD 				= 1,
	AP_PARAM_TYPE_CHANNEL 				= 2,
	AP_PARAM_TYPE_AUTHMODE 				= 3,
	AP_PARAM_TYPE_SSID_HIDDEN 		= 4,
	AP_PARAM_TYPE_MAX_CONNECT 		= 5,
	AP_PARAM_TYPE_BEACON_INTERVAL = 6,	
	AP_PARAM_TYPE_IP_ADDR					= 7
}AP_PARAM_TYPE; 
/***********************************************************************************
 * M8266WIFI_SPI_Query_AP_Param                                                    *
 * .Description:                                                                   *
 *     To query a parameter of the module AP via SPI                               *
 * .Parameter(s)                                                                   *
 *     1. param_type : the parame type to set, enum of AP_PARAM_TYPE       				 *
 *                      AP_PARAM_TYPE_SSID 						= 0,												 *
 *                      AP_PARAM_TYPE_PASSWORD 				= 1,												 *
 *                      AP_PARAM_TYPE_CHANNEL 				= 2,												 *
 *                      AP_PARAM_TYPE_AUTHMODE 				= 3,												 *
 *                      AP_PARAM_TYPE_SSID_HIDDEN 		= 4,												 *
 *                      AP_PARAM_TYPE_MAX_CONNECT 		= 5,												 *
 *                      AP_PARAM_TYPE_BEACON_INTERVAL = 6,												 *
 *                      AP_PARAM_TYPE_IP_ADDR					= 7												   *
 *     2. param      : pointer to the param value returned                         *
 *     3. param_len  : pointer to length the param, unit in bytes                  *
 *     4. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Query_AP_Param(AP_PARAM_TYPE param_type, u8* param, u8* param_len, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Config_AP_Param                                                   *
 * .Description:                                                                   *
 *     To config a parameter of the module AP via SPI                              *
 * .Parameter(s)                                                                   *
 *     1. param_type : the parame type to set, enum of AP_PARAM_TYPE       				 *
 *                      AP_PARAM_TYPE_SSID 						= 0,												 *
 *                      AP_PARAM_TYPE_PASSWORD 				= 1,												 *
 *                      AP_PARAM_TYPE_CHANNEL 				= 2,												 *
 *                      AP_PARAM_TYPE_AUTHMODE 				= 3,												 *
 *                      AP_PARAM_TYPE_SSID_HIDDEN 		= 4,												 *
 *                      AP_PARAM_TYPE_MAX_CONNECT 		= 5,												 *
 *                      AP_PARAM_TYPE_BEACON_INTERVAL = 6,												 *
 *                      AP_PARAM_TYPE_IP_ADDR					= 7												   *
 *     2. param      : pointer to the param value                                  *
 *     3. param_len  : length the param, unit in bytes                             *
 *     4. saved   : to save the param into flash or not												     *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     5. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Config_AP_Param(AP_PARAM_TYPE param_type, u8* param, u8 param_len, u8 saved, u16* status);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   3. APIs releated to Module WIFI TCP/UDP Connection Operations via SPI        //
//       - M8266WIFI_SPI_Setup_Connection                                4.3.1    //
//       - M8266WIFI_SPI_Delete_Connection                               4.3.2    //
//       - M8266WIFI_SPI_Query_Connection                                4.3.3    //
//                                                                                //
//       - M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout               4.3.4    //
//       - M8266WIFI_SPI_Op_Multicuast_Group														 4.3.5    //
//       - SPI_STA_Get_HostIP_by_HostName                                4.3.6    //
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * M8266WIFI_SPI_Setup_Connection                                                  *
 * .Description:                                                                   *
 *     To setup a UDP connection or an TCP client connection via SPI               *
 * .Parameter(s)                                                                   *
 *     1. tcp_udp      : connection type                                           *
 *                       =0, udp                                                   *
 *                       =1, tcp client                                            *
 *                       =2, tcp server                                            *
 *     2. local_port   : local_port specified                                      *
 *                       =0, M8266WIFI module will generate a random local port    *
 *                      !=0, the local_port specified here will be used            *
 *     3. remote_addr  : string of ip or dns address of the remote connection      *
 *     4. remote_port  : port of remote connection                                 *
 *     5. link_no      : the number of link used for multiple links. Max 4         *
 *     6. timeout_in_s : the max timeout connecting to a remote, unit in seconds   *
 *     7. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, 
																	char* remote_addr, u16 remote_port, 
																	u8 link_no, u8 timeout_in_s, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Delete_Connection                                                 *
 * .Description:                                                                   *
 *     To delete/disconnect a client connection on M8266WIFI via SPI               *
 * .Parameter(s)                                                                   *
 *     1. link_no : the number of link to be deleted/disconnected                  *
 *     2. status  : pointer to the status upon failure                             *
 *     6. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Delete_Connection(u8 link_no, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Query_Connection                                                  *
 * .Description:                                                                   *
 *     To Query the information about a connection on M8266WIFI via SPI            *
 *     including connection type, state, local port, remote_ip and port, etc       *
 * .Parameter(s)                                                                   *
 *     1. link_no 				: the number of link to query                            *
 *     2. connection_type	:	pointer to the connection type returned
 *                          If you don't expected this value returned, just give NULL
 *                          = 0, udp
 *                          = 1, tcp Client
 *                          = 2, tcp Server
 *                          = others, invalid
 *                          Use NULL if you don't expect it returned               *
 *     3. connection_state:	pointer to the connection state returned               *
 *                          = 0, invalid                                           *
 *                          = 1, wait                                              *
 *                          = 2, listen                                            *
 *                          = 3, connect                                           *
 *                          = 4, write                                             *
 *                          = 5, read                                              *
 *                          = 6, close                                             *
 *                          = others, invalid                                      *
 *                          Use NULL if you don't expect it returned               *
 *     4. local_port      : pointer to the local port returned                     *
 *                          Use NULL if you don't expect it returned               *
 *     5. remote_ip       : pointer to the remote ip returned                      *
 *                          Use NULL if you don't expect it returned               *
 *     6. remote_port     : pointer to the remote port returned                    *
 *                          Use NULL if you don't expect it returned               *
 *     7. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Query_Connection(u8 link_no, u8* connection_type, u8* connection_state, u16* local_port, u8* remote_ip, u16* remote_port, u16* status);
typedef enum{
		ANYLINKIN_WIFI_LINK_CONNECTION_STATE_INVALID	=	0,
		ANYLINKIN_WIFI_LINK_CONNECTION_STATE_WAIT	    =	1,
		ANYLINKIN_WIFI_LINK_CONNECTION_STATE_LISTEN	  =	2,
		ANYLINKIN_WIFI_LINK_CONNECTION_STATE_CONNECT  =	3,
	  ANYLINKIN_WIFI_LINK_CONNECTION_STATE_WRITE    =	4,
	  ANYLINKIN_WIFI_LINK_CONNECTION_STATE_READ     =	5,
	  ANYLINKIN_WIFI_LINK_CONNECTION_STATE_CLOSE    =	6
} ANYLINKIN_WIFI_LINK_CONNECTION_STATES;

/**********************************************************************************
 * M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout                                *
 * .Description:																																	*
 *     To set up the tcp server's auto-disconnection timeout time 								*
 *     when no communication from clients via SPI																	*
 * .Parameter(s)																																	*
 *     1. link_no 		: the number of link to setup																*
 *     2. timeout_in_s: the timeout in seconds the tcp server will auto 					*
 *                      disconnect the connection when no communication						*
 *										  from clients																							*
 *     2. status  		: pointer to the status upon failure												*
 * .Return value:																																	*
 *     =1, success																																*
 *     =0, has error(s)																														*
 **********************************************************************************/
u8 M8266WIFI_SPI_Set_TcpServer_Auto_Discon_Timeout(u8 link_no, u16 timeout_in_s, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Op_Multicuast_Group                                               *
 * .Description:                                                                   *
 *     To join or leave a multicuast groupo AP via SPI                             *
 *     The module should be in STA or STA+AP mode and have connected               *
 *     to a routers before call this API                                           *
 * .Parameter(s)                                                                   *
 *     1. join_not_leave : join or leave the multicuast groud       				       *
 *        =1, join                                          											 *
 *        =0, leave                                          											 *
 *     2. multicust_group_ip : the ip address of a multicuast groud       				 *
 *        e.g. "224.6.6.6".                                                        *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Op_Multicuast_Group(u8 join_not_leave, char multicust_group_ip[15+1], u16* status);

/***********************************************************************************
 * SPI_STA_Get_HostIP_by_HostName                                                  *
 * .Description:                                                                   *
 *     To get the host ip_addr by hostName                                         *
 * .Parameter(s)                                                                   *
 *     1. hostIp      : the host ip_addr returned                                  *
 *     2. hostName    : the host name to get ip                                    *
 *     3. timeout_in_s: timeout in seconds                                         *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 SPI_STA_Get_HostIP_by_HostName(char* hostIp, char* hostName, u8 timeout_in_s, u16* status);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   4. APIs releated to TCP/UDP Packets TXD and RXD via SPI                      //
//       - M8266WIFI_SPI_Send_Data                                       4.4.1    //
//       - M8266WIFI_SPI_Has_DataReceived                                4.4.2    //
//       - M8266WIFI_SPI_RecvData                                        4.4.3    //
//       - M8266WIFI_SPI_RecvData_ex                                     4.4.4    //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266WIFI_SPI_Send_Data                                                         *
 * .Description:                                                                   *
 *     To send Data to WIFI via M8266 module SPI                                   *
 * .Parameters                                                                     *
 *     1. Data   : the pointer to the Data buffer to be sent                       *
 *     2. len    : the length the Data buffer to be sent                           *
 *     3. link_no: the wifi service link number sent to                            *
 *     4. pointer to return errcode(LSB) and status(MSB) when error encountered    *
 *        use NULL if you don't expect errcode and status                          *
 *        errcode:                                                                 *
 *            = 0x10: timeout when wait Module spi rxd Buffer ready                *
 *            = 0x11: timeout when wait wifi to send data                          *
 *            = 0x12: Module Sending Buffer full                                   *
 *              Module Buffer full defined as                                      *
 *              - If total size of packets waiting to send via WIFI > 5*1024, or   *
 *              - If counts     of packets waiting to send via WIFI > 8            *
 *            = 0x13: Wrong link_no used                                           *
 *            = 0x14: connection by link_no not present                            *
 *            = 0x15: connection by link_no closed                                 *
 *            = 0x18: No clients connecting to this TCP server                     *
 *            = 0x1F: Other errors                                                 *
 * .Return value:                                                                  *
 *     Actually length that has been sent successfuuly                             *
 ***********************************************************************************/
u16 M8266WIFI_SPI_Send_Data(u8 Data[], u16 Data_len, u8 link_no, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Has_DataReceived                                                  *
 * .Description:                                                                   *
 *     To check whether the M8266WIFI module has received data awaiting master     *
 *     to fetch away                                                               *
 * .Parameters                                                                     *
 *     None                                                                        *
 * .Return value:                                                                  *
 *     1, if the M8266WIFI module has received data from wifi                      *
 *     0, if the M8266WIFI module has not received data from wifi                  *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Has_DataReceived(void);

/***********************************************************************************
 * M8266WIFI_SPI_RcvData                                                           *
 * .Description:                                                                   *
 *     To receive the wifi data from M8266WIFI                                     *
 * .Parameters                                                                     *
 *     1. Data           - the buffer to contained the received Data               *
 *     2. max_len        - the max length of Data to fetech                        *
 *     3. max_wait_in_ms - the max timeout to wait for the Data                    *
 *     4. link_no        - pointer to return the link_no that current wifi Data    *
 *                         come from. use NULL if you don't expect it returned     *
 *     5. status         - pointer to return errcode(LSB) and status(MSB)          *
 *                         when error encountered. Use NULL if you don't expect    *
 *                         them returned                                           *
 *        errcode:                                                                 *
 *            = 0x20: timeout when wait module has received data via WIFI          *
 *            = 0x22: no date in Module wifi receive buffer                        *
 *            = 0x23: Read data from the left of last packets in Module wifi       *
 *                    receive buffer                                               *
 *            = 0x24: The packet in the Module wifi receive buffer is larger       *
 *                    in size than the max_len specified here. Only part of        *
 *                    the packet received                                          *
 *            = 0x2F: Other errors                                                 *
 * .Return value:                                                                  *
 *     - the size of larger packet first in the Module wifi receive buffer         *
 *       if errcode = 0x24                                                         *
 *     - the actual length of wifi packet received successfully                    *
 *       if others                                                                 *
 ***********************************************************************************/
u16 M8266WIFI_SPI_RecvData(u8 Data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_RcvData_ex                                                        *
 * .Description:                                                                   *
 *     To receive the wifi data from M8266WIFI, extended                           *
 *     Compared with M8266WIFI_SPI_RcvData(), this function also return the        *
 *     source remote_ip and remote_port meanwhile                                  *
 * .Parameters                                                                     *
 *     1. Data           - the buffer to contained the received Data               *
 *     2. max_len        - the max length of Data to fetech                        *
 *     3. max_wait_in_ms - the max timeout to wait for the Data                    *
 *     4. link_no        - pointer to return the link_no that current wifi Data    *
 *                         come from. use NULL if you don't expect it returned     *
 *     5. remote_ip      - array[4] to return the remote_ip that current wifi Data *
 *                         come from. use NULL if you don't expect it returned     *
 *                         e.g. if remote ip is "192.168.4.2", then remote_ip will *
 *                         return with remote_ip[0]=192, remote_ip[1]=168,         *
 *                         remote_ip[2]=4, and remote_ip[3]=2                      *
 *     6. remote_ip      - pointer to return the remote_port that current wifi Data*
 *                         come from. use NULL if you don't expect it returned     *
 *     7. status         - pointer to return errcode(LSB) and status(MSB)          *
 *                         when error encountered. Use NULL if you don't expect    *
 *                         them returned                                           *
 *        errcode:                                                                 *
 *            = 0x20: timeout when wait module has received data via WIFI          *
 *            = 0x22: no date in Module wifi receive buffer                        *
 *            = 0x23: Read data from the left of last packets in Module wifi       *
 *                    receive buffer                                               *
 *            = 0x24: The packet in the Module wifi receive buffer is larger       *
 *                    in size than the max_len specified here. Only part of        *
 *                    the packet received                                          *
 *            = 0x2F: Other errors                                                 *
 * .Return value:                                                                  *
 *     - the size of larger packet first in the Module wifi receive buffer         *
 *       if errcode = 0x24                                                         *
 *     - the actual length of wifi packet received successfully                    *
 *       if others                                                                 *
 ***********************************************************************************/
u16 M8266WIFI_SPI_RecvData_ex(u8 Data[], u16 max_len, uint16_t max_wait_in_ms, 
															u8* link_no, u8 remote_ip[4], u16* remote_port, u16* status);

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   5. APIs releated to Smart Config via SPI                                     //
//       - M8266WIFI_SPI_StartSmartConfig                                4.5.1    //
//       - M8266WIFI_SPI_StartWpsConfig                                  4.5.2    //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266WIFI_SPI_StartSmartConfig                                                  *
 * .Description:                                                                   *
 *     To start/stop smart config of M8266WIFI via SPI                             *
 *     After perform a start operation, the M8266WIFI will be set in STA mode      *
 * .Parameter(s)                                                                   *
 *     1. en      : to start or stop                                               *
 *                  =0, to stop                                                    *
 *                  others, to start                                               *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_StartSmartConfig(u8 en, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_StartWpsConfig                                                    *
 * .Description:                                                                   *
 *     To start/stop wps config of M8266WIFI via SPI                               *
 *     After perform a start operation, the M8266WIFI will be set in STA mode      *
 * .Parameter(s)                                                                   *
 *     1. en      : to start or stop                                               *
 *                  =0, to stop                                                    *
 *                  others, to start                                               *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_StartWpsConfig(u8 en, u16* status);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   6. APIs releated to on-module web operations via SPI                         //
//       - SPI_Set_Web_Server                                            4.6.1    //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * SPI_Set_Web_Server                                                              *
 * .Description:                                                                   *
 *     To set the web server via SPI                                               *
 * .Parameter(s)                                                                   *
 *     1. open_not_shutdown  : to start or shutdown the local web server           *
 *            =0,      to shutdown the webserver if it is running                  *
 *            =others, to (re-)start the webserver                                 *
 *                     the previous running web server if  will be shutdown        *
 *                     before re-open. Therefore, you don't have to shutdown       *
 *                     it if previously running                                    * 
 *     2. server_port : the port used for web_server                               *
 *            - Ignored when open_not_shutdown==0.                              *
 *            - When open_not_shutdown !=0, and server_port=0, then default        *
 *              port = 80 will be used                                             *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 SPI_Set_Web_Server(u8 open_not_shutdown, u16 server_port, u16* status);
 
 
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   7. APIs releated to module low power operations via SPI                      //
//       - M8266WIFI_SPI_Set_Tx_Max_Power                                4.7.1    //
//       - M8266WIFI_SPI_Sleep_Module                                    4.7.1    //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266WIFI_SPI_Set_Tx_Max_Power                                                  *
 * .Description:                                                                   *
 *     To set the RF Tx Max Power via SPI comannd                                  *
 * .Parameter(s)                                                                   *
 *     1. tx_max_power : the max tx power in 0.25 dBm.                             *
 *                       range = 0~82, i.e. 0~20.5dBm, or 1mW~112mW                *
 *                               tx_max_power  dBm        P/mW                     *
 *                                 0            0          1.0                     *
 *                                 4            1          1.3                     *
 *                                12            3          2.0                     *
 *                                28            7          5.0                     *
 *                                40           10         10.0                     *
 *                                52           13         20.0                     *
 *                                68           17         50.0                     *
 *                                80           20        100.0                     *
 *                                82           20.5      112.20                    *
 *        upon bootup default, tx_max_power = 82, i.e. 20.5dBm or 112.20mW         *
 *                                                                                 *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Set_Tx_Max_Power(u8 tx_max_power, u16 *status);

/**********************************************************************************
 * M8266WIFI_SPI_Sleep_Module                                                      *
 * .Description:                                                                   *
 *     To bring the M8266WIFI module into sleep mode via SPI comannd               *
 * .Parameter(s)                                                                   *
 *     1. sleep_type : the type of sleep                                           *
 *        = 0      :  reserved                                                     *
 *        = 1      :  reserved                                                     *
 *        = 2      :  reserved                                                     *
 *        = 3      :  deep sleep                                                   *
 *        = others :  reserved                                                     *
 *     2. time_to_wakeup_in_ms : time to wakeup from sleep, unit in ms             *
 *        - max value 4294967 ms (about 1.19hour).                                 *
 *          if a value exceed this provided, then 4294967ms will be used           *
 *        - use 0 if expect to sleep for ever without automatic wakeup             *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * Note:                                                                           *
 *     1. The nCS should be pulled low before exit sleep and release after bootup  *
 *        from sleep if reset_type=3 in order for a normal bootup from extern flash*
 *     2. after a call of this function, normally the module should be re init     *
 *        via functions such as M8266WIFI_Module_Init_Via_SPI(), and the connection*
 *        should be re-established as well.                                        *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Sleep_Module(u8 sleep_type, u32 time_to_wakeup_in_ms, u16 *status);
  
/***********************************************************************************
 * M8266WIFI_SPI_Reset_Module                                                      *
 * .Description:                                                                   *
 *     To reset the M8266WIFI module at once via SPI comannd                       *
 * .Parameter(s)                                                                   *
 *     1. reset_type : the type of reset                                           *
 *        = 0 :  to perform a software reset to the module,                        *
 *               boot-mode of last power on will be kept                           *
 *        = 1 :  to perform a hardware reset via chip_EN                           *
 *               boot-mode will be sampled again.                                  *
 * Note:                                                                           *
 *     1. after a call of this function, normally the module should be re init     *
 *        via functions such as M8266WIFI_Module_Init_Via_SPI(), and the connection*
 *        should be re-established as well.                                        *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Reset_Module(u8 reset_type, u16* status);

/**********************************************************************************
 * M8266WIFI_SPI_Restore_Module_to_Default                                         *
 * .Description:                                                                   *
 *     To restore the M8266WIFI module to factory default and then reboot module   *
 *     via SPI comannd. e.g. module will be in AP-Only mode                        *
 * .Parameter(s)                                                                   *
 *     1. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Restore_Module_to_Default(u16* status);

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   8. APIs releated to module information                                       //
//       - M8266WIFI_SPI_Get_Module_Info                                 4.8.1    //
//       - M8266WIFI_SPI_Get_Driver_Info                                 4.8.2    //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/**********************************************************************************
 * M8266WIFI_SPI_Get_Module_Info                                                   *
 * .Description:                                                                   *
 *     To get the M8266WIFI module information, such as ESP8266_ID,                *
 *     Flash Size, Firmware Version                                                *
 * .Parameter(s)                                                                   *
 *     1. esp8266_id: the pointer to the returned esp8266_id                       *
 *                    Use NULL if you don't expect it returned                     *
 *     2. flash_size: the pointer to the returned flash_size                       *
 *                    returned value                                               *
 *                      = 0, if 512KB                                              *
 *                      = 1, if 1MB                                                *
 *                      = 2, if 2MB                                                *
 *                      = 3, if 4MB                                                *
 *                      = others, invalid                                          *
 *                    Use NULL if you don't expect it returned                     *
 *     3. fw_ver    : the pointer to the returned firmware version                 *
 *                    buffer size should be no less than 16 bytes                  *
 *                    returned value e.g. "1.1.4-4"                                *
 *                    Use NULL if you don't expect it returned                     *
 *     4. status    : pointer to return errcode(LSB) and status(MSB) upon error    *
 *                    Use NULL if you don't expect them returned                   *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Get_Module_Info(u32* esp8266_id, u8* flash_size, char* fw_ver, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Get_Driver_Info                                                   *
 * .Description:                                                                   *
 *     To get the M8266WIFI driver information                                     *
 * .Parameter(s)                                                                   *
 *     1. drv_info: the pointer to the returned driver information buffer.         *
 *                  buffer size should be no less than 64 Bytes                   *
 *                  e.g. "ANYLINKIN M8266WIFI SPI DRIVER V1.4, 20170316"           *
 * .Return value                                                                   *
 *     driver_info: the pointer to the returned driver information                 *
 ***********************************************************************************/
char* M8266WIFI_SPI_Get_Driver_Info(char* drv_info);




////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   Below specifications are required by M8266WIFIDrv.lib                        //
//   And should be implemented according to the hardware in M8266HostIf.c         //
//   Put here kust for purpose ofheader specification                             //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void M8266HostIf_Set_nRESET_Pin(u8 level);
void M8266HostIf_Set_SPI_nCS_Pin(u8 level);
void M8266HostIf_delay_us(u8 nus);
u8   M8266HostIf_SPI_ReadWriteByte(u8 byte); // Not required for most platforms.

#endif
