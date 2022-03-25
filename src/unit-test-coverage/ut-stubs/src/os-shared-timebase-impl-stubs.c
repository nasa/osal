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
 * Auto-Generated stub implementations for functions defined in os-shared-timebase header
 */

#include "os-shared-timebase.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseCreate_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseCreate_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseCreate_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseDelete_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseGetInfo_Impl(const OS_object_token_t *token, OS_timebase_prop_t *timer_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TimeBaseGetInfo_Impl, OS_timebase_prop_t *, timer_prop);

    UT_GenStub_Execute(OS_TimeBaseGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseLock_Impl()
 * ----------------------------------------------------
 */
void OS_TimeBaseLock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_TimeBaseLock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseLock_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseSet_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseSet_Impl(const OS_object_token_t *token, uint32 start_time, uint32 interval_time)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseSet_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, uint32, start_time);
    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, uint32, interval_time);

    UT_GenStub_Execute(OS_TimeBaseSet_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseSet_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseUnlock_Impl()
 * ----------------------------------------------------
 */
void OS_TimeBaseUnlock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_TimeBaseUnlock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseUnlock_Impl, Basic, NULL);
}
