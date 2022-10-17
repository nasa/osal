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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-task header
 */

#include "osapi-task.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_TaskCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TaskDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TaskFindIdBySystemData(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TaskGetId(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TaskGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TaskGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskCreate()
 * ----------------------------------------------------
 */
int32 OS_TaskCreate(osal_id_t *task_id, const char *task_name, osal_task_entry function_pointer,
                    osal_stackptr_t stack_pointer, size_t stack_size, osal_priority_t priority, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskCreate, int32);

    UT_GenStub_AddParam(OS_TaskCreate, osal_id_t *, task_id);
    UT_GenStub_AddParam(OS_TaskCreate, const char *, task_name);
    UT_GenStub_AddParam(OS_TaskCreate, osal_task_entry, function_pointer);
    UT_GenStub_AddParam(OS_TaskCreate, osal_stackptr_t, stack_pointer);
    UT_GenStub_AddParam(OS_TaskCreate, size_t, stack_size);
    UT_GenStub_AddParam(OS_TaskCreate, osal_priority_t, priority);
    UT_GenStub_AddParam(OS_TaskCreate, uint32, flags);

    UT_GenStub_Execute(OS_TaskCreate, Basic, UT_DefaultHandler_OS_TaskCreate);

    return UT_GenStub_GetReturnValue(OS_TaskCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskDelay()
 * ----------------------------------------------------
 */
int32 OS_TaskDelay(uint32 millisecond)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskDelay, int32);

    UT_GenStub_AddParam(OS_TaskDelay, uint32, millisecond);

    UT_GenStub_Execute(OS_TaskDelay, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskDelay, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskDelete()
 * ----------------------------------------------------
 */
int32 OS_TaskDelete(osal_id_t task_id)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskDelete, int32);

    UT_GenStub_AddParam(OS_TaskDelete, osal_id_t, task_id);

    UT_GenStub_Execute(OS_TaskDelete, Basic, UT_DefaultHandler_OS_TaskDelete);

    return UT_GenStub_GetReturnValue(OS_TaskDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskExit()
 * ----------------------------------------------------
 */
void OS_TaskExit(void)
{
    UT_GenStub_Execute(OS_TaskExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskFindIdBySystemData()
 * ----------------------------------------------------
 */
int32 OS_TaskFindIdBySystemData(osal_id_t *task_id, const void *sysdata, size_t sysdata_size)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskFindIdBySystemData, int32);

    UT_GenStub_AddParam(OS_TaskFindIdBySystemData, osal_id_t *, task_id);
    UT_GenStub_AddParam(OS_TaskFindIdBySystemData, const void *, sysdata);
    UT_GenStub_AddParam(OS_TaskFindIdBySystemData, size_t, sysdata_size);

    UT_GenStub_Execute(OS_TaskFindIdBySystemData, Basic, UT_DefaultHandler_OS_TaskFindIdBySystemData);

    return UT_GenStub_GetReturnValue(OS_TaskFindIdBySystemData, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskGetId()
 * ----------------------------------------------------
 */
osal_id_t OS_TaskGetId(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskGetId, osal_id_t);

    UT_GenStub_Execute(OS_TaskGetId, Basic, UT_DefaultHandler_OS_TaskGetId);

    return UT_GenStub_GetReturnValue(OS_TaskGetId, osal_id_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_TaskGetIdByName(osal_id_t *task_id, const char *task_name)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskGetIdByName, int32);

    UT_GenStub_AddParam(OS_TaskGetIdByName, osal_id_t *, task_id);
    UT_GenStub_AddParam(OS_TaskGetIdByName, const char *, task_name);

    UT_GenStub_Execute(OS_TaskGetIdByName, Basic, UT_DefaultHandler_OS_TaskGetIdByName);

    return UT_GenStub_GetReturnValue(OS_TaskGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskGetInfo()
 * ----------------------------------------------------
 */
int32 OS_TaskGetInfo(osal_id_t task_id, OS_task_prop_t *task_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskGetInfo, int32);

    UT_GenStub_AddParam(OS_TaskGetInfo, osal_id_t, task_id);
    UT_GenStub_AddParam(OS_TaskGetInfo, OS_task_prop_t *, task_prop);

    UT_GenStub_Execute(OS_TaskGetInfo, Basic, UT_DefaultHandler_OS_TaskGetInfo);

    return UT_GenStub_GetReturnValue(OS_TaskGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskInstallDeleteHandler()
 * ----------------------------------------------------
 */
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskInstallDeleteHandler, int32);

    UT_GenStub_AddParam(OS_TaskInstallDeleteHandler, osal_task_entry, function_pointer);

    UT_GenStub_Execute(OS_TaskInstallDeleteHandler, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskInstallDeleteHandler, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskSetPriority()
 * ----------------------------------------------------
 */
int32 OS_TaskSetPriority(osal_id_t task_id, osal_priority_t new_priority)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskSetPriority, int32);

    UT_GenStub_AddParam(OS_TaskSetPriority, osal_id_t, task_id);
    UT_GenStub_AddParam(OS_TaskSetPriority, osal_priority_t, new_priority);

    UT_GenStub_Execute(OS_TaskSetPriority, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskSetPriority, int32);
}
