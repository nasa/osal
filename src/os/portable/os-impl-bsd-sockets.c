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
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the network functionality for
 *      systems which implement the BSD-style socket API.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *  socket()
 *  getsockopt()
 *  setsockopt()
 *  fcntl()
 *  bind()
 *  listen()
 *  accept()
 *  connect()
 *  recvfrom()
 *  sendto()
 *  inet_pton()
 *  ntohl()/ntohs()
 *
 * As well as any headers for the struct sockaddr type and any address families in use
 */
#include <string.h>
#include <errno.h>

#include "os-impl-sockets.h"
#include "os-shared-file.h"
#include "os-shared-select.h"
#include "os-shared-sockets.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/*
 * The OS layer may define a macro to set the proper flags on newly-opened sockets.
 * If not set, then a default implementation is used, which uses fcntl() to set O_NONBLOCK
 */
#ifndef OS_IMPL_SOCKET_FLAGS
#ifdef O_NONBLOCK
#define OS_IMPL_SOCKET_FLAGS O_NONBLOCK
#else
#define OS_IMPL_SOCKET_FLAGS 0 /* do not set any flags */
#endif
#endif

#ifndef OS_IMPL_SET_SOCKET_FLAGS
#define OS_IMPL_SET_SOCKET_FLAGS(tok) OS_SetSocketDefaultFlags_Impl(tok)
#endif

typedef union
{
    char               data[OS_SOCKADDR_MAX_LEN];
    struct sockaddr    sa;
    struct sockaddr_in sa_in;
#ifdef OS_NETWORK_SUPPORTS_IPV6
    struct sockaddr_in6 sa_in6;
#endif
} OS_SockAddr_Accessor_t;

/*
 * Confirm that the abstract socket address buffer size (OS_SOCKADDR_MAX_LEN) is
 * large enough to store any of the enabled address types.  If this is true, the
 * size of the above union will match OS_SOCKADDR_MAX_LEN.  However, if any
 * implementation-provided struct types are larger than this, the union will be
 * larger, and this indicates a configuration error.
 */
CompileTimeAssert(sizeof(OS_SockAddr_Accessor_t) == OS_SOCKADDR_MAX_LEN, SockAddrSize);

/*
 * Default flags implementation: Set the O_NONBLOCK flag via fcntl().
 * An implementation can also elect custom configuration by setting
 * the OS_IMPL_SET_SOCKET_FLAGS macro to point to an alternate function.
 */
void OS_SetSocketDefaultFlags_Impl(const OS_object_token_t *token)
{
    OS_impl_file_internal_record_t *impl;
    int                             os_flags;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    os_flags = fcntl(impl->fd, F_GETFL);
    if (os_flags == -1)
    {
        /* No recourse if F_GETFL fails - just report the error and move on. */
        OS_DEBUG("fcntl(F_GETFL): %s\n", strerror(errno));
    }
    else
    {
        os_flags |= OS_IMPL_SOCKET_FLAGS;
        if (fcntl(impl->fd, F_SETFL, os_flags) == -1)
        {
            /* No recourse if F_SETFL fails - just report the error and move on. */
            OS_DEBUG("fcntl(F_SETFL): %s\n", strerror(errno));
        }
    }

    impl->selectable = true;
}

/****************************************************************************************
                                    Sockets API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketOpen_Impl(const OS_object_token_t *token)
{
    int                             os_domain;
    int                             os_type;
    int                             os_proto;
    int                             os_flags;
    OS_impl_file_internal_record_t *impl;
    OS_stream_internal_record_t *   stream;

    impl   = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);
    stream = OS_OBJECT_TABLE_GET(OS_stream_table, *token);

    os_proto = 0;

    switch (stream->socket_type)
    {
        case OS_SocketType_DATAGRAM:
            os_type  = SOCK_DGRAM;
            os_proto = IPPROTO_UDP;
            break;

        case OS_SocketType_STREAM:
            os_type  = SOCK_STREAM;
            os_proto = IPPROTO_TCP;
            break;

        default:
            return OS_ERR_NOT_IMPLEMENTED;
    }

    switch (stream->socket_domain)
    {
        case OS_SocketDomain_INET:
            os_domain = AF_INET;
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case OS_SocketDomain_INET6:
            os_domain = AF_INET6;
            break;
#endif
        default:
            return OS_ERR_NOT_IMPLEMENTED;
    }

    impl->fd = socket(os_domain, os_type, os_proto);
    if (impl->fd < 0)
    {
        return OS_ERROR;
    }

    /*
     * Setting the REUSEADDR flag helps during debugging when there might be frequent
     * code restarts.  However if setting the option fails then it is not worth bailing out over.
     */
    os_flags = 1;
    setsockopt(impl->fd, SOL_SOCKET, SO_REUSEADDR, &os_flags, sizeof(os_flags));

    /*
     * Set the standard options on the filehandle by default --
     * this may set it to non-blocking mode if the implementation supports it.
     * any blocking would be done explicitly via the select() wrappers
     *
     * NOTE: The implementation still generally works without this flag set, but
     * nonblock mode does improve robustness in the event that multiple tasks
     * attempt to accept new connections from the same server socket at the same time.
     */
    OS_IMPL_SET_SOCKET_FLAGS(token);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketBind_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr)
{
    int                             os_result;
    socklen_t                       addrlen;
    const struct sockaddr *         sa;
    OS_impl_file_internal_record_t *impl;
    OS_stream_internal_record_t *   stream;

    impl   = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);
    stream = OS_OBJECT_TABLE_GET(OS_stream_table, *token);

    sa = (const struct sockaddr *)&Addr->AddrData;

    switch (sa->sa_family)
    {
        case AF_INET:
            addrlen = sizeof(struct sockaddr_in);
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            addrlen = sizeof(struct sockaddr_in6);
            break;
#endif
        default:
            addrlen = 0;
            break;
    }

    if (addrlen == 0)
    {
        return OS_ERR_BAD_ADDRESS;
    }

    os_result = bind(impl->fd, sa, addrlen);
    if (os_result < 0)
    {
        OS_DEBUG("bind: %s\n", strerror(errno));
        return OS_ERROR;
    }

    /* Start listening on the socket (implied for stream sockets) */
    if (stream->socket_type == OS_SocketType_STREAM)
    {
        os_result = listen(impl->fd, 10);
        if (os_result < 0)
        {
            OS_DEBUG("listen: %s\n", strerror(errno));
            return OS_ERROR;
        }
    }
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketConnect_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr, int32 timeout)
{
    int32                           return_code;
    int                             os_status;
    int                             sockopt;
    socklen_t                       slen;
    uint32                          operation;
    const struct sockaddr *         sa;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    sa = (const struct sockaddr *)&Addr->AddrData;
    switch (sa->sa_family)
    {
        case AF_INET:
            slen = sizeof(struct sockaddr_in);
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            slen = sizeof(struct sockaddr_in6);
            break;
#endif
        default:
            slen = 0;
            break;
    }

    if (slen != Addr->ActualLength)
    {
        return_code = OS_ERR_BAD_ADDRESS;
    }
    else
    {
        return_code = OS_SUCCESS;
        os_status   = connect(impl->fd, sa, slen);
        if (os_status < 0)
        {
            if (errno != EINPROGRESS)
            {
                OS_DEBUG("connect: %s\n", strerror(errno));
                return_code = OS_ERROR;
            }
            else
            {
                /*
                 * If the socket was created in nonblocking mode (O_NONBLOCK flag) then the connect
                 * runs in the background and connect() returns EINPROGRESS.  In this case we still
                 * want to provide the "normal" (blocking) semantics to the calling app, such that
                 * when OS_SocketConnect() returns, the socket is ready for use.
                 *
                 * To provide consistent behavior to calling apps, this does a select() to wait
                 * for the socket to become writable, meaning that the remote side is connected.
                 *
                 * An important point here is that the calling app can control the timeout.  If the
                 * normal/blocking connect() was used, the OS/IP stack controls the timeout, and it
                 * can be quite long.
                 */
                operation = OS_STREAM_STATE_WRITABLE;
                if (impl->selectable)
                {
                    return_code = OS_SelectSingle_Impl(token, &operation, timeout);
                }
                if (return_code == OS_SUCCESS)
                {
                    if ((operation & OS_STREAM_STATE_WRITABLE) == 0)
                    {
                        return_code = OS_ERROR_TIMEOUT;
                    }
                    else
                    {
                        /*
                         * The SO_ERROR socket flag should also read back zero.
                         * If not zero, something went wrong during connect
                         */
                        sockopt   = 0;
                        slen      = sizeof(sockopt);
                        os_status = getsockopt(impl->fd, SOL_SOCKET, SO_ERROR, &sockopt, &slen);
                        if (os_status < 0 || sockopt != 0)
                        {
                            return_code = OS_ERROR;
                        }
                    }
                }
            }
        }
    }
    return return_code;
}

/*----------------------------------------------------------------

    Purpose: Graceful shutdown of a stream socket

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketShutdown_Impl(const OS_object_token_t *token, OS_SocketShutdownMode_t Mode)
{
    OS_impl_file_internal_record_t *conn_impl;
    int32                           return_code;
    int                             how;

    conn_impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    /* Note that when called via the shared layer,
     * the "Mode" arg has already been checked/validated. */
    if (Mode == OS_SocketShutdownMode_SHUT_READ)
    {
        how = SHUT_RD;
    }
    else if (Mode == OS_SocketShutdownMode_SHUT_WRITE)
    {
        how = SHUT_WR;
    }
    else
    {
        how = SHUT_RDWR;
    }

    if (shutdown(conn_impl->fd, how) == 0)
    {
        return_code = OS_SUCCESS;
    }
    else
    {
        return_code = OS_ERROR;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAccept_Impl(const OS_object_token_t *sock_token, const OS_object_token_t *conn_token,
                           OS_SockAddr_t *Addr, int32 timeout)
{
    int32                           return_code;
    uint32                          operation;
    socklen_t                       addrlen;
    OS_impl_file_internal_record_t *sock_impl;
    OS_impl_file_internal_record_t *conn_impl;

    sock_impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *sock_token);
    conn_impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *conn_token);

    operation = OS_STREAM_STATE_READABLE;
    if (sock_impl->selectable)
    {
        return_code = OS_SelectSingle_Impl(sock_token, &operation, timeout);
    }
    else
    {
        return_code = OS_SUCCESS;
    }

    if (return_code == OS_SUCCESS)
    {
        if ((operation & OS_STREAM_STATE_READABLE) == 0)
        {
            return_code = OS_ERROR_TIMEOUT;
        }
        else
        {
            addrlen       = Addr->ActualLength;
            conn_impl->fd = accept(sock_impl->fd, (struct sockaddr *)&Addr->AddrData, &addrlen);
            if (conn_impl->fd < 0)
            {
                return_code = OS_ERROR;
            }
            else
            {
                Addr->ActualLength = addrlen;

                OS_IMPL_SET_SOCKET_FLAGS(conn_token);
            }
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketRecvFrom_Impl(const OS_object_token_t *token, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr,
                             int32 timeout)
{
    int32                           return_code;
    int                             os_result;
    int                             waitflags;
    uint32                          operation;
    struct sockaddr *               sa;
    socklen_t                       addrlen;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    if (RemoteAddr == NULL)
    {
        sa      = NULL;
        addrlen = 0;
    }
    else
    {
        addrlen = OS_SOCKADDR_MAX_LEN;
        sa      = (struct sockaddr *)&RemoteAddr->AddrData;
    }

    operation = OS_STREAM_STATE_READABLE;
    /*
     * If "O_NONBLOCK" flag is set then use select()
     * Note this is the only way to get a correct timeout
     */
    if (impl->selectable)
    {
        waitflags   = MSG_DONTWAIT;
        return_code = OS_SelectSingle_Impl(token, &operation, timeout);
    }
    else
    {
        if (timeout == 0)
        {
            waitflags = MSG_DONTWAIT;
        }
        else
        {
            /* note timeout will not be honored if >0 */
            waitflags = 0;
        }
        return_code = OS_SUCCESS;
    }

    if (return_code == OS_SUCCESS)
    {
        if ((operation & OS_STREAM_STATE_READABLE) == 0)
        {
            return_code = OS_ERROR_TIMEOUT;
        }
        else
        {
            os_result = recvfrom(impl->fd, buffer, buflen, waitflags, sa, &addrlen);
            if (os_result < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    return_code = OS_QUEUE_EMPTY;
                }
                else
                {
                    OS_DEBUG("recvfrom: %s\n", strerror(errno));
                    return_code = OS_ERROR;
                }
            }
            else
            {
                return_code = os_result;

                if (RemoteAddr != NULL)
                {
                    RemoteAddr->ActualLength = addrlen;
                }
            }
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketSendTo_Impl(const OS_object_token_t *token, const void *buffer, size_t buflen,
                           const OS_SockAddr_t *RemoteAddr)
{
    int                             os_result;
    socklen_t                       addrlen;
    const struct sockaddr *         sa;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    sa = (const struct sockaddr *)&RemoteAddr->AddrData;
    switch (sa->sa_family)
    {
        case AF_INET:
            addrlen = sizeof(struct sockaddr_in);
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            addrlen = sizeof(struct sockaddr_in6);
            break;
#endif
        default:
            addrlen = 0;
            break;
    }

    if (addrlen != RemoteAddr->ActualLength)
    {
        return OS_ERR_BAD_ADDRESS;
    }

    os_result = sendto(impl->fd, buffer, buflen, MSG_DONTWAIT, sa, addrlen);
    if (os_result < 0)
    {
        OS_DEBUG("sendto: %s\n", strerror(errno));
        return OS_ERROR;
    }

    return os_result;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketGetInfo_Impl(const OS_object_token_t *token, OS_socket_prop_t *sock_prop)
{
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    sa_family_t             sa_family;
    socklen_t               addrlen;
    OS_SockAddr_Accessor_t *Accessor;

    memset(Addr, 0, sizeof(OS_SockAddr_t));
    Accessor = (OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Domain)
    {
        case OS_SocketDomain_INET:
            sa_family = AF_INET;
            addrlen   = sizeof(struct sockaddr_in);
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case OS_SocketDomain_INET6:
            sa_family = AF_INET6;
            addrlen   = sizeof(struct sockaddr_in6);
            break;
#endif
        default:
            sa_family = 0;
            addrlen   = 0;
            break;
    }

    if (addrlen == 0)
    {
        return OS_ERR_NOT_IMPLEMENTED;
    }

    Addr->ActualLength     = OSAL_SIZE_C(addrlen);
    Accessor->sa.sa_family = sa_family;

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrToString_Impl(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    const void *                  addrbuffer;
    const OS_SockAddr_Accessor_t *Accessor;

    Accessor = (const OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            addrbuffer = &Accessor->sa_in.sin_addr;
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            addrbuffer = &Accessor->sa_in6.sin6_addr;
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
            break;
    }

    if (inet_ntop(Accessor->sa.sa_family, addrbuffer, buffer, buflen) == NULL)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string)
{
    void *                  addrbuffer;
    OS_SockAddr_Accessor_t *Accessor;

    Accessor = (OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            addrbuffer = &Accessor->sa_in.sin_addr;
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            addrbuffer = &Accessor->sa_in6.sin6_addr;
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
            break;
    }

    /* This function is defined as returning 1 on success, not 0 */
    if (inet_pton(Accessor->sa.sa_family, string, addrbuffer) != 1)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    in_port_t                     sa_port;
    const OS_SockAddr_Accessor_t *Accessor;

    Accessor = (const OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            sa_port = Accessor->sa_in.sin_port;
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            sa_port = Accessor->sa_in6.sin6_port;
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
            break;
    }

    *PortNum = ntohs(sa_port);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum)
{
    in_port_t               sa_port;
    OS_SockAddr_Accessor_t *Accessor;

    sa_port  = htons(PortNum);
    Accessor = (OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            Accessor->sa_in.sin_port = sa_port;
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            Accessor->sa_in6.sin6_port = sa_port;
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
    }

    return OS_SUCCESS;
}
