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
/*******
 * Notice:
 * The parts of this file refer to https://blog.csdn.net/zlk1214/article/details/76177336
 * Originator: ZLK1214
 *******/
 
#ifndef __WPA_H__
#define __WPA_H__

#define HMAC_MD5_BLOCKSIZE      64
#define HMAC_MD5_OUTPUTSIZE     16
#define HMAC_SHA1_BLOCKSIZE     64
#define HMAC_SHA1_OUTPUTSIZE    20

typedef void (*HashFunction)(unsigned char *input, int ilen, unsigned char *output);

void ARC4_decrypt_keydata(const uint8_t *KEK, const uint8_t *key_iv, const uint8_t *data, uint16_t datalen, uint8_t *output);
uint16_t AES_unwrap(const uint8_t *key, const uint8_t *data, uint16_t datalen, uint8_t *output);
uint8_t hmac(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, HashFunction hash, uint8_t blocksize, uint8_t *output, uint8_t outputsize);
uint8_t hmac_md5(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, uint8_t output[HMAC_MD5_OUTPUTSIZE]);
uint8_t hmac_sha1(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, uint8_t output[HMAC_SHA1_OUTPUTSIZE]);
uint8_t pbkdf2_hmac_sha1(const void *password, uint16_t pwdlen, const void *salt, uint16_t saltlen, uint32_t c, uint8_t *dk, uint16_t dklen);
uint8_t PRF(const void *k, uint16_t klen, const char *a, const void *b, uint16_t blen, void *output, uint8_t n);

#endif // __WPA_H__