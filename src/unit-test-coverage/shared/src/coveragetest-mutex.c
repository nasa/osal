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
 * \file     coveragetest-mutex.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-mutex.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_MutexAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_MutexAPI_Init();

    UtAssert_True(actual == expected, "OS_MutexAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual = OS_MutSemCreate(&objid, "UT", 0);

    UtAssert_True(actual == expected, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(NULL, NULL, 0), OS_INVALID_POINTER);
    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, "UT", 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_MutSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_MutSemDelete(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_MutSemDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive ( uint32 sem_id )
     */
    OS_mutex_internal_record_t *mutex;
    int32                       expected;
    int32                       actual;

    expected = OS_SUCCESS;

    /* Set up for "last owner" matching the calling task (nominal) */
    mutex             = &OS_mutex_table[1];
    mutex->last_owner = OS_TaskGetId();
    actual            = OS_MutSemGive(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_MutSemGive() (%ld) == OS_SUCCESS", (long)actual);

    /* owner should be unset */
    UtAssert_True(!OS_ObjectIdDefined(mutex->last_owner), "Mutex owner unset");

    /* Call again when not "owned".  This still works (or at least it calls the OS impl)
     * but should generate a debug message */
    actual = OS_MutSemGive(UT_OBJID_1);
    UtAssert_True(actual == expected, "OS_MutSemGive(), not owned (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake ( uint32 sem_id )
     */
    OS_mutex_internal_record_t *mutex;
    int32                       expected;
    int32                       actual;

    expected = OS_SUCCESS;

    /* Set up for "last owner" being undefined (nominal) */
    mutex             = &OS_mutex_table[1];
    mutex->last_owner = OS_OBJECT_ID_UNDEFINED;
    actual            = OS_MutSemTake(UT_OBJID_1);
    UtAssert_True(actual == expected, "OS_MutSemTake() (%ld) == OS_SUCCESS", (long)actual);

    /* owner should be set */
    UtAssert_True(OS_ObjectIdDefined(mutex->last_owner), "Mutex owner set");

    /* Call again when not already "owned".  This still works (or at least it calls the OS impl)
     * but should generate a debug message */
    actual = OS_MutSemTake(UT_OBJID_1);
    UtAssert_True(actual == expected, "OS_MutSemTake() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_MutSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_MutSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_MutSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_MutSemGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_MutSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
    int32             expected = OS_SUCCESS;
    int32             actual   = ~OS_SUCCESS;
    OS_mut_sem_prop_t prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_MUTEX, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    actual = OS_MutSemGetInfo(UT_OBJID_1, &prop);

    UtAssert_True(actual == expected, "OS_MutSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);
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
    ADD_TEST(OS_MutexAPI_Init);
    ADD_TEST(OS_MutSemCreate);
    ADD_TEST(OS_MutSemDelete);
    ADD_TEST(OS_MutSemGive);
    ADD_TEST(OS_MutSemTake);
    ADD_TEST(OS_MutSemGetIdByName);
    ADD_TEST(OS_MutSemGetInfo);
}
