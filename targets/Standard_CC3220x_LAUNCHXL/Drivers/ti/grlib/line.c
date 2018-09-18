/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include <ti/grlib/grlib.h>

//*****************************************************************************
//
//! \addtogroup line_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x1 is the X coordinate of one end of the line.
//! \param x2 is the X coordinate of the other end of the line.
//! \param y is the Y coordinate of the line.
//!
//! This function draws a horizontal line, taking advantage of the fact that
//! the line is horizontal to draw it more efficiently.  The clipping of the
//! horizontal line to the clipping rectangle is performed within this routine;
//! the display driver's horizontal line routine is used to perform the actual
//! line drawing.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawLineH(const Graphics_Context *context,
                        int32_t x1,
                        int32_t x2,
                        int32_t y)
{
    int32_t temp;

    //
    // Check the arguments.
    //
    assert(context);

    //
    // If the Y coordinate of this line is not in the clipping region, then
    // there is nothing to be done.
    //
    if((y < context->clipRegion.yMin) ||
       (y > context->clipRegion.yMax))
    {
        return;
    }

    //
    // Swap the X coordinates if the first is larger than the second.
    //
    if(x1 > x2)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }

    //
    // If the entire line is outside the clipping region, then there is nothing
    // to be done.
    //
    if((x1 > context->clipRegion.xMax) ||
       (x2 < context->clipRegion.xMin))
    {
        return;
    }

    //
    // Clip the starting coordinate to the left side of the clipping region if
    // required.
    //
    if(x1 < context->clipRegion.xMin)
    {
        x1 = context->clipRegion.xMin;
    }

    //
    // Clip the ending coordinate to the right side of the clipping region if
    // required.
    //
    if(x2 > context->clipRegion.xMax)
    {
        x2 = context->clipRegion.xMax;
    }

    //
    // Call the low level horizontal line drawing routine.
    //
    Graphics_drawHorizontalLineOnDisplay(context->display, x1, x2, y,
                                         context->foreground);
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the line.
//! \param y1 is the Y coordinate of one end of the line.
//! \param y2 is the Y coordinate of the other end of the line.
//!
//! This function draws a vertical line, taking advantage of the fact that the
//! line is vertical to draw it more efficiently.  The clipping of the vertical
//! line to the clipping rectangle is performed within this routine; the
//! display driver's vertical line routine is used to perform the actual line
//! drawing.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawLineV(const Graphics_Context *context,
                        int32_t x,
                        int32_t y1,
                        int32_t y2)
{
    int32_t temp;

    //
    // Check the arguments.
    //
    assert(context);

    //
    // If the X coordinate of this line is not within the clipping region, then
    // there is nothing to be done.
    //
    if((x < context->clipRegion.xMin) ||
       (x > context->clipRegion.xMax))
    {
        return;
    }

    //
    // Swap the Y coordinates if the first is larger than the second.
    //
    if(y1 > y2)
    {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    //
    // If the entire line is out of the clipping region, then there is nothing
    // to be done.
    //
    if((y1 > context->clipRegion.yMax) ||
       (y2 < context->clipRegion.yMin))
    {
        return;
    }

    //
    // Clip the starting coordinate to the top side of the clipping region if
    // required.
    //
    if(y1 < context->clipRegion.yMin)
    {
        y1 = context->clipRegion.yMin;
    }

    //
    // Clip the ending coordinate to the bottom side of the clipping region if
    // required.
    //
    if(y2 > context->clipRegion.yMax)
    {
        y2 = context->clipRegion.yMax;
    }

    //
    // Call the low level vertical line drawing routine.
    //
    Graphics_drawVerticalLineOnDisplay(context->display, x, y1, y2,
                                       context->foreground);
}

//*****************************************************************************
//
//! Computes the clipping code used by the Cohen-Sutherland clipping algorithm.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the point.
//! \param y is the Y coordinate of the point.
//!
//! This function computes the clipping code used by the Cohen-Sutherland
//! clipping algorithm.  Clipping is performed by classifying the endpoints of
//! the line based on their relation to the clipping region; this determines
//! those relationships.
//!
//! \return Returns the clipping code.
//
//*****************************************************************************
static int32_t Graphics_getClipCode(const Graphics_Context *context,
                                    int32_t x,
                                    int32_t y)
{
    int32_t code;

    //
    // Initialize the clipping code to zero.
    //
    code = 0;

    //
    // Set bit zero of the clipping code if the Y coordinate is above the
    // clipping region.
    //
    if(y < context->clipRegion.yMin)
    {
        code |= 1;
    }

    //
    // Set bit one of the clipping code if the Y coordinate is below the
    // clipping region.
    //
    if(y > context->clipRegion.yMax)
    {
        code |= 2;
    }

    //
    // Set bit two of the clipping code if the X coordinate is to the left of
    // the clipping region.
    //
    if(x < context->clipRegion.xMin)
    {
        code |= 4;
    }

    //
    // Set bit three of the clipping code if the X coordinate is to the right
    // of the clipping region.
    //
    if(x > context->clipRegion.xMax)
    {
        code |= 8;
    }

    //
    // Return the clipping code.
    //
    return(code);
}

//*****************************************************************************
//
//! Clips a line to the clipping region.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x1 is the X coordinate of the start of the line.
//! \param y1 is the Y coordinate of the start of the line.
//! \param x2 is the X coordinate of the end of the line.
//! \param y2 is the Y coordinate of the end of the line.
//!
//! This function clips a line to the extents of the clipping region using the
//! Cohen-Sutherland clipping algorithm.  The ends of the line are classified
//! based on their relation to the clipping region, and the codes are used to
//! either trivially accept a line (both end points within the clipping
//! region), trivially reject a line (both end points to one side of the
//! clipping region), or to adjust an endpoint one axis at a time to the edge
//! of the clipping region until the line can either be trivially accepted or
//! trivially rejected.
//!
//! The provided coordinates are modified such that they reside within the
//! extents of the clipping region if the line is not rejected.  If it is
//! rejected, the coordinates may be modified during the process of attempting
//! to clip them.
//!
//! \return Returns one if the clipped line lies within the extent of the
//! clipping region and zero if it does not.
//
//*****************************************************************************
static int32_t Graphics_clipLine(const Graphics_Context *context,
                                 int32_t  *x1,
                                 int32_t  *y1,
                                 int32_t  *x2,
                                 int32_t  *y2)
{
    int32_t code, code1, code2, x, y;

    //
    // Compute the clipping codes for the two endpoints of the line.
    //
    code1 = Graphics_getClipCode(context, *x1, *y1);
    code2 = Graphics_getClipCode(context, *x2, *y2);

    //
    // Loop forever.  This loop will be explicitly broken out of when the line
    // is either trivially accepted or trivially rejected.
    //
    while(1)
    {
        //
        // If both codes are zero, then both points lie within the extent of
        // the clipping region.  In this case, trivally accept the line.
        //
        if((code1 == 0) && (code2 == 0))
        {
            return(1);
        }

        //
        // If the intersection of the codes is non-zero, then the line lies
        // entirely off one edge of the clipping region.  In this case,
        // trivally reject the line.
        //
        if((code1 & code2) != 0)
        {
            return(0);
        }

        //
        // Determine the end of the line to move.  The first end of the line is
        // moved until it is within the clipping region, and then the second
        // end of the line is moved until it is also within the clipping
        // region.
        //
        if(code1)
        {
            code = code1;
        }
        else
        {
            code = code2;
        }

        //
        // See if this end of the line lies above the clipping region.
        //
        if(code & 1)
        {
            //
            // Move this end of the line to the intersection of the line and
            // the top of the clipping region.
            //
            x = (*x1 + (((*x2 - *x1) *
                         (context->clipRegion.yMin - *y1)) /
                        (*y2 - *y1)));
            y = context->clipRegion.yMin;
        }
        //
        // Otherwise, see if this end of the line lies below the clipping
        // region.
        //
        else if(code & 2)
        {
            //
            // Move this end of the line to the intersection of the line and
            // the bottom of the clipping region.
            //
            x = (*x1 + (((*x2 - *x1) *
                         (context->clipRegion.yMax - *y1)) /
                        (*y2 - *y1)));
            y = context->clipRegion.yMax;
        }
        //
        // Otherwise, see if this end of the line lies to the left of the
        // clipping region.
        //
        else if(code & 4)
        {
            //
            // Move this end of the line to the intersection of the line and
            // the left side of the clipping region.
            //
            x = context->clipRegion.xMin;
            y = (*y1 + (((*y2 - *y1) *
                         (context->clipRegion.xMin - *x1)) /
                        (*x2 - *x1)));
        }
        //
        // Otherwise, this end of the line lies to the right of the clipping
        // region.
        //
        else
        {
            //
            // Move this end of the line to the intersection of the line and
            // the right side of the clipping region.
            //
            x = context->clipRegion.xMax;
            y = (*y1 + (((*y2 - *y1) *
                         (context->clipRegion.xMax - *x1)) /
                        (*x2 - *x1)));
        }

        //
        // See which end of the line just moved.
        //
        if(code1)
        {
            //
            // Save the new coordinates for the start of the line.
            //
            *x1 = x;
            *y1 = y;

            //
            // Recompute the clipping code for the start of the line.
            //
            code1 = Graphics_getClipCode(context, x, y);
        }
        else
        {
            //
            // Save the new coordinates for the end of the line.
            //
            *x2 = x;
            *y2 = y;

            //
            // Recompute the clipping code for the end of the line.
            //
            code2 = Graphics_getClipCode(context, x, y);
        }
    }
}

//*****************************************************************************
//
//! Draws a line.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x1 is the X coordinate of the start of the line.
//! \param y1 is the Y coordinate of the start of the line.
//! \param x2 is the X coordinate of the end of the line.
//! \param y2 is the Y coordinate of the end of the line.
//!
//! This function draws a line, utilizing Graphics_drawLineH() and
//! Graphics_drawLineV() to draw the line as efficiently as possible.  The line
//! is clipped to the clippping rectangle using the Cohen-Sutherland clipping
//! algorithm, and then scan converted using Bresenham's line drawing algorithm.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawLine(const Graphics_Context *context,
                       int32_t x1,
                       int32_t y1,
                       int32_t x2,
                       int32_t y2)
{
    int32_t error, deltaX, deltaY, yStep;
    bool steep;

    //
    // Check the arguments.
    //
    assert(context);

    //
    // See if this is a vertical line.
    //
    if(x1 == x2)
    {
        //
        // It is more efficient to avoid Bresenham's algorithm when drawing a
        // vertical line, so use the vertical line routine to draw this line.
        //
        Graphics_drawLineV(context, x1, y1, y2);

        //
        // The line has ben drawn, so return.
        //
        return;
    }

    //
    // See if this is a horizontal line.
    //
    if(y1 == y2)
    {
        //
        // It is more efficient to avoid Bresenham's algorithm when drawing a
        // horizontal line, so use the horizontal line routien to draw this
        // line.
        //
        Graphics_drawLineH(context, x1, x2, y1);

        //
        // The line has ben drawn, so return.
        //
        return;
    }

    //
    // Clip this line if necessary, and return without drawing anything if the
    // line does not cross the clipping region.
    //
    if(Graphics_clipLine(context, &x1, &y1, &x2, &y2) == 0)
    {
        return;
    }

    //
    // Determine if the line is steep.  A steep line has more motion in the Y
    // direction than the X direction.
    //
    if(((y2 > y1) ? (y2 - y1) : (y1 - y2)) >
       ((x2 > x1) ? (x2 - x1) : (x1 - x2)))
    {
        steep = true;
    }
    else
    {
        steep = false;
    }

    //
    // If the line is steep, then swap the X and Y coordinates.
    //
    if(steep)
    {
        error = x1;
        x1 = y1;
        y1 = error;
        error = x2;
        x2 = y2;
        y2 = error;
    }

    //
    // If the starting X coordinate is larger than the ending X coordinate,
    // then swap the start and end coordinates.
    //
    if(x1 > x2)
    {
        error = x1;
        x1 = x2;
        x2 = error;
        error = y1;
        y1 = y2;
        y2 = error;
    }

    //
    // Compute the difference between the start and end coordinates in each
    // axis.
    //
    deltaX = x2 - x1;
    deltaY = (y2 > y1) ? (y2 - y1) : (y1 - y2);

    //
    // Initialize the error term to negative half the X delta.
    //
    error = -deltaX / 2;

    //
    // Determine the direction to step in the Y axis when required.
    //
    if(y1 < y2)
    {
        yStep = 1;
    }
    else
    {
        yStep = -1;
    }

    //
    // Loop through all the points along the X axis of the line.
    //
    for(; x1 <= x2; x1++)
    {
        //
        // See if this is a steep line.
        //
        if(steep)
        {
            //
            // Plot this point of the line, swapping the X and Y coordinates.
            //
            Graphics_drawPixelOnDisplay(context->display, y1, x1,
                                        context->foreground);
        }
        else
        {
            //
            // Plot this point of the line, using the coordinates as is.
            //
            Graphics_drawPixelOnDisplay(context->display, x1, y1,
                                        context->foreground);
        }

        //
        // Increment the error term by the Y delta.
        //
        error += deltaY;

        //
        // See if the error term is now greater than zero.
        //
        if(error > 0)
        {
            //
            // Take a step in the Y axis.
            //
            y1 += yStep;

            //
            // Decrement the error term by the X delta.
            //
            error -= deltaX;
        }
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
