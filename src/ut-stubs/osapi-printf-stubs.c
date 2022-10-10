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
 * Auto-Generated stub implementations for functions defined in osapi-printf header
 */

#include <stdarg.h>

#include "osapi-printf.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_printf(void *, UT_EntryKey_t, const UT_StubContext_t *, va_list);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf()
 * ----------------------------------------------------
 */
void OS_printf(const char *string, ...)
{
    va_list UtStub_ArgList;

    UT_GenStub_AddParam(OS_printf, const char *, string);

    va_start(UtStub_ArgList, string);
    UT_GenStub_Execute(OS_printf, Va, UT_DefaultHandler_OS_printf, UtStub_ArgList);
    va_end(UtStub_ArgList);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf_disable()
 * ----------------------------------------------------
 */
void OS_printf_disable(void)
{
    UT_GenStub_Execute(OS_printf_disable, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf_enable()
 * ----------------------------------------------------
 */
void OS_printf_enable(void)
{
    UT_GenStub_Execute(OS_printf_enable, Basic, NULL);
}
