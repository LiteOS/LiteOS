/*
 * 
 * 工作流程：
数据、密钥和IV（仅在CBC模式中需要）应写入相应寄存器中
配置CTRL：
加密：Ststart +加密
解密：开始+密钥扩展->开始+解密
等待计算完成，并读出结果
 * 
 * 
 * 
 * */

#include <stdio.h>
#include "soc.h"
#include "drv_aes.h"
#include "los_hwi.h"						//应为调用了 LOS_XXXX() 函数
#include "csky_All_Demo.h"

static aes_handle_t *aes = NULL;
static uint8_t aes_int_flag = 1;

static void aes_event_cb_fun(int32_t idx, aes_event_e event)
{
	printf("[info]进入AES回调函数\n");
    aes_int_flag = 0;
}

static void aes_ecb_256()
{
    const uint8_t in[16] = "Hello, World!";
    const uint8_t expect_out[16] = {0x7b, 0xab, 0x3a, 0xb2, 0xf3, 0x3c, 0xcf, 0xca, 0x20, 0x7a, 0xc9, 0x63, 0x7f, 0x02, 0x49, 0x55 };
    uint8_t out[16];
    const uint8_t key[32] = "Demo-Key";
    uint8_t endian[] = {AES_ENDIAN_LITTLE, AES_ENDIAN_BIG};
    char string[][50] = {"endian little", "endian big"};
    uint32_t index;
	int32_t ret=0;
    int i;
    int j;
    int k;

   // HopeRun_printf("start testing AES ECB Mode\n");

    index = 1;

    for (j = 0; j < index; j++) 
	{
        aes = csi_aes_initialize(j, aes_event_cb_fun);											//success//初始化成功，得到aes句柄//此处中断函数指定无效果
		if(aes==NULL)
		{
			printf("[error]AES初始化失败\n");
			return;
		}
		printf("[success]AES初始化成功\n");
		
        for (k = 0; k < sizeof(endian); k++) 
		{
            ret=csi_aes_config(aes, AES_MODE_ECB, AES_KEY_LEN_BITS_256, endian[k]);						//success//由aes句柄，设置模式：秘钥为key，长度为256，等等
			if(ret!=0)
			{
				printf("[error]csi_aes_config error\n");
				return;
			}
			printf("[success]csi_aes_config success\n");
            
			
			ret=csi_aes_set_key(aes, NULL, (void *)key, AES_KEY_LEN_BITS_256, AES_CRYPTO_MODE_ENCRYPT);	//success//开始加密，从in（原始数据），到out中。
			if(ret!=0)
			{
				printf("[error]csi_aes_set_key error\n");
				return;
			}
			printf("[success]csi_aes_set_key success\n");
			
            
			ret=csi_aes_ecb_crypto(aes, NULL, (void *)in, (void *)out, 16);				//crypto 密码（这里是指加密），16
			if(ret<0)
				{
					printf("[error]csi_aes_ecb_crypto error\n");
					return ;//-1;
				}
			printf("[success]csi_aes_ecb_crypto success\n");
			
			
#if 0      /*it is also valid by get status*/
            aes_status_t status;

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else

            while (aes_int_flag);//等待加密完成。

            aes_int_flag = 1;
#endif

            for (i = 0; i < 16; i++) {
                if (out[i] != expect_out[i]) {
                    break;
                }
            }

            if (i != 16) {
                printf("[error]aes%d aes-256-ecb %s mode encrypt failed\n", j, string[k]);
                return;
            } else {
                printf("[success]aes%d aes-256-ecb %s mode encrypt successfully\n", j, string[k]);
            }

            csi_aes_set_key(aes, NULL, (void *)key, AES_KEY_LEN_BITS_256, AES_CRYPTO_MODE_DECRYPT);
            csi_aes_ecb_crypto(aes, NULL, (void *)out, (void *)out, 16);

#if 0       /*it is also valid by get status*/

            while (1) {
                status = csi_aes_get_status(aes);

                if (status.busy == 0) {
                    break;
                }
            }

#else

            while (aes_int_flag);

            aes_int_flag = 1;
#endif

            for (i = 0; i < 16; i++) {
                if (out[i] != in[i]) {
                    break;
                }
            }

            if (i != 16) {
                printf("[error]aes%d aes-256-ecb %s mode decrypt failed\n", j, string[k]);
                return;
            } else {
                printf("[success]aes%d aes-256-ecb %s mode decrypt successfully\n", j, string[k]);
            }
        }

        csi_aes_uninitialize(aes);
    }

    printf("[success]test aes successfully\n");
}

/*****************************************************************************
example_aes: main function of the aes test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_aes(void)
{
    aes_ecb_256();
    return 0;
}


//extern void ck_aes_irqhandler();

int task_AES(void)
{

#if LOSCFG_PLATFORM_HWI		//接管中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> AES")
#endif
	UINTPTR uvIntSave;																//用于保存所有的中断情况
	uvIntSave = LOS_IntLock();														//禁用所有中断，并返回原先中断情况
	LOS_HwiCreate (AES_IRQn, 2, 0,(void (*) (uintptr_t))ck_aes_irqhandler, 0);
	LOS_IntRestore(uvIntSave);														//恢复所有中断情况
#else							//直接中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> AES")
#endif
	//csi_vic_enable_irq(AES_IRQn);													//初始化时已经开启，此处不需要开启
#endif
	

	HopeRun_printf("开始AES测试............\n");
    return example_aes();
}
