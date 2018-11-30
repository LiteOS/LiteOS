/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     ck_aes.c
 * @brief    CSI Source File for aes driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include <string.h>
#include "drv_aes.h"
#include "ck_aes.h"
#include "csi_core.h"

//#include "los_hwi.h"	
extern void mdelay(uint32_t ms);
//#define HopeRun_printf (printf("[%s]--->[%d]",__FUNCTION__, __LINE__ ), printf) 
#define ERR_AES(errno) (CSI_DRV_ERRNO_AES_BASE | errno)
#define AES_NULL_PARA_CHK(para)                         \
    do {                                        \
        if (para == NULL) {                     \
            return ERR_AES(DRV_ERROR_PARAMETER);   \
        }                                       \
    } while (0)
static ck_aes_reg_t *aes_reg = NULL;
volatile static uint8_t block_cal_done = 0;

typedef struct {
    uint32_t base;
    uint32_t irq;
    void *iv;
    uint8_t *result_out;
    uint32_t len;
    aes_event_cb_t cb;
    aes_mode_e mode;
    aes_key_len_bits_e keylen;
    aes_endian_mode_e endian;
    aes_crypto_mode_e enc;
    aes_status_t status;
} ck_aes_priv_t;

extern int32_t target_get_aes_count(void);
extern int32_t target_get_aes(int32_t idx, uint32_t *base, uint32_t *irq);

static ck_aes_priv_t aes_handle[CONFIG_AES_NUM];

/* Driver Capabilities */
static const aes_capabilities_t driver_capabilities = {
    .ecb_mode = 1, /* ECB mode */
    .cbc_mode = 1, /* CBC mode */
    .cfb1_mode = 0, /* CFB1 mode */
    .cfb8_mode = 0, /* CFB8 mode */
    .cfb128_mode = 0, /* CFB128 mode */
    .ofb_mode = 0, /* OFB mode */
    .ctr_mode = 0, /* CTR mode */
    .bits_128 = 1, /* 128bits key length mode */
    .bits_192 = 1, /* 192bits key lenght mode */
    .bits_256 = 1  /* 256bits key length mode */
};

typedef enum {
    AES_PADDING_MODE_NO     = 0,        ///< NO-PADDING
    AES_PADDING_MODE_ZERO      ,        ///< ZERO-PADDING
    AES_PADDING_MODE_PKCS5              ///< PKCS5-PADDING
} aes_padding_mode_e;


//
// Functions
//

static inline void aes_set_opcode(aes_crypto_mode_e opcode)
{
    aes_reg->ctrl &= ~(3 << AES_OPCODE_OFFSET);          	//clear bit[7:6]
    aes_reg->ctrl |= (opcode << AES_OPCODE_OFFSET);    		//set opcode
}

static inline void aes_set_endian(aes_endian_mode_e endian)
{
    if (endian == AES_ENDIAN_LITTLE) {
        aes_reg->ctrl &= ~AES_LITTLE_ENDIAN;
    } else {
        aes_reg->ctrl |= AES_LITTLE_ENDIAN;
    }
}

static inline uint32_t aes_set_keylen(aes_key_len_bits_e keylength)
{
    aes_reg->ctrl &= ~(3 << AES_KEY_LEN_OFFSET);        	//clear bit[5:4]
    aes_reg->ctrl |= (keylength << AES_KEY_LEN_OFFSET);		// Set key length

    return 0;
}

static inline void aes_set_mode(aes_mode_e mode)
{
    aes_reg->ctrl &= ~(1 << AES_MODE_OFFSET);      			//clear bit 3
    aes_reg->ctrl |= (mode << AES_MODE_OFFSET);  			//set mode
}

static inline void aes_enable(void)
{
    aes_reg->ctrl |= (1 << AES_WORK_ENABLE_OFFSET);
}

static inline void aes_disable(void)
{
    aes_reg->ctrl &= ~(1 << AES_WORK_ENABLE_OFFSET);
}

static inline void aes_enable_interrupt(void)
{
    aes_reg->ctrl |= (1 << AES_INT_ENABLE_OFFSET);
}

static inline void aes_disable_interrupt(void)
{
    aes_reg->ctrl &= ~(1 << AES_INT_ENABLE_OFFSET);
}

static inline void aes_clear_interrupt(void)
{
    aes_reg->state = 0x0;
}

static inline uint32_t aes_get_intstatus(uint32_t AES_IT)
{
    return (aes_reg->state & AES_IT) ? 1 : 0;
}

static void aes_set_key(void *context, uint8_t *key, aes_key_len_bits_e keylen, uint32_t enc, uint32_t endian)
{
    uint8_t keynum = 0;

    if (keylen == AES_KEY_LEN_BITS_128) {
        keynum = 4;
    } else if (keylen == AES_KEY_LEN_BITS_192) {
        keynum = 6;
    } else if (keylen == AES_KEY_LEN_BITS_256) {
        keynum = 8;
    }

    uint32_t i;
    uint32_t temp = 0;

    /* set key according to the endian mode */
    if (endian == AES_ENDIAN_LITTLE) {
        for (i = 0; i < keynum; i++) {
            temp = key[3] << 24 | key[2] << 16 | key[1] << 8 | key[0];
            aes_reg->key[keynum - 1 - i] = temp;
            key += 4;
        }
    } else if (endian == AES_ENDIAN_BIG) {
        for (i = 0; i < keynum; i++) {
            temp = key[3] << 24 | key[2] << 16 | key[1] << 8 | key[0];
            aes_reg->key[i] = temp;
            key += 4;
        }
    }

    if (enc == AES_CRYPTO_MODE_DECRYPT) {
        aes_set_opcode(AES_CRYPTO_KEYEXP);      /* if the mode is decrypt before decrypt you have to keyexpand */
        aes_enable();
//        while(block_cal_done == 0);
//        block_cal_done = 0;

        while (aes_get_intstatus(AES_IT_KEYINT));

        aes_set_opcode(AES_CRYPTO_MODE_DECRYPT);
    } else if (enc == AES_CRYPTO_MODE_ENCRYPT) {
        aes_set_opcode(AES_CRYPTO_MODE_ENCRYPT);
    }

    aes_disable();
}

static void aes_set_iv(uint32_t mode, uint32_t endian, uint8_t *iv)
{
    uint32_t temp;
    uint32_t i;

    /* set iv if the mode is CBC */
    if (mode == AES_MODE_CBC) {
        if (endian == AES_ENDIAN_BIG) {
            for (i = 0; i < 4; i++) {
                temp = iv[3] << 24 | iv[2] << 16 | iv[1] << 8 | iv[0];
                aes_reg->iv[i] = temp;
                iv += 4;
            }
        } else if (endian == AES_ENDIAN_LITTLE) {
            for (i = 0; i < 4; i++) {
                temp = iv[3] << 24 | iv[2] << 16 | iv[1] << 8 | iv[0];
                aes_reg->iv[3 - i] = temp;
                iv += 4;
            }
        }
    }
}
//加密
//context (NULL when hardware implementation)
//
static int aes_crypto(void *context, uint8_t *in, uint8_t *out,
                      uint32_t len, uint8_t *iv, uint32_t mode, uint32_t endian, uint32_t enc)
{

    uint32_t temp[4];
    aes_set_iv(mode, endian, iv);			//设置初始向量（用于使得加密更复杂）
    uint32_t i = 0;
    uint32_t j = 0;
	uint8_t timeout=0;
    /* set the text before aes calculating */
	//在计算之前，先对数据进行设置
    for (i = 0; i < len; i = i + 16) {
        for (j = 0; j < 4; j++) {
            temp[j] = in[3] << 24 | in[2] << 16 | in[1] << 8 | in[0];

            if (endian == AES_ENDIAN_BIG) 
			{
                aes_reg->datain[j] = temp[j];
            } 
			else if (endian == AES_ENDIAN_LITTLE) {
                aes_reg->datain[3 - j] = temp[j];
            }

            in += 4;
			
			
        }

		//置位CTRL的最低位，AES开始工作。
        aes_enable();

        while (block_cal_done == 0)//等待块计算完成
		{
				mdelay(1000);
				if(timeout++>=3)
				{
						printf("[error]等待超时，未进入中断函数\n");
						return -1;
				}
		}

        block_cal_done = 0;

        if (enc == AES_CRYPTO_MODE_ENCRYPT && mode == AES_MODE_CBC) {
            aes_set_iv(mode, endian, out);
            memcpy(iv, out, 16);
            out += 16;
        } else if (enc == AES_CRYPTO_MODE_DECRYPT && mode == AES_MODE_CBC) {
            aes_set_iv(mode, endian, (uint8_t *)&temp);
            memcpy(iv, temp, 16);
        }
    }

    return 0;
}


//必须要进入到这个中断中，使得block_cal_done = 1;否则程序会卡在某一环节
void ck_aes_irqhandler(int32_t idx)
{
    ck_aes_priv_t *aes_priv = &aes_handle[idx];
    volatile uint32_t j;
    uint32_t tmp = 0;
	printf("[info]进入AES底层中断函数\n");
    /* get the result after aes calculating*/
    if (aes_priv->result_out != NULL) {
        for (j = 0; j < 4; j++) {
            if (aes_priv->endian == AES_ENDIAN_BIG) {
                tmp = aes_reg->dataout[j];
            } else if (aes_priv->endian == AES_ENDIAN_LITTLE) {
                tmp = aes_reg->dataout[3 - j];
            }

            memcpy(aes_priv->result_out, &tmp, 4);
            aes_priv->result_out += 4;
            aes_priv->len -= 4;
        }
    }

    block_cal_done = 1;
    /* disable aes and clear the aes interrupt */
    aes_disable();
    aes_clear_interrupt();

    /* execute the callback function */
    if (aes_priv->len == 0) {
        if (aes_priv->cb) {
            aes_priv->cb(idx, AES_EVENT_CRYPTO_COMPLETE);
        }
    }
}

/**
  \brief       Initialize AES Interface. 1. Initializes the resources needed for the AES interface 2.registers event callback function
  \param[in]   idx device id
  \param[in]   cb_event  Pointer to \ref aes_event_cb_t
  \return      return aes handle if success
*/
aes_handle_t csi_aes_initialize(int32_t idx, aes_event_cb_t cb_event)
{

    if (idx < 0 || idx >= CONFIG_AES_NUM) 
	{
        return NULL;
    }

    uint32_t irq = 0u;
    uint32_t base = 0u;
    /* obtain the aes information */
    int32_t real_idx = target_get_aes(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    ck_aes_priv_t *aes_priv = &aes_handle[idx];

    aes_priv->base = base;
    aes_priv->irq  = irq;

    /* initialize the aes context */
    aes_reg = (ck_aes_reg_t *)(aes_priv->base);
    aes_priv->cb = cb_event;
    aes_priv->iv = NULL;
    aes_priv->len = 16;
    aes_priv->result_out = NULL;
    aes_priv->mode = AES_MODE_CBC;
    aes_priv->keylen = AES_KEY_LEN_BITS_128;
    aes_priv->endian = AES_ENDIAN_LITTLE;
    aes_priv->status.busy = 0;

    aes_enable_interrupt();             /* enable the aes interrupt */

    csi_vic_enable_irq(aes_priv->irq); /* enable the aes bit in vic */

    return (aes_handle_t)aes_priv;
}

/**
  \brief       De-initialize AES Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle  aes handle to operate.
  \return      error code
*/
int32_t csi_aes_uninitialize(aes_handle_t handle)
{
    AES_NULL_PARA_CHK(handle);

    ck_aes_priv_t *aes_priv = handle;
    aes_priv->cb = NULL;

    aes_disable_interrupt();            /* disable the aes interrupt */

    csi_vic_disable_irq(aes_priv->irq);

    return 0;
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx device id.
  \return      \ref aes_capabilities_t
*/
aes_capabilities_t csi_aes_get_capabilities(int32_t idx)
{
    if (idx < 0 || idx >= CONFIG_AES_NUM) {
        aes_capabilities_t ret;
        memset(&ret, 0, sizeof(aes_capabilities_t));
        return ret;
    }

    return driver_capabilities;
}

/**
  \brief       config aes mode.
  \param[in]   handle  aes handle to operate.
  \param[in]   mode      \ref aes_mode_e
  \param[in]   keylen_bits \ref aes_key_len_bits_e
  \param[in]   endian    \ref aes_endian_mode_e
  \return      error code
*/
int32_t csi_aes_config(aes_handle_t handle, aes_mode_e mode, aes_key_len_bits_e keylen_bits, aes_endian_mode_e endian)
{
    AES_NULL_PARA_CHK(handle);

    ck_aes_priv_t *aes_priv = handle;
    aes_reg = (ck_aes_reg_t *)(aes_priv->base);

    /* config the aes mode */
    switch (mode) {
        case AES_MODE_CBC:
            aes_priv->mode = mode;
            aes_set_mode(mode);
            break;

        case AES_MODE_ECB:
            aes_priv->mode = mode;
            aes_set_mode(mode);
            break;

        case AES_MODE_CFB1:
        case AES_MODE_CFB8:
        case AES_MODE_CFB128:
        case AES_MODE_OFB:
        case AES_MODE_CTR:
            return ERR_AES(DRV_ERROR_UNSUPPORTED);

        default:
            return ERR_AES(AES_ERROR_MODE);
    }

    /* config the key length */
    switch (keylen_bits) {
        case AES_KEY_LEN_BITS_128:
        case AES_KEY_LEN_BITS_192:
        case AES_KEY_LEN_BITS_256:
            aes_priv->keylen = keylen_bits;
            aes_set_keylen(keylen_bits);
            break;

        default:
            return ERR_AES(AES_ERROR_DATA_BITS);
    }

    /* config the endian mode */
    switch (endian) {
        case AES_ENDIAN_LITTLE:
            aes_priv->endian = endian;
            aes_set_endian(endian);
            break;

        case AES_ENDIAN_BIG:
            aes_priv->endian = endian;
            aes_set_endian(endian);
            break;

        default:
            return ERR_AES(AES_ERROR_ENDIAN);
    }

    return 0;
}

/**
  \brief       set crypto key.
  \param[in]   handle    aes handle to operate.
  \param[in]   context   aes information context(NULL when hardware implementation)
  \param[in]   key       Pointer to the key buf
  \param[in]   key_len   Pointer to the aes_key_len_bits_e
  \param[in]   enc       \ref aes_crypto_mode_e
  \return      error code
*/
int32_t csi_aes_set_key(aes_handle_t handle, void *context, void *key, aes_key_len_bits_e key_len, aes_crypto_mode_e enc)
{
    AES_NULL_PARA_CHK(handle);
    AES_NULL_PARA_CHK(key);

    if ((key_len != AES_KEY_LEN_BITS_128 &&
         key_len != AES_KEY_LEN_BITS_192 &&
         key_len != AES_KEY_LEN_BITS_256) ||
        (enc != AES_CRYPTO_MODE_ENCRYPT &&
         enc != AES_CRYPTO_MODE_DECRYPT)) {
        return ERR_AES(DRV_ERROR_PARAMETER);
    }

    ck_aes_priv_t *aes_priv = handle;
    aes_priv->enc = enc;
    aes_set_key(context, key, key_len, enc, aes_priv->endian);

    return 0;
}

/**
  \brief       encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   padding \ref aes_padding_mode_e.
  \return      error code
*/
int32_t csi_aes_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, aes_padding_mode_e padding)
{
    AES_NULL_PARA_CHK(handle);	//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(in);		//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(out);		//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(len);		//aes的空参数检查，防止段错误
	//printf("参数检查，准备是否填充padding\n");
    ck_aes_priv_t *aes_priv = handle;
	int32_t ret=0;
    aes_priv->status.busy = 1;
    uint8_t left_len = len & 0xf;		//0000		0000	0000	0000	0000	0000	0001	0000(16)
	//left_len 必须要为0000，以为这样整个len长度表示的是16的整数倍。这是加密必要的条件（只能对16Byte的整数倍进行加密，（128b）），否则需要填充Padding
	//此处实例直接指定就是16 的倍数，因为不需要填充到16的倍数
    switch (padding) 
	{
        case AES_PADDING_MODE_NO:		//使用的是 加密无填充模式
            if (left_len) 				//0000不会进入该语句
			{
                return ERR_AES(DRV_ERROR_PARAMETER);
            }

            /* crypto in padding no mode */
            aes_priv->result_out = out;
            aes_priv->len = len;
			//printf("不进行填充，准备加密aes_crypto\n");
            ret=aes_crypto(context, in, out, len, aes_priv->iv, aes_priv->mode, aes_priv->endian, aes_priv->enc);//卡在这里等待
			if(ret<0)return ret;
            break;
        case AES_PADDING_MODE_ZERO:
            if (left_len == 0) {
                return ERR_AES(DRV_ERROR_PARAMETER);
            }
            uint8_t i = 0;

            for (i = 0; i < (16 - left_len); i++) {
                *((uint8_t *)in + len + i) = 0x0;
            }

            /* crypto in padding zero mode */
            aes_priv->result_out = out;
            aes_priv->len = len + 16 - left_len;
            ret=aes_crypto(context, in, out, len + 16 - left_len, aes_priv->iv, aes_priv->mode, aes_priv->endian, aes_priv->enc);
			if(ret<0)return ret;
            break;
        case AES_PADDING_MODE_PKCS5:
            return ERR_AES(DRV_ERROR_UNSUPPORTED);
        default:
            return ERR_AES(DRV_ERROR_PARAMETER);
    }
	//printf("--------------ok----------------\n");
    aes_priv->status.busy = 0;
    return 0;
}

/**
  \brief       aes ecb encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \return      error code
*/
int32_t csi_aes_ecb_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len)
{
    AES_NULL_PARA_CHK(handle);
    AES_NULL_PARA_CHK(in);
    AES_NULL_PARA_CHK(out);
    AES_NULL_PARA_CHK(len);//16
	//printf("参数检查\n");
    return csi_aes_crypto(handle, context, in, out, len, AES_PADDING_MODE_NO);
}

/**
  \brief       aes cbc encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   iv   Pointer to initialization vector(updated after use)
  \return      error code
*/
int32_t csi_aes_cbc_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, uint8_t iv[16])
{
    AES_NULL_PARA_CHK(handle);	//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(in);		//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(out);		//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(len);		//aes的空参数检查，防止段错误
    AES_NULL_PARA_CHK(iv);		//aes的空参数检查，防止段错误

    ck_aes_priv_t *aes_priv = handle;
    aes_priv->iv = (void *)iv;	//指向初始化向量的指针



	//再向里面传参
    return csi_aes_crypto(handle, context, in, out, len, AES_PADDING_MODE_NO);
}

/**
  \brief       aes cfb1 encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   iv   Pointer to initialization vector(updated after use)
  \return      error code
*/
int32_t csi_aes_cfb1_crypto(aes_handle_t handle, void *context, void *in, void *out,  uint32_t len, uint8_t iv[16])
{
    return ERR_AES(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       aes cfb8 encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   iv   Pointer to initialization vector(updated after use)
  \return      error code
*/
int32_t csi_aes_cfb8_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, uint8_t iv[16])
{
    return ERR_AES(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       aes cfb128 encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   iv   Pointer to initialization vector(updated after use)
  \param[in]   num  the number of the 128-bit block we have used(updated after use)
  \return      error code
*/
int32_t csi_aes_cfb128_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, uint8_t iv[16], uint32_t *num)
{
    return ERR_AES(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       aes ofb encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   iv   Pointer to initialization vector(updated after use)
  \param[in]   num  the number of the 128-bit block we have used(updated after use)
  \return      error code
*/
int32_t csi_aes_ofb_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, uint8_t iv[16], uint32_t *num)
{
    return ERR_AES(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       aes ofb encrypt or decrypt
  \param[in]   handle  aes handle to operate.
  \param[in]   context aes information context(NULL when hardware implementation)
  \param[in]   in   Pointer to the Source data
  \param[out]  out  Pointer to the Result data.
  \param[in]   len  the Source data len.
  \param[in]   nonce_counter   Pointer to the 128-bit nonce and counter(updated after use)
  \param[in]   stream_block  Pointer to the saved stream-block for resuming(updated after use)
  \param[in]   num  the number of the 128-bit block we have used(updated after use)
  \return      error code
*/
int32_t csi_aes_ctr_crypto(aes_handle_t handle, void *context, void *in, void *out, uint32_t len, uint8_t nonce_counter[16],
                           unsigned char stream_block[16], uint32_t *num)
{
    return ERR_AES(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       Get AES status.
  \param[in]   handle  aes handle to operate.
  \return      AES status \ref aes_status_t
*/
aes_status_t csi_aes_get_status(aes_handle_t handle)
{
    if (handle == NULL) {
        aes_status_t ret;
        memset(&ret, 0, sizeof(aes_status_t));
        return ret;
    }

    ck_aes_priv_t *aes_priv = handle;
    return aes_priv->status;
}
