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
 * Auto-Generated stub implementations for functions defined in os-shared-console header
 */

#include "os-shared-console.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConsoleCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ConsoleCreate_Impl, int32);

    UT_GenStub_AddParam(OS_ConsoleCreate_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ConsoleCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ConsoleCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConsoleWakeup_Impl()
 * ----------------------------------------------------
 */
void OS_ConsoleWakeup_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_ConsoleWakeup_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ConsoleWakeup_Impl, Basic, NULL);
}
