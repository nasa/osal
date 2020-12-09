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
 * \file     coveragetest-task.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-task.h"

#include <OCS_string.h>

static uint32 UT_TestHook_Count = 0;

static void UT_TestHook(void)
{
    ++UT_TestHook_Count;
}

/*
**********************************************************************************
**          INTERNAL API TEST CASES
**********************************************************************************
*/

void Test_OS_TaskEntryPoint(void)
{
    /*
     * Test Case For:
     * void OS_TaskEntryPoint(uint32 task_id)
     */

    /* by default with no additional setup,
     * the internal "prepare" call will return INVALID_ID,
     * which in turn will invoke OS_TaskExit()
     */
    uint32 CallCount = 0;

    UT_TestHook_Count = 0;
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERROR);
    OS_TaskEntryPoint(UT_OBJID_1);
    UtAssert_True(UT_TestHook_Count == 0, "UT_TestHook_Count (%lu) == 0", (unsigned long)UT_TestHook_Count);
    CallCount = UT_GetStubCount(UT_KEY(OS_TaskMatch_Impl));
    UtAssert_True(CallCount == 0, "OS_TaskMatch_Impl() count (%lu) == 0", (unsigned long)CallCount);

    CallCount = UT_GetStubCount(UT_KEY(OS_TaskRegister_Impl));
    UtAssert_True(CallCount == 0, "OS_TaskRegister_Impl() count (%lu) == 0", (unsigned long)CallCount);

    OS_global_task_table[1].active_id       = UT_OBJID_1;
    OS_task_table[1].entry_function_pointer = UT_TestHook;

    OS_TaskEntryPoint(UT_OBJID_1);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1", (unsigned long)UT_TestHook_Count);
    CallCount = UT_GetStubCount(UT_KEY(OS_TaskMatch_Impl));
    UtAssert_True(CallCount == 1, "OS_TaskMatch_Impl() count (%lu) == 1", (unsigned long)CallCount);

    CallCount = UT_GetStubCount(UT_KEY(OS_TaskRegister_Impl));
    UtAssert_True(CallCount == 1, "OS_TaskRegister_Impl() count (%lu) == 1", (unsigned long)CallCount);

    OS_global_task_table[1].active_id       = OS_OBJECT_ID_UNDEFINED;
    OS_task_table[1].entry_function_pointer = NULL;
}

void Test_OS_TaskAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TaskAPI_Init();

    UtAssert_True(actual == expected, "OS_TaskAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

/*
**********************************************************************************
**          TASK API TEST CASES
**********************************************************************************
*/

void Test_OS_TaskCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskCreate (uint32 *task_id, const char *task_name, osal_task_entry function_pointer,
     *                uint32 *stack_pointer, uint32 stack_size, uint32 priority, uint32 flags)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual;

    actual = OS_TaskCreate(&objid, "UT", UT_TestHook, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(128), OSAL_PRIORITY_C(0), 0);

    UtAssert_True(actual == expected, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(
        OS_TaskCreate(NULL, NULL, NULL, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(0), OSAL_PRIORITY_C(0), 0),
        OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(
        OS_TaskCreate(&objid, "UT", UT_TestHook, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(0), OSAL_PRIORITY_C(0), 0),
        OS_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(OCS_strlen), 10 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(
        OS_TaskCreate(&objid, "UT", UT_TestHook, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(128), OSAL_PRIORITY_C(0), 0),
        OS_ERR_NAME_TOO_LONG);
}

void Test_OS_TaskDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelete (uint32 task_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    UT_TestHook_Count                    = 0;
    OS_task_table[1].delete_hook_pointer = UT_TestHook;
    actual                               = OS_TaskDelete(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_TaskDelete() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1", (unsigned long)UT_TestHook_Count);

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskDelete_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelete(UT_OBJID_1), OS_ERROR);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1", (unsigned long)UT_TestHook_Count);

    OS_task_table[1].delete_hook_pointer = NULL;
}

void Test_OS_TaskExit(void)
{
    /*
     * Test Case For:
     * void OS_TaskExit()
     */

    OS_TaskExit();

    /* TaskExit should have called OS_ObjectIdFinalizeDelete to clear the active_id */
    UtAssert_STUB_COUNT(OS_ObjectIdFinalizeDelete, 1);
}
void Test_OS_TaskDelay(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelay(uint32 millisecond)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TaskDelay(1);

    UtAssert_True(actual == expected, "OS_TaskDelay() (%ld) == OS_SUCCESS", (long)actual);
}
void Test_OS_TaskSetPriority(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TaskSetPriority(UT_OBJID_1, OSAL_PRIORITY_C(1));

    UtAssert_True(actual == expected, "OS_TaskSetPriority() (%ld) == OS_SUCCESS", (long)actual);
}
void Test_OS_TaskRegister(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskRegister (void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TaskRegister();

    UtAssert_True(actual == expected, "OS_TaskRegister() (%ld) == OS_SUCCESS", (long)actual);
}
void Test_OS_TaskGetId(void)
{
    /*
     * Test Case For:
     * uint32 OS_TaskGetId (void)
     */
    UT_idbuf_t idbuf;
    osal_id_t  objid;

    idbuf.val = 5555;
    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), idbuf.val);
    objid = OS_TaskGetId();
    OSAPI_TEST_OBJID(objid, ==, idbuf.id);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    objid = OS_TaskGetId();
    OSAPI_TEST_OBJID(objid, ==, OS_OBJECT_ID_UNDEFINED);
}

void Test_OS_TaskGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid    = OS_OBJECT_ID_UNDEFINED;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_TaskGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_TaskGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_TaskGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TaskGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_TaskGetIdByName(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_TaskGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)
     */
    int32          expected = OS_SUCCESS;
    int32          actual   = ~OS_SUCCESS;
    OS_task_prop_t task_prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_TASK, UT_INDEX_1, "ABC", UT_OBJID_OTHER);
    OS_task_table[1].stack_size = OSAL_SIZE_C(222);
    OS_task_table[1].priority   = OSAL_PRIORITY_C(133);

    actual = OS_TaskGetInfo(UT_OBJID_1, &task_prop);

    UtAssert_True(actual == expected, "OS_TaskGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(task_prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(task_prop.name, "ABC") == 0, "task_prop.name (%s) == ABC", task_prop.name);
    UtAssert_True(task_prop.stack_size == 222, "task_prop.stack_size (%lu) == 222", (unsigned long)task_prop.stack_size);
    UtAssert_True(task_prop.priority == 133, "task_prop.priority (%lu) == 133", (unsigned long)task_prop.priority);

    OS_task_table[1].stack_size = OSAL_SIZE_C(0);
    OS_task_table[1].priority   = OSAL_PRIORITY_C(0);

    OSAPI_TEST_FUNCTION_RC(OS_TaskGetInfo(OS_OBJECT_ID_UNDEFINED, NULL), OS_INVALID_POINTER);
}

void Test_OS_TaskInstallDeleteHandler(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1);
    actual = OS_TaskInstallDeleteHandler(UT_TestHook);
    UtAssert_True(actual == expected, "OS_TaskInstallDeleteHandler() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_task_table[1].delete_hook_pointer == UT_TestHook,
                  "OS_task_table[1].delete_hook_pointer (%lx) == %lx",
                  (unsigned long)OS_task_table[1].delete_hook_pointer, (unsigned long)UT_TestHook);

    OS_task_table[1].delete_hook_pointer = NULL;
}

void Test_OS_TaskFindIdBySystemData(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskFindIdBySystemData(uint32 *task_id, const void *sysdata, size_t sysdata_size)
     */

    int32     expected;
    int32     actual;
    osal_id_t task_id;

    /*
     * Use a compound data struct for the system data.
     * The intent is to intentionally make something bigger that will not fit into e.g. "uint32"
     */
    struct
    {
        unsigned long v;
        void *        p;
    } test_sysdata;

    memset(&test_sysdata, 'x', sizeof(test_sysdata));

    expected = OS_SUCCESS;
    actual   = OS_TaskFindIdBySystemData(&task_id, &test_sysdata, sizeof(test_sysdata));
    UtAssert_True(actual == expected, "OS_TaskFindIdBySystemData() (%ld) == OS_SUCCESS", (long)actual);

    /* Test parameter validation branches */
    expected = OS_INVALID_POINTER;
    actual   = OS_TaskFindIdBySystemData(NULL, &test_sysdata, sizeof(test_sysdata));
    UtAssert_True(actual == expected, "OS_TaskFindIdBySystemData() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskValidateSystemData_Impl), expected);
    actual = OS_TaskFindIdBySystemData(&task_id, &test_sysdata, sizeof(test_sysdata));
    UtAssert_True(actual == expected, "OS_TaskFindIdBySystemData() (%ld) == OS_INVALID_POINTER", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TaskValidateSystemData_Impl));

    /* Test search failure */
    expected = OS_ERR_NAME_NOT_FOUND;
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch), expected);
    actual = OS_TaskFindIdBySystemData(&task_id, &test_sysdata, sizeof(test_sysdata));
    UtAssert_True(actual == expected, "OS_TaskFindIdBySystemData() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdGetBySearch));
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
    ADD_TEST(OS_TaskAPI_Init);
    ADD_TEST(OS_TaskEntryPoint);
    ADD_TEST(OS_TaskCreate);
    ADD_TEST(OS_TaskDelete);
    ADD_TEST(OS_TaskExit);
    ADD_TEST(OS_TaskDelay);
    ADD_TEST(OS_TaskSetPriority);
    ADD_TEST(OS_TaskRegister);
    ADD_TEST(OS_TaskGetId);
    ADD_TEST(OS_TaskGetIdByName);
    ADD_TEST(OS_TaskGetInfo);
    ADD_TEST(OS_TaskInstallDeleteHandler);
    ADD_TEST(OS_TaskFindIdBySystemData);
}
