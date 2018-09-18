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

//*****************************************************************************
// includes
//*****************************************************************************
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/Power.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd.h>
#include <ti/net/atcmd/atcmd_gen.h>

#include "pthread.h"
#include "mqueue.h"
#include "unistd.h"


//*****************************************************************************
// defines
//*****************************************************************************

//*****************************************************************************
// typedefs
//*****************************************************************************

//*****************************************************************************
// Globals
//*****************************************************************************
extern mqd_t     ATCmd_rxQueue;

/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
void ATCmd_valToIPv4(uint32_t val, uint8_t *ip)
{
    ip[0] = (val & 0xFF000000) >> 24;
    ip[1] = (val & 0x00FF0000) >> 16;
    ip[2] = (val & 0x0000FF00) >> 8;
    ip[3] = (val & 0x000000FF);
}

/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_checkHelp(char *buff)
{
    if (*buff == '?')
    {
        return 1;
    }
    return 0;
}

/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_checkNumParams(char *token, uint8_t num)
{    
    uint32_t i = StrMpl_getNumVals(token,ATCMD_DELIM_ARG,ATCMD_DELIM_TRM,ATCmd_excludeDelimStr);

    if ((num != 0xFF) && (i != num))
    {
       return (-1);
    }
    return 0;
}


/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_okResult(void)
{    
    ATCmd_Event_t eventMsg;
    
    eventMsg.callback = NULL;
    eventMsg.args = (void *)(ATCmd_okStr);
    ATCmd_signalEvent(&eventMsg);
    return 0;
}


/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_errorResult(const char *buff, int32_t error)
{    
    ATCmd_Event_t eventMsg;
    char *token = NULL;

    token = strchr(buff, ATCMD_DELIM_ARG);    
    if (token != NULL)
    {
       sprintf(token + 1,"%d",(int)error); 
    }
    eventMsg.callback = NULL;
    eventMsg.args = (void *)(buff);
    ATCmd_signalEvent(&eventMsg);
    return 0;
}

/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_commandResult(int32_t (*callback)(void *, int32_t, char *), void *buff, int32_t num)
{    
    ATCmd_Event_t eventMsg;
    
    eventMsg.callback = callback;
    eventMsg.args = buff;
    eventMsg.num = num;
    ATCmd_signalEvent(&eventMsg);
    
    return 0;
}

/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_commandUsage(void *args, int32_t num, char *buff)
{  
    StrMpl_setStr((char *)args,&buff,ATCMD_DELIM_TRM);
    
    return 0;
}


/*!
    \brief          Prints AT command help menu.

    \param          arg       -   Points to arguments.
    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmd_signalEvent(ATCmd_Event_t *pmsg)
{
    struct mq_attr attr;
    uint8_t doSend = 0;
    
    do
    {
        mq_getattr(ATCmd_rxQueue, &attr);
        if (attr.mq_curmsgs < 10)
        {
            mq_send(ATCmd_rxQueue, (char *)pmsg, sizeof(ATCmd_Event_t), 0);
            doSend = 1;
        }
        else
        {
            usleep(1000);
        }
    }while (!doSend);

    return 0;
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
