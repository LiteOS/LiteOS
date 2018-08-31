/**************************************************************************//**
 * @file
 * @brief EFM32GG_DK3750, TFT Initialization and setup for Adress Mapped mode
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#include <stdbool.h>
#include "em_device.h"
#include "em_gpio.h"
#include "em_ebi.h"
#include "bsp.h"
#include "tftamapped.h"
/* DMD init from dmd_ssd2119_direct.c */
#include "dmd/ssd2119/dmd_ssd2119.h"
#include "glib/glib.h"

/** Flag to indicate that we need to initialize once if starting up in */
static bool runOnce = true;

/**************************************************************************//**
 * @brief  TFT initialize or reinitialize to Address Mapped Mode
 *         Assumes EBI has been configured correctly in BSP_Init(BSP_INIT_DK_EBI)
 *
 * @return true if we should redraw into buffer, false if BC has control
 *         over display
 *****************************************************************************/
bool TFT_AddressMappedInit(void)
{
  bool     ret;
  EMSTATUS status;
  uint32_t i, freq;

  /* If we are in BC_UIF_AEM_EFM state, we can redraw graphics */
  if (BSP_RegisterRead(&BC_REGISTER->UIF_AEM) == BC_UIF_AEM_EFM)
  {
    /* If we're not BC_ARB_CTRL_EBI state, we need to reconfigure display controller */
    if ((BSP_RegisterRead(&BC_REGISTER->ARB_CTRL) != BC_ARB_CTRL_EBI) || runOnce)
    {
      /* Configure for EBI mode and reset display */
      BSP_DisplayControl(BSP_Display_EBI);
      BSP_DisplayControl(BSP_Display_ResetAssert);
      BSP_DisplayControl(BSP_Display_PowerDisable);
      /* Short reset delay */
      freq = SystemCoreClockGet();
      for (i = 0; i < (freq / 100); i++)
      {
        __NOP();
      }
      /* Configure display for Direct Drive + SPI mode */
      BSP_DisplayControl(BSP_Display_Mode8080);
      BSP_DisplayControl(BSP_Display_PowerEnable);
      BSP_DisplayControl(BSP_Display_ResetRelease);

      /* Initialize graphics - abort on failure */
      status = DMDIF_init(BC_SSD2119_BASE, BC_SSD2119_BASE + 2);
      if (status == DMD_OK) status = DMD_init(0);
      if ((status != DMD_OK) && (status != DMD_ERROR_DRIVER_ALREADY_INITIALIZED)) while (1) ;
      /* Make sure display is configured with correct rotation */
      if ((status == DMD_OK)) DMD_flipDisplay(1, 1);

      runOnce = false;
    }
    ret = true;
  }
  else
  {
    ret = false;
  }
  return ret;
}
