/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sys/socket.h
 */

#ifndef OCS_SYS_SOCKET_H
#define OCS_SYS_SOCKET_H

#include "OCS_basetypes.h"
#include "OCS_sys_types.h"

/* ----------------------------------------- */
/* constants normally defined in sys/socket.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/socket.h */
/* ----------------------------------------- */
typedef size_t             OCS_socklen_t;
typedef unsigned short int OCS_sa_family_t;

struct OCS_sockaddr
{
    OCS_sa_family_t sa_family;
};

struct OCS_sockaddr_in
{
    OCS_sa_family_t sa_family;
    uint16_t        sin_port;
    uint32_t        sin_addr;
};

struct OCS_sockaddr_in6
{
    OCS_sa_family_t sa_family;
    uint16_t        sin6_port;
    uint32_t        sin6_addr[4];
};

enum
{
    OCS_EINPROGRESS = -2,
    OCS_EWOULDBLOCK,
    OCS_AF_INET,
    OCS_AF_INET6,
    OCS_SOCK_DGRAM,
    OCS_SOCK_STREAM,
    OCS_IPPROTO_UDP,
    OCS_IPPROTO_TCP,
    OCS_SOL_SOCKET,
    OCS_SO_REUSEADDR,
    OCS_SO_ERROR,
    OCS_MSG_DONTWAIT
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/socket.h */
/* ----------------------------------------- */

extern int         OCS_accept(int fd, struct OCS_sockaddr *addr, OCS_socklen_t *addr_len);
extern int         OCS_bind(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len);
extern int         OCS_connect(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len);
extern int         OCS_getsockopt(int fd, int level, int optname, void *optval, OCS_socklen_t *optlen);
extern int         OCS_listen(int fd, int n);
extern OCS_ssize_t OCS_recvfrom(int fd, void *buf, size_t n, int flags, struct OCS_sockaddr *addr,
                                OCS_socklen_t *addr_len);
extern OCS_ssize_t OCS_sendto(int fd, const void *buf, size_t n, int flags, const struct OCS_sockaddr *addr,
                              OCS_socklen_t addr_len);
extern int         OCS_setsockopt(int fd, int level, int optname, const void *optval, OCS_socklen_t optlen);
extern int         OCS_socket(int domain, int type, int protocol);

#endif /* OCS_SYS_SOCKET_H */
