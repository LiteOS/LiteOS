/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: MPU6050 Sensor Driver
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
#include "los_typedef.h"
#include "mpu6050.h"
#include "i2c.h"
#include <stdio.h>

#define ACCEL_BUFF_LEN      6
#define GYRO_BUFF_LEN       6
#define TEMP_BUFF_LEN       2
#define DATA_SHIFT          3
#define DATA_MAX_LEN        6

/* Convert sensor data to degrees Celsius */
#define MPU_DATA_TO_TEMPERATURE(x) (INT16)((INT16)(x) / (FLOAT)340.0 + (FLOAT)36.53)

STATIC VOID MpuWriteCmd(UINT8 addr, UINT8 data)
{
    (VOID)I2C1_WriteBuffer(MPU6050_ADDRESS, addr, &data, 1);
}

INT32 MpuSensorGetId(VOID)
{
    UINT8 rData;
    I2C1_ReadBuffer(MPU6050_ADDRESS, MPU6050_SENSOR_ID_CMD, &rData, 1);
    if (rData != MPU6050_ID) {
        printf("Get mpu6050 sensor failed!\n");
        return LOS_NOK;
    }

    printf("Get mpu6050 id = %d\n", rData);
    return LOS_OK;
}

VOID MpuSensorGetData(INT16 *data, INT32 dateType)
{
    UINT8 rData[DATA_MAX_LEN] = {0};
    UINT8 i;
    switch (dateType) {
        case MPU6050_AACEL_DATA_TYPE: {
            I2C1_ReadBuffer(MPU6050_ADDRESS, RA_ACCEL_XOUT_H_CMD, &rData[0], ACCEL_BUFF_LEN);
            for (i = 0; i < DATA_SHIFT; i++) {
                data[i] = (rData[i * 2] << 8) | rData[i * 2 + 1];
            }
            break;
        }
        case MPU6050_GYRO_DATA_TYPE: {
            I2C1_ReadBuffer(MPU6050_ADDRESS, RA_GYRO_XOUT_H_CMD, &rData[0], GYRO_BUFF_LEN);
            for (i = 0; i < DATA_SHIFT; i++) {
                data[i] = (rData[i * 2] << 8) | rData[i * 2 + 1];
            }
            break;
        }
        case MPU6050_TEMP_DATA_TYPE: {
            I2C1_ReadBuffer(MPU6050_ADDRESS, RA_TEMP_OUT_H_CMD, &rData[0], TEMP_BUFF_LEN);
            *data = MPU_DATA_TO_TEMPERATURE((rData[0] << 8) | rData[1]);
            break;
        }
        default:
            break;
    }
}

VOID MpuSensorInit(VOID)
{
    MpuWriteCmd(RA_PWR_MGMT_1_CMD, RA_PWR_MGMT_1_VALUE); // wake up sensor
    MpuWriteCmd(RA_SMPLRT_DIV_CMD, RA_SMPLRT_DIV_VALUE);
    MpuWriteCmd(RA_CONFIG_CMD, RA_CONFIG_VALUE);
    MpuWriteCmd(RA_ACCEL_CONFIG_CMD, RA_ACCEL_CONFIG_VALUE);
    MpuWriteCmd(RA_GYRO_CONFIG_CMD, RA_GYRO_CONFIG_VALUE);
    HAL_Delay(150);
}
