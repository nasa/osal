/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_NETWORK_H
#define OS_SHARED_NETWORK_H

#include "osapi-network.h"
#include "os-shared-globaldefs.h"

/****************************************************************************************
              NETWORK / SOCKET API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_NetworkAPI_Init

   Purpose: Initialize the OS-independent layer for network services

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_NetworkAPI_Init(void);

/*----------------------------------------------------------------

   Function: OS_NetworkGetHostName_Impl

    Purpose: Gets the name of the current host

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl(char *host_name, size_t name_len);

/*----------------------------------------------------------------
   Function: OS_NetworkGetID_Impl

    Purpose: Gets the ID of the host on the network

    Returns: the ID value on success, or -1 on error.
 ------------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl(int32 *IdBuf);

#endif /* OS_SHARED_NETWORK_H */
