

#include "drv_iic.h"
#include "stdio.h"
#include <string.h>
#include <csi_config.h>
#include "soc.h"
#include "pin.h"
#include "csky_All_Demo.h"
#define  CK_IIC_SLAVE_ADDR         0x50
#define  EEPROM_PAGE_SIZE          0x20
#define  EEPROM_SIZE               8192
#define  EEPROM_SUB_ADDR_START     0x0000
#define  EEPROM_SUB_ADDR_END       0x1FFF

static iic_handle_t pcsi_iic;
static volatile uint8_t cb_transfer_flag = 0;

extern void mdelay(uint32_t ms);

static void iic_event_cb_fun(int32_t idx, iic_event_e event)
{
	printf("进入IIC回调函数\n");
    if (event == IIC_EVENT_TRANSFER_DONE) {
        cb_transfer_flag = 1;
    }
}

void example_pin_iic_init(void)
{
    drv_pinmux_config(EXAMPLE_PIN_IIC_SDA, EXAMPLE_PIN_IIC_SDA_FUNC);
    drv_pinmux_config(EXAMPLE_PIN_IIC_SCL, EXAMPLE_PIN_IIC_SCL_FUNC);
}

static int test_iic_eeprom(int32_t iic_idx)
{
    bool xfer_pending = false;
    uint8_t write_data[EEPROM_PAGE_SIZE + 2] = {0x0, EEPROM_PAGE_SIZE, 0};
    uint8_t read_data[EEPROM_PAGE_SIZE + 2] = {0x0, EEPROM_PAGE_SIZE, 0};
    uint8_t i = 0;
    int32_t ret;

    cb_transfer_flag = 0;

    for (i = 2; i < sizeof(write_data); i++) {
        write_data[i] = i - 2;
    }
	
    example_pin_iic_init();
	printf("[info]初始化IIC引脚\n");

    pcsi_iic = csi_iic_initialize(iic_idx, iic_event_cb_fun);
    if (pcsi_iic == NULL) {
        printf("[error]csi_iic_initialize error\n");
        return -1;
    }
	printf("[success]csi_iic_initialize success\n");
	
    ret = csi_iic_config(pcsi_iic, IIC_MODE_MASTER, IIC_BUS_SPEED_STANDARD, IIC_ADDRESS_7BIT, CK_IIC_SLAVE_ADDR);
    if (ret < 0) {
        printf("[error]csi_iic_config error\n");
        return -1;
    }
	printf("[success]csi_iic_config success\n");
	
    ret = csi_iic_master_send(pcsi_iic, CK_IIC_SLAVE_ADDR, write_data, sizeof(write_data), xfer_pending);
    if (ret < 0) {
        printf("[error]csi_iic_master_send error\n");
        return -1;
    }
	printf("[success]csi_iic_master_send success\n");
	printf("[info]waiting call back funcation......\n");
	i=0;
    while (!cb_transfer_flag)
	{
		
		printf("\r...%d...",i++);
		if(i>=3)
		{
			printf("\n[error]iic 发送响应超时,请检查硬件连接!\n");
			return -1;
		}
		mdelay(1000);
		
	}

    mdelay(5);

    cb_transfer_flag = 0;
	
    ret = csi_iic_master_send(pcsi_iic, CK_IIC_SLAVE_ADDR, read_data, 2, xfer_pending);
    if (ret < 0) {
        printf("[error]csi_iic_master_send error\n");
        return -1;
    }
	printf("[success]csi_iic_master_send success\n");
	printf("[info]waiting call back funcation......\n");
    while (!cb_transfer_flag)
	{
		
		printf("\r...%d...",i++);
		if(i>=3)
		{
			printf("\n[error]iic 发送响应超时,请检查硬件连接!\n");
			return -1;
		}
		mdelay(1000);
		
	}

    cb_transfer_flag = 0;
    ret = csi_iic_master_receive(pcsi_iic, CK_IIC_SLAVE_ADDR, read_data + 2 , sizeof(read_data) - 2, xfer_pending);
    if (ret < 0) {
        printf("csi_iic_master_receive error\n");
        return -1;
    }
	printf("[success]csi_iic_master_receive success\n");
	printf("[info]waiting call back funcation......\n");
    while (!cb_transfer_flag)
	{
		
		printf("\r...%d...",i++);
		if(i>=3)
		{
			printf("\n[error]iic 接收响应超时,请检查硬件连接!\n");
			return -1;
		}
		mdelay(1000);
		
	}


    printf("write_data,read_data:\n");
    for (i = 2; i < EEPROM_PAGE_SIZE + 2; i++) {
        printf("%x,%x \t", write_data[i], read_data[i]);

        if (((i + 3) % 4) == 0) {
            printf("\n");
        }

        if (write_data[i] != read_data[i]) {
            printf("\n[error]test at24c64 write and read failed\n");
            return -1;
        }
    }
    printf("\n[success]test at24c64 write and read successfully\n");
    /* test abort fun */
    memset(read_data + 2, 0x0, EEPROM_PAGE_SIZE);
    cb_transfer_flag = 0;
    ret = csi_iic_master_send(pcsi_iic, CK_IIC_SLAVE_ADDR, read_data, 2, xfer_pending);

    if (ret < 0) {
        printf("[error]csi_iic_master_send error\n");
        return -1;
    }

    while (!cb_transfer_flag)
	{
		
		printf("\r...%d...",i++);
		if(i>=3)
		{
			printf("\n[error]iic 发送响应超时,请检查硬件连接!\n");
			return -1;
		}
		mdelay(1000);
		
	}

    cb_transfer_flag = 0;
    ret = csi_iic_master_receive(pcsi_iic, CK_IIC_SLAVE_ADDR, read_data + 2 , sizeof(read_data) - 2, xfer_pending);

    if (ret < 0) {
        printf("[error]csi_iic_master_receive error\n");
        return -1;
    }

    ret = csi_iic_abort_transfer(pcsi_iic);

    if (ret < 0) {
        printf("[error]csi_iic_abort_transfer error\n");
        return -1;
    }

    printf("write_data,read_data:\n");

    for (i = 2; i < EEPROM_PAGE_SIZE + 2; i++) {
        printf("%x,%x\t", write_data[i], read_data[i]);

        if (write_data[i] != read_data[i]) {
            break;
        }
    }

    if (i == EEPROM_PAGE_SIZE) {
        printf("\n[error]test abort function failed\n");
        return -1;
    }

    printf("\n[success]test abort function successfully\n");

    ret = csi_iic_uninitialize(pcsi_iic);

    if (ret < 0) {
        printf("[error]csi_iic_uninitialize error\n");
        return -1;
    }

    printf("[success]test_iic_eeprom successfully\n");
    return 0;
}

int example_iic(int32_t iic_idx)
{
    int ret;

    csi_coret_config(SYSTEM_CLOCK / 100, CORET_IRQn);    //10ms
    csi_vic_enable_irq(CORET_IRQn);

    ret = test_iic_eeprom(iic_idx);

    if (ret < 0) {
        printf("[error]test_iic_eeprom fail\n");
        return -1;
    }

    return 0;
}

int task_IIC(void)
{
	HopeRun_printf("开始IIC测试............\n");
    return example_iic(EXAMPLE_IIC_IDX);
}
