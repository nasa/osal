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
 * Auto-Generated stub implementations for functions defined in os-shared-globaldefs header
 */

#include <stdarg.h>

#include "os-shared-globaldefs.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DebugPrintf()
 * ----------------------------------------------------
 */
void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...)
{
    va_list UtStub_ArgList;

    UT_GenStub_AddParam(OS_DebugPrintf, uint32, Level);
    UT_GenStub_AddParam(OS_DebugPrintf, const char *, Func);
    UT_GenStub_AddParam(OS_DebugPrintf, uint32, Line);
    UT_GenStub_AddParam(OS_DebugPrintf, const char *, Format);

    va_start(UtStub_ArgList, Format);
    UT_GenStub_Execute(OS_DebugPrintf, Va, NULL, UtStub_ArgList);
    va_end(UtStub_ArgList);
}
