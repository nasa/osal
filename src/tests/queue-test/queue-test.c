/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
** Queue read timeout test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void QueueTimeoutSetup(void);
void QueueTimeoutCheck(void);

#define MSGQ_DEPTH OS_QUEUE_MAX_DEPTH
#define MSGQ_SIZE  sizeof(uint32)
#define MSGQ_TOTAL 10
#define MSGQ_BURST 3
#define MSGQ_DELAY 400 /* post-burst inter-message time */
#define MSGQ_START 100 /* base value for data, to make it distinct from the msg counter */

/* Task 1 */
#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY   101

uint32    task_stack[TASK_STACK_SIZE];
osal_id_t task_id;
uint32    task_failures;
uint32    task_timeouts;
uint32    task_messages;
uint32    task_start_delay;

osal_id_t msgq_id;

uint32    timer_counter;
osal_id_t timer_id;
uint32    timer_start    = 10000;
uint32    timer_interval = 100000; /* 1000 = 1000 hz, 10000 == 100 hz */
uint32    timer_accuracy;

void TimerFunction(osal_id_t local_timer_id)
{
    timer_counter++;
}

void task_1(void)
{
    int32  status;
    size_t data_size;
    uint32 data_received = 0;
    uint32 expected      = MSGQ_START;

    OS_printf("TASK: Starting, Delay %ums\n", (unsigned int)task_start_delay);
    OS_TaskDelay(task_start_delay);

    OS_printf("TASK: queue reads begin\n");
    /* if errors occur do not loop endlessly */
    while (task_failures < 20)
    {
        status = OS_QueueGet(msgq_id, (void *)&data_received, OSAL_SIZE_C(MSGQ_SIZE), &data_size, 1000);

        if (status == OS_SUCCESS)
        {
            ++task_messages;
            UtAssert_True(data_received == expected,
                          "TASK: data_received (%u) == expected (%u)",
                          (unsigned int)data_received,
                          (unsigned int)expected);

            expected++;
        }
        else if (status == OS_QUEUE_TIMEOUT)
        {
            ++task_timeouts;
            OS_printf("TASK: Timeout on Queue! Timer counter = %u\n", (unsigned int)timer_counter);
        }
        else
        {
            ++task_failures;
            OS_printf("TASK: Queue Get error: %d!\n", (int)status);
            OS_TaskDelay(10);
        }
    }
}

void QueueTimeoutCheck(void)
{
    int32  status;
    uint32 limit;

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "Timer delete Rc=%d", (int)status);
    status = OS_TaskDelete(task_id);
    UtAssert_True(status == OS_SUCCESS, "Task delete Rc=%d", (int)status);
    status = OS_QueueDelete(msgq_id);
    UtAssert_True(status == OS_SUCCESS, "Queue delete Rc=%d", (int)status);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_failures == 0, "Task failures = %u", (unsigned int)task_failures);

    /*
     * Since nothing currently sends messages, message count should be zero,
     * and timer counter =~ 10 + ( 10 x task_timeouts )
     */
    UtAssert_True(task_messages == 0, "Task messages = %u", (unsigned int)task_messages);

    limit = (timer_counter / 10);
    UtAssert_True(task_timeouts <= limit, "Task timeouts %u <= %u", (unsigned int)task_timeouts, (unsigned int)limit);

    limit = ((timer_counter - 20) / 12);
    UtAssert_True(task_timeouts >= limit, "Task timeouts %u >= %u", (unsigned int)task_timeouts, (unsigned int)limit);
}

void QueueTimeoutSetup(void)
{
    int32  status;
    uint32 accuracy = 0;

    task_failures    = 0;
    task_messages    = 0;
    task_timeouts    = 0;
    task_start_delay = 1000;

    status = OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    UtAssert_True(status == OS_SUCCESS, "MsgQ create Id=%lx Rc=%d", OS_ObjectIdToInteger(msgq_id), (int)status);

    /*
    ** Create the "consumer" task.
    */
    status = OS_TaskCreate(&task_id,
                           "Task 1",
                           task_1,
                           OSAL_STACKPTR_C(task_stack),
                           sizeof(task_stack),
                           OSAL_PRIORITY_C(TASK_PRIORITY),
                           0);
    UtAssert_True(status == OS_SUCCESS, "Task create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_id), (int)status);

    /*
    ** Create a timer
    */
    status = OS_TimerCreate(&timer_id, "Timer", &accuracy, &(TimerFunction));
    UtAssert_True(status == OS_SUCCESS, "Timer create Id=%lx Rc=%d", OS_ObjectIdToInteger(timer_id), (int)status);
    UtPrintf("Timer Accuracy = %u microseconds \n", (unsigned int)accuracy);

    /*
    ** Start the timer
    */
    status = OS_TimerSet(timer_id, timer_start, timer_interval);
    UtAssert_True(status == OS_SUCCESS, "Timer set Rc=%d", (int)status);

    /* allow some time for task to run and accrue queue timeouts */
    while (timer_counter < 100)
    {
        OS_TaskDelay(100);
    }
}

void QueueMessageCheck(void)
{
    int32 status;

    OS_printf("Delay for half a second before checking\n");
    OS_TaskDelay(500);

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "Timer delete Rc=%d", (int)status);
    status = OS_TaskDelete(task_id);
    UtAssert_True(status == OS_SUCCESS, "Task delete Rc=%d", (int)status);
    status = OS_QueueDelete(msgq_id);
    UtAssert_True(status == OS_SUCCESS, "Queue delete Rc=%d", (int)status);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_failures == 0, "Task failures = %u", (unsigned int)task_failures);
    UtAssert_True(task_messages == 10, "Task messages = %u", (unsigned int)task_messages);
    UtAssert_True(task_timeouts == 0, "Task timeouts = %u", (unsigned int)task_timeouts);
}

void QueueMessageSetup(void)
{
    int32  status;
    uint32 accuracy = 0;
    int    i;
    uint32 Data = 0;

    task_failures = 0;
    task_messages = 0;
    task_timeouts = 0;

    /* Have the consumer task start emptying the queue part way through
     * the steady state (post-burst) write activity */
    task_start_delay = ((MSGQ_TOTAL - MSGQ_BURST) * MSGQ_DELAY) / 3;

    /* Sometimes if configs have been tuned it can be unclear what the active value is */
    OS_printf("Starting Test with DEPTH=%u, SIZE=%u, BURST=%u, DELAY=%ums\n",
              (unsigned int)MSGQ_DEPTH,
              (unsigned int)MSGQ_SIZE,
              (unsigned int)MSGQ_BURST,
              (unsigned int)MSGQ_DELAY);

    /*
     * Configuration check:
     * The timeouts in this test are tuned to work with MSGQ_TOTAL messages.
     * If the MSGQ_DEPTH is less than this, the test may fail in unexpected ways
     */
    UtAssert_UINT32_LTEQ(MSGQ_TOTAL, MSGQ_DEPTH);

    status = OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    UtAssert_True(status == OS_SUCCESS, "MsgQ create Id=%lx Rc=%d", OS_ObjectIdToInteger(msgq_id), (int)status);

    /*
    ** Create the "consumer" task.
    */
    status = OS_TaskCreate(&task_id,
                           "Task 1",
                           task_1,
                           OSAL_STACKPTR_C(task_stack),
                           sizeof(task_stack),
                           OSAL_PRIORITY_C(TASK_PRIORITY),
                           0);
    UtAssert_True(status == OS_SUCCESS, "Task create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_id), (int)status);

    /*
    ** Create a timer
    */
    status = OS_TimerCreate(&timer_id, "Timer", &accuracy, &(TimerFunction));
    UtAssert_True(status == OS_SUCCESS, "Timer create Id=%lx Rc=%d", OS_ObjectIdToInteger(timer_id), (int)status);
    UtPrintf("Timer Accuracy = %u microseconds \n", (unsigned int)accuracy);

    /*
    ** Start the timer
    */
    status = OS_TimerSet(timer_id, timer_start, timer_interval);
    UtAssert_True(status == OS_SUCCESS, "Timer set Rc=%d", (int)status);

    /*
     * Put 10 messages onto the que with some time in between the later messages
     * to make sure the que handles both storing and waiting for messages
     */
    for (i = 0; i < MSGQ_TOTAL; i++)
    {
        if (i > MSGQ_BURST)
        {
            OS_TaskDelay(MSGQ_DELAY);
        }

        Data   = MSGQ_START + i; /* to make it distinct from the counter */
        status = OS_QueuePut(msgq_id, (void *)&Data, sizeof(Data), 0);
        UtAssert_True(status == OS_SUCCESS, "OS Queue Put Rc=%d", (int)status);
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
    UtTest_Add(QueueTimeoutCheck, QueueTimeoutSetup, NULL, "QueueTimeoutTest");
    UtTest_Add(QueueMessageCheck, QueueMessageSetup, NULL, "QueueMessageCheck");
}
