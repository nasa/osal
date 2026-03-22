/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-rwlock.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_RwLockAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_RwLockAPI_Init(), OS_SUCCESS);
}

void Test_OS_RwLockCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockCreate (uint32 *rw_id, const char *rw_name, uint32 options)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate(&objid, "UT", 0), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate(NULL, "UT", 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate(&objid, NULL, 0), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate(&objid, "UT", 0), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate(&objid, "UT", 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_RwLockDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockDelete (uint32 rw_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockDelete(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockDelete(UT_OBJID_1), OS_ERROR);
}

void Test_OS_RwLockReadGive(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockReadGive ( uint32 rw_id )
     */
    OS_rwlock_internal_record_t *rwlock;

    /* Call when held by a writer.  This still works (or at least it calls the OS impl)
     * but should generate a debug message */
    rwlock              = &OS_rwlock_table[1];
    rwlock->last_writer = OS_TaskGetId();
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadGive(UT_OBJID_1), OS_SUCCESS);

    /* Since rwlocks have no mechanism to check if a thread holds a read lock, this will always work */
    rwlock->last_writer = OS_OBJECT_ID_UNDEFINED;
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadGive(UT_OBJID_1), OS_SUCCESS);

    /* writer should still be undefined */
    UtAssert_True(!OS_ObjectIdDefined(rwlock->last_writer), "RwLock writer unset");

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadGive(UT_OBJID_1), OS_ERROR);
}

void Test_OS_RwLockWriteGive(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockWriteGive ( uint32 rw_id )
     */
    OS_rwlock_internal_record_t *rwlock;

    /* Set up for "last writer" matching the calling task (nominal) */
    rwlock              = &OS_rwlock_table[1];
    rwlock->last_writer = OS_TaskGetId();
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteGive(UT_OBJID_1), OS_SUCCESS);

    /* writer should be unset */
    UtAssert_True(!OS_ObjectIdDefined(rwlock->last_writer), "RwLock writer unset");

    /* Call again when not held by a writer.  This still works (or at least it calls the OS impl)
     * but should generate a debug message */
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteGive(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteGive(UT_OBJID_1), OS_ERROR);
}

void Test_OS_RwLockReadTake(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockReadTake ( uint32 rw_id )
     */
    OS_rwlock_internal_record_t *rwlock;

    /* Set up for "last writer" being undefined (nominal) */
    rwlock              = &OS_rwlock_table[1];
    rwlock->last_writer = OS_OBJECT_ID_UNDEFINED;
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake(UT_OBJID_1), OS_SUCCESS);

    /* writer shouldn't be set */
    UtAssert_True(!OS_ObjectIdDefined(rwlock->last_writer), "RwLock writer unset");

    UT_SetDefaultReturnValue(UT_KEY(OS_RwLockReadTake_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake(UT_OBJID_1), OS_ERROR);

    /* Try and get the debug message to print */
    rwlock->last_writer = OS_OBJECT_ID_RESERVED;
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_RwLockReadTake_Impl), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake(UT_OBJID_1), OS_SUCCESS);
    rwlock->last_writer = OS_OBJECT_ID_UNDEFINED;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_RwLockWriteTake(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockWriteTake ( uint32 rw_id )
     */
    OS_rwlock_internal_record_t *rwlock;

    /* Set up for "last writer" being undefined (nominal) */
    rwlock              = &OS_rwlock_table[1];
    rwlock->last_writer = OS_OBJECT_ID_UNDEFINED;
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteTake(UT_OBJID_1), OS_SUCCESS);

    /* writer should be set */
    UtAssert_True(OS_ObjectIdDefined(rwlock->last_writer), "RwLock writer set");

    /* Call again when holding write lock.  This still works (or at least it calls the OS impl) */
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteTake(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_RwLockWriteTake_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteTake(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteTake(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_RwLockGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockGetIdByName (uint32 *rw_id, const char *rw_name)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetIdByName(&objid, "UT"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetIdByName(&objid, NULL), OS_INVALID_POINTER);
}

void Test_OS_RwLockGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_RwLockGetInfo (uint32 rw_id, OS_rwlock_prop_t *rw_prop)
     */
    OS_rwlock_prop_t prop;

    memset(&prop, 0, sizeof(prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_RWLOCK, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetInfo(UT_OBJID_1, &prop), OS_SUCCESS);

    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetInfo(UT_OBJID_1, &prop), OS_ERR_INVALID_ID);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
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
    ADD_TEST(OS_RwLockAPI_Init);
    ADD_TEST(OS_RwLockCreate);
    ADD_TEST(OS_RwLockDelete);
    ADD_TEST(OS_RwLockReadGive);
    ADD_TEST(OS_RwLockWriteGive);
    ADD_TEST(OS_RwLockReadTake);
    ADD_TEST(OS_RwLockWriteTake);
    ADD_TEST(OS_RwLockGetIdByName);
    ADD_TEST(OS_RwLockGetInfo);
}
