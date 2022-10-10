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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-impl-countsem.h"

#include "os-shared-countsem.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define MAX_SEM_VALUE 0x7FFFFFFF

/*
 * Define all of the RTEMS semaphore attributes
 * 4. OSAL Counting Semaphore attributes
 *     This is a counting semaphore with priority wait order.
 */

#define OSAL_COUNT_SEM_ATTRIBS (RTEMS_PRIORITY)

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*  tables for the properties of objects */
OS_impl_countsem_internal_record_t OS_impl_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_CountSemAPI_Impl_Init(void)
{
    memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    rtems_status_code                   status;
    rtems_name                          r_name;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    /*
    ** Verify that the semaphore maximum value is not too high
    */
    if (sem_initial_value > MAX_SEM_VALUE)
    {
        return OS_INVALID_SEM_VALUE;
    }

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL ID in here, as we know it is already unique
    ** and trying to use the real name would be less than useful (only 4 chars)
    */
    r_name = OS_ObjectIdToInteger(OS_ObjectIdFromToken(token));
    status = rtems_semaphore_create(r_name, sem_initial_value, OSAL_COUNT_SEM_ATTRIBS, 0, &(impl->id));

    /* check if Create failed */
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n", rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl(const OS_object_token_t *token)
{
    rtems_status_code                   status;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    status = rtems_semaphore_delete(impl->id);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_delete error: %s\n", rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGive_Impl(const OS_object_token_t *token)
{
    rtems_status_code                   status;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    status = rtems_semaphore_release(impl->id);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n", rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake_Impl(const OS_object_token_t *token)
{
    rtems_status_code                   status;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    status = rtems_semaphore_obtain(impl->id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n", rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    rtems_status_code                   status;
    int                                 TimeInTicks;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    if (OS_Milli2Ticks(msecs, &TimeInTicks) != OS_SUCCESS)
    {
        return OS_ERROR;
    }

    status = rtems_semaphore_obtain(impl->id, RTEMS_WAIT, TimeInTicks);
    if (status == RTEMS_TIMEOUT)
    {
        return OS_SEM_TIMEOUT;
    }

    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n", rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetInfo_Impl(const OS_object_token_t *token, OS_count_sem_prop_t *count_prop)
{
    /* RTEMS does not provide an API to get the value */
    return OS_SUCCESS;
}
