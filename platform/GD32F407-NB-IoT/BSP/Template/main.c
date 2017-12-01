/*!
    \file  main.c
    \brief led spark with systick 
*/

/*
    Copyright (C) 2016 GigaDevice

    2016-08-15, V1.0.0, firmware for GD32F4xx
*/

#include <stdio.h>
#include "platform.h"
#include "nb-iot_app.h"

#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_adapter.h"
#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

uint32_t g_uwInitTaskID;
uint32_t g_uwTemperTaskID;
uint32_t g_uwMPU6050TaskID;

void LOS_Temper_Test_Tsk(void)
{
    uint8_t id[8];
    uint8_t ret;
    uint8_t i;
    int16_t reg;
    
    ret = DS18B20_ReadID(id);

    if (ret == 0){
        LOS_EvbUart1Printf("未检测到 DS18B20 \r\n");
    }
    else{
        LOS_EvbUart1Printf("DS18B20 Ok, id = ");

        for (i = 0; i < 8; i++)
        {
        LOS_EvbUart1Printf("%02X ", id[i]);
        }
        LOS_EvbUart1Printf("\r\n");
    }
    
    while(1){
        reg = DS18B20_ReadTempReg();
        printf("reg = 0x%04X %6d --> %-4.04f℃\r\n", (uint16_t)reg, reg, (float)reg / 16);
        LOS_TaskDelay(2000);
    }
}

void LOS_MPU6050_Test_Tsk(void)
{
    if (gpio_i2c_checkdevice(MPU6050_SLAVE_ADDRESS) == 0){
        LOS_EvbUart1Printf("MPU-6050 Ok (0x%02X)\r\n", MPU6050_SLAVE_ADDRESS);
    }
    else{
        LOS_EvbUart1Printf("MPU-6050 Err (0x%02X)\r\n", MPU6050_SLAVE_ADDRESS);
    }
    
    while(1){
        mpu6050_readdata();		/* 读取 MPU-6050的数据到全局变量 g_tMPU6050 */
        /* 显示 g_tMPU6050 结构体成员中的数据 */
        LOS_EvbUart1Printf("AX=%6d,AY=%6d,AZ=%6d,",
            g_tMPU6050.Accel_X,
            g_tMPU6050.Accel_Y,
            g_tMPU6050.Accel_Z);

        LOS_EvbUart1Printf("GX=%6d,GY=%6d,GZ=%6d,T=%6d\r\n",
            g_tMPU6050.GYRO_X,
            g_tMPU6050.GYRO_Y,
            g_tMPU6050.GYRO_Z,
            g_tMPU6050.Temp);
            
        LOS_TaskDelay(2000);
    }
}

void LOS_TamperTask_Entry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Temper_Test_Tsk;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "InspectTsk";
    stTaskInitParam.usTaskPrio = 6;
    uwRet = LOS_TaskCreate(&g_uwTemperTaskID, &stTaskInitParam);
    
    if (uwRet != LOS_OK)
    {
        return;
    }
    
    return;
}

void LOS_MPU6050Task_Entry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_MPU6050_Test_Tsk;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "InspectTsk";
    stTaskInitParam.usTaskPrio = 6;
    uwRet = LOS_TaskCreate(&g_uwMPU6050TaskID, &stTaskInitParam);
    
    if (uwRet != LOS_OK)
    {
        return;
    }
    
    return;
}

void LOS_Init_Tsk(void)
{
    ds18b20_init(); 
    gpio_i2c_init();  
    mpu6050_init();
    nbxx_01_uart_init();
    nbxx_01_timeout_timer_init(10000);
    LOS_MPU6050Task_Entry();
    LOS_TamperTask_Entry();
    
    LOS_TaskDelete(g_uwInitTaskID);
    
    nbiot_basic_connect();            //没有NB-IOT网络，该函数会卡死
}

void LOS_InitTask_Entry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Init_Tsk;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "InspectTsk";
    stTaskInitParam.usTaskPrio = 9;
    uwRet = LOS_TaskCreate(&g_uwInitTaskID, &stTaskInitParam);
    
    if (uwRet != LOS_OK)
    {
        return;
    }
    
    return;
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    UINT32 uwRet;
    
    nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
    
    uwRet = LOS_KernelInit();
    
    /*Init LiteOS kernel */
    if (uwRet != LOS_OK) {
        return LOS_NOK;
    }
    
    /* Enable LiteOS system tick interrupt */
    LOS_EnableTick();

    /*
        Notice: add your code here
        here you can create task for your function 
        do some hw init that need after systemtick init
     */
    LOS_EvbSetup();//init the device on the dev baord

    LOS_InitTask_Entry();

    /* Kernel start to run */
    LOS_Start();
    for (;;);
}



