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
 *
 * NOTE: The Object ID manipulation calls would not be called by applications.
 * However stubs are still defined in order to support things such as
 * coverage testing of the low-level implementation.  This set of stubs
 * is implemented separately here as it is only needed when coverage testing
 * OSAL itself (not for coverage testing other units).
 */

#include "osapi-idmap.h" /* OSAL public API for this subsystem */
#include "os-shared-idmap.h"
#include "utstubs.h"

#define OSAL_MAX_VALID_PER_TYPE 16

/*
 * Note that fabricates IDs that are compatible with the IDs from the other (public) API stubs
 */
static void UT_ObjIdCompose(uint32 indx, osal_objtype_t objtype, osal_id_t *id)
{
    *id = OS_ObjectIdFromInteger((unsigned long)indx | ((0x4000UL | objtype) << 16));
}

/*
 * UT Helper function to create a fake object lock token
 */
static void UT_TokenCompose(uint32 lock_mode, uint32 indx, osal_objtype_t objtype, OS_object_token_t *token)
{
    token->lock_mode = lock_mode;
    token->obj_type  = objtype;
    token->obj_idx   = indx;
    UT_ObjIdCompose(indx, objtype, &token->obj_id);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetMaxForObjectType' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetMaxForObjectType(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_objtype_t idtype = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    int32          status;
    uint32         max;

    if (UT_Stub_GetInt32StatusCode(Context, &status))
    {
        /* interpret the registered status code as a max */
        max = status;
    }
    else if (idtype > OS_OBJECT_TYPE_UNDEFINED && idtype < OS_OBJECT_TYPE_USER)
    {
        max = OSAL_MAX_VALID_PER_TYPE;
    }
    else
    {
        max = 0;
    }

    UT_Stub_SetReturnValue(FuncKey, max);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetBaseForObjectType' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetBaseForObjectType(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_objtype_t idtype = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    int32          status;
    uint32         base;

    if (UT_Stub_GetInt32StatusCode(Context, &status))
    {
        /* interpret the registered status code as a max */
        base = status;
    }
    else if (idtype > OS_OBJECT_TYPE_UNDEFINED && idtype < OS_OBJECT_TYPE_USER)
    {
        base = OSAL_MAX_VALID_PER_TYPE * (idtype - 1);
    }
    else
    {
        base = 0;
    }

    UT_Stub_SetReturnValue(FuncKey, base);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdGlobalFromToken' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdGlobalFromToken(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    static OS_common_record_t fake_record;
    int32                     status;
    OS_common_record_t *      recptr = &fake_record;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGlobalFromToken), &recptr, sizeof(recptr)) < sizeof(recptr))
    {
        /* This function should never return null */
        recptr = &fake_record;
    }

    UT_Stub_SetReturnValue(FuncKey, recptr);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdFinalizeNew' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdFinalizeNew(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    int32              operation_status = UT_Hook_GetArgValueByName(Context, "operation_status", int32);
    OS_object_token_t *token            = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    osal_id_t *        outid            = UT_Hook_GetArgValueByName(Context, "outid", osal_id_t *);
    int32              Status;

    if (!UT_Stub_GetInt32StatusCode(Context, &Status))
    {
        /* pass through the argument status unless overridden */
        Status = operation_status;
    }

    /* need to actually write something to the output buffer */
    if (Status == OS_SUCCESS && token != NULL && outid != NULL &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFinalizeNew), outid, sizeof(*outid)) < sizeof(*outid))
    {
        *outid = token->obj_id;
    }

    UT_Stub_SetReturnValue(FuncKey, Status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdFinalizeDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdFinalizeDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    int32 operation_status = UT_Hook_GetArgValueByName(Context, "operation_status", int32);
    int32 Status;

    if (!UT_Stub_GetInt32StatusCode(Context, &Status))
    {
        /* pass through the argument status unless overridden */
        Status = operation_status;
    }

    UT_Stub_SetReturnValue(FuncKey, Status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdGetBySearch' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdGetBySearch(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_lock_mode_t     lock_mode = UT_Hook_GetArgValueByName(Context, "lock_mode", OS_lock_mode_t);
    osal_objtype_t     idtype    = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    OS_object_token_t *token     = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    int32              Status;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetBySearch), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, 1, idtype, token);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdTransactionInit' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdTransactionInit(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_object_token_t *token = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    int32              Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdTransactionInit), token, sizeof(*token)) < sizeof(*token))
    {
        memset(&token, 0, sizeof(token));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdFindByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdFindByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_objtype_t idtype    = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    osal_id_t *    object_id = UT_Hook_GetArgValueByName(Context, "object_id", osal_id_t *);
    int32          Status;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    if (!UT_Stub_GetInt32StatusCode(Context, &Status))
    {
        Status = OS_ERR_NAME_NOT_FOUND;
    }

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFindByName), object_id, sizeof(*object_id)) < sizeof(*object_id))
    {
        UT_ObjIdCompose(1, idtype, object_id);
    }

    UT_Stub_SetReturnValue(FuncKey, Status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdGetByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdGetByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_lock_mode_t     lock_mode = UT_Hook_GetArgValueByName(Context, "lock_mode", OS_lock_mode_t);
    osal_objtype_t     idtype    = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    OS_object_token_t *token     = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    int32              Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetByName), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, 1, idtype, token);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdGetById' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdGetById(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_lock_mode_t     lock_mode = UT_Hook_GetArgValueByName(Context, "lock_mode", OS_lock_mode_t);
    osal_objtype_t     idtype    = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    osal_id_t          id        = UT_Hook_GetArgValueByName(Context, "id", osal_id_t);
    OS_object_token_t *token     = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    int32              Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS && UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetById), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, OS_ObjectIdToInteger(id) & 0xFFFF, idtype, token);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdTransferToken' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdTransferToken(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_object_token_t *token_from = UT_Hook_GetArgValueByName(Context, "token_from", OS_object_token_t *);
    OS_object_token_t *token_to   = UT_Hook_GetArgValueByName(Context, "token_to", OS_object_token_t *);
    int32              Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdTransferToken), token_to, sizeof(*token_to)) < sizeof(*token_to))
    {
        /* just copy it if nothing specified */
        *token_to = *token_from;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdAllocateNew' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdAllocateNew(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_objtype_t     idtype = UT_Hook_GetArgValueByName(Context, "idtype", osal_objtype_t);
    OS_object_token_t *token  = UT_Hook_GetArgValueByName(Context, "token", OS_object_token_t *);
    int32              Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdAllocateNew), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(OS_LOCK_MODE_GLOBAL, UT_GetStubCount(UT_KEY(OS_ObjectIdAllocateNew)), idtype, token);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdIteratorInit' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdIteratorInit(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_object_iter_t *iter = UT_Hook_GetArgValueByName(Context, "iter", OS_object_iter_t *);

    if (!UT_Stub_GetInt32StatusCode(Context, NULL) &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIteratorGetNext), iter, sizeof(*iter)) < sizeof(*iter))
    {
        memset(iter, 0, sizeof(*iter));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdIterateActive' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdIterateActive(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_object_iter_t *iter = UT_Hook_GetArgValueByName(Context, "iter", OS_object_iter_t *);

    if (!UT_Stub_GetInt32StatusCode(Context, NULL) &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIterateActive), iter, sizeof(*iter)) < sizeof(*iter))
    {
        memset(iter, 0, sizeof(*iter));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ObjectIdIteratorGetNext' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ObjectIdIteratorGetNext(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_object_iter_t *iter = UT_Hook_GetArgValueByName(Context, "iter", OS_object_iter_t *);
    bool              ReturnCode;
    int32             Status;

    if (UT_Stub_GetInt32StatusCode(Context, &Status))
    {
        ReturnCode = Status;
    }
    else
    {
        /* if test case has registered something, return true, otherwise return false */
        ReturnCode = (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIteratorGetNext), &iter->token, sizeof(iter->token)) ==
                      sizeof(iter->token));
    }
    UT_Stub_SetReturnValue(FuncKey, ReturnCode);
}
