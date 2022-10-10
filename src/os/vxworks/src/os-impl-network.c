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

    return return_code;
}

/*----------------------------------------------------------------
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
}
