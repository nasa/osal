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
#include "os-impl-bsd-sockets-common.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

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
