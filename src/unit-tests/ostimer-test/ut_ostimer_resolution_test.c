/************************************************************************
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
 * \file
 * Verify POSIX timer-resolution rounding through the normal UT Assert runner.
 * Each executable supplies one clock_getres() result during OSAL initialization.
 * Only that query is replaced; no host clock or installed library is changed.
 */

#include <time.h>
#include <unistd.h>

#include "ut_os_support.h"

static uint32 UT_ClockGetResCalls;

int __wrap_clock_getres(clockid_t clock_id, struct timespec *resolution);

int __wrap_clock_getres(clockid_t clock_id, struct timespec *resolution)
{
    (void)clock_id;
    ++UT_ClockGetResCalls;
    if (resolution != NULL)
    {
        resolution->tv_sec  = 0;
        resolution->tv_nsec = UT_CLOCK_RESOLUTION_NS;
    }
    return 0;
}

static void UT_os_resolutioncallback(osal_id_t timer_id)
{
    (void)timer_id;
}

static void UT_os_checktimerinfo(osal_id_t timer_id, osal_id_t timebase_id, uint32 expected_accuracy)
{
    OS_timer_prop_t    timer_properties;
    OS_timebase_prop_t timebase_properties;

    if (UtAssert_INT32_EQ(OS_TimerGetInfo(timer_id, &timer_properties), OS_SUCCESS))
    {
        UtAssert_UINT32_EQ(timer_properties.accuracy, expected_accuracy);
    }
    if (UtAssert_INT32_EQ(OS_TimeBaseGetInfo(timebase_id, &timebase_properties), OS_SUCCESS))
    {
        UtAssert_UINT32_EQ(timebase_properties.accuracy, expected_accuracy);
    }
}

static void UT_os_timerresolution_test(void)
{
    osal_id_t          timer_id;
    osal_id_t          timebase_id;
    OS_timebase_prop_t timebase_properties;
    uint32             clock_accuracy;
    uint32             tick_accuracy;
    long               ticks_per_second;

    UtAssert_UINT32_EQ(UT_ClockGetResCalls, 1);
    ticks_per_second = sysconf(_SC_CLK_TCK);
    if (!UtAssert_True(ticks_per_second > 0, "System tick frequency is available"))
    {
        return;
    }
    tick_accuracy = (uint32)((1000000 + ticks_per_second / 2) / ticks_per_second); /* [us] */

    if (!UtAssert_INT32_EQ(OS_TimerCreate(&timer_id, "resolution", &clock_accuracy, UT_os_resolutioncallback),
                           OS_SUCCESS))
    {
        return;
    }
    UtAssert_UINT32_EQ(clock_accuracy, UT_EXPECTED_ACCURACY_US);
    if (!UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&timebase_id, "resolution"), OS_SUCCESS))
    {
        UtAssert_INT32_EQ(OS_TimerDelete(timer_id), OS_SUCCESS);
        return;
    }

    /* Creation reports clock resolution; GetInfo retains the original tick-derived property. */
    UT_os_checktimerinfo(timer_id, timebase_id, tick_accuracy);
    if (UtAssert_INT32_EQ(OS_TimerSet(timer_id, 500000, 20000), OS_SUCCESS)) /* [us] */
    {
        UT_os_checktimerinfo(timer_id, timebase_id, 20000); /* [us], periodic property */
    }
    if (UtAssert_INT32_EQ(OS_TimerSet(timer_id, 500000, 0), OS_SUCCESS)) /* [us], one-shot */
    {
        UT_os_checktimerinfo(timer_id, timebase_id, 500000); /* [us], one-shot property */
    }
    UtAssert_INT32_EQ(OS_TimerDelete(timer_id), OS_SUCCESS);

    /* Directly created timebases retain their original properties too. */
    if (!UtAssert_INT32_EQ(OS_TimeBaseCreate(&timebase_id, "direct-resolution", NULL), OS_SUCCESS))
    {
        return;
    }
    if (UtAssert_INT32_EQ(OS_TimeBaseGetInfo(timebase_id, &timebase_properties), OS_SUCCESS))
    {
        UtAssert_UINT32_EQ(timebase_properties.accuracy, tick_accuracy);
    }
    if (UtAssert_INT32_EQ(OS_TimeBaseSet(timebase_id, 500000, 23000), OS_SUCCESS)) /* [us] */
    {
        if (UtAssert_INT32_EQ(OS_TimeBaseGetInfo(timebase_id, &timebase_properties), OS_SUCCESS))
        {
            UtAssert_UINT32_EQ(timebase_properties.accuracy, 23000); /* [us] */
        }
    }
    UtAssert_INT32_EQ(OS_TimeBaseDelete(timebase_id), OS_SUCCESS);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");
    UtTest_Add(UT_os_timerresolution_test, NULL, NULL, "POSIX timer resolution and GetInfo compatibility");
}
