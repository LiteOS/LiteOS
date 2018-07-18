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

#include <stdlib.h>
#include <string.h>
#include "tiny_aes.h"

// base on mbedtls
#include "compat-1.3.h"
#include "arc4.h"
#include "md5.h"
#include "sha1.h"
#include "wpa.h"

/*
 * ARC4 cipher function
 */
void arc4_crypt_raw( arc4_context *ctx, unsigned char *buf, int buflen )
{
    int i, x, y, a, b;
    unsigned char *m;

    x = ctx->x;
    y = ctx->y;
    m = ctx->m;

    for( i = 0; i < buflen; i++ )
    {
        x = ( x + 1 ) & 0xFF; a = m[x];
        y = ( y + a ) & 0xFF; b = m[y];

        m[x] = (unsigned char) b;
        m[y] = (unsigned char) a;

        buf[i] = (unsigned char)
            ( buf[i] ^ m[(unsigned char)( a + b )] );
    }

    ctx->x = x;
    ctx->y = y;
}

/* ARC4 decrypts the TKIP Key Data packet */
// The encryption and decryption algorithm for ARC4 is the same, but the arc4_setup function must be called once every time
// Refer to the article: http://www.fenlog.com/post/111.html
void ARC4_decrypt_keydata(const uint8_t *KEK, const uint8_t *key_iv, const uint8_t *data, uint16_t datalen, uint8_t *output)
{
    arc4_context ctx;
    uint8_t dummy[256] = {0};
    uint8_t newkey[2][16];

    memcpy(newkey[0], key_iv, sizeof(newkey[0]));
    memcpy(newkey[1], KEK, sizeof(newkey[1]));

    arc4_setup(&ctx, newkey[0], sizeof(newkey));
    arc4_crypt_raw(&ctx, dummy, sizeof(dummy)); // discard the first 256 bytes of the RC4 cipher stream output
    memcpy(output, data, datalen);
    arc4_crypt_raw(&ctx, output, datalen);
}

/* AES Key Wrap decryption algorithm decrypts AES Key Data packets */
// 88W8686 does not support the CMD_802_11_CRYPTO command, so the algorithm must be implemented with software
// RFC3394.pdf: 2.2.2 Key Unwrap
// The return value is the data size of the output result
uint16_t AES_unwrap(const uint8_t *key, const uint8_t *data, uint16_t datalen, uint8_t *output)
{
    uint8_t a[8], b[16];
    uint8_t *r;
    uint16_t i, j, n, t;
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    /* Initialize variables */
    n = (datalen / 8) - 1;
    memcpy(a, data, 8);
    r = output;
    memcpy(r, data + 8, datalen - 8);

    /* Compute intermediate values */
    for (j = 5; j <= 5; j--)
    {
        r = output + (n - 1) * 8;
        for (i = n; i >= 1; i--)
        {
            t = n * j + i;
            memcpy(b, a, 8);
            b[7] ^= t;
            memcpy(b + 8, r, 8);

            AES_ECB_decrypt(&ctx, b);
            memcpy(a, b, 8);
            memcpy(r, b + 8, 8);
            r -= 8;
        }
    }
    return datalen - 8;
}

/* HMAC algorithm */
// https://en.wikipedia.org/wiki/Hash-based_message_authentication_code
// The hash function does not change the value of the input parameter, so you can safely remove the const modifier
uint8_t hmac(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, HashFunction hash, uint8_t blocksize, uint8_t *output, uint8_t outputsize)
{
    uint8_t i, *p;
    uint8_t *obuf = (uint8_t *)malloc(blocksize + outputsize); // Store the converted key in the first blocksize byte
    if (obuf == NULL)
        return 0; 

    // Keys longer than blockSize are shortened by hashing them
    if (keylen > blocksize)
    {
        hash((uint8_t *)key, keylen, obuf); // Key becomes outputSize bytes long
        keylen = outputsize;
    }
    else
        memcpy(obuf, key, keylen);

    // Keys shorter than blockSize are padded to blockSize by padding with zeros on the right
    if (keylen < blocksize)
        memset(obuf + keylen, 0, blocksize - keylen); // pad key with zeros to make it blockSize bytes long

    // Inner padded key
    p = (uint8_t *)malloc(blocksize + msglen);
    if (p == NULL)
    {
        free(obuf);
        return 0; 
    }
    for (i = 0; i < blocksize; i++)
        p[i] = obuf[i] ^ 0x36;

    memcpy(p + blocksize, msg, msglen);
    hash(p, blocksize + msglen, obuf + blocksize);
    free(p);

    // Outer padded key
    for (i = 0; i < blocksize; i++)
        obuf[i] ^= 0x5c;

    hash(obuf, blocksize + outputsize, (uint8_t *)output);
    free(obuf);
    return 1; 
}

/* Using the md5 function provided by lwip to implement the HMAC_MD5 algorithm */
uint8_t hmac_md5(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, uint8_t output[HMAC_MD5_OUTPUTSIZE])
{
    return hmac(key, keylen, msg, msglen, (HashFunction)md5, HMAC_MD5_BLOCKSIZE, output, HMAC_MD5_OUTPUTSIZE);
}

/* The HMAC_SHA1 algorithm is implemented using the sha1 function provided by lwip */
uint8_t hmac_sha1(const void *key, uint16_t keylen, const void *msg, uint16_t msglen, uint8_t output[HMAC_SHA1_OUTPUTSIZE])
{
    return hmac(key, keylen, msg, msglen, (HashFunction)sha1, HMAC_SHA1_BLOCKSIZE, output, HMAC_SHA1_OUTPUTSIZE);
}

/* The PBKDF2_HMAC_SHA1 algorithm is implemented by using hmac_sha1 function */
// https://en.wikipedia.org/wiki/PBKDF2
uint8_t pbkdf2_hmac_sha1(const void *password, uint16_t pwdlen, const void *salt, uint16_t saltlen, uint32_t c, uint8_t *dk, uint16_t dklen)
{
    uint8_t curr, k, *p;
    uint8_t ret;
    uint8_t u[3][HMAC_SHA1_OUTPUTSIZE];
    uint16_t i;
    uint32_t j;

    p = (uint8_t *)malloc(saltlen + 4);
    if (p == NULL)
        return 0;
    memcpy(p, salt, saltlen);
    memset(p + saltlen, 0, 2);

    for (i = 1; dklen; i++)
    {
        // INT_32_BE(i)
        p[saltlen + 2] = i >> 8;
        p[saltlen + 3] = i & 0xff;

        ret = hmac_sha1(password, pwdlen, p, saltlen + 4, u[1]); // The resulting U1 is put into u[1]
        if (!ret)
        {
            free(p);
            return 0;
        }
        memcpy(u[2], u[1], HMAC_SHA1_OUTPUTSIZE); // F=U1 (u[2] is used to store F)
        for (j = 2; j <= c; j++)
        {
            // F^=Uj -> F^=PRF(password, Uj-1)
            ret = hmac_sha1(password, pwdlen, u[1], HMAC_SHA1_OUTPUTSIZE, u[0]); // According to uj-1 (in u[1]), calculate Uj and put it in u[0]
            if (!ret)
            {
                free(p);
                return 0;
            }
            for (k = 0; k < HMAC_SHA1_OUTPUTSIZE; k++)
            {
                u[2][k] ^= u[0][k];
                if (j != c)
                    u[1][k] = u[0][k]; // And by the way, I'm going to copy u[0] into u[1]
            }
        }

        // u[2] is the final result F, which is copied to the result buffer dk
        curr = (dklen < HMAC_SHA1_OUTPUTSIZE) ? dklen : HMAC_SHA1_OUTPUTSIZE;
        memcpy(dk, u[2], curr);
        dk += curr;
        dklen -= curr;
    }

    free(p);
    return 1;
}

/* Pseudorandom function (PRF-n) */
// http://etutorials.org/Networking/802.11+security.+wi-fi+protected+access+and+802.11i/Part+II+The+Design+of+Wi-Fi+Security/Chapter+10.+WPA+and+RSN+Key+Hierarchy/Computing+the+Temporal+Keys/
// The parameter n is n over 8 in PRF-n
uint8_t PRF(const void *k, uint16_t klen, const char *a, const void *b, uint16_t blen, void *output, uint8_t n)
{
    uint8_t alen = strlen(a);
    uint8_t buf[HMAC_SHA1_OUTPUTSIZE];
    uint8_t curr, i, *p, *q;

    p = (uint8_t *)malloc(alen + blen + 2);
    if (p == NULL)
        return 0;

    q = (uint8_t *)output;
    strcpy((char *)p, a); // application-specific text (including '\0')
    memcpy(p + alen + 1, b, blen); // special data
    for (i = 0; n; i++)
    {
        p[alen + blen + 1] = i; // single byte counter
        hmac_sha1(k, klen, p, alen + blen + 2, buf);

        curr = (n < HMAC_SHA1_OUTPUTSIZE) ? n : HMAC_SHA1_OUTPUTSIZE;
        memcpy(q, buf, curr);
        q += curr;
        n -= curr;
    }

    free(p);
    return 1;
}
