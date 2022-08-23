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
 * Auto-Generated stub implementations for functions defined in os-shared-network header
 */

#include "os-shared-network.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_NetworkGetID_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetHostName_Impl()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetHostName_Impl(char *host_name, size_t name_len)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetHostName_Impl, int32);

    UT_GenStub_AddParam(OS_NetworkGetHostName_Impl, char *, host_name);
    UT_GenStub_AddParam(OS_NetworkGetHostName_Impl, size_t, name_len);

    UT_GenStub_Execute(OS_NetworkGetHostName_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_NetworkGetHostName_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetID_Impl()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetID_Impl(int32 *IdBuf)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetID_Impl, int32);

    UT_GenStub_AddParam(OS_NetworkGetID_Impl, int32 *, IdBuf);

    UT_GenStub_Execute(OS_NetworkGetID_Impl, Basic, UT_DefaultHandler_OS_NetworkGetID_Impl);

    return UT_GenStub_GetReturnValue(OS_NetworkGetID_Impl, int32);
}
