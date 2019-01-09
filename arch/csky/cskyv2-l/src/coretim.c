#include <csi_config.h>
#include "../Drivers/CSKY_CK802T_CSI_Driver/Inc/soc.h"
#include "../Drivers/CSKY_CK802T_CSI_Driver/Inc/csi_core.h"

#include <target_config.h>
#include "los_hwi.h"
#define CORETIM_BASE     (0xE000E000)

#define CORET_CSR         (volatile uint32_t *)(CORETIM_BASE + 0x10)

/*
 *  define the bits for TxControl
 */
#define CORETIM_TXCONTROL_ENABLE      (1UL << 0)
#define CORETIM_TXCONTROL_INTMASK     (1UL << 1)
#define CORETIM_TXCONTROL_MODE        (1UL << 16)

//extern void osTickHandler(void);
extern UINT32 osTickHandler(void);

static inline uint32_t readreg32(volatile uint32_t *addr)
{
    return *(volatile uint32_t *)addr;
}

void CORET_IRQHandler(void)
{
    readreg32(CORET_CSR);

    osTickHandler ();
}

//void osTickHandler(void)
UINT32 osTickStart (void)
{
    csi_coret_config(SYSTEM_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND, CORET_IRQn);    //10ms
    csi_vic_enable_irq(CORET_IRQn);
    LOS_HwiCreate (CORET_IRQn, 0, 0, (void (*) (uintptr_t))CORET_IRQHandler, 0);
	
	return LOS_OK;
}