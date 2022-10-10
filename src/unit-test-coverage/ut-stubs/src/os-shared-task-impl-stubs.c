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
 * Auto-Generated stub implementations for functions defined in os-shared-task header
 */

#include "os-shared-task.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskCreate_Impl, int32);

    UT_GenStub_AddParam(OS_TaskCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TaskCreate_Impl, uint32, flags);

    UT_GenStub_Execute(OS_TaskCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskDelay_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskDelay_Impl(uint32 millisecond)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskDelay_Impl, int32);

    UT_GenStub_AddParam(OS_TaskDelay_Impl, uint32, millisecond);

    UT_GenStub_Execute(OS_TaskDelay_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskDelay_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskDelete_Impl, int32);

    UT_GenStub_AddParam(OS_TaskDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TaskDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskDetach_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskDetach_Impl, int32);

    UT_GenStub_AddParam(OS_TaskDetach_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TaskDetach_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskDetach_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskExit_Impl()
 * ----------------------------------------------------
 */
void OS_TaskExit_Impl(void)
{
    UT_GenStub_Execute(OS_TaskExit_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskGetId_Impl()
 * ----------------------------------------------------
 */
osal_id_t OS_TaskGetId_Impl(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskGetId_Impl, osal_id_t);

    UT_GenStub_Execute(OS_TaskGetId_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskGetId_Impl, osal_id_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskGetInfo_Impl(const OS_object_token_t *token, OS_task_prop_t *task_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_TaskGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TaskGetInfo_Impl, OS_task_prop_t *, task_prop);

    UT_GenStub_Execute(OS_TaskGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskIdMatchSystemData_Impl()
 * ----------------------------------------------------
 */
bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskIdMatchSystemData_Impl, bool);

    UT_GenStub_AddParam(OS_TaskIdMatchSystemData_Impl, void *, ref);
    UT_GenStub_AddParam(OS_TaskIdMatchSystemData_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TaskIdMatchSystemData_Impl, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_TaskIdMatchSystemData_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskIdMatchSystemData_Impl, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskMatch_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskMatch_Impl, int32);

    UT_GenStub_AddParam(OS_TaskMatch_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TaskMatch_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskMatch_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskRegister_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskRegister_Impl, int32);

    UT_GenStub_AddParam(OS_TaskRegister_Impl, osal_id_t, global_task_id);

    UT_GenStub_Execute(OS_TaskRegister_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskRegister_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskSetPriority_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskSetPriority_Impl, int32);

    UT_GenStub_AddParam(OS_TaskSetPriority_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TaskSetPriority_Impl, osal_priority_t, new_priority);

    UT_GenStub_Execute(OS_TaskSetPriority_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskSetPriority_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskValidateSystemData_Impl()
 * ----------------------------------------------------
 */
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, size_t sysdata_size)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskValidateSystemData_Impl, int32);

    UT_GenStub_AddParam(OS_TaskValidateSystemData_Impl, const void *, sysdata);
    UT_GenStub_AddParam(OS_TaskValidateSystemData_Impl, size_t, sysdata_size);

    UT_GenStub_Execute(OS_TaskValidateSystemData_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskValidateSystemData_Impl, int32);
}
