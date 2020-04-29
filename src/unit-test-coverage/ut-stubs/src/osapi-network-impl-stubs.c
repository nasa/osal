/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
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
UT_DEFAULT_STUB(OS_SocketOpen_Impl,(uint32 sock_id))
UT_DEFAULT_STUB(OS_SocketClose_Impl,(uint32 sock_id))
UT_DEFAULT_STUB(OS_SocketBind_Impl,(uint32 sock_id, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAccept_Impl,(uint32 sock_id, uint32 connsock_id, OS_SockAddr_t *Addr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketConnect_Impl,(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketRecvFrom_Impl,(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout))
UT_DEFAULT_STUB(OS_SocketSendTo_Impl,(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr))
UT_DEFAULT_STUB(OS_SocketGetInfo_Impl,(uint32 sock_id, OS_socket_prop_t *sock_prop))

UT_DEFAULT_STUB(OS_SocketAddrInit_Impl,(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain))
UT_DEFAULT_STUB(OS_SocketAddrToString_Impl,(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAddrGetPort_Impl,(uint16 *PortNum, const OS_SockAddr_t *Addr))
UT_DEFAULT_STUB(OS_SocketAddrFromString_Impl,(OS_SockAddr_t *Addr, const char *string))
UT_DEFAULT_STUB(OS_SocketAddrSetPort_Impl,(OS_SockAddr_t *Addr, uint16 PortNum))
UT_DEFAULT_STUB(OS_NetworkGetHostName_Impl,(char *host_name, uint32 name_len))

int32 OS_NetworkGetID_Impl       (int32 *IdBuf)
{
    int32 Status = UT_DEFAULT_IMPL(OS_NetworkGetID_Impl);
    if (Status == 0)
    {
        *IdBuf = 42;
    }
    return Status;
}

