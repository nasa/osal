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
 * Auto-Generated stub implementations for functions defined in osapi-version header
 */

#include "osapi-version.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetVersionCodeName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_GetVersionString(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetBuildNumber()
 * ----------------------------------------------------
 */
uint32 OS_GetBuildNumber(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetBuildNumber, uint32);

    UT_GenStub_Execute(OS_GetBuildNumber, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_GetBuildNumber, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionCodeName()
 * ----------------------------------------------------
 */
const char *OS_GetVersionCodeName(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetVersionCodeName, const char *);

    UT_GenStub_Execute(OS_GetVersionCodeName, Basic, UT_DefaultHandler_OS_GetVersionCodeName);

    return UT_GenStub_GetReturnValue(OS_GetVersionCodeName, const char *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionNumber()
 * ----------------------------------------------------
 */
void OS_GetVersionNumber(uint8 VersionNumbers[4])
{
    UT_GenStub_Execute(OS_GetVersionNumber, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionString()
 * ----------------------------------------------------
 */
const char *OS_GetVersionString(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetVersionString, const char *);

    UT_GenStub_Execute(OS_GetVersionString, Basic, UT_DefaultHandler_OS_GetVersionString);

    return UT_GenStub_GetReturnValue(OS_GetVersionString, const char *);
}
