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

#include "osapi-queue.h" /* OSAL public API for this subsystem */
#include "osapi-idmap.h"
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueueCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueueCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *queue_id = UT_Hook_GetArgValueByName(Context, "queue_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *queue_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_QUEUE);
    }
    else
    {
        *queue_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueueDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueueDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t queue_id = UT_Hook_GetArgValueByName(Context, "queue_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_QUEUE, queue_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueueGet' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueueGet(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t queue_id    = UT_Hook_GetArgValueByName(Context, "queue_id", osal_id_t);
    void *    data        = UT_Hook_GetArgValueByName(Context, "data", void *);
    size_t    size        = UT_Hook_GetArgValueByName(Context, "size", size_t);
    size_t *  size_copied = UT_Hook_GetArgValueByName(Context, "size_copied", size_t *);
    int32     status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        *size_copied = UT_Stub_CopyToLocal((UT_EntryKey_t)OS_ObjectIdToInteger(queue_id), data, size);
        if (*size_copied == 0)
        {
            status = OS_QUEUE_EMPTY;
        }

        UT_Stub_SetReturnValue(FuncKey, status);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueuePut' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueuePut(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t   queue_id = UT_Hook_GetArgValueByName(Context, "queue_id", osal_id_t);
    const void *data     = UT_Hook_GetArgValueByName(Context, "data", const void *);
    size_t      size     = UT_Hook_GetArgValueByName(Context, "size", size_t);
    int32       status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_SetDataBuffer((UT_EntryKey_t)OS_ObjectIdToInteger(queue_id), (void *)data, size, true);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueueGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueueGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *queue_id = UT_Hook_GetArgValueByName(Context, "queue_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetIdByName), queue_id, sizeof(*queue_id)) < sizeof(*queue_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_QUEUE, queue_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_QueueGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_QueueGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_queue_prop_t *queue_prop = UT_Hook_GetArgValueByName(Context, "queue_prop", OS_queue_prop_t *);
    int32            status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetInfo), queue_prop, sizeof(*queue_prop)) < sizeof(*queue_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &queue_prop->creator);
        strncpy(queue_prop->name, "Name", sizeof(queue_prop->name) - 1);
        queue_prop->name[sizeof(queue_prop->name) - 1] = '\0';
    }
}
