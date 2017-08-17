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

/**
 * \file
 */
#include "board.h"

#define MT9V022_SLAVE_ADDRESS   (0x90>>1)
#define MT9V022_PIDH_ADDRESS    0x0
#define MT9V022_PIDL_ADDRESS    0x0
#define MT9V022_PIDH            0x13
#define MT9V022_PIDL            0x13
#define MT9V022_PID_VER_MASK    0xFFF0
/*------------------------------------------------------------------------------
 *         Local Variables
 *------------------------------------------------------------------------------*/

const sensorReg_t mt9v022_mono_vga[] = {
	{0x01, 0x0100 },    // COL_WINDOW_START_CONTEXTA_REG
	{0x02, 0x0400 },    // ROW_WINDOW_START_CONTEXTA_REG
	{0x03, 0xE001 },    // ROW_WINDOW_SIZE_CONTEXTA_REG
	{0x04, 0x8002 },    // COL_WINDOW_SIZE_CONTEXTA_REG
	{0x05, 0x5E00 },    // HORZ_BLANK_CONTEXTA_REG
	{0x06, 0x2D00 },    // VERT_BLANK_CONTEXTA_REG
	{0x07, 0x8801 },    // CONTROL_MODE_REG
	{0x08, 0x9001 },    // COARSE_SHUTTER_WIDTH_1_CONTEXTA
	{0x09, 0xBD01 },    // COARSE_SHUTTER_WIDTH_2_CONTEXTA
	{0x0A, 0x6401 },    // SHUTTER_WIDTH_CONTROL_CONTEXTA
	{0x0B, 0xC201 },    // COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
	{0x0C, 0x0 },    // RESET_REG
	{0x0D, 0x0003 },    // READ_MODE_REG
	{0x0E, 0x0 },    // READ_MODE2_REG
	{0x0F, 0x1100 },    // PIXEL_OPERATION_MODE 0x0100
	//                  {0x0F, 0x1500 },    // PIXEL_OPERATION_MODE 0x0100
	{0x10, 0x4000 },    // RESERVED_CORE_10
	{0x11, 0x4280 },    // RESERVED_CORE_11
	{0x12, 0x2200 },    // RESERVED_CORE_12
	{0x13, 0x322D },    // RESERVED_CORE_13
	{0x14, 0x020E },    // RESERVED_CORE_14
	{0x15, 0x327F },    // RESERVED_CORE_15
	{0x16, 0x0228 },    // RESERVED_CORE_16
	{0x17, 0x383E },    // RESERVED_CORE_17
	{0x18, 0x383E },    // RESERVED_CORE_18
	{0x19, 0x0228 },    // RESERVED_CORE_19
	{0x1A, 0x2804 },    // RESERVED_CORE_1A
	{0x1B, 0x0 },    // LED_OUT_CONTROL
	{0x1C, 0x0203 },    // DATA_COMPRESSION
	{0x1D, 0x0 },    // RESERVED_CORE_1D
	{0x1E, 0x0 },    // RESERVED_CORE_1E
	{0x1F, 0x0 },    // RESERVED_CORE_1F
	{0x20, 0xD101 },    // RESERVED_CORE_20
	{0x21, 0x2000 },    // RESERVED_CORE_21
	{0x22, 0x2000 },    // RESERVED_CORE_22
	{0x23, 0x1000 },    // RESERVED_CORE_23
	{0x24, 0x1000 },    // RESERVED_CORE_24
	{0x25, 0x2000 },    // RESERVED_CORE_25
	{0x26, 0x1000 },    // RESERVED_CORE_26
	{0x27, 0x1000 },    // RESERVED_CORE_27
	{0x28, 0x1000 },    // RESERVED_CORE_28
	{0x29, 0x1000 },    // RESERVED_CORE_29
	{0x2A, 0x2000 },    // RESERVED_CORE_2A
	{0x2B, 0x0300 },    // RESERVED_CORE_2B
	{0x2C, 0x0400 },    // VREF_ADC_CONTROL  //0~7
	{0x2D, 0x0400 },    // RESERVED_CORE_2D
	{0x2E, 0x0700 },    // RESERVED_CORE_2E
	{0x2F, 0x0400 },    // RESERVED_CORE_2F
	{0x30, 0x0300 },    // RESERVED_CORE_30
	{0x31, 0x1F00 },    // V1_CONTROL_CONTEXTA
	{0x32, 0x1A00 },    // V2_CONTROL_CONTEXTA
	{0x33, 0x1200 },    // V3_CONTROL_CONTEXTA
	{0x34, 0x0300 },    // V4_CONTROL_CONTEXTA
	{0x35, 0x1000 },    // GLOBAL_GAIN_CONTEXTA_REG
	{0x36, 0x4000 },    // GLOBAL_GAIN_CONTEXTB_REG
	{0x37, 0x0 },    // RESERVED_CORE_37
	{0x38, 0x0 },    // RESERVED_CORE_38
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x60, 0x0 },    // RESERVED_CORE_60
	{0x61, 0x0 },    // RESERVED_CORE_61
	{0x62, 0x0 },    // RESERVED_CORE_62
	{0x63, 0x0 },    // RESERVED_CORE_63
	{0x64, 0x0 },    // RESERVED_CORE_64
	{0x65, 0x0 },    // RESERVED_CORE_65
	{0x66, 0x0 },    // RESERVED_CORE_66
	{0x67, 0x0 },    // RESERVED_CORE_67
	{0x6C, 0x0 },    // RESERVED_CORE_6C
	{0x70, 0x0400 },    // ROW_NOISE_CONTROL
	{0x71, 0x0 },    // NOISE_CONSTANT
	{0x72, 0x2A00 },    // PIXCLK_CONTROL
	{0x73, 0xF702 },    //
	{0x74, 0x0 },    //
	{0x7F, 0x0 },    // TEST_DATA
	//{0x7F, 0x0028 },    // TEST_DATA
	{0x80, 0xF400 },    // TILE_X0_Y0
	{0x81, 0xF400 },    // TILE_X1_Y0
	{0x82, 0xF400 },    // TILE_X2_Y0
	{0x83, 0xF400 },    // TILE_X3_Y0
	{0x84, 0xF400 },    // TILE_X4_Y0
	{0x85, 0xF400 },    // TILE_X0_Y1
	{0x86, 0xF400 },    // TILE_X1_Y1
	{0x87, 0xF400 },    // TILE_X2_Y1
	{0x88, 0xF400 },    // TILE_X3_Y1
	{0x89, 0xF400 },    // TILE_X4_Y1
	{0x8A, 0xF400 },    // TILE_X0_Y2
	{0x8B, 0xF400 },    // TILE_X1_Y2
	{0x8C, 0xF400 },    // TILE_X2_Y2
	{0x8D, 0xF400 },    // TILE_X3_Y2
	{0x8E, 0xF400 },    // TILE_X4_Y2
	{0x8F, 0xF400 },    // TILE_X0_Y3
	{0x90, 0xF400 },    // TILE_X1_Y3
	{0x91, 0xF400 },    // TILE_X2_Y3
	{0x92, 0xF400 },    // TILE_X3_Y3
	{0x93, 0xF400 },    // TILE_X4_Y3
	{0x94, 0xF400 },    // TILE_X0_Y4
	{0x95, 0xF400 },    // TILE_X1_Y4
	{0x96, 0xF400 },    // TILE_X2_Y4
	{0x97, 0xF400 },    // TILE_X3_Y4
	{0x98, 0xF400 },    // TILE_X4_Y4
	{0x99, 0x0 },    // X0_SLASH5
	{0x9A, 0x9600 },    // X1_SLASH5
	{0x9B, 0x2C01 },    // X2_SLASH5
	{0x9C, 0xC201 },    // X3_SLASH5
	{0x9D, 0x5802 },    // X4_SLASH5
	{0x9E, 0xF002 },    // X5_SLASH5
	{0x9F, 0x0 },    // Y0_SLASH5
	{0xA0, 0x6000 },    // Y1_SLASH5
	{0xA1, 0xC000 },    // Y2_SLASH5
	{0xA2, 0x2001 },    // Y3_SLASH5
	{0xA3, 0x8001 },    // Y4_SLASH5
	{0xA4, 0xE001 },    // Y5_SLASH5
	{0xA5, 0x3A00 },    // DESIRED_BIN
	{0xA6, 0x0200 },    // EXP_SKIP_FRM_H
	{0xA8, 0x0200 },    // EXP_LPF
	{0xA9, 0x0200 },    // GAIN_SKIP_FRM
	{0xAA, 0x0 },    // GAIN_LPF_H
	{0xAB, 0x0200 },    // MAX_GAIN
	{0xAC, 0x0100 },    // MIN_COARSE_EXPOSURE
	{0xAD, 0xE001 },    // MAX_COARSE_EXPOSURE
	{0xAE, 0x1400 },    // BIN_DIFF_THRESHOLD
	{0xAF, 0x0300 },    // AUTO_BLOCK_CONTROL
	{0xB0, 0xE0AB },    // PIXEL_COUNT
	{0xB1, 0x0200 },    // LVDS_MASTER_CONTROL
	{0xB2, 0x1000 },    // LVDS_SHFT_CLK_CONTROL
	{0xB3, 0x1000 },    // LVDS_DATA_CONTROL
	{0xB4, 0x0 },    // LVDS_DATA_STREAM_LATENCY
	{0xB5, 0x0 },    // LVDS_INTERNAL_SYNC
	{0xB6, 0x0 },    // LVDS_USE_10BIT_PIXELS
	{0xB7, 0x0 },    // STEREO_ERROR_CONTROL
	{0xBD, 0xE001 },    // Max Shutter ??(??AEC???) 1~0x7FF
	{0xBF, 0x1600 },    // INTERLACE_FIELD_VBLANK
	{0xC0, 0x0A00 },    // IMAGE_CAPTURE_NUM
	{0xC2, 0x4008 },    // ANALOG_CONTROLS
	{0xC3, 0x0 },    // RESERVED_CORE_C3  0x007F
	{0xC4, 0x1644 },    // RESERVED_CORE_C4
	{0xC5, 0x2144 },    // RESERVED_CORE_C5
	{0xF1, 0x0 },    // RESERVED_CORE_ F1
	{0xFE, 0xEFBE },    // RESERVED_CORE_FE
	{0xFF, 0xFF},    // END use
};

const sensorReg_t mt9v022_mono_qvga[] = {
	{0x01, 0x0100 },    // COL_WINDOW_START_CONTEXTA_REG
	{0x02, 0x0400 },    // ROW_WINDOW_START_CONTEXTA_REG
	{0x03, 0xE001 },    // ROW_WINDOW_SIZE_CONTEXTA_REG
	{0x04, 0x8002 },    // COL_WINDOW_SIZE_CONTEXTA_REG
	{0x05, 0x5E00 },    // HORZ_BLANK_CONTEXTA_REG
	//{0x06, 0x2D00 },    // VERT_BLANK_CONTEXTA_REG
	{0x06, 0x1D01 },    // VERT_BLANK_CONTEXTA_REG QVGA VB 285
	{0x07, 0x8801 },    // CONTROL_MODE_REG
	{0x08, 0x9001 },    // COARSE_SHUTTER_WIDTH_1_CONTEXTA
	{0x09, 0xBD01 },    // COARSE_SHUTTER_WIDTH_2_CONTEXTA
	{0x0A, 0x6401 },    // SHUTTER_WIDTH_CONTROL_CONTEXTA
	{0x0B, 0xC201 },    // COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
	{0x0C, 0x0 },    // RESET_REG
	//{0x0D, 0x0003 },    // READ_MODE_REG
	{0x0D, 0x0503 },    // READ_MODE_REG QVGA bin2
	{0x0E, 0x0 },    // READ_MODE2_REG
	{0x0F, 0x1100 },    // PIXEL_OPERATION_MODE 0x0100
	//                  {0x0F, 0x1500 },    // PIXEL_OPERATION_MODE 0x0100
	{0x10, 0x4000 },    // RESERVED_CORE_10
	{0x11, 0x4280 },    // RESERVED_CORE_11
	{0x12, 0x2200 },    // RESERVED_CORE_12
	{0x13, 0x322D },    // RESERVED_CORE_13
	{0x14, 0x020E },    // RESERVED_CORE_14
	{0x15, 0x327F },    // RESERVED_CORE_15
	{0x16, 0x0228 },    // RESERVED_CORE_16
	{0x17, 0x383E },    // RESERVED_CORE_17
	{0x18, 0x383E },    // RESERVED_CORE_18
	{0x19, 0x0228 },    // RESERVED_CORE_19
	{0x1A, 0x2804 },    // RESERVED_CORE_1A
	{0x1B, 0x0 },    // LED_OUT_CONTROL
	{0x1C, 0x0203 },    // DATA_COMPRESSION
	{0x1D, 0x0 },    // RESERVED_CORE_1D
	{0x1E, 0x0 },    // RESERVED_CORE_1E
	{0x1F, 0x0 },    // RESERVED_CORE_1F
	{0x20, 0xD101 },    // RESERVED_CORE_20
	{0x21, 0x2000 },    // RESERVED_CORE_21
	{0x22, 0x2000 },    // RESERVED_CORE_22
	{0x23, 0x1000 },    // RESERVED_CORE_23
	{0x24, 0x1000 },    // RESERVED_CORE_24
	{0x25, 0x2000 },    // RESERVED_CORE_25
	{0x26, 0x1000 },    // RESERVED_CORE_26
	{0x27, 0x1000 },    // RESERVED_CORE_27
	{0x28, 0x1000 },    // RESERVED_CORE_28
	{0x29, 0x1000 },    // RESERVED_CORE_29
	{0x2A, 0x2000 },    // RESERVED_CORE_2A
	{0x2B, 0x0300 },    // RESERVED_CORE_2B
	{0x2C, 0x0400 },    // VREF_ADC_CONTROL  //0~7
	{0x2D, 0x0400 },    // RESERVED_CORE_2D
	{0x2E, 0x0700 },    // RESERVED_CORE_2E
	{0x2F, 0x0400 },    // RESERVED_CORE_2F
	{0x30, 0x0300 },    // RESERVED_CORE_30
	{0x31, 0x1F00 },    // V1_CONTROL_CONTEXTA
	{0x32, 0x1A00 },    // V2_CONTROL_CONTEXTA
	{0x33, 0x1200 },    // V3_CONTROL_CONTEXTA
	{0x34, 0x0300 },    // V4_CONTROL_CONTEXTA
	{0x35, 0x1000 },    // GLOBAL_GAIN_CONTEXTA_REG
	{0x36, 0x4000 },    // GLOBAL_GAIN_CONTEXTB_REG
	{0x37, 0x0 },    // RESERVED_CORE_37
	{0x38, 0x0 },    // RESERVED_CORE_38
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x39, 0x2500 },    // V1_CONTROL_CONTEXTB//none
	{0x3A, 0x2000 },    // V2_CONTROL_CONTEXTB//none
	{0x3B, 0x0300 },    // V3_CONTROL_CONTEXTB//none
	{0x3C, 0x0300 },    // V4_CONTROL_CONTEXTB//none
	{0x46, 0x1D23 },    // DARK_AVG_THRESHOLDS
	{0x47, 0x8080 },    // CALIB_CONTROL_REG
	{0x4C, 0x0200 },    // STEP_SIZE_AVG_MODE
	{0x60, 0x0 },    // RESERVED_CORE_60
	{0x61, 0x0 },    // RESERVED_CORE_61
	{0x62, 0x0 },    // RESERVED_CORE_62
	{0x63, 0x0 },    // RESERVED_CORE_63
	{0x64, 0x0 },    // RESERVED_CORE_64
	{0x65, 0x0 },    // RESERVED_CORE_65
	{0x66, 0x0 },    // RESERVED_CORE_66
	{0x67, 0x0 },    // RESERVED_CORE_67
	{0x6C, 0x0 },    // RESERVED_CORE_6C
	{0x70, 0x0400 },    // ROW_NOISE_CONTROL
	{0x71, 0x0 },    // NOISE_CONSTANT
	{0x72, 0x2A00 },    // PIXCLK_CONTROL
	{0x73, 0xF702 },    //
	{0x74, 0x0 },    //
	{0x7F, 0x0 },    // TEST_DATA
	//{0x7F, 0x0028 },    // TEST_DATA
	{0x80, 0xF400 },    // TILE_X0_Y0
	{0x81, 0xF400 },    // TILE_X1_Y0
	{0x82, 0xF400 },    // TILE_X2_Y0
	{0x83, 0xF400 },    // TILE_X3_Y0
	{0x84, 0xF400 },    // TILE_X4_Y0
	{0x85, 0xF400 },    // TILE_X0_Y1
	{0x86, 0xF400 },    // TILE_X1_Y1
	{0x87, 0xF400 },    // TILE_X2_Y1
	{0x88, 0xF400 },    // TILE_X3_Y1
	{0x89, 0xF400 },    // TILE_X4_Y1
	{0x8A, 0xF400 },    // TILE_X0_Y2
	{0x8B, 0xF400 },    // TILE_X1_Y2
	{0x8C, 0xF400 },    // TILE_X2_Y2
	{0x8D, 0xF400 },    // TILE_X3_Y2
	{0x8E, 0xF400 },    // TILE_X4_Y2
	{0x8F, 0xF400 },    // TILE_X0_Y3
	{0x90, 0xF400 },    // TILE_X1_Y3
	{0x91, 0xF400 },    // TILE_X2_Y3
	{0x92, 0xF400 },    // TILE_X3_Y3
	{0x93, 0xF400 },    // TILE_X4_Y3
	{0x94, 0xF400 },    // TILE_X0_Y4
	{0x95, 0xF400 },    // TILE_X1_Y4
	{0x96, 0xF400 },    // TILE_X2_Y4
	{0x97, 0xF400 },    // TILE_X3_Y4
	{0x98, 0xF400 },    // TILE_X4_Y4
	{0x99, 0x0 },    // X0_SLASH5
	{0x9A, 0x9600 },    // X1_SLASH5
	{0x9B, 0x2C01 },    // X2_SLASH5
	{0x9C, 0xC201 },    // X3_SLASH5
	{0x9D, 0x5802 },    // X4_SLASH5
	{0x9E, 0xF002 },    // X5_SLASH5
	{0x9F, 0x0 },    // Y0_SLASH5
	{0xA0, 0x6000 },    // Y1_SLASH5
	{0xA1, 0xC000 },    // Y2_SLASH5
	{0xA2, 0x2001 },    // Y3_SLASH5
	{0xA3, 0x8001 },    // Y4_SLASH5
	{0xA4, 0xE001 },    // Y5_SLASH5
	{0xA5, 0x3A00 },    // DESIRED_BIN
	{0xA6, 0x0200 },    // EXP_SKIP_FRM_H
	{0xA8, 0x0200 },    // EXP_LPF
	{0xA9, 0x0200 },    // GAIN_SKIP_FRM
	{0xAA, 0x0 },    // GAIN_LPF_H
	{0xAB, 0x0200 },    // MAX_GAIN
	{0xAC, 0x0100 },    // MIN_COARSE_EXPOSURE
	{0xAD, 0xE001 },    // MAX_COARSE_EXPOSURE
	{0xAE, 0x1400 },    // BIN_DIFF_THRESHOLD
	{0xAF, 0x0300 },    // AUTO_BLOCK_CONTROL
	{0xB0, 0xE0AB },    // PIXEL_COUNT
	{0xB1, 0x0200 },    // LVDS_MASTER_CONTROL
	{0xB2, 0x1000 },    // LVDS_SHFT_CLK_CONTROL
	{0xB3, 0x1000 },    // LVDS_DATA_CONTROL
	{0xB4, 0x0 },    // LVDS_DATA_STREAM_LATENCY
	{0xB5, 0x0 },    // LVDS_INTERNAL_SYNC
	{0xB6, 0x0 },    // LVDS_USE_10BIT_PIXELS
	{0xB7, 0x0 },    // STEREO_ERROR_CONTROL
	{0xBD, 0xE001 },    // Max Shutter ??(??AEC???) 1~0x7FF
	{0xBF, 0x1600 },    // INTERLACE_FIELD_VBLANK
	{0xC0, 0x0A00 },    // IMAGE_CAPTURE_NUM
	{0xC2, 0x4008 },    // ANALOG_CONTROLS
	{0xC3, 0x0 },    // RESERVED_CORE_C3  0x007F
	{0xC4, 0x1644 },    // RESERVED_CORE_C4
	{0xC5, 0x2144 },    // RESERVED_CORE_C5
	{0xF1, 0x0 },    // RESERVED_CORE_ F1
	{0xFE, 0xEFBE },    // RESERVED_CORE_FE
	{0xFF, 0xFF},    // END use
};

const sensorOutput_t mt9v022_output_mono_vga =
{0, VGA, MONO_12_BIT, 1, 640, 480, mt9v022_mono_vga};

const sensorOutput_t mt9v022_output_mono_qvga =
{0, QVGA, MONO_12_BIT, 1, 320, 240, mt9v022_mono_qvga};

const sensorProfile_t mt9v022Profile = {
	SENSOR_COMS,                     /* Sensor type for CMOS sensor or CCD */
	SENSOR_TWI_REG_BYTE_DATA_2BYTE,  /* TWI interface mode  */
	MT9V022_SLAVE_ADDRESS,            /* TWI slave address */
	MT9V022_PIDH_ADDRESS,             /* Register address for product ID high byte */
	MT9V022_PIDL_ADDRESS,             /* Register address for product ID low byte*/
	MT9V022_PIDH,                     /* product ID high byte */
	MT9V022_PIDL,                     /* product ID low byte */
	MT9V022_PID_VER_MASK,             /* version mask */
	{&mt9v022_output_mono_vga,
	&mt9v022_output_mono_qvga,
	0,
	0,
	0,
	0,
	0}
};
