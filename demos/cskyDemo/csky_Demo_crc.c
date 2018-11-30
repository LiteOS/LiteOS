
#include <stdio.h>
#include "soc.h"
#include "drv_crc.h"
#include "csky_All_Demo.h"
static crc_handle_t crc = NULL;

void crc_modbus()
{
    uint32_t crc_input[] = {0x44332211, 0x44332211, 0x44332211, 0x44332211};
    uint32_t expect_out = 0x8efa;
    uint32_t out;
    uint32_t index;
    uint32_t i;
    crc_status_t status;
    status.busy = 1;
    index = 1;

    for (i = 0; i < index; i++) {
        crc = csi_crc_initialize(i, NULL);

        csi_crc_config(crc, CRC_MODE_CRC16, CRC_STANDARD_CRC_MODBUS);
        csi_crc_calculate(crc, &crc_input, &out, 16);

        while (status.busy == 1) {
            status = csi_crc_get_status(crc);
        }

        csi_crc_uninitialize(crc);

        if (out != expect_out) {
            printf("[error]index %d crc MODBUS mode calculate failed!!!\n", i);
            return;
        } else {
            printf("[success]index %d crc MODBUS mode calculate successfully!!!\n", i);
        }
    }

    printf("[success]test crc successfully\n");
}

/*****************************************************************************
test_main: main function of the crc test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_crc(void)
{
    crc_modbus();
    return 0;
}

int task_CRC(void)
{
	HopeRun_printf("开始CRC测试............\n");
    return example_crc();
}
