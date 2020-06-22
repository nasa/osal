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
 * \file   osnetwork.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the network functionality for the osapi.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"

#ifdef OS_INCLUDE_NETWORK

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>


/*
 * Leverage the POSIX-style File I/O as this will mostly work on RTEMS,
 * with the exception that regular files cannot be passed to select(),
 * which means that we should NOT set the O_NONBLOCK flag on filehandles
 * like the standard POSIX OSAL does.
 */
const int OS_IMPL_SOCKET_FLAGS = O_NONBLOCK;

/* Leverage the portable BSD sockets implementation */
#include "../portable/os-impl-bsd-sockets.c"

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl          (int32 *IdBuf)
{
    /* RTEMS does not have the GetHostId call -
     * it is deprecated in other OS's anyway and not a good idea to use it
     */
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetID_Impl */

#else  /* OS_INCLUDE_NETWORK */

/****************************************************************************************
                         NOT IMPLEMENTED OPTION
            This block provides stubs in case this module is disabled by config
 ****************************************************************************************/

/*
 * The "no-network" block includes the required API calls
 * that all return OS_ERR_NOT_IMPLEMENTED
 */
#include "../portable/os-impl-no-network.c"


#endif

