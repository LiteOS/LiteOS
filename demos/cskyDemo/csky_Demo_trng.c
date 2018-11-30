

#include <stdio.h>
#include "soc.h"
#include "drv_trng.h"
#include "csky_All_Demo.h"
#define NUM     20

static trng_handle_t trng = NULL;

static void trng_get_num_cb(int32_t idx, trng_event_e event)
{
    printf("[info]进入trng回调函数：the data genetate complete\n");
}

static void get_num(void)
{
    int i;
    int j;
    int k;
    uint8_t  data[NUM] = {0x0};
    trng_status_t status;
    int index = 0;
    index = 1;

    if (index >= 0) {
        for (k = 0; k < index; k++) 
		{
            trng = csi_trng_initialize(k, trng_get_num_cb);
            csi_trng_get_data(trng, data, NUM);

            while (1) 
			{
                status = csi_trng_get_status(trng);

                if (status.busy == 0 && status.data_valid == 1) {
                    break;
                }
            }

            csi_trng_uninitialize(trng);

            for (i = 0; i < NUM; i++) {
                printf("[info]the index %d data is %x\n", i, data[i]);
            }

            for (i = 1; i < NUM; i++) {
                for (j = 0; j < i; j++) {
                    if (data[i] == data[j]) {
                        break;
                    }
                }
            }

            if (i != NUM) {
                printf("[error]the index %d trng get num is failed\r\n", k);
                return;
            } else {
                printf("[success]the index %d trng get num is successfully\r\n", k);
            }
        }
    }

    printf("[success]test trng successfully\n");
}

/*****************************************************************************
test_trng: main function of the trng test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_trng(void)
{
    get_num();
    return 0;
}

int task_TRNG(void)
{
	//开始测试
	HopeRun_printf("开始TRNG测试............\n");
    return example_trng();
}
