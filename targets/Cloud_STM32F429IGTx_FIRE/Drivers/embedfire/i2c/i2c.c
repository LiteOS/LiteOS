/**
 ******************************************************************************
 * @file    i2c.c
 * @author  fire
 * @version V1.0
 * @date    2015-xx-xx
 * @brief   i2c 总线驱动，方便与各种I2C传感器通讯。
 ******************************************************************************
 * @attention
 *
 * 实验平台:野火  STM32 F767 开发板
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :http://fire-stm32.taobao.com
 *
 ******************************************************************************
 */
#include "i2c.h"

I2C_HandleTypeDef I2C_Handle;
/*******************************  Function ************************************/

/**
 * @brief  初始化I2C总线，使用I2C前需要调用
 * @param  无
 * @retval 无
 */
void I2cMaster_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能I2Cx时钟 */
    SENSORS_I2C_RCC_CLK_ENABLE();

    /* 使能I2C GPIO 时钟 */
    SENSORS_I2C_SCL_GPIO_CLK_ENABLE();
    SENSORS_I2C_SDA_GPIO_CLK_ENABLE();

    /* 配置I2Cx引脚: SCL ----------------------------------------*/
    GPIO_InitStructure.Pin = SENSORS_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = SENSORS_I2C_AF;
    HAL_GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* 配置I2Cx引脚: SDA ----------------------------------------*/
    GPIO_InitStructure.Pin = SENSORS_I2C_SDA_GPIO_PIN;
    HAL_GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    if (HAL_I2C_GetState(&I2C_Handle) == HAL_I2C_STATE_RESET) {
        /* 强制复位I2C外设时钟 */
        SENSORS_I2C_FORCE_RESET();

        /* 释放I2C外设时钟复位 */
        SENSORS_I2C_RELEASE_RESET();

        /* I2C 配置 */
        I2C_Handle.Instance = SENSORS_I2C;
        I2C_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        I2C_Handle.Init.ClockSpeed = 400000;
        I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        I2C_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
        I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        I2C_Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        I2C_Handle.Init.OwnAddress1 = 0;
        I2C_Handle.Init.OwnAddress2 = 0;
        /* 初始化I2C */
        HAL_I2C_Init(&I2C_Handle);
        /* 使能模拟滤波器 */
        HAL_I2CEx_AnalogFilter_Config(&I2C_Handle, I2C_ANALOGFILTER_ENABLE);
    }
}
/**
 * @brief  Manages error callback by re-initializing I2C.
 * @param  Addr: I2C Address
 * @retval None
 */
static void I2Cx_Error(uint8_t Addr)
{
    /* 恢复I2C寄存器为默认值 */
    HAL_I2C_DeInit(&I2C_Handle);
    /* 重新初始化I2C外设 */
    I2cMaster_Init();
}
/**
 * @brief  写寄存器，这是提供给上层的接口
 * @param  slave_addr: 从机地址
 * @param 	reg_addr:寄存器地址
 * @param len：写入的长度
 *	@param data_ptr:指向要写入的数据
 * @retval 正常为0，不正常为非0
 */
int Sensors_I2C_WriteRegister(
    unsigned char slave_addr, unsigned char reg_addr, unsigned short len, unsigned char* data_ptr)
{
    HAL_StatusTypeDef status = HAL_OK;
    status =
        HAL_I2C_Mem_Write(&I2C_Handle, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, I2Cx_FLAG_TIMEOUT);
    /* 检查通讯状态 */
    if (status != HAL_OK) {
        /* 总线出错处理 */
        I2Cx_Error(slave_addr);
    }
    while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
    }
    /* 检查SENSOR是否就绪进行下一次读写操作 */
    while (HAL_I2C_IsDeviceReady(&I2C_Handle, slave_addr, I2Cx_FLAG_TIMEOUT, I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT)
        ;
    /* 等待传输结束 */
    while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
    }
    return status;
}

/**
 * @brief  读寄存器，这是提供给上层的接口
 * @param  slave_addr: 从机地址
 * @param 	reg_addr:寄存器地址
 * @param len：要读取的长度
 *	@param data_ptr:指向要存储数据的指针
 * @retval 正常为0，不正常为非0
 */
int Sensors_I2C_ReadRegister(
    unsigned char slave_addr, unsigned char reg_addr, unsigned short len, unsigned char* data_ptr)
{
    HAL_StatusTypeDef status = HAL_OK;
    status =
        HAL_I2C_Mem_Read(&I2C_Handle, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, I2Cx_FLAG_TIMEOUT);
    /* 检查通讯状态 */
    if (status != HAL_OK) {
        /* 总线出错处理 */
        I2Cx_Error(slave_addr);
    }
    while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
    }
    /* 检查SENSOR是否就绪进行下一次读写操作 */
    while (HAL_I2C_IsDeviceReady(&I2C_Handle, slave_addr, I2Cx_FLAG_TIMEOUT, I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT)
        ;
    /* 等待传输结束 */
    while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
    }
    return status;
}
