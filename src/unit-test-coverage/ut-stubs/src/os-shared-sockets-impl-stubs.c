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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-sockets header
 */

#include "os-shared-sockets.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_SocketAddrGetPort_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SetSocketDefaultFlags_Impl()
 * ----------------------------------------------------
 */
void OS_SetSocketDefaultFlags_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_SetSocketDefaultFlags_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_SetSocketDefaultFlags_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAccept_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAccept_Impl(const OS_object_token_t *sock_token, const OS_object_token_t *conn_token,
                           OS_SockAddr_t *Addr, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAccept_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAccept_Impl, const OS_object_token_t *, sock_token);
    UT_GenStub_AddParam(OS_SocketAccept_Impl, const OS_object_token_t *, conn_token);
    UT_GenStub_AddParam(OS_SocketAccept_Impl, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAccept_Impl, int32, timeout);

    UT_GenStub_Execute(OS_SocketAccept_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAccept_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrFromString_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrFromString_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAddrFromString_Impl, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrFromString_Impl, const char *, string);

    UT_GenStub_Execute(OS_SocketAddrFromString_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAddrFromString_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrGetPort_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrGetPort_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAddrGetPort_Impl, uint16 *, PortNum);
    UT_GenStub_AddParam(OS_SocketAddrGetPort_Impl, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketAddrGetPort_Impl, Basic, UT_DefaultHandler_OS_SocketAddrGetPort_Impl);

    return UT_GenStub_GetReturnValue(OS_SocketAddrGetPort_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrInit_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrInit_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAddrInit_Impl, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrInit_Impl, OS_SocketDomain_t, Domain);

    UT_GenStub_Execute(OS_SocketAddrInit_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAddrInit_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrSetPort_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrSetPort_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAddrSetPort_Impl, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrSetPort_Impl, uint16, PortNum);

    UT_GenStub_Execute(OS_SocketAddrSetPort_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAddrSetPort_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrToString_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrToString_Impl(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrToString_Impl, int32);

    UT_GenStub_AddParam(OS_SocketAddrToString_Impl, char *, buffer);
    UT_GenStub_AddParam(OS_SocketAddrToString_Impl, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketAddrToString_Impl, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketAddrToString_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAddrToString_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketBind_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketBind_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketBind_Impl, int32);

    UT_GenStub_AddParam(OS_SocketBind_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketBind_Impl, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketBind_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketBind_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketConnect_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketConnect_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketConnect_Impl, int32);

    UT_GenStub_AddParam(OS_SocketConnect_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketConnect_Impl, const OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketConnect_Impl, int32, timeout);

    UT_GenStub_Execute(OS_SocketConnect_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketConnect_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketGetInfo_Impl(const OS_object_token_t *token, OS_socket_prop_t *sock_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_SocketGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketGetInfo_Impl, OS_socket_prop_t *, sock_prop);

    UT_GenStub_Execute(OS_SocketGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketOpen_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketOpen_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketOpen_Impl, int32);

    UT_GenStub_AddParam(OS_SocketOpen_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_SocketOpen_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketOpen_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketRecvFrom_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketRecvFrom_Impl(const OS_object_token_t *token, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr,
                             int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketRecvFrom_Impl, int32);

    UT_GenStub_AddParam(OS_SocketRecvFrom_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketRecvFrom_Impl, void *, buffer);
    UT_GenStub_AddParam(OS_SocketRecvFrom_Impl, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketRecvFrom_Impl, OS_SockAddr_t *, RemoteAddr);
    UT_GenStub_AddParam(OS_SocketRecvFrom_Impl, int32, timeout);

    UT_GenStub_Execute(OS_SocketRecvFrom_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketRecvFrom_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketSendTo_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketSendTo_Impl(const OS_object_token_t *token, const void *buffer, size_t buflen,
                           const OS_SockAddr_t *RemoteAddr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketSendTo_Impl, int32);

    UT_GenStub_AddParam(OS_SocketSendTo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketSendTo_Impl, const void *, buffer);
    UT_GenStub_AddParam(OS_SocketSendTo_Impl, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketSendTo_Impl, const OS_SockAddr_t *, RemoteAddr);

    UT_GenStub_Execute(OS_SocketSendTo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketSendTo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketShutdown_Impl()
 * ----------------------------------------------------
 */
int32 OS_SocketShutdown_Impl(const OS_object_token_t *token, OS_SocketShutdownMode_t Mode)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketShutdown_Impl, int32);

    UT_GenStub_AddParam(OS_SocketShutdown_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SocketShutdown_Impl, OS_SocketShutdownMode_t, Mode);

    UT_GenStub_Execute(OS_SocketShutdown_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketShutdown_Impl, int32);
}
