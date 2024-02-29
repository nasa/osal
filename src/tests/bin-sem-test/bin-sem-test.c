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
 * @file Binary semaphore test
 */
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"

#define TASK_STACK_SIZE 16384

uint32 task_counter[3];

void Test_BinSem_Task0(void)
{
    osal_id_t sem_id;
    uint8     i;

    UtAssert_INT32_EQ(OS_BinSemGetIdByName(&sem_id, "Test_Sem"), OS_SUCCESS);

    for (i = 0; i < 3; i++)
    {
        UtAssert_INT32_EQ(OS_BinSemTake(sem_id), OS_SUCCESS);
        task_counter[0]++;
    }
}

void Test_BinSem_Task1(void)
{
    osal_id_t sem_id;
    uint8     i;

    UtAssert_INT32_EQ(OS_BinSemGetIdByName(&sem_id, "Test_Sem"), OS_SUCCESS);

    for (i = 0; i < 3; i++)
    {
        UtAssert_INT32_EQ(OS_BinSemTake(sem_id), OS_SUCCESS);
        task_counter[1]++;
    }
}

void Test_BinSem_Task2(void)
{
    osal_id_t sem_id;
    uint8     i;

    UtAssert_INT32_EQ(OS_BinSemGetIdByName(&sem_id, "Test_Sem"), OS_SUCCESS);

    for (i = 0; i < 3; i++)
    {
        UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id, 1000), OS_SUCCESS);
        task_counter[2]++;
    }
}

void Test_BinSem(void)
{

    osal_id_t         sem_id[2];
    osal_id_t         task_id[3];
    char              long_name[OS_MAX_API_NAME + 1];
    OS_bin_sem_prop_t sem_prop;
    uint32            test_val;
    bool              get_info_implemented;

    memset(&sem_prop, 0, sizeof(sem_prop));
    memset(task_counter, 0, sizeof(task_counter));

    /* Invalid id checks */
    UtAssert_INT32_EQ(OS_BinSemGetInfo(OS_OBJECT_ID_UNDEFINED, &sem_prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_BinSemFlush(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_BinSemGive(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_BinSemTake(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_BinSemTimedWait(OS_OBJECT_ID_UNDEFINED, 0), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_BinSemDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /* Null checks */
    UtAssert_INT32_EQ(OS_BinSemCreate(NULL, "Test_Sem", 0, 0), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_BinSemCreate(&sem_id[0], NULL, 0, 0), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[0], NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_BinSemGetIdByName(NULL, "Test_Sem"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_BinSemGetIdByName(&sem_id[0], NULL), OS_INVALID_POINTER);

    /* Name too long */
    memset(long_name, 'a', sizeof(long_name));
    UtAssert_INT32_EQ(OS_BinSemCreate(&sem_id[0], long_name, 0, 0), OS_ERR_NAME_TOO_LONG);
    UtAssert_INT32_EQ(OS_BinSemGetIdByName(&sem_id[0], long_name), OS_ERR_NAME_TOO_LONG);

    /* Valid create and name taken */
    UtAssert_INT32_EQ(OS_BinSemCreate(&sem_id[0], "Test_Sem", 0, 0), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemCreate(&sem_id[1], "Test_Sem", 0, 0), OS_ERR_NAME_TAKEN);

    /* Nonzero create */
    UtAssert_INT32_EQ(OS_BinSemCreate(&sem_id[1], "Test_Sem_Nonzero", 1, 0), OS_SUCCESS);

    /* Check get info implementation */
    get_info_implemented = (OS_BinSemGetInfo(sem_id[0], &sem_prop) != OS_ERR_NOT_IMPLEMENTED);

    /* Validate values */
    if (get_info_implemented)
    {
        UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[0], &sem_prop), OS_SUCCESS);
        UtAssert_INT32_EQ(sem_prop.value, 0);
        UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[1], &sem_prop), OS_SUCCESS);
        UtAssert_INT32_EQ(sem_prop.value, 1);
    }

    /* Poll and wait timeouts on sem initialized to zero */
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 0), OS_SEM_TIMEOUT);
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 1), OS_SEM_TIMEOUT);

    /* Give twice and take and poll to confirm binary nature (second give response unspecified) */
    UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
    OS_BinSemGive(sem_id[0]);
    UtAssert_INT32_EQ(OS_BinSemTake(sem_id[0]), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 0), OS_SEM_TIMEOUT);

    /* Successful poll and timed wait */
    UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 0), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 1), OS_SUCCESS);

    /* Successful poll on sem initialized to nonzero */
    UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[1], 0), OS_SUCCESS);

    /* Validate zeros */
    if (get_info_implemented)
    {
        UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[0], &sem_prop), OS_SUCCESS);
        UtAssert_INT32_EQ(sem_prop.value, 0);
        UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[1], &sem_prop), OS_SUCCESS);
        UtAssert_INT32_EQ(sem_prop.value, 0);
    }
    else
    {
        UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 0), OS_SEM_TIMEOUT);
        UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[1], 0), OS_SEM_TIMEOUT);
    }

    /* Start child task, confirm waiting, delete task, and get sem info to confirm still valid (heritage test) */
    UtAssert_INT32_EQ(
        OS_TaskCreate(&task_id[0], "Task_0", Test_BinSem_Task0, NULL, TASK_STACK_SIZE, OSAL_PRIORITY_C(100), 0),
        OS_SUCCESS);
    OS_TaskDelay(100);
    UtAssert_INT32_EQ(OS_TaskDelete(task_id[0]), OS_SUCCESS);
    UtAssert_UINT32_EQ(task_counter[0], 0);

    if (get_info_implemented)
    {
        UtAssert_INT32_EQ(OS_BinSemGetInfo(sem_id[0], &sem_prop), OS_SUCCESS);
        UtAssert_INT32_EQ(sem_prop.value, 0);
    }
    else
    {
        UtAssert_INT32_EQ(OS_BinSemTimedWait(sem_id[0], 0), OS_SEM_TIMEOUT);
    }

    /* Start 3 child tasks, give and confirm highest priority task increments, flush and confirm all three */
    UtAssert_INT32_EQ(
        OS_TaskCreate(&task_id[0], "Task_0", Test_BinSem_Task0, NULL, TASK_STACK_SIZE, OSAL_PRIORITY_C(100), 0),
        OS_SUCCESS);
    UtAssert_INT32_EQ(
        OS_TaskCreate(&task_id[1], "Task_1", Test_BinSem_Task1, NULL, TASK_STACK_SIZE, OSAL_PRIORITY_C(200), 0),
        OS_SUCCESS);
    UtAssert_INT32_EQ(
        OS_TaskCreate(&task_id[2], "Task_2", Test_BinSem_Task2, NULL, TASK_STACK_SIZE, OSAL_PRIORITY_C(250), 0),
        OS_SUCCESS);
    OS_TaskDelay(100);
    UtAssert_UINT32_EQ(task_counter[0] + task_counter[1] + task_counter[2], 0);
    UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
    OS_TaskDelay(100);
    UtAssert_UINT32_EQ(task_counter[0], 1);
    UtAssert_UINT32_EQ(task_counter[1] + task_counter[2], 0);
    UtAssert_INT32_EQ(OS_BinSemFlush(sem_id[0]), OS_SUCCESS);
    OS_TaskDelay(100);
    UtAssert_UINT32_EQ(task_counter[0], 2);
    UtAssert_UINT32_EQ(task_counter[1], 1);
    UtAssert_UINT32_EQ(task_counter[2], 1);

    /* Give loop for tasks to complete */
    test_val = 4;
    while (test_val < 9)
    {
        test_val++;
        UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
        OS_TaskDelay(100);
        UtAssert_UINT32_EQ(task_counter[0] + task_counter[1] + task_counter[2], test_val);
    }

    /* One more give and confirm nothing increments */
    UtAssert_INT32_EQ(OS_BinSemGive(sem_id[0]), OS_SUCCESS);
    OS_TaskDelay(100);
    UtAssert_UINT32_EQ(task_counter[0] + task_counter[1] + task_counter[2], test_val);

    /* Confirm successful delete of zero and nonzero sem */
    UtAssert_INT32_EQ(OS_BinSemDelete(sem_id[0]), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemDelete(sem_id[1]), OS_SUCCESS);
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
    UtTest_Add(Test_BinSem, NULL, NULL, "Test_BinSem");
}
