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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to clock getting / setting.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * User defined include files
 */
#include "os-shared-clock.h"

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    /* Check parameters */
    OS_CHECK_POINTER(time_struct);

    return OS_GetLocalTime_Impl(time_struct);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SetLocalTime(const OS_time_t *time_struct)
{
    /* Check parameters */
    OS_CHECK_POINTER(time_struct);

    return OS_SetLocalTime_Impl(time_struct);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 * Helper function to convert a relative timeout to absolute time
 *
 *-----------------------------------------------------------------*/
OS_time_t OS_TimeFromRelativeMilliseconds(int32 relative_msec)
{
    OS_time_t abs_time;

    if (relative_msec == OS_CHECK)
    {
        abs_time = OS_TIME_ZERO;
    }
    else if (relative_msec > 0)
    {
        OS_GetLocalTime_Impl(&abs_time);
        abs_time = OS_TimeAdd(abs_time, OS_TimeFromTotalMilliseconds(relative_msec));
    }
    else
    {
        abs_time = OS_TIME_MAX;
    }

    return abs_time;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 * Helper function to convert an absolute time to relative timeout
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeToRelativeMilliseconds(OS_time_t time)
{
    int64     relative_msec;
    OS_time_t curr_time;

    /*
     * This is an optimization that assumes all negative time values are
     * in the past - no need to read the clock to figure that out.
     *
     * Also OS_TIME_MAX is equivalent to pend forever, and does not
     * need to read the clock either.
     */
    if (OS_TimeGetSign(time) <= 0 || OS_TimeEqual(time, OS_TIME_MAX))
    {
        curr_time = time;
    }
    else
    {
        OS_GetLocalTime_Impl(&curr_time);
        curr_time = OS_TimeSubtract(time, curr_time);
    }

    if (OS_TimeGetSign(curr_time) <= 0)
    {
        /* map this to a polling op (OS_CHECK) */
        relative_msec = OS_CHECK;
    }
    else
    {
        /* note this result is 64 bits, as this can exceed int32 potentially */
        relative_msec = OS_TimeGetTotalMilliseconds(curr_time);

        /* Just in case: cap the value at INT32_MAX */
        if (relative_msec > INT32_MAX)
        {
            /* map this to an indefinite block (OS_PEND) */
            relative_msec = OS_PEND;
        }
    }

    return relative_msec;
}
