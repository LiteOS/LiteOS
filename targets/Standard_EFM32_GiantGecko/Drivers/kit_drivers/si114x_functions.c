/*************************************************************************//**
 * @file
 * @brief Si114x reusable functions
 * @version 5.0.0
 *****************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 ******************************************************************************
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 *****************************************************************************/
#include "si114x_functions.h"

/*****************************************************************************/
/**************    Compile Switches   ****************************************/
/*****************************************************************************/

#ifndef INCLUDE_SI114X_CALIBRATIONCODE
/***************************************************************************//**
 * @brief
 *   INCLUDE_SI114X_CALIBRATIONCODE should be assigned to 1 for the Si1132,
 *   Si1145, Si1146 and Si1147. For the Si1141, Si1142 and Si1143, assign it to
 *   0 to save code space.
 ******************************************************************************/
#define INCLUDE_SI114X_CALIBRATIONCODE   1
#endif



#ifndef INCLUDE_SI114X_COMPRESS_CODE
/***************************************************************************//**
 * @brief
 *   INCLUDE_SI114X_COMPRESS_CODE should be assigned to 1
 *   for the Si1141, Si1142, Si1143. This switch should be
 *   assigned to 0 only if it is certain that the code is not
 *   used elsewhere.
 ******************************************************************************/
#define INCLUDE_SI114X_COMPRESS_CODE     1
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define LOOP_TIMEOUT_MS 200
/***************************************************************************//**
 * @brief
 *   Waits until the Si113x/4x is sleeping before proceeding
 ******************************************************************************/
static int16_t _waitUntilSleep(HANDLE si114x_handle)
{
  int8_t  retval = -1;
  uint8_t count = 0;
  // This loops until the Si114x is known to be in its sleep state
  // or if an i2c error occurs
  while(count < LOOP_TIMEOUT_MS)
  {
    retval = Si114xReadFromRegister(si114x_handle, REG_CHIP_STAT);
    if(retval == 1) break;
    if(retval <  0) return retval;
    count++;
    delay_1ms();
  }
  return 0;
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Resets the Si113x/4x, clears any interrupts and initializes the HW_KEY
 *   register.
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xReset(HANDLE si114x_handle)
{
  int32_t retval = 0;

  //
  // Do not access the Si114x earlier than 25 ms from power-up.
  // Uncomment the following lines if Si114xReset() is the first
  // instruction encountered, and if your system MCU boots up too
  // quickly.
  //
  delay_10ms();
  delay_10ms();
  delay_10ms();

  retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_ALS_RATE,   0x00);
  retval+=Si114xPauseAll(si114x_handle);

  // The clearing of the registers could be redundant, but it is okay.
  // This is to make sure that these registers are cleared.
  retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, 0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE1,  0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE2,  0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_INT_CFG  ,  0x00);
  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, 0xFF);

  // Perform the Reset Command
  retval+=Si114xWriteToRegister(si114x_handle, REG_COMMAND, 1);

  // Delay for 10 ms. This delay is needed to allow the Si114x
  // to perform internal reset sequence.
  delay_10ms();

  // Write Hardware Key
  retval+=Si114xWriteToRegister(si114x_handle, REG_HW_KEY, HW_KEY_VAL0);

  return retval;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Helper function to send a command to the Si113x/4x
 ******************************************************************************/
static int16_t _sendCmd(HANDLE si114x_handle, uint8_t command)
{
  int16_t  response;
  int8_t   retval;
  uint8_t  count = 0;

  // Get the response register contents
  response = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
  if(response < 0)
    return response;

  // Double-check the response register is consistent
  while(count < LOOP_TIMEOUT_MS)
  {
    if((retval=_waitUntilSleep(si114x_handle)) != 0) return retval;

    if(command==0) break; // Skip if the command is NOP

    retval=Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
    if(retval==response) break;
    else if(retval<0) return retval;
    else response = retval;
    count++;
  }

  // Send the Command
  if((retval=Si114xWriteToRegister(si114x_handle, REG_COMMAND, command)) !=0)
    return retval;

  count = 0;
  // Expect a change in the response register
  while(count < LOOP_TIMEOUT_MS)
  {
    if(command==0) break; // Skip if the command is NOP

    retval= Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
    if(retval != response) break;
    else if(retval<0) return retval;
    count++;
    delay_1ms();
  }
  return 0;
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Sends a NOP command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       Success
 * @retval  <0      Error
 ******************************************************************************/
int16_t Si114xNop(HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x00);
}

/***************************************************************************//**
 * @brief
 *   Sends a PSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xPsForce(HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x05);
}

/***************************************************************************//**
 * @brief
 *   Sends an ALSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xAlsForce(HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x06);
}

/***************************************************************************//**
 * @brief
 *   Sends a PSALSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xPsAlsForce(HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x07);
}

/***************************************************************************//**
 * @brief
 *   Sends a PSALSAUTO command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xPsAlsAuto (HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x0F);
}

/***************************************************************************//**
 * @brief
 *   Reads a Parameter from the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @param[in] address
 *   The address of the parameter.
 * @retval <0
 *   Error
 * @retval 0-255
 *   Parameter contents
 ******************************************************************************/
int16_t Si114xParamRead(HANDLE si114x_handle, uint8_t address)
{
  // returns Parameter[address]
  int16_t retval;
  uint8_t cmd = 0x80 + (address & 0x1F);

  retval=_sendCmd(si114x_handle, cmd);
  if( retval != 0 ) return retval;

  retval = Si114xReadFromRegister(si114x_handle, REG_PARAM_RD);
  return retval;
}

/***************************************************************************//**
 * @brief
 *   Writes a byte to an Si113x/4x Parameter
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @param[in] address
 *   The parameter address
 * @param[in] value
 *   The byte value to be written to the Si113x/4x parameter
 * @retval 0
 *   Success
 * @retval <0
 *   Error
 * @note This function ensures that the Si113x/4x is idle and ready to
 * receive a command before writing the parameter. Furthermore,
 * command completion is checked. If setting parameter is not done
 * properly, no measurements will occur. This is the most common
 * error. It is highly recommended that host code make use of this
 * function.
 ******************************************************************************/
int16_t Si114xParamSet(HANDLE si114x_handle, uint8_t address, uint8_t value)
{
  int16_t retval;
  uint8_t buffer[2];
  int16_t response_stored;
  int16_t response;

  if((retval = _waitUntilSleep(si114x_handle))!=0) return retval;

  response_stored = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);

  buffer[0]= value;
  buffer[1]= 0xA0 + (address & 0x1F);

  retval=Si114xBlockWrite(si114x_handle, REG_PARAM_WR, 2, ( uint8_t* ) buffer);
  if(retval != 0) return retval;

  // Wait for command to finish
  response = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
  while(response == response_stored )
  {
    response = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
    if (response == response_stored)
    {
      delay_1ms();
    }
  }

  if(retval < 0)
    return retval;
  else
    return 0;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Pause measurement helper function
 ******************************************************************************/
static int16_t _PsAlsPause (HANDLE si114x_handle)
{
  return _sendCmd(si114x_handle,0x0B);
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Pauses autonomous measurements
 * @param[in] si114x_handle
 *  The programmer's toolkit handle
 * @retval  0
 *   Success
 * @retval  <0
 *   Error
 ******************************************************************************/
int16_t Si114xPauseAll(HANDLE si114x_handle)
{
  uint8_t countA, countB;
  int8_t  retval;


  //  After a RESET, if the Si114x receives a command (including NOP) before the
  //  Si114x has gone to sleep, the chip hangs. This first while loop avoids
  //  this.  The reading of the REG_CHIPSTAT does not disturb the internal MCU.
  //

   retval = 0; //initialize data so that we guarantee to enter the loop
   while(retval != 0x01)
   {
     retval = Si114xReadFromRegister( si114x_handle, REG_CHIP_STAT);
     if (retval != 0x01)
     {
       delay_1ms();
     }
   }

  countA = 0;
  while(countA < LOOP_TIMEOUT_MS)
  {
    countB = 0;
    // Keep sending nops until the response is zero
    while(countB < LOOP_TIMEOUT_MS)
    {
      retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
      if( retval == 0 )
          break;
      else
      {
        // Send the NOP Command to clear any error...we cannot use Si114xNop()
        // because it first checks if REG_RESPONSE < 0 and if so it does not
        // perform the cmd. Since we have a saturation REG_RESPONSE will be <0
        Si114xWriteToRegister(si114x_handle, REG_COMMAND, 0x00);
      }
      countB++;
      delay_1ms();
    }

    // Pause the device
    _PsAlsPause(si114x_handle);

    countB = 0;
    // Wait for response
    while(countB < LOOP_TIMEOUT_MS)
    {
      retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
      if( retval !=0 )
        break;
      countB++;
      delay_1ms();
    }

    // When the PsAlsPause() response is good, we expect it to be a '1'.
    retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
    if( retval == 1 )
      break;  // otherwise, start over.
    countA++;
  }
  return 0;
}

/******************************************************************************/
/***********   INCLUDE_SI114x_COMPRESS_CODE  **********************************/
/******************************************************************************/
#if( INCLUDE_SI114X_COMPRESS_CODE == 1 )
//
// The goal of uncompress is to arrive at a 16-bit value, when the input is a
// single byte of information.
//
// The approach taken here is to reuse the floating point concept, but apply it
// to this. Just as it is possible to store relatively large numbers using an
// IEEE 754 representation of a 32 bit value, we make use of a similar concept.
//
// In IEEE 754 representation, there consists of concept of a signed exponent,
// and a signed significand. The signed exponent allows representation of
// values between 2^127 to 2^-128. The signficand is also signed.
//
// The term 'significand' is the integer bit plus the fraction. The 'fraction'
// is the fractional part of the significand.
//
//              IEEE Single Precision Format
//
//     |  b31   |   b30 to  b23   |  bit22 to bit0  |
//     |  Sign  | Signed Exponent |   Fraction      |
//
// In what we need, we do not need signed exponents nor do we need signed
// significands. So, we use an unsigned exponent representation and an unsigned
// significand.
//
// uncompress takes an input byte and interprets the first 4 bits as an
// exponent, and the last 4 bits as a fraction, with an implicit integer bit
//
// The mathematical representation is similar to the concept for floating point
// numbers. First off, the bit field 7:4 is the Exponent, and the bit field 3:0
// is the fractional part of the significand.
//
//
//      | b7 b6 b5 b4 | b3 b2 b1 b0 |
//      |   unsigned  |             |
//      |   Exponent  |  Fraction   |
//
// The number representation is:
//
//      ( 2 ^ Exponent ) * 1.Fraction
//
// Note the 'implicit integer bit'. Normally, the hidden integer is 1. However,
// there is an exception. If the Exponent is zero, the representation
// becomes the following:
//
//      ( 2 ^ 0 ) * 0.Fraction
//
// This is the concept called the 'denormalized number' identical to the IEEE
// 754 representation of floating point numbers. Concept isn't new... this
// allows us to represent the value 0.
//
// Let's go through one example...
//
// Let's say input is 0x9A.
//
//     Exponent = 9
//     Fraction = A
//
// Since the Exponent is non-zero, the number representation is:
//
//     2 ^ 9 * (1.1010)
//
// So, we take 1.1010 and shift left by 9 positions. It is best illustrated in
// binary...
//
//    1.1010 << 9 = 1 1010 00000 = 0x340
//
// The main advantage is that it allows a very large range dynamic range
// to be represented in 8 bits. The largest number that can be represented
// is 0xFF, and this translates to:
//
//     2 ^ 15 * 1.1111
//
//     1.1111 << 15 = 1111 1000 0000 0000 = 0xF800
//
// When the exponent is less than 4, notice that the fraction bits are
// truncated. What this means is that there can be multiple ways of getting an
// output from 0 to
// the value '0x0000' to 0x000F
//
// To illustrate the case where exponents are less than 4:
//      Input     Output
//      00        0000
//      02        0000
//      08        0001
//      0A        0001
//      10        0002
//      14        0002
//      18        0003
//      1A        0003
//      20        0004
//      24        0005
//      28        0006
//      2c        0007
//      30        0008
//      32        0009
//      34        000a
//      36        000b
//      38        000c
//      3c        000e
//      3e        000f
//
// At exponent of 4 or greater, the fraction bits are no longer being thrown
// away, so, we now have linear values
//      40        0010
//      41        0011
//      42        0012
//      43        0013
//      44        0014
//
// But alas, once the exponent is greater than 4, we now stuff the lower
// fractional bits with zero, and we begin to skip numbers...
//      50        0020
//      51        0022
//      52        0024
//      53        0026
//      54        0028
//
// Well...strictly speaking, the IEEE format treats the largest possible
// exponent as 'infinity' or NAN. Let's not go there... Denorm concept is useful
// for us since it allows us to represent zero. However, infinity or NAN
// concepts are not useful for us.
//

/***************************************************************************//**
 * @brief
 *   Converts an 8-bit compressed value to 16-bit
 * @param[in] input
 *   The 8-bit compressed input to be uncompressed
 * @return
 *   uncompressed value
 * @note The Si1132, Si1145/6/7 does not make use of 8-bit
 * compressed values and does not need this.
 ******************************************************************************/
uint16_t Uncompress(uint8_t input) // It is important for the input to be
                                   // unsigned 8-bit.
{
  uint16_t output   = 0;
  uint8_t  exponent = 0;

  // Handle denorm case where exponent is zero. In this case, we are
  // evaluating the value with the integer bit is zero (0.F). So, we round up
  // if the fraction represents a value of 1/2 or greater. Since the fraction
  // is 4 bits, an input of less than 8/16 is less than half. If less than
  // half, return zero. Otherwise, we know that we will return a 1 later.
  //
  if( input < 8 ) return 0;

  //
  // At this point, the exponent is non-zero, so, put in the implicit
  // fraction. Note that when we get the input, it comes in already shifted
  // by 4. So, we are dealing with a value already 4 times larger than the
  // actual starting point.
  //
  // Never fear... we just make an adjustment to the exponent and shift
  // left/right accordingly. The result will be the same as the floating
  // point concept described above.
  //

  exponent = (input & 0xF0 ) >> 4;      // extracts the exponent
  output = 0x10  | (input & 0x0F);      // extracts the fraction and adds
                                        // in the implicit integer

  if( exponent >= 4 ) return ( output << (exponent-4) );
  return( output >> (4-exponent) );
}


// --------------------------------------------------------------------
// What if someone wants to do the inverse function?
//
// Let's say we want to figure out what byte value best represents the number
// of 32 KHz timer ticks for 500 ms.
//
// We start of by knowing how many 32 KHz cycles are in that given time period.
// Let's say that we want to have the RTC wake up every 500 ms.
//
//     500 ms * 32 KHz = 16000 cycles
//
// Then, we take the calculator, and find out what 64 looks like from a binary
// value viewpoint. Using a hex calculator, we see that:
//
//     16000 = 11111010000000
//
//     ... in floating point representation...
//
//           = 11111010000000.00000
//
// The next step is to normalize the value. Normalizing the value means that
// we represent the value in 1.F format. We do this by moving the decimal value
// left until we get the 1.F representation. The number of times we move the
// decimal point left is the exponent. Since we need to move the decimal point
// left before we get to the 1.F represenation...
//
//     16000 = 2^13 * 1.1111010000000
//
// The exponent is therefore 13, and the digits to the right hand side of the
// decimal point is the fraction. What we need is the the first four fractional
// bits. The first four fraction bits is 1111. We truncate the rest,
// unfortunately.
//
// Therefore, the nearest byte representation for 500 ms is 0xDF
//
// Notice that if you plugged in 0xDF into this uncompress function, you will
// get 496 ms. The reason we didn't quite get 500 ms is that we had to throw
// away the 6th fractional bit.
//
// Anyway, this leads us to the following function. This function takes in a
// 16-bit value and compresses it.

/***************************************************************************//**
 * @brief
 *   Converts a 16-bit value to 8-bit value
 * @param[in] input
 *   The 16-bit input to be compressed
 * @retval 0-255
 *   The compressed value
 * @retval <0
 *   Error
 * @note The Si1132, Si1145/6/7 does not make use of 8-bit
 * compressed values and does not need this.
 ******************************************************************************/
uint8_t Compress(uint16_t input) // input should be a 16-bit unsigned value
{
  uint32_t tmp         = 0;
  uint32_t exponent    = 0;
  uint32_t significand = 0;

  if(input==0)
    return 0;


  // handle denorm cases
  // There are multiple answers to 0x0000 and 0x0001 input due to rounding
  // error introduced throught the gradual underflow
  //     Answer for 0x0000 is from 0x00 to 0x07
  //     Answer for 0x0001 is from 0x08 to 0x0F
  // We will just 'pick one' answer.
  if(input == 0x0000) return 0x00;
  if(input == 0x0001) return 0x08;

  // Now we have the denorm cases out of the way, the exponent should be at
  // least one at this point.
  exponent = 0;
  tmp = input;
  while(1)
  {
    tmp >>= 1;  // Shift until there is only the integer in the lease
                //  significant position
    exponent += 1;
    if(tmp == 1)
    {
      break;  // the integer bit has been found. Stop.
    }
  }

  // Once exponent is found, look for the four fractional bits.
  //
  // If the exponent is between 1 to 4, we do not need to do any kind of
  // fractional rounding. Take care of those cases first

  if(exponent < 5) // shift left to align the significant and return the
                    // result
  {
    significand = ( input << (4 - exponent) ) ;
    return ( (exponent << 4) | (significand & 0xF));
  }

  // At this point, we need to calculate the fraction.
  //
  // Easiest way is to align the value so that we have the integer and
  // fraction bits at a known bit position.
  //
  // We then round the signficand to the nearest four fractional bits. To do
  // so, it is best that we also look at the 5th fractional bit and update
  // the 4th fractional bit as necessary. During rounding, it is possible for
  // a carry to occur. If this happens, simply add one to the exponent, and
  // shift the signficand by one to get to the same bit positioning.

  significand = input >> (exponent - 5);

  //
  // After the shift, the significand looks like this since we shift the
  // value by 5 less than the exponent. This is what we expect at this point:
  //
  // bit[15:6]     bit5  bit4  bit3  bit2  bit1  bit0
  //
  //   zeroes       1    2^-1  2^-2  2^-3  2^-4  2^-5
  //
  //                ^    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //               int            fraction
  //

  if(significand & 1) // Check if we need to round up
  {
    significand += 2;   // Increment the 4th fraction (in bit1 position)

    // We then check if a carry occurred due to the addition. If a carry
    // did occur, it would have bumped up the number such that bit6 would
    // be set. Bit6 is 0x0040.
    if(significand & 0x0040)         // Check for a carry
    {
      exponent += 1;                // A carry occurred. Increment the exponent
      significand >>= 1;            // shift the signficand right by one
    }
  }

  // Rounding is done... Encode value and return.
  return ( (exponent << 4) | ( (significand >> 1) & 0xF ) );
}
#endif // INCLUDE_SI114X_COMPRESS_CODE

/******************************************************************************/
/***********   END INCLUDE_SI114X_COMPRESS_CDOE  ******************************/
/******************************************************************************/

/******************************************************************************/
/***********   INCLUDE_SI114x_COMPRESS_CODE  **********************************/
/******************************************************************************/
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
// Start of Calibration Code addition
#define FLT_TO_FX20(x)       ((int32_t)((x*1048576)+.5))
#define FX20_ONE             FLT_TO_FX20( 1.000000)
#define FX20_BAD_VALUE       0xffffffff
/** @endcond */

#if(INCLUDE_SI114X_CALIBRATIONCODE == 1)
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
//                                             msb   lsb   align
//                                             i2c   i2c   ment
//                                             addr  addr
#define SIRPD_ADCHI_IRLED    (collect(buffer, 0x23, 0x22,  0))
#define SIRPD_ADCLO_IRLED    (collect(buffer, 0x22, 0x25,  1))
#define SIRPD_ADCLO_WHLED    (collect(buffer, 0x24, 0x26,  0))
#define VISPD_ADCHI_WHLED    (collect(buffer, 0x26, 0x27,  1))
#define VISPD_ADCLO_WHLED    (collect(buffer, 0x28, 0x29,  0))
#define LIRPD_ADCHI_IRLED    (collect(buffer, 0x29, 0x2a,  1))
#define LED_DRV65            (collect(buffer, 0x2b, 0x2c,  0))

// This is for internal Silabs debug only. Keep it as it is as
// this automatically defines away the embedded debug code
#ifdef SI114x_CAL_DEBUG
#include  "Si114x_cal_debug.c"
#else
#define DEBUG_PRINT_OUTPUT
#define DEBUG_PRINT_OUTPUT_2
#define DEBUG_UCOEF
#endif
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Structure Definition for calref array
 ******************************************************************************/
struct cal_ref_t
{
  uint32_t sirpd_adchi_irled; /**< Small IR PD gain, IR LED, Hi ADC Range     */
  uint32_t sirpd_adclo_irled; /**< Small IR PD gain, IR LED, Lo ADC Range     */
  uint32_t sirpd_adclo_whled; /**< Small IR PD gain, White LED, Lo ADC Range  */
  uint32_t vispd_adchi_whled; /**< VIS PD gain, White LED, Lo ADC Range       */
  uint32_t vispd_adclo_whled; /**< VIS PD gain, White LED, Lo ADC Range       */
  uint32_t lirpd_adchi_irled; /**< Large IR PD gain, IR LED, Hi ADC Range     */
  uint32_t ledi_65ma;         /**< LED Current Ratio at 65 mA                 */
  uint8_t  ucoef[4];          /**< UV Coefficient Storage                     */
};

/***************************************************************************//**
 * @brief
 *   Factory Calibration Reference Values
 ******************************************************************************/
struct cal_ref_t calref[2] =
{
  {
    FLT_TO_FX20( 4.021290),  // sirpd_adchi_irled
    FLT_TO_FX20(57.528500),  // sirpd_adclo_irled
    FLT_TO_FX20( 2.690010),  // sirpd_adclo_whled
    FLT_TO_FX20( 0.042903),  // vispd_adchi_whled
    FLT_TO_FX20( 0.633435),  // vispd_adclo_whled
    FLT_TO_FX20(23.902900),  // lirpd_adchi_irled
    FLT_TO_FX20(56.889300),  // ledi_65ma
    {0x7B, 0x6B, 0x01, 0x00} // default ucoef
  },
  {
    FLT_TO_FX20( 2.325484),  // sirpd_adchi_irled
    FLT_TO_FX20(33.541500),  // sirpd_adclo_irled
    FLT_TO_FX20( 1.693750),  // sirpd_adclo_whled
    FLT_TO_FX20( 0.026775),  // vispd_adchi_whled
    FLT_TO_FX20( 0.398443),  // vispd_adclo_whled
    FLT_TO_FX20(12.190900),  // lirpd_adchi_irled
    FLT_TO_FX20(56.558200),  // ledi_65ma
    {0xdb, 0x8f, 0x01, 0x00} // default ucoef
  }
};

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Converts the 12-bit factory test value from the Si114x and returns the
 *   fixed-point representation of this 12-bit factory test value.
 ******************************************************************************/
static uint32_t decode(uint32_t input)
{
  int32_t  exponent, exponent_bias9;
  uint32_t mantissa;

  if(input==0) return 0.0;

  exponent_bias9 = (input & 0x0f00) >> 8;
  exponent       = exponent_bias9 - 9;

  mantissa       = input & 0x00ff; // fraction
  mantissa       |=        0x0100; // add in integer

  // representation in 12 bit integer, 20 bit fraction
  mantissa       = mantissa << (12+exponent);
  return mantissa;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   The buffer[] is assumed to point to a byte array that containst the
 *   factory calibration values after writing 0x12 to the command register
 *   This function takes the 12 bytes from the Si114x, then converts it
 *   to a fixed point representation, with the help of the decode() function
 ******************************************************************************/
static uint32_t collect(uint8_t* buffer,
                        uint8_t msb_addr,
                        uint8_t lsb_addr,
                        uint8_t alignment)
{
  uint16_t value;
  uint8_t  msb_ind = msb_addr - 0x22;
  uint8_t  lsb_ind = lsb_addr - 0x22;

  if(alignment == 0)
  {
    value =  buffer[msb_ind]<<4;
    value += buffer[lsb_ind]>>4;
  }
  else
  {
    value =  buffer[msb_ind]<<8;
    value += buffer[lsb_ind];
    value &= 0x0fff;
  }

  if(   ( value == 0x0fff )
     || ( value == 0x0000 ) ) return FX20_BAD_VALUE;
  else return decode( value );
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   This performs a shift_left function. For convenience, a negative
 *   shift value will shift the value right. Value pointed will be
 *   overwritten.
 ******************************************************************************/
static void shift_left(uint32_t* value_p, int8_t shift)
{
  if(shift > 0)
    *value_p = *value_p<<shift ;
  else
    *value_p = *value_p>>(-shift) ;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define ALIGN_LEFT   1
#define ALIGN_RIGHT -1
/***************************************************************************//**
 * @brief
 *   Aligns the value pointed by value_p to either the LEFT or RIGHT
 *   the number of shifted bits is returned. The value in value_p is
 *   overwritten.
 ******************************************************************************/
static int8_t align( uint32_t* value_p, int8_t direction )
{
  int8_t   local_shift, shift ;
  uint32_t mask;

  // Check invalid value_p and *value_p, return without shifting if bad.
  if( value_p  == NULL )  return 0;
  if( *value_p == 0 )     return 0;

  // Make sure direction is valid
  switch( direction )
  {
    case ALIGN_LEFT:
      local_shift =  1 ;
      mask  = 0x80000000L;
      break;

    case ALIGN_RIGHT:
      local_shift = -1 ;
      mask  = 0x00000001L;
      break;

    default:
      // Invalid direction, return without shifting
      return 0;
  }

  shift = 0;
  while(1)
  {
    if(*value_p & mask ) break;
    shift++;
    shift_left( value_p, local_shift );
  }
  return shift;
}
/** @endcond */

/***************************************************************************//**
 * @brief
 * This compile switch used only to experiment with
 * various rounding precisions. The flexibility has
 * a small performance price.
 ******************************************************************************/
#define FORCE_ROUND_16 1

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *
 *   The fx20_round was designed to perform rounding to however many significant
 *   digits. However, for the factory calibration code, rounding to 16 always is
 *   sufficient. So, the FORCE_ROUND_16 define is provided just in case it would
 *   be necessary to dynamically choose how many bits to round to.
 *   Rounds the uint32_t value pointed by ptr, by the number of bits
 *   specified by round.
 ******************************************************************************/
static void fx20_round
(
  uint32_t *value_p
  #if !FORCE_ROUND_16
  , int8_t round
  #endif
)
{
  int8_t  shift;

  #if FORCE_ROUND_16
    // Use the following to force round = 16
    uint32_t mask1  = 0xffff8000;
    uint32_t mask2  = 0xffff0000;
    uint32_t lsb    = 0x00008000;
  #else
    // Use the following if you want to routine to be
    // capable of rounding to something other than 16.
    uint32_t mask1  = ((2<<(round))-1)<<(31-(round));
    uint32_t mask2  = ((2<<(round-1))-1)<<(31-(round-1));
    uint32_t lsb    = mask1-mask2;
  #endif

  shift = align( value_p, ALIGN_LEFT );
  if( ( (*value_p)&mask1 ) == mask1 )
  {
    *value_p = 0x80000000;
    shift -= 1;
  }
  else
  {
    *value_p += lsb;
    *value_p &= mask2;
  }

  shift_left( value_p, -shift );
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   The fx20_divide and fx20_multiply uses this structure to pass
 *   values into it.
 ******************************************************************************/
struct operand_t
{
  uint32_t op1;  /**< Operand 1 */
  uint32_t op2;  /**< Operand 2 */
};

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns a fixed-point (20-bit fraction) after dividing op1/op2
 ******************************************************************************/
static uint32_t fx20_divide( struct operand_t* operand_p )
{
  int8_t    numerator_sh=0, denominator_sh=0;
  uint32_t  result;
  uint32_t* numerator_p;
  uint32_t* denominator_p;

  if( operand_p == NULL ) return FX20_BAD_VALUE;

  numerator_p   = &operand_p->op1;
  denominator_p = &operand_p->op2;

  if(   (*numerator_p   == FX20_BAD_VALUE)
      || (*denominator_p == FX20_BAD_VALUE)
      || (*denominator_p == 0             ) ) return FX20_BAD_VALUE;

  fx20_round  ( numerator_p   );
  fx20_round  ( denominator_p );
  numerator_sh   = align ( numerator_p,   ALIGN_LEFT  );
  denominator_sh = align ( denominator_p, ALIGN_RIGHT );

  result = *numerator_p / ( (uint16_t)(*denominator_p) );
  shift_left( &result , 20-numerator_sh-denominator_sh );

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns a fixed-point (20-bit fraction) after multiplying op1*op2
 ******************************************************************************/
static uint32_t fx20_multiply( struct operand_t* operand_p )
{
  uint32_t  result;
  int8_t    val1_sh, val2_sh;
  uint32_t* val1_p;
  uint32_t* val2_p;

  if( operand_p == NULL ) return FX20_BAD_VALUE;

  val1_p = &(operand_p->op1);
  val2_p = &(operand_p->op2);

  fx20_round( val1_p );
  fx20_round( val2_p );

  val1_sh = align( val1_p, ALIGN_RIGHT );
  val2_sh = align( val2_p, ALIGN_RIGHT );


  result = (uint32_t)( ( (uint32_t)(*val1_p) ) * ( (uint32_t)(*val2_p) ) );
  shift_left( &result, -20+val1_sh+val2_sh );

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Due to small differences in factory test setup, the reference calibration
 *   values may have slight variation. This function retrieves the calibration
 *   index stored in the Si114x so that it is possible to know which calibration
 *   reference values to use.
 ******************************************************************************/
static int16_t find_cal_index( uint8_t* buffer )
{
  int16_t index;
  uint8_t size;

  // buffer[12] is the LSB, buffer[13] is the MSB
  index = ( int16_t )( buffer[12] + ( (uint16_t)( buffer[13] ) << 8 ) );

  switch( index )
  {
    case -1:
      index = 0;
      break;
    case -2:
      index = 0;
      break;
    case -3:
      index = 1;
    default:
      index = -(4+index) ;
  }

  size = sizeof(calref)/sizeof(calref[0]);

  if( index < size )
  {
    return  index;
  }
  else
  {
    return -1;
  }
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns the calibration ratio to be applied to VIS measurements
 ******************************************************************************/
static uint32_t vispd_correction(uint8_t* buffer)
{

  struct operand_t op;
  uint32_t         result;
  int16_t          index = find_cal_index( buffer );

  if( index < 0 ) result = FX20_ONE;

  op.op1 = calref[ index ].vispd_adclo_whled;
  op.op2 = VISPD_ADCLO_WHLED;
  result = fx20_divide( &op );

  if( result == FX20_BAD_VALUE ) result = FX20_ONE;

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns the calibration ratio to be applied to IR measurements
 ******************************************************************************/
static uint32_t irpd_correction(uint8_t* buffer)
{
  struct operand_t op;
  uint32_t         result;
  int16_t          index = find_cal_index( buffer );

  if( index < 0 ) result = FX20_ONE;

  // op.op1 = SIRPD_ADCLO_IRLED_REF; op.op2 = SIRPD_ADCLO_IRLED;
  op.op1 = calref[ index ].sirpd_adclo_irled;
  op.op2 = SIRPD_ADCLO_IRLED;
  result = fx20_divide( &op );

  if( result == FX20_BAD_VALUE ) result = FX20_ONE;

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns the ratio to correlate between x_RANGE=0 and x_RANGE=1
 *   It is typically 14.5, but may have some slight component-to-component
 *   differences.
 ******************************************************************************/
static uint32_t adcrange_ratio(uint8_t* buffer)
{
  struct operand_t op;
  uint32_t         result;

  op.op1 = SIRPD_ADCLO_IRLED  ; op.op2 = SIRPD_ADCHI_IRLED  ;
  result = fx20_divide( &op );

  if( result == FX20_BAD_VALUE ) result = FLT_TO_FX20( 14.5 );

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns the ratio to correlate between measurements made from large PD
 *   to measurements made from small PD.
 ******************************************************************************/
static uint32_t irsize_ratio(uint8_t* buffer)
{
  struct operand_t op;
  uint32_t         result;

  op.op1 = LIRPD_ADCHI_IRLED  ; op.op2 = SIRPD_ADCHI_IRLED  ;

  result = fx20_divide( &op );

  if( result == FX20_BAD_VALUE ) result = FLT_TO_FX20(  6.0 );

  return  result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   Returns the ratio to adjust for differences in IRLED drive strength. Note
 *   that this does not help with LED irradiance variation.
 ******************************************************************************/
static uint32_t ledi_ratio(uint8_t* buffer)
{

  struct operand_t op;
  uint32_t         result;
  int16_t          index;

  index = find_cal_index( buffer );

  if( index < 0 ) result = FX20_ONE;

  // op.op1 = LED_DRV65_REF; op.op2 = LED_DRV65;
  op.op1 = calref[ index ].ledi_65ma;
  op.op2 = LED_DRV65;
  result = fx20_divide( &op );

  if( result == FX20_BAD_VALUE ) result = FX20_ONE;

  return result;
}
/** @endcond */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/***************************************************************************//**
 * @brief
 *   This is a helper function called from si114x_get_calibration()
 *   Writes 0x11 to the Command Register, then populates buffer[12]
 *   and buffer[13] with the factory calibration index
 ******************************************************************************/
static int16_t si114x_get_cal_index( HANDLE si114x_handle, uint8_t* buffer )
{
  int16_t retval;
  uint8_t response;

  if( ( si114x_handle == NULL ) || ( buffer == NULL ) )
    return -1;

  // Check to make sure that the device is ready to receive commands
  do
  {
    retval = Si114xNop( si114x_handle );
    if( retval != 0 ) return -1;


    response = Si114xReadFromRegister( si114x_handle, REG_RESPONSE );
    if (response != 0)
    {
      delay_1ms();
    }
  } while( response != 0 );

  // Retrieve the index
  retval = Si114xWriteToRegister( si114x_handle, REG_COMMAND, 0x11 );
  _waitUntilSleep(si114x_handle);

  if( retval != 0 ) return -1;

  retval = Si114xBlockRead( si114x_handle, REG_PS1_DATA0, 2, &(buffer[12]) );
  if( retval != 0 ) return -1;

  return 0;
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Populates the SI114X_CAL_S structure
 * @details
 *   Performs some initial checking based on the security
 *   level. If the checks fail, the function returns without attempting
 *   to fetch calibration values. The reason for the checking is that the
 *   Si114x uses the same registers to store calibration values as used for
 *   storing proximity and ambient light measurements. Therefore, this function
 *   needs to be used only if there is no possibility of an autonomous process
 *   possibly overwriting the output registers.
 *
 *   If the checks are successful, then the si114x retrieves the compressed
 *   values from the Si114x, then populates the SI114X_CAL_S   structure whose
 *   pointer is passed to si114x_calibration()
 *
 *   If there are any errors, si114x_cal   is populated with default values
 *
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @param[out] si114x_cal
 *   Points to the SI114X_CAL_S structure that will hold the calibration values
 *   from the Si114x. If there are any errors, si114x_cal is populated with
 *   default values.
 * @param[in] security
 * >        0            Minimal checking
 * >
 * >        1            Checks to make sure that interface registers
 * >                     are zero, otherwise, returns -1
 * >                     interface registers are zero only when the Si114x
 * >                     has been reset, and no autonomous measurements have
 * >                     started.
 * @retval   0
 *  Success
 * @retval  -1
 *   Security Check failed
 * @retval  -2
 *   i2c communication error
 * @retval  -3
 *   Chip does not support factory calibration
 * @retval  -4
 *   Null pointers found for si114x_handle or si114x_cal
 ******************************************************************************/
/*
 * Side-effects:
 *     - Writes 0x11 to command reg to retrieve factory calibration values in
 *       buffer[0] to buffer[11]
 *
 *     - Calls the various helper functions such as vispd_correction()
 *       irpd_correction, to populate the SI114X_CAL_S structure
 *
 *     - Writes 0x12 to command reg to retrieve factory cal_index to
 *       buffer[12] to buffer[13]
 ******************************************************************************/
int16_t si114x_get_calibration( HANDLE        si114x_handle,
                                SI114X_CAL_S* si114x_cal,
                                uint8_t       security)
{
  uint8_t buffer[14];
  int16_t retval = 0;
  uint8_t response;

  if( si114x_handle == NULL ) { retval = -4; goto error_exit; }

  if( si114x_cal    == NULL ) { retval = -4; goto error_exit; }

  // if requested, check to make sure the interface registers are zero
  // as an indication of a device that has not started any autonomous
  // operation
  if( security == 1 )
  {
    int8_t i;

    retval = Si114xBlockRead( si114x_handle, REG_ALS_VIS_DATA0, 12, buffer );
    if( retval != 0 ) { retval = -2; goto error_exit; }

    for( i=0; i<12; i++)
    {
        if( buffer[i] != 0 ) { retval = -1; goto error_exit; }
    }

    DEBUG_PRINT_OUTPUT;

  }

  // Check to make sure that the device is ready to receive commands
  do
  {
    retval = Si114xNop( si114x_handle );
    if( retval != 0 ) { retval = -2; goto error_exit; }


    response = Si114xReadFromRegister( si114x_handle, REG_RESPONSE );
    if (response != 0)
    {
      delay_1ms();
    }
  } while( response != 0 );

  // Request for the calibration data
  retval = Si114xWriteToRegister( si114x_handle, REG_COMMAND, 0x12 );
  _waitUntilSleep(si114x_handle);

  if( retval != 0 ) { retval = -2; goto error_exit; }

  // Wait for the response register to increment
  do
  {
    response = Si114xReadFromRegister( si114x_handle, REG_RESPONSE );
    // If the upper nibbles are non-zero, something is wrong
    if( response == 0x80 )
    {
      // calibration code has not been implemented on this device
      // leading to command error. So, rather than returning an
      // error, handle the error by Nop and set ratios to -1.0
      // and return normally.
      Si114xNop( si114x_handle );
      retval = -3;
      goto error_exit;
    }
    else if( response & 0xfff0 )
    {
      // if upper nibble is anything but 0x80, exit with an error
      retval = -2;
      goto error_exit;
    }
    if (response != 1)
    {
      delay_1ms();
    }
  } while( response != 1 );

  // Retrieve the 12 bytes from the interface registers
  retval = Si114xBlockRead( si114x_handle, REG_ALS_VIS_DATA0, 12, buffer );
  if( retval != 0 ) { retval = -2; goto error_exit; }

  DEBUG_PRINT_OUTPUT;

  retval=si114x_get_cal_index( si114x_handle, buffer );

  if( retval != 0 )
  {
    retval = -2; goto error_exit;
  }

  si114x_cal->ledi_ratio       = ledi_ratio(buffer);
  si114x_cal->vispd_correction = vispd_correction(buffer);
  si114x_cal->irpd_correction  = irpd_correction(buffer);
  si114x_cal->adcrange_ratio   = adcrange_ratio(buffer);
  si114x_cal->ucoef_p          = calref[find_cal_index(buffer)].ucoef;
  si114x_cal->irsize_ratio     = irsize_ratio(buffer);

  DEBUG_PRINT_OUTPUT_2;

  return 0;

error_exit:
  si114x_cal->vispd_correction = FX20_ONE;
  si114x_cal->irpd_correction  = FX20_ONE;
  si114x_cal->adcrange_ratio   = FLT_TO_FX20( 14.5 );
  si114x_cal->irsize_ratio     = FLT_TO_FX20(  6.0 );
  si114x_cal->ledi_ratio       = FX20_ONE;
  si114x_cal->ucoef_p          = NULL;
  return retval;
}

/***************************************************************************//**
 * @brief
 *   Initializes the Si113x/46/47/48 UCOEF Registers.
 *
 * @details
 *   Takes in an optional input ucoef pointer, then modifies
 *   it based on calibration. If the input ucoef is NULL, default values for
 *   clear overlay is assumed and then the si114x_cal is used to adjust it
 *   before setting the ucoef. Note that the Si114x ucoef registers are
 *   updated by this function; no additional action is required. This routine
 *   also performs the necessary querying of the chip identification to make
 *   sure that it is a uvindex-capable device.
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @param[in] input_ucoef
 *   if NULL, a clear overlay is assumed, and datasheet values for ucoef is
 *   used. Otherwise, pointer to 4 bytes array representing the reference
 *   coefficients is passed.
 * @param[in] si114x_cal
 *   Points to the SI114X_CAL_S structure that holds the calibration values
 *   from the Si113x/4x
 * @retval   0
 *   Success
 * @retval  -1
 *   The device is neither Si1132, Si1145, Si1146 nor Si1147
 * @retval  <-1
 *   Error
 ******************************************************************************/
int16_t si114x_set_ucoef( HANDLE        si114x_handle,
                          uint8_t*      input_ucoef,
                          SI114X_CAL_S* si114x_cal )
{
  int8_t           response;
  uint8_t          temp;
  uint32_t         vc=FX20_ONE, ic=FX20_ONE, long_temp;
  struct operand_t op;
  uint8_t*         ref_ucoef = si114x_cal->ucoef_p;
  uint8_t          out_ucoef[4];

  if( input_ucoef != NULL ) ref_ucoef = input_ucoef;

  if( ref_ucoef == NULL )   return -1 ;

  // retrieve part identification
  response = Si114xReadFromRegister( si114x_handle, REG_PART_ID );
  switch( response )
  {
    case 0x32: case 0x45: case 0x46: case 0x47: temp = 1; break;
    default:                                    temp = 0; break;
  }
  if( !temp ) return -1;

  if(si114x_cal != 0)
  {
    if(si114x_cal->vispd_correction > 0) vc = si114x_cal->vispd_correction;
    if(si114x_cal->irpd_correction  > 0) ic = si114x_cal->irpd_correction;
  }

  op.op1 = ref_ucoef[0] + ((ref_ucoef[1])<<8);
  op.op2 = vc;
  long_temp   = fx20_multiply( &op );
  out_ucoef[0] = (long_temp & 0x00ff);
  out_ucoef[1] = (long_temp & 0xff00)>>8;

  op.op1 = ref_ucoef[2] + (ref_ucoef[3]<<8);
  op.op2 = ic;
  long_temp   = fx20_multiply( &op );
  out_ucoef[2] = (long_temp & 0x00ff);
  out_ucoef[3] = (long_temp & 0xff00)>>8;

  DEBUG_UCOEF

  response = Si114xBlockWrite( si114x_handle, REG_UCOEF0 , 4, out_ucoef);

  return response;
}
#else // INCLUDE_SI114X_CALIBRATION_CODE

/********************   STUB FUNCTIONS ONLY  **********************************/

int16_t si114x_get_calibration( HANDLE        si114x_handle,
                                SI114X_CAL_S* si114x_cal,
                                uint8_t       security)
{
  // although the SI114x_CAL_S structure is not filled up properly, the
  // set_ucoef() function will not use it.
  return 0;
}

int16_t si114x_set_ucoef( HANDLE        si114x_handle,
                          uint8_t*      input_ucoef,
                          SI114X_CAL_S* si114x_cal )
{
  int16_t response;
  uint8_t code ucoef[4] = { 0x7B, 0x6B, 0x01, 0x00 } ;

  // This will write 4 bytes starting with I2C address 0x13
  response = Si114xBlockWrite( si114x_handle, REG_UCOEF0, 4, &ucoef[0] );
  return response;
}
#endif // INCLUDE_SI114X_CALIBRATION_CODE

/******************************************************************************/
/***********   END INCLUDE_SI114x_COMPRESS_CODE  ******************************/
/******************************************************************************/

