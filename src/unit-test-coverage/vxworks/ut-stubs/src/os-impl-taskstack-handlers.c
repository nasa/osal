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

#include "os-impl-taskstack.h"
#include "utstubs.h"

#include <string.h>

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_VxWorks_TaskAPI_AcquireStackBlock' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_VxWorks_TaskAPI_AcquireStackBlock(void *UserObj, UT_EntryKey_t FuncKey,
                                                            const UT_StubContext_t *Context)
{
    OS_impl_task_stack_mblock_t *bp = UT_Hook_GetArgValueByName(Context, "bp", OS_impl_task_stack_mblock_t *);
    int32                        status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == 0 && UT_Stub_CopyToLocal(UT_KEY(OS_VxWorks_TaskAPI_AcquireStackBlock), bp, sizeof(*bp)) < sizeof(*bp))
    {
        memset(bp, 0, sizeof(*bp));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_VxWorks_TaskAPI_StackBlockToAddr' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_VxWorks_TaskAPI_StackBlockToAddr(void *UserObj, UT_EntryKey_t FuncKey,
                                                           const UT_StubContext_t *Context)
{
    OS_impl_task_stack_addr_t *addr = UT_Hook_GetArgValueByName(Context, "addr", OS_impl_task_stack_addr_t *);
    int32                      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_VxWorks_TaskAPI_AcquireStackBlock), addr, sizeof(*addr)) < sizeof(*addr))
    {
        memset(addr, 0, sizeof(*addr));
    }
}
