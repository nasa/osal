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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         This code only uses very basic C library calls that are expected
 *         to be available on every sane C-language compiler.  For everything else,
 *         a platform-specific implementation function is used.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-task.h"
#include "os-shared-common.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_TASKS) || (OS_MAX_TASKS <= 0)
#error "osconfig.h must define OS_MAX_TASKS to a valid value"
#endif

enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_TASKS,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_TASK
};

OS_task_internal_record_t OS_task_table[LOCAL_NUM_OBJECTS];

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Helper function for registering new tasks in the global database.
 *           This maps the given task_id back to the array entry (OS_task_internal_record_t)
 *           so that the caller can call the real entry point.
 *
 *           In the process, this also verifies that the task_id is valid and
 *           it matches the expected entry, and this calls the implementation's
 *           "Register" function to make sure that the appropriate thread-specific
 *           variables are set - this guarantees that GetTaskId will work.
 *
 *
 *-----------------------------------------------------------------*/
static int32 OS_TaskPrepare(osal_id_t task_id, osal_task_entry *entrypt)
{
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TASK, task_id, &token);
    if (return_code == OS_SUCCESS)
    {
        task = OS_OBJECT_TABLE_GET(OS_task_table, token);

        return_code = OS_TaskMatch_Impl(&token);
        *entrypt    = task->entry_function_pointer;

        OS_ObjectIdRelease(&token);
    }

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_TaskRegister_Impl(task_id);
    }

    if (return_code == OS_SUCCESS)
    {
        /* Give event callback to the application */
        return_code = OS_NotifyEvent(OS_EVENT_TASK_STARTUP, task_id, NULL);
    }

    if (return_code != OS_SUCCESS)
    {
        *entrypt = NULL;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           The entry point for all OSAL tasks
 *           This function is called from the OS-specific layers after a task is spawned
 *           and is the first thing to run under the context of the task itself.
 *           This will register the task appropriately in the global data structures and
 *           call the user's intended entry point function.
 *
 *-----------------------------------------------------------------*/
void OS_TaskEntryPoint(osal_id_t task_id)
{
    osal_task_entry task_entry;

    if (OS_TaskPrepare(task_id, &task_entry) == OS_SUCCESS)
    {
        if (task_entry != NULL)
        {
            (*task_entry)();
        }
    }

    /* If the function returns, treat as a normal exit and do the proper cleanup */
    OS_TaskExit();
}

/*
 *********************************************************************************
 *          TASK API
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskAPI_Init(void)
{
    memset(OS_task_table, 0, sizeof(OS_task_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskCreate(osal_id_t *task_id, const char *task_name, osal_task_entry function_pointer,
                    osal_stackptr_t stack_pointer, size_t stack_size, osal_priority_t priority, uint32 flags)
{
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;

    /*
     * Check parameters
     *
     * Note "stack_pointer" is not checked, because in certain configurations it can be validly null.
     */
    OS_CHECK_POINTER(task_id);
    OS_CHECK_POINTER(function_pointer);
    OS_CHECK_APINAME(task_name);
    OS_CHECK_SIZE(stack_size);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, task_name, &token);
    if (return_code == OS_SUCCESS)
    {
        task = OS_OBJECT_TABLE_GET(OS_task_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, task, task_name, task_name);

        task->stack_size             = stack_size;
        task->priority               = priority;
        task->entry_function_pointer = function_pointer;
        task->stack_pointer          = stack_pointer;

        /* Add default flags */
        flags |= OS_ADD_TASK_FLAGS;

        /* Now call the OS-specific implementation.  This reads info from the task table. */
        return_code = OS_TaskCreate_Impl(&token, flags);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, task_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelete(osal_id_t task_id)
{
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;
    osal_task_entry            delete_hook;

    delete_hook = NULL;
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, task_id, &token);
    if (return_code == OS_SUCCESS)
    {
        task = OS_OBJECT_TABLE_GET(OS_task_table, token);

        /* Save the delete hook, as we do not want to call it while locked */
        delete_hook = task->delete_hook_pointer;

        return_code = OS_TaskDelete_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    /*
    ** Call the thread Delete hook if there is one.
    */
    if (return_code == OS_SUCCESS && delete_hook != NULL)
    {
        delete_hook();
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit()
{
    osal_id_t         task_id;
    OS_object_token_t token;

    task_id = OS_TaskGetId_Impl();
    if (OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &token) == OS_SUCCESS)
    {
        OS_TaskDetach_Impl(&token);

        /* Complete the operation via the common routine */
        OS_ObjectIdFinalizeDelete(OS_SUCCESS, &token);
    }

    /* call the implementation */
    OS_TaskExit_Impl();

    /* Impl function never returns */
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelay(uint32 millisecond)
{
    /* just call the implementation */
    return OS_TaskDelay_Impl(millisecond);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskSetPriority(osal_id_t task_id, osal_priority_t new_priority)
{
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &token);
    if (return_code == OS_SUCCESS)
    {
        task = OS_OBJECT_TABLE_GET(OS_task_table, token);

        return_code = OS_TaskSetPriority_Impl(&token, new_priority);

        if (return_code == OS_SUCCESS)
        {
            /* Use the abstracted priority, not the OS one */
            /* Change the priority in the table as well */
            task->priority = new_priority;
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
osal_id_t OS_TaskGetId(void)
{
    osal_id_t task_id;

    task_id = OS_TaskGetId_Impl();

    return task_id;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetIdByName(osal_id_t *task_id, const char *task_name)
{
    int32 return_code;

    /* Check parameters */
    OS_CHECK_POINTER(task_id);
    OS_CHECK_POINTER(task_name);

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, task_name, task_id);

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo(osal_id_t task_id, OS_task_prop_t *task_prop)
{
    OS_common_record_t *       record;
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;

    /* Check parameters */
    OS_CHECK_POINTER(task_prop);

    memset(task_prop, 0, sizeof(OS_task_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_task_table, token);
        task   = OS_OBJECT_TABLE_GET(OS_task_table, token);

        if (record->name_entry != NULL)
        {
            strncpy(task_prop->name, record->name_entry, sizeof(task_prop->name) - 1);
            task_prop->name[sizeof(task_prop->name) - 1] = 0;
        }
        task_prop->creator    = record->creator;
        task_prop->stack_size = task->stack_size;
        task_prop->priority   = task->priority;

        return_code = OS_TaskGetInfo_Impl(&token, task_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
{
    int32                      return_code;
    OS_object_token_t          token;
    OS_task_internal_record_t *task;
    osal_id_t                  task_id;

    task_id     = OS_TaskGetId_Impl();
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, task_id, &token);
    if (return_code == OS_SUCCESS)
    {
        task = OS_OBJECT_TABLE_GET(OS_task_table, token);

        /*
        ** Install the pointer
        */
        task->delete_hook_pointer = function_pointer;

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskFindIdBySystemData(osal_id_t *task_id, const void *sysdata, size_t sysdata_size)
{
    int32             return_code;
    OS_object_token_t token;

    /* Check parameters */
    OS_CHECK_POINTER(task_id);

    /* The "sysdata" and "sysdata_size" must be passed to the underlying impl for validation */
    return_code = OS_TaskValidateSystemData_Impl(sysdata, sysdata_size);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, OS_TaskIdMatchSystemData_Impl,
                                         (void *)sysdata, &token);
    if (return_code == OS_SUCCESS)
    {
        *task_id = OS_ObjectIdFromToken(&token);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}
