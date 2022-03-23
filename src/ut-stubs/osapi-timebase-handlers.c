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

#include "osapi-timebase.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimeBaseCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimeBaseCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timebase_id = UT_Hook_GetArgValueByName(Context, "timebase_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *timebase_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMEBASE);
    }
    else
    {
        *timebase_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimeBaseDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimeBaseDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t timebase_id = UT_Hook_GetArgValueByName(Context, "timebase_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimeBaseGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimeBaseGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timebase_id = UT_Hook_GetArgValueByName(Context, "timebase_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetIdByName), timebase_id, sizeof(*timebase_id)) < sizeof(*timebase_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimeBaseGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimeBaseGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_timebase_prop_t *timebase_prop = UT_Hook_GetArgValueByName(Context, "timebase_prop", OS_timebase_prop_t *);
    int32               status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetInfo), timebase_prop, sizeof(*timebase_prop)) < sizeof(*timebase_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &timebase_prop->creator);
        strncpy(timebase_prop->name, "Name", sizeof(timebase_prop->name) - 1);
        timebase_prop->name[sizeof(timebase_prop->name) - 1] = '\0';
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimeBaseGetFreeRun' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimeBaseGetFreeRun(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint32 *freerun_val = UT_Hook_GetArgValueByName(Context, "freerun_val", uint32 *);
    int32   status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetFreeRun), freerun_val, sizeof(*freerun_val)) < sizeof(*freerun_val))
    {
        /*
         * Use the call count such that the value increases with each successive call.
         */
        *freerun_val = UT_GetStubCount(FuncKey);
    }
}
