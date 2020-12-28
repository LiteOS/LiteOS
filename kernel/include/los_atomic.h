/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch32 Atomic HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

/**
 * @defgroup los_atomic Atomic
 * @ingroup kernel
 */

#ifndef _LOS_ATOMIC_H
#define _LOS_ATOMIC_H

#include "los_typedef.h"
#include "arch/atomic.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  los_atomic
 * @brief Atomic read.
 *
 * @par Description:
 * This API is used to implement the atomic read and return the value read from the input parameter v.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN] The pointer to the value to be read.
 *
 * @retval           #INT32 The result value read from the input parameter v.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Atomic64Read
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT32 LOS_AtomicRead(Atomic *v)
{
    return ArchAtomicRead(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic setting.
 *
 * @par Description:
 * This API is used to implement the atomic setting operation. It can set the value of
 * the input parameter setVal to the variable pointed by the pointer v.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN/OUT] The pointer to the variable to be set.
 * @param  setVal    [IN] The value used to set the first parameter *v.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Atomic64Set
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE VOID LOS_AtomicSet(Atomic *v, INT32 setVal)
{
    ArchAtomicSet(v, setVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic addition.
 *
 * @par Description:
 * This API is used to implement the atomic addition and return the sum.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>If the addition result is not in the range of representable values for 32-bit
 * signed integer, an integer overflow may occur in the return value.</li>
 * </ul>
 *
 * @param  v         [IN/OUT] The pointer to the augend.
 * @param  addVal    [IN] The addend.
 *
 * @retval           #INT32 The sum after the addition is performed. And the sum is
 *                          saved to the first parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Atomic64Add | LOS_AtomicSub
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE INT32 LOS_AtomicAdd(Atomic *v, INT32 addVal)
{
    return ArchAtomicAdd(v, addVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic subtraction.
 *
 * @par Description:
 * This API is used to implement the atomic subtraction and return the difference.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>If the subtraction result is not in the range of representable values for 32-bit
 * signed integer, an integer overflow may occur to the return value.</li>
 * </ul>
 *
 * @param  v         [IN/OUT] The pointer to the minuend.
 * @param  subVal    [IN] The subtrahend.
 *
 * @retval           #INT32 The difference after the subtraction is performed. And the
 *                          difference is saved to the first parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicAdd | LOS_Atomic64Sub
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE INT32 LOS_AtomicSub(Atomic *v, INT32 subVal)
{
    return ArchAtomicSub(v, subVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic self-addition.
 *
 * @par Description:
 * This API is used to implement the atomic self-addition. It can add 1 to the input parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT_MAX to avoid integer overflow after adding 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-addition. And the self-addition result
 *                         will be saved to this pointer.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicIncRet | LOS_Atomic64Inc | LOS_AtomicDec
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE VOID LOS_AtomicInc(Atomic *v)
{
    ArchAtomicInc(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic self-addition.
 *
 * @par Description:
 * This API is used to implement the atomic self-addition and return the self-addition result.
 * It can add 1 to the input parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT_MAX to avoid integer overflow after adding 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-addition.
 *
 * @retval        #INT32 The self-addition result after adding 1. And the result will be saved to
 *                       the input parameter *v.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicInc | LOS_Atomic64IncRet | LOS_AtomicDecRet
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE INT32 LOS_AtomicIncRet(Atomic *v)
{
    return ArchAtomicIncRet(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic self-decrement.
 *
 * @par Description:
 * This API is used to implement the atomic self-decrement. It can subtract 1 from the input
 * parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT_MIN to avoid overflow after subtracting 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-decrement. And the self-decrement
 *                         result will be saved to this pointer.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicDecRet | LOS_AtomicInc | LOS_Atomic64Dec
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE VOID LOS_AtomicDec(Atomic *v)
{
    ArchAtomicDec(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic self-decrement.
 *
 * @par Description:
 * This API is used to implement the atomic self-decrement and return the self-decrement result.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT_MIN to avoid overflow after subtracting 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-decrement.
 *
 * @retval        #INT32 The self-decrement result after subtracting 1. And the result will be
 *                       saved to the input parameter *v.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicDec | LOS_AtomicIncRet | LOS_Atomic64DecRet
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE INT32 LOS_AtomicDecRet(Atomic *v)
{
    return ArchAtomicDecRet(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 read.
 *
 * @par Description:
 * This API is used to implement the atomic64 read and return the value read from the input parameter v.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN] The pointer to the value to be read.
 *
 * @retval           #INT64 The result value read from the input parameter v.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicRead
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_Atomic64Read(const Atomic64 *v)
{
    return ArchAtomic64Read(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 setting.
 *
 * @par Description:
 * This API is used to implement the atomic64 setting operation. It can set the value of
 * the input parameter setVal to the variable pointed by the pointer v.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN/OUT] The pointer to the variable to be set.
 * @param  setVal    [IN] The value used to set the first parameter *v.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicSet
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE VOID LOS_Atomic64Set(Atomic64 *v, INT64 setVal)
{
    ArchAtomic64Set(v, setVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 addition.
 *
 * @par Description:
 * This API is used to implement the atomic64 addition and return the sum.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>If the addition result is not in the range of representable values for 64-bit
 * signed integer, an integer overflow may occurs in the return value.</li>
 * </ul>
 *
 * @param  v         [IN/OUT] The pointer to the augend.
 * @param  addVal    [IN] The addend.
 *
 * @retval           #INT64 The sum after the addition is performed. And the sum is
 *                          saved to the first parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicAdd | LOS_Atomic64Sub
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_Atomic64Add(Atomic64 *v, INT64 addVal)
{
    return ArchAtomic64Add(v, addVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 subtraction.
 *
 * @par Description:
 * This API is used to implement the atomic64 subtraction and return the difference.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>If the subtraction result is not in the range of representable values for 64-bit
 * signed integer, an integer overflow may occurs in the return value.</li>
 * </ul>
 *
 * @param  v         [IN/OUT] The pointer to the minuend.
 * @param  subVal    [IN] The subtrahend.
 *
 * @retval           #INT64 The difference after the subtraction is performed. And the
 *                          result is saved to the first parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicSub | LOS_Atomic64Add
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_Atomic64Sub(Atomic64 *v, INT64 subVal)
{
    return ArchAtomic64Sub(v, subVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 self-addition.
 *
 * @par Description:
 * This API is used to implement the atomic64 self-addition. It can add 1 to the input parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT64_MAX to avoid integer overflow after adding 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-addition. And the self-addition
 *                         result is saved to the input parameter.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicInc | LOS_Atomic64IncRet | LOS_Atomic64Dec
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE VOID LOS_Atomic64Inc(Atomic64 *v)
{
    ArchAtomic64Inc(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 self-addition.
 *
 * @par Description:
 * This API is used to implement the atomic64 self-addition and return the self-addition result.
 * It can add 1 to the input parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT64_MAX to avoid integer overflow after adding 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-addition.
 *
 * @retval        #INT64 The self-addition result after adding 1. And the result is saved to the
 *                       input parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_Atomic64Inc | LOS_AtomicIncRet | LOS_Atomic64DecRet
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_Atomic64IncRet(Atomic64 *v)
{
    return ArchAtomic64IncRet(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 self-decrement.
 *
 * @par Description:
 * This API is used to implement the atomic64 self-decrement. It can subtract 1 from the input
 * parameter *v.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT64_MIN to avoid overflow after subtracting 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-decrement. And the self-decrement
 *                         result is saved to the input parameter.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicDec | LOS_Atomic64DecRet | LOS_Atomic64Inc
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE VOID LOS_Atomic64Dec(Atomic64 *v)
{
    ArchAtomic64Dec(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic64 self-decrement.
 *
 * @par Description:
 * This API is used to implement the atomic64 self-decrement and return the self-decrement result.
 * @attention
 * <ul>
 * <li>The pointer v must not be NULL.</li>
 * <li>The value which v points to must not be INT64_MIN to avoid overflow after subtracting 1.</li>
 * </ul>
 *
 * @param  v      [IN/OUT] The pointer to the value need to self-decrement.
 *
 * @retval        #INT64 The self-decrement result after subtracting 1. And the result is saved to
 *                       the input parameter.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicDecRet | LOS_Atomic64Dec | LOS_Atomic64IncRet
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_Atomic64DecRet(Atomic64 *v)
{
    return ArchAtomic64DecRet(v);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic change 32-bit variable.
 *
 * @par Description:
 * This API is used to implement the atomic change 32-bit variable *v to the value of second parameter.
 * And return the original value of the first parameter *v before change.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN/OUT] The pointer to the variable need to be changed.
 * @param  val       [IN] The value used to change the value of first parameter *v.
 *
 * @retval           #INT32 The original value of the first parameter *v before change.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicXchg64bits | LOS_AtomicCmpXchg32bits
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE INT32 LOS_AtomicXchg32bits(Atomic *v, INT32 val)
{
    return ArchAtomicXchg32bits(v, val);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic change 64-bit variable.
 *
 * @par Description:
 * This API is used to implement the atomic change 64-bit variable *v to the value of second parameter.
 * And return the original value of the first parameter *v before change.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v         [IN/OUT] The pointer to the variable need to be changed.
 * @param  val       [IN] The value used to change the value of first parameter *v.
 *
 * @retval           #INT64 The original value of the first parameter *v before change.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicXchg32bits | LOS_AtomicCmpXchg64bits
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE INT64 LOS_AtomicXchg64bits(Atomic64 *v, INT64 val)
{
    return ArchAtomicXchg64bits(v, val);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic change 32-bit variable with comparison.
 *
 * @par Description:
 * This API is used to implement the atomic change 32-bit variable *v to the value of second parameter
 * while the value of *v (the first parameter) is equal to oldVal (the third parameter). If the value
 * of variable *v is not equal to oldVal, return true. Otherwise return false.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v           [IN/OUT] The pointer to the variable need to be changed.
 * @param  val         [IN] The value used to change the value of first parameter *v.
 * @param  oldVal      [IN] The value used to compare with the first parameter *v.
 *
 * @retval #TRUE  The original value of the first parameter is not equal to oldVal.
 * @retval #FALSE The original value of the first parameter is equal to oldVal.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicCmpXchg64bits | LOS_AtomicXchg32bits
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE BOOL LOS_AtomicCmpXchg32bits(Atomic *v, INT32 val, INT32 oldVal)
{
    return ArchAtomicCmpXchg32bits(v, val, oldVal);
}

/**
 * @ingroup  los_atomic
 * @brief Atomic change 64-bit variable with comparison.
 *
 * @par Description:
 * This API is used to implement the atomic change 64-bit variable *v to the value of second parameter
 * while the value of *v (the first parameter) is equal to oldVal (the third parameter). If the value
 * of variable *v is not equal to oldVal, return true. Otherwise return false.
 * @attention
 * The pointer v must not be NULL.
 *
 * @param  v           [IN/OUT] The pointer to the variable need to be changed.
 * @param  val         [IN] The value used to change the value of first parameter *v.
 * @param  oldVal      [IN] The value used to compare with the first parameter *v.
 *
 * @retval #TRUE  The original value of the first parameter is not equal to oldVal.
 * @retval #FALSE The original value of the first parameter is equal to oldVal.
 * @par Dependency:
 * <ul><li>los_atomic.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_AtomicCmpXchg32bits | LOS_AtomicXchg64bits
 * @since Huawei LiteOS V200R003C00
 */
STATIC INLINE BOOL LOS_AtomicCmpXchg64bits(Atomic64 *v, INT64 val, INT64 oldVal)
{
    return ArchAtomicCmpXchg64bits(v, val, oldVal);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ATOMIC_H */
