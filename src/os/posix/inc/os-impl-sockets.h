/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_SOCKETS_H
#define OS_IMPL_SOCKETS_H

#include "os-impl-io.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define OS_NETWORK_SUPPORTS_IPV6

/*
 * Socket descriptors should be usable with the select() API
 */
#define OS_IMPL_SOCKET_SELECTABLE true

/*
 * A full POSIX-compliant I/O layer should support using
 * nonblocking I/O calls in combination with select().
 */
#define OS_IMPL_SOCKET_FLAGS O_NONBLOCK

#endif /* OS_IMPL_SOCKETS_H */
