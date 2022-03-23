/**
 * \file
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-network.h"
#include "os-shared-network.h"

#define OS_HOST_NAME_LEN 48

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
    int32 return_code;

    if (gethostname(host_name, name_len) < 0)
    {
        return_code = OS_ERROR;
    }
    else
    {
        host_name[name_len - 1] = 0;
        return_code             = OS_SUCCESS;
    }

    return (return_code);
} /* end OS_NetworkGetHostName_Impl */

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
    int   host_id;
    int32 status;
    char  host_name[OS_HOST_NAME_LEN];

    status = OS_NetworkGetHostName_Impl(host_name, sizeof(host_name));
    if (status == OS_SUCCESS)
    {
        host_id = hostGetByName(host_name);
        if (host_id == ERROR)
        {
            status = OS_ERROR;
        }
        else
        {
            *IdBuf = (int32)host_id;
        }
    }

    return status;

} /* end OS_NetworkGetID_Impl */
