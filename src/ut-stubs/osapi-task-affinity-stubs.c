/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * Auto-Generated stub implementations for functions defined in osapi-task-affinity header
 */

#include "osapi-task-affinity.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_TaskAffinityGetAffinity(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskAffinityCoresConfigured()
 * ----------------------------------------------------
 */
uint32 OS_TaskAffinityCoresConfigured(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskAffinityCoresConfigured, uint32);

    UT_GenStub_Execute(OS_TaskAffinityCoresConfigured, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskAffinityCoresConfigured, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskAffinityGetAffinity()
 * ----------------------------------------------------
 */
int32 OS_TaskAffinityGetAffinity(osal_id_t task_id, OS_cpuset_t *cpuset)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskAffinityGetAffinity, int32);

    UT_GenStub_AddParam(OS_TaskAffinityGetAffinity, osal_id_t, task_id);
    UT_GenStub_AddParam(OS_TaskAffinityGetAffinity, OS_cpuset_t *, cpuset);

    UT_GenStub_Execute(OS_TaskAffinityGetAffinity, Basic, UT_DefaultHandler_OS_TaskAffinityGetAffinity);

    return UT_GenStub_GetReturnValue(OS_TaskAffinityGetAffinity, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskAffinityGetCoresConfigured()
 * ----------------------------------------------------
 */
uint32 OS_TaskAffinityGetCoresConfigured(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskAffinityGetCoresConfigured, uint32);

    UT_GenStub_Execute(OS_TaskAffinityGetCoresConfigured, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskAffinityGetCoresConfigured, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskAffinitySetAffinity()
 * ----------------------------------------------------
 */
int32 OS_TaskAffinitySetAffinity(osal_id_t task_id, const OS_cpuset_t cpuset)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskAffinitySetAffinity, int32);

    UT_GenStub_AddParam(OS_TaskAffinitySetAffinity, osal_id_t, task_id);
    UT_GenStub_AddParam(OS_TaskAffinitySetAffinity, const OS_cpuset_t, cpuset);

    UT_GenStub_Execute(OS_TaskAffinitySetAffinity, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskAffinitySetAffinity, int32);
}
