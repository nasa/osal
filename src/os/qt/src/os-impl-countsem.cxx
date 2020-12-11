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
 * \file     os-impl-countsem.c
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "os-impl-countsem.h"
#include "os-shared-countsem.h"
#include "os-shared-idmap.h"

/*
 * Added SEM_VALUE_MAX Define
 */
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX (UINT32_MAX / 2)
#endif

/* Tables where the OS object information is stored */
OS_impl_countsem_internal_record_t OS_impl_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

/****************************************************************************************
                               COUNTING SEMAPHORE API
 ***************************************************************************************/

/*
 * Unlike binary semaphores, counting semaphores can use the standard QT semaphore facility.
 * This has the advantage of more correct behavior on "give" operations:
 *  - give may be done from a signal / ISR context
 *  - give should not cause an unexpected task switch nor should it ever block
 */

/*---------------------------------------------------------------------------------------
   Name: OS_QT_CountSemAPI_Impl_Init

   Purpose: Initialize the Counting Semaphore data structures

---------------------------------------------------------------------------------------*/
int32 OS_QT_CountSemAPI_Impl_Init(void)
{
    memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
    return OS_SUCCESS;
} /* end OS_QT_CountSemAPI_Impl_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    if (sem_initial_value > SEM_VALUE_MAX)
    {
        return OS_INVALID_SEM_VALUE;
    }

    impl->id = new QSemaphore();
    if(impl->id == NULL)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_CountSemCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);
    delete impl->id;
    impl->id = 0x0;
    /* TODO Warning: Destroying a semaphore that is in use may result in undefined behavior.*/
    // return OS_SEM_FAILURE;

    return OS_SUCCESS;

} /* end OS_CountSemDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGive_Impl(const OS_object_token_t *token)
{
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);
    impl->id->release(1);

    return OS_SUCCESS;

} /* end OS_CountSemGive_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake_Impl(const OS_object_token_t *token)
{
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);
    impl->id->acquire(1);

    return OS_SUCCESS;
} /* end OS_CountSemTake_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    struct timespec                     ts;
    int                                 result;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    /*
     ** Compute an absolute time for the delay
     */
    if(impl->id->tryAcquire(1, msecs) == false)
        return OS_SEM_TIMEOUT;

    return OS_SUCCESS;
} /* end OS_CountSemTimedWait_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetInfo_Impl(const OS_object_token_t *token, OS_count_sem_prop_t *count_prop)
{
    int                                 sval;
    OS_impl_countsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_count_sem_table, *token);

    /* put the info into the stucture */
    count_prop->value = impl->id->available();
    return OS_SUCCESS;
} /* end OS_CountSemGetInfo_Impl */
