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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-tasks.h"

#include "os-shared-task.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"
#include "osapi-bsp.h"

#include <errnoLib.h>
#include <taskLib.h>
#include <sysLib.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/*
 * macros for stack size manipulation.
 * These are normally provided by vxWorks.h if relevant for the platform.
 * If they are not defined, use a reasonable default/substitute.
 */
#if defined(_STACK_ALIGN_SIZE)
#define VX_IMPL_STACK_ALIGN_SIZE _STACK_ALIGN_SIZE
#else
#define VX_IMPL_STACK_ALIGN_SIZE ((size_t)16)
#endif

#if defined(STACK_ROUND_DOWN)
#define VX_IMPL_STACK_ROUND_DOWN(x) STACK_ROUND_DOWN(x)
#else
#define VX_IMPL_STACK_ROUND_DOWN(x) ((x) & ~(VX_IMPL_STACK_ALIGN_SIZE - 1))
#endif

#if defined(STACK_ROUND_UP)
#define VX_IMPL_STACK_ROUND_UP(x) STACK_ROUND_UP(x)
#else
#define VX_IMPL_STACK_ROUND_UP(x) (((x) + (VX_IMPL_STACK_ALIGN_SIZE - 1)) & ~(VX_IMPL_STACK_ALIGN_SIZE - 1))
#endif

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

/*---------------------------------------------------------------------------------------
   Name: OS_VxWorksEntry

   Purpose: A Simple VxWorks-compatible entry point that calls the common task entry function

   NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
int OS_VxWorks_TaskEntry(int arg)
{
    OS_TaskEntryPoint(OS_ObjectIdFromInteger(arg));

    return 0;
}

/****************************************************************************************
                                    TASK API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_TaskAPI_Impl_Init(void)
{
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    STATUS                          status;
    int                             vxflags;
    int                             vxpri;
    size_t                          actualsz;
    unsigned long                   userstackbase;
    unsigned long                   actualstackbase;
    OS_impl_task_internal_record_t *lrec;
    OS_task_internal_record_t *     task;

    lrec = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    task = OS_OBJECT_TABLE_GET(OS_task_table, *token);

    /* Create VxWorks Task */

    /* see if the user wants floating point enabled. If
     * so, then se the correct option.
     */
    vxflags = OS_BSP_GetResourceTypeConfig(OS_OBJECT_TYPE_OS_TASK);
    if (flags & OS_FP_ENABLED)
    {
        vxflags |= VX_FP_TASK;
    }

    /*
     * Get priority/stack specs from main struct
     * priority should be a direct passthru
     */
    vxpri         = task->priority;
    actualsz      = task->stack_size;
    userstackbase = (unsigned long)task->stack_pointer;

    /*
     * NOTE: Using taskInit() here rather than taskSpawn() allows us
     * to specify a specific statically-allocated WIND_TCB instance.
     *
     * This is very important as it allows for efficient reverse-lookup;
     * a call to taskTcb() will get the WIND_TCB pointer back, which
     * in turn provides an index into OSAL local data structures.  With
     * this we can have the equivalent of a taskVar that works on both
     * UMP and SMP deployments.
     *
     * The difficulty with taskInit() is that we must also manually
     * allocate the stack as well (there is no API that allows
     * a specific WIND_TCB but automatically allocates the stack).
     * Furthermore, VxWorks uses this pointer directly as the CPU
     * stack pointer register, so we need to manually adjust it for
     * downward-growing stacks.
     *
     * NOTE: Allocation of the stack requires a malloc() of some form.
     * This is what taskSpawn() effectively does internally to create
     * stack.  If the system malloc() is unacceptable here then this
     * could be replaced with a locally scoped statically allocated buffer.
     *
     * ALSO NOTE: The stack-rounding macros are normally supplied from
     * vxWorks.h on relevant platforms.  If not provided then it is
     * assumed that no specific alignment is needed on this platform.
     */

    if (userstackbase == 0)
    {
        /* add a little extra in case the base address needs alignment too.
         * this helps ensure that the final aligned stack is not less
         * than what was originally requested (but might be a bit more)  */
        actualsz += VX_IMPL_STACK_ALIGN_SIZE;
        actualsz = VX_IMPL_STACK_ROUND_UP(actualsz);

        /*
         * VxWorks does not provide a way to deallocate
         * a taskInit-provided stack when a task exits.
         *
         * So in this case we will find the leftover heap
         * buffer when OSAL reuses this local record block.
         *
         * If that leftover heap buffer is big enough it
         * can be used directly.  Otherwise it needs to be
         * re-created.
         */
        if (lrec->heap_block_size < actualsz)
        {
            if (lrec->heap_block != NULL)
            {
                /* release the old block */
                free(lrec->heap_block);
                lrec->heap_block_size = 0;
            }

            /* allocate a new heap block to use for a stack */
            lrec->heap_block = malloc(actualsz);

            if (lrec->heap_block != NULL)
            {
                lrec->heap_block_size = actualsz;
            }
        }

        userstackbase = (unsigned long)lrec->heap_block;
    }

    if (userstackbase == 0)
    {
        /* no stack - cannot create task */
        return OS_ERROR;
    }

    actualstackbase = userstackbase;

    /* also round the base address */
    actualstackbase = VX_IMPL_STACK_ROUND_UP(actualstackbase);
    actualsz -= (actualstackbase - userstackbase);
    actualsz = VX_IMPL_STACK_ROUND_DOWN(actualsz);

    /*
     * On most CPUs the stack grows downward, so assume that to be
     * the case in the event that _STACK_DIR is not defined/known
     */
#if !defined(_STACK_DIR) || (_STACK_DIR != _STACK_GROWS_UP)
    actualstackbase += actualsz; /* move to last byte of stack block */
#endif

    status = taskInit((WIND_TCB *)&lrec->tcb,                            /* address of new task's TCB */
                      (char *)task->task_name, vxpri,                    /* priority of new task */
                      vxflags,                                           /* task option word */
                      (char *)actualstackbase,                           /* base of new task's stack */
                      actualsz,                                          /* size (bytes) of stack needed */
                      (FUNCPTR)OS_VxWorks_TaskEntry,                     /* entry point of new task */
                      OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), /* 1st arg is ID */
                      0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (status != OK)
    {
        return OS_ERROR;
    }

    lrec->vxid = (TASK_ID)&lrec->tcb;

    taskActivate(lrec->vxid);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
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
    if (taskDelete(impl->vxid) != OK)
    {
        OS_DEBUG("taskDelete() - vxWorks errno %d\n", errno);
        return OS_ERROR;
    }

    impl->vxid = 0;
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    /* No-op on VxWorks */
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    taskExit(0);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelay_Impl(uint32 milli_second)
{
    /* msecs rounded to the closest system tick count */
    int sys_ticks;

    /* Convert to ticks if possible */
    if (OS_Milli2Ticks(milli_second, &sys_ticks) != OS_SUCCESS)
    {
        return OS_ERROR;
    }

    /* if successful, the execution of task will pend here until delay finishes */
    if (taskDelay(sys_ticks) != OK)
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
int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /* Set VxWorks Task Priority */
    if (taskPrioritySet(impl->vxid, new_priority) != OK)
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
int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /*
    ** Get VxWorks Task Id
    */
    if (taskIdSelf() != impl->vxid)
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
int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
osal_id_t OS_TaskGetId_Impl(void)
{
    void *    lrec;
    size_t    idx;
    osal_id_t id;

    id   = OS_OBJECT_ID_UNDEFINED;
    lrec = taskTcb(taskIdSelf());

    if (lrec != NULL)
    {
        idx = (OS_impl_task_internal_record_t *)lrec - &OS_impl_task_table[0];
        if (idx < OS_MAX_TASKS)
        {
            id = OS_global_task_table[idx].active_id;
        }
    }

    return id;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo_Impl(const OS_object_token_t *token, OS_task_prop_t *task_prop)
{
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, size_t sysdata_size)
{
    if (sysdata == NULL || sysdata_size != sizeof(TASK_ID))
    {
        return OS_INVALID_POINTER;
    }
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    const TASK_ID *                 target = (const TASK_ID *)ref;
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    return (*target == impl->vxid);
}
