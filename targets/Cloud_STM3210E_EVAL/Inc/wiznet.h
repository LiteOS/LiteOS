/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd> 

 ******************************************************************************
  File Name     : wiznet.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/13
  Last Modified :
  Description   : wiznet.c header file
  Function List :
  History       :
  1.Date        : 2018/7/13
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __WIZNET_H__
#define __WIZNET_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

/* Defines ------------------------------------------------------------------*/

/* Macros -------------------------------------------------------------------*/
#define WIZNET_WITH_DEBUGS
#ifdef WIZNET_WITH_DEBUGS
#define WIZ_LOG(fmt, arg...)  printf("[%s:%d]"fmt"\r\n", __func__, __LINE__, ##arg)
#else
#define WIZ_LOG(fmt, arg...)
#endif

#define WIZ_ERROR(fmt, arg...)  printf("[ERR][%s:%d]"fmt"\r\n", __func__, __LINE__, ##arg)

/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
void    wiznet_init(void);
int32_t wiznet_connect(const char* host, const char* port, int protocol);
int32_t wiznet_send(int32_t id , const uint8_t *buf, uint32_t len);
int32_t wiznet_recv(int32_t id, unsigned char* buf, size_t len);
int32_t wiznet_recv_timeout(int32_t id, unsigned char* buf, size_t len, uint32_t timeout);
int32_t wiznet_close(int32_t id);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __WIZNET_H__ */
