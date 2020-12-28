/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Sensor Manager HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#ifndef _SENSOR_MANAGER_H
#define _SENSOR_MANAGER_H

#include "los_list.h"
#include "los_typedef.h"
#include "los_task.h"
#include "protocol.h"
#include "devices.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SAMPLE_PERIOD_MIN  10
#define MAIL_OFFSET_LENGTH 3
#define SM_DELAY           5000

enum {
    SENSOR_STAT_CLOSE  = 0,
    SENSOR_STAT_INITED = (1 << 0),
    SENSOR_STAT_OPEN   = (1 << 1),
};

typedef struct {
    INT32 result;
    INT32 offset[MAIL_OFFSET_LENGTH];
} MailCalibrate;

typedef VOID (*RespFunc)(UINT32 arg, UINT16 id, INT32 cmd, INT32 res);
typedef VOID (*UpdateFunc)(UINT32 arg, const INT8 *data, UINT32 len);

typedef enum {
    IDLE = 0,
    CLOSE,
    WORKING,
    OPEN,
} SensorItemState;

typedef enum {
    SENSOR_IOCTL_SET_CALIBRAT,
    SENSOR_IOCTL_SET_SELFTEST,
    SENSOR_IOCTL_SET_OFFSET,
    SENSOR_IOCTL_SET_PARAMET,
    SENSOR_IOCTL_MAX,
} SmPrivEvent;

typedef enum {
    SENSOR_ATTACH_NONE     = 0x00,
    SENSOR_ATTACH_UNCHANGE = 0x01,
    SENSOR_ATTACH_CHANGE   = 0x02,
    SENSOR_ATTACH_MAX,
} SensorAttachStatus;

typedef struct {
    UINT16 id;
    UINT8  resp;
    UINT8  reserved;
} PrivResponse;

typedef struct {
    struct LOS_DL_LIST list;
    OpenParam          openParam;   // The date update period of this item
    CloseParam         closeParam;
    RespFunc           respFunc;    // respond callback when operate
    UpdateFunc         updateFunc;  // date update callback
    UINT32             updateArg;   // the argument of date update callback
    UINT32             updateTime;  // date update time
    UINT32             updateCnt;   // date update count
    ObjTag             tag;         // the argument of respond callback
    SensorItemState    state;
    UINT16             id;          // identify for each item operate
    UINT8              resp;        // 0 no respond; 1 respond
    UINT8              reserverd;
} SensorItem;

typedef struct {
    struct LOS_DL_LIST      list;
    struct LOS_DL_LIST      slist;
    struct LOS_DL_LIST      attachedApp;
    struct DeviceType       *attachDevice;
    struct LOS_DL_LIST      slaveList;
    struct SensorOperation  *sensorOp;
    OpenParam               openParam;  // the interval time of driver update date
    CloseParam              closeParam;
    VOID                    *sensorData;
    UINT32                  slaveAddr;
    UINT32                  interval; // ticks
    INT32                   count;
    UINT32                  sensorMuxLock;
    VOID                    *priv;
    PrivResponse            privResp;
    SensorItem              commuItem;
    UINT8                   sensorDataLen;
    UINT8                   tag;
    UINT8                   cmd;
    UINT8                   sensorStat;
} SensorType;

typedef struct {
    SensorType *sensorInterface;
} SensorScbTable;

struct SensorOperation {
    INT32 (*Open)(SensorType *sensor, OpenParam *para);
    INT32 (*Close)(SensorType *sensor);
    INT32 (*ReadData)(SensorType *sensor);
    INT32 (*Init)(SensorType *sensor);
    INT32 (*IrqHandler)(VOID);
    INT32 (*IoCtl)(SensorType *sensor, UINT8 type, UINT8 *data);
};

typedef enum {
    SM_EVENT_SENSOR_SAMPLE,
    SM_EVENT_ITEM_ENABLE,
    SM_EVENT_ITEM_DISABLE,
    SM_EVENT_ITEM_INIT,
    SM_EVENT_CONFIG,
} SmEvent;

typedef struct {
    SensorItem *item;
    RespFunc   respFunc;
    UpdateFunc updateFunc;
    UINT32     updateArg;
    ObjTag     tag;
} MailItemInit;

typedef struct {
    SensorItem *item;
    OpenParam  openParam;
    UINT16     id;
    UINT8      resp;
    UINT8      reserved;
} MailItemEnable;

typedef struct {
    SensorItem *item;
    CloseParam closeParam;
    UINT16     id;
    UINT8      resp;
    UINT8      reserved;
} MailItemDisable;

typedef struct {
    ObjTag tag;
    UINT16 id;
    UINT8  resp;
    UINT8  cmd;
} MailConfig;

typedef struct {
    INT32 result;
} MailSelftest;

typedef struct {
    ObjTag tag;
} MailSample;

typedef struct {
    SmEvent event;
    union {
        MailSample      sample;
        MailItemInit    itemInit;
        MailItemEnable  itemEnable;
        MailItemDisable itemDisable;
        MailConfig      config;
        MailCalibrate   calibrat;
        MailSelftest    selftest;
    };
} SmMail;

extern SensorScbTable *SensorTableGet(VOID);
extern INT32 SensorConfigNotify(const PktParameterReq *pkt, UINT8 resp);
extern INT32 SensorItemInit(SensorItem *item, RespFunc respFunc, ObjTag tag, UpdateFunc updateFunc, UINT32 upArg);
extern INT32 SensorItemDisable(SensorItem *item, CloseParam *param, UINT16 id, UINT8 resp);
extern INT32 SensorItemEnable(SensorItem *item, OpenParam *param, UINT16 id, UINT8 resp);
extern INT32 SensorRegister(SensorType *sensor);
extern INT32 SensorManagerInit(VOID);
extern INT32 SensorSample(SensorType *sensor);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _SENSOR_MANAGER_H */
