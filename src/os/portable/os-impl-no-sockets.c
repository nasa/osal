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
 * \file   os-impl-no-sockets.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: All functions return OS_ERR_NOT_IMPLEMENTED.
 * This is used when network functionality is disabled by config.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "osapi-sockets.h"
#include "os-shared-sockets.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                    Socket API
 ***************************************************************************************/

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketOpen_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketBind_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketConnect_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr, int32 timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAccept_Impl(const OS_object_token_t *sock_token, const OS_object_token_t *conn_token,
                           OS_SockAddr_t *Addr, int32 timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketRecvFrom_Impl(const OS_object_token_t *token, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr,
                             int32 timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketSendTo_Impl(const OS_object_token_t *token, const void *buffer, size_t buflen,
                           const OS_SockAddr_t *RemoteAddr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketGetInfo_Impl(const OS_object_token_t *token, OS_socket_prop_t *sock_prop)
{
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrToString_Impl(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no network configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
