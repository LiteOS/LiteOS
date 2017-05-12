#include "los_bsp_lcd.h"

#ifdef EFM32PG1B200F256GM48
#include "bsp.h"
#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"
#include <stdio.h>

extern EMSTATUS RETARGET_WriteString(char*   str);

static DISPLAY_Device_t displayDevice;    /* Display device handle.         */
#endif

void LOS_EvbLcdInit(void)
{

#ifdef EFM32PG1B200F256GM48
		/* Initialize LCD driver */
		DISPLAY_Init();

		/* Retrieve the properties of the display. */
		if (DISPLAY_DeviceGet( 0, &displayDevice ) != DISPLAY_EMSTATUS_OK)
		{
		  /* Unable to get display handle. */
		  while (1);
		}

		/* Retarget stdio to the display. */
		if (TEXTDISPLAY_EMSTATUS_OK != RETARGET_TextDisplayInit())
		{
		  /* Text display initialization failed. */
		  while (1);
		}

		RETARGET_WriteString( "\n\n Welcome"
			"\n\n Lite OS"
			"\n\n Reginald-szr\n\n"
			"\n\n "
			"\n\n press BTN1 Test\n\n"
			);
#endif

	return ;
}

