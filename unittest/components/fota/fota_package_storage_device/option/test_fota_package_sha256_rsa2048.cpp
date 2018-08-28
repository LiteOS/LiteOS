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

#include "test_fota_package_sha256_rsa2048.h"

#define FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN 256
#define FOTA_PACK_SHA256_CHECKSUM_LEN 32

extern int fota_pack_sha256_rsa2048_init(fota_pack_sha256_rsa2048_s *thi, fota_pack_head_s *head);
extern int mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s );
extern int mbedtls_rsa_check_pubkey( const mbedtls_rsa_context *ctx );
extern void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] );

extern int mbedtls_rsa_pkcs1_verify( mbedtls_rsa_context *ctx,
                      int (*f_rng)(void *, unsigned char *, size_t),
                      void *p_rng,
                      int mode,
                      mbedtls_md_type_t md_alg,
                      unsigned int hashlen,
                      const unsigned char *hash,
                      const unsigned char *sig );
extern void mbedtls_rsa_free( mbedtls_rsa_context *ctx );

static void test_mbedtls_rsa_free( mbedtls_rsa_context *ctx )
{
}

static int flag_test_mbedtls_mpi_read_string;
static int test_mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s )
{   
    if (flag_test_mbedtls_mpi_read_string == 0)
        return FOTA_OK;
    if (flag_test_mbedtls_mpi_read_string != 0)
        return FOTA_ERR;
}

static int flag_test_mbedtls_rsa_check_pubkey;
static int test_mbedtls_rsa_check_pubkey( const mbedtls_rsa_context *ctx )
{
    if (flag_test_mbedtls_rsa_check_pubkey == 0)
        {
            return 0;
        }
    if (flag_test_mbedtls_rsa_check_pubkey != 0)
        return -1;
}

static int flag_test_mbedtls_sha256_finish;
static void test_mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] )
{
    return;
}

static int flag_test_mbedtls_rsa_pkcs1_verify;
static int test_mbedtls_rsa_pkcs1_verify( mbedtls_rsa_context *ctx,
                      int (*f_rng)(void *, unsigned char *, size_t),
                      void *p_rng,
                      int mode,
                      mbedtls_md_type_t md_alg,
                      unsigned int hashlen,
                      const unsigned char *hash,
                      const unsigned char *sig )
{
    if (flag_test_mbedtls_rsa_pkcs1_verify == 0)
        return 0;
    if (flag_test_mbedtls_rsa_pkcs1_verify != 0)
        return -1;
}


static fota_pack_sha256_rsa2048_s thi;

void TestFotaPackageSha256Rsa2048::test_fota_pack_sha256_rsa2048_init()
{
    int ret = fota_pack_sha256_rsa2048_init(&thi, NULL);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_rsa2048_init() is failed");
}

void TestFotaPackageSha256Rsa2048::test_fota_pack_sha256_rsa2048_check()
{   
    stubInfo si_read_string;
    setStub((void *)mbedtls_mpi_read_string, (void *)test_mbedtls_mpi_read_string, &si_read_string);
    stubInfo si_check_pubkey;
    setStub((void *)mbedtls_rsa_check_pubkey, (void *) test_mbedtls_rsa_check_pubkey, &si_check_pubkey);
    stubInfo si_finish;
    setStub((void *)mbedtls_sha256_finish, (void *)test_mbedtls_sha256_finish, &si_finish);
    stubInfo si_verify;
    setStub((void *)mbedtls_rsa_pkcs1_verify, (void *)test_mbedtls_rsa_pkcs1_verify, &si_verify);
    stubInfo si_rsa_free;
    setStub((void *)mbedtls_rsa_free, (void *)test_mbedtls_rsa_free, &si_rsa_free);
    int ret = 0;
    ret = thi.sha256.base.check(NULL, NULL, 0);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_rsa2048_check() is failed");

    thi.head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(thi.head, 0, sizeof(fota_pack_head_s));
    const char a = '1';
    const char b = '1';
    thi.head->key.rsa_E = &a;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_rsa2048_check() is failed");

    thi.head->key.rsa_N = &b;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_rsa2048_check() is failed");
#if 1
    flag_test_mbedtls_mpi_read_string = -1;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_rsa2048_check() is failed");
   
    flag_test_mbedtls_mpi_read_string = 0;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_rsa2048_check() is failed");

    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_rsa2048_check() is failed");

    flag_test_mbedtls_rsa_check_pubkey = -1;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_rsa2048_check() is failed");

    flag_test_mbedtls_rsa_check_pubkey = 0;
    flag_test_mbedtls_rsa_pkcs1_verify = -1;
    ret = thi.sha256.base.check(&thi.sha256.base, NULL, FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_rsa2048_check() is failed");
#endif
    free(thi.head);
}

TestFotaPackageSha256Rsa2048::TestFotaPackageSha256Rsa2048()
{
    TEST_ADD(TestFotaPackageSha256Rsa2048::test_fota_pack_sha256_rsa2048_init);
    TEST_ADD(TestFotaPackageSha256Rsa2048::test_fota_pack_sha256_rsa2048_check);
}

void TestFotaPackageSha256Rsa2048::setup()
{
    std::cout<<"in steup\n";
}

void TestFotaPackageSha256Rsa2048::tear_down()
{
    std::cout<<"in teardown\n";
}



