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
 * \brief    Stubs for sys/sockets.h
 * \ingroup  ut-stubs
 */
#include "utstubs.h"
#include "OCS_sys_socket.h"

int OCS_accept(int fd, struct OCS_sockaddr *addr, OCS_socklen_t *addr_len)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_accept), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_accept), addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_accept), addr_len);

    return UT_DEFAULT_IMPL(OCS_accept);
}

int OCS_bind(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_bind), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_bind), addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_bind), len);

    return UT_DEFAULT_IMPL(OCS_bind);
}

int OCS_connect(int fd, const struct OCS_sockaddr *addr, OCS_socklen_t len)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_connect), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_connect), addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_connect), len);

    return UT_DEFAULT_IMPL(OCS_connect);
}

int OCS_getsockopt(int fd, int level, int optname, void *optval, OCS_socklen_t *optlen)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_getsockopt), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_getsockopt), level);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_getsockopt), optname);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_getsockopt), optval);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_getsockopt), optlen);

    return UT_DEFAULT_IMPL(OCS_getsockopt);
}

int OCS_listen(int fd, int n)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_listen), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_listen), n);

    return UT_DEFAULT_IMPL(OCS_listen);
}

OCS_ssize_t OCS_recvfrom(int fd, void *buf, size_t n, int flags, struct OCS_sockaddr *addr, OCS_socklen_t *addr_len)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), buf);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), n);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), flags);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_recvfrom), addr_len);

    return UT_DEFAULT_IMPL(OCS_recvfrom);
}

OCS_ssize_t OCS_sendto(int fd, const void *buf, size_t n, int flags, const struct OCS_sockaddr *addr,
                       OCS_socklen_t addr_len)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), buf);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), n);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), flags);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), addr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_sendto), addr_len);

    return UT_DEFAULT_IMPL(OCS_sendto);
}

int OCS_setsockopt(int fd, int level, int optname, const void *optval, OCS_socklen_t optlen)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_setsockopt), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_setsockopt), level);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_setsockopt), optname);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_setsockopt), optval);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_setsockopt), optlen);

    return UT_DEFAULT_IMPL(OCS_setsockopt);
}

int OCS_shutdown(int fd, int how)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_shutdown), fd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_shutdown), how);

    return UT_DEFAULT_IMPL(OCS_shutdown);
}

int OCS_socket(int domain, int type, int protocol)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_socket), domain);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_socket), type);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_socket), protocol);

    return UT_DEFAULT_IMPL(OCS_socket);
}
