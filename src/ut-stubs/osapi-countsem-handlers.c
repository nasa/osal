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

#include "osapi-countsem.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_CountSemCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_CountSemCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_COUNTSEM);
    }
    else
    {
        *sem_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_CountSemDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_CountSemDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_COUNTSEM, sem_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_CountSemGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_CountSemGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_CountSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_COUNTSEM, sem_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_CountSemGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_CountSemGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_count_sem_prop_t *count_prop = UT_Hook_GetArgValueByName(Context, "count_prop", OS_count_sem_prop_t *);
    int32                status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_CountSemGetInfo), count_prop, sizeof(*count_prop)) < sizeof(*count_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &count_prop->creator);
        strncpy(count_prop->name, "Name", sizeof(count_prop->name) - 1);
        count_prop->name[sizeof(count_prop->name) - 1] = '\0';
    }
}
