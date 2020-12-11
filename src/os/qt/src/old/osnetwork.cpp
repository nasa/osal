/*
** File   : osnetwork.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Nicholas Yanchik
**
** Purpose: This file contains the network functionality for the osapi.
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "common_types.h"
#include "osapi.h"
#include <QtCore>
#include <QNetworkInterface>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/



/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/


/****************************************************************************************
                                    Network API
****************************************************************************************/
/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetID
    
    Purpose: Get the Host ID from Networking

    Returns: OS_ERROR if the  host id could not be found
             an opaque 32 bit host id if success (NOT AN IPv4 ADDRESS).

    WARNING: OS_NetworkGetID is currently [[deprecated]] as its behavior is
             unknown and not consistent across operating systems.

---------------------------------------------------------------------------------------*/
int32 OS_NetworkGetID             (void)
{
    int    host_id = -1;

  QList<QHostAddress> list = QNetworkInterface::allAddresses();

   for(int nIter=0; nIter<list.count(); nIter++)

    {
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                host_id = list[nIter].toIPv4Address();
    }
   return host_id;
    
}/* end OS_NetworkGetID */
/*--------------------------------------------------------------------------------------
    Name: OS_NetworkGetHostName
    
    Purpose: Gets the name of the current host

    Returns: OS_ERROR if the  host name could not be found
             OS_SUCCESS if the name was copied to host_name successfully
---------------------------------------------------------------------------------------*/

int32 OS_NetworkGetHostName       (char *host_name, uint32 name_len)
{
   int    retval;
   uint32 return_code;
   
   if ( host_name == NULL)
   {
      return_code = OS_INVALID_POINTER;
   }
   else if ( name_len == 0 )
   {
      return_code = OS_ERROR;
   }
   else
   {
      retval = gethostname( host_name, name_len);
      if ( retval == -1 )
      {
      
         return_code = OS_ERROR;
      }
      else
      {
         return_code = OS_SUCCESS;
      }
   }

   return(return_code);
}/* end OS_NetworkGetHostName */


/*--------------------------------------------------------------------------------------
    Name: OS_OpenSocket

    Purpose: Create a socket of type specified via the input domain and type.
    @NOTE socket id is not used currently as the id will be returned via the
    socket decriptor returned when the socket is created.

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketOpen(uint32 *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
{
    int32 socketDescriptor = -1;

    if((Domain == OS_SocketDomain_INET) && (Type == OS_SocketType_DATAGRAM))
    {
        socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else if((Domain == OS_SocketDomain_INET6) && (Type == OS_SocketType_DATAGRAM))
    {
        socketDescriptor = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    }
    else if((Domain == OS_SocketDomain_INET) && (Type == OS_SocketType_STREAM))
    {
        socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if((Domain == OS_SocketDomain_INET6) && (Type == OS_SocketType_STREAM))
    {
        socketDescriptor = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    }

    if(socketDescriptor == -1)
    {
        return OS_ERROR;
    }

    return (socketDescriptor);

}/* end OS_OpenSocket */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketClose

    Purpose: Close a file descriptor. Any record locks held on the file
    it was associated with, and owned by the process, are removed.
    @NOTE Calls OS_Close from osapi.c
---------------------------------------------------------------------------------------*/
int32 OS_SocketClose(uint32 sock_id)
{
    close(sock_id);
    return 0;
}/* end OS_SocketClose */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketBind

    Purpose: Assigns the address specified by addr to the socket referred to by the file
    descriptor sockfd.  addrlen specifies the size, in bytes, of the
    address structure pointed to by addr.

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketBind(uint32 sock_id, const OS_SockAddr_t *Addr)
{
    int32 retVal;

    retVal = bind(sock_id,(struct sockaddr *) Addr, sizeof(OS_SockAddr_t));

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketBind */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketConnect

    Purpose: Connects the socket referred to by the file descriptor
    based on the socket descriptor specified by addr.

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketConnect(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout)
{
    int32 retVal;

    retVal = connect(sock_id, (struct sockaddr *) Addr, sizeof(OS_SockAddr_t));

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketConnect */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketAccept

    Purpose: Waits for a incoming client clonnection on a specified socket. Once
    client request is seen accept the request.

    @NOTE timeout not currently used in with Linux version

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketAccept(uint32 sock_id, uint32 *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
    int32 retVal;
	uint32 sz = sizeof(OS_SockAddr_t);

    retVal = accept(sock_id, (struct sockaddr *) Addr, &sz);

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketAccept */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketGetIdByName

    Purpose: Get the host id based on the ip address passed into the function.

    Returns: OS_ERROR if get function returns null
---------------------------------------------------------------------------------------*/
int32 OS_SocketGetIdByName(const char *sock_name)
{
	uint32 ipAddr = 1;
    struct hostent *name;
	name = gethostbyname(sock_name);

	ipAddr = inet_network(name->h_addr);

    if((name == NULL) || (ipAddr < 0))
    {
        return OS_ERROR;
    }

    return (ipAddr);
}/* end OS_SocketGetIdByName */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketRecv

    Purpose: Recv data from an open file descriptor.

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketRecv(uint32 sock_id, void *buffer, uint32 buflen, int32 flags)
{
    int32 retVal;
    int32 intFlags = 0;

    // if(flags & OS_SocketFlag_MsgPeek)
    // {
    //     intFlags |= MSG_PEEK;
    // }
    // if(flags & OS_SocketFlag_Msg_DontWait)
    // {
    //     intFlags |= MSG_DONTWAIT;
    // }

    retVal = recv(sock_id, buffer, buflen, intFlags);

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketRecv */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketSend

    Purpose: Write data to an open file descriptor.

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketSend(uint32 sock_id, void *buffer, uint32 buflen, int32 flags)
{
    int32 retVal;

    retVal = send(sock_id, buffer, buflen, flags);

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketSend */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketSelect

    Purpose: Waits for a incoming client clonnection on a specified socket. Once
    client request is seen accept the request.

    Returns: OS_ERROR if select input arguments incorrect or fails (times out)
---------------------------------------------------------------------------------------*/
int32 OS_SocketSelect(uint32 sock_id, const char rw_char, struct timeval *timeout)
{
    int32 status;
    fd_set set;
    fd_set errSet;

    FD_ZERO(&set);
    FD_ZERO(&errSet);
    FD_SET(sock_id, &set);
    FD_SET(sock_id, &errSet);

    if(sock_id < 0) //Bad file descriptors input
    {
        return OS_ERROR;
    }

    if(rw_char == 'r')
    {
        status = select(sock_id+1, &set, NULL, &errSet, timeout);
    }
    else if(rw_char == 'w')
    {
        status = select(sock_id+1, NULL, &set, &errSet, timeout);
    }
    else
    {
        return OS_ERROR;
    }

    if(status <= 0)
    {
        return OS_ERROR;
    }

    if(!(FD_ISSET(sock_id, &set)))
    {
       status = -1;
    }

    return (status);
}/* end OS_SocketSelect */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketSetOpt

    Purpose: Select options for socket

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketSetOpt(uint32 sock_id, int Option, const void *OptionVal, int32 len)
{
    int32 retVal;
    int32 SelectedOption;
    enum OS_SockOptions{
        OS_SocketOpt_DEBUG,
        OS_SocketOpt_BROADCAST,
        OS_SocketOpt_REUSEADDR,
        OS_SocketOpt_KEEPALIVE
    };
    switch(Option)
    {
        case OS_SocketOpt_DEBUG:
            SelectedOption = SO_DEBUG;
            break;
        case OS_SocketOpt_BROADCAST:
            SelectedOption = SO_BROADCAST;
            break;
        case OS_SocketOpt_REUSEADDR:
            SelectedOption = SO_REUSEADDR;
            break;
        case OS_SocketOpt_KEEPALIVE:
            SelectedOption = SO_KEEPALIVE;
            break;
        default:
            return OS_ERROR;
    }

    retVal = setsockopt(sock_id, SOL_SOCKET, SelectedOption, OptionVal, len);

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketSetOpt */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketListen

    Purpose: Listen for incoming client connections

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
int32 OS_SocketListen(uint32 sock_id, int32 backlog)
{
    int32 retVal;

    retVal = listen(sock_id, backlog);

    if(retVal == -1)
    {
        return OS_ERROR;
    }

    return (retVal);
}/* end OS_SocketListen */

/*--------------------------------------------------------------------------------------
    Name: OS_SocketListen

    Purpose: Configure the OS sock structure according to specified inputs

    Returns: OS_ERROR if socket creation failed
---------------------------------------------------------------------------------------*/
typedef char OS_sockaddr_in_t ;
int32 OS_SocketConfigStruct(OS_sockaddr_in_t *addr, OS_SocketDomain_t Domain, uint32 AddrOption, uint16 Port)
{
	switch (Domain)
	{
		case OS_SocketDomain_INET:
			break;
		default:
			return OS_ERR_NOT_IMPLEMENTED;
	}

    // addr->sin_family = AF_INET;
    // addr->sin_addr.s_addr = htonl(INADDR_ANY);
    // addr->sin_port = htons(Port);

	return OS_SUCCESS;
}















int32 OS_SocketRecvFrom(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketSendTo(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketGetInfo (uint32 sock_id, OS_socket_prop_t *sock_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketAddrToString(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
