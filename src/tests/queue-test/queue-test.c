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
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void QueueTimeoutSetup(void);
void QueueTimeoutCheck(void);
void QueueTimeoutTimeJumpSetup(void);

#define MSGQ_DEPTH 50
#define MSGQ_SIZE  sizeof(uint32)
#define MSGQ_TOTAL 10
#define MSGQ_BURST 3

#define TIMEJUMP_SECONDS          10
#define TIMEJUMP_INTERVAL_SECONDS 5

/* Task 1 */
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101
#define TASK_2_STACK_SIZE 4096
#define TASK_2_PRIORITY   50

uint32    task_1_stack[TASK_1_STACK_SIZE];
osal_id_t task_1_id;
uint32    task_1_failures;
uint32    task_1_timeouts;
uint32    task_1_messages;
uint32    task_2_stack[TASK_2_STACK_SIZE];
osal_id_t task_2_id;
osal_id_t msgq_id;

static bool queue_test_timejump_created;
static bool queue_test_timejump_skip;

#ifdef __linux__
static bool            queue_test_timejump_applied;
static int             queue_test_timejump_errno;
static bool            queue_test_saved_time_valid;
static struct timespec queue_test_saved_realtime;
static struct timespec queue_test_saved_monotonic;

static void QueueTest_AddTimespec(const struct timespec *left, const struct timespec *right, struct timespec *result)
{
    result->tv_sec  = left->tv_sec + right->tv_sec;
    result->tv_nsec = left->tv_nsec + right->tv_nsec;

    if (result->tv_nsec >= 1000000000L)
    {
        result->tv_nsec -= 1000000000L;
        ++result->tv_sec;
    }
}

static void QueueTest_SubtractTimespec(const struct timespec *left, const struct timespec *right,
                                       struct timespec *result)
{
    result->tv_sec  = left->tv_sec - right->tv_sec;
    result->tv_nsec = left->tv_nsec - right->tv_nsec;

    if (result->tv_nsec < 0)
    {
        result->tv_nsec += 1000000000L;
        --result->tv_sec;
    }
}

static void QueueTimeoutRestoreRealtime(void)
{
    struct timespec current_monotonic;
    struct timespec elapsed_time;
    struct timespec restore_realtime;
    int             status;

    /* Remove the artificial jump while preserving real elapsed time. */
    if (!queue_test_saved_time_valid || !queue_test_timejump_applied)
    {
        return;
    }

    status = clock_gettime(CLOCK_MONOTONIC, &current_monotonic);
    UtAssert_True(status == 0, "clock_gettime CLOCK_MONOTONIC Rc=0");
    if (status != 0)
    {
        return;
    }

    QueueTest_SubtractTimespec(&current_monotonic, &queue_test_saved_monotonic, &elapsed_time);
    QueueTest_AddTimespec(&queue_test_saved_realtime, &elapsed_time, &restore_realtime);

    status = clock_settime(CLOCK_REALTIME, &restore_realtime);
    UtAssert_True(status == 0, "clock_settime CLOCK_REALTIME restore Rc=0");
    if (status == 0)
    {
        queue_test_timejump_applied = false;
    }
}

static void QueueTimeJumpTask(void)
{
    struct timespec realtime_clock_timeval;

    /* Apply one jump, then idle until teardown deletes this task. */
    OS_TaskDelay(TIMEJUMP_INTERVAL_SECONDS * 1000);

    if (clock_gettime(CLOCK_REALTIME, &realtime_clock_timeval) != 0)
    {
        queue_test_timejump_errno = errno;
    }
    else
    {
        realtime_clock_timeval.tv_sec += TIMEJUMP_SECONDS;

        if (clock_settime(CLOCK_REALTIME, &realtime_clock_timeval) != 0)
        {
            queue_test_timejump_errno = errno;
        }
        else
        {
            queue_test_timejump_applied = true;
        }
    }

    while (1)
    {
        OS_TaskDelay(1000);
    }
}
#endif

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
    uint32 expected      = 0;

    OS_printf("Starting task 1\n");

    OS_printf("Delay for 1 second before starting\n");
    OS_TaskDelay(1000);

    /* if errors occur do not loop endlessly */
    while (task_1_failures < 20)
    {
        status = OS_QueueGet(msgq_id, (void *)&data_received, OSAL_SIZE_C(MSGQ_SIZE), &data_size, 1000);

        if (status == OS_SUCCESS)
        {
            ++task_1_messages;
            UtAssert_True(data_received == expected, "TASK 1: data_received (%u) == expected (%u)",
                          (unsigned int)data_received, (unsigned int)expected);

            expected++;
        }
        else if (status == OS_QUEUE_TIMEOUT)
        {
            ++task_1_timeouts;
        }
        else
        {
            ++task_1_failures;
            OS_printf("TASK 1: Queue Get error: %d!\n", (int)status);
            OS_TaskDelay(10);
        }
    }
}

static void QueueTimeoutCheckInternal(void)
{
    int32  status;
    uint32 limit;

    if (queue_test_timejump_skip)
    {
        return;
    }

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "Timer delete Rc=%d", (int)status);
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);
    if (queue_test_timejump_created)
    {
        status = OS_TaskDelete(task_2_id);
        UtAssert_True(status == OS_SUCCESS, "Task 2 delete Rc=%d", (int)status);
    }

#ifdef __linux__
    if (queue_test_timejump_created)
    {
        UtAssert_True(queue_test_timejump_errno == 0, "Timejump task errno=%d", queue_test_timejump_errno);
        UtAssert_True(queue_test_timejump_applied, "Timejump task applied");
        QueueTimeoutRestoreRealtime();
    }
#endif

    status = OS_QueueDelete(msgq_id);
    UtAssert_True(status == OS_SUCCESS, "Queue 1 delete Rc=%d", (int)status);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u", (unsigned int)task_1_failures);

    /*
     * Since nothing currently sends messages, message count should be zero,
     * and timer counter =~ 10 + ( 10 x task_1_timeouts )
     */
    UtAssert_True(task_1_messages == 0, "Task 1 messages = %u", (unsigned int)task_1_messages);

    limit = (timer_counter / 10);
    UtAssert_True(task_1_timeouts <= limit, "Task 1 timeouts %u <= %u", (unsigned int)task_1_timeouts,
                  (unsigned int)limit);

    limit = ((timer_counter - 20) / 12);
    UtAssert_True(task_1_timeouts >= limit, "Task 1 timeouts %u >= %u", (unsigned int)task_1_timeouts,
                  (unsigned int)limit);
}

static void QueueTimeoutSetupInternal(bool enable_timejump)
{
    int32  status;
    uint32 accuracy = 0;

    task_1_failures = 0;
    task_1_messages = 0;
    task_1_timeouts = 0;
    queue_test_timejump_created = false;
    queue_test_timejump_skip    = false;
    timer_counter               = 0;

#ifdef __linux__
    queue_test_timejump_applied = false;
    queue_test_timejump_errno   = 0;
    queue_test_saved_time_valid = false;
#endif

    if (enable_timejump)
    {
#ifndef __linux__
        queue_test_timejump_skip = true;
        UtAssert_WARN("Timejump test skipped: Linux only");
        return;
#else
        if (geteuid() != 0)
        {
            queue_test_timejump_skip = true;
            UtAssert_WARN("Timejump test skipped: requires root");
            return;
        }

        status = clock_gettime(CLOCK_REALTIME, &queue_test_saved_realtime);
        UtAssert_True(status == 0, "clock_gettime CLOCK_REALTIME Rc=0");
        if (status != 0)
        {
            return;
        }

        status = clock_gettime(CLOCK_MONOTONIC, &queue_test_saved_monotonic);
        UtAssert_True(status == 0, "clock_gettime CLOCK_MONOTONIC Rc=0");
        if (status != 0)
        {
            return;
        }

        queue_test_saved_time_valid = true;
#endif
    }

    status = OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    UtAssert_True(status == OS_SUCCESS, "MsgQ create Id=%lx Rc=%d", OS_ObjectIdToInteger(msgq_id), (int)status);

    /*
    ** Create the "consumer" task.
    */
    status = OS_TaskCreate(&task_1_id, "Task 1", task_1, OSAL_STACKPTR_C(task_1_stack), sizeof(task_1_stack),
                           OSAL_PRIORITY_C(TASK_1_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_1_id), (int)status);

    if (enable_timejump)
    {
#ifdef __linux__
        /*
        ** Create the time jumper task.
        */
        status = OS_TaskCreate(&task_2_id, "Task 2", QueueTimeJumpTask, OSAL_STACKPTR_C(task_2_stack),
                               sizeof(task_2_stack), OSAL_PRIORITY_C(TASK_2_PRIORITY), 0);
        UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%lx Rc=%d", OS_ObjectIdToInteger(task_2_id),
                      (int)status);
        if (status == OS_SUCCESS)
        {
            queue_test_timejump_created = true;
        }
#endif
    }

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

    /* allow some time for task to run and accrue queue timeouts */
    while (timer_counter < 100)
    {
        OS_TaskDelay(100);
    }

#ifdef __linux__
    if (enable_timejump)
    {
        UtAssert_True(queue_test_timejump_errno == 0, "Timejump task errno=%d", queue_test_timejump_errno);
        UtAssert_True(queue_test_timejump_applied, "Timejump task applied");
    }
#endif
}

void QueueTimeoutSetup(void)
{
    QueueTimeoutSetupInternal(false);
}

void QueueTimeoutTimeJumpSetup(void)
{
    QueueTimeoutSetupInternal(true);
}

void QueueTimeoutCheck(void)
{
    QueueTimeoutCheckInternal();
}

void QueueMessageCheck(void)
{
    int32 status;

    OS_printf("Delay for half a second before checking\n");
    OS_TaskDelay(500);

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "Timer delete Rc=%d", (int)status);
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);
    status = OS_QueueDelete(msgq_id);
    UtAssert_True(status == OS_SUCCESS, "Queue 1 delete Rc=%d", (int)status);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u", (unsigned int)task_1_failures);
    UtAssert_True(task_1_messages == 10, "Task 1 messages = %u", (unsigned int)task_1_messages);
    UtAssert_True(task_1_timeouts == 0, "Task 1 timeouts = %u", (unsigned int)task_1_timeouts);
}

void QueueMessageSetup(void)
{
    int32  status;
    uint32 accuracy = 0;
    int    i;
    uint32 Data     = 0;
    task_1_failures = 0;
    task_1_messages = 0;
    task_1_timeouts = 0;

    status = OS_QueueCreate(&msgq_id, "MsgQ", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    UtAssert_True(status == OS_SUCCESS, "MsgQ create Id=%lx Rc=%d", OS_ObjectIdToInteger(msgq_id), (int)status);

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
     * Put 10 messages onto the que with some time in between the later messages
     * to make sure the que handles both storing and waiting for messages
     */
    for (i = 0; i < MSGQ_TOTAL; i++)
    {
        if (i > MSGQ_BURST)
            OS_TaskDelay(400);

        Data   = i;
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
    UtTest_Add(QueueTimeoutCheck, QueueTimeoutTimeJumpSetup, NULL, "QueueTimeoutTimeJumpTest");
    UtTest_Add(QueueMessageCheck, QueueMessageSetup, NULL, "QueueMessageCheck");
}
