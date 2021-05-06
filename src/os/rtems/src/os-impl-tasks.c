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
 * \file     os-impl-tasks.c
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
#include "os-impl-tasks.h"

#include "os-shared-task.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"

#include "osapi-printf.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

/*---------------------------------------------------------------------------------------
   Name: OS_RtemsEntry

   Purpose: A Simple RTEMS-compatible entry point that calls the common task entry function

   NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
static rtems_task OS_RtemsEntry(rtems_task_argument arg)
{
    OS_TaskEntryPoint(OS_ObjectIdFromInteger(arg));
} /* end OS_RtemsEntry */

/****************************************************************************************
                                    TASK API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_TaskAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_TaskAPI_Impl_Init(void)
{
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));
    return (OS_SUCCESS);
} /* end OS_Rtems_TaskAPI_Impl_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    rtems_status_code               status;
    rtems_name                      r_name;
    rtems_mode                      r_mode;
    rtems_attribute                 r_attributes;
    OS_impl_task_internal_record_t *impl;
    OS_task_internal_record_t *     task;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    task = OS_OBJECT_TABLE_GET(OS_task_table, *token);

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL task ID in here, as we know it is already unique
    ** and trying to use the real task name would be less than useful (only 4 chars)
    */
    r_name = OS_ObjectIdToInteger(OS_ObjectIdFromToken(token));
    r_mode = RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0);

    /*
    ** see if the user wants floating point enabled. If
    ** so, then set the correct option.
    */
    r_attributes = RTEMS_LOCAL;
    if (flags & OS_FP_ENABLED)
    {
        r_attributes |= RTEMS_FLOATING_POINT;
    }

    status = rtems_task_create(r_name, task->priority, task->stack_size, r_mode, r_attributes, &impl->id);

    /* check if task_create failed */
    if (status != RTEMS_SUCCESSFUL)
    {
        /* Provide some freedback as to why this failed */
        OS_printf("rtems_task_create failed: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

    /* will place the task in 'ready for scheduling' state */
    status = rtems_task_start(impl->id,                        /*rtems task id*/
                              (rtems_task_entry)OS_RtemsEntry, /* task entry point */
                              (rtems_task_argument)r_name);    /* passed argument  */

    if (status != RTEMS_SUCCESSFUL)
    {
        OS_printf("rtems_task_start failed: %s\n", rtems_status_text(status));
        rtems_task_delete(impl->id);
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_TaskCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /*
    ** Try to delete the task
    ** If this fails, not much recourse - the only potential cause of failure
    ** to cancel here is that the thread ID is invalid because it already exited itself,
    ** and if that is true there is nothing wrong - everything is OK to continue normally.
    */

    rtems_task_delete(impl->id);
    return OS_SUCCESS;
} /* end OS_TaskDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskDetach_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    /* No-op on RTEMS */
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 * Function: OS_TaskExit_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    rtems_task_delete(RTEMS_SELF);

} /* end OS_TaskExit_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelay_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelay_Impl(uint32 milli_second)
{
    int   tick_count;
    int32 return_code;

    return_code = OS_Milli2Ticks(milli_second, &tick_count);

    if (return_code != OS_SUCCESS)
    {
        /*
         * always want to do some form of delay, because if
         * this function becomes a no-op then this might create a
         * tight loop that doesn't ever yield the CPU - effectively
         * locking the system in an RTOS environment.
         */
        tick_count = 10;
    }

    /*
    ** Always successful ( from RTEMS docs )
    */
    rtems_task_wake_after((rtems_interval)tick_count);

    return (return_code);

} /* end OS_TaskDelay_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskSetPriority_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    rtems_task_priority             old_pri;
    rtems_status_code               status;
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /* Set RTEMS Task Priority */
    status = rtems_task_set_priority(impl->id, new_priority, &old_pri);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled task_set_priority error: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_TaskSetPriority_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskMatch_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /*
    ** Get RTEMS Task Id
    */
    if (rtems_task_self() != impl->id)
    {
        return (OS_ERROR);
    }

    return OS_SUCCESS;
} /* end OS_TaskMatch_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskRegister_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    /*
     * This is supposed to maintain the "reverse lookup" information used
     * to map an RTEMS task ID back into an OSAL ID.
     *
     * Originally this used "task variables" which got deprecated.
     * So this changed to "task notes" which are also now deprecated in 4.11.
     *
     * So there is now no documented per-task thread local storage facility in RTEMS
     * with these two options gone.  RTEMS does seem to have TLS, but there is just
     * no published (non-deprecated) API to access it.
     *
     * Right now this does nothing and the OS_TaskGetId() must brute-force it.
     *
     * An alternative for performance improvements might be to use a locally maintained
     * hash table here.
     */
    return OS_SUCCESS;

} /* end OS_TaskRegister_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetId_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
osal_id_t OS_TaskGetId_Impl(void)
{
    osal_id_t         global_task_id;
    rtems_id          task_self;
    rtems_name        self_name;
    rtems_status_code status;

    task_self = rtems_task_self();
    /* When the task was created the OSAL ID was used as the "classic name",
     * which gives us an easy way to map it back again */
    status = rtems_object_get_classic_name(task_self, &self_name);
    if (status == RTEMS_SUCCESSFUL)
    {
        global_task_id = OS_ObjectIdFromInteger(self_name);
    }
    else
    {
        global_task_id = OS_OBJECT_ID_UNDEFINED;
    }

    return global_task_id;

} /* end OS_TaskGetId_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo_Impl(const OS_object_token_t *token, OS_task_prop_t *task_prop)
{
    return OS_SUCCESS;

} /* end OS_TaskGetInfo_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskValidateSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, size_t sysdata_size)
{
    if (sysdata == NULL || sysdata_size != sizeof(rtems_id))
    {
        return OS_INVALID_POINTER;
    }
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 * Function: OS_TaskIdMatchSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    const rtems_id *                target = (const rtems_id *)ref;
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    return (*target == impl->id);
}
