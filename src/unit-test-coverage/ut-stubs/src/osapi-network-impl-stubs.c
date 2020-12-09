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
 * \file     osapi-network-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-network.h"
#include "os-shared-sockets.h"

/*
 * Sockets API abstraction layer
 */
UT_DEFAULT_STUB(OS_SocketOpen_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_SocketClose_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_SocketBind_Impl, (const OS_object_token_t *token, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAccept_Impl, (const OS_object_token_t *sock_token, const OS_object_token_t *conn_token,
                                       OS_SockAddr_t *Addr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketConnect_Impl, (const OS_object_token_t *token, const OS_SockAddr_t *Addr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketRecvFrom_Impl,
                (const OS_object_token_t *token, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketSendTo_Impl,
                (const OS_object_token_t *token, const void *buffer, size_t buflen, const OS_SockAddr_t *RemoteAddr))
UT_DEFAULT_STUB(OS_SocketGetInfo_Impl, (const OS_object_token_t *token, OS_socket_prop_t *sock_prop))

UT_DEFAULT_STUB(OS_SocketAddrInit_Impl, (OS_SockAddr_t * Addr, OS_SocketDomain_t Domain))
UT_DEFAULT_STUB(OS_SocketAddrToString_Impl, (char *buffer, size_t buflen, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAddrGetPort_Impl, (uint16 * PortNum, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAddrFromString_Impl, (OS_SockAddr_t * Addr, const char *string))
UT_DEFAULT_STUB(OS_SocketAddrSetPort_Impl, (OS_SockAddr_t * Addr, uint16 PortNum))
UT_DEFAULT_STUB(OS_NetworkGetHostName_Impl, (char *host_name, size_t name_len))

int32 OS_NetworkGetID_Impl(int32 *IdBuf)
{
    int32 Status = UT_DEFAULT_IMPL(OS_NetworkGetID_Impl);
    if (Status == 0)
    {
        *IdBuf = 42;
    }
    return Status;
}
