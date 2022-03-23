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
 *
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-task.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *task_id = UT_Hook_GetArgValueByName(Context, "task_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *task_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TASK);
    }
    else
    {
        *task_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t task_id = UT_Hook_GetArgValueByName(Context, "task_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_TASK, task_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskGetId' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskGetId(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t TaskId;
    int32     status;

    /* Unless set otherwise this returns a task ID that correlates to table position 1.
     * This is for historical reasons, many old test cases assume this is the default */
    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        status = 1;
    }
    UT_ObjIdCompose(status, OS_OBJECT_TYPE_OS_TASK, &TaskId);

    UT_Stub_SetReturnValue(FuncKey, TaskId);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *task_id = UT_Hook_GetArgValueByName(Context, "task_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TaskGetIdByName), task_id, sizeof(*task_id)) < sizeof(*task_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, task_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_task_prop_t *task_prop = UT_Hook_GetArgValueByName(Context, "task_prop", OS_task_prop_t *);
    int32           status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TaskGetInfo), task_prop, sizeof(*task_prop)) < sizeof(*task_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &task_prop->creator);
        task_prop->stack_size = OSAL_SIZE_C(100);
        task_prop->priority   = OSAL_PRIORITY_C(150);
        strncpy(task_prop->name, "UnitTest", sizeof(task_prop->name) - 1);
        task_prop->name[sizeof(task_prop->name) - 1] = '\0';
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TaskFindIdBySystemData' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TaskFindIdBySystemData(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *task_id = UT_Hook_GetArgValueByName(Context, "task_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TaskFindIdBySystemData), task_id, sizeof(*task_id)) < sizeof(*task_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, task_id);
    }
}
