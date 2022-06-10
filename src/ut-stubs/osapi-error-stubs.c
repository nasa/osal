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
 * Auto-Generated stub implementations for functions defined in osapi-error header
 */

#include "osapi-error.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetErrorName(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetErrorName()
 * ----------------------------------------------------
 */
int32 OS_GetErrorName(int32 error_num, os_err_name_t *err_name)
{
    UT_GenStub_SetupReturnBuffer(OS_GetErrorName, int32);

    UT_GenStub_AddParam(OS_GetErrorName, int32, error_num);
    UT_GenStub_AddParam(OS_GetErrorName, os_err_name_t *, err_name);

    UT_GenStub_Execute(OS_GetErrorName, Basic, UT_DefaultHandler_OS_GetErrorName);

    return UT_GenStub_GetReturnValue(OS_GetErrorName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_StatusToString()
 * ----------------------------------------------------
 */
char *OS_StatusToString(osal_status_t status, os_status_string_t *status_string)
{
    UT_GenStub_SetupReturnBuffer(OS_StatusToString, char *);

    UT_GenStub_AddParam(OS_StatusToString, osal_status_t, status);
    UT_GenStub_AddParam(OS_StatusToString, os_status_string_t *, status_string);

    UT_GenStub_Execute(OS_StatusToString, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_StatusToString, char *);
}
