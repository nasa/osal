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
 * \author joseph.p.hickey@nasa.gov
 *
 * This file contains implementation for OS_GetLocalTime() and OS_SetLocalTime()
 * that map to the C library clock_gettime() and clock_settime() calls.
 * This should be usable on any OS that supports those standard calls.
 * The OS-specific code must \#include the correct headers that define the
 * prototypes for these functions before including this implementation file.
 *
 * NOTE: The OS-specific header must also define which POSIX clock ID to use -
 * this specifies the clockid_t parameter to use with clock_gettime().  In
 * most cases this should be CLOCK_REALTIME to allow the clock to be set, and
 * so the application will also see any manual/administrative clock changes.
 *
 * The clock ID is selected by defining the #OSAL_GETTIME_SOURCE_CLOCK macro.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must provide the prototypes of these functions:
 *
 *   clock_gettime()
 *   clock_settime()
 *
 * and the "struct timespec" definition
 */
#include <string.h>
#include <errno.h>

#include "osapi-clock.h"
#include "os-impl-gettime.h"
#include "os-shared-clock.h"

/****************************************************************************************
                                FUNCTIONS
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetLocalTime_Impl(OS_time_t *time_struct)
{
    int             Status;
    int32           ReturnCode;
    struct timespec TimeSp;

    Status = clock_gettime(OSAL_GETTIME_SOURCE_CLOCK, &TimeSp);

    if (Status == 0)
    {
        *time_struct = OS_TimeAssembleFromNanoseconds(TimeSp.tv_sec, TimeSp.tv_nsec);
        ReturnCode   = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error calling clock_gettime: %s\n", strerror(errno));
        ReturnCode = OS_ERROR;
    }

    return ReturnCode;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct)
{
    int             Status;
    int32           ReturnCode;
    struct timespec TimeSp;

    TimeSp.tv_sec  = OS_TimeGetTotalSeconds(*time_struct);
    TimeSp.tv_nsec = OS_TimeGetNanosecondsPart(*time_struct);

    Status = clock_settime(OSAL_GETTIME_SOURCE_CLOCK, &TimeSp);

    if (Status == 0)
    {
        ReturnCode = OS_SUCCESS;
    }
    else
    {
        ReturnCode = OS_ERROR;
    }

    return ReturnCode;
}
