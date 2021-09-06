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
 * \file     os-impl-console.c
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "os-impl-console.h"
extern "C" {
#include "os-shared-idmap.h"
#include "os-shared-printf.h"
#include "os-shared-common.h"
}

#include <QSemaphore>
/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC         true
#define OS_CONSOLE_TASK_PRIORITY OS_UTILITYTASK_PRIORITY

typedef struct
{
    bool  is_async;
    QSemaphore data_sem;
} OS_impl_console_internal_record_t;

/* Tables where the OS object information is stored */
OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/

extern "C" {


/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleWakeup_Impl
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
    local->data_sem.release();

} /* end OS_ConsoleWakeup_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleTask_Entry
 *
 *  Purpose: Local Helper function
 *           Implements the console output task
 *
 *-----------------------------------------------------------------*/
static void *OS_ConsoleTask_Entry(void *arg)
{
    OS_VoidPtrValueWrapper_t           local_arg;
    OS_impl_console_internal_record_t *local;
    OS_object_token_t                  token;

    local_arg.opaque_arg = arg;
    if (OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_CONSOLE, local_arg.id, &token) == OS_SUCCESS)
    {
        local = OS_OBJECT_TABLE_GET(OS_impl_console_table, token);

        /* Loop forever (unless shutdown is set) */
        while (OS_SharedGlobalVars.GlobalState != OS_SHUTDOWN_MAGIC_NUMBER)
        {
            OS_ConsoleOutput_Impl(&token);
            local->data_sem.acquire();
        }
        OS_ObjectIdRelease(&token);
    }
    return NULL;
} /* end OS_ConsoleTask_Entry */

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;
    OS_console_internal_record_t *     console;
    OS_impl_task_internal_record_t     consoletask;
    int32                              return_code;
    OS_VoidPtrValueWrapper_t           local_arg = {0};

    console = OS_OBJECT_TABLE_GET(OS_console_table, *token);
    local   = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);

    if (token->obj_idx == 0)
    {
        return_code = OS_SUCCESS;

        if (console->IsAsync)
        {
            local_arg.id = OS_ObjectIdFromToken(token);
                return_code  = OS_QT_InternalTaskCreate_Impl(&consoletask, OS_CONSOLE_TASK_PRIORITY, 0,
                                                            OS_ConsoleTask_Entry, local_arg.opaque_arg);

        }
    }
    else
    {
        /* only one physical console device is implemented */
        return_code = OS_ERR_NOT_IMPLEMENTED;
    }

    return return_code;
} /* end OS_ConsoleCreate_Impl */

} /* End extern "C" */