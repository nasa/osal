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
 * Auto-Generated stub implementations for functions defined in os-shared-condvar header
 */

#include "os-shared-condvar.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_CondVarAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarAPI_Init, int32);

    UT_GenStub_Execute(OS_CondVarAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarAPI_Init, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarBroadcast_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarBroadcast_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarBroadcast_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarBroadcast_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarBroadcast_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarBroadcast_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarCreate_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CondVarCreate_Impl, uint32, options);

    UT_GenStub_Execute(OS_CondVarCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarDelete_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarGetInfo_Impl(const OS_object_token_t *token, OS_condvar_prop_t *condvar_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CondVarGetInfo_Impl, OS_condvar_prop_t *, condvar_prop);

    UT_GenStub_Execute(OS_CondVarGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarLock_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarLock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarLock_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarLock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarLock_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarLock_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarSignal_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarSignal_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarSignal_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarSignal_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarSignal_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarSignal_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarTimedWait_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarTimedWait_Impl(const OS_object_token_t *token, const OS_time_t *abs_wakeup_time)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarTimedWait_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarTimedWait_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CondVarTimedWait_Impl, const OS_time_t *, abs_wakeup_time);

    UT_GenStub_Execute(OS_CondVarTimedWait_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarTimedWait_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarUnlock_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarUnlock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarUnlock_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarUnlock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarUnlock_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarUnlock_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarWait_Impl()
 * ----------------------------------------------------
 */
int32 OS_CondVarWait_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarWait_Impl, int32);

    UT_GenStub_AddParam(OS_CondVarWait_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CondVarWait_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarWait_Impl, int32);
}
