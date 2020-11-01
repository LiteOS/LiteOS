/**
 * @defgroup inet Inet
 * @ingroup libc
 */

#ifndef _ARPA_INET_H
#define	_ARPA_INET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <netinet/in.h>

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

/**
 * @ingroup  inet
 *
 * @par Description:
 * This function is used to convert the IPv4 address from string notation (number and dot format)
 * to binary notation of network byte order.
 *
 * @param    cp    [IN]   Pointer to IPv4 address string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #in_addr_t      On success, in unsigned int (32bit) format.
 * @retval    #IPADDR_NONE    On failure.
 *
 * @par Dependency:
 * <ul><li>inet.h</li></ul>
 *
 * @see inet_aton
 *
 * @since Huawei LiteOS V100R001C00
 */
in_addr_t inet_addr (const char *);
in_addr_t inet_network (const char *);

/**
 * @ingroup  inet
 *
 * @par Description:
 * This function is used to convert the IPv4 address from binary notation (network byte order)
 * to string notation (number and dot format).
 *
 * @param    in    [IN]    Pointer to binary notation of IPv4 address.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval   #char*          On success, returns pointer to string notation of IPv4 address.
 * @retval   #NULL           On failure.
 *
 * @par Dependency:
 * <ul><li>inet.h</li></ul>
 *
 * @see inet_ntop
 *
 * @since Huawei LiteOS V100R001C00
 */
char *inet_ntoa (struct in_addr);

/**
 * @ingroup  inet
 *
 * @par Description:
 * This  function is used to convert the character string src into a network address
 * structure in the af address family, then copies the network address structure to dst.
 * The af argument must be AF_INET.
 *
 * @param    af    [IN]    Address family.
 * @param    src   [IN]    Pointer to IPv4 address string.
 * @param    dst   [OUT]   Result string of IPv4 address will be updated here.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #1   On success, network address was successfully converted.
 * @retval    #0   On failure, if src does not contain a character string representing a
 *                 valid network address in the specified address family.
 * @retval    #-1  On failure, if af does not contain a valid address family.
 *
 * @par Errors
 * <ul>
 * <li><b>EAFNOSUPPORT</b>: Address family not supported.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>inet.h</li></ul>
 *
 * @see inet_ntop
 *
 * @since Huawei LiteOS V100R001C00
 */
int inet_pton (int, const char *__restrict, void *__restrict);

/**
 * @ingroup  inet
 *
 * @par Description:
 * This  function is used to convert the network address structure src in the af address family into a character string.
 * The resulting string is copied to the buffer pointed to by dst, which must be a non-null pointer.
 * The caller specifies the number of bytes available in this buffer in the argument size.
 *
 * @param    af     [IN]    Address family.
 * @param    src    [IN]    Pointer to IPv4 address string.
 * @param    size   [IN]    Size of IPv4 address string.
 * @param    dst    [OUT]   Result string of IPv4 address will be updated here.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval   #char*          On success, returns pointer to string notation of IPv4 address.
 * @retval   #NULL           On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EAFNOSUPPORT</b>: Address family not supported.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>inet.h</li></ul>
 *
 * @see inet_pton
 *
 * @since Huawei LiteOS V100R001C00
 */
const char *inet_ntop (int, const void *__restrict, char *__restrict, socklen_t);

/**
 * @ingroup  inet
 *
 * @par Description:
 * This function is used to convert the IPv4 address from string notation (number and dot format)
 * to binary notation of network byte order and stores it in the structure that addr points to.
 *
 * @param    cp    [IN]    Pointer to IPv4 address string.
 * @param    inp   [OUT]   Generated binary notation of IPv4 address will be updated here.
 *
 * @attention
 * <ul>
 * <li>This interface is same as inet_addr(). Only difference is that the generated binary notation
 * of IPv4 address is updated in the input parameter "inp", instead of returning it.</li>
 * </ul>
 *
 * @retval   #1   On success.
 * @retval   #0   On failure.
 *
 * @par Dependency:
 * <ul><li>inet.h</li></ul>
 *
 * @see inet_addr
 *
 * @since Huawei LiteOS V100R001C00
 */
int inet_aton (const char *, struct in_addr *);
struct in_addr inet_makeaddr(in_addr_t, in_addr_t);
in_addr_t inet_lnaof(struct in_addr);
in_addr_t inet_netof(struct in_addr);

#undef INET_ADDRSTRLEN
#undef INET6_ADDRSTRLEN
#define INET_ADDRSTRLEN  16
#define INET6_ADDRSTRLEN 46

#ifdef __cplusplus
}
#endif

#endif
