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
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-file.h" /* OSAL public API for this subsystem */
#include "os-shared-file.h"
#include "utstubs.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GenericRead_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GenericRead_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    void * buffer = UT_Hook_GetArgValueByName(Context, "buffer", void *);
    size_t nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    int32  status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        status = UT_Stub_CopyToLocal(UT_KEY(OS_GenericRead_Impl), buffer, nbytes);
        UT_Stub_SetReturnValue(FuncKey, status);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GenericWrite_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GenericWrite_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const void *buffer = UT_Hook_GetArgValueByName(Context, "buffer", const void *);
    size_t      nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    int32       status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        status = UT_Stub_CopyFromLocal(UT_KEY(OS_GenericWrite_Impl), buffer, nbytes);
        UT_Stub_SetReturnValue(FuncKey, status);
    }
}
