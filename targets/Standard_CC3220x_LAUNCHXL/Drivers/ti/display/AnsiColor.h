/*
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
 */

#ifndef ANSI_COLOR_H_
#define ANSI_COLOR_H_

#define _ANSI_TERM_GET_OVERRIDE(_1, _2, _3, _4, _5, NAME, ...)    NAME

#define _ANSI_TERM_CONCAT_COLOR5(a, b, c, d, e)                           \
    "\x1b[" _ANSI_TERM_ ##  a ";" _ANSI_TERM_ ##  b ";" _ANSI_TERM_ ##  c \
    ";" _ANSI_TERM_ ##  d ";" _ANSI_TERM_ ##  e  "m"
#define _ANSI_TERM_CONCAT_COLOR4(a, b, c, d)                              \
    "\x1b[" _ANSI_TERM_ ##  a ";" _ANSI_TERM_ ##  b ";" _ANSI_TERM_ ##  c \
    ";" _ANSI_TERM_ ##  d "m"
#define _ANSI_TERM_CONCAT_COLOR3(a, b, c) \
    "\x1b[" _ANSI_TERM_ ##  a ";" _ANSI_TERM_ ##  b ";" _ANSI_TERM_ ##  c "m"
#define _ANSI_TERM_CONCAT_COLOR2(a, b) \
    "\x1b[" _ANSI_TERM_ ##  a ";" _ANSI_TERM_ ##  b                       "m"
#define _ANSI_TERM_CONCAT_COLOR1(a) \
    "\x1b[" _ANSI_TERM_ ##  a                                             "m"

#define _ANSI_TERM_FG_BLACK          "30"
#define _ANSI_TERM_FG_RED            "31"
#define _ANSI_TERM_FG_GREEN          "32"
#define _ANSI_TERM_FG_YELLOW         "33"
#define _ANSI_TERM_FG_BLUE           "34"
#define _ANSI_TERM_FG_MAGENTA        "35"
#define _ANSI_TERM_FG_CYAN           "36"
#define _ANSI_TERM_FG_WHITE          "37"

#define _ANSI_TERM_BG_BLACK          "40"
#define _ANSI_TERM_BG_RED            "41"
#define _ANSI_TERM_BG_GREEN          "42"
#define _ANSI_TERM_BG_YELLOW         "43"
#define _ANSI_TERM_BG_BLUE           "44"
#define _ANSI_TERM_BG_MAGENTA        "45"
#define _ANSI_TERM_BG_CYAN           "46"
#define _ANSI_TERM_BG_WHITE          "47"

#define _ANSI_TERM_ATTR_RESET        "0"
#define _ANSI_TERM_ATTR_BOLD         "1"
#define _ANSI_TERM_ATTR_UNDERLINE    "4"
#define _ANSI_TERM_ATTR_BLINK        "5"

/* @brief Returns an ANSI escape sequence for the chosen colors and style
 *
 * The parameters are variadic, and up to five can be specified in any order:
 *
 * FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE,
 * BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE,
 * ATTR_RESET, ATTR_BOLD, ATTR_UNDERLINE, ATTR_BLINK
 *
 * Example use:
 *   char str[] = ANSI_COLOR(FG_WHITE, ATTR_BOLD) "Hello" ANSI_COLOR(ATTR_RESET)
 *   --> "Esc[37;1mHelloEsc[0m" where Esc is the Escape control character 0x1B.
 *
 * @param 1 - color or style
 * @param 2 - color or style
 * @param 3 - color or style
 * @param 4 - color or style
 * @param 5 - color or style
 *
 */
#define ANSI_COLOR(...)                                                                                   \
    _ANSI_TERM_GET_OVERRIDE(__VA_ARGS__,                                                                  \
                            _ANSI_TERM_CONCAT_COLOR5, _ANSI_TERM_CONCAT_COLOR4, _ANSI_TERM_CONCAT_COLOR3, \
                            _ANSI_TERM_CONCAT_COLOR2, _ANSI_TERM_CONCAT_COLOR1) (__VA_ARGS__)


#endif /* ANSI_COLOR_H_ */
