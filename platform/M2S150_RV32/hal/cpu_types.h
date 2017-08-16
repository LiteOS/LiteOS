/*******************************************************************************
 * (c) Copyright 2007-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * SVN $Revision: 9085 $
 * SVN $Date: 2017-04-28 14:29:14 +0530 (Fri, 28 Apr 2017) $
 */
#ifndef __CPU_TYPES_H
#define __CPU_TYPES_H   1

#include <stdint.h>

/*------------------------------------------------------------------------------
 */
typedef unsigned int size_t;

/*------------------------------------------------------------------------------
 * addr_t: address type.
 * Used to specify the address of peripherals present in the processor's memory
 * map.
 */
typedef unsigned int addr_t;

/*------------------------------------------------------------------------------
 * psr_t: processor state register.
 * Used by HAL_disable_interrupts() and HAL_restore_interrupts() to store the
 * processor's state between disabling and restoring interrupts.
 */
typedef unsigned int psr_t;

#endif  /* __CPU_TYPES_H */

