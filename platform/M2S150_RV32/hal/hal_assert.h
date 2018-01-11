/*******************************************************************************
 * (c) Copyright 2008-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * SVN $Revision: 9085 $
 * SVN $Date: 2017-04-28 14:29:14 +0530 (Fri, 28 Apr 2017) $
 */
#ifndef HAL_ASSERT_HEADER
#define HAL_ASSERT_HEADER

#define NDEBUG 1

#if defined(NDEBUG)
/***************************************************************************//**
 * HAL_ASSERT() is defined out when the NDEBUG symbol is used.
 ******************************************************************************/
#define HAL_ASSERT(CHECK)

#else
/***************************************************************************//**
 * Default behaviour for HAL_ASSERT() macro:
 *------------------------------------------------------------------------------
 * Using the HAL_ASSERT() macro is the same as directly using the SmartFusion2
 * CMSIS ASSERT() macro. The behaviour is toolchain specific and project
 * setting specific.
 ******************************************************************************/
#define HAL_ASSERT(CHECK)     ASSERT(CHECK);

#endif  /* NDEBUG */

#endif  /* HAL_ASSERT_HEADER */

