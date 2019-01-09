


#include "drv_spi.h"
#include "stdio.h"
#include "soc.h"
#include <string.h>
#include "pin.h"	
#include "los_hwi.h"
#include "csky_All_Demo.h"
#define OPERATE_ADDR    0x0
#define OPERATE_LEN     256
#define SPIFLASH_BASE_VALUE 0x0
static uint8_t erase_read_flag = 0;
static uint8_t program_read_flag = 0;

extern int32_t at25flash_read_id(spi_handle_t handle, uint32_t *id_num);
extern int32_t at25flash_erase_sector(spi_handle_t handle, uint32_t addr);
extern int32_t at25flash_erase_chip(spi_handle_t handle);
extern int32_t at25flash_program_data(spi_handle_t handle, uint32_t addr, const void *data, uint32_t cnt);
extern int32_t at25flash_read_data(spi_handle_t handle, uint32_t addr, void *data, uint32_t cnt);

static void spi_event_cb_fun(int32_t idx, spi_event_e event)
{
    printf("[info]进入spi的回调函数:%d  [0]传输完成,[1]发送完成,[2]接受完成,[3]数据丢失,[4]模式错误\n",event);
	csi_vic_disable_irq(SPI1_IRQn); //成功关闭
}

void example_pin_spi_init(void)
{
	printf("[info]SPI引脚初始化\n");
    drv_pinmux_config(EXAMPLE_PIN_SPI_MISO, EXAMPLE_PIN_SPI_MISO_FUNC);	//PA22
    drv_pinmux_config(EXAMPLE_PIN_SPI_MOSI, EXAMPLE_PIN_SPI_MOSI_FUNC);	//PA23
    drv_pinmux_config(EXAMPLE_PIN_SPI_CS, EXAMPLE_PIN_SPI_CS_FUNC);		//PA18
    drv_pinmux_config(EXAMPLE_PIN_SPI_SCK, EXAMPLE_PIN_SPI_SCK_FUNC);	//PA21
}

static int test_spi_eeprom(int32_t idx)
{
    uint8_t id[3] = {0x11, 0x11};
    uint8_t input[OPERATE_LEN] = {0};
    uint8_t output[OPERATE_LEN] = {0};
    int i;
    int32_t ret;
    spi_handle_t spi_handle_t;

    example_pin_spi_init();
	
	
	
    spi_handle_t = csi_spi_initialize(idx, spi_event_cb_fun);//idx =1

    if (spi_handle_t == NULL) 
	{
        printf(" [error]SPI初始化失败\n");
        return -1;
    }
	printf("[success]SPI初始化成功\n");
	
	printf("\n[info]SPI1 能力检测:\n");
	spi_capabilities_t cap; 
	cap=csi_spi_get_capabilities(1); 
	printf("[info]spi %s simplex mode\n",cap.simplex==1?"supports":"not supports"); 
	printf("[info]spi %s TI Synchronous Serial Interface\n",cap.ti_ssi==1?"supports": "not supports"); 
	printf("[info]spi %s Microwire Interface\n",cap.microwire==1?"supports":"not supports"); 
	printf("[info]spi %s signal mode fault event\n\n",cap.event_mode_fault==1?"have": "have not");

	printf("[info]准备读取at25flash的ID号：\n");
    ret = at25flash_read_id(spi_handle_t, (uint32_t *)&id);

    if (ret < 0) 
	{
        printf("[error]flash_read_id error\n");
        return -1;
    }

    printf("[success]the spiflash id is %x %x\r\n", id[0], id[1]);

    ret = at25flash_erase_sector(spi_handle_t, OPERATE_ADDR);

    if (ret < 0) {
        printf("[error]flash_erase_sector error\n");
        return -1;
    }

    ret = at25flash_read_data(spi_handle_t, OPERATE_ADDR, output, OPERATE_LEN);

    if (ret < 0) {
        printf("[error]flash_read_data error\n");
        return -1;
    }

    printf("[info]erase sector and then read\n");

    for (i = 0; i < OPERATE_LEN; i++) {
        if ((i % 10) == 0) {
            printf("output[%d]", i);
        }

        printf("%x ", output[i]);

        if (((i + 1) % 10) == 0) {
            printf("\n");
        }

        if (output[i] != 0xff) {
            erase_read_flag = 1;
            break;
        }
    }

    printf("\n");

    if (erase_read_flag == 1) {
        printf("[error]flash erase check and read check error\n");
        return -1;
    }

    for (i = 0; i < sizeof(input); i++) {
        input[i] = i + SPIFLASH_BASE_VALUE;
    }

    printf("[success]flash erase sector successfully\n");
    memset(output, 0xfb, sizeof(output));

    at25flash_program_data(spi_handle_t, OPERATE_ADDR, input, OPERATE_LEN);
    at25flash_read_data(spi_handle_t, OPERATE_ADDR, output, OPERATE_LEN);

    printf("[info]program data and then read\n");

    for (i = 0; i < OPERATE_LEN; i++) {
        if ((i % 10) == 0) {
            printf("output[%d]", i);
        }

        printf("%x ", output[i]);

        if (((i + 1) % 10) == 0) {
            printf("\n");
        }

        if (output[i] != input[i]) {
            program_read_flag = 1;
            break;
        }
    }

    printf("\n");

    if (program_read_flag == 1) {
        printf("[error]flash program and read check error\n");
        return -1;
    }

    printf("[success]flash program data successfully\n");

    ret = csi_spi_uninitialize(spi_handle_t);
    return 0;

}

int example_spi(int32_t idx)
{
    int ret;
	
    ret = test_spi_eeprom(idx);

    if (ret < 0) {
        printf("[error]test spi eeprom failed\n");
        return -1;
    }

    printf("[success]test spi eeprom successfully\n");

    return 0;
}

int task_SPI_EEPROM(void)
{
	#if LOSCFG_PLATFORM_HWI		//接管中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> SPI")
#endif
	UINTPTR uvIntSave;										//用于保存所有的中断情况
	uvIntSave = LOS_IntLock();								//禁用所有中断，并返回原先中断情况
	LOS_HwiCreate (SPI1_IRQn, 0, 0, (void (*) (uintptr_t))spi_event_cb_fun, 0);	//将硬件中断向量号绑定到系统中，指向中断服务函数	
	LOS_IntRestore(uvIntSave);								//恢复所有中断情况
#else						//直接中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> SPI")
#endif
	//csi_vic_enable_irq(SPI1_IRQn);						//初始化时已经开启，此处不需要开启
#endif
	HopeRun_printf("开始SPI测试............\n");
    return example_spi(EXAMPLE_SPI_IDX);//EXAMPLE_SPI_IDX = 1  中断向量号SPI1_IRQn
}
