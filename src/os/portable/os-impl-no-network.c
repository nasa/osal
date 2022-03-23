/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains the network implementation for
 * systems where OSAL_CONFIG_INCLUDE_NETWORK is false or otherwise
 * do not provide any network functions.
 *
 */

#include "os-shared-network.h"

/****************************************************************************************
                                    Network API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl(int32 *IdBuf)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetID_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl(char *host_name, size_t name_len)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetHostName_Impl */
