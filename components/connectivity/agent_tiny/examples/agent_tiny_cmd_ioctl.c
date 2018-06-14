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

#include "agent_tiny_cmd_ioctl.h"
#include "agenttiny.h"
#include "atiny_adapter.h"

#define ATINY_POWER_VOLTAGE_MIN 3800
#define ATINY_POWER_VOLTAGE_MAX 5000
#define ATINY_BATTERY_LEVEL     90
#define ATINY_MEMORY_FREE       50
#define ATINY_NETWORK_BEARER    5
#define ATINY_SIGNAL_STRENGTH   90
#define ATINY_CELL_ID           21103
#define ATINY_LINK_QUALITY      98
#define ATINY_LINK_UTRILIZATION 10
#define ATINY_POWER_SOURCE_1    1
#define ATINY_POWER_SOURCE_2    5
#define ATINY_POWER_CURRENT_1   125
#define ATINY_POWER_CURRENT_2   900
#define ATINY_LATITUDE          27.986065f
#define ATINY_LONGITUDE         86.922623f
#define ATINY_ALTITUDE          8495.0000f
#define ATINY_RADIUS            0.0f
#define ATINY_SPEED             0.0f
#define ATINY_TIME_CODE         1367491215

int atiny_get_bind_mode(char* mode,int len)
{
    (void)atiny_printf("bind type is UQ......\r\n");
    (void)atiny_snprintf(mode, len, "UQ");
    return ATINY_OK;
}
int atiny_get_power_current_1(int* arg)
{
    *arg = ATINY_POWER_CURRENT_1;
    return ATINY_OK;
}
int atiny_get_power_current_2(int* arg)
{
    *arg = ATINY_POWER_CURRENT_2;
    return ATINY_OK;
}

int atiny_get_power_source_1(int* arg)
{
    *arg = ATINY_POWER_SOURCE_1;
    return ATINY_OK;
}
int atiny_get_power_source_2(int* arg)
{
    *arg = ATINY_POWER_SOURCE_2;
    return ATINY_OK;
}

int atiny_get_dev_err(int* arg)
{
    *arg = ATINY_OK;
    return ATINY_OK;
}

int atiny_get_model_mode(char* mode,int len)
{
    (void)atiny_snprintf(mode, len, "Lightweight M2M Client");
    return ATINY_OK;
}

int atiny_do_dev_reboot(void)
{
    (void)atiny_printf("device is rebooting......\r\n");
    return ATINY_OK;
}

int atiny_do_factory_reset(void)
{
    (void)atiny_printf("\n\t FACTORY RESET\r\n\n");
    return ATINY_OK;
}

int atiny_get_serial_number(char* num,int len)
{
    (void)atiny_snprintf(num, len, "345000123");
    return ATINY_OK;
}

int atiny_get_min_voltage(int* voltage)
{
    *voltage = ATINY_POWER_VOLTAGE_MIN;
    return ATINY_OK;
}

int atiny_get_max_voltage(int* voltage)
{
    *voltage = ATINY_POWER_VOLTAGE_MAX;
    return ATINY_OK;
}

int atiny_get_baterry_level(int* voltage)
{
    *voltage = ATINY_BATTERY_LEVEL;
    return ATINY_OK;
}

int atiny_get_memory_free(int* voltage)
{
    int tmp;
    (void)atiny_random(&tmp, sizeof(tmp));
    tmp %= 30;
    *voltage = ATINY_MEMORY_FREE + tmp;
    return ATINY_OK;
}

int atiny_get_firmware_ver(char* version, int len)
{
    (void)atiny_snprintf(version, len, "example_ver001");
    return ATINY_OK;
}

int atiny_trig_firmware_update(void)
{
    (void)atiny_printf("firmware is updating......\r\n");
    return ATINY_OK;
}
int atiny_get_firmware_result(int* result)
{
    *result = 0;
    return ATINY_OK;
}

int atiny_get_firmware_state(int* state)
{
    *state = 0;
    return ATINY_OK;
}

int atiny_get_network_bearer(int* network_brearer)
{
    *network_brearer = ATINY_NETWORK_BEARER;
    return ATINY_OK;
}

int atiny_get_signal_strength(int* singal_strength)
{
    *singal_strength = ATINY_SIGNAL_STRENGTH;
    return ATINY_OK;
}

int atiny_get_cell_id(long* cell_id)
{
    *cell_id = ATINY_CELL_ID;
    return ATINY_OK;
}

int atiny_get_link_quality(int* quality)
{
    *quality = ATINY_LINK_QUALITY;
    return ATINY_OK;
}

int atiny_get_link_utilization(int* utilization)
{
    *utilization = ATINY_LINK_UTRILIZATION;
    return ATINY_OK;
}

int atiny_write_app_write(void* user_data, int len)
{
    (void)atiny_printf("write num19 object success\r\n");
    return ATINY_OK;
}

int atiny_update_psk(char* psk_id, int len)
{
    //memcpy_s(g_psk_value,psk_id,len,16);
    (void)atiny_printf("update psk success\r\n");
    return ATINY_OK;
}

int atiny_get_latitude(float* latitude)
{
    *latitude = ATINY_LATITUDE;
    return ATINY_OK;
}

int atiny_get_longitude(float* longitude)
{
    *longitude = ATINY_LONGITUDE;
    return ATINY_OK;
}

int atiny_get_altitude(float* altitude)
{
    *altitude = ATINY_ALTITUDE;
    return ATINY_OK;
}

int atiny_get_radius(float* radius)
{
    *radius = ATINY_RADIUS;
    return ATINY_OK;
}

int atiny_get_speed(float* speed)
{
    *speed = ATINY_SPEED;
    return ATINY_OK;
}

int atiny_get_timestamp(uint64_t* timestamp)
{
    *timestamp = atiny_gettime_ms() / 1000 + ATINY_TIME_CODE;
    return ATINY_OK;
}

//-----  3GPP TS 23.032 V11.0.0(2012-09) ---------
#define HORIZONTAL_VELOCITY                  0
#define HORIZONTAL_VELOCITY_VERTICAL         1
#define HORIZONTAL_VELOCITY_WITH_UNCERTAINTY 2

#define VELOCITY_OCTETS                      5

void location_get_velocity(atiny_velocity_s* velocity,
                          uint16_t bearing,
                          uint16_t horizontal_speed,
                          uint8_t speed_uncertainty)
{
    uint8_t tmp[VELOCITY_OCTETS];
    int copy_len;

    tmp[0] = HORIZONTAL_VELOCITY_WITH_UNCERTAINTY << 4;
    tmp[0] |= (bearing & 0x100) >> 8;
    tmp[1] = (bearing & 0xff);
    tmp[2] = horizontal_speed >> 8;
    tmp[3] = horizontal_speed & 0xff;
    tmp[4] = speed_uncertainty;

    copy_len = MAX_VELOCITY_LEN > sizeof(tmp) ? sizeof(tmp) : MAX_VELOCITY_LEN;
    memcpy(velocity->opaque, tmp, copy_len);
    velocity->length = copy_len;
}

int atiny_get_velocity(atiny_velocity_s* velocity)
{
    location_get_velocity(velocity, 0, 0, 255);
    return ATINY_OK;
}

int atiny_cmd_ioctl(atiny_cmd_e cmd, char* arg, int len)
{
    int result = ATINY_OK;
    switch(cmd)
    {
        case ATINY_GET_BINDING_MODES:
             result = atiny_get_bind_mode(arg, len);
            break;
        case ATINY_GET_MODEL_NUMBER:
            result = atiny_get_model_mode(arg, len);
            break;
        case ATINY_DO_DEV_REBOOT:
             result = atiny_do_dev_reboot();
            break;
        case ATINY_GET_MIN_VOLTAGE:
            result = atiny_get_min_voltage((int*)arg);
            break;
        case ATINY_GET_MAX_VOLTAGE:
            result = atiny_get_max_voltage((int*)arg);
            break;

        case ATINY_GET_BATERRY_LEVEL:
            result = atiny_get_baterry_level((int*)arg);
            break;
        case ATINY_GET_MEMORY_FREE:
            result = atiny_get_memory_free((int*)arg);
            break;
        case ATINY_GET_DEV_ERR:
            result = atiny_get_dev_err((int*)arg);
            break;
        case ATINY_GET_POWER_CURRENT_1:
            result = atiny_get_power_current_1((int*)arg);
            break;
        case ATINY_GET_POWER_CURRENT_2:
            result = atiny_get_power_current_2((int*)arg);
            break;
        case ATINY_GET_POWER_SOURCE_1:
            result = atiny_get_power_source_1((int*)arg);
            break;
        case ATINY_GET_POWER_SOURCE_2:
            result = atiny_get_power_source_2((int*)arg);
            break;
        case ATINY_DO_FACTORY_RESET:
            result = atiny_do_factory_reset();
            break;
        case ATINY_GET_SERIAL_NUMBER:
            result = atiny_get_serial_number(arg, len);
            break;
        case ATINY_GET_FIRMWARE_VER:
            result = atiny_get_firmware_ver(arg, len);
            break;
        case ATINY_TRIG_FIRMWARE_UPDATE:
            result = atiny_trig_firmware_update();
            break;
        case ATINY_GET_FIRMWARE_STATE:
            result = atiny_get_firmware_state((int*)arg);
            break;
        case ATINY_GET_FIRMWARE_RESULT:
            result = atiny_get_firmware_result((int*)arg);
            break;
        case ATINY_GET_NETWORK_BEARER:
            result = atiny_get_network_bearer((int*)arg);
            break;
        case ATINY_GET_SIGNAL_STRENGTH:
            result = atiny_get_signal_strength((int*)arg);
            break;
        case ATINY_GET_CELL_ID:
            result = atiny_get_cell_id((long*)arg);
            break;
        case ATINY_GET_LINK_QUALITY:
            result = atiny_get_link_quality((int*)arg);
            break;
        case ATINY_GET_LINK_UTILIZATION:
            result = atiny_get_link_utilization((int*)arg);
            break;
        case ATINY_WRITE_APP_DATA:
            result = atiny_write_app_write((int*)arg, len);
            break;
        case ATINY_UPDATE_PSK:
            result = atiny_update_psk(arg, len);
            break;
        case ATINY_GET_LATITUDE:
            result = atiny_get_latitude((float*)arg);
            break;
        case ATINY_GET_LONGITUDE:
            result = atiny_get_longitude((float*)arg);
            break;
        case ATINY_GET_ALTITUDE:
            result = atiny_get_altitude((float*)arg);
            break;
        case ATINY_GET_RADIUS:
            result = atiny_get_radius((float*)arg);
            break;
        case ATINY_GET_SPEED:
            result = atiny_get_speed((float*)arg);
            break;
        case ATINY_GET_TIMESTAMP:
            result = atiny_get_timestamp((uint64_t*)arg);
            break;
        case ATINY_GET_VELOCITY:
            result = atiny_get_velocity((atiny_velocity_s*)arg);
            break;
        default:
            break;
    }
    return result;
}

void atiny_event_notify(atiny_event_e event, const char* arg, int len)
{
    (void)atiny_printf("notify:stat:%d\r\n", event);
}

