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
 * \file     os-impl-idmap.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "bsp-impl.h"
#include <sched.h>

#include "os-shared-idmap.h"

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} POSIX_GlobalLock_t;

static POSIX_GlobalLock_t OS_global_task_table_mut;
static POSIX_GlobalLock_t OS_queue_table_mut;
static POSIX_GlobalLock_t OS_bin_sem_table_mut;
static POSIX_GlobalLock_t OS_mutex_table_mut;
static POSIX_GlobalLock_t OS_count_sem_table_mut;
static POSIX_GlobalLock_t OS_stream_table_mut;
static POSIX_GlobalLock_t OS_dir_table_mut;
static POSIX_GlobalLock_t OS_timebase_table_mut;
static POSIX_GlobalLock_t OS_timecb_table_mut;
static POSIX_GlobalLock_t OS_module_table_mut;
static POSIX_GlobalLock_t OS_filesys_table_mut;
static POSIX_GlobalLock_t OS_console_mut;

static POSIX_GlobalLock_t *const MUTEX_TABLE[] = {
    [OS_OBJECT_TYPE_UNDEFINED]   = NULL,
    [OS_OBJECT_TYPE_OS_TASK]     = &OS_global_task_table_mut,
    [OS_OBJECT_TYPE_OS_QUEUE]    = &OS_queue_table_mut,
    [OS_OBJECT_TYPE_OS_COUNTSEM] = &OS_count_sem_table_mut,
    [OS_OBJECT_TYPE_OS_BINSEM]   = &OS_bin_sem_table_mut,
    [OS_OBJECT_TYPE_OS_MUTEX]    = &OS_mutex_table_mut,
    [OS_OBJECT_TYPE_OS_STREAM]   = &OS_stream_table_mut,
    [OS_OBJECT_TYPE_OS_DIR]      = &OS_dir_table_mut,
    [OS_OBJECT_TYPE_OS_TIMEBASE] = &OS_timebase_table_mut,
    [OS_OBJECT_TYPE_OS_TIMECB]   = &OS_timecb_table_mut,
    [OS_OBJECT_TYPE_OS_MODULE]   = &OS_module_table_mut,
    [OS_OBJECT_TYPE_OS_FILESYS]  = &OS_filesys_table_mut,
    [OS_OBJECT_TYPE_OS_CONSOLE]  = &OS_console_mut,
};

enum
{
    MUTEX_TABLE_SIZE = (sizeof(MUTEX_TABLE) / sizeof(MUTEX_TABLE[0]))
};

/*---------------------------------------------------------------------------------------
 * Helper function for releasing the mutex in case the thread
 * executing pthread_condwait() is canceled.
 ----------------------------------------------------------------------------------------*/
void OS_Posix_ReleaseTableMutex(void *mut)
{
    pthread_mutex_unlock(mut);
}

/*----------------------------------------------------------------
 *
 * Function: OS_Lock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    POSIX_GlobalLock_t *mut;
    int                 ret;

    if (idtype < MUTEX_TABLE_SIZE)
    {
        mut = MUTEX_TABLE[idtype];
    }
    else
    {
        mut = NULL;
    }

    if (mut != NULL)
    {
        ret = pthread_mutex_lock(&mut->mutex);
        if (ret != 0)
        {
            OS_DEBUG("pthread_mutex_lock(&mut->mutex): %s", strerror(ret));
            return OS_ERROR;
        }
    }

    return OS_SUCCESS;
} /* end OS_Lock_Global_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_Unlock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    POSIX_GlobalLock_t *mut;
    int                 ret;

    if (idtype < MUTEX_TABLE_SIZE)
    {
        mut = MUTEX_TABLE[idtype];
    }
    else
    {
        mut = NULL;
    }

    if (mut != NULL)
    {
        /* Notify any waiting threads that the state _may_ have changed */
        ret = pthread_cond_broadcast(&mut->cond);
        if (ret != 0)
        {
            OS_DEBUG("pthread_cond_broadcast(&mut->cond): %s", strerror(ret));
            /* unexpected but keep going (not critical) */
        }

        ret = pthread_mutex_unlock(&mut->mutex);
        if (ret != 0)
        {
            OS_DEBUG("pthread_mutex_unlock(&mut->mutex): %s", strerror(ret));
            return OS_ERROR;
        }
    }

    return OS_SUCCESS;
} /* end OS_Unlock_Global_Impl */

/*----------------------------------------------------------------
 *
 *  Function: OS_WaitForStateChange_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_WaitForStateChange_Impl(osal_objtype_t idtype, uint32 attempts)
{
    POSIX_GlobalLock_t *impl;
    struct timespec     ts;

    impl = MUTEX_TABLE[idtype];

    if (impl != NULL)
    {
        /*
         * because pthread_cond_timedwait() is also a cancellation point,
         * this pushes a cleanup handler to ensure that if canceled during this call,
         * the mutex will be released.
         */
        pthread_cleanup_push(OS_Posix_ReleaseTableMutex, &impl->mutex);

        clock_gettime(CLOCK_REALTIME, &ts);

        if (attempts <= 10)
        {
            /* Wait an increasing amount of time, starting at 10ms */
            ts.tv_nsec += attempts * attempts * 10000000;
            if (ts.tv_nsec >= 1000000000)
            {
                ts.tv_nsec -= 1000000000;
                ++ts.tv_sec;
            }
        }
        else
        {
            /* wait 1 second (max for polling) */
            ++ts.tv_sec;
        }

        pthread_cond_timedwait(&impl->cond, &impl->mutex, &ts);

        pthread_cleanup_pop(false);
    }
}

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_TableMutex_Init

   Purpose: Initialize the mutex that the OS API uses for the shared state tables

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_Posix_TableMutex_Init(osal_objtype_t idtype)
{
    int                 ret;
    int32               return_code = OS_SUCCESS;
    pthread_mutexattr_t mutex_attr;
    POSIX_GlobalLock_t *impl;

    do
    {
        if (idtype >= MUTEX_TABLE_SIZE)
        {
            break;
        }

        impl = MUTEX_TABLE[idtype];

        /* Initialize the table mutex for the given idtype */
        if (impl == NULL)
        {
            break;
        }

        /*
         * initialize the pthread mutex attribute structure with default values
         */
        ret = pthread_mutexattr_init(&mutex_attr);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_init failed: %s\n", strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        /*
         * Allow the mutex to use priority inheritance
         */
        ret = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n", strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        /*
         * Use normal (faster/non-recursive) mutex implementation
         * There should not be any instances of OSAL locking its own table more than once.
         */
        ret = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_settype failed: %s\n", strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        ret = pthread_mutex_init(&impl->mutex, &mutex_attr);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_mutex_init failed: %s\n", strerror(ret));
            return_code = OS_ERROR;
            break;
        }

        /* create a condition variable with default attributes.
         * This will be broadcast every time the object table changes */
        ret = pthread_cond_init(&impl->cond, NULL);
        if (ret != 0)
        {
            OS_DEBUG("Error: pthread_cond_init failed: %s\n", strerror(ret));
            return_code = OS_ERROR;
            break;
        }

    } while (0);

    return (return_code);
} /* end OS_Posix_TableMutex_Init */
