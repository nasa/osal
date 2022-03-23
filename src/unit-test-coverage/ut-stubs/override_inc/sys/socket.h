/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sys/socket.h
 */

#ifndef OVERRIDE_SYS_SOCKET_H
#define OVERRIDE_SYS_SOCKET_H

#include "OCS_sys_socket.h"

/* ----------------------------------------- */
/* mappings for declarations in sys/socket.h */
/* ----------------------------------------- */
#define socklen_t    OCS_socklen_t
#define sockaddr     OCS_sockaddr
#define sockaddr_in  OCS_sockaddr_in
#define sockaddr_in6 OCS_sockaddr_in6
#define sa_family_t  OCS_sa_family_t
#define accept       OCS_accept
#define bind         OCS_bind
#define connect      OCS_connect
#define getsockopt   OCS_getsockopt
#define listen       OCS_listen
#define recvfrom     OCS_recvfrom
#define sendto       OCS_sendto
#define setsockopt   OCS_setsockopt
#define shutdown     OCS_shutdown
#define socket       OCS_socket

#define EINPROGRESS  OCS_EINPROGRESS
#define EWOULDBLOCK  OCS_EWOULDBLOCK
#define AF_INET      OCS_AF_INET
#define AF_INET6     OCS_AF_INET6
#define SOCK_DGRAM   OCS_SOCK_DGRAM
#define SOCK_STREAM  OCS_SOCK_STREAM
#define IPPROTO_UDP  OCS_IPPROTO_UDP
#define IPPROTO_TCP  OCS_IPPROTO_TCP
#define SOL_SOCKET   OCS_SOL_SOCKET
#define SO_REUSEADDR OCS_SO_REUSEADDR
#define SO_ERROR     OCS_SO_ERROR
#define MSG_DONTWAIT OCS_MSG_DONTWAIT
#define SHUT_WR      OCS_SHUT_WR
#define SHUT_RD      OCS_SHUT_RD
#define SHUT_RDWR    OCS_SHUT_RDWR

#endif /* OVERRIDE_SYS_SOCKET_H */
