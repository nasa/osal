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
 * Auto-Generated stub implementations for functions defined in os-shared-mutex header
 */

#include "os-shared-mutex.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemCreate_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_MutSemCreate_Impl, uint32, options);

    UT_GenStub_Execute(OS_MutSemCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemDelete_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_MutSemGetInfo_Impl, OS_mut_sem_prop_t *, mut_prop);

    UT_GenStub_Execute(OS_MutSemGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGive_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemGive_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGive_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemGive_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemGive_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemGive_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemTake_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemTake_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemTake_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemTake_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemTake_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemTake_Impl, int32);
}
