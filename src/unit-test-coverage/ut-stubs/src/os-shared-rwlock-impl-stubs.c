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
 * Auto-Generated stub implementations for functions defined in os-shared-rwlock header
 */

#include "os-shared-rwlock.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockCreate_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_RwLockCreate_Impl, uint32, options);

    UT_GenStub_Execute(OS_RwLockCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockDelete_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_RwLockDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockGetInfo_Impl(const OS_object_token_t *token, OS_rwlock_prop_t *rw_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_RwLockGetInfo_Impl, OS_rwlock_prop_t *, rw_prop);

    UT_GenStub_Execute(OS_RwLockGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockReadGive_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockReadGive_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockReadGive_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockReadGive_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_RwLockReadGive_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockReadGive_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockReadTake_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockReadTake_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockReadTake_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockReadTake_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_RwLockReadTake_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockReadTake_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockWriteGive_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockWriteGive_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockWriteGive_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockWriteGive_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_RwLockWriteGive_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockWriteGive_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockWriteTake_Impl()
 * ----------------------------------------------------
 */
int32 OS_RwLockWriteTake_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockWriteTake_Impl, int32);

    UT_GenStub_AddParam(OS_RwLockWriteTake_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_RwLockWriteTake_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockWriteTake_Impl, int32);
}
