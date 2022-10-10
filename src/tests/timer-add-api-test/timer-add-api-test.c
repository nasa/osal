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
 * Filename: timer-add-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-timer"
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

#define NUMBER_OF_TIMERS  4
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101

OS_time_t StartTime;
OS_time_t EndTime;
uint32    TimerStart[NUMBER_OF_TIMERS]    = {1000, 2000000, 3000000, 4000000};
uint32    TimerInterval[NUMBER_OF_TIMERS] = {500000, 400000, 800000, 600000};

uint32 TimerTestTaskStack[TASK_1_STACK_SIZE];
uint32 timer_counter[NUMBER_OF_TIMERS];

void counter_func(osal_id_t timer_id, void *arg)
{
    uint32 *counter = arg;
    ++(*counter);
}

void null_func(osal_id_t timer_id, void *arg) {}

/* *************************************** MAIN ************************************** */

void TestTimerAddApi(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t
     * callback_ptr, void *callback_arg)
     */

    uint32    expected;
    osal_id_t badid;
    osal_id_t timer_id;
    osal_id_t time_base_id = OS_OBJECT_ID_UNDEFINED;
    int       i            = 0;
    int32     TimerStatus[NUMBER_OF_TIMERS];
    osal_id_t TimerID[OS_MAX_TIMERS];
    char      temp_name[OS_MAX_API_NAME + 5];
    char      TimerName[NUMBER_OF_TIMERS][20] = {"TIMER1", "TIMER2", "TIMER3", "TIMER4"};
    uint32    microsecs;

    /* Create and set the TimeBase obj and verify success */

    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, "TimeBase", 0), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, 10000, 10000), OS_SUCCESS);

    memset(temp_name, 'x', sizeof(temp_name) - 1);
    temp_name[sizeof(temp_name) - 1] = 0;
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, temp_name, time_base_id, &null_func, NULL), OS_ERR_NAME_TOO_LONG);

    for (i = 0; i < OS_MAX_TIMERS; i++)
    {
        TimerID[i] = OS_OBJECT_ID_UNDEFINED;
        snprintf(temp_name, sizeof(temp_name), "Timer%d", i);
        UtAssert_INT32_EQ(OS_TimerAdd(&TimerID[i], temp_name, time_base_id, &null_func, NULL), OS_SUCCESS);
        UtPrintf("Timer %d Created ID=%lx", i, OS_ObjectIdToInteger(TimerID[i]));
    }

    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "TooMany", time_base_id, &null_func, NULL), OS_ERR_NO_FREE_IDS);
    for (i = 0; i < OS_MAX_TIMERS; i++)
    {
        UtAssert_INT32_EQ(OS_TimerDelete(TimerID[i]), OS_SUCCESS);
    }

    for (i = 0; i < NUMBER_OF_TIMERS; i++)
    {
        UtAssert_INT32_EQ(OS_TimerAdd(&TimerID[i], TimerName[i], time_base_id, &counter_func, &timer_counter[i]),
                          OS_SUCCESS);
        UtPrintf("Timer %d Created ID=%lx", i, OS_ObjectIdToInteger(TimerID[i]));
    }

    /* Sample the clock now, before starting any timer */
    OS_GetLocalTime(&StartTime);
    for (i = 0; i < NUMBER_OF_TIMERS; i++)
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
    for (i = 0; i < NUMBER_OF_TIMERS; i++)
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

    for (i = NUMBER_OF_TIMERS - 1; i >= 0; --i)
    {
        TimerStatus[i] = OS_TimerDelete(TimerID[i]);
    }

    for (i = 0; i < NUMBER_OF_TIMERS; i++)
    {
        UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d delete RC=%d. Count total = %d", i, (int)TimerStatus[i],
                      (int)timer_counter[i]);
    }

    /*
     * Time limited test
     */
    microsecs = OS_TimeGetTotalMicroseconds(OS_TimeSubtract(EndTime, StartTime));

    /* Make sure the ratio of the timers are OK */
    for (i = 0; i < NUMBER_OF_TIMERS; i++)
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

    /* Test nominal inputs */
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "Timer", time_base_id, null_func, NULL), OS_SUCCESS);

    /* create a bad ID by flipping the bits of a good ID */
    badid = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(time_base_id) ^ 0xFFFFFFFF);

    /* Test invalid inputs */
    UtAssert_INT32_EQ(OS_TimerAdd(NULL, "Timer", time_base_id, null_func, NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "Timer", OS_OBJECT_ID_UNDEFINED, null_func, NULL), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "Timer", badid, null_func, NULL), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "Timer", time_base_id, NULL, NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, "Timer", time_base_id, null_func, NULL), OS_ERR_NAME_TAKEN);
    UtAssert_INT32_EQ(OS_TimerAdd(&timer_id, 0, time_base_id, null_func, NULL), OS_INVALID_POINTER);
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
    UtTest_Add(TestTimerAddApi, NULL, NULL, "TestTimerAddApi");
}
