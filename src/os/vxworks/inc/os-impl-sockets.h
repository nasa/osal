/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_SOCKETS_H
#define OS_IMPL_SOCKETS_H

#include "os-impl-io.h"
#include "os-shared-globaldefs.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <hostLib.h>
#include <ioLib.h>

/*
 * Override the socket flag set routine on this platform.
 * This is required because some versions of VxWorks do not support
 * the standard POSIX fcntl() opcodes, and must use ioctl() instead.
 */
#define OS_IMPL_SET_SOCKET_FLAGS(impl) OS_VxWorks_SetSocketFlags_Impl(impl)

/* The "in.h" header file supplied in VxWorks 6.9 is missing the "in_port_t" typedef */
typedef u_short in_port_t;

/* VxWorks-specific helper function to configure the socket flags on a connection */
void OS_VxWorks_SetSocketFlags_Impl(const OS_object_token_t *token);

#endif /* OS_IMPL_SOCKETS_H */
