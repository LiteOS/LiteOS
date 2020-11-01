/**
 * @defgroup termios Termios
 * @ingroup libc
 */

#ifndef	_TERMIOS_H
#define	_TERMIOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_pid_t

#include <bits/alltypes.h>

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS 32

#include <bits/termios.h>

/**
 *@ingroup  termios
 *
 *@par Description:
 *The cfgetospeed() function shall extract the output baud rate from the
 *termios structure to which the s argument points. This function
 *shall return exactly the value in the termios data structure, without interpretation.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #speed_t Upon successful completion, cfgetospeed() shall return a value
 *of type speed_t representing the output baud rate.
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see cfgetispeed | cfsetispeed | cfsetospeed | tcgetattr
 *@since Huawei LiteOS V100R001C00
 */
speed_t cfgetospeed (const struct termios *);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The cfgetispeed() function shall extract the input baud rate from the termios
 *structure to which the s argument points. This function shall return
 *exactly the value in the termios data structure, without interpretation.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #speed_t Upon successful completion, cfgetispeed() shall return a value of
 *type speed_t representing the input baud rate.
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see cfgetospeed | cfsetispeed | cfsetospeed | tcgetattr
 *@since Huawei LiteOS V100R001C00
 */
speed_t cfgetispeed (const struct termios *);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The cfsetospeed() function shall set the output baud rate stored in
 *the structure pointed to by s to speed.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #0 cfsetospeed() shall return 0.
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see cfgetispeed | cfgetospeed | cfsetispeed | tcsetattr
 *@since Huawei LiteOS V100R001C00
 */
int cfsetospeed (struct termios *, speed_t);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The cfsetispeed() function shall set the input baud rate stored in
 *the structure pointed to by s to speed.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #0 cfsetispeed() shall return 0.
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see cfgetispeed | cfgetospeed | cfsetospeed | tcsetattr
 *@since Huawei LiteOS V100R001C00
 */
int cfsetispeed (struct termios *, speed_t);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The tcgetattr() function shall get the parameters associated with the terminal referred
 *to by fd and store them in the termios structure referenced by s.
 *The fd argument is an open file descriptor associated with a terminal.
 *
 *@attention
 * <ul>
 * <li>The API is not supported.</li>
 * </ul>
 *
 *@retval #0 Upon successful completion, 0 shall be returned.
 *@retval #-1 If it fails, -1 shall be returned and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see tcsetattr
 *@since Huawei LiteOS V100R001C00
 */
int tcgetattr (int, struct termios *);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The tcsetattr() function shall set the parameters associated with the terminal
 *referred to by the open file descriptor fd (an open file descriptor associated with a terminal)
 *from the termios structure referenced by s.
 *
 *@attention
 * <ul>
 * <li>The API is not supported.</li>
 * </ul>
 *
 *@retval #0 Upon successful completion, 0 shall be returned.
 *@retval #-1 If it fails, -1 shall be returned and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see cfgetispeed | tcgetattr
 *@since Huawei LiteOS V100R001C00
 */
int tcsetattr (int, int, const struct termios *);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The tcsendbreak() function sends a break for a specific duration.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #0 Upon successful completion, 0 shall be returned.
 *@retval #-1 If it fails, -1 shall be returned and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
int tcsendbreak (int, int);
int tcdrain (int);

/**
 *@ingroup  termios
 *
 *@par Description:
 *Upon successful completion, tcflush() shall discard data written to the object
 *referred to by fd (an open file descriptor associated with a terminal) but not transmitted,
 *or data received but not read, depending on the value of __queue.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #0 Upon successful completion, 0 shall be returned.
 *@retval #-1 If it fails, -1 shall be returned and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
int tcflush (int, int);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The tcflow() function shall suspend or restart transmission or reception of
 *data on the object referred to by fd, depending on the value of action.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #0 Upon successful completion, 0 shall be returned.
 *@retval #-1 If it fails, -1 shall be returned and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see tcsendbreak
 *@since Huawei LiteOS V100R001C00
 */
int tcflow (int, int);

/**
 *@ingroup  termios
 *
 *@par Description:
 *The tcgetsid() function shall obtain the process group ID of the session for which the terminal
 *specified by fildes is the controlling terminal.
 *
 *@attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@retval #pid_t Upon successful completion, tcgetsid() shall return the process group ID of
 *the session associated with the terminal. Otherwise, a value of -1 shall be returned
 *and errno set to indicate the error.
 *
 *@par Errors
 *<ul>
 *<li><b>EBADF</b>:The fd argument is not a valid open file descriptor.</li>
 *<li><b>EINVAL</b>:The request or arg argument is not valid for this device.</li>
 *<li><b>ENOMEM</b>:If any resource allocation fails, ENOMEM is set as errno.</li>
 *<li><b>EAFNOSUPPORT</b>:If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
pid_t tcgetsid (int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

/**
 *@ingroup  termios
 *
 *@par Description:
 *The cfmakeraw() sets the terminal to something like the "raw" mode of terminal driver:
 *input is available character by character, echoing is disabled, and all special processing
 *of terminal input and output characters is disabled.
 *
 *@attention
 *<ul>
 *<li>
 *The cfmakeraw() function is not a standard C Library Function.
 *</li>
 *</ul>
 *
 *@retval #void None.
 *
 *@par Dependency:
 *<ul><li>termios.h</li></ul>
 *
 *@see cfgetispeed | cfsetispeed | cfsetospeed | tcgetattr
 *
 *@since Huawei LiteOS V100R001C00
 */
void cfmakeraw(struct termios *);
int cfsetspeed(struct termios *, speed_t);
#endif

#ifdef __cplusplus
}
#endif

#endif
