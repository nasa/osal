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

/*
 * Filename: idmap-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-idmap"
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

osal_id_t task_id;
osal_id_t queue_id;
osal_id_t count_sem_id;
osal_id_t bin_sem_id;
osal_id_t mutex_id1;
osal_id_t mutex_id2;
osal_id_t mutex_id3;
osal_id_t time_base_id;
osal_id_t badid;

#define UT_EXIT_LOOP_MAX 100

/* *************************************** MAIN ************************************** */

typedef struct
{
    uint32 TaskCount;
    uint32 QueueCount;
    uint32 CountSemCount;
    uint32 BinSemCount;
    uint32 MutexCount;
    uint32 TimeBaseCount;
    uint32 OtherCount;
} Test_OS_ObjTypeCount_t;

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
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            ++count->CountSemCount;
            break;
        case OS_OBJECT_TYPE_OS_BINSEM:
            ++count->BinSemCount;
            break;
        case OS_OBJECT_TYPE_OS_MUTEX:
            ++count->MutexCount;
            break;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            ++count->TimeBaseCount;
            break;
        default:
            ++count->OtherCount;
            break;
    }
}

/*
 * A void test function that creates an object for testing
 */
void Test_Void_Fn(void)
{
    osal_id_t bin_sem_id_my_task;
    OS_BinSemCreate(&bin_sem_id_my_task, "BinSemTaskMyTask", 1, 0);
    OS_TaskDelay(5);
}

void TestIdMapApi_Setup(void)
{
    uint32         loopcnt;
    int32          status;
    OS_task_prop_t taskprop;

    /*
     * Create all allowed objects
     */
    status = OS_TaskCreate(&task_id, "Task", Test_Void_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(4096),
                           OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_QueueCreate(&queue_id, "Queue", OSAL_BLOCKCOUNT_C(5), OSAL_SIZE_C(5), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_CountSemCreate(&count_sem_id, "CountSem", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_CountSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_BinSemCreate(&bin_sem_id, "BinSem", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate(&mutex_id1, "Mutex1", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate(&mutex_id2, "Mutex2", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate(&mutex_id3, "Mutex3", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_TimeBaseCreate(&time_base_id, "TimeBase", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Looping delay in parent task to wait for child task to exit */
    loopcnt = 0;
    while ((OS_TaskGetInfo(task_id, &taskprop) == OS_SUCCESS) && (loopcnt < UT_EXIT_LOOP_MAX))
    {
        OS_TaskDelay(10);
        loopcnt++;
    }
    UtAssert_True(loopcnt < UT_EXIT_LOOP_MAX, "Task exited after %ld iterations", (long)loopcnt);

    /*
     * manufacture a "bad" ID value which is neither valid
     * nor equivalent to OS_OBJECT_ID_UNDEFINED
     *
     * This can be created by flipping the bits of a valid ID.
     */
    badid = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(mutex_id2) ^ 0xFFFFFFFF);
}
/* *************************************** MAIN ************************************** */

void Test_OS_IdentifyObject(void)
{
    /*
     * Test Case For:
     * int32 OS_IdentifyObject(void)
     */

    /*
     * NOTE: The following objects were not created and tested:
     * OS_OBJECT_TYPE_OS_STREAM
     * OS_OBJECT_TYPE_OS_DIR
     * OS_OBJECT_TYPE_OS_TIMECB
     * OS_OBJECT_TYPE_OS_FILESYS
     * OS_OBJECT_TYPE_OS_CONSOLE
     * OS_OBJECT_TYPE_USER
     */

    /*
     * Test with nominal values
     */
    UtAssert_UINT32_EQ(OS_IdentifyObject(task_id), OS_OBJECT_TYPE_OS_TASK);
    UtAssert_UINT32_EQ(OS_IdentifyObject(queue_id), OS_OBJECT_TYPE_OS_QUEUE);
    UtAssert_UINT32_EQ(OS_IdentifyObject(count_sem_id), OS_OBJECT_TYPE_OS_COUNTSEM);
    UtAssert_UINT32_EQ(OS_IdentifyObject(bin_sem_id), OS_OBJECT_TYPE_OS_BINSEM);
    UtAssert_UINT32_EQ(OS_IdentifyObject(mutex_id1), OS_OBJECT_TYPE_OS_MUTEX);
    UtAssert_UINT32_EQ(OS_IdentifyObject(time_base_id), OS_OBJECT_TYPE_OS_TIMEBASE);
    UtAssert_UINT32_EQ(OS_IdentifyObject(OS_OBJECT_ID_UNDEFINED), OS_OBJECT_TYPE_UNDEFINED);

    /*
     * Test with extreme cases using min and max values
     * Note: There are no asserts, checks or expected values
     * here.  The only check is that the function doesn't return
     * an error when called
     */
    OS_IdentifyObject(badid);
}

void Test_OS_ConvertToArrayIndex(void)
{
    /*
     * Test Case For:
     * int32 OS_ConvertToArrayIndex(void)
     */
    osal_index_t TestArrayIndex  = OSAL_INDEX_C(0);
    osal_index_t TestMutex1Index = OSAL_INDEX_C(0);
    osal_index_t TestMutex2Index = OSAL_INDEX_C(0);

    /*
     * Check different id types and verify array indices
     * Each Object Type index is added to an array index of its own type
     * Each object type is checked once, and MUTEX is checked twice to
     * verify multiple indices
     */

    /*
     * Test with nominal values
     */
    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(task_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_TASKS, "TestArrayIndex(%lu) < OS_MAX_TASKS", (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(queue_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_QUEUES, "TestArrayIndex(%lu) < OS_MAX_QUEUES", (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(count_sem_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_COUNT_SEMAPHORES, "TestArrayIndex(%lu) < OS_MAX_COUNT_SEMAPHORES",
                  (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(bin_sem_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_BIN_SEMAPHORES, "TestArrayIndex(%lu) < OS_MAX_BIN_SEMAPHORES",
                  (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(mutex_id1, &TestMutex1Index), OS_SUCCESS);
    UtAssert_True(TestMutex1Index < OS_MAX_MUTEXES, "TestMutex1Index(%lu) < OS_MAX_MUTEXES", (long)TestMutex1Index);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(mutex_id2, &TestMutex2Index), OS_SUCCESS);
    UtAssert_True(TestMutex2Index < OS_MAX_MUTEXES, "TestMutex2Index(%lu) < OS_MAX_MUTEXES", (long)TestMutex2Index);
    UtAssert_True(TestMutex1Index != TestMutex2Index, "TestMutex1Index(%lu) !=  TestMutex2Index(%lu)",
                  (long)TestMutex1Index, (long)TestMutex2Index);

    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(time_base_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_TIMEBASES, "TestArrayIndex(%lu) < OS_MAX_TIMEBASES", (long)TestArrayIndex);

    /*
     * Test with extreme cases using invalid inputs and checking
     * for an error return code
     */
    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(OS_OBJECT_ID_UNDEFINED, &TestArrayIndex), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(badid, &TestArrayIndex), OS_ERR_INVALID_ID);

    /* Check with null pointer */
    UtAssert_INT32_EQ(OS_ConvertToArrayIndex(bin_sem_id, NULL), OS_INVALID_POINTER);
}

void Test_OS_ObjectIdToArrayIndex(void)
{
    /*
     * Test case for:
     * int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t object_id, osal_index_t *ArrayIndex);
     */
    osal_index_t TestArrayIndex  = OSAL_INDEX_C(0);
    osal_index_t TestMutex1Index = OSAL_INDEX_C(0);
    osal_index_t TestMutex2Index = OSAL_INDEX_C(0);

    /* Test with nominal (correct) object types */
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, task_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_TASKS, "TestArrayIndex(%lu) < OS_MAX_TASKS", (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_QUEUE, queue_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_QUEUES, "TestArrayIndex(%lu) < OS_MAX_QUEUES", (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_COUNTSEM, count_sem_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_COUNT_SEMAPHORES, "TestArrayIndex(%lu) < OS_MAX_COUNT_SEMAPHORES",
                  (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_BINSEM, bin_sem_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_BIN_SEMAPHORES, "TestArrayIndex(%lu) < OS_MAX_BIN_SEMAPHORES",
                  (long)TestArrayIndex);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_MUTEX, mutex_id1, &TestMutex1Index), OS_SUCCESS);
    UtAssert_True(TestMutex1Index < OS_MAX_MUTEXES, "TestMutex1Index(%lu) < OS_MAX_MUTEXES", (long)TestMutex1Index);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_MUTEX, mutex_id2, &TestMutex2Index), OS_SUCCESS);
    UtAssert_True(TestMutex2Index < OS_MAX_MUTEXES, "TestMutex2Index(%lu) < OS_MAX_MUTEXES", (long)TestMutex2Index);
    UtAssert_True(TestMutex1Index != TestMutex2Index, "TestMutex1Index(%lu) !=  TestMutex2Index(%lu)",
                  (long)TestMutex1Index, (long)TestMutex2Index);

    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMEBASE, time_base_id, &TestArrayIndex), OS_SUCCESS);
    UtAssert_True(TestArrayIndex < OS_MAX_TIMEBASES, "TestArrayIndex(%lu) < OS_MAX_TIMEBASES", (long)TestArrayIndex);

    /* Check cases where the object type and the ID are _not_ matched */
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_QUEUE, task_id, &TestArrayIndex), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_COUNTSEM, queue_id, &TestArrayIndex),
                      OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_BINSEM, count_sem_id, &TestArrayIndex),
                      OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_MUTEX, bin_sem_id, &TestArrayIndex), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMEBASE, mutex_id2, &TestMutex2Index),
                      OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, time_base_id, &TestArrayIndex),
                      OS_ERR_INVALID_ID);

    /* Check with bad object IDs */
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, OS_OBJECT_ID_UNDEFINED, &TestArrayIndex),
                      OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_MUTEX, badid, &TestArrayIndex), OS_ERR_INVALID_ID);

    /* Check with null pointer */
    UtAssert_INT32_EQ(OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_BINSEM, bin_sem_id, NULL), OS_INVALID_POINTER);
}

void Test_OS_ForEachObject(void)
{
    /*
     * Test Case For:
     * void OS_ForEachObject (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);
     */
    Test_OS_ObjTypeCount_t Count;

    memset(&Count, 0, sizeof(Count));

    OS_ForEachObject(OS_OBJECT_CREATOR_ANY, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount (%lu) == 0", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 1, "OS_ForEachObject() QueueCount (%lu) == 1", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 1, "OS_ForEachObject() CountSemCount (%lu) == 1",
                  (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 2, "OS_ForEachObject() BinSemCount (%lu) == 2",
                  (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 3, "OS_ForEachObject() MutexCount (%lu) == 3", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 1, "OS_ForEachObject() TimeBaseCount (%lu) == 1",
                  (unsigned long)Count.TimeBaseCount);

    /* OS_ForEachObjectOfType() is similar but only iterates one type */
    memset(&Count, 0, sizeof(Count));
    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_MUTEX, OS_OBJECT_CREATOR_ANY, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount (%lu) == 0", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 0, "OS_ForEachObject() QueueCount (%lu) == 1", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 0, "OS_ForEachObject() CountSemCount (%lu) == 1",
                  (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 0, "OS_ForEachObject() BinSemCount (%lu) == 2",
                  (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 3, "OS_ForEachObject() MutexCount (%lu) == 3", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 0, "OS_ForEachObject() TimeBaseCount (%lu) == 1",
                  (unsigned long)Count.TimeBaseCount);

    /*
     * Pass an invalid input, and verify that object counts are not increased
     */
    memset(&Count, 0, sizeof(Count));
    OS_ForEachObject(badid, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 0, "OS_ForEachObject() QueueCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 0, "OS_ForEachObject() CountSemCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 0, "OS_ForEachObject() BinSemCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 0, "OS_ForEachObject() MutexCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 0, "OS_ForEachObject() TimeBaseCount Invalid Input (%lu) == 0",
                  (unsigned long)Count.TimeBaseCount);

    /*
     * Use current task as an input
     */
    memset(&Count, 0, sizeof(Count));
    OS_ForEachObject(task_id, &ObjTypeCounter, &Count);

    /* Verify Output */
    UtAssert_True(Count.BinSemCount == 1, "OS_ForEachObject() BinSemCount MyTask (%lu) == 1",
                  (unsigned long)Count.BinSemCount);

    /*
     * Delete all created objects, and verify that the count is now zero
     */
    memset(&Count, 0, sizeof(Count));
    OS_DeleteAllObjects();
    OS_ForEachObject(OS_OBJECT_CREATOR_ANY, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount After Delete (%lu) == 0",
                  (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 0, "OS_ForEachObject() QueueCount After Delete (%lu) == 0",
                  (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 0, "OS_ForEachObject() CountSemCount After Delete (%lu) == 0",
                  (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 0, "OS_ForEachObject() BinSemCount After Delete (%lu) == 0",
                  (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 0, "OS_ForEachObject() MutexCount After Delete (%lu) == 0",
                  (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 0, "OS_ForEachObject() TimeBaseCount After Delete (%lu) == 0",
                  (unsigned long)Count.TimeBaseCount);
}

void Test_OS_GetResourceName(void)
{
    /*
     * Test case for:
     * int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size)
     */
    char name[OS_MAX_API_NAME];

    memset(name, 0, sizeof(name));

    /* Nominal cases */
    UtAssert_INT32_EQ(OS_GetResourceName(queue_id, name, sizeof(name)), OS_SUCCESS);
    UtAssert_StrCmp(name, "Queue", "%s == %s", name, "Queue");
    UtAssert_INT32_EQ(OS_GetResourceName(count_sem_id, name, sizeof(name)), OS_SUCCESS);
    UtAssert_StrCmp(name, "CountSem", "%s == %s", name, "CountSem");
    UtAssert_INT32_EQ(OS_GetResourceName(bin_sem_id, name, sizeof(name)), OS_SUCCESS);
    UtAssert_StrCmp(name, "BinSem", "%s == %s", name, "BinSem");
    UtAssert_INT32_EQ(OS_GetResourceName(mutex_id1, name, sizeof(name)), OS_SUCCESS);
    UtAssert_StrCmp(name, "Mutex1", "%s == %s", name, "Mutex1");
    UtAssert_INT32_EQ(OS_GetResourceName(time_base_id, name, sizeof(name)), OS_SUCCESS);
    UtAssert_StrCmp(name, "TimeBase", "%s == %s", name, "TimeBase");

    /* Error cases */
    /* note the task has exited, so the task ID is no longer valid */
    UtAssert_INT32_EQ(OS_GetResourceName(task_id, name, sizeof(name)), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_GetResourceName(badid, name, sizeof(name)), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_GetResourceName(OS_OBJECT_ID_UNDEFINED, name, sizeof(name)), OS_ERR_INVALID_ID);

    UtAssert_INT32_EQ(OS_GetResourceName(time_base_id, name, 2), OS_ERR_NAME_TOO_LONG);
    UtAssert_StrCmp(name, "T", "%s == %s", name, "T");

    UtAssert_INT32_EQ(OS_GetResourceName(queue_id, NULL, sizeof(name)), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_GetResourceName(queue_id, name, 0), OS_ERR_INVALID_SIZE);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(NULL, TestIdMapApi_Setup, NULL, "TestIdMapApi");
    UtTest_Add(Test_OS_IdentifyObject, NULL, NULL, "OS_IdentifyObject");
    UtTest_Add(Test_OS_ConvertToArrayIndex, NULL, NULL, "OS_ConvertToArrayIndex");
    UtTest_Add(Test_OS_ObjectIdToArrayIndex, NULL, NULL, "OS_ObjectIdToArrayIndex");
    UtTest_Add(Test_OS_GetResourceName, NULL, NULL, "OS_GetResourceName");
    UtTest_Add(Test_OS_ForEachObject, NULL, NULL, "OS_ForEachObject");
}
