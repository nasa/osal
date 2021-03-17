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
 * \file     coveragetest-countsem.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-countsem.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_CountSemAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_CountSemAPI_Init();

    UtAssert_True(actual == expected, "OS_CountSemAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemCreate (uint32 *sem_id, const char *sem_name,
     *          uint32 sem_initial_value, uint32 options)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual = OS_CountSemCreate(&objid, "UT", 0, 0);

    UtAssert_True(actual == expected, "OS_CountSemCreate() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate(NULL, NULL, 0, 0), OS_INVALID_POINTER);
    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate(&objid, "UT", 0, 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_CountSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemDelete (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_CountSemDelete(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_CountSemDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGive ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_CountSemGive(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_CountSemGive() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTake ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_CountSemTake(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_CountSemTake() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemTimedWait(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTimedWait ( uint32 sem_id, uint32 msecs )
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_CountSemTimedWait(UT_OBJID_1, 1);

    UtAssert_True(actual == expected, "OS_CountSemTimedWait() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_CountSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_CountSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_CountSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_CountSemGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetIdByName(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_CountSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)
     */
    int32               expected = OS_SUCCESS;
    int32               actual   = ~OS_SUCCESS;
    OS_count_sem_prop_t prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_COUNTSEM, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    actual = OS_CountSemGetInfo(UT_OBJID_1, &prop);

    UtAssert_True(actual == expected, "OS_CountSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);
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
    ADD_TEST(OS_CountSemAPI_Init);
    ADD_TEST(OS_CountSemCreate);
    ADD_TEST(OS_CountSemDelete);
    ADD_TEST(OS_CountSemGive);
    ADD_TEST(OS_CountSemTake);
    ADD_TEST(OS_CountSemTimedWait);
    ADD_TEST(OS_CountSemGetIdByName);
    ADD_TEST(OS_CountSemGetInfo);
}
