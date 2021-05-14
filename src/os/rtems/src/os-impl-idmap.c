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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-idmap.h"
#include "os-impl-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_TABLE_MUTEX_ATTRIBS (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY)

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

static OS_impl_objtype_lock_t OS_task_table_lock;
static OS_impl_objtype_lock_t OS_queue_table_lock;
static OS_impl_objtype_lock_t OS_bin_sem_table_lock;
static OS_impl_objtype_lock_t OS_mutex_table_lock;
static OS_impl_objtype_lock_t OS_count_sem_table_lock;
static OS_impl_objtype_lock_t OS_stream_table_lock;
static OS_impl_objtype_lock_t OS_dir_table_lock;
static OS_impl_objtype_lock_t OS_timebase_table_lock;
static OS_impl_objtype_lock_t OS_timecb_table_lock;
static OS_impl_objtype_lock_t OS_module_table_lock;
static OS_impl_objtype_lock_t OS_filesys_table_lock;
static OS_impl_objtype_lock_t OS_console_lock;

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
    [OS_OBJECT_TYPE_OS_CONSOLE]  = &OS_console_lock,
};

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
    rtems_status_code       rtems_sc;

    impl = OS_impl_objtype_lock_table[idtype];

    rtems_sc = rtems_semaphore_obtain(impl->id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (rtems_sc != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("OS_Lock_Global_Impl: rtems_semaphore_obtain failed: %s\n", rtems_status_text(rtems_sc));
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
    rtems_status_code       rtems_sc;

    impl = OS_impl_objtype_lock_table[idtype];

    rtems_sc = rtems_semaphore_release(impl->id);
    if (rtems_sc != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("OS_Unlock_Global_Impl: rtems_semaphore_release failed: %s\n", rtems_status_text(rtems_sc));
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
    rtems_interval wait_ticks;

    if (attempts <= 10)
    {
        wait_ticks = attempts * attempts;
    }
    else
    {
        wait_ticks = 100;
    }

    OS_Unlock_Global_Impl(idtype);
    rtems_task_wake_after(wait_ticks);
    OS_Lock_Global_Impl(idtype);
}

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_Rtems_TableMutex_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_Rtems_TableMutex_Init(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;
    rtems_status_code       rtems_sc;

    impl = OS_impl_objtype_lock_table[idtype];
    if (impl == NULL)
    {
        return OS_SUCCESS;
    }

    /* Initialize the table mutex for the given idtype */
    rtems_sc = rtems_semaphore_create(idtype, 1, OSAL_TABLE_MUTEX_ATTRIBS, 0, &impl->id);
    if (rtems_sc != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Error: rtems_semaphore_create failed: %s\n", rtems_status_text(rtems_sc));
        return OS_ERROR;
    }

    return OS_SUCCESS;
} /* end OS_Rtems_TableMutex_Init */
