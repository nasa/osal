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
 * \file     coveragetest-idmap.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"
#include "os-shared-task.h"

#include <OCS_string.h>

typedef struct
{
    uint32 TaskCount;
    uint32 QueueCount;
    uint32 MutexCount;
    uint32 OtherCount;
} Test_OS_ObjTypeCount_t;

/* a match function that always matches */
static bool TestAlwaysMatch(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return true;
}

static void ObjTypeCounter(osal_id_t object_id, void *arg)
{
    Test_OS_ObjTypeCount_t *count = arg;

    switch (OS_IdentifyObject(object_id))
    {
        case OS_OBJECT_TYPE_OS_TASK:
            ++count->TaskCount;
            break;
        case OS_OBJECT_TYPE_OS_QUEUE:
            ++count->QueueCount;
            break;
        case OS_OBJECT_TYPE_OS_MUTEX:
            ++count->MutexCount;
            break;
        default:
            ++count->OtherCount;
            break;
    }
}

void Test_OS_ObjectIdInit(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdInit(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_ObjectIdInit();

    UtAssert_True(actual == expected, "OS_ObjectIdInit() (%ld) == %ld", (long)actual, (long)expected);
}

void Test_OS_LockUnlockGlobal(void)
{
    /*
     * Test Case For:
     * void OS_Lock_Global(uint32 idtype)
     * void OS_Unlock_Global(uint32 idtype)
     */

    /*
     * As these have no return codes, these tests
     * exist to get coverage of the paths.
     */
    OS_Lock_Global(OS_OBJECT_TYPE_OS_COUNTSEM);
    OS_Unlock_Global(OS_OBJECT_TYPE_OS_COUNTSEM);
    OS_Lock_Global(0);
    OS_Unlock_Global(0);
    OS_Lock_Global(55555);
    OS_Unlock_Global(55555);

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId), 0);
    OS_Lock_Global(OS_OBJECT_TYPE_OS_BINSEM);
    OS_Unlock_Global(OS_OBJECT_TYPE_OS_BINSEM);
}

void Test_OS_ObjectIdConvertToken(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdConvertToken(OS_object_token_t *token)
     *
     * NOTE: These test cases just focus on code paths that are not exercised
     * by the other test cases in this file.
     */
    int32               expected;
    int32               actual;
    OS_object_token_t   token;
    OS_common_record_t *record;
    osal_id_t           objid;

    /* get a valid (fake) OSAL ID to start with */
    OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "ut", &token);
    objid = token.obj_id;

    record            = OS_OBJECT_TABLE_GET(OS_global_task_table, token);
    record->refcount  = 5;
    record->active_id = objid;

    /*
     * Attempt to obtain a lock for the same record with a non-matching ID
     * This should return an error.
     */
    token.lock_mode = OS_LOCK_MODE_NONE;
    token.obj_id    = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(token.obj_id) ^ 0x10); /* flip a bit */
    actual          = OS_ObjectIdConvertToken(&token);
    expected        = OS_ERR_INVALID_ID;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock() (%ld) == OS_ERR_INVALID_ID (%ld)", (long)actual,
                  (long)expected);

    /*
     * Use mode OS_LOCK_MODE_NONE with matching ID
     * This should return success.
     */
    token.lock_mode = OS_LOCK_MODE_NONE;
    token.obj_id    = objid;
    actual          = OS_ObjectIdConvertToken(&token);
    expected        = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock(NONE) (%ld) == OS_SUCCESS (%ld)", (long)actual,
                  (long)expected);

    /*
     * Use mode OS_LOCK_MODE_GLOBAL with matching ID
     * This should return success, not change refcount
     */
    token.lock_mode = OS_LOCK_MODE_GLOBAL;
    token.obj_id    = objid;
    actual          = OS_ObjectIdConvertToken(&token);
    expected        = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock(GLOBAL) (%ld) == OS_SUCCESS (%ld)", (long)actual,
                  (long)expected);
    UtAssert_UINT32_EQ(record->refcount, 5);

    /*
     * Use mode OS_LOCK_MODE_REFCOUNT with matching ID
     * This should return success, increment refcount
     */
    token.lock_mode = OS_LOCK_MODE_REFCOUNT;
    token.obj_id    = objid;
    actual          = OS_ObjectIdConvertToken(&token);
    expected        = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock(REFCOUNT) (%ld) == OS_SUCCESS (%ld)", (long)actual,
                  (long)expected);
    UtAssert_UINT32_EQ(record->refcount, 6);

    /*
     * Use mode OS_LOCK_MODE_EXCLUSIVE with matching ID and other refs.
     * This should return OS_ERR_OBJECT_IN_USE.
     */
    token.lock_mode = OS_LOCK_MODE_EXCLUSIVE;
    token.obj_id    = objid;
    actual          = OS_ObjectIdConvertToken(&token);
    expected        = OS_ERR_OBJECT_IN_USE;
    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock(EXCLUSIVE) (%ld) == OS_ERR_OBJECT_IN_USE (%ld)",
                  (long)actual, (long)expected);

    /* should have delayed 4 times, on the 5th try it returns error */
    UtAssert_STUB_COUNT(OS_TaskDelay_Impl, 4);

    /*
     * Use mode OS_LOCK_MODE_EXCLUSIVE with matching ID and no other refs.
     * This should return success.
     */
    record->refcount = 0;
    actual           = OS_ObjectIdConvertToken(&token);
    expected         = OS_SUCCESS;
    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock(EXCLUSIVE) (%ld) == OS_SUCCESS (%ld)", (long)actual,
                  (long)expected);
}

void Test_OS_ObjectIdGetBySearch(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, uint32 idtype,
     *      OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record)
     *
     * NOTE: These test cases just focus on code paths that are not exercised
     * by the other test cases in this file.
     */
    int32             expected;
    int32             actual;
    OS_object_token_t token;

    OS_global_task_table[0].active_id = UT_OBJID_OTHER;
    actual   = OS_ObjectIdGetBySearch(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TASK, TestAlwaysMatch, NULL, &token);
    expected = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdGetBySearch() (%ld) == OS_SUCCESS (%ld)", (long)actual,
                  (long)expected);

    UtAssert_Bool(OS_ObjectIdEqual(token.obj_id, UT_OBJID_OTHER), "Token Object ID");
    UtAssert_UINT32_EQ(token.obj_idx, 0);

    OS_global_task_table[0].active_id = OS_OBJECT_ID_UNDEFINED;
}

void Test_OS_GetMaxForObjectType(void)
{
    /*
     * Test Case For:
     * uint32 OS_GetMaxForObjectType(uint32 idtype);
     */
    osal_objtype_t idtype;
    uint32         expected = 0xFFFFFFFF;
    uint32         max      = 0;

    for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
    {
        if (idtype == OS_OBJECT_TYPE_UNDEFINED)
        {
            expected = 0;
        }
        else
        {
            expected = 0xFFFFFFFF;
        }

        max = OS_GetMaxForObjectType(idtype);

        if (expected != 0xFFFFFFFF)
        {
            /* Verify Outputs */
            /* only the "0" is verifiable, since everything else depends on OS config */
            UtAssert_True(max == expected, "OS_GetMaxForObjectType(%lu) (%lu) == 0", (unsigned long)idtype,
                          (unsigned long)max);
        }
    }
}

void Test_OS_GetBaseForObjectType(void)
{
    /*
     * Test Case For:
     * uint32 OS_GetBaseForObjectType(uint32 idtype);
     */
    osal_objtype_t idtype;
    uint32         expected = 0xFFFFFFFF;
    uint32         max      = 0;

    for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
    {
        if (idtype == OS_OBJECT_TYPE_UNDEFINED)
        {
            expected = 0;
        }
        else
        {
            expected = 0xFFFFFFFF;
        }

        max = OS_GetBaseForObjectType(idtype);

        if (expected != 0xFFFFFFFF)
        {
            /* Verify Outputs */
            /* only the "0" is verifiable, since everything else depends on OS config */
            UtAssert_True(max == expected, "OS_GetBaseForObjectType(%lu) (%lu) == 0", (unsigned long)idtype,
                          (unsigned long)max);
        }
    }
}

void Test_OS_ObjectIdToArrayIndex(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdToArrayIndex(uint32 idtype, uint32 id, uint32 *ArrayIndex);
     *
     * Internally this uses OS_Id_UnMap for the nominal case, which is covered by a
     * different test case.  The only additional test here is to provide a value
     * which is out of range.
     */
    osal_id_t    objid;
    osal_index_t local_idx;
    int32        expected = OS_SUCCESS;
    int32        actual   = ~OS_SUCCESS;

    /* need to get a "valid" objid for the nominal case */
    OS_ObjectIdCompose_Impl(OS_OBJECT_TYPE_OS_TASK, 1, &objid);
    actual = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, objid, &local_idx);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdToArrayIndex() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(local_idx == 1, "local_idx (%lu) == 1", (unsigned long)local_idx);

    /* coverage for off-nominal case */
    expected = OS_ERR_INVALID_ID;
    actual   = OS_ObjectIdToArrayIndex(0xFFFF, UT_OBJID_OTHER, &local_idx);
    UtAssert_True(actual == expected, "OS_ObjectIdToArrayIndex() (%ld) == OS_ERR_INVALID_ID", (long)actual);
}

void Test_OS_ObjectIdFindByName(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectFindIdByName (uint32 idtype, const char *name, uint32 *object_id);
     *
     * Nominal case (with no additional setup) should return OS_ERR_NAME_NOT_FOUND
     * Setting up a special matching entry should yield OS_SUCCESS
     */
    char      TaskName[] = "UT_find";
    osal_id_t objid;
    int32     expected = OS_ERR_NAME_NOT_FOUND;
    int32     actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_UNDEFINED, NULL, NULL);

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_NOT_FOUND", "NULL", (long)actual);

    /*
     * Pass in a name that is beyond OS_MAX_API_NAME
     */
    UT_SetDefaultReturnValue(UT_KEY(OCS_strlen), OS_MAX_API_NAME + 10);
    expected = OS_ERR_NAME_TOO_LONG;
    actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_TOO_LONG", TaskName,
                  (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    /*
     * Pass in a name that is actually not found
     */
    OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_NOT_FOUND", TaskName,
                  (long)actual);

    /*
     * Set up for the ObjectIdSearch function to return success
     */
    OS_global_task_table[0].active_id  = UT_OBJID_OTHER;
    OS_global_task_table[0].name_entry = TaskName;
    actual                             = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    expected                           = OS_SUCCESS;
    OS_global_task_table[0].active_id  = OS_OBJECT_ID_UNDEFINED;
    OS_global_task_table[0].name_entry = NULL;

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_SUCCESS", TaskName, (long)actual);
}

void Test_OS_ObjectIdGetById(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, osal_objtype_t idtype, osal_id_t id, OS_object_token_t
     * *token);
     *
     */
    int32               actual   = ~OS_SUCCESS;
    int32               expected = OS_SUCCESS;
    osal_id_t           refobjid;
    osal_index_t        local_idx;
    OS_common_record_t *rptr = NULL;
    OS_object_token_t   token1;
    OS_object_token_t   token2;

    /* verify that the call returns ERROR when not initialized */
    OS_SharedGlobalVars.Initialized = false;
    actual                          = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, 0, OS_OBJECT_ID_UNDEFINED, &token1);
    expected                        = OS_ERROR;
    UtAssert_True(actual == expected, "OS_ObjectIdGetById(uninitialized) (%ld) == OS_ERROR", (long)actual);

    /* set "true" for the remainder of tests */
    OS_SharedGlobalVars.Initialized = true;

    OS_ObjectIdCompose_Impl(OS_OBJECT_TYPE_OS_TASK, 1000, &refobjid);
    OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, refobjid, &local_idx);
    rptr            = &OS_global_task_table[local_idx];
    rptr->active_id = refobjid;
    expected        = OS_SUCCESS;
    actual          = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_TASK, refobjid, &token1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_UINT32_EQ(token1.obj_idx, local_idx);
    UtAssert_True(rptr->refcount == 1, "refcount (%u) == 1", (unsigned int)rptr->refcount);

    /* attempting to get an exclusive lock should return IN_USE error */
    expected = OS_ERR_OBJECT_IN_USE;
    actual   = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TASK, refobjid, &token2);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_OBJECT_IN_USE", (long)actual);

    /* refcount decrement should work */
    OS_ObjectIdRelease(&token1);
    UtAssert_True(rptr->refcount == 0, "refcount (%u) == 0", (unsigned int)rptr->refcount);

    /* noop if done a second time */
    OS_ObjectIdRelease(&token1);
    UtAssert_True(rptr->refcount == 0, "refcount (%u) == 0", (unsigned int)rptr->refcount);

    /* attempt to get non-exclusive lock during shutdown should fail */
    OS_SharedGlobalVars.ShutdownFlag = OS_SHUTDOWN_MAGIC_NUMBER;
    expected                         = OS_ERR_INCORRECT_OBJ_STATE;
    actual                           = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TASK, refobjid, &token1);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    OS_SharedGlobalVars.ShutdownFlag = 0;

    /* attempt to get lock for invalid type object should fail */
    expected = OS_ERR_INCORRECT_OBJ_TYPE;
    actual   = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, 0xFFFF, refobjid, &token1);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);
    OS_SharedGlobalVars.ShutdownFlag = 0;

    /* clear out state entry */
    memset(&OS_global_task_table[local_idx], 0, sizeof(OS_global_task_table[local_idx]));
}

void Test_OS_ObjectIdFindNextFree(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdFindNextFree(OS_object_token_t *token);
     * int32 OS_ObjectIdFinalizeNew(int32 operation_status, &token, uint32 *outid);
     *
     * Note This test case covers both functions because they are somewhat interlinked and
     * they share state between them - The output of FindNext() should be passed to Finalize()
     * before use.
     */

    int32               expected;
    int32               actual;
    OS_object_token_t   token1;
    OS_object_token_t   token2;
    OS_common_record_t *rec1;
    OS_common_record_t *rec2;
    osal_id_t           id1;
    osal_id_t           id2;
    osal_id_t           saved_id;
    uint32              i;

    memset(&token1, 0, sizeof(token1));
    token1.lock_mode = OS_LOCK_MODE_GLOBAL;
    token1.obj_type  = OS_OBJECT_TYPE_OS_TASK;

    /* Need to first obtain a valid ID to finalize */
    expected = OS_SUCCESS;
    actual   = OS_ObjectIdFindNextFree(&token1);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNextFree() (%ld) == OS_SUCCESS", (long)actual);

    /* nominal case (success) */
    id1    = OS_OBJECT_ID_UNDEFINED;
    actual = OS_ObjectIdFinalizeNew(OS_SUCCESS, &token1, &id1);
    OSAPI_TEST_OBJID(id1, ==, token1.obj_id);

    /* Verify Outputs */
    rec1 = OS_OBJECT_TABLE_GET(OS_global_task_table, token1);
    UtAssert_True(actual == expected, "OS_ObjectIdFinalizeNew() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(token1.obj_id, ==, rec1->active_id);

    /* Allocate another ID (should be different!) */
    memset(&token2, 0, sizeof(token2));
    token2.lock_mode = OS_LOCK_MODE_GLOBAL;
    token2.obj_type  = OS_OBJECT_TYPE_OS_TASK;
    actual           = OS_ObjectIdFindNextFree(&token2);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNextFree() (%ld) == OS_SUCCESS", (long)actual);
    rec2 = OS_OBJECT_TABLE_GET(OS_global_task_table, token2);
    OSAPI_TEST_OBJID(token2.obj_id, !=, token1.obj_id);

    /* Failure to initialize the second one.
     * Verify the error code passes thru */
    expected = -1234;
    saved_id = token2.obj_id;
    id2      = OS_OBJECT_ID_UNDEFINED;
    actual   = OS_ObjectIdFinalizeNew(expected, &token2, &id2);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdFinalizeNew() (%ld) == %ld", (long)actual, (long)expected);
    OSAPI_TEST_OBJID(id2, ==, OS_OBJECT_ID_UNDEFINED);
    OSAPI_TEST_OBJID(rec2->active_id, ==, OS_OBJECT_ID_UNDEFINED);
    OSAPI_TEST_OBJID(token2.obj_id, ==, saved_id);

    /* next call should not re-issue the same id */
    memset(&token2, 0, sizeof(token2));
    token2.obj_type = OS_OBJECT_TYPE_OS_TASK;
    expected        = OS_SUCCESS;
    actual          = OS_ObjectIdFindNextFree(&token2);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNextFree() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(token2.obj_id, !=, saved_id);

    /*
     * Finally - test the wrap-around function to verify that object IDs
     * will continue to allocate correctly after OS_OBJECT_INDEX_MASK
     */
    expected = OS_SUCCESS;
    saved_id = OS_OBJECT_ID_UNDEFINED;
    for (i = 0; i < (OS_OBJECT_INDEX_MASK + 2); ++i)
    {
        actual = OS_ObjectIdFindNextFree(&token2);
        /* not using UtAssert_True here as it will create thousands  of duplicates. */
        if (expected != actual)
        {
            UtAssert_Failed("OS_ObjectIdFindNextFree() failure (%ld)", (long)actual);
            break;
        }

        actual = OS_ObjectIdFinalizeNew(OS_SUCCESS, &token2, NULL);
        if (expected != actual)
        {
            UtAssert_Failed("OS_ObjectIdFinalizeNew() failure (%ld)", (long)actual);
            break;
        }

        /* should always be different than the previous ID */
        if (OS_ObjectIdEqual(saved_id, token2.obj_id))
        {
            UtAssert_Failed("OS_ObjectIdFindNextFree() re-issued ID (%lx)", OS_ObjectIdToInteger(token2.obj_id));
            break;
        }

        /* it also should never be id1, which was previously allocated */
        if (OS_ObjectIdEqual(token1.obj_id, token2.obj_id))
        {
            UtAssert_Failed("OS_ObjectIdFindNextFree() duplicate ID (%lx)", OS_ObjectIdToInteger(token1.obj_id));
            break;
        }
        if (rec1 == rec2)
        {
            UtAssert_Failed("OS_ObjectIdFindNextFree() duplicate slot (%p)", (void *)rec1);
            break;
        }

        /* Find the wrap.  Once this occurs the test is successful. */
        if (OS_ObjectIdToInteger(saved_id) > OS_ObjectIdToInteger(token2.obj_id))
        {
            /* Success */
            break;
        }

        /* clear the entry for re-use */
        saved_id = token2.obj_id;
        rec2     = OS_OBJECT_TABLE_GET(OS_global_task_table, token2);
        memset(rec2, 0, sizeof(*rec2));
    }

    /* verify that the wrap occurred */
    UtAssert_True(i < (OS_OBJECT_INDEX_MASK + 2), "OS_ObjectIdFindNextFree() wrap around occurred");
}

void Test_OS_ObjectIdAllocateNew(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdAllocateNew(osal_objtype_t idtype, const char *name, OS_object_token_t *token)
     *
     * Most of the business logic is done by OS_ObjectIdFindNextFree() which is tested separately
     * This test case mainly focuses on additional error checking
     */
    int32             expected = OS_SUCCESS;
    int32             actual   = ~OS_SUCCESS;
    OS_object_token_t token;

    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &token);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_UINT32_EQ(token.lock_mode, OS_LOCK_MODE_EXCLUSIVE);
    UtAssert_UINT32_EQ(token.obj_type, OS_OBJECT_TYPE_OS_TASK);
    UtAssert_Bool(OS_ObjectIdDefined(token.obj_id), "ObjectIdDefined(token.obj_id)");

    /* Passing a NULL name also should work here (used for internal objects) */
    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, NULL, &token);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate(NULL) (%ld) == OS_SUCCESS", (long)actual);

    OS_global_task_table[0].name_entry = "UT_alloc";
    expected                           = OS_ERR_NAME_TAKEN;
    actual                             = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &token);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_NAME_TAKEN", (long)actual);

    OS_SharedGlobalVars.ShutdownFlag = OS_SHUTDOWN_MAGIC_NUMBER;
    expected                         = OS_ERR_INCORRECT_OBJ_STATE;
    actual                           = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &token);
    OS_SharedGlobalVars.ShutdownFlag = 0;
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

    expected = OS_ERR_INCORRECT_OBJ_TYPE;
    actual   = OS_ObjectIdAllocateNew(0xFFFF, "UT_alloc", &token);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);
}

void Test_OS_ConvertToArrayIndex(void)
{
    /*
     * Test Case For:
     * int32 OS_ConvertToArrayIndex(uint32 object_id, uint32 *ArrayIndex);
     *
     *
     */
    int32        expected = OS_SUCCESS;
    int32        actual   = ~OS_SUCCESS; // OS_ConvertToArrayIndex();
    osal_id_t    refobjid;
    osal_index_t local_idx;

    /* Need a valid ID to work with */
    OS_ObjectIdCompose_Impl(OS_OBJECT_TYPE_OS_TASK, 1234, &refobjid);
    actual = OS_ConvertToArrayIndex(refobjid, &local_idx);
    UtAssert_True(actual == expected, "OS_ConvertToArrayIndex() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(local_idx < OS_MAX_TASKS, "local_idx (%lu) < OS_MAX_TASKS (%lu)", (unsigned long)local_idx,
                  (unsigned long)OS_MAX_TASKS);

    expected = OS_ERR_INVALID_ID;
    actual   = OS_ConvertToArrayIndex(OS_OBJECT_ID_UNDEFINED, &local_idx);
    UtAssert_True(actual == expected, "OS_ConvertToArrayIndex() (%ld) == OS_ERR_INVALID_ID", (long)actual);
}

void Test_OS_ForEachObject(void)
{
    /*
     * Test Case For:
     * void OS_ForEachObject (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);
     */
    OS_object_token_t      token;
    UT_idbuf_t             self_id;
    Test_OS_ObjTypeCount_t Count;

    self_id.id = OS_TaskGetId();

    memset(&Count, 0, sizeof(Count));
    memset(&token, 0, sizeof(token));

    while (token.obj_type < OS_OBJECT_TYPE_USER)
    {
        if (OS_ObjectIdFindNextFree(&token) == OS_SUCCESS)
        {
            OS_ObjectIdGlobalFromToken(&token)->active_id = token.obj_id;
        }
        ++token.obj_type;
    }

    OS_ForEachObject(OS_OBJECT_ID_UNDEFINED, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObject() TaskCount (%lu) == 1", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 1, "OS_ForEachObject() QueueCount (%lu) == 1", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObject() MutexCount (%lu) == 1", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.OtherCount == 9, "OS_ForEachObject() OtherCount (%lu) == 9", (unsigned long)Count.OtherCount);

    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_QUEUE, self_id.id, ObjTypeCounter, &Count);
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObjectOfType(), creator %08lx TaskCount (%lu) == 1",
                  (unsigned long)self_id.val, (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 2, "OS_ForEachObjectOfType() QueueCount (%lu) == 2",
                  (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObjectOfType() MutexCount (%lu) == 1",
                  (unsigned long)Count.MutexCount);

    self_id.val ^= 0x01;
    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_QUEUE, self_id.id, ObjTypeCounter, &Count);
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObjectOfType(), non-matching creator TaskCount (%lu) == 1",
                  (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 2, "OS_ForEachObjectOfType() QueueCount (%lu) == 2",
                  (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObjectOfType() MutexCount (%lu) == 1",
                  (unsigned long)Count.MutexCount);
}

void Test_OS_GetResourceName(void)
{
    /*
     * Test Case For:
     * int32 OS_GetResourceName(uint32 id, char *buffer, uint32 buffer_size)
     */
    OS_object_token_t   token;
    OS_common_record_t *rptr;
    char                NameBuffer[OS_MAX_API_NAME];
    int32               expected;
    int32               actual;

    /*
     * Set up for the OS_GetResourceName function to return success
     */
    /* Need a valid ID to work with */
    memset(&token, 0, sizeof(token));
    token.obj_type = OS_OBJECT_TYPE_OS_TASK;
    OS_ObjectIdFindNextFree(&token);
    rptr             = OS_OBJECT_TABLE_GET(OS_global_task_table, token);
    rptr->name_entry = "UTTask";
    rptr->active_id  = token.obj_id;

    expected = OS_SUCCESS;
    actual   = OS_GetResourceName(token.obj_id, NameBuffer, sizeof(NameBuffer));
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(NameBuffer, "UTTask") == 0, "NameBuffer (%s) == UTTask", NameBuffer);

    expected = OS_ERR_NAME_TOO_LONG;
    actual   = OS_GetResourceName(token.obj_id, NameBuffer, OSAL_SIZE_C(2));
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_GetResourceName(token.obj_id, NULL, OSAL_SIZE_C(0));
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_INVALID_POINTER", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    /* for sanity also clear out the task table, which is used by several test cases */
    memset(OS_global_task_table, 0, OS_MAX_TASKS * sizeof(OS_common_record_t));

    /*
     * The OS_SharedGlobalVars is also used here, but set the
     * "Initialized" field to true by default, as this is needed by most tests.
     */
    memset(&OS_SharedGlobalVars, 0, sizeof(OS_SharedGlobalVars));
    OS_SharedGlobalVars.Initialized = true;
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ObjectIdInit);
    ADD_TEST(OS_LockUnlockGlobal);
    ADD_TEST(OS_ObjectIdFindNextFree);
    ADD_TEST(OS_ObjectIdToArrayIndex);
    ADD_TEST(OS_ObjectIdFindByName);
    ADD_TEST(OS_ObjectIdGetById);
    ADD_TEST(OS_ObjectIdAllocateNew);
    ADD_TEST(OS_ObjectIdConvertToken);
    ADD_TEST(OS_ObjectIdGetBySearch);
    ADD_TEST(OS_ConvertToArrayIndex);
    ADD_TEST(OS_ForEachObject);
    ADD_TEST(OS_GetMaxForObjectType);
    ADD_TEST(OS_GetBaseForObjectType);
    ADD_TEST(OS_GetResourceName);
}
