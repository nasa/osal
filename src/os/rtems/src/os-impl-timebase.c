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

/**
 * \file     os-impl-timebase.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#define _USING_RTEMS_INCLUDES_

#include "os-rtems.h"

#include "os-shared-common.h"
#include "os-shared-timebase.h"
#include "os-shared-idmap.h"

#include "osapi-printf.h"

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

void OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks);

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/*
 * Prefer to use the MONOTONIC clock if available, as it will not get disrupted by setting
 * the time like the REALTIME clock will.
 */
#ifndef OS_PREFERRED_CLOCK
#ifdef _POSIX_MONOTONIC_CLOCK
#define OS_PREFERRED_CLOCK CLOCK_MONOTONIC
#else
#define OS_PREFERRED_CLOCK CLOCK_REALTIME
#endif
#endif

/****************************************************************************************
                                    LOCAL TYPEDEFS
 ***************************************************************************************/

typedef struct
{
    rtems_id       rtems_timer_id;
    rtems_id       tick_sem;
    rtems_id       handler_mutex;
    rtems_id       handler_task;
    uint8          simulate_flag;
    uint8          reset_flag;
    rtems_interval interval_ticks;
    uint32         configured_start_time;
    uint32         configured_interval_time;
} OS_impl_timebase_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseLock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TimeBaseLock_Impl(const OS_object_token_t *token)
{
    OS_impl_timebase_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);

    rtems_semaphore_obtain(impl->handler_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
} /* end OS_TimeBaseLock_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseUnlock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TimeBaseUnlock_Impl(const OS_object_token_t *token)
{
    OS_impl_timebase_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);

    rtems_semaphore_release(impl->handler_mutex);
} /* end OS_TimeBaseUnlock_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_ISR
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           An ISR to service a timer tick interrupt, which in turn
 *           posts a semaphore so the user task can execute
 *
 *-----------------------------------------------------------------*/
static rtems_timer_service_routine OS_TimeBase_ISR(rtems_id rtems_timer_id, void *arg)
{
    OS_VoidPtrValueWrapper_t            user_data;
    OS_object_token_t                   token;
    OS_impl_timebase_internal_record_t *local;

    user_data.opaque_arg = arg;
    if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMEBASE, user_data.id, &token) == OS_SUCCESS)
    {
        local = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, token);

        /*
         * Reset the timer, but only if an interval was selected
         */
        if (local->interval_ticks > 0)
        {
            rtems_timer_fire_after(rtems_timer_id, local->interval_ticks, OS_TimeBase_ISR, user_data.opaque_arg);
        }

        /*
         * RTEMS OS timers implemented with an ISR callback
         * this must be downgraded to an ordinary task context
         *
         * This is accomplished by just releasing a semaphore here.
         */
        rtems_semaphore_release(local->tick_sem);
    }

} /* end OS_TimeBase_ISR */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_WaitImpl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Pends on the semaphore for the next timer tick
 *
 *-----------------------------------------------------------------*/
static uint32 OS_TimeBase_WaitImpl(osal_id_t timebase_id)
{
    OS_object_token_t                   token;
    OS_impl_timebase_internal_record_t *impl;
    uint32                              tick_time;

    tick_time = 0;

    if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id, &token) == OS_SUCCESS)
    {
        impl = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, token);

        /*
         * Pend for the tick arrival
         */
        rtems_semaphore_obtain(impl->tick_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

        /*
         * Determine how long this tick was.
         * Note that there are plenty of ways this become wrong if the timer
         * is reset right around the time a tick comes in.  However, it is
         * impossible to guarantee the behavior of a reset if the timer is running.
         * (This is not an expected use-case anyway; the timer should be set and forget)
         */
        if (impl->reset_flag == 0)
        {
            tick_time = impl->configured_interval_time;
        }
        else
        {
            tick_time        = impl->configured_start_time;
            impl->reset_flag = 0;
        }
    }

    return tick_time;
} /* end OS_TimeBase_WaitImpl */

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_TimeBaseAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_TimeBaseAPI_Impl_Init(void)
{
    /*
    ** Store the clock accuracy for 1 tick.
    */
    rtems_interval ticks_per_sec = rtems_clock_get_ticks_per_second();

    if (ticks_per_sec <= 0)
    {
        return OS_ERROR;
    }

    /*
     * For the global ticks per second, use the value direct from RTEMS
     */
    OS_SharedGlobalVars.TicksPerSecond = (int32)ticks_per_sec;

    /*
     * Compute the clock accuracy in Nanoseconds (ns per tick)
     * This really should be an exact/whole number result; otherwise this
     * will round to the nearest nanosecond.
     */
    RTEMS_GlobalVars.ClockAccuracyNsec =
        (1000000000 + (OS_SharedGlobalVars.TicksPerSecond / 2)) / OS_SharedGlobalVars.TicksPerSecond;

    /*
     * Finally compute the Microseconds per tick
     * This must further round again to the nearest microsecond, so it is undesirable to use
     * this for time computations if the result is not exact.
     */
    OS_SharedGlobalVars.MicroSecPerTick = (RTEMS_GlobalVars.ClockAccuracyNsec + 500) / 1000;

    return (OS_SUCCESS);
} /* end OS_Rtems_TimeBaseAPI_Impl_Init */

/****************************************************************************************
                                INTERNAL FUNCTIONS
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_UsecsToTicks
 *
 *  Purpose:  Convert Microseconds to a number of ticks.
 *
 *-----------------------------------------------------------------*/
void OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks)
{
    uint32 result;

    /*
     * In order to compute without overflowing a 32 bit integer,
     * this is done in 2 parts -
     * the fractional seconds first then add any whole seconds.
     * the fractions are rounded UP so that this is guaranteed to produce
     * a nonzero number of ticks for a nonzero number of microseconds.
     */

    result = (1000 * (usecs % 1000000) + RTEMS_GlobalVars.ClockAccuracyNsec - 1) / RTEMS_GlobalVars.ClockAccuracyNsec;

    if (usecs >= 1000000)
    {
        result += (usecs / 1000000) * OS_SharedGlobalVars.TicksPerSecond;
    }

    *ticks = (rtems_interval)result;
} /* end OS_UsecsToTicks */

/****************************************************************************************
                                   Time Base API
 ***************************************************************************************/

/* The user may specify whether to use priority inheritance on mutexes via osconfig.h */
#define OSAL_TIMEBASE_MUTEX_ATTRIBS RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseCreate_Impl(const OS_object_token_t *token)
{
    int32                               return_code;
    rtems_status_code                   rtems_sc;
    OS_impl_timebase_internal_record_t *local;
    rtems_name                          r_name;
    OS_timebase_internal_record_t *     timebase;

    return_code = OS_SUCCESS;
    local       = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    timebase    = OS_OBJECT_TABLE_GET(OS_timebase_table, *token);

    /*
     * The RTEMS classic name for dependent resources
     */
    r_name = OS_ObjectIdToInteger(OS_ObjectIdFromToken(token));

    /*
     * Set up the necessary OS constructs
     *
     * If an external sync function is used then there is nothing to do here -
     * we simply call that function and it should synchronize to the time source.
     *
     * If no external sync function is provided then this will set up an RTEMS
     * timer to locally simulate the timer tick using the CPU clock.
     */
    local->simulate_flag = (timebase->external_sync == NULL);
    if (local->simulate_flag)
    {
        timebase->external_sync = OS_TimeBase_WaitImpl;

        /*
         * The tick_sem is a simple semaphore posted by the ISR and taken by the
         * timebase helper task (created later).
         */
        rtems_sc =
            rtems_semaphore_create(r_name, 0, RTEMS_SIMPLE_BINARY_SEMAPHORE | RTEMS_PRIORITY, 0, &local->tick_sem);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error: Tick Sem could not be created: %d\n", (int)rtems_sc);
            return_code = OS_TIMER_ERR_INTERNAL;
        }

        /*
         * The handler_mutex is deals with access to the callback list for this timebase
         */
        rtems_sc = rtems_semaphore_create(r_name, 1, OSAL_TIMEBASE_MUTEX_ATTRIBS, 0, &local->handler_mutex);

        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error: Handler Mutex could not be created: %d\n", (int)rtems_sc);
            rtems_semaphore_delete(local->tick_sem);
            return_code = OS_TIMER_ERR_INTERNAL;
        }

        rtems_sc = rtems_timer_create(r_name, &local->rtems_timer_id);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error: Timer object could not be created: %d\n", (int)rtems_sc);
            rtems_semaphore_delete(local->handler_mutex);
            rtems_semaphore_delete(local->tick_sem);
            return_code = OS_TIMER_ERR_UNAVAILABLE;
        }
    }

    /*
     * Spawn a dedicated time base handler thread
     *
     * This alleviates the need to handle expiration in the context of a signal handler -
     * The handler thread can call a BSP synchronized delay implementation as well as the
     * application callback function.  It should run with elevated priority to reduce latency.
     *
     * Note the thread will not actually start running until this function exits and releases
     * the global table lock.
     */
    if (return_code == OS_SUCCESS)
    {
        /* note on the priority - rtems is inverse (like vxworks) so that the lowest numeric
         * value will preempt other threads in the ready state.
         * Using "RTEMS_MINIMUM_PRIORITY + 1" because rtems seems to not schedule it at all if
         * the priority is set to RTEMS_MINIMUM_PRIORITY.
         */
        rtems_sc = rtems_task_create(r_name, RTEMS_MINIMUM_PRIORITY + 1, 0,
                                     RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
                                     RTEMS_LOCAL, &local->handler_task);

        /* check if task_create failed */
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            /* Provide some feedback as to why this failed */
            OS_printf("rtems_task_create failed: %s\n", rtems_status_text(rtems_sc));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
        else
        {
            /* will place the task in 'ready for scheduling' state */
            rtems_sc = rtems_task_start(local->handler_task,                          /*rtems task id*/
                                        (rtems_task_entry)OS_TimeBase_CallbackThread, /* task entry point */
                                        (rtems_task_argument)r_name);                 /* passed argument  */

            if (rtems_sc != RTEMS_SUCCESSFUL)
            {
                OS_printf("rtems_task_start failed: %s\n", rtems_status_text(rtems_sc));
                rtems_task_delete(local->handler_task);
                return_code = OS_TIMER_ERR_INTERNAL;
            }
        }

        if (return_code != OS_SUCCESS)
        {
            /* Also delete the resources we allocated earlier */
            rtems_timer_delete(local->rtems_timer_id);
            rtems_semaphore_delete(local->handler_mutex);
            rtems_semaphore_delete(local->tick_sem);
            return return_code;
        }
    }

    return return_code;
} /* end OS_TimeBaseCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseSet_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseSet_Impl(const OS_object_token_t *token, uint32 start_time, uint32 interval_time)
{
    OS_VoidPtrValueWrapper_t            user_data;
    OS_impl_timebase_internal_record_t *local;
    int32                               return_code;
    int                                 status;
    rtems_interval                      start_ticks;
    OS_timebase_internal_record_t *     timebase;

    local       = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    timebase    = OS_OBJECT_TABLE_GET(OS_timebase_table, *token);
    return_code = OS_SUCCESS;

    /* There is only something to do here if we are generating a simulated tick */
    if (local->simulate_flag)
    {
        /*
        ** Note that UsecsToTicks() already protects against intervals
        ** less than os_clock_accuracy -- no need for extra checks which
        ** would actually possibly make it less accurate.
        **
        ** Still want to preserve zero, since that has a special meaning.
        */

        if (start_time <= 0)
        {
            interval_time = 0; /* cannot have interval without start */
        }

        if (interval_time <= 0)
        {
            local->interval_ticks = 0;
        }
        else
        {
            OS_UsecsToTicks(interval_time, &local->interval_ticks);
        }

        /*
        ** The defined behavior is to not arm the timer if the start time is zero
        ** If the interval time is zero, then the timer will not be re-armed.
        */
        if (start_time > 0)
        {
            /*
            ** Convert from Microseconds to the timeout
            */
            OS_UsecsToTicks(start_time, &start_ticks);

            user_data.opaque_arg = NULL;
            user_data.id         = OS_ObjectIdFromToken(token);

            status = rtems_timer_fire_after(local->rtems_timer_id, start_ticks, OS_TimeBase_ISR, user_data.opaque_arg);
            if (status != RTEMS_SUCCESSFUL)
            {
                return_code = OS_TIMER_ERR_INTERNAL;
            }
            else
            {
                local->configured_start_time    = (10000 * start_ticks) / OS_SharedGlobalVars.TicksPerSecond;
                local->configured_interval_time = (10000 * local->interval_ticks) / OS_SharedGlobalVars.TicksPerSecond;
                local->configured_start_time *= 100;
                local->configured_interval_time *= 100;

                if (local->configured_start_time != start_time)
                {
                    OS_DEBUG("WARNING: timer %lu start_time requested=%luus, configured=%luus\n",
                             OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), (unsigned long)start_time,
                             (unsigned long)local->configured_start_time);
                }
                if (local->configured_interval_time != interval_time)
                {
                    OS_DEBUG("WARNING: timer %lu interval_time requested=%luus, configured=%luus\n",
                             OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), (unsigned long)interval_time,
                             (unsigned long)local->configured_interval_time);
                }

                if (local->interval_ticks > 0)
                {
                    timebase->accuracy_usec = local->configured_interval_time;
                }
                else
                {
                    timebase->accuracy_usec = local->configured_start_time;
                }
            }
        }
    }

    if (local->reset_flag == 0 && return_code == OS_SUCCESS)
    {
        local->reset_flag = 1;
    }
    return return_code;
} /* end OS_TimeBaseSet_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseDelete_Impl(const OS_object_token_t *token)
{
    rtems_status_code                   rtems_sc;
    OS_impl_timebase_internal_record_t *local;
    int32                               return_code;

    local       = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    return_code = OS_SUCCESS;

    /*
    ** Delete the tasks and timer OS constructs first, then delete the
    ** semaphores.  If the task/timer is running it might try to use them.
    */
    if (local->simulate_flag)
    {
        rtems_sc = rtems_timer_delete(local->rtems_timer_id);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error deleting rtems timer: %s\n", rtems_status_text(rtems_sc));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
    }

    rtems_sc = rtems_task_delete(local->handler_task);
    if (rtems_sc != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Error deleting timebase helper task: %s\n", rtems_status_text(rtems_sc));
        return_code = OS_TIMER_ERR_INTERNAL;
    }

    /*
     * If any delete/cleanup calls fail, unfortunately there is no recourse.
     * Just report the error via OS_DEBUG and the resource will be leaked.
     */
    if (return_code == OS_SUCCESS)
    {
        rtems_sc = rtems_semaphore_delete(local->handler_mutex);
        if (rtems_sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("Error deleting handler mutex: %s\n", rtems_status_text(rtems_sc));
        }

        if (local->simulate_flag)
        {
            rtems_sc = rtems_semaphore_delete(local->tick_sem);
            if (rtems_sc != RTEMS_SUCCESSFUL)
            {
                OS_DEBUG("Error deleting tick semaphore: %s\n", rtems_status_text(rtems_sc));
            }
            local->simulate_flag = 0;
        }
    }

    return return_code;
} /* end OS_TimeBaseDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo_Impl(const OS_object_token_t *token, OS_timebase_prop_t *timer_prop)
{
    return OS_SUCCESS;

} /* end OS_TimeBaseGetInfo_Impl */
