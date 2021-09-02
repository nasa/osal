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
** timertest.c
**
** This program is an OSAL sample that tests the OSAL timer functions.
**
*/

#include <stdio.h>
#include <unistd.h>

#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

#define NUMBER_OF_TIMERS 5

#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101

void TimerTestSetup(void);
void TimerTestTask(void);
void TimerTestCheck(void);

OS_time_t StartTime;
OS_time_t EndTime;
uint32    TimerStart[NUMBER_OF_TIMERS]    = {1000, 2000000, 3000000, 4000000, 1000000};
uint32    TimerInterval[NUMBER_OF_TIMERS] = {500000, 400000, 800000, 600000, 0};

uint32 TimerTestTaskStack[TASK_1_STACK_SIZE];
int32  timer_counter[NUMBER_OF_TIMERS];
uint32 timer_idlookup[OS_MAX_TIMERS];

/*
 * Test timer function.
 * Note: For some Host OSs, this is the equivalent of an ISR, so the calls available are limited.
 * For example, Linux and vxWorks can call functions like printf, but RTEMS cannot.
 */
void test_func(osal_id_t timer_id)
{
    osal_index_t idx;
    OS_ConvertToArrayIndex(timer_id, &idx);
    timer_counter[timer_idlookup[idx]]++;
}

/* ********************** MAIN **************************** */

void UtTest_Setup(void)
{

    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the timer test setup and check routines in UT assert
     */
    UtTest_Add(TimerTestCheck, TimerTestSetup, NULL, "TimerTest");
}

void TimerTestSetup(void)
{
    int32     status;
    osal_id_t TimerTestTaskId;

    /*
     * In the new versions of OSAL, timers do NOT work in the "main" thread,
     * so we must create a task to handle them.
     */
    status = OS_TaskCreate(&TimerTestTaskId, "Task 1", TimerTestTask, OSAL_STACKPTR_C(TimerTestTaskStack),
                           sizeof(TimerTestTaskStack), OSAL_PRIORITY_C(TASK_1_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "Timer Test Task Created RC=%d", (int)status);

    /*
     * Invoke OS_IdleLoop() so that the task/timers can run
     *
     * OS_IdleLoop() will return once TimerTestTask calls OS_ApplicationShutdown
     *
     * It is important to note that UT Assert does NOT officially
     * support multi-threaded testing.  HOWEVER, the architecture of
     * this test ensures that the "TimerTestCheck" will NOT execute until
     * "TimerTestTask" gets to the end of its run.
     *
     * Therefore it is OK to use UT asserts within both functions.
     */
    OS_IdleLoop();
}

void TimerTestTask(void)
{

    int          i = 0;
    int32        TimerStatus[NUMBER_OF_TIMERS];
    osal_index_t TableId;
    osal_id_t    TimerID[NUMBER_OF_TIMERS];
    char         TimerName[NUMBER_OF_TIMERS][20] = {"TIMER1", "TIMER2", "TIMER3", "TIMER4", "TIMER5"};
    uint32       ClockAccuracy;

    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        TimerStatus[i] = OS_TimerCreate(&TimerID[i], TimerName[i], &ClockAccuracy, &(test_func));
        UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d Created RC=%d ID=%lx", i, (int)TimerStatus[i],
                      OS_ObjectIdToInteger(TimerID[i]));

        UtPrintf("Timer %d Accuracy = %d microseconds \n", i, (int)ClockAccuracy);

        OS_ConvertToArrayIndex(TimerID[i], &TableId);
        timer_idlookup[TableId] = i;
    }

    /* Sample the clock now, before starting any timer */
    OS_GetLocalTime(&StartTime);
    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        /*
         * to ensure that all timers are started as closely as possible,
         * this just stores the result and does not assert/printf now
         */
        TimerStatus[i] = OS_TimerSet(TimerID[i], TimerStart[i], TimerInterval[i]);
    }

    /*
     * Now the actual OS_TimerSet() return code can be checked.
     */
    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d programmed RC=%d", i, (int)TimerStatus[i]);
    }

    /*
    ** Let the main thread sleep
    */
    UtPrintf("Starting Delay loop.\n");
    for (i = 0; i < 30; i++)
    {
        /*
        ** Even though this sleep call is for 1 second,
        ** the sigalarm timer for the 1hz will keep waking
        ** it up. Keep that in mind when setting the timer down
        ** to something very small.
        */
        OS_TaskDelay(1000);
    }
    OS_GetLocalTime(&EndTime);

    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        TimerStatus[i] = OS_TimerDelete(TimerID[i]);
    }

    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d delete RC=%d. Count total = %d", i, (int)TimerStatus[i],
                      (int)timer_counter[i]);
    }

    OS_ApplicationShutdown(true);
    OS_TaskExit();
}

void TimerTestCheck(void)
{
    uint32 microsecs;
    int32  expected;
    uint32 i;

    /*
     * Time limited test - check and exit
     */
    microsecs = OS_TimeGetTotalMicroseconds(OS_TimeSubtract(EndTime, StartTime));

    /* Make sure the ratio of the timers are OK */
    for (i = 0; i < NUMBER_OF_TIMERS && i < OS_MAX_TIMERS; i++)
    {
        if (TimerInterval[i] == 0)
        {
            /*
             * When the Timer Interval is 0, it's a one shot so expect exactly 1 tick
             */
            expected = 1;
            UtAssert_True(timer_counter[i] == (expected), "Timer %d count = %d", (int)i, (int)(expected));
        }
        else
        {
            /*
             * Expect one tick after the start time (i.e. first tick)
             * Plus one tick for every interval that occurred during the test
             */
            expected = 1 + (microsecs - TimerStart[i]) / TimerInterval[i];
            UtAssert_True(expected > 0, "Expected ticks = %u", (unsigned int)expected);

            /*
             * Since all these counts are affected by test system load,
             * allow for some fudge factor before declaring failure
             */
            UtAssert_True(timer_counter[i] >= (expected - 3), "Timer %d count >= %d", (int)i, (int)(expected - 3));
            UtAssert_True(timer_counter[i] <= (expected + 3), "Timer %d count <= %d", (int)i, (int)(expected + 3));
        }
    }
}
