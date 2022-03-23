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

#include "osapi-timer.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerAdd' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerAdd(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_TIMECB, timer_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetIdByName), timer_id, sizeof(*timer_id)) < sizeof(*timer_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TIMECB, timer_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_timer_prop_t *timer_prop = UT_Hook_GetArgValueByName(Context, "timer_prop", OS_timer_prop_t *);
    int32            status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetInfo), timer_prop, sizeof(*timer_prop)) < sizeof(*timer_prop))
    {
        memset(timer_prop, 0, sizeof(*timer_prop));
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &timer_prop->creator);
    }
}
