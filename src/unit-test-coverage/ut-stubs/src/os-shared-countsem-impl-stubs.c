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
 * Auto-Generated stub implementations for functions defined in os-shared-countsem header
 */

#include "os-shared-countsem.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemCreate_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CountSemCreate_Impl, uint32, sem_initial_value);
    UT_GenStub_AddParam(OS_CountSemCreate_Impl, uint32, options);

    UT_GenStub_Execute(OS_CountSemCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemDelete_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CountSemDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemGetInfo_Impl(const OS_object_token_t *token, OS_count_sem_prop_t *count_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CountSemGetInfo_Impl, OS_count_sem_prop_t *, count_prop);

    UT_GenStub_Execute(OS_CountSemGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemGive_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemGive_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemGive_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemGive_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CountSemGive_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemGive_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemTake_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemTake_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemTake_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemTake_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_CountSemTake_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemTake_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemTimedWait_Impl()
 * ----------------------------------------------------
 */
int32 OS_CountSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemTimedWait_Impl, int32);

    UT_GenStub_AddParam(OS_CountSemTimedWait_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CountSemTimedWait_Impl, uint32, msecs);

    UT_GenStub_Execute(OS_CountSemTimedWait_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemTimedWait_Impl, int32);
}
