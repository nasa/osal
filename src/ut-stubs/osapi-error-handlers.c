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

#include "osapi-error.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetErrorName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetErrorName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    int32          error_num = UT_Hook_GetArgValueByName(Context, "error_num", int32);
    os_err_name_t *err_name  = UT_Hook_GetArgValueByName(Context, "err_name", os_err_name_t *);
    int32          status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && UT_Stub_CopyToLocal(UT_KEY(OS_GetErrorName), *err_name, sizeof(*err_name)) == 0)
    {
        snprintf(*err_name, sizeof(*err_name), "ut%d", (int)error_num);
    }
}
