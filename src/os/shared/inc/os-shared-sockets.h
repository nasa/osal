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
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_SOCKETS_H
#define OS_SHARED_SOCKETS_H

#include "osapi-sockets.h"
#include "os-shared-globaldefs.h"

/*
 * Sockets API abstraction layer
 */

/*---------------------------------------------------------------------------------------
   Name: OS_SocketAPI_Init

   Purpose: Initialize the OS-independent layer for network sockets

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_SocketAPI_Init(void);

/*----------------------------------------------------------------

    Purpose: Opens the OS socket indicated by the sock_id table entry

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketOpen_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

   Purpose: Binds the indicated socket table entry to the passed-in address

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketBind_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------

   Purpose: Accept an incoming connection on the indicated socket (must be a STREAM socket)
            Will wait up to "timeout" milliseconds for an incoming connection
            Will wait forever if timeout is negative

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAccept_Impl(const OS_object_token_t *sock_token, const OS_object_token_t *conn_token,
                           OS_SockAddr_t *Addr, int32 timeout);

/*----------------------------------------------------------------

    Purpose: Connects the socket to a remote address.
             Socket must be of the STREAM variety.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketConnect_Impl(const OS_object_token_t *token, const OS_SockAddr_t *Addr, int32 timeout);

/*----------------------------------------------------------------

    Purpose: Graceful shutdown of a stream socket

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketShutdown_Impl(const OS_object_token_t *token, OS_SocketShutdownMode_t Mode);

/*----------------------------------------------------------------

    Purpose: Receives a datagram from the specified socket (must be of the DATAGRAM type)
             Stores the datagram in "buffer" which has a maximum size of "buflen"
             Stores the remote address (sender of the datagram) in "RemoteAddr"
             Will wait up to "timeout" milliseconds to receive a packet
             (zero to poll, negative to wait forever)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketRecvFrom_Impl(const OS_object_token_t *token, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr,
                             int32 timeout);

/*----------------------------------------------------------------

    Purpose: Sends a datagram from the specified socket (must be of the DATAGRAM type)
             to the remote address specified by "RemoteAddr"
             The datagram to send must be stored in "buffer" with a size of "buflen"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketSendTo_Impl(const OS_object_token_t *token, const void *buffer, size_t buflen,
                           const OS_SockAddr_t *RemoteAddr);

/*----------------------------------------------------------------

    Purpose: Get OS-specific information about a socket

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketGetInfo_Impl(const OS_object_token_t *token, OS_socket_prop_t *sock_prop);

/*----------------------------------------------------------------

    Purpose: Initializes an OSAL SockAddr structure to the given address domain

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain);

/*----------------------------------------------------------------

   Purpose: Converts a Socket Address structure to a printable string
            Useful for including a dotted-decimal IP address in a message or log

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrToString_Impl(char *buffer, size_t buflen, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------

   Purpose: Sets the Address portion of the SockAddr structure according to the string
            For IPV4 (SocketDomain_INET) this will parse the dotted decimal IP address.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string);

/*----------------------------------------------------------------

   Purpose: Retrieve the TCP/UDP port number from the SockAddr structure

   NOTE: The port number is output to the caller in native byte order
            (the value is converted from network order before return)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr);

/*----------------------------------------------------------------

    Purpose: Set the TCP/UDP port number in the SockAddr structure

   NOTE: The port number should be passed in native byte order
             (this function will convert to network order)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum);

/*
 * Internal helper functions
 * Not normally called outside the local unit, except during unit test
 */
void OS_CreateSocketName(const OS_object_token_t *token, const OS_SockAddr_t *Addr, const char *parent_name);
void OS_SetSocketDefaultFlags_Impl(const OS_object_token_t *token);

#endif /* OS_SHARED_SOCKETS_H */
