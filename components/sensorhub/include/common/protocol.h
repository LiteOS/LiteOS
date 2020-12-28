/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Protocol HeadFile
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

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OPEN_PARAM_RESERVED_LENGTH  3
#define CLOSE_PARAM_RESERVED_LENGTH 4
#define BATCH_COUNT_MAX             10000

// tag--------------------------------------------------------
typedef enum {
    TAG_BEGIN = 0x01,
    TAG_SENSOR_BEGIN = TAG_BEGIN,
    TAG_ACCEL = TAG_SENSOR_BEGIN,
    TAG_GYRO,
    TAG_MAG,
    TAG_ALS,
    TAG_PS,
    TAG_SCREEN_ROTATE,
    TAG_LINEAR_ACCEL,
    TAG_GRAVITY,
    TAG_ORIENTATION,
    TAG_ROTATION_VECTORS,
    TAG_PRESSURE,
    TAG_TEMP,
    TAG_HUMIDITY,
    TAG_AMBIENT_TEMP,
    TAG_LABC,
    TAG_HALL,
    TAG_MAG_UNCALIBRATED,
    TAG_GAME_RV,
    TAG_GYRO_UNCALIBRATED,
    TAG_SIGNIFICANT_MOTION,
    TAG_STEP_DETECTOR,
    TAG_STEP_COUNTER,
    TAG_GEOMAGNETIC_RV,
    TAG_HANDPRESS,
    TAG_CAP_PROX,
    TAG_SENSOR_END,
    TAG_TP = TAG_SENSOR_END,  // 26
    TAG_END
} ObjTag;

// cmd----------------------------------------------------------------------
typedef enum {
    // common command
    CMD_CMN_OPEN_REQ = 0x01,
    CMD_CMN_OPEN_RESP,
    CMD_CMN_CLOSE_REQ,
    CMD_CMN_CLOSE_RESP,
    CMD_CMN_INTERVAL_REQ,
    CMD_CMN_INTERVAL_RESP,

    CMD_PRIVATE = 0x1f,

    // accelerometer command
    CMD_ACCEL_DATA_REQ = CMD_PRIVATE,
    CMD_ACCEL_DATA_RESP,
    CMD_ACCEL_SELFCALI_REQ,
    CMD_ACCEL_SELFCALI_RESP,
    CMD_ACCEL_PARAMET_REQ,
    CMD_ACCEL_PARAMET_RESP,
    CMD_ACCEL_OFFSET_REQ,
    CMD_ACCEL_OFFSET_RESP,
    CMD_ACCEL_SELFTEST_REQ,
    CMD_ACCEL_SELFTEST_RESP,

    // gyroscopy command
    CMD_GYRO_DATA_REQ = CMD_PRIVATE,
    CMD_GYRO_DATA_RESP,
    CMD_GYRO_SELFTEST_REQ,
    CMD_GYRO_SELFTEST_RESP,
    CMD_GYRO_PARAMET_REQ,
    CMD_GYRO_PARAMET_RESP,
    CMD_GYRO_CONFIG_REQ,
    CMD_GYRO_CONFIG_RESP,

    // magnetometer command
    CMD_MAG_DATA_REQ = CMD_PRIVATE,
    CMD_MAG_DATA_RESP,
    CMD_MAG_SELFTEST_REQ,
    CMD_MAG_SELFTEST_RESP,
    CMD_MAG_PARAMET_REQ,
    CMD_MAG_PARAMET_RESP,
    CMD_MAG_SET_CALIBRATE_TO_MCU_REQ,
    CMD_MAG_SET_CALIBRATE_TO_MCU_RESP,
    CMD_MAG_SEND_CALIBRATE_TO_AP_REQ,
    CMD_MAG_SEND_CALIBRATE_TO_AP_RESP,

    // ambient light sensor
    CMD_ALS_DATA_REQ = CMD_PRIVATE,
    CMD_ALS_DATA_RESP,
    CMD_ALS_PARAMET_REQ,
    CMD_ALS_PARAMET_RESP,

    // proximity sensor
    CMD_PS_DATA_REQ = CMD_PRIVATE,
    CMD_PS_DATA_RESP,
    CMD_PS_PARAMET_REQ,
    CMD_PS_PARAMET_RESP,
    CMD_PS_SELFCALI_REQ,
    CMD_PS_SELFCALI_RESP,
    CMD_PS_OFFSET_REQ,
    CMD_PS_OFFSET_RESP,

    // err resp
    CMD_ERR_RESP = 0xfe,
} ObjCmd;

typedef enum {
    EN_OK = 0,
    EN_FAIL,
} ErrNo;

typedef enum {
    NO_RESP = 0,
    RESP,
} ObjResp;

typedef enum {
    AUTO_MODE = 0,
    FIFO_MODE,
    INTEGRATE_MODE,
    REALTIME_MODE,
    MODE_END
} ObjReportMode;

typedef enum {
    // system status
    ST_NULL = 0,
    ST_BEGIN,
    ST_POWERON = ST_BEGIN,
    ST_MINSYSREADY,
    ST_DYNLOAD,
    ST_MCUREADY,
    ST_TIMEOUTSCREENOFF,
    ST_SCREENON,
    ST_SCREENOFF,
    ST_SLEEP,
    ST_WAKEUP,
    ST_POWEROFF,
    ST_END
} SysStatus;

typedef struct {
    UINT16 tranId;
    UINT16 length;
    UINT8  tag;
    UINT8  cmd;
    UINT8  resp;  // value CMD_RESP means need resp, CMD_NO_RESP means need not resp
    UINT8  partialOrder;
} PktHeader;

#define CMN_RESP_LENGTH   4
#define RESP_ERRNO_LENGTH 4

typedef struct {
    UINT16 tranId;
    UINT16 length;
    UINT8  tag;
    UINT8  cmd;
    UINT8  resp;
    UINT8  partialOrder;
    UINT32 errNo;  // In win32, errno is function name and conflict
} PktHeaderResp;

typedef struct {
    PktHeader hd;
    INT32 x;
    INT32 y;
    INT32 z;
    UINT32 accuracy;
} PktXyzDataReq;

typedef struct {
    UINT32 period; // ticks
    UINT32 batchCount;
    UINT8  mode;
    UINT8  reserved[OPEN_PARAM_RESERVED_LENGTH];
} OpenParam;

typedef struct {
    UINT8 reserved[CLOSE_PARAM_RESERVED_LENGTH];
} CloseParam;

typedef struct {
    PktHeader hd;
    OpenParam para;
} PktCmnIntervalReq;

typedef struct {
    PktHeader  hd;
    CloseParam clsParam;
} PktCmnCloseReq;

typedef struct {
    PktHeader hd;
    UINT8 para[];
} PktParameterReq;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _PROTOCOL_H */
