/**************************************************************************//**
 * @file display.h
 * @brief Display device interface
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



#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdbool.h>
#include <stdint.h>
#include "emstatus.h"
#include "displayconfigall.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Display
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/** EMSTATUS codes of the display interface. */
#define DISPLAY_EMSTATUS_OK                                        (0) /**< Operation successful. */
#define DISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY (DISPLAY_EMSTATUS_BASE | 1) /**< Not enough memory. */
#define DISPLAY_EMSTATUS_OUT_OF_RANGE      (DISPLAY_EMSTATUS_BASE | 2) /**< Parameters out of range. */
#define DISPLAY_EMSTATUS_INVALID_PARAMETER (DISPLAY_EMSTATUS_BASE | 3) /**< Invalid parameter. */
#define DISPLAY_EMSTATUS_NOT_SUPPORTED     (DISPLAY_EMSTATUS_BASE | 4) /**< Feature/option not supported. */
#define DISPLAY_EMSTATUS_NOT_INITIALIZED   (DISPLAY_EMSTATUS_BASE | 5) /**< Feature/option not supported. */


/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Display device colour modes. */
typedef enum DISPLAY_ColourMode_t
{
  DISPLAY_COLOUR_MODE_MONOCHROME,         /**< White = pixel bit value = 0,
                                               Black = pixel bit value = 1 */
  DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE, /**< Black = pixel bit value = 0,
                                               White = pixel bit value = 1 */
} DISPLAY_ColourMode_t;


/** Display device address modes. */
typedef enum DISPLAY_AddressMode_t
{
  DISPLAY_ADDRESSING_BY_ROWS_ONLY,        /**< Display device is addressed by
                                             rows only. I.e. a full line of
                                             pixel data is required to update
                                             a single pixel. */
  DISPLAY_ADDRESSING_BY_ROWS_AND_COLUMNS  /**< Display device is addressed by
                                             both rows and columns. I.e. single
                                             pixel updates is supported. */
} DISPLAY_AddressMode_t;


/** Pixel matrix handle. */
typedef void* DISPLAY_PixelMatrix_t;


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** Display geometry specification. */
typedef struct DISPLAY_Geometry_t
{
  unsigned int   width;   /**< Pixel width of display. */
  unsigned int   stride;  /**< Total line width of display including pixels and
                               controls words. */
  unsigned int   height;  /**< Pixel height of display. */
} DISPLAY_Geometry_t;


/* Forward declaration of struct DISPLAY_Device_t in order to reference it
   inside the typdef. */
struct DISPLAY_Device_t;

/**
 * Display device data structure, including a specification of how the
 * display device behaves.
 */
typedef struct DISPLAY_Device_t
{
  char*                 name;        /**< Name of the display device. */
  DISPLAY_Geometry_t    geometry;    /**< Geometry of the display device. */
  DISPLAY_ColourMode_t  colourMode;  /**< Colour mode of the display device. */
  DISPLAY_AddressMode_t addressMode; /**< Address mode of the display device. */

  /** Turn power on display on or off. */
  EMSTATUS (*pDisplayPowerOn) (struct DISPLAY_Device_t* device,
                               bool                     on);

  /** Allocates a pixelMatrix buffer in the format specified by the geometry
      (DISPLAY_Geometry_t) of the display device. */
  EMSTATUS (*pPixelMatrixAllocate) (struct DISPLAY_Device_t* device,
                                    unsigned int  width,
#ifdef EMWIN_WORKAROUND
                                    unsigned int  userStride,
#endif
                                    unsigned int  height,
                                    DISPLAY_PixelMatrix_t  *pixelMatrix
                                    );

  /** Frees a pixelMatrix buffer */
  EMSTATUS (*pPixelMatrixFree) (struct DISPLAY_Device_t* device,
                                DISPLAY_PixelMatrix_t pixelMatrix);

  /** Copies the contents of the specified pixelMatrix buffer to the display
      device. */
  EMSTATUS (*pPixelMatrixDraw) (struct DISPLAY_Device_t* device,
                                DISPLAY_PixelMatrix_t pixelMatrix,
                                unsigned int startColumn,
                                unsigned int width,
#ifdef EMWIN_WORKAROUND
                                unsigned int userStride,
#endif
                                unsigned int startRow,
                                unsigned int height);

  /** Clears a pixelMatrix buffer by setting all pixels to black. */
  EMSTATUS (*pPixelMatrixClear) (struct DISPLAY_Device_t* device,
                                 DISPLAY_PixelMatrix_t pixelMatrix,
                                 unsigned int width,
                                 unsigned int height
                                 );

  /** Refreshes the display device driver after system change, like changing
      a clock frequency of some related device. */
  EMSTATUS (*pDriverRefresh) (struct DISPLAY_Device_t* device);

} DISPLAY_Device_t;


/**
 * Display device driver init function pointer type.
 * The displayconfig.h file includes a table that contains the default
 * display devices to initialize.
 */
typedef EMSTATUS (*pDisplayDeviceDriverInitFunction_t) (void);


/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

EMSTATUS DISPLAY_Init          (void);
EMSTATUS DISPLAY_DriverRefresh (void);
EMSTATUS DISPLAY_DeviceGet     (int               displayDeviceNo,
                                DISPLAY_Device_t* device);

#ifdef __cplusplus
}
#endif

/** @} (end group Display) */
/** @} (end group Drivers) */

#endif  /*  _DISPLAY_H_  */
