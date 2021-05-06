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
** Binary semaphore Producer/Consumer test ( test the timeout feature )
*/
#include <stdio.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void BinSemTimeoutSetup(void);
void BinSemTimeoutCheck(void);

/* Task 1 */
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101
#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   50

uint32    task_1_stack[TASK_1_STACK_SIZE];
osal_id_t task_1_id;
uint32    task_1_timeouts;
uint32    task_1_work;
uint32    task_1_failures;
uint32    task_2_stack[TASK_2_STACK_SIZE];
osal_id_t task_2_id;

osal_id_t bin_sem_id;

uint32    timer_counter;
osal_id_t timer_id;
uint32    timer_start    = 1000000;
uint32    timer_interval = 2000000; /* 2 second period */
uint32    timer_accuracy;
uint32    timer_function_failures;

int counter = 0;

/*
 * Note that we should not call "printf" or anything
 * like that during a timer callback routine (may be ISR context)
 *
 * On RTEMS even a call to BinSemGetInfo has very ill effects.
 */
void TimerFunction(osal_id_t local_timer_id)
{
    int32 status;

    timer_counter++;

    status = OS_BinSemGive(bin_sem_id);
    if (status != OS_SUCCESS)
    {
        ++timer_function_failures;
    }

    {
        OS_bin_sem_prop_t bin_sem_prop;
        status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
        if (status != OS_SUCCESS)
        {
            ++timer_function_failures;
        }
        else if (bin_sem_prop.value > 1)
        {
            ++timer_function_failures;
        }
        else if (bin_sem_prop.value < -1)
        {
            ++timer_function_failures;
        }
    }
}

void task_1(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;

    OS_printf("Starting task 1\n");

    OS_printf("Delay for 1 second before starting\n");
    OS_TaskDelay(1000);

    while (1)
    {
        OS_printf("TASK 1: Waiting on the semaphore\n");

        status = OS_BinSemTimedWait(bin_sem_id, 1000);
        if (status == OS_SUCCESS)
        {
            OS_printf("TASK 1:   Doing some work: %d\n", (int)counter++);
            status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
            if (status != OS_SUCCESS)
            {
                OS_printf("Error: OS_BinSemGetInfo\n");
                ++task_1_failures;
            }
            else if (bin_sem_prop.value > 1)
            {
                OS_printf("Error: Binary sem value > 1 ( in task):%d !\n", (int)bin_sem_prop.value);
                ++task_1_failures;
            }
            else if (bin_sem_prop.value < -1)
            {
                OS_printf("Error: Binary sem value < -1 ( in task):%d !\n", (int)bin_sem_prop.value);
                ++task_1_failures;
            }
        }
        else if (status == OS_SEM_TIMEOUT)
        {
            OS_printf("TASK 1:Timeout on OS_BinSemTimedWait\n");
            ++task_1_timeouts;
            OS_TaskDelay(500);
        }
        else
        {
            ++task_1_failures;
            OS_printf("TASK 1:Error calling OS_BinSemTimedWait\n");
        }
    }
}

void BinSemTimeoutCheck(void)
{
    int32  status;
    uint32 limit;

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TimerDelete Rc=%d", (int)status);

    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskDelete Rc=%d", (int)status);

    UtAssert_True(counter == timer_counter, "Task counter (%d) == timer counter (%d)", (int)counter,
                  (int)timer_counter);
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u", (unsigned int)task_1_failures);
    UtAssert_True(timer_function_failures == 0, "Timer function failures = %u", (unsigned int)timer_function_failures);

    /*
     * With the timers as configured, there should be approximately 1 task_1 timeout
     * per real tick.  Task 1 Sequence is:
     *   Sem Take (success after ??? ms) --> Sem Take (timeout after 1000ms) --> 500ms delay -->
     *   Sem Take (success after ~500ms) --> repeat
     *   And Repeat...
     */
    limit = counter / 2;
    UtAssert_True(task_1_timeouts >= limit, "Task 1 timeouts=%u >= %u", (unsigned int)task_1_timeouts,
                  (unsigned int)limit);
    limit = counter * 2;
    UtAssert_True(task_1_timeouts <= limit, "Task 1 timeouts=%u <= %u", (unsigned int)task_1_timeouts,
                  (unsigned int)limit);
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
    UtTest_Add(BinSemTimeoutCheck, BinSemTimeoutSetup, NULL, "BinSemTimeoutTest");
}

void BinSemTimeoutSetup(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;
    uint32            accuracy;

    task_1_timeouts         = 0;
    task_1_work             = 0;
    task_1_failures         = 0;
    timer_function_failures = 0;

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
    ** Create the "consumer" task.
    */
    status = OS_TaskCreate(&task_1_id, "Task 1", task_1, OSAL_STACKPTR_C(task_1_stack), sizeof(task_1_stack),
                           OSAL_PRIORITY_C(TASK_1_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_1_id), (int)status);

    /*
    ** Create a timer
    */
    status = OS_TimerCreate(&timer_id, "Timer 1", &accuracy, &(TimerFunction));
    UtAssert_True(status == OS_SUCCESS, "Timer 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(timer_id), (int)status);
    UtPrintf("Timer Accuracy = %u microseconds \n", (unsigned int)accuracy);

    /*
    ** Start the timer
    */
    status = OS_TimerSet(timer_id, timer_start, timer_interval);
    UtAssert_True(status == OS_SUCCESS, "Timer 1 set Rc=%d", (int)status);

    /*
     * Give tasks time to run
     */
    while (counter < 10)
    {
        OS_TaskDelay(100);
    }
}
