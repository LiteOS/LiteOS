
/**
 * @defgroup if If
 * @ingroup libc
 */

#ifndef _NET_IF_H
#define _NET_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define IF_NAMESIZE 16

struct if_nameindex {
	unsigned int if_index;
	char *if_name;
};

/**
 * @ingroup  if
 *
 * @par Description:
 * This function is used to convert the index of the network interface corresponding to the name ifname.
 *
 * @param    ifname     [IN]  The name of the network interface.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #>0       On success, the index number of the network interface was successfully converted
 * @retval    #0        On failure, 0 is returned and errno is set appropriately
 *
 * @par Errors
 * <ul>
 * <li><b>ENXIO</b>: Could not find the index corresponding to the interface name.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>if.h</li></ul>
 *
 * @see  if_indextoname
 *
 * @since Huawei LiteOS V100R001C00
 */
unsigned int if_nametoindex (const char *);

/**
 * @ingroup  if
 *
 * @par Description:
 * This  function is used to convert the name of the network interface corresponding
 * to the interface index ifindex. The name is placed in the buffer pointed to by ifname.
 * The buffer must allow for the storage of at least IF_NAMESIZE bytes.
 *
 * @param    #ifindex    [IN]   The index of the network interface.
 * @param    #ifname     [OUT]  The name of the network interface.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    ifname    On success, the ifname was successfully converted
 * @retval    NULL      On failure, NULL is returned and errno is set appropriately
 *
 * @par Errors
 * <ul>
 * <li><b>ENXIO</b>: There is no name as per the index specified.</li>
 * <li><b>EINVAL</b>: The ifname is a null pointer.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>if.h</li></ul>
 *
 * @see  if_nametoindex
 *
 * @since Huawei LiteOS V100R001C00
 */
char *if_indextoname (unsigned int, char *);

/**
 * @ingroup  if
 *
 * @par Description:
 * This function returns an array of if_nameindex structures, one structure per interface.
 * The if_nameindex structure holds the information about a single interface and is defined as a result of including the <if.h> header.
 * The end of the array of structures is indicated by a structure with an if_index of 0 and an if_name of NULL.
 * The function returns a NULL pointer upon an error, and would set errno to the appropriate value.
 * The memory used for this array of structures along with the interface names pointed to by the if_name members is obtained dynamically.
 *
 * @param    void     [IN]  The function does not take any input instead returns all the interfaces in format if_nameindex structures.
 *
 * @attention
 * <ul>
 * <li>This API is only supported since lwip 2.0.</li>
 * <li>The end of the array of structures is indicated by a structure with   an if_index of 0 and an if_name of NULL.
 * The function returns a NULL   pointer upon an error, and would set errno to the appropriate value.</li>
 * </ul>
 *
 * @retval    #if_nameindex*   On success, the function returns an array of if_nameindex structures, one structure per interface. On failure, NULL shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOBUFS</b>: Insufficient resources available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>if.h</li></ul>
 *
 * @see  if_freenameindex
 *
 * @since Huawei LiteOS V100R001C00
 */
struct if_nameindex *if_nameindex (void);

/**
 * @ingroup  if
 *
 * @par Description:
 * This function frees the dynamic memory that was allocated by if_nameindex().
 * The argument to this function must be a pointer that was returned by if_nameindex().
 *
 * @param    ptr     [IN]  The argument to this function must be a pointer that was returned by if_nameindex().
 *
 * @attention
 * <ul>
 * <li>This API is only supported since lwip 2.0.</li>
 * </ul>
 *
 * @retval  #void    None.
 *
 * @par Dependency:
 * <ul><li>if.h</li></ul>
 *
 * @see  if_nameindex
 *
 * @since Huawei LiteOS V100R001C00
 */
void if_freenameindex (struct if_nameindex *);




#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#include <sys/socket.h>

#define IFF_UP	0x1
#define IFF_BROADCAST 0x2
#define IFF_DEBUG 0x4
#define IFF_LOOPBACK 0x8
#define IFF_POINTOPOINT 0x10
#define IFF_NOTRAILERS 0x20
#define IFF_RUNNING 0x40
#define IFF_NOARP 0x80
#define IFF_PROMISC 0x100
#define IFF_ALLMULTI 0x200
#define IFF_MASTER 0x400
#define IFF_SLAVE 0x800
#define IFF_MULTICAST 0x1000
#define IFF_PORTSEL 0x2000
#define IFF_AUTOMEDIA 0x4000
#define IFF_DYNAMIC 0x8000
#define IFF_LOWER_UP 0x10000
#define IFF_DORMANT 0x20000
#define IFF_ECHO 0x40000
#define IFF_VOLATILE (IFF_LOOPBACK|IFF_POINTOPOINT|IFF_BROADCAST| \
        IFF_ECHO|IFF_MASTER|IFF_SLAVE|IFF_RUNNING|IFF_LOWER_UP|IFF_DORMANT)

struct ifaddr {
	struct sockaddr ifa_addr;
	union {
		struct sockaddr	ifu_broadaddr;
		struct sockaddr	ifu_dstaddr;
	} ifa_ifu;
	struct iface *ifa_ifp;
	struct ifaddr *ifa_next;
};

#define ifa_broadaddr	ifa_ifu.ifu_broadaddr
#define ifa_dstaddr	ifa_ifu.ifu_dstaddr

struct ifmap {
	unsigned long int mem_start;
	unsigned long int mem_end;
	unsigned short int base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

#define IFHWADDRLEN	6
#define IFNAMSIZ	IF_NAMESIZE

struct ifreq {
	union {
		char ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	union {
		struct sockaddr ifru_addr;
		struct sockaddr ifru_dstaddr;
		struct sockaddr ifru_broadaddr;
		struct sockaddr ifru_netmask;
		struct sockaddr ifru_hwaddr;
		short int ifru_flags;
		int ifru_ivalue;
		int ifru_mtu;
		struct ifmap ifru_map;
		char ifru_slave[IFNAMSIZ];
		char ifru_newname[IFNAMSIZ];
		char *ifru_data;
	} ifr_ifru;
};

#define ifr_name	ifr_ifrn.ifrn_name
#define ifr_hwaddr	ifr_ifru.ifru_hwaddr
#define ifr_addr	ifr_ifru.ifru_addr
#define ifr_dstaddr	ifr_ifru.ifru_dstaddr
#define ifr_broadaddr	ifr_ifru.ifru_broadaddr
#define ifr_netmask	ifr_ifru.ifru_netmask
#define ifr_flags	ifr_ifru.ifru_flags
#define ifr_metric	ifr_ifru.ifru_ivalue
#define ifr_mtu		ifr_ifru.ifru_mtu
#define ifr_map		ifr_ifru.ifru_map
#define ifr_slave	ifr_ifru.ifru_slave
#define ifr_data	ifr_ifru.ifru_data
#define ifr_ifindex	ifr_ifru.ifru_ivalue
#define ifr_bandwidth	ifr_ifru.ifru_ivalue
#define ifr_qlen	ifr_ifru.ifru_ivalue
#define ifr_newname	ifr_ifru.ifru_newname
#define _IOT_ifreq	_IOT(_IOTS(char),IFNAMSIZ,_IOTS(char),16,0,0)
#define _IOT_ifreq_short _IOT(_IOTS(char),IFNAMSIZ,_IOTS(short),1,0,0)
#define _IOT_ifreq_int	_IOT(_IOTS(char),IFNAMSIZ,_IOTS(int),1,0,0)

struct ifconf {
	int ifc_len;		
	union {
		char *ifcu_buf;
		struct ifreq *ifcu_req;
	} ifc_ifcu;
};

#define ifc_buf		ifc_ifcu.ifcu_buf
#define ifc_req		ifc_ifcu.ifcu_req
#define _IOT_ifconf _IOT(_IOTS(struct ifconf),1,0,0,0,0)

#define __UAPI_DEF_IF_IFCONF                                    0
#define __UAPI_DEF_IF_IFMAP                                     0
#define __UAPI_DEF_IF_IFNAMSIZ                                  0
#define __UAPI_DEF_IF_IFREQ                                     0
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS                          0
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO    0

#endif

#ifdef __cplusplus
}
#endif

#endif
