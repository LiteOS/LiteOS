/**************************************************************************//**
 * @file displayls013b7dh03.c
 * @brief Display driver for the Sharp Memory LCD LS013B7DH03
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



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "em_gpio.h"

/* DISPLAY driver inclustions */
#include "displayconfigall.h"
#include "displaypal.h"
#include "displaybackend.h"
#include "displayls013b7dh03.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/* LS013B7DH03 SPI commands */
#define LS013B7DH03_CMD_UPDATE     (0x01)
#define LS013B7DH03_CMD_ALL_CLEAR  (0x04)

/* Frequency of LCD polarity inversion. */
#ifndef LS013B7DH03_POLARITY_INVERSION_FREQUENCY
#define LS013B7DH03_POLARITY_INVERSION_FREQUENCY (128)
#endif

#ifdef USE_CONTROL_BYTES
#define LS013B7DH03_CONTROL_BYTES     (2)
#else
#define LS013B7DH03_CONTROL_BYTES     (0)
#endif

#ifdef PIXEL_MATRIX_ALLOC_SUPPORT

  #ifdef USE_STATIC_PIXEL_MATRIX_POOL
    /* Static pool has been chosen for pixelmatix allocation.
       Disable the use of malloc for allocation of pixel matrices. */
    #undef USE_MALLOC
  #endif

  #ifdef USE_MALLOC
    /* malloc has been chosen for pixelmatix allocation.
       Disable the use of static pool for allocation of pixel matrices. */
    #undef USE_STATIC_PIXEL_MATRIX_POOL
  #endif

#endif /*  PIXEL_MATRIX_ALLOC_SUPPORT  */


/*******************************************************************************
 *********************************  TYPEDEFS  **********************************
 ******************************************************************************/

#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
#ifndef PIXEL_MATRIX_ALIGNMENT
typedef uint8_t PixelMatrixAlign_t;
#else
  #if (1 == PIXEL_MATRIX_ALIGNMENT)
    typedef uint8_t PixelMatrixAlign_t;
  #elif (2 == PIXEL_MATRIX_ALIGNMENT)
    typedef uint16_t PixelMatrixAlign_t;
  #elif (4 == PIXEL_MATRIX_ALIGNMENT)
    typedef uint32_t PixelMatrixAlign_t;
  #else
    #error Unsupported PIXEL_MATRIX_ALIGNMENT.
  #endif
#endif
#endif


/*******************************************************************************
 ********************************  STATICS  ************************************
 ******************************************************************************/

/* Static variables: */
static uint8_t        lcdPolarity = 0;

#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
#ifdef USE_STATIC_PIXEL_MATRIX_POOL
#define PIXEL_MATRIX_POOL_ELEMENTS                                \
  (PIXEL_MATRIX_POOL_SIZE/sizeof(PixelMatrixAlign_t) +            \
   ((PIXEL_MATRIX_POOL_SIZE%sizeof(PixelMatrixAlign_t))? 1 : 0))
static PixelMatrixAlign_t  pixelMatrixPoolBase[PIXEL_MATRIX_POOL_ELEMENTS];
static PixelMatrixAlign_t* pixelMatrixPool = pixelMatrixPoolBase;
#endif
#endif


/*******************************************************************************
 ************************   STATIC FUNCTION PROTOTYPES   ***********************
 ******************************************************************************/

static EMSTATUS DisplayEnable(DISPLAY_Device_t*     device,
                              bool enable);
static EMSTATUS DisplayClear(void);
#ifndef POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
static EMSTATUS DisplayPolarityInverse (void);
#endif

#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
static EMSTATUS PixelMatrixAllocate( DISPLAY_Device_t*     device,
                                     unsigned int           width,
#ifdef EMWIN_WORKAROUND
                                     unsigned int           userStride,
#endif
                                     unsigned int           height,
                                     DISPLAY_PixelMatrix_t *pixelMatrix);
static EMSTATUS PixelMatrixFree( DISPLAY_Device_t*     device,
                                 DISPLAY_PixelMatrix_t pixelMatrix);
#endif
static EMSTATUS PixelMatrixDraw( DISPLAY_Device_t*     device,
                                 DISPLAY_PixelMatrix_t pixelMatrix,
                                 unsigned int          startColumn,
                                 unsigned int          width,
#ifdef EMWIN_WORKAROUND
                                 unsigned int          userStride,
#endif
                                 unsigned int          startRow,
                                 unsigned int          height );
static EMSTATUS PixelMatrixClear( DISPLAY_Device_t*      device,
                                  DISPLAY_PixelMatrix_t  pixelMatrix,
                                  unsigned int           width,
                                  unsigned int           height);
static EMSTATUS DriverRefresh (DISPLAY_Device_t* device);


/*******************************************************************************
 **************************     GLOBAL FUNCTIONS      **************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief  Initialize the LS013B7DH03 display driver
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS DISPLAY_Ls013b7dh03Init(void)
{
  DISPLAY_Device_t      display;
  EMSTATUS              status;

  /* Initialize the Platform Abstraction Layer (PAL) interface.  */
  PAL_TimerInit();
  PAL_SpiInit();
  PAL_GpioInit();

  /* Setup GPIOs */
  PAL_GpioPinModeSet(LCD_PORT_SCLK,    LCD_PIN_SCLK,    palGpioModePushPull,0);
  PAL_GpioPinModeSet(LCD_PORT_SI,      LCD_PIN_SI,      palGpioModePushPull,0);
  PAL_GpioPinModeSet(LCD_PORT_SCS,     LCD_PIN_SCS,     palGpioModePushPull,0);
#if defined( LCD_PORT_DISP_SEL  )
  PAL_GpioPinModeSet(LCD_PORT_DISP_SEL,LCD_PIN_DISP_SEL,palGpioModePushPull,0);
#endif

#if defined( LCD_PORT_DISP_PWR )
  PAL_GpioPinModeSet(LCD_PORT_DISP_PWR,LCD_PIN_DISP_PWR,palGpioModePushPull,0);
#endif

#if defined( LCD_PORT_EXTMODE )
  PAL_GpioPinModeSet(LCD_PORT_EXTMODE, LCD_PIN_EXTMODE, palGpioModePushPull,0);
#endif
#if defined (LCD_PORT_EXTCOMIN)
  PAL_GpioPinModeSet(LCD_PORT_EXTCOMIN,LCD_PIN_EXTCOMIN,palGpioModePushPull,0);
#endif
#ifdef PAL_TIMER_REPEAT_FUNCTION
  /* If the platform specifies to use a timer repeat function we should
     register the DisplayPolarityInverse to be called every second in
     order to toggle the EXTCOMIN pin at 1Hz.
  */
  status =
    PAL_TimerRepeat((void(*)(void*)) DisplayPolarityInverse, 0,
                    LS013B7DH03_POLARITY_INVERSION_FREQUENCY);
#elif defined POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
  /* Setup system (via PAL) to toggle the EXTCOMIN pin every second. */
  status = PAL_GpioPinAutoToggle(LCD_PORT_EXTCOMIN, LCD_PIN_EXTCOMIN,
                                 LS013B7DH03_POLARITY_INVERSION_FREQUENCY);
#else
  /* System does not support toggling the EXTCOMIN pin. Return error. */
  return DISPLAY_EMSTATUS_NOT_SUPPORTED;
#endif
  if (PAL_EMSTATUS_OK != status)
  {
    return status;
  }

  /* Setup and register the LS013B7DH03 as a DISPLAY device now. */
  display.name                  = SHARP_MEMLCD_DEVICE_NAME;
  display.colourMode            = DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE;
  display.addressMode           = DISPLAY_ADDRESSING_BY_ROWS_ONLY;
  display.geometry.width        = LS013B7DH03_WIDTH;
  display.geometry.height       = LS013B7DH03_HEIGHT;
  /* stride = pixels + ctrl bytes */
  display.geometry.stride       =
    display.geometry.width + LS013B7DH03_CONTROL_BYTES*8;

  display.pDisplayPowerOn       = DisplayEnable;
#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
  display.pPixelMatrixAllocate  = PixelMatrixAllocate;
  display.pPixelMatrixFree      = PixelMatrixFree;
#else
  display.pPixelMatrixAllocate  = NULL;
  display.pPixelMatrixFree      = NULL;
#endif
  display.pPixelMatrixDraw      = PixelMatrixDraw;
  display.pPixelMatrixClear     = PixelMatrixClear;
  display.pDriverRefresh        = DriverRefresh;

  status = DISPLAY_DeviceRegister (&display);

  if (DISPLAY_EMSTATUS_OK == status)
  {
    /* Turn on display. */
    DisplayEnable(&display, true);

    /* Clear display */
    DisplayClear();
  }

  return status;
}


/*******************************************************************************
 *****************************   STATIC FUNCTIONS   ****************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief  Refresh the LS013B7DH03 display device.
 *
 * @param[in] device  Display device pointer.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS DriverRefresh(DISPLAY_Device_t* device)
{
  EMSTATUS      status = DISPLAY_EMSTATUS_OK;

  (void) device; /* Suppress compiler warning: unused parameter. */

  /* Reinitialize the timer and SPI configuration.  */
  PAL_TimerInit();
  PAL_SpiInit();

  return status;
}


/**************************************************************************//**
 * @brief  Enable or disable the display.
 *
 * @detail  This function enables or disables the display. Disabling the
 *          display does not make it lose it's data.
 *
 * @param[in] device  Display device pointer.
 *
 * @param[in] enable  Set this parameter to 'true' to enable the display.
 *                    Set to 'false' in order to disable the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS DisplayEnable(DISPLAY_Device_t* device,
                              bool              enable)
{
  (void) device; /* Suppress compiler warning: unused parameter. */

  if (enable)
  {
#if defined( LCD_PORT_DISP_SEL )
    /* Set EFM_DISP_SELECT pin. */
    PAL_GpioPinOutSet(LCD_PORT_DISP_SEL, LCD_PIN_DISP_SEL);
#endif

#if defined( LCD_PORT_DISP_PWR )
    /* Drive voltage on EFM_DISP_PWR_EN pin. */
    PAL_GpioPinOutSet(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
#endif
  }
  else
  {
#if defined( LCD_PORT_DISP_PWR )
    /* Stop driving voltage on EFM_DISP_PWR_EN pin. */
    PAL_GpioPinOutClear(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
#endif

#if defined( LCD_PORT_DISP_SEL )
    /* Clear EFM_DISP_SELECT pin. */
    PAL_GpioPinOutClear(LCD_PORT_DISP_SEL, LCD_PIN_DISP_SEL);
#endif
  }

  return DISPLAY_EMSTATUS_OK;
}


/**************************************************************************//**
 * @brief  Clear the display.
 *
 * @detail  This function clears the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS DisplayClear ( void )
{
  uint16_t cmd;

  /* Set SCS */
  PAL_GpioPinOutSet( LCD_PORT_SCS, LCD_PIN_SCS );

  /* SCS setup time: min 6us */
  PAL_TimerMicroSecondsDelay(6);

  /* Send command */
  cmd = LS013B7DH03_CMD_ALL_CLEAR | lcdPolarity;
  PAL_SpiTransmit ((uint8_t*) &cmd, 2 );

  /* SCS hold time: min 2us */
  PAL_TimerMicroSecondsDelay(2);

  /* Clear SCS */
  PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );

  return DISPLAY_EMSTATUS_OK;
}


#ifdef PAL_TIMER_REPEAT_FUNCTION

/**************************************************************************//**
 * @brief   Inverse polarity across the Liquid Crystal cells in the display.
 *
 * @detail  This function inverses the polarity across the Liquid Crystal cells
 *          in the LCD. Must be called at least every second.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS DisplayPolarityInverse (void)
{
#ifdef POLARITY_INVERSION_EXTCOMIN

#if defined(LCD_PORT_EXTCOMIN)
  /* Toggle extcomin gpio */
  PAL_GpioPinOutToggle( LCD_PORT_EXTCOMIN, LCD_PIN_EXTCOMIN );
#endif
#else /* POLARITY_INVERSION_EXTCOMIN */

  /* Send a packet with inverted com */
  PAL_GpioPinOutSet( LCD_PORT_SCS, LCD_PIN_SCS );

  /* SCS setup time: min 6us */
  PAL_TimerMicroSecondsDelay(6);

  /* Send polarity command including dummy bits */
  PAL_SpiTransmit ((uint8_t*) &lcdPolarity, 2 );

  /* SCS hold time: min 2us */
  PAL_TimerMicroSecondsDelay(2);

  PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );

  /* Invert com polarity */
  if (lcdPolarity == 0x00)
  {
    lcdPolarity = 0x02;
  }
  else
  {
    lcdPolarity = 0x00;
  }

#endif /* POLARITY_INVERSION_EXTCOMIN */

  return DISPLAY_EMSTATUS_OK;
}

#endif /* POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE */


#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
/**************************************************************************//**
 * @brief   Allocate a pixel matrix buffer for the display.
 *
 * @detail  This function allocates a pixel matrix buffer which is specifically
 *          associated with the specific display device model.
 *          The returned pixel matrix buffer is in the format specified by the
 *          DISPLAY_Geometry_t data structure associated with the pixelMatrix
 *          device.
 *
 * @param[in]  device       Display device pointer.
 * @param[in]  width        Width in pixel columns of the pixel matrix buffer
 *                          to allocated. Must be max width of display if the
 *                          addresssing mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in]  height       Height in pixel rows/lines of the pixel matrix.
 * @param[out] pixelMatrix  Pointer to the pixel matrix buffer to draw.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS PixelMatrixAllocate( DISPLAY_Device_t*      device,
                                     unsigned int           width,
#ifdef EMWIN_WORKAROUND
                                     unsigned int           userStride,
#endif
                                     unsigned int           height,
                                     DISPLAY_PixelMatrix_t *pixelMatrix)
{
#ifdef EMWIN_WORKAROUND
  unsigned int allocSize = (userStride/8 + LS013B7DH03_CONTROL_BYTES) * height;
#else
  unsigned int allocSize = (width/8 + LS013B7DH03_CONTROL_BYTES) * height;
#endif

  (void) device; /* Suppress compiler warning: unused parameter. */

  if (width != LS013B7DH03_WIDTH)
    return DISPLAY_EMSTATUS_OUT_OF_RANGE;
#ifdef EMWIN_WORKAROUND
  if (userStride < width)
    return DISPLAY_EMSTATUS_INVALID_PARAMETER;
#endif

#ifdef USE_MALLOC

  /* Allocate the pixel matrix buffer including 2 control bytes per line. */
  *pixelMatrix = (DISPLAY_PixelMatrix_t) malloc (allocSize);

  if (NULL == *pixelMatrix)
    return DISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY;
  else
    return DISPLAY_EMSTATUS_OK;

#endif /* USE_MALLOC */

#ifdef USE_STATIC_PIXEL_MATRIX_POOL

  if (((uint8_t*)pixelMatrixPool)     + allocSize >
      ((uint8_t*)pixelMatrixPoolBase) + PIXEL_MATRIX_POOL_SIZE)
  {
    *pixelMatrix     = NULL;
    return DISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY;
  }
  else
  {
    *pixelMatrix     = pixelMatrixPool;
    pixelMatrixPool += allocSize / sizeof(PixelMatrixAlign_t) +
      ((allocSize % sizeof(PixelMatrixAlign_t))? 1 : 0);
    return DISPLAY_EMSTATUS_OK;
  }

#endif /* USE_STATIC_PIXEL_MATRIX_POOL */

}
#endif /* PIXEL_MATRIX_ALLOC_SUPPORT */


#ifdef PIXEL_MATRIX_ALLOC_SUPPORT
/**************************************************************************//**
 * @brief   Free a pixel matrix buffer.
 *
 * @detail  This function frees a pixel matrix buffer.
 *
 * @param[in] device       Display device pointer.
 * @param[in] pixelMatrix  Handle to pixel matrix buffer to clear.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS PixelMatrixFree( DISPLAY_Device_t*     device,
                                 DISPLAY_PixelMatrix_t pixelMatrix)
{
  (void) device; /* Suppress compiler warning: unused parameter. */

#ifdef USE_MALLOC
  free(pixelMatrix);
  return DISPLAY_EMSTATUS_OK;
#endif /* USE_MALLOC */

#ifdef USE_STATIC_PIXEL_MATRIX_POOL
  /* The non-malloc PixelMatrixAllocate function only allocates buffers
     consequtively from a pool. It is not possible to free the buffers.
     I.e. this allocator can only be used for one-shot allocations of
     buffers that will should never be freed and re-alloced.
  */
  (void) pixelMatrix;   /* Supress compiles warning: unused parameter. */
  return DISPLAY_EMSTATUS_NOT_SUPPORTED;
#endif
}
#endif /* PIXEL_MATRIX_ALLOC_SUPPORT */


/**************************************************************************//**
 * @brief   Clear a pixel matrix buffer.
 *
 * @detail  This function clears the pixel data of a pixel matrix buffer by
 *          setting all pixels to 0, i.e. all pixels will be blank if the
 *          buffer is drawn on the display after this operation.
 *          The control words of the pixel matrix are not touched.
 *
 * @param[in]  device       Display device pointer.
 * @param[in]  pixelMatrix  Handle to pixel matrix buffer to clear.
 * @param[in]  width        Width in pixel columns of the pixel matrix buffer
 *                          to clear. Must be max width of display if the
 *                          addresssing mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in]  height       Height in pixel rows/lines of the pixel matrix.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS PixelMatrixClear( DISPLAY_Device_t*      device,
                                  DISPLAY_PixelMatrix_t  pixelMatrix,
                                  unsigned int           width,
                                  unsigned int           height)
{
  uint8_t*       pByte = (uint8_t*) pixelMatrix;
  unsigned int   i;

  (void) device; /* Suppress compiler warning: unused parameter. */
  (void) width;  /* Suppress compiler warning: unused parameter. */

  for (i=0; i<height; i++)
  {
    /* Clear line */
    memset(pByte, 0, LS013B7DH03_WIDTH/8);
    pByte += LS013B7DH03_WIDTH/8;

#ifdef USE_CONTROL_BYTES
    /* Set dummy byte. */
    *pByte++ = 0xff;
    /* Set address of next line */
    *pByte++ = i+1;
#endif
  }

  return DISPLAY_EMSTATUS_OK;
}


#ifdef USE_CONTROL_BYTES
/**************************************************************************//**
 * @brief   Set control words of a pixel matrix buffer.
 *
 * @detail  This function sets the control words of a pixel matrix buffer.
 *
 * @param[in]  pixelMatrix  Handle to pixel matrix buffer to clear.
 * @param[in]  startRow     Number of first row.
 * @param[in]  height       Height in pixel rows/lines of the pixel matrix.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS pixelMatrixSetup( DISPLAY_PixelMatrix_t  pixelMatrix,
                                  unsigned int           startRow,
                                  unsigned int           height
#ifdef EMWIN_WORKAROUND
                                  ,
                                  unsigned int           userStride
#endif
                                  )
{
  int       i          = 0;
  uint8_t*  pByte      = (uint8_t*) pixelMatrix;
#ifdef EMWIN_WORKAROUND
  int       strideGap  =
    (userStride-LS013B7DH03_WIDTH-(LS013B7DH03_CONTROL_BYTES*8)) /
    8 / sizeof(uint8_t);
  if ((userStride-LS013B7DH03_WIDTH) % sizeof(uint16_t))
    return DISPLAY_EMSTATUS_INVALID_PARAMETER;
#endif

  while (i<height)
  {
    pByte += LS013B7DH03_WIDTH/8;
    /* Set dummy byte. */
    *pByte++ = 0xff;

    if (i == height-1)
    {
      /* Set dummy data at end of last line. */
      *pByte++ = 0xff;
      break;
    }
    else
      /* Set address of next line */
      *pByte++ = startRow + (++i);

#ifdef EMWIN_WORKAROUND
    pByte += strideGap;
#endif
  }

  return DISPLAY_EMSTATUS_OK;
}

#endif /* USE_CONTROL_BYTES */


/**************************************************************************//**
 * @brief Move and show the contents of a pixel matrix buffer onto the display.
 *
 * @param[in] device       Display device pointer.
 * @param[in] pixelMatrix  Pointer to the pixel matrix buffer to draw.
 * @param[in] startColumn  Start column on the display where to start
 *                         drawing the pixel matrix. Must be 0 if addressing
 *                         mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in] width        Width in pixel columns of the pixel matrix to draw
 *                         Must be max width of display if the addresssing
 *                         mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in] startRow     Start row on the display where to start drawing
 *                         the pizel matrix.
 * @param[in] height       Height in pixel rows/lines of the pixel matrix.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS PixelMatrixDraw( DISPLAY_Device_t*      device,
                                 DISPLAY_PixelMatrix_t  pixelMatrix,
                                 unsigned int           startColumn,
                                 unsigned int           width,
#ifdef EMWIN_WORKAROUND
                                 unsigned int           userStride,
#endif
                                 unsigned int           startRow,
                                 unsigned int           height )
{
  unsigned int i;
  uint16_t*    p = (uint16_t *)pixelMatrix;
  uint16_t     cmd;
#ifdef EMWIN_WORKAROUND
  int          strideGap =
    (userStride-width-(LS013B7DH03_CONTROL_BYTES*8)) / 8 / sizeof(uint16_t);
  if ((userStride-width) % sizeof(uint16_t))
    return DISPLAY_EMSTATUS_INVALID_PARAMETER;
#else
  (void) width;  /* Suppress compiler warning: unused parameter. */
#endif
  (void) startColumn;  /* Suppress compiler warning: unused parameter. */
  (void) device; /* Suppress compiler warning: unused parameter. */

  /* Need to adjust start row by one because LS013B7DH03 starts counting lines
     from 1, while the DISPLAY interface starts from 0. */
  startRow++;

#ifdef USE_CONTROL_BYTES
  /* Setup line addressing in control words. */
  pixelMatrixSetup(pixelMatrix, startRow, height
#ifdef EMWIN_WORKAROUND
                   , userStride
#endif
                   );
#endif

  /* Assert SCS */
  PAL_GpioPinOutSet( LCD_PORT_SCS, LCD_PIN_SCS );

  /* SCS setup time: min 6us */
  PAL_TimerMicroSecondsDelay(6);

  /* Send update command and first line address */
  cmd = LS013B7DH03_CMD_UPDATE | (startRow << 8);
  PAL_SpiTransmit((uint8_t*) &cmd, 2 );

  /* Get start address to draw from */
  for ( i=0; i<height; i++ ) {

    /* Send pixels for this line */
    PAL_SpiTransmit((uint8_t*) p,
                    LS013B7DH03_WIDTH/8 + LS013B7DH03_CONTROL_BYTES);
    p+=(LS013B7DH03_WIDTH/8 + LS013B7DH03_CONTROL_BYTES) / sizeof(uint16_t);

#ifndef USE_CONTROL_BYTES
    if (i==height-1)
    {
      cmd = 0xffff;
    }
    else
    {
      cmd = 0xff | ((startRow+i+1) << 8);
    }
    PAL_SpiTransmit((uint8_t*) &cmd, 2 );
#endif

#ifdef EMWIN_WORKAROUND
    p += strideGap;
#endif

  }

  /* SCS hold time: min 2us */
  PAL_TimerMicroSecondsDelay(2);

  /* De-assert SCS */
  PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );

  return DISPLAY_EMSTATUS_OK;
}

/** @endcond */
