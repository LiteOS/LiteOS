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


#include <stdio.h>
//****************************************************************************
//                       FUNCTION PROTOTYPES
//****************************************************************************

/*
    \brief          ATCmd Recv

    The Function receives AT Command response (events).

    \param          buffer      -   Points to buffer allocated by the Application.

    \param          nonblock    -   Set 0 for waits forever on queue, otherwise set 1 

    \return         Upon successful, the function shall return 0.
                    In case of failure, return -1
*/
int32_t ATCmd_recv(char *buffer, uint8_t nonblock);

/*
    \brief          ATCmd Send

    The Function sends AT Command.

    \param          buffer      -   Points to buffer allocated by the Application.

    \return         Upon successful, the function shall return 0.
                    In case of failure, return -1
*/
int32_t ATCmd_send(char *buffer);

/*
    \brief          ATCmd Create

    The Function creates AT Command module.

    \param          None

    \return         Upon successful, the function shall return 0.
                    In case of failure, return -1
*/
int32_t ATCmd_create(void);

