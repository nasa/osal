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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-shared-condvar.h"
#include "os-shared-idmap.h"
#include "os-impl-condvar.h"

/* Tables where the OS object information is stored */
OS_impl_condvar_internal_record_t OS_impl_condvar_table[OS_MAX_CONDVARS];

/*---------------------------------------------------------------------------------------
 * Helper function for releasing the mutex in case the thread
 * executing pthread_cond_wait() is canceled.
 ----------------------------------------------------------------------------------------*/
static void OS_Posix_CondVarReleaseMutex(void *mut)
{
    pthread_mutex_unlock(mut);
}

/****************************************************************************************
                                  CONDVAR API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_CondVarAPI_Impl_Init(void)
{
    memset(OS_impl_condvar_table, 0, sizeof(OS_impl_condvar_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    int32                              final_status;
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    final_status = OS_SUCCESS;
    impl         = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    /*
    ** create the underlying mutex
    */
    status = pthread_mutex_init(&impl->mut, NULL);
    if (status != 0)
    {
        OS_DEBUG("Error: CondVar mutex could not be created. ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), strerror(status));
        final_status = OS_ERROR;
    }
    else
    {
        /*
        ** create the condvar
        */
        status = pthread_cond_init(&impl->cv, NULL);
        if (status != 0)
        {
            pthread_mutex_destroy(&impl->mut);

            OS_DEBUG("Error: CondVar could not be created. ID = %lu: %s\n",
                     OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), strerror(status));
            final_status = OS_ERROR;
        }
    }

    return final_status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarDelete_Impl(const OS_object_token_t *token)
{
    int32                              final_status;
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    final_status = OS_SUCCESS;
    impl         = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    status = pthread_cond_destroy(&impl->cv);
    if (status != 0)
    {
        final_status = OS_ERROR;
    }

    status = pthread_mutex_destroy(&impl->mut);
    if (status != 0)
    {
        final_status = OS_ERROR;
    }

    return final_status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarUnlock_Impl(const OS_object_token_t *token)
{
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    status = pthread_mutex_unlock(&impl->mut);
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarLock_Impl(const OS_object_token_t *token)
{
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    status = pthread_mutex_lock(&impl->mut);
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarSignal_Impl(const OS_object_token_t *token)
{
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    status = pthread_cond_signal(&impl->cv);
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarBroadcast_Impl(const OS_object_token_t *token)
{
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    status = pthread_cond_broadcast(&impl->cv);
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarWait_Impl(const OS_object_token_t *token)
{
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    /*
     * note that because pthread_cond_wait is a cancellation point, this needs to
     * employ the same protection that is in the binsem module.  In the event that
     * the thread is canceled inside pthread_cond_wait, the mutex will be re-acquired
     * before the cancellation occurs, leaving the mutex in a locked state.
     */
    pthread_cleanup_push(OS_Posix_CondVarReleaseMutex, &impl->mut);
    status = pthread_cond_wait(&impl->cv, &impl->mut);
    pthread_cleanup_pop(false);

    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarTimedWait_Impl(const OS_object_token_t *token, const OS_time_t *abs_wakeup_time)
{
    struct timespec                    limit;
    int                                status;
    OS_impl_condvar_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_condvar_table, *token);

    limit.tv_sec  = OS_TimeGetTotalSeconds(*abs_wakeup_time);
    limit.tv_nsec = OS_TimeGetNanosecondsPart(*abs_wakeup_time);

    pthread_cleanup_push(OS_Posix_CondVarReleaseMutex, &impl->mut);
    status = pthread_cond_timedwait(&impl->cv, &impl->mut, &limit);
    pthread_cleanup_pop(false);

    if (status == ETIMEDOUT)
    {
        return OS_ERROR_TIMEOUT;
    }
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarGetInfo_Impl(const OS_object_token_t *token, OS_condvar_prop_t *condvar_prop)
{
    return OS_SUCCESS;
}
