/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Tables
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef _LOS_TABLES_H
#define _LOS_TABLES_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef LOS_HAL_TABLE_WOW_BEGIN
#define LOS_HAL_TABLE_WOW_BEGIN(label, name)                                     \
    __asm__(".section \".liteos.table." X_STRING(name) ".wow.begin\",\"aw\"\n"   \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                       \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"              \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                        \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                                \
            ".previous\n"                                                        \
            )
#endif

#ifndef LOS_HAL_TABLE_WOW_END
#define LOS_HAL_TABLE_WOW_END(label, name)                                       \
    __asm__(".section \".liteos.table." X_STRING(name) ".wow.finish\",\"aw\"\n"  \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                       \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"              \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                        \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                                \
            ".previous\n"                                                        \
            )
#endif

#ifndef LOS_HAL_TABLE_SCATTER_BEGIN
#define LOS_HAL_TABLE_SCATTER_BEGIN(label, name)                                    \
    __asm__(".section \".liteos.table." X_STRING(name) ".scatter.begin\",\"aw\"\n"  \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                          \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"                 \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                           \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                                   \
            ".previous\n"                                                           \
            )
#endif

#ifndef LOS_HAL_TABLE_SCATTER_END
#define LOS_HAL_TABLE_SCATTER_END(label, name)                                      \
    __asm__(".section \".liteos.table." X_STRING(name) ".scatter.finish\",\"aw\"\n" \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                          \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"                 \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                           \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                                   \
            ".previous\n"                                                           \
            )
#endif

#ifndef LOS_HAL_TABLE_BEGIN
#define LOS_HAL_TABLE_BEGIN(label, name)                                     \
    __asm__(".section \".liteos.table." X_STRING(name) ".begin\",\"aw\"\n"   \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                   \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"          \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                    \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                            \
            ".previous\n"                                                    \
            )
#endif

#ifndef LOS_HAL_TABLE_END
#define LOS_HAL_TABLE_END(label, name)                                       \
    __asm__(".section \".liteos.table." X_STRING(name) ".finish\",\"aw\"\n"  \
            ".globl " X_STRING(LOS_LABEL_DEFN(label)) "\n"                   \
            ".type    " X_STRING(LOS_LABEL_DEFN(label)) ",object\n"          \
            ".p2align " X_STRING(LOSARC_P2ALIGNMENT) "\n"                    \
            X_STRING(LOS_LABEL_DEFN(label)) ":\n"                            \
            ".previous\n"                                                    \
            )
#endif

/* This macro must be applied to any types whose objects are to be placed in tables */
#ifndef LOS_HAL_TABLE_TYPE
#define LOS_HAL_TABLE_TYPE LOSBLD_ATTRIB_ALIGN(LOSARC_ALIGNMENT)
#endif

#ifndef LOS_HAL_TABLE_EXTRA
#define LOS_HAL_TABLE_EXTRA(name) \
    LOSBLD_ATTRIB_SECTION(".liteos.table." X_STRING(name) ".extra")
#endif

#ifndef LOS_HAL_TABLE_WOW_ENTRY
#define LOS_HAL_TABLE_WOW_ENTRY(name)                                  \
    LOSBLD_ATTRIB_SECTION(".liteos.table." X_STRING(name) ".wow.data") \
    LOSBLD_ATTRIB_USED
#endif

#ifndef LOS_HAL_TABLE_SCATTER_ENTRY
#define LOS_HAL_TABLE_SCATTER_ENTRY(name)                                  \
    LOSBLD_ATTRIB_SECTION(".liteos.table." X_STRING(name) ".scatter.data") \
    LOSBLD_ATTRIB_USED
#endif

#ifndef LOS_HAL_TABLE_ENTRY
#define LOS_HAL_TABLE_ENTRY(name)                                  \
    LOSBLD_ATTRIB_SECTION(".liteos.table." X_STRING(name) ".data") \
    LOSBLD_ATTRIB_USED
#endif

#ifndef LOS_HAL_TABLE_QUALIFIED_ENTRY
#define LOS_HAL_TABLE_QUALIFIED_ENTRY(name, _qual)                                  \
    LOSBLD_ATTRIB_SECTION(".liteos.table." X_STRING(name) ".data." X_STRING(_qual)) \
    LOSBLD_ATTRIB_USED
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TABLES_H */
