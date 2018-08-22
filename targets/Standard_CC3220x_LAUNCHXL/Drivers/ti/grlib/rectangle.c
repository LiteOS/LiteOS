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
//! \addtogroup rectangle_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a rectangle.
//!
//! \param context is a pointer to the drawing context to use.
//! \param rect is a pointer to the structure containing the extents of the
//! rectangle.
//!
//! This function draws a rectangle.  The rectangle will extend from \e xMin
//! to \e xMax and \e yMin to \e yMax, inclusive.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawRectangle(const Graphics_Context *context,
                            const Graphics_Rectangle *rect)
{
    //
    // Check the arguments.
    //
    assert(context);
    assert(rect);

    //
    // Draw a line across the top of the rectangle.
    //
    Graphics_drawLineH(context, rect->xMin, rect->xMax, rect->yMin);

    //
    // Return if the rectangle is one pixel tall.
    //
    if(rect->yMin == rect->yMax)
    {
        return;
    }

    //
    // Draw a line down the right side of the rectangle.
    //
    Graphics_drawLineV(context, rect->xMax, rect->yMin + 1, rect->yMax);

    //
    // Return if the rectangle is one pixel wide.
    //
    if(rect->xMin == rect->xMax)
    {
        return;
    }

    //
    // Draw a line across the bottom of the rectangle.
    //
    Graphics_drawLineH(context, rect->xMax - 1, rect->xMin, rect->yMax);

    //
    // Return if the rectangle is two pixels tall.
    //
    if((rect->yMin + 1) == rect->yMax)
    {
        return;
    }

    //
    // Draw a line up the left side of the rectangle.
    //
    Graphics_drawLineV(context, rect->xMin, rect->yMax - 1, rect->yMin + 1);
}

//*****************************************************************************
//
//! Draws a filled rectangle.
//!
//! \param context is a pointer to the drawing context to use.
//! \param rect is a pointer to the structure containing the extents of the
//! rectangle.
//!
//! This function draws a filled rectangle.  The rectangle will extend from
//! \e xMin to \e xMax and \e yMin to \e yMax, inclusive.  The clipping of
//! the rectangle to the clipping rectangle is performed within this routine;
//! the display driver's rectangle fill routine is used to perform the actual
//! rectangle fill.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_fillRectangle(const Graphics_Context *context,
                            const Graphics_Rectangle *rect)
{
    Graphics_Rectangle temp;

    //
    // Check the arguments.
    //
    assert(context);
    assert(rect);

    //
    // Swap the X coordinates if xMin is greater than xMax.
    //
    if(rect->xMin > rect->xMax)
    {
        temp.xMin = rect->xMax;
        temp.xMax = rect->xMin;
    }
    else
    {
        temp.xMin = rect->xMin;
        temp.xMax = rect->xMax;
    }

    //
    // Swap the Y coordinates if yMin is greater than yMax.
    //
    if(rect->yMin > rect->yMax)
    {
        temp.yMin = rect->yMax;
        temp.yMax = rect->yMin;
    }
    else
    {
        temp.yMin = rect->yMin;
        temp.yMax = rect->yMax;
    }

    //
    // Now that the coordinates are ordered, return without drawing anything if
    // the entire rectangle is out of the clipping region.
    //
    if((temp.xMin > context->clipRegion.xMax) ||
       (temp.xMax < context->clipRegion.xMin) ||
       (temp.yMin > context->clipRegion.yMax) ||
       (temp.yMax < context->clipRegion.yMin))
    {
        return;
    }

    //
    // Clip the X coordinates to the edges of the clipping region if necessary.
    //
    if(temp.xMin < context->clipRegion.xMin)
    {
        temp.xMin = context->clipRegion.xMin;
    }
    if(temp.xMax > context->clipRegion.xMax)
    {
        temp.xMax = context->clipRegion.xMax;
    }

    //
    // Clip the Y coordinates to the edges of the clipping region if necessary.
    //
    if(temp.yMin < context->clipRegion.yMin)
    {
        temp.yMin = context->clipRegion.yMin;
    }
    if(temp.yMax > context->clipRegion.yMax)
    {
        temp.yMax = context->clipRegion.yMax;
    }

    //
    // Call the low level rectangle fill routine.
    //
    Graphics_fillRectangleOnDisplay(context->display, &temp,
                                    context->foreground);
}

//*****************************************************************************
//
//! Determines if two rectangles overlap.
//!
//! \param rect1 is a pointer to the first rectangle.
//! \param rect2 is a pointer to the second rectangle.
//!
//! This function determines whether two rectangles overlap.  It assumes that
//! rectangles \e rect1 and \e rect2 are valid with \e xMin < \e xMax and
//! \e yMin < \e yMax.
//!
//! \return Returns 1 if there is an overlap or 0 if not.
//
//*****************************************************************************
int32_t Graphics_isRectangleOverlap(Graphics_Rectangle *rect1,
                                    Graphics_Rectangle *rect2)
{
    if((rect1->xMax < rect2->xMin) ||
       (rect2->xMax < rect1->xMin) ||
       (rect1->yMax < rect2->yMin) ||
       (rect2->yMax < rect1->yMin))
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//
//! Determines the intersection of two rectangles.
//!
//! \param rect1 is a pointer to the first rectangle.
//! \param rect2 is a pointer to the second rectangle.
//! \param intersect is a pointer to a rectangle which will be written with
//! the intersection of \e rect1 and \e rect2.
//!
//! This function determines if two rectangles overlap and, if they do,
//! calculates the rectangle representing their intersection.  If the rectangles
//! do not overlap, 0 is returned and \e intersect is not written.
//!
//! \return Returns 1 if there is an overlap or 0 if not.
//
//*****************************************************************************
int32_t Graphics_getRectangleIntersection(Graphics_Rectangle *rect1,
                                          Graphics_Rectangle *rect2,
                                          Graphics_Rectangle *intersect)
{
    //
    // Make sure we were passed valid rectangles.
    //
    if((rect1->xMax <= rect1->xMin) ||
       (rect1->yMax <= rect1->yMin) ||
       (rect2->xMax <= rect2->xMin) ||
       (rect2->yMax <= rect2->yMin))
    {
        return(0);
    }

    //
    // Make sure that there is an intersection between the two rectangles.
    //
    if(!Graphics_isRectangleOverlap(rect1, rect2))
    {
        return(0);
    }

    //
    // The rectangles do intersect so determine the rectangle of the
    // intersection.
    //
    intersect->xMin = max(rect1->xMin, rect2->xMin);
    intersect->xMax = min(rect1->xMax, rect2->xMax);
    intersect->yMin = max(rect1->yMin, rect2->yMin);
    intersect->yMax = min(rect1->yMax, rect2->yMax);

    return(1);
}

//*****************************************************************************
//
//! Determines if a point lies within a given rectangle.
//!
//! \param rect is a pointer to the rectangle which the point is to be checked
//! against.
//! \param x is the X coordinate of the point to be checked.
//! \param y is the Y coordinate of the point to be checked.
//!
//! This function determines whether point (x, y) lies within the rectangle
//! described by \e rect.
//!
//! \return Returns 1 if the point is within the rectangle or 0 otherwise.
//
//*****************************************************************************
bool Graphics_isPointWithinRectangle(const Graphics_Rectangle *rect,
                                     uint16_t x,
                                     uint16_t y)
{
    return ((((x) >= (rect)->xMin) && ((x) <= (rect)->xMax) &&
             ((y) >= (rect)->yMin) && ((y) <= (rect)->yMax)) ? true : false);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
