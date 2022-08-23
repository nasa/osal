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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-idmap header
 */

#include "os-shared-idmap.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_GetBaseForObjectType(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_GetMaxForObjectType(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdAllocateNew(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdFinalizeDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdFinalizeNew(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdFindByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdGetById(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdGetByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdGetBySearch(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdGlobalFromToken(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdIterateActive(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdIteratorGetNext(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdIteratorInit(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdTransactionInit(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_ObjectIdTransferToken(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetBaseForObjectType()
 * ----------------------------------------------------
 */
uint32 OS_GetBaseForObjectType(osal_objtype_t idtype)
{
    UT_GenStub_SetupReturnBuffer(OS_GetBaseForObjectType, uint32);

    UT_GenStub_AddParam(OS_GetBaseForObjectType, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_GetBaseForObjectType, Basic, UT_DefaultHandler_OS_GetBaseForObjectType);

    return UT_GenStub_GetReturnValue(OS_GetBaseForObjectType, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetMaxForObjectType()
 * ----------------------------------------------------
 */
uint32 OS_GetMaxForObjectType(osal_objtype_t idtype)
{
    UT_GenStub_SetupReturnBuffer(OS_GetMaxForObjectType, uint32);

    UT_GenStub_AddParam(OS_GetMaxForObjectType, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_GetMaxForObjectType, Basic, UT_DefaultHandler_OS_GetMaxForObjectType);

    return UT_GenStub_GetReturnValue(OS_GetMaxForObjectType, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Lock_Global()
 * ----------------------------------------------------
 */
void OS_Lock_Global(OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_Lock_Global, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_Lock_Global, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectFilterActive()
 * ----------------------------------------------------
 */
bool OS_ObjectFilterActive(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectFilterActive, bool);

    UT_GenStub_AddParam(OS_ObjectFilterActive, void *, ref);
    UT_GenStub_AddParam(OS_ObjectFilterActive, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ObjectFilterActive, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_ObjectFilterActive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectFilterActive, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdAllocateNew()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdAllocateNew(osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdAllocateNew, int32);

    UT_GenStub_AddParam(OS_ObjectIdAllocateNew, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdAllocateNew, const char *, name);
    UT_GenStub_AddParam(OS_ObjectIdAllocateNew, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdAllocateNew, Basic, UT_DefaultHandler_OS_ObjectIdAllocateNew);

    return UT_GenStub_GetReturnValue(OS_ObjectIdAllocateNew, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdConvertToken()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdConvertToken(OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdConvertToken, int32);

    UT_GenStub_AddParam(OS_ObjectIdConvertToken, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdConvertToken, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectIdConvertToken, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdFinalizeDelete()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdFinalizeDelete(int32 operation_status, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdFinalizeDelete, int32);

    UT_GenStub_AddParam(OS_ObjectIdFinalizeDelete, int32, operation_status);
    UT_GenStub_AddParam(OS_ObjectIdFinalizeDelete, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdFinalizeDelete, Basic, UT_DefaultHandler_OS_ObjectIdFinalizeDelete);

    return UT_GenStub_GetReturnValue(OS_ObjectIdFinalizeDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdFinalizeNew()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_object_token_t *token, osal_id_t *outid)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdFinalizeNew, int32);

    UT_GenStub_AddParam(OS_ObjectIdFinalizeNew, int32, operation_status);
    UT_GenStub_AddParam(OS_ObjectIdFinalizeNew, OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ObjectIdFinalizeNew, osal_id_t *, outid);

    UT_GenStub_Execute(OS_ObjectIdFinalizeNew, Basic, UT_DefaultHandler_OS_ObjectIdFinalizeNew);

    return UT_GenStub_GetReturnValue(OS_ObjectIdFinalizeNew, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdFindByName()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdFindByName(osal_objtype_t idtype, const char *name, osal_id_t *object_id)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdFindByName, int32);

    UT_GenStub_AddParam(OS_ObjectIdFindByName, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdFindByName, const char *, name);
    UT_GenStub_AddParam(OS_ObjectIdFindByName, osal_id_t *, object_id);

    UT_GenStub_Execute(OS_ObjectIdFindByName, Basic, UT_DefaultHandler_OS_ObjectIdFindByName);

    return UT_GenStub_GetReturnValue(OS_ObjectIdFindByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdFindNextFree()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdFindNextFree(OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdFindNextFree, int32);

    UT_GenStub_AddParam(OS_ObjectIdFindNextFree, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdFindNextFree, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectIdFindNextFree, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdFindNextMatch()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdFindNextMatch(OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdFindNextMatch, int32);

    UT_GenStub_AddParam(OS_ObjectIdFindNextMatch, OS_ObjectMatchFunc_t, MatchFunc);
    UT_GenStub_AddParam(OS_ObjectIdFindNextMatch, void *, arg);
    UT_GenStub_AddParam(OS_ObjectIdFindNextMatch, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdFindNextMatch, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectIdFindNextMatch, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdGetById()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, osal_objtype_t idtype, osal_id_t id, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdGetById, int32);

    UT_GenStub_AddParam(OS_ObjectIdGetById, OS_lock_mode_t, lock_mode);
    UT_GenStub_AddParam(OS_ObjectIdGetById, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdGetById, osal_id_t, id);
    UT_GenStub_AddParam(OS_ObjectIdGetById, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdGetById, Basic, UT_DefaultHandler_OS_ObjectIdGetById);

    return UT_GenStub_GetReturnValue(OS_ObjectIdGetById, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdGetByName()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdGetByName(OS_lock_mode_t lock_mode, osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdGetByName, int32);

    UT_GenStub_AddParam(OS_ObjectIdGetByName, OS_lock_mode_t, lock_mode);
    UT_GenStub_AddParam(OS_ObjectIdGetByName, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdGetByName, const char *, name);
    UT_GenStub_AddParam(OS_ObjectIdGetByName, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdGetByName, Basic, UT_DefaultHandler_OS_ObjectIdGetByName);

    return UT_GenStub_GetReturnValue(OS_ObjectIdGetByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdGetBySearch()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg,
                             OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdGetBySearch, int32);

    UT_GenStub_AddParam(OS_ObjectIdGetBySearch, OS_lock_mode_t, lock_mode);
    UT_GenStub_AddParam(OS_ObjectIdGetBySearch, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdGetBySearch, OS_ObjectMatchFunc_t, MatchFunc);
    UT_GenStub_AddParam(OS_ObjectIdGetBySearch, void *, arg);
    UT_GenStub_AddParam(OS_ObjectIdGetBySearch, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdGetBySearch, Basic, UT_DefaultHandler_OS_ObjectIdGetBySearch);

    return UT_GenStub_GetReturnValue(OS_ObjectIdGetBySearch, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdGlobalFromToken()
 * ----------------------------------------------------
 */
OS_common_record_t *OS_ObjectIdGlobalFromToken(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdGlobalFromToken, OS_common_record_t *);

    UT_GenStub_AddParam(OS_ObjectIdGlobalFromToken, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdGlobalFromToken, Basic, UT_DefaultHandler_OS_ObjectIdGlobalFromToken);

    return UT_GenStub_GetReturnValue(OS_ObjectIdGlobalFromToken, OS_common_record_t *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdInit()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdInit(void)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdInit, int32);

    UT_GenStub_Execute(OS_ObjectIdInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectIdInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdIterateActive()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdIterateActive(osal_objtype_t objtype, OS_object_iter_t *iter)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdIterateActive, int32);

    UT_GenStub_AddParam(OS_ObjectIdIterateActive, osal_objtype_t, objtype);
    UT_GenStub_AddParam(OS_ObjectIdIterateActive, OS_object_iter_t *, iter);

    UT_GenStub_Execute(OS_ObjectIdIterateActive, Basic, UT_DefaultHandler_OS_ObjectIdIterateActive);

    return UT_GenStub_GetReturnValue(OS_ObjectIdIterateActive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdIteratorDestroy()
 * ----------------------------------------------------
 */
void OS_ObjectIdIteratorDestroy(OS_object_iter_t *iter)
{
    UT_GenStub_AddParam(OS_ObjectIdIteratorDestroy, OS_object_iter_t *, iter);

    UT_GenStub_Execute(OS_ObjectIdIteratorDestroy, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdIteratorGetNext()
 * ----------------------------------------------------
 */
bool OS_ObjectIdIteratorGetNext(OS_object_iter_t *iter)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdIteratorGetNext, bool);

    UT_GenStub_AddParam(OS_ObjectIdIteratorGetNext, OS_object_iter_t *, iter);

    UT_GenStub_Execute(OS_ObjectIdIteratorGetNext, Basic, UT_DefaultHandler_OS_ObjectIdIteratorGetNext);

    return UT_GenStub_GetReturnValue(OS_ObjectIdIteratorGetNext, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdIteratorInit()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdIteratorInit(OS_ObjectMatchFunc_t matchfunc, void *matcharg, osal_objtype_t objtype,
                              OS_object_iter_t *iter)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdIteratorInit, int32);

    UT_GenStub_AddParam(OS_ObjectIdIteratorInit, OS_ObjectMatchFunc_t, matchfunc);
    UT_GenStub_AddParam(OS_ObjectIdIteratorInit, void *, matcharg);
    UT_GenStub_AddParam(OS_ObjectIdIteratorInit, osal_objtype_t, objtype);
    UT_GenStub_AddParam(OS_ObjectIdIteratorInit, OS_object_iter_t *, iter);

    UT_GenStub_Execute(OS_ObjectIdIteratorInit, Basic, UT_DefaultHandler_OS_ObjectIdIteratorInit);

    return UT_GenStub_GetReturnValue(OS_ObjectIdIteratorInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdIteratorProcessEntry()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdIteratorProcessEntry(OS_object_iter_t *iter, OS_ObjectIdIteratorProcessFunc_t func)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdIteratorProcessEntry, int32);

    UT_GenStub_AddParam(OS_ObjectIdIteratorProcessEntry, OS_object_iter_t *, iter);
    UT_GenStub_AddParam(OS_ObjectIdIteratorProcessEntry, OS_ObjectIdIteratorProcessFunc_t, func);

    UT_GenStub_Execute(OS_ObjectIdIteratorProcessEntry, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectIdIteratorProcessEntry, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdRelease()
 * ----------------------------------------------------
 */
void OS_ObjectIdRelease(OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_ObjectIdRelease, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdRelease, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdTransactionCancel()
 * ----------------------------------------------------
 */
void OS_ObjectIdTransactionCancel(OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_ObjectIdTransactionCancel, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdTransactionCancel, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdTransactionFinish()
 * ----------------------------------------------------
 */
void OS_ObjectIdTransactionFinish(OS_object_token_t *token, const osal_id_t *final_id)
{
    UT_GenStub_AddParam(OS_ObjectIdTransactionFinish, OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ObjectIdTransactionFinish, const osal_id_t *, final_id);

    UT_GenStub_Execute(OS_ObjectIdTransactionFinish, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdTransactionInit()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdTransactionInit(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdTransactionInit, int32);

    UT_GenStub_AddParam(OS_ObjectIdTransactionInit, OS_lock_mode_t, lock_mode);
    UT_GenStub_AddParam(OS_ObjectIdTransactionInit, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdTransactionInit, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ObjectIdTransactionInit, Basic, UT_DefaultHandler_OS_ObjectIdTransactionInit);

    return UT_GenStub_GetReturnValue(OS_ObjectIdTransactionInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdTransferToken()
 * ----------------------------------------------------
 */
void OS_ObjectIdTransferToken(OS_object_token_t *token_from, OS_object_token_t *token_to)
{
    UT_GenStub_AddParam(OS_ObjectIdTransferToken, OS_object_token_t *, token_from);
    UT_GenStub_AddParam(OS_ObjectIdTransferToken, OS_object_token_t *, token_to);

    UT_GenStub_Execute(OS_ObjectIdTransferToken, Basic, UT_DefaultHandler_OS_ObjectIdTransferToken);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectNameMatch()
 * ----------------------------------------------------
 */
bool OS_ObjectNameMatch(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectNameMatch, bool);

    UT_GenStub_AddParam(OS_ObjectNameMatch, void *, ref);
    UT_GenStub_AddParam(OS_ObjectNameMatch, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ObjectNameMatch, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_ObjectNameMatch, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ObjectNameMatch, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Unlock_Global()
 * ----------------------------------------------------
 */
void OS_Unlock_Global(OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_Unlock_Global, OS_object_token_t *, token);

    UT_GenStub_Execute(OS_Unlock_Global, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_WaitForStateChange()
 * ----------------------------------------------------
 */
void OS_WaitForStateChange(OS_object_token_t *token, uint32 attempts)
{
    UT_GenStub_AddParam(OS_WaitForStateChange, OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_WaitForStateChange, uint32, attempts);

    UT_GenStub_Execute(OS_WaitForStateChange, Basic, NULL);
}
