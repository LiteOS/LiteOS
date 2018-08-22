/*
 *
 *   Copyright (c) 2001, Carlos E. Vidales. All rights reserved.
 *
 *   This sample program was written and put in the public domain
 *    by Carlos E. Vidales.  The program is provided "as is"
 *    without warranty of any kind, either expressed or implied.
 *   If you choose to use the program within your own products
 *    you do so at your own risk, and assume the responsibility
 *    for servicing, repairing or correcting the program should
 *    it prove defective in any manner.
 *   You may copy and distribute the program's source code in any
 *    medium, provided that you also include in each copy an
 *    appropriate copyright notice and disclaimer of warranty.
 *   You may also modify this program and distribute copies of
 *    it provided that you include prominent notices stating
 *    that you changed the file(s) and the date of any change,
 *    and that you do not charge any royalties or licenses for
 *    its use.
 *
 *
 *   File Name:  calibrate.h
 *
 *
 *   Definition of constants and structures, and declaration of functions
 *    in Calibrate.c
 *
 */

#ifndef _CALIBRATE_H_

#define _CALIBRATE_H_

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif



/****************************************************/
/*                                                  */
/* Definitions                                      */
/*                                                  */
/****************************************************/

#ifndef		_CALIBRATE_C_
	#define		EXTERN         extern
#else
	#define		EXTERN
#endif



#ifndef		OK
	#define		OK		        0
	#define		NOT_OK		   -1
#endif



#define			INT32				long




/****************************************************/
/*                                                  */
/* Structures                                       */
/*                                                  */
/****************************************************/


typedef struct Point {
                        INT32    x,
                                 y ;
                     } POINT ;



typedef struct Matrix {
							/* This arrangement of values facilitates
							 *  calculations within getDisplayPoint()
							 */
                        INT32    An,     /* A = An/Divider */
                                 Bn,     /* B = Bn/Divider */
                                 Cn,     /* C = Cn/Divider */
                                 Dn,     /* D = Dn/Divider */
                                 En,     /* E = En/Divider */
                                 Fn,     /* F = Fn/Divider */
                                 Divider ;
                     } MATRIX ;




/****************************************************/
/*                                                  */
/* Function declarations                            */
/*                                                  */
/****************************************************/


EXTERN int setCalibrationMatrix( POINT * display,
                                 POINT * screen,
                                 MATRIX * matrix) ;


EXTERN int getDisplayPoint( POINT * display,
                            POINT * screen,
                            MATRIX * matrix ) ;


#ifdef __cplusplus
}
#endif

#endif  /* _CALIBRATE_H_ */
