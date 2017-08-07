/***************************************************************************//**
 * (c) Copyright 2007-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * Hardware registers access functions.
 * The implementation of these function is platform and toolchain specific.
 * The functions declared here are implemented using assembler as part of the 
 * processor/toolchain specific HAL.
 * 
 * SVN $Revision: 9085 $
 * SVN $Date: 2017-04-28 14:29:14 +0530 (Fri, 28 Apr 2017) $
 */
#ifndef HW_REG_ACCESS
#define HW_REG_ACCESS

#include "cpu_types.h"

/***************************************************************************//**
 * HW_set_32bit_reg is used to write the content of a 32 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  write.
 * @param value     Value to be written into the peripheral register.
 */
void
HW_set_32bit_reg
(
    addr_t reg_addr,
    uint32_t value
)
{

    volatile uint32_t * foo = (uint32_t * ) reg_addr;
    *foo = value;
}

/***************************************************************************//**
 * HW_get_32bit_reg is used to read the content of a 32 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @return          32 bits value read from the peripheral register.
 */
uint32_t
HW_get_32bit_reg
(
    addr_t reg_addr
)
{
    volatile uint32_t * foo = (uint32_t *) reg_addr;
    return *foo;
}

/***************************************************************************//**
 * HW_set_32bit_reg_field is used to set the content of a field in a 32 bits 
 * wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  be written.
 * @param shift     Bit offset of the register field to be read within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 * @param value     Value to be written in the specified field.
 */
void
HW_set_32bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint32_t mask,
    uint32_t value
)
{
    volatile uint32_t * foo = (uint32_t *) reg_addr;
    uint32_t old_value = *foo;
    uint32_t new_value = (value << shift);
    *foo = (new_value & mask) | (old_value & ~mask);
}

/***************************************************************************//**
 * HW_get_32bit_reg_field is used to read the content of a field out of a 
 * 32 bits wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @param shift     Bit offset of the register field to be written within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 *
 * @return          32 bits value containing the register field value specified
 *                  as parameter.
 */
uint32_t 
HW_get_32bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint32_t mask
)
{
    volatile uint32_t * foo = (uint32_t * ) reg_addr;
    uint32_t value = ((*foo) & mask) >> shift;
    return (value);
}

/***************************************************************************//**
 * HW_set_16bit_reg is used to write the content of a 16 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  write.
 * @param value     Value to be written into the peripheral register.
 */
void
HW_set_16bit_reg
(
    addr_t reg_addr,
    uint_fast16_t value
)
{
    volatile uint16_t * foo = (uint16_t * ) reg_addr;
    *foo = value;
}

/***************************************************************************//**
 * HW_get_16bit_reg is used to read the content of a 16 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @return          16 bits value read from the peripheral register.
 */
uint16_t
HW_get_16bit_reg
(
    addr_t reg_addr
)
{
    volatile uint16_t * foo = (uint16_t*) reg_addr;
    return *foo;
}

/***************************************************************************//**
 * HW_set_16bit_reg_field is used to set the content of a field in a 16 bits 
 * wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  be written.
 * @param shift     Bit offset of the register field to be read within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 * @param value     Value to be written in the specified field.
 */
void HW_set_16bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint_fast16_t mask,
    uint_fast16_t value
)
{
    volatile uint16_t * foo = (uint16_t *) reg_addr;
    uint16_t old_value = *foo;
    uint16_t new_value = (value << shift);
    *foo = (new_value & mask) | (old_value & ~mask);
}

/***************************************************************************//**
 * HW_get_16bit_reg_field is used to read the content of a field from a 
 * 16 bits wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @param shift     Bit offset of the register field to be written within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 *
 * @return          16 bits value containing the register field value specified
 *                  as parameter.
 */
uint16_t HW_get_16bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint_fast16_t mask
)
{
    volatile uint16_t * foo = (uint16_t * ) reg_addr;
    uint16_t value = ((*foo) & mask) >> shift;
    return (value);
}

/***************************************************************************//**
 * HW_set_8bit_reg is used to write the content of a 8 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  write.
 * @param value     Value to be written into the peripheral register.
 */
void
HW_set_8bit_reg
(
    addr_t reg_addr,
    uint_fast8_t value
)
{
    volatile uint8_t * foo = (uint8_t*) reg_addr;
    *foo = value;
}

/***************************************************************************//**
 * HW_get_8bit_reg is used to read the content of a 8 bits wide peripheral
 * register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @return          8 bits value read from the peripheral register.
 */
uint8_t
HW_get_8bit_reg
(
    addr_t reg_addr
)
{
    volatile uint8_t * foo = (uint8_t*) reg_addr;
    return *foo;
}

/***************************************************************************//**
 * HW_set_8bit_reg_field is used to set the content of a field in a 8 bits 
 * wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  be written.
 * @param shift     Bit offset of the register field to be read within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 * @param value     Value to be written in the specified field.
 */
void HW_set_8bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint_fast8_t mask,
    uint_fast8_t value
)
{
    volatile uint8_t * foo = (uint8_t *) reg_addr;
    uint8_t old_value = *foo;
    uint8_t new_value = (value << shift);
    *foo = (new_value & mask) | (old_value & ~mask);
}

/***************************************************************************//**
 * HW_get_8bit_reg_field is used to read the content of a field from a 
 * 8 bits wide peripheral register.
 * 
 * @param reg_addr  Address in the processor's memory map of the register to
 *                  read.
 * @param shift     Bit offset of the register field to be written within the 
 *                  register.
 * @param mask      Bit mask to be applied to the raw register value to filter
 *                  out the other register fields values.
 *
 * @return          8 bits value containing the register field value specified
 *                  as parameter.
 */
uint8_t
HW_get_8bit_reg_field
(
    addr_t reg_addr,
    int_fast8_t shift,
    uint_fast8_t mask
)
{
    volatile uint8_t * foo = (uint8_t * ) reg_addr;
    uint8_t value = ((*foo) & mask) >> shift;
    return (value);
}

#endif /* HW_REG_ACCESS */
