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

#include "osapi-binsem.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_BinSemCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_BinSemCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_BINSEM);
    }
    else
    {
        *sem_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_BinSemGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_BinSemGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_bin_sem_prop_t *bin_prop = UT_Hook_GetArgValueByName(Context, "bin_prop", OS_bin_sem_prop_t *);
    int32              status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetInfo), bin_prop, sizeof(*bin_prop)) < sizeof(*bin_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &bin_prop->creator);
        strncpy(bin_prop->name, "Name", sizeof(bin_prop->name) - 1);
        bin_prop->name[sizeof(bin_prop->name) - 1] = '\0';
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_BinSemDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_BinSemDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_BINSEM, sem_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_BinSemGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_BinSemGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_BINSEM, sem_id);
    }
}
