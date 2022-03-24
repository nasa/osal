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
 * Auto-Generated stub implementations for functions defined in os-shared-module header
 */

#include "os-shared-module.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleLoad_Static()
 * ----------------------------------------------------
 */
int32 OS_ModuleLoad_Static(const char *ModuleName)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleLoad_Static, int32);

    UT_GenStub_AddParam(OS_ModuleLoad_Static, const char *, ModuleName);

    UT_GenStub_Execute(OS_ModuleLoad_Static, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleLoad_Static, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolLookup_Static()
 * ----------------------------------------------------
 */
int32 OS_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolLookup_Static, int32);

    UT_GenStub_AddParam(OS_SymbolLookup_Static, cpuaddr *, SymbolAddress);
    UT_GenStub_AddParam(OS_SymbolLookup_Static, const char *, SymbolName);
    UT_GenStub_AddParam(OS_SymbolLookup_Static, const char *, ModuleName);

    UT_GenStub_Execute(OS_SymbolLookup_Static, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SymbolLookup_Static, int32);
}
