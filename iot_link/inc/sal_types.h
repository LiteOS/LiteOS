/*
 * sal_types.h
 *
 *  Created on: 2019Äê4ÔÂ26ÈÕ
 *      Author: zhangqf
 */

#ifndef __SAL_TYPES_H
#define __SAL_TYPES_H

#include <stdint.h>
#include <stddef.h>


/**
 * @brief: all the structure and defines needed by the  applications should include
 *         this file;if you have the one yourself, please check whether they have the same value
 *         And these defines keep compatible with linux and
 */

#ifndef socklen_t
#define socklen_t int
#endif

#ifndef ssize_t
#define ssize_t   int
#endif


#ifndef sa_family_t
#define sa_family_t  uint16_t
#endif

#ifndef in_port_t
#define in_port_t  uint16_t
#endif


#ifndef in_addr_t
#define in_addr_t uint32_t
#endif



struct sockaddr
{
    sa_family_t     sa_family;      /* address family, AF_xxx   */
    char            sa_data[14];    /* 14 bytes of protocol address */
};

struct linger
{
    int l_onoff;    /* Linger active            */
    int l_linger;   /* How long to linger for   */
};


;

struct in_addr
{
    in_addr_t s_addr;                    /* IPv4 address */
};


struct sockaddr_in
{
    sa_family_t sin_family;             /* AF_INET */
    in_port_t sin_port;                 /* Port number.  */
    struct in_addr sin_addr;            /* Internet address.  */
    unsigned char sin_zero[8];          /* Pad to size of `struct sockaddr'.  */
};

struct hostent {
    char  *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int    h_addrtype;  /* Address type. */
    int    h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};

#endif /* __SAL_TYPES_H */
