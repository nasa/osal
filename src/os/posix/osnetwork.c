/**
 * \file   osnetwork.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the network functionality for the osapi.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"

/*
 * FIXME: this belongs in the osconfig.h file, really.
 */
#define OS_NETWORK_SUPPORTS_IPV6

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>



#ifdef OS_INCLUDE_NETWORK


/*
 * Leverage the POSIX-style File I/O as this will mostly work on RTEMS,
 * with the exception that regular files cannot be passed to select(),
 * which means that we should NOT set the O_NONBLOCK flag on filehandles
 * like the standard POSIX OSAL does.
 */
const int OS_IMPL_SOCKET_FLAGS = O_NONBLOCK;

/****************************************************************************************
                                     COMMON ROUTINES
 After including the OS/C-library specific include files, the basic UNIX file API is used
 ****************************************************************************************/

/*
 * The "bsd-socket" portable block includes the generic
 * bsd-style network operations
 */
#include "../portable/os-impl-bsd-sockets.c"


/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    /* BSD-style gethostid() has no failure modes */
    *IdBuf = gethostid();
    return OS_SUCCESS;
} /* end OS_NetworkGetID_Impl */

#else       /* OS_INCLUDE_NETWORK */


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


