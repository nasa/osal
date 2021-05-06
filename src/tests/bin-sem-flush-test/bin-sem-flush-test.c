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

/*
** Binary Semaphore Flush Test
*/
#include <stdio.h>
#include "common_types.h"

#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

void BinSemFlushSetup(void);
void BinSemFlushCheck(void);
void BinSemFlushTeardown(void);

#define TASK_STACK_SIZE 4096
#define TASK_1_PRIORITY 100
#define TASK_2_PRIORITY 110
#define TASK_3_PRIORITY 120

uint32    task_1_stack[TASK_STACK_SIZE];
osal_id_t task_1_id;
uint32    task_1_failures;
uint32    task_1_work;

uint32    task_2_stack[TASK_STACK_SIZE];
osal_id_t task_2_id;
uint32    task_2_failures;
uint32    task_2_work;

uint32    task_3_stack[TASK_STACK_SIZE];
osal_id_t task_3_id;
uint32    task_3_failures;
uint32    task_3_work;

osal_id_t bin_sem_id;

void task_1(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;
    int               counter = 0;

    OS_printf("Starting task 1\n");

    OS_printf("TASK 1: Waiting on the semaphore\n");
    status = OS_BinSemTake(bin_sem_id);
    if (status != OS_SUCCESS)
    {
        ++task_1_failures;
        OS_printf("TASK 1: Error calling OS_BinSemTake\n");
    }
    else
    {
        status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
        if (status != OS_SUCCESS)
        {
            ++task_1_failures;
            OS_printf("TASK 1: Error calling OS_BinSemGetInfo\n");
        }
        else
        {
            OS_printf("TASK 1: out of BinSemTake: %d\n", (int)bin_sem_prop.value);
        }
    }

    while (1)
    {
        ++task_1_work;
        OS_TaskDelay(100);
        OS_printf("TASK 1:   Doing some work: %d\n", (int)counter++);
    }
}

void task_2(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;
    int               counter = 0;

    task_2_failures = 0;
    OS_printf("Starting task 2\n");

    OS_printf("TASK 2: Waiting on the semaphore\n");
    status = OS_BinSemTake(bin_sem_id);
    if (status != OS_SUCCESS)
    {
        ++task_2_failures;
        OS_printf("TASK 2: Error calling OS_BinSemTake\n");
    }
    else
    {
        status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
        if (status != OS_SUCCESS)
        {
            ++task_2_failures;
            OS_printf("TASK 2: Error calling OS_BinSemGetInfo\n");
        }
        else
        {
            OS_printf("TASK 2: out of BinSemTake: %d\n", (int)bin_sem_prop.value);
        }
    }

    while (1)
    {
        ++task_2_work;
        OS_TaskDelay(100);
        OS_printf("TASK 2:   Doing some work: %d\n", (int)counter++);
    }
}

void task_3(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;
    int               counter = 0;

    OS_printf("Starting task 3\n");

    OS_printf("TASK 3: Waiting on the semaphore\n");
    status = OS_BinSemTake(bin_sem_id);
    if (status != OS_SUCCESS)
    {
        ++task_3_failures;
        OS_printf("TASK 3: Error calling OS_BinSemTake\n");
    }
    else
    {
        status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
        if (status != OS_SUCCESS)
        {
            ++task_3_failures;
            OS_printf("TASK 3: Error calling OS_BinSemGetInfo\n");
        }
        else
        {
            OS_printf("TASK 3: out of BinSemTake: %d\n", (int)bin_sem_prop.value);
        }
    }

    while (1)
    {
        ++task_3_work;
        OS_TaskDelay(100);
        OS_printf("TASK 3:   Doing some work: %d\n", (int)counter++);
    }
}

/*
** Main function
*/
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
    UtTest_Add(BinSemFlushCheck, BinSemFlushSetup, BinSemFlushTeardown, "BinSemFlushTest");
}

void BinSemFlushSetup(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;

    /* Note that UT assert is not multi-thread safe,
     * so each thread must use a separate error counter
     * and then we will assert that these remain zero
     */
    task_1_failures = 0;
    task_2_failures = 0;
    task_3_failures = 0;
    task_1_work     = 0;
    task_2_work     = 0;
    task_3_work     = 0;

    /*
    ** Create the binary semaphore
    */
    status = OS_BinSemCreate(&bin_sem_id, "BinSem1", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(bin_sem_id), (int)status);

    status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 value=%d Rc=%d", (int)bin_sem_prop.value, (int)status);

    /*
    ** Take the semaphore so the value is 0 and the next SemTake call should block
    */
    status = OS_BinSemTake(bin_sem_id);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 take Rc=%d", (int)status);
    status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 value=%d Rc=%d", (int)bin_sem_prop.value, (int)status);

    /*
    ** Create the tasks
    */
    status = OS_TaskCreate(&task_1_id, "Task 1", task_1, OSAL_STACKPTR_C(task_1_stack), sizeof(task_1_stack),
                           OSAL_PRIORITY_C(TASK_1_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_1_id), (int)status);

    status = OS_TaskCreate(&task_2_id, "Task 2", task_2, OSAL_STACKPTR_C(task_2_stack), sizeof(task_2_stack),
                           OSAL_PRIORITY_C(TASK_2_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_2_id), (int)status);

    status = OS_TaskCreate(&task_3_id, "Task 3", task_3, OSAL_STACKPTR_C(task_3_stack), sizeof(task_3_stack),
                           OSAL_PRIORITY_C(TASK_3_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 3 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_3_id), (int)status);

    /*
    ** Delay, then check the status
    */
    OS_TaskDelay(2000);
}

void BinSemFlushCheck(void)
{
    int32 status;

    /* At first, No task should have done any work yet (all blocked) */
    UtAssert_True(task_1_work == 0, "Task 1 work = %u", (unsigned int)task_1_work);
    UtAssert_True(task_2_work == 0, "Task 2 work = %u", (unsigned int)task_2_work);
    UtAssert_True(task_3_work == 0, "Task 3 work = %u", (unsigned int)task_3_work);

    status = OS_BinSemFlush(bin_sem_id);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 flush Rc=%d", (int)status);

    /* After more delay the work done should be nonzero on all tasks */
    /* NOTE - There is a slight race condition here as the task could be blocked
     * by something else other than the bin sem.
     */
    OS_TaskDelay(4000);

    UtAssert_True(task_1_work != 0, "Task 1 work = %u", (unsigned int)task_1_work);
    UtAssert_True(task_2_work != 0, "Task 2 work = %u", (unsigned int)task_2_work);
    UtAssert_True(task_3_work != 0, "Task 3 work = %u", (unsigned int)task_3_work);

    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u", (unsigned int)task_1_failures);
    UtAssert_True(task_2_failures == 0, "Task 2 failures = %u", (unsigned int)task_2_failures);
    UtAssert_True(task_3_failures == 0, "Task 3 failures = %u", (unsigned int)task_3_failures);
}

void BinSemFlushTeardown(void)
{
    int32 status;

    /*
    ** Delete the tasks
    */
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);

    status = OS_TaskDelete(task_2_id);
    UtAssert_True(status == OS_SUCCESS, "Task 2 delete Rc=%d", (int)status);

    status = OS_TaskDelete(task_3_id);
    UtAssert_True(status == OS_SUCCESS, "Task 3 delete Rc=%d", (int)status);
}
