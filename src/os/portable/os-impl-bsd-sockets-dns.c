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
#include <netdb.h>

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
    int32                         status;
    const OS_SockAddr_Accessor_t *Accessor;
    int                           rc;

    Accessor = (const OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
            break;
    }

    rc = getnameinfo(&Accessor->sa, Addr->ActualLength, buffer, buflen, NULL, 0, 0);
    if (rc == 0)
    {
        status = OS_SUCCESS;
    }
    else if (rc == EAI_AGAIN)
    {
        status = OS_ERR_TRY_AGAIN;
    }
    else if (rc == EAI_NONAME)
    {
        status = OS_ERR_EMPTY_SET;
    }
    else
    {
        status = OS_ERROR;
    }

    return status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string)
{
    int32                   status;
    size_t                  addr_offset;
    size_t                  addr_len;
    OS_SockAddr_Accessor_t *Accessor;
    struct addrinfo *       res;
    struct addrinfo *       rp;
    int                     rc;

    Accessor = (OS_SockAddr_Accessor_t *)&Addr->AddrData;

    switch (Accessor->sa.sa_family)
    {
        case AF_INET:
            addr_offset = offsetof(struct sockaddr_in, sin_addr);
            addr_len    = sizeof(Accessor->sa_in.sin_addr);
            break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
        case AF_INET6:
            addr_offset = offsetof(struct sockaddr_in6, sin6_addr);
            addr_len    = sizeof(Accessor->sa_in6.sin6_addr);
            break;
#endif
        default:
            return OS_ERR_BAD_ADDRESS;
            break;
    }

    status = OS_ERR_EMPTY_SET;

    rc = getaddrinfo(string, NULL, NULL, &res);
    if (rc == 0)
    {
        for (rp = res; rp != NULL; rp = rp->ai_next)
        {
            /* the library call should never return something that does not meet this criteria */
            if (rp->ai_family == Accessor->sa.sa_family && rp->ai_addrlen >= sizeof(struct sockaddr))
            {
                memcpy(&Addr->AddrData.Buffer[addr_offset], (const char *)rp->ai_addr + addr_offset, addr_len);
                Addr->ActualLength = rp->ai_addrlen;
                status             = OS_SUCCESS;
                break;
            }
        }

        freeaddrinfo(res);
    }
    else if (rc == EAI_AGAIN)
    {
        /* Indicate the error is temporary; treat this like a timeout or similar */
        status = OS_ERR_TRY_AGAIN;
    }
    else if (rc != EAI_NONAME)
    {
        status = OS_ERROR;
    }

    return status;
}
