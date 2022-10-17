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
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 *
 * NOTE: The Object ID manipulation calls would not be called by applications.
 * However stubs are still defined in order to support things such as
 * coverage testing of the low-level implementation.  This set of stubs
 * is implemented separately here as it is only needed when coverage testing
 * OSAL itself (not for coverage testing other units).
 */

#include "osapi-idmap.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdToArrayIndex' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdToArrayIndex(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t      object_id  = UT_Hook_GetArgValueByName(Context, "object_id", osal_id_t);
    osal_index_t * ArrayIndex = UT_Hook_GetArgValueByName(Context, "ArrayIndex", osal_index_t *);
    int32          Status;
    osal_objtype_t checktype;
    uint32         tempserial;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdToArrayIndex), ArrayIndex, sizeof(*ArrayIndex)) < sizeof(*ArrayIndex))
    {
        /* this needs to output something valid or code will break */
        UT_ObjIdDecompose(object_id, &tempserial, &checktype);
        *ArrayIndex = OSAL_INDEX_C(tempserial);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetResourceName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetResourceName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    char * buffer      = UT_Hook_GetArgValueByName(Context, "buffer", char *);
    size_t buffer_size = UT_Hook_GetArgValueByName(Context, "buffer_size", size_t);
    int32  status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        if (buffer_size > 0 && UT_Stub_CopyToLocal(UT_KEY(OS_GetResourceName), buffer, buffer_size) == 0)
        {
            /* return an empty string by default */
            buffer[0] = 0;
        }
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ConvertToArrayIndex' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ConvertToArrayIndex(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t      object_id  = UT_Hook_GetArgValueByName(Context, "object_id", osal_id_t);
    osal_index_t * ArrayIndex = UT_Hook_GetArgValueByName(Context, "ArrayIndex", osal_index_t *);
    osal_objtype_t ObjType;
    int32          status;
    uint32         tempserial;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_ObjIdDecompose(object_id, &tempserial, &ObjType);
        if (ObjType != OS_OBJECT_TYPE_UNDEFINED && ObjType < OS_OBJECT_TYPE_USER)
        {
            tempserial %= UT_MAXOBJS[ObjType];
        }
    }
    else
    {
        /*
         * If set to fail, then set the output to something bizarre - if the code
         * actually tries to use this, chances are it will segfault and be fixed
         */
        tempserial = 0xDEADBEEFU;
    }

    *ArrayIndex = OSAL_INDEX_C(tempserial);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ForEachObjectOfType' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ForEachObjectOfType(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_ArgCallback_t callback_ptr = UT_Hook_GetArgValueByName(Context, "callback_ptr", OS_ArgCallback_t);
    void *           callback_arg = UT_Hook_GetArgValueByName(Context, "callback_arg", void *);
    osal_id_t        NextId;
    size_t           IdSize;

    while (1)
    {
        IdSize = UT_Stub_CopyToLocal(UT_KEY(OS_ForEachObjectOfType), &NextId, sizeof(NextId));
        if (IdSize < sizeof(NextId))
        {
            break;
        }
        (*callback_ptr)(NextId, callback_arg);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ForEachObject' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ForEachObject(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_ArgCallback_t callback_ptr = UT_Hook_GetArgValueByName(Context, "callback_ptr", OS_ArgCallback_t);
    void *           callback_arg = UT_Hook_GetArgValueByName(Context, "callback_arg", void *);
    osal_id_t        NextId;
    size_t           IdSize;

    while (1)
    {
        IdSize = UT_Stub_CopyToLocal((UT_EntryKey_t)&OS_ForEachObject, &NextId, sizeof(NextId));
        if (IdSize < sizeof(NextId))
        {
            break;
        }
        (*callback_ptr)(NextId, callback_arg);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_IdentifyObject' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_IdentifyObject(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t      object_id = UT_Hook_GetArgValueByName(Context, "object_id", osal_id_t);
    osal_objtype_t ObjType;
    uint32         checkindx;
    int32          status;

    if (UT_Stub_GetInt32StatusCode(Context, &status))
    {
        /* Use the "status code" as the object type if it was set */
        ObjType = status;
    }
    else
    {
        /* output a type that will actually match the ID */
        UT_ObjIdDecompose(object_id, &checkindx, &ObjType);
    }

    UT_Stub_SetReturnValue(FuncKey, ObjType);
}
