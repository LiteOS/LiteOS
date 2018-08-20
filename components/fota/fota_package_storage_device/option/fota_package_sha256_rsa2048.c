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
#ifdef INCLUDE_FOTA_PACK_OPTION_FILE
#include "fota_package_sha256_rsa2048.h"
#include <string.h>
#include "rsa.h"

#define FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN 256
#define FOTA_PACK_SHA256_CHECKSUM_LEN 32


static int fota_pack_sha256_rsa2048_check(fota_pack_checksum_alg_s *thi, const uint8_t  *checksum, uint16_t checksum_len)
{
    fota_pack_sha256_rsa2048_s *rsa = (fota_pack_sha256_rsa2048_s *)thi;
    mbedtls_rsa_context *dtls_rsa = NULL;
    fota_pack_key_s *key = NULL;
    uint8_t real_sha256[FOTA_PACK_SHA256_CHECKSUM_LEN];
    int ret = FOTA_ERR;

    if(checksum_len != FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN)
    {
        FOTA_LOG("checksum_len err %d", checksum_len);
        return FOTA_ERR;
    }

    key = fota_pack_head_get_key(rsa->head);
    if(NULL == key
            || (NULL == key->rsa_E)
            || (NULL == key->rsa_N))
    {
        FOTA_LOG("key null");
        return FOTA_ERR;
    }

    dtls_rsa = (mbedtls_rsa_context *)atiny_malloc(sizeof(*dtls_rsa));
    if(NULL == dtls_rsa)
    {
        FOTA_LOG("atiny_malloc null");
        return FOTA_ERR;
    }

    mbedtls_rsa_init(dtls_rsa, MBEDTLS_RSA_PKCS_V21, 0);
    dtls_rsa->len = FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN;
    if(mbedtls_mpi_read_string(&dtls_rsa->N, 16, key->rsa_N) != FOTA_OK)
    {
        FOTA_LOG("mbedtls_mpi_read_string fail");
        goto EXIT;
    }

    if(mbedtls_mpi_read_string(&dtls_rsa->E, 16, key->rsa_E) != FOTA_OK)
    {
        FOTA_LOG("mbedtls_mpi_read_string fail");
        goto EXIT;
    }

    if(mbedtls_rsa_check_pubkey(dtls_rsa) != FOTA_OK)
    {
        FOTA_LOG("mbedtls_rsa_check_pubkey fail");
        goto EXIT;
    }

    mbedtls_sha256_finish(&rsa->sha256.sha256_context, real_sha256);

    if( mbedtls_rsa_pkcs1_verify(dtls_rsa, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_SHA256, 0,
                                 real_sha256, checksum) != FOTA_OK)
    {
        FOTA_LOG("mbedtls_rsa_pkcs1_verify fail");
        goto EXIT;
    }

    ret = FOTA_OK;
EXIT:

    mbedtls_rsa_free(dtls_rsa);
    atiny_free(dtls_rsa);
    return ret;

}



int fota_pack_sha256_rsa2048_init(fota_pack_sha256_rsa2048_s *thi, fota_pack_head_s *head)
{
    (void)fota_pack_sha256_init(&thi->sha256);
    thi->sha256.base.check = fota_pack_sha256_rsa2048_check;
    thi->head = head;
    return FOTA_OK;
}
#endif


