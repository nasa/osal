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

    Purpose: Gets the name of the current host

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl(char *host_name, size_t name_len);

/*----------------------------------------------------------------

    Purpose: Gets the ID of the host on the network

    Returns: the ID value on success, or -1 on error.
 ------------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl(int32 *IdBuf);

#endif /* OS_SHARED_NETWORK_H */
