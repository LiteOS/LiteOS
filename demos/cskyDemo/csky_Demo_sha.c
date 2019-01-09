

#include <stdio.h>
#include "soc.h"
#include "drv_sha.h"
#include "csky_All_Demo.h"
#define CONFIG_SHA_BLOCK_MODE   1
#define CONFIG_SHA_BIG_MODE     1
#define CONFIG_SHA_LITTLE_MODE  0

sha_handle_t *sha = NULL;
static uint8_t sha1_int_flag = 1;

static void sha_interrupt_handler(int32_t idx, sha_event_e event)
{
	printf("[info]进入SHA回调函数\n");
    sha1_int_flag = 0;
}

/*****************************************************************************
sha_512: test the 512 bit encode

INPUT: NULL

RETURN: NULL

*****************************************************************************/
void sha1_512()
{
    uint8_t sha_context[SHA_CONTEXT_SIZE] = {0x0};
    uint32_t data[5];
    uint32_t data_in_big[] = {0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
                              0x64636261, 0x68676665, 0x64636261, 0x68676665, 0x64636261, 0x68676665,
                              0x64636261, 0x68676665, 0x64636261, 0x68676665
                             };
    uint32_t expect_out[] = {0xeec1d8cb, 0x82117907, 0x90225616, 0x2d909a57, 0x0a08c7a9};

    printf("[info]Testing sha-1 512bit\n");

    int i;
    int j;
    uint32_t index;
    i = 5;

    index = 1;
    sha_status_t status;

    for (i = 0; i < index; i++) {
        /* init the sha and select the mode and endian mode of sha */
//        sha = csi_sha_initialize(i, sha_interrupt_handler);
        sha = csi_sha_initialize(i, &sha_context, sha_interrupt_handler);
#if CONFIG_SHA_LITTLE_MODE
        uint32_t data_in_little[] = {0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
                                     0x61626364, 0x65666768, 0x61626364, 0x65666768, 0x61626364, 0x65666768,
                                     0x61626364, 0x65666768, 0x61626364, 0x65666768
                                    };
        csi_sha_config(sha, SHA_MODE_1, SHA_ENDIAN_MODE_LITTLE);

        csi_sha_start(sha, NULL);
        csi_sha_update(sha, NULL, data_in_little, sizeof(data_in_little));

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/

        while (1) {
            status = csi_sha_get_status(sha);

            if (status.busy == 0) {
                break;
            }
        }

        csi_sha_finish(sha, NULL, data);
#else
        csi_sha_finish(sha, NULL, data);

        while (sha1_int_flag);

        sha1_int_flag = 1;
#endif

        for (j = 0; j < 5; j++) {
            if (data[j] != expect_out[j]) {
                break;
            }
        }

        if (j == 5) {
            printf("[success]index %d sha1-512 little encode successfully\n", i);
        } else {
            printf("[error]index %d sha1-512 little encode failed\n", i);
            return;
        }

#endif

#if CONFIG_SHA_BIG_MODE
        csi_sha_config(sha, SHA_MODE_1, SHA_ENDIAN_MODE_BIG);
        csi_sha_start(sha, NULL);
        csi_sha_update(sha, NULL, data_in_big, sizeof(data_in_big));

#if CONFIG_SHA_BLOCK_MODE   /*it is also valid by get status*/

        while (1) {
            status = csi_sha_get_status(sha);

            if (status.busy == 0) {
                break;
            }
        }

        csi_sha_finish(sha, NULL, data);
#else
        csi_sha_finish(sha, NULL, data);

        while (sha1_int_flag);

        sha1_int_flag = 1;
#endif
        csi_sha_uninitialize(sha);

        for (j = 0; j < 5; j++) {
            if (data[j] != expect_out[j]) {
                break;
            }
        }

        if (j == 5) {
            printf("[success]index %d sha1-512 big encode successfully\n", i);
        } else {
            printf("[error]index %d sha1-512 big encode failed\n", i);
            return;
        }

#endif
    }

    printf("[success]test sha successfully\n");
}

/*****************************************************************************
test_sha: main function of the sha test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_sha(void)
{
    sha1_512();
    return 0;
}

int task_SHA(void)
{
	//开始测试
	HopeRun_printf("开始SHA测试............\n");
    return example_sha();
}
