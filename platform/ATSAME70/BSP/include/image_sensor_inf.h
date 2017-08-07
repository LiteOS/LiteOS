/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2015, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"


/*---------------------------------------------------------------------------
 *         Definition
 *---------------------------------------------------------------------------*/
#define SENDOR_SUPPORTED_OUTPUTS 7

/** terminating list entry for register in configuration file */
#define SENSOR_REG_TERM         0xFF
/** terminating list entry for value in configuration file */
#define SENSOR_VAL_TERM         0xFF

/*----------------------------------------------------------------------------
 *        Types
 *----------------------------------------------------------------------------*/

/** Sensor type */
typedef enum _sensorType {
	SENSOR_COMS = 0,
	SENSOR_CCD
} sensorType_t;

/** Sensor status or return code */
typedef enum _sensorStatus {
	SENSOR_OK = 0,        /**< Operation is successful */
	SENSOR_TWI_ERROR,
	SENSOR_ID_ERROR,
	SENSOR_RESOLUTION_NOT_SUPPORTED
} sendorStatus_t;

/** Sensor TWI mode */
typedef enum _sensorTwiMode {
	SENSOR_TWI_REG_BYTE_DATA_BYTE = 0,
	SENSOR_TWI_REG_2BYTE_DATA_BYTE,
	SENSOR_TWI_REG_BYTE_DATA_2BYTE
} sensorTwiMode_t;

/** Sensor resolution */
typedef enum _sensorResolution {
	QVGA = 0,
	VGA,
	SVGA,
	XGA,
	WXGA,
	UVGA
} sensorOutputResolution_t;

/** Sensor output format */
typedef enum _sensorOutputFormat {
	RAW_BAYER_12_BIT = 0,
	RAW_BAYER_10_BIT,
	YUV_422_8_BIT,
	YUV_422_10_BIT,
	MONO_12_BIT
} sensorOutputFormat_t;

/** define a structure for sensor register initialization values */
typedef struct _sensor_reg {
	uint16_t reg; /* Register to be written */
	uint16_t val; /* value to be written */
} sensorReg_t;

typedef struct _sensor_output {
	uint8_t type;                              /** Index 0: normal, 1: AF setting*/
	sensorOutputResolution_t output_resolution; /** sensor output resolution */
	sensorOutputFormat_t output_format;         /** sensor output format */
	uint8_t supported;                          /** supported for current output_resolution*/
	uint32_t output_width;                      /** output width */
	uint32_t output_height;                     /** output height */
	const sensorReg_t *output_setting;          /** sensor registers setting */
} sensorOutput_t;

/** define a structure for sensor profile */
typedef struct _sensor_profile {
	sensorType_t cmos_ccd;        /** Sensor type for CMOS sensor or CCD */
	sensorTwiMode_t twi_inf_mode; /** TWI interface mode  */
	uint32_t twi_slave_addr;      /** TWI slave address */
	uint16_t pid_high_reg;        /** Register address for product ID high byte */
	uint16_t pid_low_reg;         /** Register address for product ID low byte*/
	uint16_t pid_high;            /** product ID high byte */
	uint16_t pid_low;             /** product ID low byte */
	uint16_t version_mask;        /** version mask */
	const sensorOutput_t
		*outputConf[SENDOR_SUPPORTED_OUTPUTS]; /** sensor settings */
} sensorProfile_t;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
extern sendorStatus_t sensor_twi_write_regs(Twid *pTwid,
		const sensorReg_t *pReglist);

extern sendorStatus_t sensor_twi_read_regs(Twid *pTwid,
		const sensorReg_t *pReglist);

extern sendorStatus_t sensor_setup(Twid *pTwid,
		const sensorProfile_t *sensor_profile,
		sensorOutputResolution_t resolution);

extern sendorStatus_t sensor_get_output(sensorOutputFormat_t *format,
		uint32_t *width,
		uint32_t *height,
		sensorOutputResolution_t resolution);
