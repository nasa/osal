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
 * Auto-Generated stub implementations for functions defined in osapi-sockets header
 */

#include "osapi-sockets.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_SocketAddrFromString(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketAddrGetPort(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketAddrInit(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketAddrToString(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketOpen(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketRecvFrom(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SocketSendTo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAccept()
 * ----------------------------------------------------
 */
int32 OS_SocketAccept(osal_id_t sock_id, osal_id_t *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAccept, int32);

    UT_GenStub_AddParam(OS_SocketAccept, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketAccept, osal_id_t *, connsock_id);
    UT_GenStub_AddParam(OS_SocketAccept, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAccept, int32, timeout);

    UT_GenStub_Execute(OS_SocketAccept, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAccept, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrFromString()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrFromString, int32);

    UT_GenStub_AddParam(OS_SocketAddrFromString, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrFromString, const char *, string);

    UT_GenStub_Execute(OS_SocketAddrFromString, Basic, UT_DefaultHandler_OS_SocketAddrFromString);

    return UT_GenStub_GetReturnValue(OS_SocketAddrFromString, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrGetPort()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrGetPort, int32);

    UT_GenStub_AddParam(OS_SocketAddrGetPort, uint16 *, PortNum);
    UT_GenStub_AddParam(OS_SocketAddrGetPort, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketAddrGetPort, Basic, UT_DefaultHandler_OS_SocketAddrGetPort);

    return UT_GenStub_GetReturnValue(OS_SocketAddrGetPort, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrInit()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrInit, int32);

    UT_GenStub_AddParam(OS_SocketAddrInit, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrInit, OS_SocketDomain_t, Domain);

    UT_GenStub_Execute(OS_SocketAddrInit, Basic, UT_DefaultHandler_OS_SocketAddrInit);

    return UT_GenStub_GetReturnValue(OS_SocketAddrInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrSetPort()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrSetPort, int32);

    UT_GenStub_AddParam(OS_SocketAddrSetPort, OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketAddrSetPort, uint16, PortNum);

    UT_GenStub_Execute(OS_SocketAddrSetPort, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAddrSetPort, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAddrToString()
 * ----------------------------------------------------
 */
int32 OS_SocketAddrToString(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAddrToString, int32);

    UT_GenStub_AddParam(OS_SocketAddrToString, char *, buffer);
    UT_GenStub_AddParam(OS_SocketAddrToString, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketAddrToString, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketAddrToString, Basic, UT_DefaultHandler_OS_SocketAddrToString);

    return UT_GenStub_GetReturnValue(OS_SocketAddrToString, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketBind()
 * ----------------------------------------------------
 */
int32 OS_SocketBind(osal_id_t sock_id, const OS_SockAddr_t *Addr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketBind, int32);

    UT_GenStub_AddParam(OS_SocketBind, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketBind, const OS_SockAddr_t *, Addr);

    UT_GenStub_Execute(OS_SocketBind, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketBind, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketConnect()
 * ----------------------------------------------------
 */
int32 OS_SocketConnect(osal_id_t sock_id, const OS_SockAddr_t *Addr, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketConnect, int32);

    UT_GenStub_AddParam(OS_SocketConnect, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketConnect, const OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_SocketConnect, int32, timeout);

    UT_GenStub_Execute(OS_SocketConnect, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketConnect, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_SocketGetIdByName(osal_id_t *sock_id, const char *sock_name)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketGetIdByName, int32);

    UT_GenStub_AddParam(OS_SocketGetIdByName, osal_id_t *, sock_id);
    UT_GenStub_AddParam(OS_SocketGetIdByName, const char *, sock_name);

    UT_GenStub_Execute(OS_SocketGetIdByName, Basic, UT_DefaultHandler_OS_SocketGetIdByName);

    return UT_GenStub_GetReturnValue(OS_SocketGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketGetInfo()
 * ----------------------------------------------------
 */
int32 OS_SocketGetInfo(osal_id_t sock_id, OS_socket_prop_t *sock_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketGetInfo, int32);

    UT_GenStub_AddParam(OS_SocketGetInfo, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketGetInfo, OS_socket_prop_t *, sock_prop);

    UT_GenStub_Execute(OS_SocketGetInfo, Basic, UT_DefaultHandler_OS_SocketGetInfo);

    return UT_GenStub_GetReturnValue(OS_SocketGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketOpen()
 * ----------------------------------------------------
 */
int32 OS_SocketOpen(osal_id_t *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketOpen, int32);

    UT_GenStub_AddParam(OS_SocketOpen, osal_id_t *, sock_id);
    UT_GenStub_AddParam(OS_SocketOpen, OS_SocketDomain_t, Domain);
    UT_GenStub_AddParam(OS_SocketOpen, OS_SocketType_t, Type);

    UT_GenStub_Execute(OS_SocketOpen, Basic, UT_DefaultHandler_OS_SocketOpen);

    return UT_GenStub_GetReturnValue(OS_SocketOpen, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketRecvFrom()
 * ----------------------------------------------------
 */
int32 OS_SocketRecvFrom(osal_id_t sock_id, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketRecvFrom, int32);

    UT_GenStub_AddParam(OS_SocketRecvFrom, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketRecvFrom, void *, buffer);
    UT_GenStub_AddParam(OS_SocketRecvFrom, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketRecvFrom, OS_SockAddr_t *, RemoteAddr);
    UT_GenStub_AddParam(OS_SocketRecvFrom, int32, timeout);

    UT_GenStub_Execute(OS_SocketRecvFrom, Basic, UT_DefaultHandler_OS_SocketRecvFrom);

    return UT_GenStub_GetReturnValue(OS_SocketRecvFrom, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketSendTo()
 * ----------------------------------------------------
 */
int32 OS_SocketSendTo(osal_id_t sock_id, const void *buffer, size_t buflen, const OS_SockAddr_t *RemoteAddr)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketSendTo, int32);

    UT_GenStub_AddParam(OS_SocketSendTo, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketSendTo, const void *, buffer);
    UT_GenStub_AddParam(OS_SocketSendTo, size_t, buflen);
    UT_GenStub_AddParam(OS_SocketSendTo, const OS_SockAddr_t *, RemoteAddr);

    UT_GenStub_Execute(OS_SocketSendTo, Basic, UT_DefaultHandler_OS_SocketSendTo);

    return UT_GenStub_GetReturnValue(OS_SocketSendTo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketShutdown()
 * ----------------------------------------------------
 */
int32 OS_SocketShutdown(osal_id_t sock_id, OS_SocketShutdownMode_t Mode)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketShutdown, int32);

    UT_GenStub_AddParam(OS_SocketShutdown, osal_id_t, sock_id);
    UT_GenStub_AddParam(OS_SocketShutdown, OS_SocketShutdownMode_t, Mode);

    UT_GenStub_Execute(OS_SocketShutdown, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketShutdown, int32);
}
