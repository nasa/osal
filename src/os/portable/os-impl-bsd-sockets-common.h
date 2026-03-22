/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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

#ifndef OS_IMPL_BSD_SOCKETS_COMMON_H
#define OS_IMPL_BSD_SOCKETS_COMMON_H

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
 * Default flags implementation: Set the O_NONBLOCK flag via fcntl().
 * An implementation can also elect custom configuration by setting
 * the OS_IMPL_SET_SOCKET_FLAGS macro to point to an alternate function.
 */
void OS_SetSocketDefaultFlags_Impl(const OS_object_token_t *token);

#endif /* OS_IMPL_BSD_SOCKETS_COMMON_H */
