/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "board.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_i2c.h"

#include <stdbool.h>

#include "app_pct2075.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

uint8_t pct2075_i2c_init(void)
{
	uint32_t i = 0;
	i2c_slave_config_t slaveConfig;
	i2c_master_config_t masterConfig;
	
  IOCON_PinMuxSet(IOCON, 1,  1, IOCON_MODE_PULLUP | IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
  IOCON_PinMuxSet(IOCON, 1,  2, IOCON_MODE_PULLUP | IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
	
	
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
	RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);
	
	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = 100000U;
	I2C_MasterInit((I2C_Type *)I2C4_BASE, &masterConfig, 12000000);
}

uint8_t pct2075_i2c_read(float *data)
{
	status_t reVal = kStatus_Fail;
	uint8_t buf[2];
#if 0
    if (kStatus_Success == I2C_MasterStart((I2C_Type *)I2C4_BASE, 0x4C, kI2C_Read))
    {
			  g_master_buff[0] = 0;
        reVal = I2C_MasterWriteBlocking((I2C_Type *)I2C4_BASE, g_master_buff, 1, 1);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
				memset(g_master_buff, 0x00, 10);
				reVal = I2C_MasterReadBlocking((I2C_Type *)I2C4_BASE, g_master_buff, 2, 0);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
				
        reVal = I2C_MasterStop((I2C_Type *)I2C4_BASE);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }
#endif	
		i2c_master_transfer_t xfer;
		xfer.slaveAddress   = 0x4C;
		xfer.direction      = kI2C_Read;
		xfer.subaddress     = 0x00;
		xfer.subaddressSize = 1;
		xfer.data           = buf;
		xfer.dataSize       = 2;
		xfer.flags          = 0;
		
		reVal = I2C_MasterTransferBlocking((I2C_Type *)I2C4_BASE, &xfer);
#if 0		
// test MGN
		buf[0] = 0xE7; buf[1] = 0x00; // -25oC
		buf[0] = 0xC9; buf[1] = 0x20; // -54.875oC
		buf[0] = 0xFF; buf[1] = 0xE0; // -1oC
#endif		
		if( (buf[0]&0x80) == 0x00)
		{
			*data = ((float)( ((buf[0]<<8) + buf[1])>>5) * 0.125); 
		}
		else
		{
			*data = 0x800 - ((buf[0]<<8) + buf[1]>>5 );
			*data = -(((float)(*data)) * 0.125); 
		}
		if( reVal != kStatus_Success )
		{
			return 0;
		}
		else
		{
			return 1;
		}
}


// end file
