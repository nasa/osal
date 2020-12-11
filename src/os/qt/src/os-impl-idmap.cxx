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
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "bsp-impl.h"
#include <sched.h>

#include "os-shared-idmap.h"

typedef struct
{
    QMutex *mutex;
    // sigset_t        sigmask;
} QT_GlobalLock_t;

static QT_GlobalLock_t OS_global_task_table_mut;
static QT_GlobalLock_t OS_queue_table_mut;
static QT_GlobalLock_t OS_bin_sem_table_mut;
static QT_GlobalLock_t OS_mutex_table_mut;
static QT_GlobalLock_t OS_count_sem_table_mut;
static QT_GlobalLock_t OS_stream_table_mut;
static QT_GlobalLock_t OS_dir_table_mut;
static QT_GlobalLock_t OS_timebase_table_mut;
static QT_GlobalLock_t OS_timecb_table_mut;
static QT_GlobalLock_t OS_module_table_mut;
static QT_GlobalLock_t OS_filesys_table_mut;
static QT_GlobalLock_t OS_console_mut;

static QT_GlobalLock_t *const MUTEX_TABLE[] = {
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
    QT_GlobalLock_t *mut;
    sigset_t            previous;

    mut = MUTEX_TABLE[idtype];

    if (mut == NULL)
    {
        return OS_ERROR;
    }
    /* TODO */
    // if (pthread_sigmask(SIG_SETMASK, &QT_GlobalVars.MaximumSigMask, &previous) != 0)
    // {
    //     return OS_ERROR;
    // }

    mut->mutex->lock();


    /* Only set values inside the GlobalLock _after_ it is locked */
    /* TODO */
//    mut->sigmask = previous;

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
    QT_GlobalLock_t *mut;
    sigset_t            previous;

    if (idtype < MUTEX_TABLE_SIZE)
    {
        mut = MUTEX_TABLE[idtype];
    }
    else
    {
        mut = NULL;
    }

    if (mut == NULL)
    {
        return OS_ERROR;
    }

    /* Only get values inside the GlobalLock _before_ it is unlocked */
    /* TODO */
//    previous = mut->sigmask;
    mut->mutex->unlock();

    /*TODO */
    // pthread_sigmask(SIG_SETMASK, &previous, NULL);

    return OS_SUCCESS;
} /* end OS_Unlock_Global_Impl */

/*---------------------------------------------------------------------------------------
   Name: OS_QT_TableMutex_Init

   Purpose: Initialize the mutex that the OS API uses for the shared state tables

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_QT_TableMutex_Init(osal_objtype_t idtype)
{
    int                 ret;
    int32               return_code = OS_SUCCESS;

    do
    {
        if (idtype >= MUTEX_TABLE_SIZE)
        {
            break;
        }

        /* Initialize the table mutex for the given idtype */
        if (MUTEX_TABLE[idtype] == NULL)
        {
            break;
        }


        /*
         ** Allow the mutex to use priority inheritance
         */
        // ret = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
        // if (ret != 0)
        // {
        //     OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n", strerror(ret));
        //     return_code = OS_ERROR;
        //     break;
        // }

        /*
         **  Set the mutex type to RECURSIVE so a thread can do nested locks
         **  TBD - not sure if this is really desired, but keep it for now.
         */
        // ret = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        // if (ret != 0)
        // {
        //     OS_DEBUG("Error: pthread_mutexattr_settype failed: %s\n", strerror(ret));
        //     return_code = OS_ERROR;
        //     break;
        // }
        MUTEX_TABLE[idtype]->mutex = new QMutex();
        if (MUTEX_TABLE[idtype]->mutex == 0)
        {
            OS_DEBUG("Error: pthread_mutex_init failed: %s\n", "new qmutex failed");
            return_code = OS_ERROR;
            break;
        }
    } while (0);

    return (return_code);
} /* end OS_QT_TableMutex_Init */
