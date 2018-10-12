/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
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

#ifndef ti_net_bsd_netdb__include
#define ti_net_bsd_netdb__include

#include <ti/net/slneterr.h>
#include <ti/net/slnetutils.h>

#ifdef    __cplusplus
extern "C" {
#endif

typedef struct hostent
{
    char  *h_name;              /* Host name */
    char **h_aliases;           /* alias list */
    int    h_addrtype;          /* host address type */
    int    h_length;            /* length of address */
    char **h_addr_list;         /* list of addresses */
#define h_addr  h_addr_list[0]  /* Address, for backward compatibility.  */
}hostent_t;

/* Flag values for getaddrinfo() */
#define AI_PASSIVE     SLNETUTIL_AI_PASSIVE
#define AI_NUMERICHOST SLNETUTIL_AI_NUMERICHOST

/* Error codes for getaddrinfo and gai_strerror */
#define EAI_AGAIN      SLNETUTIL_EAI_AGAIN
#define EAI_BADFLAGS   SLNETUTIL_EAI_BADFLAGS
#define EAI_FAIL       SLNETUTIL_EAI_FAIL
#define EAI_FAMILY     SLNETUTIL_EAI_FAMILY
#define EAI_MEMORY     SLNETUTIL_EAI_MEMORY
#define EAI_NONAME     SLNETUTIL_EAI_NONAME
#define EAI_SERVICE    SLNETUTIL_EAI_SERVICE
#define EAI_SOCKTYPE   SLNETUTIL_EAI_SOCKTYPE
#define EAI_SYSTEM     SLNETUTIL_EAI_SYSTEM
#define EAI_OVERFLOW   SLNETUTIL_EAI_OVERFLOW
#define EAI_ADDRFAMILY SLNETUTIL_EAI_ADDRFAMILY

/* Data structure mappings for getaddrinfo */
#define addrinfo SlNetUtil_addrInfo_t

int getaddrinfo(const char *node, const char *service,
        const struct addrinfo *hints, struct addrinfo **res);

static inline void freeaddrinfo(struct addrinfo *res)
{
    SlNetUtil_freeAddrInfo(res);
}

static inline const char *gai_strerror(int32_t errorcode)
{
    return (SlNetUtil_gaiStrErr(errorcode));
}

struct hostent* gethostbyname(const char *name);

#ifdef  __cplusplus
}
#endif

#endif /* ti_net_bsd_netdb__include */
