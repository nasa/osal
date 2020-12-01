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
 * \file     coveragetest-queue.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-queue.h"

#include <OCS_string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_QueueAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_QueueAPI_Init();

    UtAssert_True(actual == expected, "OS_QueueAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_QueueCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth, uint32 data_size, uint32
     * flags)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual = OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(0), OSAL_SIZE_C(0), 0);

    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual   = OS_QueueCreate(NULL, "UT", OSAL_BLOCKCOUNT_C(0), OSAL_SIZE_C(0), 0);
    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    expected = OS_ERR_NAME_TOO_LONG;
    actual   = OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(0), OSAL_SIZE_C(0), 0);
    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    expected = OS_QUEUE_INVALID_SIZE;
    actual   = OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(1 + OS_QUEUE_MAX_DEPTH), OSAL_SIZE_C(0), 0);
    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_QUEUE_INVALID_SIZE", (long)actual);
}

void Test_OS_QueueDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueDelete (uint32 queue_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_QueueDelete(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_QueueDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_QueueGet(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
     */
    int32  expected = OS_SUCCESS;
    int32  actual   = ~OS_SUCCESS;
    size_t actual_size;
    char   Buf[4];

    actual = OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), &actual_size, 0);

    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual   = OS_QueueGet(UT_OBJID_1, NULL, sizeof(Buf), &actual_size, 0);
    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_INVALID_POINTER", (long)actual);

    OS_queue_table[1].max_size = sizeof(Buf) + 10;
    expected                   = OS_QUEUE_INVALID_SIZE;
    actual                     = OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), &actual_size, 0);
    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_QUEUE_INVALID_SIZE", (long)actual);
}

void Test_OS_QueuePut(void)
{
    /*
     * Test Case For:
     * int32 OS_QueuePut (uint32 queue_id, const void *data, uint32 size, uint32 flags)
     */
    int32      expected = OS_SUCCESS;
    int32      actual   = ~OS_SUCCESS;
    const char Data[4]  = "xyz";

    actual = OS_QueuePut(UT_OBJID_1, Data, sizeof(Data), 0);

    UtAssert_True(actual == expected, "OS_QueuePut() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual   = OS_QueuePut(UT_OBJID_1, NULL, sizeof(Data), 0);
    UtAssert_True(actual == expected, "OS_QueuePut() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_QueueGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_QueueGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_QueueGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    expected = OS_INVALID_POINTER;
    actual   = OS_QueueGetIdByName(NULL, "NF");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_QueueGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop)
     */
    int32           expected = OS_SUCCESS;
    int32           actual   = ~OS_SUCCESS;
    OS_queue_prop_t queue_prop;
    osal_id_t       id;

    id = UT_OBJID_OTHER;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_QUEUE, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    actual = OS_QueueGetInfo(UT_OBJID_1, &queue_prop);

    UtAssert_True(actual == expected, "OS_QueueGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_MemCmp(&queue_prop.creator, &id, sizeof(osal_id_t), "queue_prop.creator == UT_OBJID_OTHER");
    UtAssert_True(strcmp(queue_prop.name, "ABC") == 0, "queue_prop.name (%s) == ABC", queue_prop.name);

    expected = OS_INVALID_POINTER;
    actual   = OS_QueueGetInfo(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_QueueGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);
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
    ADD_TEST(OS_QueueAPI_Init);
    ADD_TEST(OS_QueueCreate);
    ADD_TEST(OS_QueueDelete);
    ADD_TEST(OS_QueueGet);
    ADD_TEST(OS_QueuePut);
    ADD_TEST(OS_QueueGetIdByName);
    ADD_TEST(OS_QueueGetInfo);
}
