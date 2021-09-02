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
** Semaphore Speed Test
**
** This is a simple way to gauge the performance of the
** OS/kernel task switching and semaphore implementations
** on a given machine.
**
** This implements a flip-flop between two tasks using
** two semaphores.  Task 1 waits for semaphore 1 and
** gives semaphore 2, while task 2 waits for semaphore 2
** and gives semaphore 1.
**
** The two tasks will run continuously, each pending
** on each other and swapping between them, for 5 seconds.
**
** At the end of the test, the total number of "work"
** cycles for each task is indicated.  Higher numbers
** indicate better performance.
**
**
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/*
 * Note the worker priority must be lower than that of
 * the executive (init) task.  Otherwise, the SemRun()
 * function may never get CPU time to stop the test.
 */
#define SEMTEST_TASK_PRIORITY 150

/*
 * A limit for the maximum amount
 * of iterations that this test will
 * perform.  This prevents the test
 * from running infinitely in case the
 * time-based stop routine does not
 * work correctly.  See note above
 * about priority requirements.
 */
#define SEMTEST_WORK_LIMIT 10000000

/* Define setup and test functions for UT assert */
void SemSetup(void);
void SemRun(void);

/*
 * A macro for semaphore operation depending on type.
 *
 * This resolves to a function name like e.g. OS_BinSemTake.
 */
#define SEMCALL(type, op) OS_##type##Sem##op

/*
 * This test works with either binary or counting
 * semaphores.  To switch between them, set this
 * to either "Bin" or "Count" without quotes.
 */
#define SEMOP(op) SEMCALL(Count, op)

osal_id_t task_1_id;
uint32    task_1_work;

osal_id_t task_2_id;
uint32    task_2_work;

osal_id_t sem_id_1;
osal_id_t sem_id_2;

void task_1(void)
{
    uint32 status;

    OS_printf("Starting task 1\n");

    while (task_1_work < SEMTEST_WORK_LIMIT)
    {
        status = SEMOP(Take)(sem_id_1);
        if (status != OS_SUCCESS)
        {
            OS_printf("TASK 1: Error calling SemTake 1: %d\n", (int)status);
            break;
        }

        ++task_1_work;

        status = SEMOP(Give)(sem_id_2);
        if (status != OS_SUCCESS)
        {
            OS_printf("TASK 1: Error calling SemGive 2: %d\n", (int)status);
            break;
        }
    }
}

void task_2(void)
{
    uint32 status;

    OS_printf("Starting task 2\n");

    while (task_2_work < SEMTEST_WORK_LIMIT)
    {
        status = SEMOP(Take)(sem_id_2);
        if (status != OS_SUCCESS)
        {
            OS_printf("TASK 2: Error calling SemTake 2: %d\n", (int)status);
            break;
        }

        ++task_2_work;

        status = SEMOP(Give)(sem_id_1);
        if (status != OS_SUCCESS)
        {
            OS_printf("TASK 2: Error calling SemGive 1: %d\n", (int)status);
            break;
        }
    }
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
    UtTest_Add(SemRun, SemSetup, NULL, "SemSpeedTest");
}

void SemSetup(void)
{
    uint32 status;

    task_1_work = 0;
    task_2_work = 0;

    /*
    ** Create the Bin semaphore
    */
    status = SEMOP(Create)(&sem_id_1, "Sem1", 0, 0);
    UtAssert_True(status == OS_SUCCESS, "Sem 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(sem_id_1), (int)status);
    status = SEMOP(Create)(&sem_id_2, "Sem2", 0, 0);
    UtAssert_True(status == OS_SUCCESS, "Sem 2 create Id=%lx Rc=%d", OS_ObjectIdToInteger(sem_id_2), (int)status);

    /*
    ** Create the tasks
    */
    status = OS_TaskCreate(&task_1_id, "Task 1", task_1, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(4096),
                           OSAL_PRIORITY_C(SEMTEST_TASK_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_1_id), (int)status);

    status = OS_TaskCreate(&task_2_id, "Task 2", task_2, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(4096),
                           OSAL_PRIORITY_C(SEMTEST_TASK_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_2_id), (int)status);

    /* A small delay just to allow the tasks
     * to start and pend on the sem */
    OS_TaskDelay(10);
}

void SemRun(void)
{
    int32 status;

    /* Give the initial sem that starts the loop */
    SEMOP(Give)(sem_id_1);

    /* Time Limited Execution */
    OS_TaskDelay(5000);

    /*
    ** Delete resources
    **
    ** NOTE: if the work limit was reached, the
    ** OS_TaskDelete calls may return non-success.
    */
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);

    status = OS_TaskDelete(task_2_id);
    UtAssert_True(status == OS_SUCCESS, "Task 2 delete Rc=%d", (int)status);

    status = SEMOP(Delete)(sem_id_1);
    UtAssert_True(status == OS_SUCCESS, "Sem 1 delete Rc=%d", (int)status);
    status = SEMOP(Delete)(sem_id_2);
    UtAssert_True(status == OS_SUCCESS, "Sem 2 delete Rc=%d", (int)status);

    /* Task 1 and 2 should have both executed */
    UtAssert_True(task_1_work != 0, "Task 1 work counter = %u", (unsigned int)task_1_work);
    UtAssert_True(task_2_work != 0, "Task 2 work counter = %u", (unsigned int)task_2_work);
}
