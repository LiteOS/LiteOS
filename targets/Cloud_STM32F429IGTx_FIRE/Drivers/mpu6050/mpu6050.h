/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: MPU6050 Sensor Driver HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-12-25
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

#ifndef _MPU6050_H
#define _MPU6050_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MPU6050_ID            0x68

#define MPU6050_ADDRESS       0xD0
#define MPU6050_SENSOR_ID_CMD 0x75
#define RA_TEMP_OUT_H_CMD     0x41
#define RA_GYRO_XOUT_H_CMD    0x43
#define RA_ACCEL_XOUT_H_CMD   0x3B

#define RA_PWR_MGMT_1_CMD     0x6B
#define RA_SMPLRT_DIV_CMD     0x19
#define RA_CONFIG_CMD         0x1A
#define RA_GYRO_CONFIG_CMD    0x1B
#define RA_ACCEL_CONFIG_CMD   0x1C

#define RA_PWR_MGMT_1_VALUE   0x00
#define RA_SMPLRT_DIV_VALUE   0x07
#define RA_CONFIG_VALUE       0x06
#define RA_ACCEL_CONFIG_VALUE 0x01
#define RA_GYRO_CONFIG_VALUE  0x18

typedef enum {
    MPU6050_AACEL_DATA_TYPE = 0,
    MPU6050_GYRO_DATA_TYPE = 1,
    MPU6050_TEMP_DATA_TYPE = 2
} Mpu6050DataType;

VOID MpuSensorInit(VOID);
INT32 MpuSensorGetId(VOID);
VOID MpuSensorGetData(INT16 *data, INT32 dateType);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MPU6050_H */
