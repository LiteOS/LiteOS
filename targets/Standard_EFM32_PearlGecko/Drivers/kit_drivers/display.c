/***************************************************************************//**
 * @file display.c
 * @brief Display interface.
 * @version 5.0.0
 *******************************************************************************
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
#include <string.h>

#include "displayconfigall.h"
#include "display.h"
#include "displaybackend.h"


/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 ********************************  STATICS  ************************************
 ******************************************************************************/

/* Table of display devices. */
static DISPLAY_Device_t deviceTable[DISPLAY_DEVICES_MAX];

/* This variable keeps track of whether the DISPLAY module is initialized
   or not. */
static bool moduleInitialized = false;

/* This variable keeps track of the number of devices that have been
    registered. */
static int devicesRegistered=0;

/*
 * Table of display device driver initialization functions to be called in
 * DISPLAY_Init, the DISPLAY module init function.
 */
static pDisplayDeviceDriverInitFunction_t pDisplayDeviceDriverInitFunctions[] =
  DISPLAY_DEVICE_DRIVER_INIT_FUNCTIONS;

/** @endcond */


/*******************************************************************************
 **************************     GLOBAL FUNCTIONS      **************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Initialize the DISPLAY module.
 *
 * @return  EMSTATUS code of the operation.
 ******************************************************************************/
EMSTATUS DISPLAY_Init (void)
{
  EMSTATUS status;

  pDisplayDeviceDriverInitFunction_t* pDisplayDeviceDriverInitFunction =
    pDisplayDeviceDriverInitFunctions;

  if (false == moduleInitialized)
  {
    moduleInitialized = true;
    for (; *pDisplayDeviceDriverInitFunction; pDisplayDeviceDriverInitFunction++)
    {
      status = (*pDisplayDeviceDriverInitFunction)();
      if (DISPLAY_EMSTATUS_OK != status)
        return status;
    }
  }

  return DISPLAY_EMSTATUS_OK;
}


/***************************************************************************//**
 * @brief
 *   Refresh all DISPLAY devices.
 *
 * @details
 *   This function requests all DISPLAY device drivers to update their internal
 *   state with respect to system resource changes, like a bus clock frequency.
 *   This function may need to be called after system changes.
 *
 * @return  EMSTATUS code of the operation.
 ******************************************************************************/
EMSTATUS DISPLAY_DriverRefresh (void)
{
  int        i;
  EMSTATUS status = DISPLAY_EMSTATUS_OK;

  if (false == moduleInitialized)
  {
    status = DISPLAY_EMSTATUS_NOT_INITIALIZED;
  }
  else
  {
    for (i=0; i<devicesRegistered; i++)
    {
      if (deviceTable[i].pDriverRefresh)
      {
        status = (*deviceTable[i].pDriverRefresh)(&deviceTable[i]);
        if (DISPLAY_EMSTATUS_OK != status)
        {
          return status;
        }
      }
    }
  }

  return status;
}


/***************************************************************************//**
 * @brief
 *   Get the display device data structure corresponding to the device number.
 *
 * @param[in] displayDeviceNo
 *   Unique device number of one of the display devices in the system.
 *
 * @param[out] device
 *   Pointer to a <em>DISPLAY_Device_t</em> structure which will be populated
 *   with the properties of the specified display device if the function is
 *   successful.
 *
 * @return  EMSTATUS code of the operation.
 ******************************************************************************/
EMSTATUS DISPLAY_DeviceGet(int                displayDeviceNo,
                           DISPLAY_Device_t*  device)
{
  EMSTATUS status;

  if (false == moduleInitialized)
  {
    status = DISPLAY_EMSTATUS_NOT_INITIALIZED;
  }
  else
  {
    if (displayDeviceNo < DISPLAY_DEVICES_MAX)
    {
      memcpy(device, &deviceTable[displayDeviceNo], sizeof(DISPLAY_Device_t));
      status = DISPLAY_EMSTATUS_OK;
    }
    else
    {
      status = DISPLAY_EMSTATUS_OUT_OF_RANGE;
    }
  }
  return status;
}


/**************************************************************************//**
 * @brief Register a display device
 *
 * @param device  The display device structure which specifies the properties
 *                of the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS DISPLAY_DeviceRegister(DISPLAY_Device_t *device)
{
  EMSTATUS status;

  if (false == moduleInitialized)
  {
    status = DISPLAY_EMSTATUS_NOT_INITIALIZED;
  }
  else
  {
    if (devicesRegistered < DISPLAY_DEVICES_MAX)
    {
      memcpy(&deviceTable[devicesRegistered++], device, sizeof(DISPLAY_Device_t));
      status = DISPLAY_EMSTATUS_OK;
    }
    else
    {
      status = DISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY;
    }
  }
  return status;
}


/***************  THE REST OF THE FILE IS DOCUMENTATION ONLY !  ***************/

/*******************************************************************************
 **************************       DOCUMENTATION       **************************
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Display
 * @brief Display device driver stack library. See @ref display_doc for
 *       more information.
 * @{

@n @section display_doc Display Device Driver Stack

  The source files for the DISPLAY device driver stack library resides in the
  kits/common/drivers directory and follows the naming convention:
  display<em>module_name</em>.c/h.

  @li @ref display_intro
  @li @ref display_getting_started
  @li @ref display_driver_programming
  @li @ref display_config_all

@n @section display_intro Introduction

  The DISPLAY device driver stack implements a common API for different types
  of display devices. Thus making it easy to port applications between
  different hardware and software platforms. Additionally the DISPLAY device
  driver stack implements a Platform Abstraction Layer (PAL) in order to
  provide abstractions of hardware and software services needed by the
  specific display device drivers. Thus making it easy to port the
  DISPLAY device driver stack itself between different platforms.

  The stack provides a configuration interface via dedicated configuration files
  in order to suit various application and platform requirements. Please refer
  to @ref display_config_all for more information.

  There are also several example programs in order to get you started fast.

  We recommend that you read through this documentation, and then proceed to
  build and test a few example programs in order to get started.

@n @section display_getting_started Getting Started

  This section contains brief descriptions of some of the functions in the API.
  You will find detailed information on input and output parameters and return
  values by clicking on the hyperlinked function names. It may also be a good
  idea to study the code in the example programs.

  Your application code must include one header file, @em display.h, which
  defines the user interface of the DISPLAY module which is common for all
  display devices.

  The first step an application should do is to call the <em>
  DISPLAY_Init() </em> function which initializes the DISPLAY driver
  stack. This includes calling
  the initialization functions of all registered display device
  drivers which will initialize the specific display device(s) and make them
  readily available through the DISPLAY interface defined in display.h.

  It is good practice that the user programmer checks the return value of the
  DISPLAY_Init() call, and for the sake of it, all calls in of the DISPLAY
  device driver interface. All functions in the DISPLAY device driver interface
  return an <em>EMSTATUS</em> code which indicates whether the operation was
  successful or not, and what type of error may have occurred. The possible
  error codes are listed in display.h.

  The second step is typically to retrieve the properties of a DISPLAY device.
  This can be done by calling the <em> DISPLAY_DeviceGet() </em> function
  which receives the following two parameters:
  @li <em> displayDeviceNo </em> which is a unique device number of one of the
      existing display devices in the system. Othen this value is zero because
      often there is just one display device in the system.
  @li <em> displayDevice </em> which is a pointer to a <em>DISPLAY_Device_t</em>
      structure which will be populated with the properties of the specified
      display device if the function is successful.

  If <em> DISPLAY_DeviceGet() </em> is successful, the specified
  <em> DISPLAY_Device_t </em> structure contains the properties of the display
  device. The properties include the
  <em> geometry </em> (@ref DISPLAY_Geometry_t),
  the <em> colour mode  </em> (@ref DISPLAY_ColourMode_t) and the
  <em> address mode </em> (@ref DISPLAY_AddressMode_t) of the display
  device. Additionally the  <em> DISPLAY_Device_t </em> structure contains
  function pointers to the device specific functions of the display device.
  The user should be aware that the device driver need not support all the
  function pointers of the <em> DISPLAY_Device_t </em> structure. Therefore
  the user should check if the function pointer is NULL before calling it.

  The <em> pPixelMatrixDraw() </em> function is used to move and
  show the contents of a framebuffer (or pixel matrix buffer) onto the display
  device. A pixel matrix buffer is a 'partial' framebuffer which covers only
  part, or the whole, of the geometry of the display.

  The format of the pixel data in the pixel matrix buffer is defined by the
  @ref DISPLAY_ColourMode_t. At the time of writing only two colour
  modi are defined in @ref DISPLAY_ColourMode_t :
  @li DISPLAY_COLOUR_MODE_MONOCHROME and
  @li DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE

  The DISPLAY_COLOUR_MODE_MONOCHROME mode defines a pixel bit value of 0
  as white, and a pixel bit value of 1 as black.
  The DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE mode is the opposite, and thus
  defines a pixel bit value of 0 as black, and a pixel bit value of 1 as white.

  The pixel matrix buffer format for the monochrome modi is defined as a byte
  array where
  @li bit 0 of the 0th byte is pixel 0 on line 0, top left on the display,
  @li bit 1 of the 0th byte is pixel 1 on line 0,
  @li ...,
  @li bit 7 of the 15th byte is pixel 127 on line 0,
  @li bit 0 of the 16th byte is pixel 0 on line 1,
  @li ...,
  @li bit 7 of the 2047th byte is pixel 127 on line 127, bottom right on the
  display.

  Some device drivers may need/want to support a pixelMatrix allocation
  function, pointed to by <em> pPixelMatrixAllocate </em>, in order to
  handle device specific data structures related to the pixel matrix buffers.
  The pixel matrix buffers allocated with the <em> pPixelMatrixAllocate </em>
  function may also include control data/padding in order to accomodate for
  efficient use of the underlying hardware. If the <em> stride </em> member of
  @ref DISPLAY_Geometry_t structure is bigger than the
  <em> width </em> member, the pixel matrix buffer contains such control data,
  and should not be overwritten by the user, unless the user knows how to
  handle this data (which is usually not necessary).

  The user can make use of graphic libraries such as EMWIN and GLIB in order to
  draw graphical objects in the pixel matrix buffers. Please refer to the
  documentation of EMWIN and/or GLIB in reptile/emwin/src/Doc and reptile/glib
  respectively. The documentation of GLIB is embedded in the source code.

  NOTE:
  There is an issue with EMWIN that does not allow drawing on framebuffers
  of size 128x128 which happens to the size of the Sharp Memory LCD model
  LS013B7DH03. The DISPLAY interface supports a workaround that allows
  the user to allocate bigger framebuffers (pixelMatrix buffers) by using
  the userStride parameter which is included in the DISPLAY interface when
  EMWIN_WORKAROUND is defined. Therefore, in order to use EMWIN on the
  LS013B7DH03, the user must define EMWIN_WORKAROUND (typically in
  displayconfigapp.h) and request a userStride of at least 160 bits, not
  128 bits which is the real width (in pixels) of the LS013B7DH03.

  The <em> pPixelMatrixClear </em> function clears the contents of
  the pixel matrix buffer by setting it all to the default background colour.

  The <em> pDriverRefresh </em> function should be called if the
  system resources. e.g. bus clock frequency, has undergone any changes. The
  <em> pDriverRefresh </em> function will recalibrate internal parameters
  associated with the display devices.

@n @section display_driver_programming DISPLAY Device Driver Programming

  This section contains a brief introduction to writing device drivers for
  specific display devices that can run properly in the DISPLAY device driver
  stack.

  If you want to implement a new DISPLAY device driver, basically you need to
  implement an initialization function that populates a <em>
  DISPLAY_Device_t </em> data structure with the properties of the display
  device. At least the <em> geometry </em> @ref DISPLAY_Geometry_t,
  the <em> colour mode  </em> @ref DISPLAY_ColourMode_t,
  the <em> address mode </em> @ref DISPLAY_AddressMode_t, and the
  <em> pPixelMatrixDraw </em> function must be implemented. Whether the rest of
  the device specific functions may need to be implemented depends on the type
  of display device and application requirements. Also, the device driver
  programmer should be aware that the upper layers and examples of the DISPLAY
  device driver stack does not support all types of display devices and is
  continually being updated for new display devices. Therefore the upper layers
  and examples may need  to be updated in order for existing software to work
  with a new display device driver.

  We recommend to study the existing DISPLAY device driver(s). At the time of
  writing this text, there exists only one DISPLAY device driver for the Sharp
  Memory LCD (model LS013B7DH03) implemented in displayls013b7dh03.c/h.

  When the <em> DISPLAY_Device_t </em> data structure is properly
  populated it should be registered in the DISPLAY module by calling the
  <em> DISPLAY_DeviceRegister </em> function (declared in
  displaybackend.h) with a pointer to
  the DISPLAY_Device_t structure as parameter. This will make the display
  device available via the DISPLAY interface which is used by existing upper
  layer modules, examples and applications directly.

  In order to automatically initialize the new display device driver from
  withing the <em> DISPLAY_Init </em> function, the driver
  initialization function can be added to the list of initialization functions
  in <em> displayconfig.h </em>:
  @verbatim
// Define all display device driver initialization functions here.
#define DISPLAY_DEVICE_DRIVER_INIT_FUNCTIONS \
  {                                          \
    DISPLAY_Ls013b7dh03Init,                 \
    NULL                                     \
  }
  @endverbatim

  The <em> displayconfig.h </em> file should also include \#define inclusion
  constants for the specific display device drivers included in the system.
  Typically there is only one display device, however some systems may add
  more than one display devices if present.

  Additionally, we recommended to implement a platform abstraction layer in
  order to facilitate for easy porting of the display device driver between
  different hardware and software platforms. The <em> displaypal.h </em> file
  declares an interface that abstracts the platform specifics required by the
  Sharp Memory LCD (model LS013B7DH03) device driver implemented in
  displayls013b7dh03.c. And displaypalemlib.c implements the PAL functions on
  top of EMLIB. A new device driver may need additional hardware and software
  services and the displaypal.h can be extended to support any such device.


@n @section display_config_all DISPLAY Device Driver Stack Configuration.

  This section contains a description of the configuration interface of
  the DISPLAY device driver stack. The configuraion interface consists of a
  set of configuration files:
  @li One for each module in the DISPLAY device driver stack, where the
      platform default configurations are specified.
  @li One for each application called <em> displayconfigapp.h </em> in which
      the user can specify application specific configurations and override
      defaults if desired.
  @li And one that ties everything together by including all configuration
      files, called displayconfigall.h (included in display.h).

  Normally the application programmer just need to deal with the application
  specific configuration file  <em> displayconfigapp.h </em> which typically
  should be minimalistic and easy to setup. Below is a list of the typical
  configuration parameters that the application programmer may need to relate
  to on a given platform/kit:

  @verbatim
#define PIXEL_MATRIX_ALLOC_SUPPORT
      Specifies whether the DISPLAY device driver stack should include
      support for allocation of pixel matrices. The user should be aware
      that this may pull in malloc which consumes a relatively large amount
      of memory which is typically not wanted on kits with small amount of
      RAM. The user can define USE_STATIC_PIXEL_MATRIX_POOL in order to
      avoid malloc from being used.

#define USE_STATIC_PIXEL_MATRIX_POOL
      Specifies to use a statically allocated buffer pool to allocate pixel
      matrix buffers from. The user can specify the size of the pixel matrix
      buffer pool with PIXEL_MATRIX_POOL_SIZE.
      NOTE:
      The allocator does not support free'ing pixel matrices. It allocates
      continuosly from the static pool without keeping track of the sizes of
      old allocations. I.e. this is a one-shot allocator, and the  user should
      allocate buffers once at the beginning of the program.

#define PIXEL_MATRIX_POOL_SIZE
      Specifies the size of the static pixel matrix buffer pool. The pool size
      is highly application dependent, and the application programmer should
      consider to minimize this on system with a small amount of RAM.

#define INCLUDE_TEXTDISPLAY_SUPPORT
      Specifies whether to include TEXTDISPLAY support which implements a
      line based text output terminal interface supporting the C language
      stdout (standard output) interface including functions like printf,
      puts, putchar, etc.  The application programmer must select only one
      of the following fonts.

#define TEXTDISPLAY_FONT_6x8
      Select a font which is 6 pixels wide and 8 pixels high, resulting in
      21 column and 16 lines on a 128x128 display.

#define TEXTDISPLAY_FONT_8x8
      Select a font which is 8 pixels wide and 8 pixels high, resulting in
      16 column and 16 lines on a 128x128 display.

#define TEXTDISPLAY_NUMBER_FONT_16x20
      Select a _NUMBERS_ONLY_ font which is 16 pixels wide and 20 pixels high,
      resulting in 8 column and 6 lines on a 128x128 display.
      Note that this font does not include letters. It includes only the
      number characters 0,1,2,3,4,5,6,7,8,9 and additionally the colon ':' sign.
      This font is used by the digital clock mode in the clock example on the
      Zero Gecko starting kit (EFM32ZG_STK3200).

#define INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT
      Include support for some VT52/VT100 escape sequences in order to move the
      cursor around the screen without clearing the existing characters. Please
      refer to the textdisplay.h file for a list of supported escape sequence
      codes.

#define RETARGETTEXTDISPLAY_SCROLL_MODE
      Set to 'true' to enable scroll mode on the text display device where
      stdout is retargeted. Set to 'false' to disable scroll mode.

#define RETARGETTEXTDISPLAY_LINE_FEED_MODE
      Set to 'true' to enable adding Carriage Return (CR) to Line Feed (LF)
      characters on the text display device where stdout is retargeted.
      Set to 'false' to disable line feed mode.

#define PAL_TIMER_REPEAT_FUNCTION
      Specify a function that can register a callback function to be called
      repeatedly at a given frequency. On some platforms, like the
      EFM32ZG_STK3200, the DISPLAY driver Platform Abstraction Layer (PAL)
      uses the RTC to time and toggle the EXTCOMIN pin of the Sharp memory
      LCD per default. However, some applications, like the clock example
      want to use the RTC to keep track of time, i.e. generate interrupt
      every second. Therefore such applications need to support a
      'timer repeat' service function in order to support the PAL and
      take control over the RTC on the application level. E.g. the clock
      example implements a function (RtcIntCallbackRegister)) that
      enables the PAL to register the callback function that needs to be
      called in order to toggle the EXTCOMIN pin.
  @endverbatim

  The rest of this section lists and describes the configuration parameters
  included in the module configuration files which are set to default values
  specific for the kit/platform. Each development kit from Silicon Labs that
  supports the DISPLAY Device driver stack includes a unique set of these
  configuration files in the kits/kit_name/config folder.


  @n @subsection display_config DISPLAY Module Configuration
     This section includes descriptions of the configuration parameters for
     the DISPLAY Module specified in displayconfig.h.

  @verbatim
#define INCLUDE_DISPLAY_SHARP_LS013B7DH03
      Include support for the SHARP Memory LCD model LS013B7DH03.

#define DISPLAY_DEVICES_MAX
      Maximum number of display devices the display module is configured
      to support. This number may be increased if the system includes more
      than one display device. However, the number should be kept low in order
      to save memory.

#define DISPLAY0_WIDTH and DISPLAY0_HEIGHT
      Specifies the geometry of display device #0 in the system. (I.e. on the
      Zero Gecko Kit (EFM32ZG_STK3200) the Sharp Memory LCD is 128x128 pixels
      high and wide, the DISPLAY0_WIDTH and DISPLAY0_HEIGHT should be 128.
      These defines can be used to declare static framebuffers in the
      application in order to save extra memory consumed by malloc.

#define DISPLAY_DEVICE_DRIVER_INIT_FUNCTIONS
      Lists the display device driver initialization function to be called
      automaticallyt by <em> DISPLAY_Init </em>.
  @endverbatim


  @n @subsection display_ls013b7dh03config Sharp Memory LCD Configuration
     This section includes descriptions of the configuration parameters for
     the Sharp Memory LCD Device Driver specified in displayls013b7dh03config.h.

  @verbatim
#define SHARP_MEMLCD_DEVICE_NAME
      Name of display device.

#define LCD_PORT_SCLK and LCD_PIN_SCLK
      Location of SPI clock pin.

#define LCD_PORT_SI and LCD_PIN_SI
      Location of SPI Slave Input pin.

#define LCD_PORT_SCS and LCD_PIN_SCS
      Location of SPI Chip Select pin.

#define LCD_PORT_EXTCOMIN and LCD_PIN_EXTCOMIN
      Location of External COM inversion signal pin.

#define LCD_PORT_DISP_SEL and LCD_PIN_DISP_SEL
      Location of Display on/off pin.

#define LCD_PORT_DISP_PWR and LCD_PIN_DISP_PWR
      Location of Display Power pin (if present on kit).

#define LCD_PORT_EXTMODE and LCD_PIN_EXTMODE
      Location of External COM inversion mode pin.

#define POLARITY_INVERSION_EXTCOMIN
      Select how LCD polarity inversion should be handled.
      If POLARITY_INVERSION_EXTCOMIN is defined, the EXTMODE pin is set to HIGH,
      and the polarity inversion is armed for every rising edge of the EXTCOMIN
      pin. The actual polarity inversion is triggered at the next transision of
      SCS. This mode is recommended because it causes less CPU and SPI load
      than the alternative mode, see below.
      If POLARITY_INVERSION_EXTCOMIN is undefined, the EXTMODE pin is set to
      LOW, and the polarity inversion is toggled by sending an SPI command.
      This mode causes more CPU and SPI load than using the EXTCOMIN pin mode.

#define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
      Define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE if you want the PAL
      (Platform Abstraction Layer interface) to automatically toggle the
      EXTCOMIN pin.
      If the PAL_TIMER_REPEAT function is defined the EXTCOMIN toggling is
      handled by a timer repeat system, then
      POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE should be undefined.
  @endverbatim


  @n @subsection display_palconfig PAL Configuration
     This section includes descriptions of the configuration parameters for
     the PAL (Platform Abstraction Layer) specified in displaypalconfig.h.

  @verbatim
     // The user should select one of the following as  RTC clock source.
#define PAL_RTC_CLOCK_LFXO
      Selects the LFXO oscillator as source for the RTC clock.

#define PAL_RTC_CLOCK_LFRCO
      Selects the LFRCO oscillator as source for the RTC clock.

#define PAL_RTC_CLOCK_ULFRCO
      Selects the ULFRCO oscillator as source for the RTC clock.

#define PAL_SPI_USART_UNIT
      Select which USART device to use as SPI interface.

#define PAL_SPI_USART_CLOCK
      Select which USART clock is used to clock the SPI interface.

#define PAL_SPI_USART_LOCATION
      Route location of the USART/SPI pins.

#define PAL_SPI_BAUDRATE
      Specifies the SPI baud rate.
  @endverbatim

  @n @subsection display_textdisplayconfig TEXTDISPLAY Configuration
    The configuration of the TEXTDISPLAY and RETARGETTEXTDISPLAY is described
    in @ref textdisplay_doc and @ref retargettextdisplay_doc.

***************************************************************************/

/** @} (end group Display) */
