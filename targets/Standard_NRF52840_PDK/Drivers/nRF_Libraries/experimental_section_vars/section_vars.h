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


#ifndef SECTION_VARS_H__
#define SECTION_VARS_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup section_vars Section variables
 * @ingroup app_common
 * @{
 *
 * @brief Section variables.
 */


#if defined(__ICCARM__)
// Enable IAR language extensions
#pragma language=extended
#endif


// Macro to delay macro expansion.
#define NRF_PRAGMA(x)                                       _Pragma(#x)


//lint -save -e27 Illegal character (0x24)


/**@brief   Macro to obtain the symbol marking the beginning of a given section.
 *
 * @details The symbol that this macro resolves to is used to obtain a section start address.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         section_name ## $$Base

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __start_ ## section_name

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __section_begin(#section_name)

#endif


/**@brief   Macro to obtain the symbol marking the end of a given section.
 *
 * @details The symbol that this macro resolves to is used to obtain a section stop address.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           section_name ## $$Limit

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __stop_ ## section_name

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __section_end(#section_name)

#endif


//lint -restore


/**@brief   Macro for retrieving the length of a given section, in bytes.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined(__GNUC__)

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined(__ICCARM__)

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)NRF_SECTION_VARS_START_SYMBOL(section_name))

#endif


/**@brief   Macro to obtain the address of the beginning of a section.
 *
 * param[in]    section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)iar_ ## section_name ## _start

#endif


/**@brief    Macro to obtain the address of end of a section.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)iar_ ## section_name ## _end

#endif


//lint -save -e19 -esym(526, fs_dataBase) -esym(526, fs_dataLimit) -esym(526, dfu_transBase) -esym(526, dfu_transLimit)

/**@brief   Macro to create a section to register variables in.
 *
 * @param[in]   data_type       The data type of the variables to be registered in the section.
 * @param[in]   section_name    Name of the section.
 *
 * @warning The data type must be word aligned to prevent padding.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_CREATE_SECTION(section_name, data_type)    \
    extern data_type * NRF_SECTION_VARS_START_SYMBOL(section_name); \
    extern void      * NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_CREATE_SECTION(section_name, data_type)    \
    extern data_type * NRF_SECTION_VARS_START_SYMBOL(section_name); \
    extern void      * NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__ICCARM__)

// No symbol registration required for IAR.
#define NRF_SECTION_VARS_CREATE_SECTION(section_name, data_type)                    \
    NRF_PRAGMA(section = #section_name);                                            \
    extern void * iar_ ## section_name ## _start = __section_begin(#section_name);  \
    extern void * iar_ ## section_name ## _end   = __section_end(#section_name)

#endif

//lint -restore


/**@brief   Macro to declare a variable and register it in a section.
 *
 * @details Declares a variable and registers it in a named section. This macro ensures that the
 *          variable is not stripped away when using optimizations.
 *
 * @note The order with which variables are placed in a section is dependant on the order with
 *       which the linker encouters the variables during linking.
 *
 * @param[in]   section_name    Name of the section.
 * @param[in]   section_var     The variable to register in the given section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_REGISTER_VAR(section_name, section_var) \
    static section_var __attribute__ ((section(#section_name))) __attribute__((used))

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_REGISTER_VAR(section_name, section_var) \
    static section_var __attribute__ ((section("."#section_name))) __attribute__((used))

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_REGISTER_VAR(section_name, section_var) \
    __root section_var @ #section_name

#endif


/**@brief   Macro to retrieve a variable from a section.
 *
 * @warning     The stored symbol can only be resolved using this macro if the
 *              type of the data is word aligned. The operation of acquiring
 *              the stored symbol relies on sizeof of the stored type, no
 *              padding can exist in the named section in between individual
 *              stored items or this macro will fail.
 *
 * @param[in]   i               Index of the variable in section.
 * @param[in]   data_type       Data type of the variable.
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_GET(i, data_type, section_name) \
    (data_type*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(data_type))

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_GET(i, data_type, section_name) \
    (data_type*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(data_type))

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_GET(i, data_type, section_name) \
    (data_type*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(data_type))

#endif


/**@brief   Macro to get number of variables registered in a section.
 *
 * @param[in]   data_type       Data type of the variables in the section.
 * @param[in]   section_name    Name of the section.
 */
#define NRF_SECTION_VARS_COUNT(data_type, section_name) \
    NRF_SECTION_VARS_LENGTH(section_name) / sizeof(data_type)

/** @} */


#ifdef __cplusplus
}
#endif

#endif // SECTION_VARS_H__
