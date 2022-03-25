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
 * Auto-Generated stub implementations for functions defined in osapi-clock header
 */

#include "osapi-clock.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetLocalTime(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SetLocalTime(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetLocalTime()
 * ----------------------------------------------------
 */
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_GetLocalTime, int32);

    UT_GenStub_AddParam(OS_GetLocalTime, OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_GetLocalTime, Basic, UT_DefaultHandler_OS_GetLocalTime);

    return UT_GenStub_GetReturnValue(OS_GetLocalTime, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SetLocalTime()
 * ----------------------------------------------------
 */
int32 OS_SetLocalTime(const OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_SetLocalTime, int32);

    UT_GenStub_AddParam(OS_SetLocalTime, const OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_SetLocalTime, Basic, UT_DefaultHandler_OS_SetLocalTime);

    return UT_GenStub_GetReturnValue(OS_SetLocalTime, int32);
}
