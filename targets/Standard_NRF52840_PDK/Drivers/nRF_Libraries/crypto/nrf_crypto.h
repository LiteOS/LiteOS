/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef NRF_CRYPTO_H__
#define NRF_CRYPTO_H__

#include <stdint.h>
#include "nrf_svci.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup nrf_crypto Cryptography library
 * @ingroup app_common
 * @{
 *
 * @brief   Cryptography library (nrf_crypto).
 *
 * @details The cryptography library provides functions to compute keys, shared secrets, and hashes,
 *          and to sign and verify data using digital signatures.
 */

#define NRF_CRYPTO_SVCI_BASE 0                               //!< Base SVCI number for the nrf_crypto module.

/** @brief SVCIs for the nrf_crypto module. */
enum NRF_CRYPTO_SVCIS
{
  NRF_CRYPTO_SVCI_INIT  = NRF_CRYPTO_SVCI_BASE,              /**< Initialize the nrf_crypto module. */
  NRF_CRYPTO_SVCI_PUBLIC_KEY_COMPUTE,                        /**< Compute a public key from a private key. */
  NRF_CRYPTO_SVCI_SHARED_SECRET_COMPUTE,                     /**< Compute a shared secret from a key pair. */
  NRF_CRYPTO_SVCI_SIGN,                                      /**< Compute a signature from a private key and a hash. */
  NRF_CRYPTO_SVCI_VERIFY,                                    /**< Verify a signature with a public key and a hash. */
  NRF_CRYPTO_SVCI_HASH_COMPUTE,                              /**< Compute a digest from data. */
};

/**@brief Cryptographic key.
 *
 * @details This structure is a generic container for a cryptographic key, shared secret,
 *          hash, or signature. The data is contained in raw bytes in little-endian format.
 *          If more than one array is present in the data (for example, multiple point
 *          coordinates), those must be placed consecutively in memory.
 *
 * For ECC, the key length depends on the length of the ECC curve:
 * Key type      | Key length
 * --------------|---------------------
 * Public key    | 2 * curve length
 * Private key   | 1 * curve length
 * Shared secret | 2 * curve length
 * Signature     | 2 * curve length
 */
typedef struct
{
    uint8_t    *p_le_data;       /**< Pointer to the key data in little-endian format. */
    uint32_t   len;              /**< Length of the key data in bytes. */
} nrf_crypto_key_t;


/**@defgroup NRF_CRYPTO_CURVES Cryptographic curves
 * @brief Cryptographic curves that are available to the application. Currently, the nrf_crypto library supports only NIST p-256.
 * @{ */
#define NRF_CRYPTO_CURVE_INVALID        0x00        //!< Invalid curve.
#define NRF_CRYPTO_CURVE_SECP160R1      0x01        //!< NIST 160-bit.
#define NRF_CRYPTO_CURVE_SECP192R1      0x02        //!< NIST 192-bit.
#define NRF_CRYPTO_CURVE_SECP224R1      0x03        //!< NIST 224-bit.
#define NRF_CRYPTO_CURVE_SECP256R1      0x04        //!< NIST 256-bit.
#define NRF_CRYPTO_CURVE_SECP384R1      0x05        //!< NIST 384-bit.
#define NRF_CRYPTO_CURVE_SECP521R1      0x06        //!< NIST 521-bit.
#define NRF_CRYPTO_CURVE_SECP192K1      0x07        //!< Koblitz 192-bit.
#define NRF_CRYPTO_CURVE_SECP224K1      0x08        //!< Koblitz 224-bit.
#define NRF_CRYPTO_CURVE_SECP256K1      0x09        //!< Koblitz 256-bit.
#define NRF_CRYPTO_CURVE_CURVE25519     0x0A        //!< Curve 25519.
/** @}  */

/**@defgroup NRF_CRYPTO_HASH_ALGS Hashing algorithms
 * @brief Hashing algorithms that are available to the application. Currently, the nrf_crypto library supports only SHA-256.
 * @{ */
#define NRF_CRYPTO_HASH_ALG_INVALID     0x00        //!< Invalid hashing algorithm.
#define NRF_CRYPTO_HASH_ALG_MD5         0x01        //!< MD5.
#define NRF_CRYPTO_HASH_ALG_SHA0        0x02        //!< SHA-0.
#define NRF_CRYPTO_HASH_ALG_SHA1        0x03        //!< SHA-1.
#define NRF_CRYPTO_HASH_ALG_SHA224      0x04        //!< SHA-224 (SHA-2).
#define NRF_CRYPTO_HASH_ALG_SHA256      0x05        //!< SHA-256 (SHA-2).
#define NRF_CRYPTO_HASH_ALG_SHA384      0x06        //!< SHA-384 (SHA-2).
#define NRF_CRYPTO_HASH_ALG_SHA512      0x07        //!< SHA-512 (SHA-2).
/** @}  */


/**@brief Function for initializing the cryptography library.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
  uint32_t nrf_crypto_init(void);
#else
SVCI(NRF_CRYPTO_SVCI_INIT, uint32_t, nrf_crypto_init);
#endif

/**@brief Function for computing a public key from a private key.
 *
 * @param[in]   curve   Elliptic curve to use (see @ref NRF_CRYPTO_CURVES).
 * @param[in]   p_sk    Private key (see @ref nrf_crypto_key_t).
 * @param[out]  p_pk    Public key (see @ref nrf_crypto_key_t).
 *
 * @retval  NRF_SUCCESS If the public key was computed successfully.
 * @retval  NRF_ERROR_NOT_SUPPORTED If the selected curve is not supported.
 * @retval  NRF_ERROR_INVALID_ADDR If any of the provided pointers is invalid.
 * @retval  NRF_ERROR_INVALID_LENGTH If the length of the provided private key is invalid or the public key is bigger than the size of the provided buffer.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
 uint32_t nrf_crypto_public_key_compute(uint32_t curve, nrf_crypto_key_t const *p_sk, nrf_crypto_key_t *p_pk);
#else
SVCI(NRF_CRYPTO_SVCI_PUBLIC_KEY_COMPUTE, uint32_t,\
       nrf_crypto_public_key_compute, uint32_t, curve, \
                                     nrf_crypto_key_t const *, p_sk, \
                                     nrf_crypto_key_t *, p_pk);
#endif


/**@brief Function for computing a shared secret from a key pair.
 *
 * @param[in]   curve   Elliptic curve to use (see @ref NRF_CRYPTO_CURVES).
 * @param[in]   p_sk    Private key (see @ref nrf_crypto_key_t).
 * @param[in]   p_pk    Public key (see @ref nrf_crypto_key_t).
 * @param[out]  p_ss    Shared secret (see @ref nrf_crypto_key_t).
 *
 * @retval  NRF_SUCCESS If the shared secret was computed successfully.
 * @retval  NRF_ERROR_NOT_SUPPORTED If the selected curve is not supported.
 * @retval  NRF_ERROR_INVALID_ADDR If any of the provided pointers is invalid.
 * @retval  NRF_ERROR_INVALID_LENGTH If the length of any of the provided keys is invalid or the shared secret is bigger than the size of the provided buffer.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
  uint32_t nrf_crypto_shared_secret_compute(uint32_t curve, nrf_crypto_key_t const *p_sk, nrf_crypto_key_t const *p_pk, nrf_crypto_key_t *p_ss);
#else
SVCI(NRF_CRYPTO_SVCI_SHARED_SECRET_COMPUTE, uint32_t, \
        nrf_crypto_shared_secret_compute, uint32_t, curve, \
                                         nrf_crypto_key_t const *, p_sk, \
                                         nrf_crypto_key_t const *, p_pk, \
                                         nrf_crypto_key_t *, p_ss);
#endif

/**@brief Function for signing a hash or a digest using a private key.
 *
 * @param[in]   curve   Elliptic curve to use (see @ref NRF_CRYPTO_CURVES).
 * @param[in]   p_sk    Private key (see @ref nrf_crypto_key_t).
 * @param[in]   p_hash  Hash or digest to sign (see @ref nrf_crypto_key_t).
 * @param[out]  p_sig   Signature (see @ref nrf_crypto_key_t).
 *
 * @retval  NRF_SUCCESS If the signature was created successfully.
 * @retval  NRF_ERROR_NOT_SUPPORTED If the selected curve is not supported.
 * @retval  NRF_ERROR_INVALID_ADDR If any of the provided pointers is invalid.
 * @retval  NRF_ERROR_INVALID_LENGTH If the length of the provided private key or hash is invalid or the signature is bigger than the size of the provided buffer.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
  uint32_t nrf_crypto_sign(uint32_t curve, nrf_crypto_key_t const *p_sk, nrf_crypto_key_t const *p_hash, nrf_crypto_key_t *p_sig);
#else
SVCI(NRF_CRYPTO_SVCI_SIGN, uint32_t,	\
        nrf_crypto_sign, uint32_t, curve, \
                        nrf_crypto_key_t const *, p_sk,	\
                        nrf_crypto_key_t const *, p_hash, \
                        nrf_crypto_key_t *, p_sig);
#endif

/**@brief Function for verifying the signature of a hash or a digest using a public key.
 *
 * @param[in]   curve   Elliptic curve to use (see @ref NRF_CRYPTO_CURVES).
 * @param[in]   p_pk    Public key (see @ref nrf_crypto_key_t).
 * @param[in]   p_hash  Hash or digest to compare to (see @ref nrf_crypto_key_t).
 * @param[in]   p_sig   Signature (see @ref nrf_crypto_key_t).
 *
 * @retval  NRF_SUCCESS If the signature was verified and is valid.
 * @retval  NRF_ERROR_INVALID_DATA If the signature did not match the provided hash or digest.
 * @retval  NRF_ERROR_NOT_SUPPORTED If the selected curve is not supported.
 * @retval  NRF_ERROR_INVALID_ADDR If any of the provided pointers is invalid.
 * @retval  NRF_ERROR_INVALID_LENGTH If the length of the provided public key, hash, or signature is invalid.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
uint32_t nrf_crypto_verify(uint32_t curve, nrf_crypto_key_t const *p_pk, nrf_crypto_key_t const *p_hash, nrf_crypto_key_t const *p_sig);
#else
SVCI(NRF_CRYPTO_SVCI_VERIFY, uint32_t, \
        nrf_crypto_verify, uint32_t, curve, \
                          nrf_crypto_key_t const *, p_pk, \
                          nrf_crypto_key_t const *, p_hash, \
                          nrf_crypto_key_t const *, p_sig);
#endif

/**@brief Function for computing a hash or a digest from arbitrary data.
 *
 * @param[in]   hash_alg     Hashing algorithm to use (see @ref NRF_CRYPTO_HASH_ALGS).
 * @param[in]   p_data       Data to be hashed.
 * @param[in]   len          Length of the data to be hashed.
 * @param[out]  p_hash       Hash or digest (see @ref nrf_crypto_key_t).
 *
 * @retval  NRF_SUCCESS If the hash was computed successfully.
 * @retval  NRF_ERROR_NOT_SUPPORTED If the selected hashing algorithm is not supported.
 * @retval  NRF_ERROR_INVALID_ADDR If any of the provided pointers is invalid.
 * @retval  NRF_ERROR_INVALID_LENGTH If the hash is bigger than the size of the provided buffer.
 */
#ifdef SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION
uint32_t nrf_crypto_hash_compute(uint32_t hash_alg, uint8_t const *p_data, uint32_t len, nrf_crypto_key_t *p_hash);

#else
SVCI(NRF_CRYPTO_SVCI_HASH_COMPUTE, uint32_t, \
        nrf_crypto_hash_compute, uint32_t, hash_alg, \
                                uint8_t const *, p_data, \
                                uint32_t, len, \
                                nrf_crypto_key_t *, p_hash);
#endif

/** @} */
#ifdef __cplusplus
}
#endif

#endif // NRF_CRYPTO_H__

