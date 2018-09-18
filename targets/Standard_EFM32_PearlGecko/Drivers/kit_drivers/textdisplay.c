/***************************************************************************//**
 * @file textdisplay.c
 * @brief Provide stdio retargeting to the text display interface.
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "displayconfigall.h"
#include "display.h"
#include "textdisplay.h"

#ifdef TEXTDISPLAY_FONT_8x8
#include "displayfont8x8.h"
#define FONT_ASCII_START  (' ')
#define FONT_CHARACTERS   (100)
#define FONT_BITS_MASK    (0x7)
#define FONT_BITS_LOG2      (3)
#define fontBits chars_8x8_bits
#endif

#ifdef TEXTDISPLAY_FONT_6x8
#include "displayfont6x8.h"
#define FONT_ASCII_START  (' ')
#define FONT_CHARACTERS   (100)
#define FONT_BITS_MASK    (0x7)
#define FONT_BITS_LOG2      (3)
#define fontBits chars_6x8_bits
#endif

#ifdef TEXTDISPLAY_NUMBER_FONT_16x20
#include "displayfont16x20.h"
#define FONT_ASCII_START  ('0')
#define FONT_CHARACTERS    (12)
#define FONT_BITS_MASK    (0xF)
#define FONT_BITS_LOG2      (4)
#define fontBits numbers_16x20_bits
#endif


/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 *******************************  TYPEDEFS   ***********************************
 ******************************************************************************/

typedef enum TEXTDISPLAY_UpdateMode_t
{
  TEXTDISPLAY_UPDATE_MODE_FULL,
  TEXTDISPLAY_UPDATE_MODE_LINE,
  TEXTDISPLAY_UPDATE_MODE_CHAR,
  TEXTDISPLAY_UPDATE_MODE_NONE
} TEXTDISPLAY_UpdateMode_t;


typedef struct TEXTDISPLAY_Device_t
{
  int               displayDeviceNo;    /* Display device number to initialize
                                           test display on. */
  DISPLAY_Device_t  displayDevice;      /* Display properties. */

  unsigned int      columns;            /* Number of columns to use. */
  unsigned int      lines;              /* Number of lines to use. */

  bool              scrollEnable;       /* Enable/disable scrolling mode on the
                                           text display. Scrolling mode will
                                           consume more memory because the lines
                                           lines will have to be stored in
                                           memory. */

  bool                     lfToCrLf;    /**< Enable or disable LF to CR+LF
                                           conversion. */

  DISPLAY_PixelMatrix_t    lineBuffer;  /**< Line buffer for display devices
                                           that only support addressing by
                                           rows/lines. */
  char*                    charBuffer;  /**< Character buffer */
  char**                   charArray;   /**< Character pointer array */

  uint8_t                  rgbColor[3]; /**< Color for font */

  unsigned int             xpos;        /**< X Cursor position */
  unsigned int             ypos;        /**< Y Cursor position */

  TEXTDISPLAY_UpdateMode_t updateMode;  /**< Display update mode. */

  bool                     initialized; /**< Flag to indicate that the display
                                           is intialized. */
} TEXTDISPLAY_Device_t;


typedef struct TEXTDISPLAY_CharBuffers_t
{
  unsigned int    lines;         /**< Number of lines to use. */
  unsigned int    columns;       /**< Number of columns to use. */

  char*           charBuffer;    /**< Character buffer */
  char**          charArray;     /**< Character pointer array */
} TEXTDISPLAY_CharBuffers_t;


/*******************************************************************************
 *******************************   STATICS   ***********************************
 ******************************************************************************/

/* Table of text display device structures. */
static TEXTDISPLAY_Device_t textdisplayTbl[TEXTDISPLAY_DEVICES_MAX];


/* Static Character buffers */
char  charBufferDevice0[TEXTDISPLAY_DEVICE_0_LINES * TEXTDISPLAY_DEVICE_0_COLUMNS];
char* charArrayDevice0[TEXTDISPLAY_DEVICE_0_LINES];

#if (TEXTDISPLAY_DEVICES_MAX == 2)
char  charBufferDevice1[TEXTDISPLAY_DEVICE_1_LINES * TEXTDISPLAY_DEVICE_1_COLUMNS];
char* charArrayDevice1[TEXTDISPLAY_DEVICE_1_LINES];
#endif

#if (TEXTDISPLAY_DEVICES_MAX == 3)
char  charBufferDevice2[TEXTDISPLAY_DEVICE_2_LINES * TEXTDISPLAY_DEVICE_2_COLUMNS];
char* charArrayDevice2[TEXTDISPLAY_DEVICE_2_LINES];
#endif

#if (TEXTDISPLAY_DEVICES_MAX == 4)
char  charBufferDevice3[TEXTDISPLAY_DEVICE_3_LINES * TEXTDISPLAY_DEVICE_3_COLUMNS];
char* charArrayDevice3[TEXTDISPLAY_DEVICE_3_LINES];
#endif


static TEXTDISPLAY_CharBuffers_t charBufferTbl[TEXTDISPLAY_DEVICES_MAX] =
  {
    {
      TEXTDISPLAY_DEVICE_0_LINES,
      TEXTDISPLAY_DEVICE_0_COLUMNS,
      charBufferDevice0,
      charArrayDevice0
    },
#if (TEXTDISPLAY_DEVICES_MAX == 2)
    {
      TEXTDISPLAY_DEVICE_1_LINES,
      TEXTDISPLAY_DEVICE_1_COLUMNS,
      charBufferDevice1,
      charArrayDevice1
    },
#endif
#if (TEXTDISPLAY_DEVICES_MAX == 3)
    {
      TEXTDISPLAY_DEVICE_2_LINES,
      TEXTDISPLAY_DEVICE_2_COLUMNS,
      charBufferDevice2,
      charArrayDevice2
    },
#endif
#if (TEXTDISPLAY_DEVICES_MAX == 4)
    {
      TEXTDISPLAY_DEVICE_3_LINES,
      TEXTDISPLAY_DEVICE_3_COLUMNS,
      charBufferDevice3,
      charArrayDevice3
    },
#endif
  };


/*******************************************************************************
 ************************   STATIC FUNCTION PROTOTYPES   ***********************
 ******************************************************************************/

/* Static functions: */
static void TextdisplayClear    (TEXTDISPLAY_Device_t*  textdisplay);
static void TextdisplayScrollUp (TEXTDISPLAY_Device_t*  textdisplay);
static void TextdisplayCharAdd  (TEXTDISPLAY_Device_t*  textdisplay,
                                 int                    c);
static EMSTATUS TextdisplayLineDraw (TEXTDISPLAY_Device_t*  textdisplay,
                                     unsigned int           y);
static EMSTATUS TextdisplayUpdate   (TEXTDISPLAY_Device_t*  textdisplay);

/** @endcond */


/*******************************************************************************
 **************************     GLOBAL FUNCTIONS      **************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief Create a new text display device.
 *
 * @param[in]  config  Configuration data structure for the text display device
 *                     to create.
 * @param[out] handle  Pointer to text display handle which will be returned
 *                     if the function is successful.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS TEXTDISPLAY_New(TEXTDISPLAY_Config_t  *config,
                         TEXTDISPLAY_Handle_t  *handle)
{
  TEXTDISPLAY_Device_t*    textdisplay;
  int                      deviceNo;
  unsigned int             i;
  EMSTATUS                 status;

  /* Find unused text display device structure. */
  for (textdisplay=NULL,
         deviceNo=0; deviceNo<TEXTDISPLAY_DEVICES_MAX; deviceNo++)
  {
    if (false == textdisplayTbl[deviceNo].initialized)
    {
      textdisplay = &textdisplayTbl[deviceNo];
      break;
    }
  }
  if (NULL == textdisplay)
  {
    return TEXTDISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY;
  }

  /* Retrieve the properties of the DISPLAY. */
  status =
    DISPLAY_DeviceGet(config->displayDeviceNo, &textdisplay->displayDevice);
  if (DISPLAY_EMSTATUS_OK != status)
    return status;

  /* If the display address mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY we need
     a buffer to draw the current line with new characters as they arrive. */
  if (DISPLAY_ADDRESSING_BY_ROWS_ONLY == textdisplay->displayDevice.addressMode)
  {
    status =
      textdisplay->displayDevice.
      pPixelMatrixAllocate (&textdisplay->displayDevice,
                            textdisplay->displayDevice.geometry.width,
#ifdef EMWIN_WORKAROUND
                            textdisplay->displayDevice.geometry.width,
#endif
                            FONT_HEIGHT,
                            &textdisplay->lineBuffer);
    if (DISPLAY_EMSTATUS_OK != status)
      return status;

    status =
      textdisplay->displayDevice.
      pPixelMatrixClear (&textdisplay->displayDevice,
                         textdisplay->lineBuffer,
                         textdisplay->displayDevice.geometry.width,
                         FONT_HEIGHT);

    if (DISPLAY_EMSTATUS_OK != status)
      return status;
  }
  else
  {
    textdisplay->lineBuffer = NULL;
  }

  /* Check that the requested number of text rows and columns fits inside the
     geometry of the display device. */
  if (charBufferTbl[deviceNo].lines >
      textdisplay->displayDevice.geometry.height / FONT_HEIGHT)
    return TEXTDISPLAY_EMSTATUS_OUT_OF_RANGE;
  if (charBufferTbl[deviceNo].columns >
      textdisplay->displayDevice.geometry.width / FONT_WIDTH)
    return TEXTDISPLAY_EMSTATUS_OUT_OF_RANGE;

  /* Remember the character buffer parameters. */
  textdisplay->lines      = charBufferTbl[deviceNo].lines;
  textdisplay->columns    = charBufferTbl[deviceNo].columns;
  textdisplay->charBuffer = charBufferTbl[deviceNo].charBuffer;
  textdisplay->charArray  = charBufferTbl[deviceNo].charArray;

  /* Store user configuration options */
  textdisplay->displayDeviceNo   = config->displayDeviceNo;
  textdisplay->scrollEnable      = config->scrollEnable;
  textdisplay->lfToCrLf          = config->lfToCrLf;

  /* Setup char pointer array */
  for (i=0; i<textdisplay->lines; i++)
    textdisplay->charArray[i] =
      textdisplay->charBuffer + textdisplay->columns * i;

  /* Initialize color for text */
  /* Use \b for red text (bell/warning) */
  textdisplay->rgbColor[0]       = 0xff;
  textdisplay->rgbColor[1]       = 0xff;
  textdisplay->rgbColor[2]       = 0xff;

  /* Clear character buffer */
  TextdisplayClear(textdisplay);

  textdisplay->initialized      = true;

  *handle = (TEXTDISPLAY_Handle_t*) textdisplay;

  return TEXTDISPLAY_EMSTATUS_OK;
}


/**************************************************************************//**
 * @brief Delete a text display device.
 *
 * @details Delete a text display device and release all resources associataed
 *          with the text display device.
 *
 * @param[in] handle  Handle to the text display device to delete.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS TEXTDISPLAY_Delete(TEXTDISPLAY_Handle_t  handle)
{
  TEXTDISPLAY_Device_t*  textdisplay = (TEXTDISPLAY_Device_t*) handle;

  if (textdisplay->initialized)
  {
    textdisplay->charBuffer = NULL;
    textdisplay->charArray = NULL;

    if (textdisplay->lineBuffer)
    {
      textdisplay->displayDevice.pPixelMatrixFree (&textdisplay->displayDevice,
                                                   textdisplay->lineBuffer);
      textdisplay->lineBuffer = NULL;
    }

    /* Clear display device number to allow reinitialization of this display.*/
    textdisplay->displayDeviceNo = -1;

    textdisplay->initialized = false;

    return TEXTDISPLAY_EMSTATUS_OK;
  }

  return TEXTDISPLAY_EMSTATUS_INVALID_PARAM;
}


/**************************************************************************//**
 * @brief Enable or disable LF to CR+LF conversion
 *
 * @param[in] handle  Text display handle associated with the text display to
 *                    write the character on.
 * @param[in] on
 *     If true, automatic LF to CRLF conversion will be enabled.
 *     If false, LF will only result in a line feed on the terminal.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS TEXTDISPLAY_LfToCrLf(TEXTDISPLAY_Handle_t  handle,
                              bool                  on)
{
  TEXTDISPLAY_Device_t*  textdisplay = (TEXTDISPLAY_Device_t*) handle;

  textdisplay->lfToCrLf = on;

  return TEXTDISPLAY_EMSTATUS_OK;
}


/**************************************************************************//**
 * @brief Write a single character to a text display.
 *
 * @param[in] handle  Text display handle associated with the text display to
 *                    write the character on.
 * @param[in] c       Character to write.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS TEXTDISPLAY_WriteChar(TEXTDISPLAY_Handle_t  handle,
                               char                  c)
{
  TEXTDISPLAY_Device_t*  textdisplay = (TEXTDISPLAY_Device_t*) handle;

  /* Check that we are initialized. */
  if (false == textdisplay->initialized)
  {
    return TEXTDISPLAY_EMSTATUS_NOT_INITIALIZED;
  }

  /* Determine initial display update mode. */
  if (DISPLAY_ADDRESSING_BY_ROWS_ONLY == textdisplay->displayDevice.addressMode)
  {
    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_LINE;
  }
  else
  {
    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_CHAR;
  }

  if (textdisplay->scrollEnable)
  {
    /* Update the full screen (which has been scrolled up) if we start at the
       bottom left position and scroll is enabled. */
    if ((0==textdisplay->xpos) && (textdisplay->ypos == textdisplay->lines-1))
    {
      textdisplay->updateMode  = TEXTDISPLAY_UPDATE_MODE_FULL;
    }
  }
  else
  {
    /* Update the full screen (which is cleared) if we start at top left
       position and scroll is not enabled. */
    if ((0==textdisplay->xpos) && (0==textdisplay->ypos))
    {
      textdisplay->updateMode  = TEXTDISPLAY_UPDATE_MODE_FULL;
    }
  }

  /* Check for form feed - clear screen */
  if (c == '\f')
  {
    TextdisplayClear(textdisplay);
    /* Update the whole display and return. */
    textdisplay->updateMode  = TEXTDISPLAY_UPDATE_MODE_FULL;
    return TextdisplayUpdate(textdisplay);
  }

  /* Add CR or LF to CRLF if enabled */
  if (textdisplay->lfToCrLf && (c == '\n'))
  {
    TextdisplayCharAdd(textdisplay, '\r');
  }
  TextdisplayCharAdd(textdisplay, c);

  if (TEXTDISPLAY_UPDATE_MODE_NONE != textdisplay->updateMode)
    /* Update display and return. */
    return TextdisplayUpdate(textdisplay);
  else
    return TEXTDISPLAY_EMSTATUS_OK;
}


/**************************************************************************//**
 * @brief Write a string of characters to a text display.
 *
 * @param[in] handle  Text display handle associated with the text display to
 *                    write the character string on.
 * @param[in] str     String to write.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
EMSTATUS TEXTDISPLAY_WriteString(TEXTDISPLAY_Handle_t  handle,
                                 const char*           str)
{
  TEXTDISPLAY_Device_t*  textdisplay = (TEXTDISPLAY_Device_t*) handle;
  EMSTATUS               status;
  char                   c;
  bool                   displayUpdated=false;

  /* Check that we are initialized. */
  if (false == textdisplay->initialized)
  {
    return TEXTDISPLAY_EMSTATUS_NOT_INITIALIZED;
  }

  /* Determine initial display update mode. */
  if (DISPLAY_ADDRESSING_BY_ROWS_ONLY == textdisplay->displayDevice.addressMode)
  {
    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_LINE;
  }
  else
  {
    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_CHAR;
  }

  while (*str)
  {
    /* Mark display as NOT updated. */
    displayUpdated = false;

    /* Clear screen if we start at top left position and scroll is not
       enabled. */
    if ((0==textdisplay->xpos) && (0==textdisplay->ypos) &&
        (false == textdisplay->scrollEnable))
    {
      TextdisplayClear(textdisplay);
      textdisplay->updateMode  = TEXTDISPLAY_UPDATE_MODE_FULL;
    }

    c = *str;

    switch (c)
    {
    case '\f':
      /* Form feed - clear screen */
      TextdisplayClear(textdisplay);
      textdisplay->updateMode  = TEXTDISPLAY_UPDATE_MODE_FULL;
      break;

    case '\n':
      /* Update display before proceeding to the next line. */
      status = TextdisplayUpdate(textdisplay);
      if (status != TEXTDISPLAY_EMSTATUS_OK)
        return status;

      displayUpdated = true;

      /* Add CR or LF to CRLF if enabled */
      if (textdisplay->lfToCrLf)
      {
        TextdisplayCharAdd(textdisplay, '\r');
      }

      TextdisplayCharAdd(textdisplay, c);
      break;

    default:
      TextdisplayCharAdd(textdisplay, c);
      break;
    }

    /* Update display if we reach the end of the line or a full display update
       is required. */
    if ((textdisplay->xpos >= textdisplay->columns) ||
        (textdisplay->updateMode == TEXTDISPLAY_UPDATE_MODE_FULL))
    {
      status = TextdisplayUpdate(textdisplay);
      if (status != TEXTDISPLAY_EMSTATUS_OK)
        return status;
      displayUpdated = true;
    }

    str++;
  }

  /* Update display if not updated. */
  if (!displayUpdated)
    return TextdisplayUpdate(textdisplay);
  else
    return TEXTDISPLAY_EMSTATUS_OK;
}


/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 ****************************   STATIC FUNCTIONS   *****************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief  Clear the character buffer.
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 *****************************************************************************/
static void TextdisplayClear(TEXTDISPLAY_Device_t* textdisplay)
{
  if (textdisplay->charArray)
  {

#if (FONT_ASCII_START>' ') || (FONT_ASCII_START+FONT_CHARACTERS<' ')
    /* The font does not include the space character at the standard position
       according to the ascii table. Then the pace should be placed at the
       "end" of the font table. */
    memset(textdisplay->charBuffer, FONT_CHARACTERS - 1,
           textdisplay->lines * textdisplay->columns);
#else
    memset(textdisplay->charBuffer, 0,
           textdisplay->lines * textdisplay->columns);
#endif

    /* Set cursor position to upper left */
    textdisplay->xpos = 0;
    textdisplay->ypos = 0;

    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_FULL;
  }
}


/**************************************************************************//**
 * @brief Scroll one line of characters up on the screen
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 *****************************************************************************/
static void TextdisplayScrollUp(TEXTDISPLAY_Device_t*  textdisplay)
{
  unsigned int y;

  /* copy all lines one line up */
  for (y = 0; y < (textdisplay->lines - 1); y++)
  {
    memcpy(textdisplay->charArray[y],
           textdisplay->charArray[y + 1],
           textdisplay->columns);
  }

  /* clear last line */
  memset(&textdisplay->charArray[textdisplay->lines - 1][0],
         0,
         textdisplay->columns);

  textdisplay->xpos       = 0;
  textdisplay->ypos       = textdisplay->lines - 1;
  textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_FULL;
}


#ifdef INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT
/**************************************************************************//**
 * @brief Check and handle if the character is part of a VT100/VT52(etc)
 *        escape sequence.
 *
 * @detail This function checks if the character is part of a VT100 escape
 *         sequence, and handles the sequence when fully received.
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 * @param[in] c               ASCII character from user.
 *
 *****************************************************************************/
static bool TextdisplayEscapeSequence(TEXTDISPLAY_Device_t* textdisplay,
                                      int                   c)
{
#define ASCII_ESC                           (27)
#define ESC_SEQ_SIZE_MAX                     (4)
#define ESC_SEQ_MAX                          (6)
  static char escapeSequence[ESC_SEQ_SIZE_MAX]= {0,0,0,0};
  static const char escapeSequences[ESC_SEQ_MAX][ESC_SEQ_SIZE_MAX] =
  {
    /* Do not change the order of the escape sequences in the table below,
     and make sure they correspond to the TEXTDISPLAY_ESC_SEQNO_XXX contants
     below. */
    TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100,
    TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT52,
    TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE,
    TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE,
    TEXTDISPLAY_ESC_SEQ_CURSOR_RIGHT_ONE_CHAR,
    TEXTDISPLAY_ESC_SEQ_CURSOR_LEFT_ONE_CHAR
  };
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_HOME_VT100        (0)
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_HOME_VT52         (1)
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_UP_ONE_LINE       (2)
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_DOWN_ONE_LINE     (3)
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_RIGHT_ONE_CHAR    (4)
#define TEXTDISPLAY_ESC_SEQNO_CURSOR_LEFT_ONE_CHAR     (5)

  char*       pChar;
  int         seqNo;
  bool        anyMatch = false;

  /* Handle escape sequence */
  if (ASCII_ESC == c)
  {
    /* Clear escape sequence, store the ESC character, update mode none and
       return true. */
    memset(escapeSequence, 0, ESC_SEQ_SIZE_MAX);
    escapeSequence[0]=c;
    textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_NONE;
    return true;
  }

  if (ASCII_ESC != escapeSequence[0])
  {
    /* We are not currently receiving an escape sequence. Just return false. */
    return false;
  }
  else
  {
    unsigned int len;

    /* We are currently receiving an escape sequence. Append the character to
       the end of the current escape sequence buffer and check if the sequence
       matches any of the supported sequences. */

    pChar = escapeSequence;
    while (*(++pChar));
    *pChar = c;

    len = strlen(escapeSequence);

    for (seqNo=0; seqNo<ESC_SEQ_MAX; seqNo++)
    {
      /* Check for supported escape sequences.*/
      if (0 == strncmp(escapeSequence, escapeSequences[seqNo], len))
      {
        /* We match part of the cursor home sequence. Set anyMatch and update
           mode to none. */
        anyMatch = true;
        textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_NONE;

        /* Full match? */
        if (len == strlen(escapeSequences[seqNo]))
        {
          switch (seqNo)
          {
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_HOME_VT100:
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_HOME_VT52:
            /* Move cursor to upper left corner, and set update mode to none. */
            textdisplay->ypos = 0;
            textdisplay->xpos = 0;
            break;
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_UP_ONE_LINE:
            /* Move cursor up one line. */
            if (textdisplay->ypos)
              textdisplay->ypos--;
            break;
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_DOWN_ONE_LINE:
            /* Move cursor down one line. */
            if (textdisplay->ypos < textdisplay->lines-1)
              textdisplay->ypos++;
            break;
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_RIGHT_ONE_CHAR:
            /* Move cursor right one column. */
            if (textdisplay->xpos < textdisplay->columns-1)
              textdisplay->xpos++;
            break;
          case TEXTDISPLAY_ESC_SEQNO_CURSOR_LEFT_ONE_CHAR:
            /* Move cursor left one column. */
            if (textdisplay->xpos)
              textdisplay->xpos--;
            break;
          }

          /* Clear escape sequence, set no display update and return true. */
          memset(escapeSequence, 0, ESC_SEQ_SIZE_MAX);
          textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_NONE;
          return true;
        }

        /* We found a partial match. Break out of the loop and return in order
           to await the next character. */
        break;
      }
    }

    if (false == anyMatch)
    {
      /* No match found. Clear escape sequence and return false. */
      memset(escapeSequence, 0, ESC_SEQ_SIZE_MAX);
      return false;
    }

    if (ESC_SEQ_SIZE_MAX-1 == len)
    {
      /* The next character will overflow the escape sequence buffer.
         Clear the escape sequence buffer and return true. */
      memset(escapeSequence, 0, ESC_SEQ_SIZE_MAX);
      textdisplay->updateMode = TEXTDISPLAY_UPDATE_MODE_NONE;
      return true;
    }

    return anyMatch;
  }
}

#endif /* INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT */


/**************************************************************************//**
 * @brief Add a character to the character buffer.
 *
 * @detail This function adds a character to the character buffer. It does
 *         not update he display. Some special characters are handled:
 *         '\b'   Bell warning which toggles red text on colour displays.
 *         '\r'   Carriage Return (CR) moves the cursor position to column 0.
 *         '\n'   Line Feed (LF) moves the cursor position to the next line.
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 * @param[in] c               ASCII character to output.
 *
 *****************************************************************************/
static void TextdisplayCharAdd(TEXTDISPLAY_Device_t* textdisplay,
                               int                   c)
{
  switch (c)
  {
  case '\r':  /* check for CR */
    textdisplay->xpos = 0;
    return;

  case '\n':  /* check for LF */
    textdisplay->ypos = textdisplay->ypos + 1;
    textdisplay->xpos = 0;
    if (textdisplay->ypos >= textdisplay->lines)
    {
      if (textdisplay->scrollEnable)
      {
        /* Scroll characters one line up */
        TextdisplayScrollUp(textdisplay);
        textdisplay->ypos = (textdisplay->lines - 1);
      }
      else
      {
        TextdisplayClear(textdisplay);
      }
    }
    return;

  case '\b':  /* check for bell character, changes color to red */
    if ( (DISPLAY_COLOUR_MODE_MONOCHROME !=
          textdisplay->displayDevice.colourMode) &&
         (DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE !=
          textdisplay->displayDevice.colourMode) )
    {
      if (textdisplay->rgbColor[1] == 0xff)
      {
        textdisplay->rgbColor[1] = 0x00;
        textdisplay->rgbColor[2] = 0x00;
      }
      else
      {
        textdisplay->rgbColor[1] = 0xff;
        textdisplay->rgbColor[2] = 0xff;
      }
      return;
    }

  }

#ifdef INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT
  if (TextdisplayEscapeSequence(textdisplay, c))
  {
    /* We have detected an escape sequence. Return now. */
    return;
  }
#endif

  /* Check for non-printable characters, and, if so, replace with space. */
  if (!isprint(c))
  {
    c = ' ';
  }

#if 100 > FONT_CHARACTERS
  /* The font does not have all printable characters in the ASCII table.
     Check if character is supported. */
  if ( (FONT_ASCII_START>c) || (FONT_ASCII_START+FONT_CHARACTERS<=c) )
  {
#if (FONT_ASCII_START>' ') || (FONT_ASCII_START+FONT_CHARACTERS<' ')
    /* Replace with an empty character (space) which should be placed at the
       "end" of the font table. */
    c = FONT_ASCII_START + FONT_CHARACTERS - 1;
#else
    c = ' ';
#endif
  }
#endif

  if (textdisplay->xpos >= textdisplay->columns)
  {
    textdisplay->xpos = 0;
    textdisplay->ypos = textdisplay->ypos + 1;
  }
  if (textdisplay->ypos >= textdisplay->lines)
  {
    if (textdisplay->scrollEnable)
    {
      TextdisplayScrollUp(textdisplay);
      textdisplay->ypos = textdisplay->lines - 1;
    }
    else
    {
      TextdisplayClear(textdisplay);
    }
  }
  textdisplay->charArray[textdisplay->ypos][textdisplay->xpos] =
    c - FONT_ASCII_START;

  textdisplay->xpos = textdisplay->xpos + 1;

  return;
}


/**************************************************************************//**
 * @brief  Update display
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 * @param[in] y               Number of line/row to draw.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS TextdisplayLineDraw(TEXTDISPLAY_Device_t*  textdisplay,
                                    unsigned int           y)
{
  unsigned int x, i;
  uint8_t      c;
  FontBits_t*  rowPtr = (FontBits_t*) textdisplay->lineBuffer;
  FontBits_t   pixelBits;

  for (i = 0; i < FONT_HEIGHT; i++)
  {
    for (x = 0; x < textdisplay->columns; x++)
    {
      c = textdisplay->charArray[y][x];

      pixelBits = fontBits[c + FONT_CHARACTERS * i];

      switch (textdisplay->displayDevice.colourMode)
      {
#if (8 == FONT_WIDTH) || (16 == FONT_WIDTH)
      case DISPLAY_COLOUR_MODE_MONOCHROME:
        rowPtr[x] = pixelBits;
        break;

      case DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE:
        rowPtr[x] = ~pixelBits;
        break;
#elif (8 > FONT_WIDTH) || (16 > FONT_WIDTH)
      case DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE:
        pixelBits = ~pixelBits;
      case DISPLAY_COLOUR_MODE_MONOCHROME:
      {
        int startPixel = x * FONT_WIDTH;
        int pixelNo = startPixel;
        for (; pixelNo<startPixel+FONT_WIDTH; pixelBits>>=1, pixelNo++)
        {
          if (pixelBits & 0x1)
          {
            rowPtr[pixelNo>>FONT_BITS_LOG2] |= 1 << (pixelNo&FONT_BITS_MASK);
          }
          else
          {
            rowPtr[pixelNo>>FONT_BITS_LOG2] &= ~(1 << (pixelNo&FONT_BITS_MASK));
          }
        }
      }
      break;
#endif
      }
    }

#if (8 != FONT_WIDTH) || (16 != FONT_WIDTH)
    /* Set background colour at the end of the display line. */
    if (textdisplay->displayDevice.colourMode ==
        DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE)
    {
      int usedBits;
      int startPixel = x * FONT_WIDTH;
      /* Check if the startPixel is not aligned at an FontBits_t boundary,
         meaning that we need to adjust it to the next FontBits_t boundary. */
      usedBits = startPixel & FONT_BITS_MASK;
      if (usedBits)
      {
        /* First, clear the last bits in the FontBits_t word of the startPixel. */
        rowPtr[startPixel>>FONT_BITS_LOG2] &= (1<<usedBits)-1;
        /* Adjust the startPixel to the next FontBits_t boundary. */
        startPixel += (sizeof(FontBits_t)*8) - usedBits;
      }
      memset (&rowPtr[startPixel>>FONT_BITS_LOG2], 0xff,
              (textdisplay->displayDevice.geometry.width - startPixel)/8);
    }
#endif

    rowPtr +=
      textdisplay->displayDevice.geometry.stride / (sizeof(FontBits_t) * 8) ;
  }

  return
    textdisplay->displayDevice.pPixelMatrixDraw(&textdisplay->displayDevice,
                                                textdisplay->lineBuffer,
                                                0,
                                                textdisplay->columns*FONT_WIDTH,
#ifdef EMWIN_WORKAROUND
                                                textdisplay->columns*FONT_WIDTH,
#endif
                                                y * FONT_HEIGHT,
                                                FONT_HEIGHT);
}


/**************************************************************************//**
 * @brief  Update display
 *
 * @param[in] textdisplay     Pointer to the text display device structure.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS TextdisplayUpdate(TEXTDISPLAY_Device_t*  textdisplay)
{
  unsigned int  i;
  EMSTATUS      status;

  switch (textdisplay->updateMode)
  {
  case TEXTDISPLAY_UPDATE_MODE_NONE:
  default:
    break;

  case TEXTDISPLAY_UPDATE_MODE_FULL:

    /* Draw a full screen */

    switch (textdisplay->displayDevice.addressMode)
    {
    case DISPLAY_ADDRESSING_BY_ROWS_ONLY:

      /* Update all text lines. */
      for (i=0; i<textdisplay->lines; i++)
        /* Draw the current text line on the display. */
        TextdisplayLineDraw(textdisplay, i);

      /* Clear lowest and unused part of the screen. */
      if (textdisplay->lines * FONT_HEIGHT <
          textdisplay->displayDevice.geometry.height)
      {
        switch (textdisplay->displayDevice.colourMode)
        {
        case DISPLAY_COLOUR_MODE_MONOCHROME:
          memset (textdisplay->lineBuffer, 0x00,
                  textdisplay->displayDevice.geometry.width/8);
          break;
        case DISPLAY_COLOUR_MODE_MONOCHROME_INVERSE:
          memset (textdisplay->lineBuffer, 0xff,
                  textdisplay->displayDevice.geometry.width/8);
          break;
        default:
          return TEXTDISPLAY_EMSTATUS_NOT_SUPPORTED;
        }

        for (i = textdisplay->lines * FONT_HEIGHT;
             i < textdisplay->displayDevice.geometry.height;
             i++)
        {
          status =
            textdisplay->displayDevice.pPixelMatrixDraw(&textdisplay->displayDevice,
                                                        textdisplay->lineBuffer,
                                                        0,
                                                        textdisplay->displayDevice.geometry.width,
#ifdef EMWIN_WORKAROUND
                                                        textdisplay->displayDevice.geometry.width,
#endif
                                                        i,
                                                        1);
          if (DISPLAY_EMSTATUS_OK != status)
            return status;
         }
      }
      break;

    default:
      return TEXTDISPLAY_EMSTATUS_NOT_SUPPORTED;
    }

  case TEXTDISPLAY_UPDATE_MODE_LINE:

    switch (textdisplay->displayDevice.addressMode)
    {
    case DISPLAY_ADDRESSING_BY_ROWS_ONLY:

      /* Draw the current text line on the display. */
      return TextdisplayLineDraw(textdisplay, textdisplay->ypos);

    default:
      return TEXTDISPLAY_EMSTATUS_NOT_SUPPORTED;
    }

  case TEXTDISPLAY_UPDATE_MODE_CHAR:
    return TEXTDISPLAY_EMSTATUS_NOT_SUPPORTED;
  }
  return TEXTDISPLAY_EMSTATUS_OK;
}

/** @endcond */


/***************  THE REST OF THE FILE IS DOCUMENTATION ONLY !  ***************/

/*******************************************************************************
 **************************       DOCUMENTATION       **************************
 ******************************************************************************/

/**************************************************************************//**
 * @addtogroup Textdisplay
 * @brief Line based text output terminal interface on top of the DISPLAY
 *        device driver stack. See @ref textdisplay_doc for more information.
 * @{

@n @section textdisplay_doc TextDisplay Library

  The source code of the TEXTDISPLAY module is implemented in
  kits/common/drivers/textdisplay.c and textdisplay.h.

  @li @ref textdisplay_intro
  @li @ref textdisplay_config

@n @section textdisplay_intro Introduction

  The TEXTDISPLAY library implements a line based text output terminal on top
  of the DISPLAY device driver stack. It exports a set of functions that enables
  the user to print text on any of the display devices that are available via
  the DISPLAY device driver interface. Additionally the retargettextdisplay
  module (@ref retargettextdisplay_doc) retargets the stdout interface to a
  textdisplay device in order for  the user to print text by calling standard
  C language functions (like printf) that prints text to stdout.

@n @section textdisplay_config TEXTDISPLAY Configuration.

  This section contains a description of the configuration parameters of
  the TEXTDISPLAY Library.

  In order to enable the TEXTDISPLAY module the user must define
  INCLUDE_TEXTDISPLAY_SUPPORT (typically in displayconfigapp.h).

  The user can select one out of a few different fonts, and
  the display will be organized in a number of lines and columns to fit
  the maximum number of characters depending on the font size. Currently
  the available fonts are:

  @verbatim
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
  @endverbatim

  The TEXTDISPLAY module may supports scroll mode which may be enabled via the
  input parameter to the TEXTDISPLAY_New function during initialization, or by
  defining RETARGETTEXTDISPLAY_SCROLL_MODE to 'true' which enables scroll mode
  when using the retargettextdisplay module to retarget stdout to a TEXTDISPLAY
  device.

  The user can select whether the LINE FEED (LF) character should be
  automatically accompanied by a CARRIAGE RETURN (CR) in order to bring the
  cursor position to the leftmost column on the display when a LF is printed.
  This can be enabled via the input parameter to the TEXTDISPLAY_New function
  during initialization, or by defining RETARGETTEXTDISPLAY_LINE_FEED_MODE to
  'true' which enables LF+CR when using the retargettextdisplay module to
  retarget stdout to a TEXTDISPLAY device.

  The TEXTDISPLAY module supports some standard VT52/VT100 escape sequences in
  order to move the cursor around the screen without clearing the existing
  characters. Please refer to the textdisplay.h file for a list of supported
  escape sequence codes. The user must define
  INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT
  (typically in displayconfigapp.h) in order to include support for escape
  sequences.

  @n @subsection textdisplay_config_kit TEXTDISPLAY Kit Configuration
  This section lists and describes the configuration parameters
  defined to default values in kits/kit_name/config/textdisplayconfig.h.
  The user may need or want to override some of these configuration
  parameters in displayconfigapp.h.

  @verbatim
#define TEXTDISPLAY_DEVICES_MAX
     Maximum number of text display devices the display module is configured
     to support. This number may be increased if the system includes more than
     one display device.

#define TEXTDISPLAY_DEVICE_0_LINES        (DISPLAY0_HEIGHT / FONT_HEIGHT)
#define TEXTDISPLAY_DEVICE_0_COLUMNS        (DISPLAY0_WIDTH / FONT_WIDTH)
     Specify the number of lines and columns of the textdisplay devices. The
     default values are the maximal possible and utilizes the full width and
     height of the display. The user should not increase the values above the
     maximal values, however the values may be reduced.
     These constants are used for static memory allocation in the textdisplay
     driver.
  @endverbatim

@}

(end group Textdisplay) */
