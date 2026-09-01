/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
#include "os-shared-mutex.h"
#include "os-shared-idmap.h"
#include "os-impl-mutex.h"

/* Tables where the OS object information is stored */
OS_impl_mutex_internal_record_t OS_impl_mutex_table[OS_MAX_MUTEXES];

/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_MutexAPI_Impl_Init(void)
{
    memset(OS_impl_mutex_table, 0, sizeof(OS_impl_mutex_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    int                              return_code;
    pthread_mutexattr_t              mutex_attr;
    OS_impl_mutex_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);

    /*
    ** initialize the attribute with default values
    */
    return_code = pthread_mutexattr_init(&mutex_attr);
    if (return_code != 0)
    {
        OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_init failed ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)),
                 strerror(return_code));
        return OS_SEM_FAILURE;
    }

    /*
    ** Allow the mutex to use priority inheritance
    */
    return_code = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    if (return_code != 0)
    {
        OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_setprotocol failed ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)),
                 strerror(return_code));
        return OS_SEM_FAILURE;
    }

    /*
    **  Set the mutex type to RECURSIVE so a thread can do nested locks
    */
    return_code = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    if (return_code != 0)
    {
        OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_settype failed ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)),
                 strerror(return_code));
        return OS_SEM_FAILURE;
    }

    /*
    ** Make the mutex robust so that if the owning thread is cancelled
    ** (e.g. during shutdown) while holding the lock, the next attempt to
    ** take the mutex returns EOWNERDEAD instead of deadlocking the other
    ** tasks that pend on it.  See nasa/cFE#2433.
    ** Note: Robust mutex not supported on RTEMS/FACE, skip if unavailable.
    */
#ifdef PTHREAD_MUTEX_ROBUST
    return_code = pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_ROBUST);
    if (return_code != 0)
    {
        OS_DEBUG("Error: Mutex could not be created. pthread_mutexattr_setrobust failed ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)),
                 strerror(return_code));
        return OS_SEM_FAILURE;
    }
#endif

    /*
    ** create the mutex
    ** upon successful initialization, the state of the mutex becomes initialized and unlocked
    */
    return_code = pthread_mutex_init(&impl->id, &mutex_attr);
    if (return_code != 0)
    {
        OS_DEBUG("Error: Mutex could not be created. ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)),
                 strerror(return_code));
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
int32 OS_MutSemDelete_Impl(const OS_object_token_t *token)
{
    int                              status;
    OS_impl_mutex_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);

    status = pthread_mutex_destroy(&(impl->id)); /* 0 = success */

    if (status != 0)
    {
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
int32 OS_MutSemGive_Impl(const OS_object_token_t *token)
{
    int                              status;
    OS_impl_mutex_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);

    /*
     ** Unlock the mutex
     */
    status = pthread_mutex_unlock(&(impl->id));
    if (status != 0)
    {
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
int32 OS_MutSemTake_Impl(const OS_object_token_t *token)
{
    int                              status;
    OS_impl_mutex_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);

    /*
    ** Lock the mutex
    */
    status = pthread_mutex_lock(&(impl->id));
#ifdef EOWNERDEAD
    if (status == EOWNERDEAD)
    {
        /*
        ** The previous owner terminated while holding the mutex (e.g. thread
        ** cancellation during shutdown).  Restore it to a consistent state so
        ** the lock can be safely used.  See nasa/cFE#2433.
        ** Note: Robust not supported on RTEMS/FACE, guard EOWNERDEAD.
        */
        status = pthread_mutex_consistent(&(impl->id));
    }
#endif
    if (status != 0)
    {
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
int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop)
{
    return OS_SUCCESS;
}

