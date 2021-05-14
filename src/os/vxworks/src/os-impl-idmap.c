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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-idmap.h"
#include "os-shared-idmap.h"

#include <taskLib.h>
#include <errnoLib.h>
#include <objLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

VX_MUTEX_SEMAPHORE(OS_task_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_queue_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_bin_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_mutex_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_count_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_stream_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_dir_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_timebase_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_timecb_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_module_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_filesys_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_console_table_mut_mem);

static OS_impl_objtype_lock_t OS_task_table_lock      = {.mem = OS_task_table_mut_mem};
static OS_impl_objtype_lock_t OS_queue_table_lock     = {.mem = OS_queue_table_mut_mem};
static OS_impl_objtype_lock_t OS_bin_sem_table_lock   = {.mem = OS_bin_sem_table_mut_mem};
static OS_impl_objtype_lock_t OS_mutex_table_lock     = {.mem = OS_mutex_table_mut_mem};
static OS_impl_objtype_lock_t OS_count_sem_table_lock = {.mem = OS_count_sem_table_mut_mem};
static OS_impl_objtype_lock_t OS_stream_table_lock    = {.mem = OS_stream_table_mut_mem};
static OS_impl_objtype_lock_t OS_dir_table_lock       = {.mem = OS_dir_table_mut_mem};
static OS_impl_objtype_lock_t OS_timebase_table_lock  = {.mem = OS_timebase_table_mut_mem};
static OS_impl_objtype_lock_t OS_timecb_table_lock    = {.mem = OS_timecb_table_mut_mem};
static OS_impl_objtype_lock_t OS_module_table_lock    = {.mem = OS_module_table_mut_mem};
static OS_impl_objtype_lock_t OS_filesys_table_lock   = {.mem = OS_filesys_table_mut_mem};
static OS_impl_objtype_lock_t OS_console_table_lock   = {.mem = OS_console_table_mut_mem};

OS_impl_objtype_lock_t *const OS_impl_objtype_lock_table[OS_OBJECT_TYPE_USER] = {
    [OS_OBJECT_TYPE_UNDEFINED]   = NULL,
    [OS_OBJECT_TYPE_OS_TASK]     = &OS_task_table_lock,
    [OS_OBJECT_TYPE_OS_QUEUE]    = &OS_queue_table_lock,
    [OS_OBJECT_TYPE_OS_COUNTSEM] = &OS_count_sem_table_lock,
    [OS_OBJECT_TYPE_OS_BINSEM]   = &OS_bin_sem_table_lock,
    [OS_OBJECT_TYPE_OS_MUTEX]    = &OS_mutex_table_lock,
    [OS_OBJECT_TYPE_OS_STREAM]   = &OS_stream_table_lock,
    [OS_OBJECT_TYPE_OS_DIR]      = &OS_dir_table_lock,
    [OS_OBJECT_TYPE_OS_TIMEBASE] = &OS_timebase_table_lock,
    [OS_OBJECT_TYPE_OS_TIMECB]   = &OS_timecb_table_lock,
    [OS_OBJECT_TYPE_OS_MODULE]   = &OS_module_table_lock,
    [OS_OBJECT_TYPE_OS_FILESYS]  = &OS_filesys_table_lock,
    [OS_OBJECT_TYPE_OS_CONSOLE]  = &OS_console_table_lock};

/*----------------------------------------------------------------
 *
 * Function: OS_Lock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;

    impl = OS_impl_objtype_lock_table[idtype];

    if (semTake(impl->vxid, WAIT_FOREVER) != OK)
    {
        OS_DEBUG("semTake() - vxWorks errno %d\n", errno);
    }

} /* end OS_Lock_Global_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_Unlock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;

    impl = OS_impl_objtype_lock_table[idtype];

    if (semGive(impl->vxid) != OK)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n", errno);
    }

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
    int wait_ticks;

    if (attempts <= 10)
    {
        wait_ticks = attempts * attempts;
    }
    else
    {
        wait_ticks = 100;
    }

    OS_Unlock_Global_Impl(idtype);
    taskDelay(wait_ticks);
    OS_Lock_Global_Impl(idtype);
}

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_TableMutex_Init
 *
 *  Purpose: Initialize the tables that the OS API uses to keep track of information
 *           about objects
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_TableMutex_Init(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;
    SEM_ID                  semid;

    impl = OS_impl_objtype_lock_table[idtype];
    if (impl == NULL)
    {
        return OS_SUCCESS;
    }

    /* Initialize the table mutex for the given idtype */
    semid = semMInitialize(impl->mem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    if (semid == (SEM_ID)0)
    {
        OS_DEBUG("Error: semMInitialize() failed - vxWorks errno %d\n", errno);
        return OS_ERROR;
    }

    impl->vxid = semid;

    return OS_SUCCESS;

} /* end OS_VxWorks_TableMutex_Init */
