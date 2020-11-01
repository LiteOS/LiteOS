#ifndef _IFADDRS_H
#define _IFADDRS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <netinet/in.h>
#include <sys/socket.h>

struct ifaddrs {
	struct ifaddrs *ifa_next;
	char *ifa_name;
	unsigned ifa_flags;
	struct sockaddr *ifa_addr;
	struct sockaddr *ifa_netmask;
	union {
		struct sockaddr *ifu_broadaddr;
		struct sockaddr *ifu_dstaddr;
	} ifa_ifu;
	void *ifa_data;
};
#define ifa_broadaddr ifa_ifu.ifu_broadaddr
#define ifa_dstaddr ifa_ifu.ifu_dstaddr

/**
 * @defgroup Network_Interfaces_Info Network Interfaces Info
 * This section contains the interfaces to get information about Network interfaces in LwIP.
 * @ingroup User_interfaces
 */

/*
 * Func Name:  freeifaddrs
 */

/**
 * @ingroup Network_Interfaces_Info
 * @par Prototype
 * @code
 * void freeifaddrs(struct ifaddrs *ifa);
 * @endcode
 *
 * @par Purpose
 * This function is to free the memory of struct ifaddrs * provided by getifaddrs.
 *
 * @par Description
 * The function getifaddrs, provides the list of network interfaces in struct ifaddrs*.
 * The application has to free the memory of struct ifaddrs * by using this function.
 *
 * @param[in]    ifap                Pointer to struct ifaddrs [N/A]
 *
 * @par Return values
 * None
 *
 * @par Required Header File
 * ifaddrs.h.h
 *
 * @par Note
 * \n
 * N/A
 *
 * @par Related Topics
 * \n
 * N/A
 */
void freeifaddrs(struct ifaddrs *);

/*
 * Func Name:  getifaddrs
 */

/**
 * @ingroup Network_Interfaces_Info
 * @par Prototype
 * @code
 * int getifaddrs(struct ifaddrs **ifap);
 * @endcode
 *
 * @par Purpose
 * This function creates a linked list of structures describing the network
 * interfaces of the local system, and stores the address of the first item
 * of the list in *ifap.
 *
 * @par Description
 * This function creates a linked list of struct ifaddrs, which holds the information
 * about the network interfaces of the local system. The ifa_next field contains a
 * pointer to the next structure on the list, or NULL if this is the last item of the list.
 * The ifa_name points to the null-terminated interface name. The ifa_flags field
 * contains the interface flags. The ifa_addr field points to a structure containing
 * the interface address. The ifa_netmask field points to a structure containing
 * the netmask associated with ifa_addr, if applicable for the address family.
 * Depending on whether the bit IFF_brcast or IFF_POINTOPOINT is set
 * in ifa_flags (only one can be set at a time), either ifa_broadaddr will contain
 * the brcast address associated with ifa_addr (if applicable for the address
 * family) or ifa_dstaddr will contain the destination address of the point-to-point
 * interface.
 *
 * @param[in]    ifap                Double Pointer to struct ifaddrs [N/A]
 *
 * @par Return values
 *  ERR_OK : on success
 *  Non zero value : on failure
 *
 * @par Required Header File
 * ifaddrs.h.h
 *
 * @par Note
 * The data returned by getifaddrs() is dynamically allocated and should be freed
 * using freeifaddrs() when no longer needed.\n
 *
 * @par Related Topics
 * \n
 * N/A
 */
int getifaddrs(struct ifaddrs **);

#ifdef __cplusplus
}
#endif

#endif

