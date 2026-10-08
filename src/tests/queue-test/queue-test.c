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
#define MSGQ_DELAY 400 /* post-burst inter-message time */
#define MSGQ_START 100 /* base value for data, to make it distinct from the msg counter */
#define MSGQ_WAIT  (2 * MSGQ_DELAY)

/* Task 1 */
#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY   101

uint32    task_stack[TASK_STACK_SIZE];
osal_id_t task_id;
uint32    task_failures;
uint32    task_timeouts;
uint32    task_messages;
uint32    task_expected;
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
        status = OS_QueueGet(msgq_id, (void *)&data_received, OSAL_SIZE_C(MSGQ_SIZE), &data_size, MSGQ_WAIT);

        if (status == OS_SUCCESS)
        {
            ++task_messages;
            UtAssert_INT32_EQ(data_received, expected);

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
    uint32 limit;

    UtAssert_INT32_EQ(OS_TimerDelete(timer_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TaskDelete(task_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_QueueDelete(msgq_id), OS_SUCCESS);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_INT32_EQ(task_failures, 0);

    /* Since nothing currently sends messages, message count should be zero */
    UtAssert_INT32_EQ(task_messages, 0);

    limit = ((timer_counter + 5) * 100) / MSGQ_WAIT;
    UtAssert_True(task_timeouts <= limit, "Task timeouts %u <= %u", (unsigned int)task_timeouts, (unsigned int)limit);

    limit = ((timer_counter - 5) * 100) / MSGQ_WAIT;
    UtAssert_True(task_timeouts >= limit, "Task timeouts %u >= %u", (unsigned int)task_timeouts, (unsigned int)limit);
}

void QueueTimeoutSetup(void)
{
    int32  status;
    uint32 accuracy = 0;

    task_failures    = 0;
    task_messages    = 0;
    task_timeouts    = 0;
    task_start_delay = 1000000 / timer_interval;

    UtAssert_INT32_EQ(OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0),
                      OS_SUCCESS);

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
    UtAssert_INT32_EQ(status, OS_SUCCESS);

    /*
    ** Create a timer
    */
    UtAssert_INT32_EQ(OS_TimerCreate(&timer_id, "Timer 1", &accuracy, &(TimerFunction)), OS_SUCCESS);
    UtPrintf("Timer Accuracy = %u microseconds \n", (unsigned int)accuracy);

    /*
    ** Start the timer
    */
    UtAssert_INT32_EQ(OS_TimerSet(timer_id, timer_start, timer_interval), OS_SUCCESS);

    /* allow some time for task to run and accrue queue timeouts */
    while (timer_counter < 100)
    {
        OS_TaskDelay(100);
    }
}

void QueueMessageCheck(void)
{
    /* this delay is just to ensure the consumer task can empty the queue, plus
     * enough extra delay to get one (and only one) queue timeout event */
    UtPrintf("Delay before checking\n");
    OS_TaskDelay(MSGQ_WAIT - (MSGQ_DELAY / 2));

    UtAssert_INT32_EQ(OS_TimerDelete(timer_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TaskDelete(task_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_QueueDelete(msgq_id), OS_SUCCESS);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_INT32_EQ(task_failures, 0);
    UtAssert_INT32_EQ(task_messages, task_expected);
    UtAssert_INT32_EQ(task_timeouts, 1);
}

void QueueMessageSetup(void)
{
    int32           status;
    uint32          accuracy = 0;
    uint32          put_count;
    int             i;
    uint32          Data = 0;
    OS_queue_prop_t queue_prop;

    task_failures    = 0;
    task_messages    = 0;
    task_timeouts    = 0;
    task_start_delay = MSGQ_DELAY;

    UtAssert_INT32_EQ(OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0),
                      OS_SUCCESS);

    UtAssert_INT32_EQ(OS_QueueGetInfo(msgq_id, &queue_prop), OS_SUCCESS);

    /* Sometimes if configs have been tuned it can be unclear what the active value is */
    UtPrintf("Starting Test with DEPTH=%u, SIZE=%u, DELAY=%u\n",
             (unsigned int)queue_prop.queue_depth,
             (unsigned int)queue_prop.data_size,
             MSGQ_DELAY);

    put_count = 0;

    /* pre-fill the queue.  With no reader/consumer running yet, should be able to write up to q depth */
    for (i = 0; i < queue_prop.queue_depth; ++i)
    {
        Data = MSGQ_START + put_count;
        UtAssert_INT32_EQ(OS_QueuePut(msgq_id, (void *)&Data, sizeof(Data), 0), OS_SUCCESS);
        ++put_count;
    }

    Data = MSGQ_START + put_count;
    UtAssert_INT32_EQ(OS_QueuePut(msgq_id, (void *)&Data, sizeof(Data), 0), OS_QUEUE_FULL);

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
    UtAssert_INT32_EQ(status, OS_SUCCESS);

    /*
    ** Create a timer
    */
    UtAssert_INT32_EQ(OS_TimerCreate(&timer_id, "Timer 1", &accuracy, &(TimerFunction)), OS_SUCCESS);
    UtPrintf("Timer Accuracy = %u microseconds \n", (unsigned int)accuracy);

    /*
    ** Start the timer
    */
    UtAssert_INT32_EQ(OS_TimerSet(timer_id, timer_start, timer_interval), OS_SUCCESS);

    /*
     * Put messages onto the que with some time in between the later messages
     * to make sure the queue handles both storing and waiting for messages
     */
    OS_TaskDelay(task_start_delay + (MSGQ_DELAY / 4));
    for (i = 0; i < MSGQ_TOTAL; i++)
    {
        Data = MSGQ_START + put_count;
        UtAssert_INT32_EQ(OS_QueuePut(msgq_id, (void *)&Data, sizeof(Data), 0), OS_SUCCESS);
        ++put_count;
        OS_TaskDelay(MSGQ_DELAY);
    }

    task_expected = put_count;
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
