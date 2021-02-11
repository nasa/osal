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
 * \brief    Stubs for sys/sockets.h
 * \ingroup  ut-stubs
 */
#include "utstubs.h"
#include <OCS_sys_socket.h>

int OCS_accept(int fd, struct OCS_sockaddr *addr, OCS_socklen_t *addr_len)
{
    return UT_DEFAULT_IMPL(OCS_accept);
}

int OCS_bind(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len)
{
    return UT_DEFAULT_IMPL(OCS_bind);
}

int OCS_connect(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len)
{
    return UT_DEFAULT_IMPL(OCS_connect);
}

int OCS_getsockopt(int fd, int level, int optname, void *optval, OCS_socklen_t *optlen)
{
    return UT_DEFAULT_IMPL(OCS_getsockopt);
}

int OCS_listen(int fd, int n)
{
    return UT_DEFAULT_IMPL(OCS_listen);
}

OCS_ssize_t OCS_recvfrom(int fd, void *buf, size_t n, int flags, struct OCS_sockaddr *addr, OCS_socklen_t *addr_len)
{
    return UT_DEFAULT_IMPL(OCS_recvfrom);
}

OCS_ssize_t OCS_sendto(int fd, const void *buf, size_t n, int flags, const struct OCS_sockaddr *addr,
                       OCS_socklen_t addr_len)
{
    return UT_DEFAULT_IMPL(OCS_sendto);
}

int OCS_setsockopt(int fd, int level, int optname, const void *optval, OCS_socklen_t optlen)
{
    return UT_DEFAULT_IMPL(OCS_setsockopt);
}

int OCS_socket(int domain, int type, int protocol)
{
    return UT_DEFAULT_IMPL(OCS_socket);
}
