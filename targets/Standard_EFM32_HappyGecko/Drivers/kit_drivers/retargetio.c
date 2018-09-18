/***************************************************************************//**
 * @file
 * @brief Provide stdio retargeting for all supported toolchains.
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



/***************************************************************************//**
 * @addtogroup RetargetIo
 * @{ This module provide low-level stubs for retargetting stdio for all
 *    supported toolchains.
 *    The stubs are minimal yet sufficient implementations.
 *    Refer to chapter 12 in the reference manual for newlib 1.17.0
 *    for details on implementing newlib stubs.
 ******************************************************************************/

extern int RETARGET_ReadChar(void);
extern int RETARGET_WriteChar(char c);

#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "em_device.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
int fileno(FILE *);
/** @endcond */

int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _lseek(int file, int ptr, int dir);
int _read(int file, char *ptr, int len);
caddr_t _sbrk(int incr);
int _write(int file, const char *ptr, int len);

extern char _end;                 /**< Defined by the linker */

/**************************************************************************//**
 * @brief
 *  Close a file.
 *
 * @param[in] file
 *  File you want to close.
 *
 * @return
 *  Returns 0 when the file is closed.
 *****************************************************************************/
int _close(int file)
{
  (void) file;
  return 0;
}

/**************************************************************************//**
 * @brief Exit the program.
 * @param[in] status The value to return to the parent process as the
 *            exit status (not used).
 *****************************************************************************/
void _exit (int status)
{
  (void) status;
  while (1) {}      /* Hang here forever... */
}

/**************************************************************************//**
 * @brief
 *  Status of an open file.
 *
 * @param[in] file
 *  Check status for this file.
 *
 * @param[in] st
 *  Status information.
 *
 * @return
 *  Returns 0 when st_mode is set to character special.
 *****************************************************************************/
int _fstat(int file, struct stat *st)
{
  (void) file;
  st->st_mode = S_IFCHR;
  return 0;
}

/**************************************************************************//**
 * @brief Get process ID.
 *****************************************************************************/
int _getpid(void)
{
  return 1;
}

/**************************************************************************//**
 * @brief
 *  Query whether output stream is a terminal.
 *
 * @param[in] file
 *  Descriptor for the file.
 *
 * @return
 *  Returns 1 when query is done.
 *****************************************************************************/
int _isatty(int file)
{
  (void) file;
  return 1;
}

/**************************************************************************//**
 * @brief Send signal to process.
 * @param[in] pid Process id (not used).
 * @param[in] sig Signal to send (not used).
 *****************************************************************************/
int _kill(int pid, int sig)
{
  (void)pid;
  (void)sig;
  return -1;
}

/**************************************************************************//**
 * @brief
 *  Set position in a file.
 *
 * @param[in] file
 *  Descriptor for the file.
 *
 * @param[in] ptr
 *  Poiter to the argument offset.
 *
 * @param[in] dir
 *  Directory whence.
 *
 * @return
 *  Returns 0 when position is set.
 *****************************************************************************/
int _lseek(int file, int ptr, int dir)
{
  (void) file;
  (void) ptr;
  (void) dir;
  return 0;
}

/**************************************************************************//**
 * @brief
 *  Read from a file.
 *
 * @param[in] file
 *  Descriptor for the file you want to read from.
 *
 * @param[in] ptr
 *  Pointer to the chacaters that are beeing read.
 *
 * @param[in] len
 *  Number of characters to be read.
 *
 * @return
 *  Number of characters that have been read.
 *****************************************************************************/
int _read(int file, char *ptr, int len)
{
  int c, rxCount = 0;

  (void) file;

  while (len--)
  {
    if ((c = RETARGET_ReadChar()) != -1)
    {
      *ptr++ = c;
      rxCount++;
    }
    else
    {
      break;
    }
  }

  if (rxCount <= 0)
  {
    return -1;                        /* Error exit */
  }

  return rxCount;
}

/**************************************************************************//**
 * @brief
 *  Increase heap.
 *
 * @param[in] incr
 *  Number of bytes you want increment the program's data space.
 *
 * @return
 *  Rsturns a pointer to the start of the new area.
 *****************************************************************************/
caddr_t _sbrk(int incr)
{
  static char       *heap_end;
  char              *prev_heap_end;
  static const char heaperr[] = "Heap and stack collision\n";

  if (heap_end == 0)
  {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;
  if ((heap_end + incr) > (char*) __get_MSP())
  {
    _write(fileno(stdout), heaperr, strlen(heaperr));
    exit(1);
  }
  heap_end += incr;

  return (caddr_t) prev_heap_end;
}

/**************************************************************************//**
 * @brief
 *  Write to a file.
 *
 * @param[in] file
 *  Descriptor for the file you want to write to.
 *
 * @param[in] ptr
 *  Pointer to the text you want to write
 *
 * @param[in] len
 *  Number of characters to be written.
 *
 * @return
 *  Number of characters that have been written.
 *****************************************************************************/
int _write(int file, const char *ptr, int len)
{
  int txCount;

  (void) file;

  for (txCount = 0; txCount < len; txCount++)
  {
    RETARGET_WriteChar(*ptr++);
  }

  return len;
}
#endif /* !defined( __CROSSWORKS_ARM ) && defined( __GNUC__ ) */

#if defined(__ICCARM__)
/*******************
 *
 * Copyright 1998-2003 IAR Systems.  All rights reserved.
 *
 * $Revision: 38614 $
 *
 * This is a template implementation of the "__write" function used by
 * the standard library.  Replace it with a system-specific
 * implementation.
 *
 * The "__write" function should output "size" number of bytes from
 * "buffer" in some application-specific way.  It should return the
 * number of characters written, or _LLIO_ERROR on failure.
 *
 * If "buffer" is zero then __write should perform flushing of
 * internal buffers, if any.  In this case "handle" can be -1 to
 * indicate that all handles should be flushed.
 *
 * The template implementation below assumes that the application
 * provides the function "MyLowLevelPutchar".  It should return the
 * character written, or -1 on failure.
 *
 ********************/

#include <yfuns.h>
#include <stdint.h>

_STD_BEGIN

/**************************************************************************//**
 * @brief Transmit buffer to USART1
 * @param buffer Array of characters to send
 * @param nbytes Number of bytes to transmit
 * @return Number of bytes sent
 *****************************************************************************/
static int TxBuf(uint8_t *buffer, int nbytes)
{
  int i;

  for (i = 0; i < nbytes; i++)
  {
    RETARGET_WriteChar(*buffer++);
  }
  return nbytes;
}

/*
 * If the __write implementation uses internal buffering, uncomment
 * the following line to ensure that we are called with "buffer" as 0
 * (i.e. flush) when the application terminates.
 */

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */

  size_t nChars = 0;

  if (buffer == 0)
  {
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    return 0;
  }

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */
  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
  {
    return _LLIO_ERROR;
  }

  /* Hook into USART1 transmit function here */
  if (TxBuf((uint8_t *) buffer, size) != size)
    return _LLIO_ERROR;
  else
    nChars = size;

  return nChars;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = RETARGET_ReadChar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}

_STD_END

#endif /* defined( __ICCARM__ ) */

#if defined(__CROSSWORKS_ARM)

/* Pass each of these function straight to the USART */
int __putchar(int ch)
{
  return(RETARGET_WriteChar(ch));
}

int __getchar(void)
{
  return(RETARGET_ReadChar());
}

#endif /* defined( __CROSSWORKS_ARM ) */

#if defined(__CC_ARM)
/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <stdio.h>

/* #pragma import(__use_no_semihosting_swi) */

struct __FILE
{
  int handle;
};

/**Standard output stream*/
FILE __stdout;

/**************************************************************************//**
 * @brief
 *  Writes character to file
 *
 * @param[in] f
 *  File
 *
 * @param[in] ch
 *  Character
 *
 * @return
 *  Written character
 *****************************************************************************/
int fputc(int ch, FILE *f)
{
  return(RETARGET_WriteChar(ch));
}

/**************************************************************************//**
 * @brief
 *  Reads character from file
 *
 * @param[in] f
 *  File
 *
 * @return
 *  Character
 *****************************************************************************/
int fgetc(FILE *f)
{
  return(RETARGET_ReadChar());
}

/**************************************************************************//**
 * @brief
 *  Tests the error indicator for the stream pointed
 *  to by file
 *
 * @param[in] f
 *  File
 *
 * @return
 *  Returns non-zero if it is set
 *****************************************************************************/
int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}

/**************************************************************************//**
 * @brief
 *  Writes a character to the console
 *
 * @param[in] ch
 *  Character
 *****************************************************************************/
void _ttywrch(int ch)
{
  RETARGET_WriteChar(ch);
}

/**************************************************************************//**
 * @brief
 *  Library exit function. This function is called if stack
 *  overflow occurs.
 *
 * @param[in] return_code
 *  Return code
 *****************************************************************************/
void _sys_exit(int return_code)
{
 label:  goto label; /* endless loop */
}
#endif /* defined( __CC_ARM ) */

/** @} (end group RetargetIo) */
