/* ----------------------------------------------------------------------------
 *         SAM Software Package License 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2014, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------- */
/*         Headers                                                           */
/*-------------------------------------------------------------------------- */
#include <chip.h>
#include "mpu.h"

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/
#define ARM_MODE_USR            0x10

#define PRIVILEGE_MODE 0
#define USER_MODE      1

#define MPU_DEFAULT_ITCM_REGION                 ( 1 )
#define MPU_DEFAULT_IFLASH_REGION               ( 2 )
#define MPU_DEFAULT_DTCM_REGION                 ( 3 )
#define MPU_DEFAULT_SRAM_REGION_1               ( 4 )
#define MPU_DEFAULT_SRAM_REGION_2               ( 5 )
#define MPU_PERIPHERALS_REGION                  ( 6 )
#define MPU_EXT_EBI_REGION                      ( 7 )
#define MPU_DEFAULT_SDRAM_REGION                ( 8 )
#define MPU_QSPIMEM_REGION                      ( 9 )
#define MPU_USBHSRAM_REGION                     ( 10 )
#define MPU_NOCACHE_SRAM_REGION                 ( 11 )
 
#define MPU_REGION_VALID                        ( 0x10 )
#define MPU_REGION_ENABLE                       ( 0x01 )
#define MPU_REGION_DISABLE                      ( 0x0 )

#define MPU_ENABLE                              ( 0x1 << MPU_CTRL_ENABLE_Pos)
#define MPU_HFNMIENA                            ( 0x1 << MPU_CTRL_HFNMIENA_Pos )   
#define MPU_PRIVDEFENA                          ( 0x1 << MPU_CTRL_PRIVDEFENA_Pos )

#define MPU_REGION_BUFFERABLE                   ( 0x01 << MPU_RASR_B_Pos )
#define MPU_REGION_CACHEABLE                    ( 0x01 << MPU_RASR_C_Pos )
#define MPU_REGION_SHAREABLE                    ( 0x01 << MPU_RASR_S_Pos )

#define MPU_REGION_EXECUTE_NEVER                ( 0x01 << MPU_RASR_XN_Pos )

#define MPU_AP_NO_ACCESS                        ( 0x00 << MPU_RASR_AP_Pos )
#define MPU_AP_PRIVILEGED_READ_WRITE            ( 0x01 << MPU_RASR_AP_Pos )
#define MPU_AP_UNPRIVILEGED_READONLY            ( 0x02 << MPU_RASR_AP_Pos )
#define MPU_AP_FULL_ACCESS                      ( 0x03 << MPU_RASR_AP_Pos )
#define MPU_AP_RES                              ( 0x04 << MPU_RASR_AP_Pos )
#define MPU_AP_PRIVILEGED_READONLY              ( 0x05 << MPU_RASR_AP_Pos )
#define MPU_AP_READONLY                         ( 0x06 << MPU_RASR_AP_Pos )
#define MPU_AP_READONLY2                        ( 0x07 << MPU_RASR_AP_Pos )

#define MPU_TEX_B000                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B001                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B010                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B011                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B100                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B101                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B110                            ( 0x01 << MPU_RASR_TEX_Pos )
#define MPU_TEX_B111                            ( 0x01 << MPU_RASR_TEX_Pos )

/********* IFLASH memory macros *********************/
#define ITCM_START_ADDRESS                  0x00000000UL
#define ITCM_END_ADDRESS                    0x003FFFFFUL
#define IFLASH_START_ADDRESS                0x00400000UL
#define IFLASH_END_ADDRESS                  0x005FFFFFUL

#define IFLASH_PRIVILEGE_START_ADDRESS      (IFLASH_START_ADDRESS)
#define IFLASH_PRIVILEGE_END_ADDRESS        (IFLASH_START_ADDRESS + 0xFFF)

#define IFLASH_UNPRIVILEGE_START_ADDRESS    (IFLASH_PRIVILEGE_END_ADDRESS + 1)
#define IFLASH_UNPRIVILEGE_END_ADDRESS      (IFLASH_END_ADDRESS)

/**************** DTCM  *******************************/
#define DTCM_START_ADDRESS                  0x20000000UL
#define DTCM_END_ADDRESS                    0x203FFFFFUL


/******* SRAM memory macros ***************************/

#define SRAM_START_ADDRESS                  0x20400000UL
#define SRAM_END_ADDRESS                    0x2045FFFFUL

#define NOCACHE_SRAM_REGION_SIZE            0x1000

/* Regions should be a 2^(N+1)  where 4 < N < 31 */
#define SRAM_FIRST_START_ADDRESS            (SRAM_START_ADDRESS)
#define SRAM_FIRST_END_ADDRESS              (SRAM_FIRST_START_ADDRESS + 0x3FFFF)        // (2^18) 256 KB 

#define SRAM_SECOND_START_ADDRESS           (SRAM_FIRST_END_ADDRESS+1)
#define SRAM_SECOND_END_ADDRESS             (SRAM_END_ADDRESS - NOCACHE_SRAM_REGION_SIZE )              // (2^17) 128 - 0x1000 KB
#define SRAM_NOCACHE_START_ADDRESS          (SRAM_SECOND_END_ADDRESS + 1)
#define SRAM_NOCACHE_END_ADDRESS            (SRAM_END_ADDRESS )

/************** Peripherals memory region macros ********/
#define PERIPHERALS_START_ADDRESS            0x40000000UL
#define PERIPHERALS_END_ADDRESS              0x5FFFFFFFUL

/******* Ext EBI memory macros ***************************/
#define EXT_EBI_START_ADDRESS                0x60000000UL
#define EXT_EBI_END_ADDRESS                  0x6FFFFFFFUL

/******* Ext-SRAM memory macros ***************************/
#define SDRAM_START_ADDRESS                  0x70000000UL
#define SDRAM_END_ADDRESS                    0x7FFFFFFFUL

/******* QSPI macros ***************************/
#define QSPI_START_ADDRESS                   0x80000000UL
#define QSPI_END_ADDRESS                     0x9FFFFFFFUL

/************** USBHS_RAM region macros ******************/
#define USBHSRAM_START_ADDRESS               0xA0100000UL
#define USBHSRAM_END_ADDRESS                 0xA01FFFFFUL


/* Default memory map 
   Address range        Memory region      Memory type   Shareability  Cache policy
   0x00000000- 0x1FFFFFFF Code             Normal        Non-shareable  WT
   0x20000000- 0x3FFFFFFF SRAM             Normal        Non-shareable  WBWA
   0x40000000- 0x5FFFFFFF Peripheral       Device        Non-shareable  -
   0x60000000- 0x7FFFFFFF RAM              Normal        Non-shareable  WBWA
   0x80000000- 0x9FFFFFFF RAM              Normal        Non-shareable  WT
   0xA0000000- 0xBFFFFFFF Device           Device        Shareable
   0xC0000000- 0xDFFFFFFF Device           Device        Non Shareable
   0xE0000000- 0xFFFFFFFF System           -                  -
   */

/**
 * \brief Set up a memory region.
 */
void _SetupMemoryRegion( void )
{

	uint32_t dwRegionBaseAddr;
	uint32_t dwRegionAttr;
	
	memory_barrier();

/***************************************************
	ITCM memory region --- Normal 
	START_Addr:-  0x00000000UL
	END_Addr:-    0x00400000UL
****************************************************/
	dwRegionBaseAddr = 
		ITCM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_ITCM_REGION;        // 1

	dwRegionAttr = 
		MPU_AP_PRIVILEGED_READ_WRITE | 
		MPU_CalMPURegionSize(ITCM_END_ADDRESS - ITCM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
	Internal flash memory region --- Normal read-only 
	(update to Strongly ordered in write accesses)
	START_Addr:-  0x00400000UL
	END_Addr:-    0x00600000UL
******************************************************/
	
	dwRegionBaseAddr = 
		IFLASH_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_IFLASH_REGION;      //2

	dwRegionAttr = 
		MPU_AP_READONLY |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
	DTCM memory region --- Normal
	START_Addr:-  0x20000000L
	END_Addr:-    0x20400000UL
******************************************************/

	/* DTCM memory region */
	dwRegionBaseAddr = 
		DTCM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_DTCM_REGION;         //3

	dwRegionAttr = 
		MPU_AP_PRIVILEGED_READ_WRITE | 
		MPU_CalMPURegionSize(DTCM_END_ADDRESS - DTCM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
	SRAM Cacheable memory region --- Normal
	START_Addr:-  0x20400000UL
	END_Addr:-    0x2043FFFFUL
******************************************************/
	/* SRAM memory  region */
	dwRegionBaseAddr = 
		SRAM_FIRST_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_SRAM_REGION_1;         //4

	dwRegionAttr = 
		MPU_AP_FULL_ACCESS    |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		MPU_CalMPURegionSize(SRAM_FIRST_END_ADDRESS - SRAM_FIRST_START_ADDRESS) 
		| MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

	
/****************************************************
	Internal SRAM second partition memory region --- Normal 
	START_Addr:-  0x20440000UL
	END_Addr:-    0x2045FFFFUL
******************************************************/
	/* SRAM memory region */
	dwRegionBaseAddr = 
		SRAM_SECOND_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_SRAM_REGION_2;         //5

	dwRegionAttr = 
		MPU_AP_FULL_ACCESS    |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		MPU_CalMPURegionSize(SRAM_SECOND_END_ADDRESS - SRAM_SECOND_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
  
  /* NOCACHE_REGION */
	dwRegionBaseAddr =
        SRAM_NOCACHE_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_NOCACHE_SRAM_REGION;          //11

    dwRegionAttr =
        MPU_AP_FULL_ACCESS    |
        INNER_OUTER_NORMAL_NOCACHE_TYPE( SHAREABLE ) |
        MPU_CalMPURegionSize(NOCACHE_SRAM_REGION_SIZE) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

	
/****************************************************
	Peripheral memory region --- DEVICE Shareable
	START_Addr:-  0x40000000UL
	END_Addr:-    0x5FFFFFFFUL
******************************************************/
	dwRegionBaseAddr = 
		PERIPHERALS_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_PERIPHERALS_REGION;          //6

	dwRegionAttr = MPU_AP_FULL_ACCESS |
		MPU_REGION_EXECUTE_NEVER |
		SHAREABLE_DEVICE_TYPE |
		MPU_CalMPURegionSize(PERIPHERALS_END_ADDRESS - PERIPHERALS_START_ADDRESS)
		|MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);


/****************************************************
	External EBI memory  memory region --- Strongly Ordered
	START_Addr:-  0x60000000UL
	END_Addr:-    0x6FFFFFFFUL
******************************************************/
	dwRegionBaseAddr =
		EXT_EBI_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_EXT_EBI_REGION;              //7

	dwRegionAttr =
		MPU_AP_FULL_ACCESS |
		/* External memory Must be defined with 'Device' or 'Strongly Ordered'
		attribute for write accesses (AXI) */
		STRONGLY_ORDERED_SHAREABLE_TYPE |
		MPU_CalMPURegionSize(EXT_EBI_END_ADDRESS - EXT_EBI_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
	SDRAM Cacheable memory region --- Normal
	START_Addr:-  0x70000000UL
	END_Addr:-    0x7FFFFFFFUL
******************************************************/
	dwRegionBaseAddr = 
		SDRAM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_SDRAM_REGION;        //8

	dwRegionAttr = 
		MPU_AP_FULL_ACCESS    |
		INNER_NORMAL_WB_RWA_TYPE( SHAREABLE ) |
		MPU_CalMPURegionSize(SDRAM_END_ADDRESS - SDRAM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
	QSPI memory region --- Strongly ordered
	START_Addr:-  0x80000000UL
	END_Addr:-    0x9FFFFFFFUL
******************************************************/
	dwRegionBaseAddr = 
		QSPI_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_QSPIMEM_REGION;              //9

	dwRegionAttr = 
		MPU_AP_FULL_ACCESS |
		STRONGLY_ORDERED_SHAREABLE_TYPE |
		MPU_CalMPURegionSize(QSPI_END_ADDRESS - QSPI_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

  
/****************************************************
	USB RAM Memory region --- Device
	START_Addr:-  0xA0100000UL
	END_Addr:-    0xA01FFFFFUL
******************************************************/
	dwRegionBaseAddr = 
		USBHSRAM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_USBHSRAM_REGION;              //10

	dwRegionAttr = 
		MPU_AP_FULL_ACCESS |
		MPU_REGION_EXECUTE_NEVER |
		SHAREABLE_DEVICE_TYPE |
		MPU_CalMPURegionSize(USBHSRAM_END_ADDRESS - USBHSRAM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);


	/* Enable the memory management fault , Bus Fault, Usage Fault exception */
	SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk
					| SCB_SHCSR_USGFAULTENA_Msk);

	/* Enable the MPU region */
	MPU_Enable( MPU_ENABLE | MPU_PRIVDEFENA);
	
	memory_sync();
}
