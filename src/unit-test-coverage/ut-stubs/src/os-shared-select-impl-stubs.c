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
 * Auto-Generated stub implementations for functions defined in os-shared-select header
 */

#include "os-shared-select.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectMultiple_Impl()
 * ----------------------------------------------------
 */
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectMultiple_Impl, int32);

    UT_GenStub_AddParam(OS_SelectMultiple_Impl, OS_FdSet *, ReadSet);
    UT_GenStub_AddParam(OS_SelectMultiple_Impl, OS_FdSet *, WriteSet);
    UT_GenStub_AddParam(OS_SelectMultiple_Impl, int32, msecs);

    UT_GenStub_Execute(OS_SelectMultiple_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectMultiple_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectSingle_Impl()
 * ----------------------------------------------------
 */
int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectSingle_Impl, int32);

    UT_GenStub_AddParam(OS_SelectSingle_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SelectSingle_Impl, uint32 *, SelectFlags);
    UT_GenStub_AddParam(OS_SelectSingle_Impl, int32, msecs);

    UT_GenStub_Execute(OS_SelectSingle_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectSingle_Impl, int32);
}
