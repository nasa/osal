/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     osapi-utstub-idmap.c
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

#include "utstub-helpers.h"
#include "os-shared-idmap.h"

/*
 * UT Helper function to create a fake object lock token
 */
static void UT_TokenCompose(uint32 lock_mode, uint32 indx, UT_ObjType_t objtype, OS_object_token_t *token)
{
    token->lock_mode = lock_mode;
    token->obj_type  = objtype;
    token->obj_idx   = indx;
    UT_ObjIdCompose(indx, objtype, &token->obj_id);
}

UT_DEFAULT_STUB(OS_ObjectIdInit, (void))

/* Lock/Unlock for global tables */
void OS_Lock_Global(osal_objtype_t idtype)
{
    UT_DEFAULT_IMPL(OS_Lock_Global);
}
void OS_Unlock_Global(osal_objtype_t idtype)
{
    UT_DEFAULT_IMPL(OS_Unlock_Global);
}

/*****************************************************************************
 *
 * Stub function for OS_GetMaxForObjectType()
 *
 *****************************************************************************/
uint32 OS_GetMaxForObjectType(osal_objtype_t idtype)
{
    int32 max;

    if (idtype > UT_OBJTYPE_NONE && idtype < UT_OBJTYPE_MAX)
    {
        max = OSAL_MAX_VALID_PER_TYPE;
    }
    else
    {
        max = 0;
    }

    return UT_DEFAULT_IMPL_RC(OS_GetMaxForObjectType, max);
}

/*****************************************************************************
 *
 * Stub function for OS_GetBaseForObjectType()
 *
 *****************************************************************************/
uint32 OS_GetBaseForObjectType(osal_objtype_t idtype)
{
    int32 base;

    if (idtype > UT_OBJTYPE_NONE && idtype < UT_OBJTYPE_MAX)
    {
        base = OSAL_MAX_VALID_PER_TYPE * (idtype - 1);
    }
    else
    {
        base = 0;
    }

    return UT_DEFAULT_IMPL_RC(OS_GetBaseForObjectType, base);
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdToArrayIndex()
 *
 *****************************************************************************/
int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t id, osal_index_t *ArrayIndex)
{
    int32        Status;
    UT_ObjType_t checktype;
    uint32       tempserial;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdToArrayIndex);

    if (Status == 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdToArrayIndex), ArrayIndex, sizeof(*ArrayIndex)) < sizeof(*ArrayIndex))
    {
        /* this needs to output something valid or code will break */
        UT_ObjIdDecompose(id, &tempserial, &checktype);
        *ArrayIndex = OSAL_INDEX_C(tempserial);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGlobalFromToken()
 *
 *****************************************************************************/
OS_common_record_t *OS_ObjectIdGlobalFromToken(const OS_object_token_t *token)
{
    static OS_common_record_t fake_record;
    int32                     status;
    OS_common_record_t *      recptr;

    status = UT_DEFAULT_IMPL(OS_ObjectIdGlobalFromToken);
    if (status == 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGlobalFromToken), &recptr, sizeof(recptr)) < sizeof(recptr))
    {
        /* This function should never return null */
        recptr = &fake_record;
    }

    return recptr;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFinalize()
 *
 *****************************************************************************/
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_object_token_t *token, osal_id_t *outid)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdFinalizeNew, operation_status);

    /* need to actually write something to the output buffer */
    if (Status == OS_SUCCESS && token != NULL && outid != NULL &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFinalizeNew), outid, sizeof(*outid)) < sizeof(*outid))
    {
        *outid = token->obj_id;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFinalizeDelete()
 *
 *****************************************************************************/
int32 OS_ObjectIdFinalizeDelete(int32 operation_status, OS_object_token_t *token)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdFinalizeDelete, operation_status);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFindMatch()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg,
                             OS_object_token_t *token)
{
    int32 Status;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetBySearch);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetBySearch), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, 1, idtype, token);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFindByName()
 *
 *****************************************************************************/
int32 OS_ObjectIdFindByName(osal_objtype_t idtype, const char *name, osal_id_t *object_id)
{
    int32 Status;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdFindByName, OS_ERR_NAME_NOT_FOUND);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFindByName), object_id, sizeof(*object_id)) < sizeof(*object_id))
    {
        UT_ObjIdCompose(1, idtype, object_id);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetByName()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetByName(OS_lock_mode_t lock_mode, osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetByName);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetByName), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, 1, idtype, token);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetById()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, osal_objtype_t idtype, osal_id_t id, OS_object_token_t *token)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetById);

    if (Status == OS_SUCCESS && UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetById), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(lock_mode, OS_ObjectIdToInteger(id) & 0xFFFF, idtype, token);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdRelease()
 *
 *****************************************************************************/
void OS_ObjectIdRelease(OS_object_token_t *token)
{
    UT_DEFAULT_IMPL(OS_ObjectIdRelease);
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdTransferToken()
 *
 *****************************************************************************/
void OS_ObjectIdTransferToken(OS_object_token_t *token_from, OS_object_token_t *token_to)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdTransferToken);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdTransferToken), token_to, sizeof(*token_to)) < sizeof(*token_to))
    {
        /* just copy it if nothing specified */
        *token_to = *token_from;
    }
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetNext()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetNext(osal_objtype_t idtype, uint32 *curr_index, OS_common_record_t **record)
{
    int32                     Status;
    uint32                    local_id;
    OS_common_record_t *      local_record;
    static OS_common_record_t fake_record;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetNext);

    if (Status == 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetNext), &local_id, sizeof(local_id)) < sizeof(local_id))
        {
            local_id = UT_GetStubCount(UT_KEY(OS_ObjectIdGetNext));
        }

        if (local_id >= OSAL_MAX_VALID_PER_TYPE)
        {
            Status = OS_ERROR;
        }
        else
        {
            if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetNext), &local_record, sizeof(local_record)) <
                sizeof(local_record))
            {
                memset(&fake_record, 0, sizeof(fake_record));
                UT_ObjIdCompose(local_id, idtype, &fake_record.active_id);
                local_record = &fake_record;
            }

            if (curr_index != NULL)
            {
                *curr_index = local_id;
            }

            if (record != NULL)
            {
                *record = local_record;
            }
        }
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdAllocateNew()
 *
 *****************************************************************************/
int32 OS_ObjectIdAllocateNew(osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdAllocateNew);

    if (Status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdAllocateNew), token, sizeof(*token)) < sizeof(*token))
    {
        UT_TokenCompose(OS_LOCK_MODE_GLOBAL, UT_GetStubCount(UT_KEY(OS_ObjectIdAllocateNew)), idtype, token);
    }

    return Status;
}

/*--------------------------------------------------------------------------------------
     Name: OS_GetResourceName

    Purpose: Stub function for OS_GetResourceName, returns either the test-supplied string
             or an empty string.

    returns: status
---------------------------------------------------------------------------------------*/
int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_GetResourceName), object_id);
    UT_Stub_RegisterContext(UT_KEY(OS_GetResourceName), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_GetResourceName), buffer_size);

    int32 return_code;

    return_code = UT_DEFAULT_IMPL(OS_GetResourceName);

    if (return_code == OS_SUCCESS)
    {
        if (buffer_size > 0 && UT_Stub_CopyToLocal(UT_KEY(OS_GetResourceName), buffer, buffer_size) == 0)
        {
            /* return an empty string by default */
            buffer[0] = 0;
        }
    }

    return return_code;
}

/*--------------------------------------------------------------------------------------
     Name: OS_ConvertToArrayIndex

    Purpose: Converts any abstract ID into a number suitable for use as an array index.
             This is necessary for code that breaks when IDs are converted
             to nonzero ranges.  Note that this does NOT verify the validity of the ID,
             that is left to the caller.  This is only the conversion logic.

    returns: status
---------------------------------------------------------------------------------------*/
int32 OS_ConvertToArrayIndex(osal_id_t object_id, osal_index_t *ArrayIndex)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ConvertToArrayIndex), object_id);
    UT_Stub_RegisterContext(UT_KEY(OS_ConvertToArrayIndex), ArrayIndex);

    int32  return_code;
    uint32 tempserial;

    return_code = UT_DEFAULT_IMPL(OS_ConvertToArrayIndex);

    if (return_code == OS_SUCCESS)
    {
        UT_ObjType_t ObjType;
        UT_ObjIdDecompose(object_id, &tempserial, &ObjType);
        if (ObjType != UT_OBJTYPE_NONE && ObjType < UT_OBJTYPE_MAX)
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

    return return_code;
} /* end OS_ConvertToArrayIndex */

/*--------------------------------------------------------------------------------------
     Name: OS_ForEachObjectOfType

    Purpose: Stub function for OS_ForEachObjectOfType

    returns: None
---------------------------------------------------------------------------------------*/
void OS_ForEachObjectOfType(osal_objtype_t objtype, osal_id_t creator_id, OS_ArgCallback_t callback_ptr,
                            void *callback_arg)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), objtype);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), creator_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), callback_ptr);
    UT_Stub_RegisterContext(UT_KEY(OS_ForEachObjectOfType), callback_arg);

    osal_id_t NextId;
    size_t    IdSize;

    /* Although this is "void", Invoke the default impl to log it and invoke any hooks */
    UT_DEFAULT_IMPL(OS_ForEachObjectOfType);

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

/*--------------------------------------------------------------------------------------
     Name: OS_ForEachOject

    Purpose: Loops through all defined OSAL objects and calls callback_ptr on each one
             If creator_id is nonzero then only objects with matching creator id are processed.

    returns: None
---------------------------------------------------------------------------------------*/
void OS_ForEachObject(osal_id_t creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObject), creator_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObject), callback_ptr);
    UT_Stub_RegisterContext(UT_KEY(OS_ForEachObject), callback_arg);

    osal_id_t NextId;
    size_t    IdSize;

    /* Although this is "void", Invoke the default impl to log it and invoke any hooks */
    UT_DEFAULT_IMPL(OS_ForEachObject);

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

int32 OS_ObjectIdIteratorInit(OS_ObjectMatchFunc_t matchfunc, void *matcharg, osal_objtype_t objtype,
                              OS_object_iter_t *iter)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIteratorInit), matchfunc);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIteratorInit), matcharg);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIteratorInit), objtype);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIteratorInit), iter);

    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdIteratorInit, 1);

    if (Status == 1 && UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIteratorGetNext), iter, sizeof(*iter)) < sizeof(*iter))
    {
        memset(iter, 0, sizeof(*iter));
        Status = OS_SUCCESS;
    }

    return Status;
}

int32 OS_ObjectIdIterateActive(osal_objtype_t objtype, OS_object_iter_t *iter)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIterateActive), objtype);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIterateActive), iter);

    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdIterateActive, 1);

    if (Status == 1 && UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIterateActive), iter, sizeof(*iter)) < sizeof(*iter))
    {
        memset(iter, 0, sizeof(*iter));
        Status = OS_SUCCESS;
    }

    return Status;
}

bool OS_ObjectIdIteratorGetNext(OS_object_iter_t *iter)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ObjectIdIteratorGetNext), iter);

    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdIteratorGetNext, -1);

    if (Status == -1)
    {
        /* if test case has registered something, return true, otherwise return false */
        Status = (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdIteratorGetNext), &iter->token, sizeof(iter->token)) ==
                  sizeof(iter->token));
    }

    return (bool)Status;
}

void OS_ObjectIdIteratorDestroy(OS_object_iter_t *iter)
{
    UT_DEFAULT_IMPL(OS_ObjectIdIteratorDestroy);
}

int32 OS_ObjectIdIteratorProcessEntry(OS_object_iter_t *iter, int32 (*func)(osal_id_t))
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdIteratorProcessEntry);

    return Status;
}

/*---------------------------------------------------------------------------------------
   Name: OS_IdentifyObject

   Purpose: Given an arbitrary object ID, get the type of the object

   returns: The type of object that the ID represents
---------------------------------------------------------------------------------------*/
osal_objtype_t OS_IdentifyObject(osal_id_t object_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_IdentifyObject), object_id);

    UT_ObjType_t ObjType;
    uint32       checkindx;
    int32        DefaultType;

    UT_ObjIdDecompose(object_id, &checkindx, &ObjType);

    switch (ObjType)
    {
        case UT_OBJTYPE_TASK:
            DefaultType = OS_OBJECT_TYPE_OS_TASK;
            break;
        case UT_OBJTYPE_QUEUE:
            DefaultType = OS_OBJECT_TYPE_OS_QUEUE;
            break;
        case UT_OBJTYPE_COUNTSEM:
            DefaultType = OS_OBJECT_TYPE_OS_COUNTSEM;
            break;
        case UT_OBJTYPE_BINSEM:
            DefaultType = OS_OBJECT_TYPE_OS_BINSEM;
            break;
        case UT_OBJTYPE_MUTEX:
            DefaultType = OS_OBJECT_TYPE_OS_MUTEX;
            break;
        case UT_OBJTYPE_TIMECB:
            DefaultType = OS_OBJECT_TYPE_OS_TIMECB;
            break;
        case UT_OBJTYPE_MODULE:
            DefaultType = OS_OBJECT_TYPE_OS_MODULE;
            break;
        case UT_OBJTYPE_FILESTREAM:
            DefaultType = OS_OBJECT_TYPE_OS_STREAM;
            break;
        case UT_OBJTYPE_TIMEBASE:
            DefaultType = OS_OBJECT_TYPE_OS_TIMEBASE;
            break;
        case UT_OBJTYPE_DIR:
            DefaultType = OS_OBJECT_TYPE_OS_DIR;
            break;
        case UT_OBJTYPE_FILESYS:
            DefaultType = OS_OBJECT_TYPE_OS_FILESYS;
            break;
        default:
            DefaultType = OS_OBJECT_TYPE_UNDEFINED;
            break;
    }

    DefaultType = UT_DEFAULT_IMPL_RC(OS_IdentifyObject, DefaultType);

    return DefaultType;
}
