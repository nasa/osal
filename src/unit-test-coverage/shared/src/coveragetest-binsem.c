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
 * \file     coveragetest-binsem.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-binsem.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_BinSemAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemAPI_Init(), OS_SUCCESS);
}

void Test_OS_BinSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemCreate (uint32 *sem_id, const char *sem_name,
     *          uint32 sem_initial_value, uint32 options)
     */
    osal_id_t objid;

    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(&objid, "UT", 0, 0), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(NULL, "UT", 0, 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(&objid, NULL, 0, 0), OS_INVALID_POINTER);
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(&objid, "UT", 0, 0), OS_ERR_NAME_TOO_LONG);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(&objid, "UT", 0, 0), OS_ERR_NO_FREE_IDS);
}

void Test_OS_BinSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemDelete (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemDelete(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemDelete(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_BinSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGive ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGive(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGive(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_BinSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTake ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTake(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTake(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_BinSemFlush(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemFlush (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemFlush(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemFlush(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_BinSemTimedWait(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTimedWait ( uint32 sem_id, uint32 msecs )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTimedWait(UT_OBJID_1, 1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTimedWait(UT_OBJID_1, 1), OS_ERR_INVALID_ID);
}

void Test_OS_BinSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_BinSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_BinSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_BinSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_BinSemGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetIdByName(&objid, NULL), OS_INVALID_POINTER);
}

void Test_OS_BinSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetInfo (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
     */
    int32             expected = OS_SUCCESS;
    int32             actual   = ~OS_SUCCESS;
    OS_bin_sem_prop_t prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_BINSEM, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    actual = OS_BinSemGetInfo(UT_OBJID_1, &prop);

    UtAssert_True(actual == expected, "OS_BinSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetInfo(UT_OBJID_1, &prop), OS_ERR_INVALID_ID);
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
    ADD_TEST(OS_BinSemAPI_Init);
    ADD_TEST(OS_BinSemCreate);
    ADD_TEST(OS_BinSemDelete);
    ADD_TEST(OS_BinSemGive);
    ADD_TEST(OS_BinSemTake);
    ADD_TEST(OS_BinSemFlush);
    ADD_TEST(OS_BinSemTimedWait);
    ADD_TEST(OS_BinSemGetIdByName);
    ADD_TEST(OS_BinSemGetInfo);
}
