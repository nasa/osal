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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-queue.h"

#include "OCS_string.h"

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
    OSAPI_TEST_FUNCTION_RC(OS_QueueAPI_Init(), OS_SUCCESS);
}

void Test_OS_QueueCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth, uint32 data_size, uint32
     * flags)
     */
    osal_id_t objid;

    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(4), 0), OS_SUCCESS);

    /* test error cases */
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(NULL, "UT", OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(4), 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, NULL, OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(4), 0), OS_INVALID_POINTER);

    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(0), 0), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(UINT32_MAX), 0),
                           OS_ERR_INVALID_SIZE);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(0), OSAL_SIZE_C(4), 0), OS_ERR_NAME_TOO_LONG);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(1 + OS_QUEUE_MAX_DEPTH), OSAL_SIZE_C(4), 0),
                           OS_QUEUE_INVALID_SIZE);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate(&objid, "UT", OSAL_BLOCKCOUNT_C(4), OSAL_SIZE_C(4), 0), OS_ERROR);
}

void Test_OS_QueueDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueDelete (uint32 queue_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_QueueDelete(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueDelete(UT_OBJID_1), OS_ERROR);
}

void Test_OS_QueueGet(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
     */
    size_t actual_size;
    char   Buf[4];

    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), &actual_size, 0), OS_SUCCESS);

    /* test error cases */
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, NULL, sizeof(Buf), &actual_size, 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), NULL, 0), OS_INVALID_POINTER);

    OS_queue_table[1].max_size = sizeof(Buf) + 10;
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), &actual_size, 0), OS_QUEUE_INVALID_SIZE);

    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, OSAL_SIZE_C(0), &actual_size, 0), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, OSAL_SIZE_C(UINT32_MAX), &actual_size, 0), OS_ERR_INVALID_SIZE);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet(UT_OBJID_1, Buf, sizeof(Buf), &actual_size, 0), OS_ERROR);
}

void Test_OS_QueuePut(void)
{
    /*
     * Test Case For:
     * int32 OS_QueuePut (uint32 queue_id, const void *data, uint32 size, uint32 flags)
     */
    const char Data[4] = "xyz";

    OS_queue_table[1].max_depth = 10;
    OS_queue_table[1].max_size  = sizeof(Data);

    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, Data, sizeof(Data), 0), OS_SUCCESS);

    /* test error cases */
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, NULL, sizeof(Data), 0), OS_INVALID_POINTER);

    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, Data, 1 + sizeof(Data), 0), OS_QUEUE_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, Data, OSAL_SIZE_C(0), 0), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, Data, OSAL_SIZE_C(UINT32_MAX), 0), OS_ERR_INVALID_SIZE);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut(UT_OBJID_1, Data, sizeof(Data), 0), OS_ERROR);
}

void Test_OS_QueueGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
     */
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGetIdByName(&objid, "UT"), OS_SUCCESS);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    OSAPI_TEST_FUNCTION_RC(OS_QueueGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    OSAPI_TEST_FUNCTION_RC(OS_QueueGetIdByName(NULL, "NF"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGetIdByName(&objid, NULL), OS_INVALID_POINTER);
}

void Test_OS_QueueGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop)
     */
    OS_queue_prop_t queue_prop;
    osal_id_t       id = UT_OBJID_OTHER;

    memset(&queue_prop, 0, sizeof(queue_prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_QUEUE, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OSAPI_TEST_FUNCTION_RC(OS_QueueGetInfo(UT_OBJID_1, &queue_prop), OS_SUCCESS);
    UtAssert_MemCmp(&queue_prop.creator, &id, sizeof(osal_id_t), "queue_prop.creator == UT_OBJID_OTHER");
    UtAssert_True(strcmp(queue_prop.name, "ABC") == 0, "queue_prop.name (%s) == ABC", queue_prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_QueueGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGetInfo(UT_OBJID_1, &queue_prop), OS_ERROR);
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
