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
 * \file
 *
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-clock.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetLocalTime' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetLocalTime(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_time_t *time_struct = UT_Hook_GetArgValueByName(Context, "time_struct", OS_time_t *);
    uint32     count       = UT_GetStubCount(FuncKey);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_GetLocalTime), time_struct, sizeof(*time_struct)) < sizeof(*time_struct))
    {
        *time_struct = OS_TimeAssembleFromNanoseconds(1 + (count / 100), 10000000 * (count % 100));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SetLocalTime' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SetLocalTime(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const OS_time_t *time_struct = UT_Hook_GetArgValueByName(Context, "time_struct", const OS_time_t *);
    int32            status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_Stub_CopyFromLocal(UT_KEY(OS_SetLocalTime), time_struct, sizeof(*time_struct));
    }
} /*end OS_SetLocalTime */
