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
#include "os-impl-console.h"

#include "os-shared-printf.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC          true
#define OS_CONSOLE_TASK_PRIORITY  OS_UTILITYTASK_PRIORITY
#define OS_CONSOLE_TASK_STACKSIZE OS_UTILITYTASK_STACK_SIZE

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ConsoleWakeup_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;

    local = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);

    /* post the sem for the utility task to run */
    if (semGive(local->datasem) == ERROR)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n", errno);
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int OS_VxWorks_ConsoleTask_Entry(int arg)
{
    OS_impl_console_internal_record_t *local;
    OS_object_token_t                  token;

    if (OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_CONSOLE, OS_ObjectIdFromInteger(arg), &token) ==
        OS_SUCCESS)
    {
        local = OS_OBJECT_TABLE_GET(OS_impl_console_table, token);

        /* Loop forever (unless shutdown is set) */
        while (OS_SharedGlobalVars.GlobalState != OS_SHUTDOWN_MAGIC_NUMBER)
        {
            OS_ConsoleOutput_Impl(&token);
            if (semTake(local->datasem, WAIT_FOREVER) == ERROR)
            {
                OS_DEBUG("semTake() - vxWorks errno %d\n", errno);
                break;
            }
        }
        OS_ObjectIdRelease(&token);
    }

    /* Return OK since called from taskSpawn, error is reported in debug message */
    return OK;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;
    int32                              return_code;
    OS_console_internal_record_t *     console;

    local   = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);
    console = OS_OBJECT_TABLE_GET(OS_console_table, *token);

    if (OS_ObjectIndexFromToken(token) == 0)
    {
        return_code = OS_SUCCESS;

        if (console->IsAsync)
        {
            OS_DEBUG("%s(): Starting Async Console Handler\n", __func__);

            /* Initialize VxWorks Semaphore.
             * The memory for this sem is statically allocated. */
            local->datasem = semCInitialize(local->cmem, SEM_Q_PRIORITY, 0);

            /* check if semCInitialize failed */
            if (local->datasem == (SEM_ID)0)
            {
                OS_DEBUG("semCInitialize() - vxWorks errno %d\n", errno);
                return OS_SEM_FAILURE;
            }

            /* spawn the async output helper task */
            local->taskid = taskSpawn(console->device_name, OS_CONSOLE_TASK_PRIORITY, 0, OS_CONSOLE_TASK_STACKSIZE,
                                      (FUNCPTR)OS_VxWorks_ConsoleTask_Entry,
                                      OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), 0, 0, 0, 0, 0, 0, 0, 0, 0);

            if (local->taskid == (TASK_ID)ERROR)
            {
                OS_DEBUG("taskSpawn() - vxWorks errno %d\n", errno);
                return_code = OS_ERROR;
            }
        }
    }
    else
    {
        /* only one physical console device is implemented */
        return_code = OS_ERR_NOT_IMPLEMENTED;
    }

    return return_code;
}
