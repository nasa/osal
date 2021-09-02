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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-timebase.h"

#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"

#include <signal.h>
#include <taskLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <errnoLib.h>

#include "osapi-task.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/* Each "timebase" resource spawns a dedicated servicing task-
 * this task (not the timer ISR) is the context that calls back to
 * the user application.
 *
 * This should run at the highest priority to reduce latency.
 */
#define OSAL_TIMEBASE_TASK_STACK_SIZE  4096
#define OSAL_TIMEBASE_TASK_PRIORITY    0
#define OSAL_TIMEBASE_TASK_OPTION_WORD 0

#define OSAL_TIMEBASE_REG_WAIT_LIMIT 100
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
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];

static uint32 OS_ClockAccuracyNsec;

/****************************************************************************************
                                INTERNAL FUNCTIONS
****************************************************************************************/

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

    semTake(impl->handler_mutex, WAIT_FOREVER);
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

    semGive(impl->handler_mutex);
} /* end OS_TimeBaseUnlock_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_Impl_UsecToTimespec
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
void OS_VxWorks_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
{
    if (usecs < 1000000)
    {
        time_spec->tv_nsec = (usecs * 1000);
        time_spec->tv_sec  = 0;
    }
    else
    {
        time_spec->tv_sec  = usecs / 1000000;
        time_spec->tv_nsec = (usecs % 1000000) * 1000;
    }
} /* end OS_Impl_UsecToTimespec */

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_SigWait
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Blocks the calling task until the timer tick arrives
 *
 *-----------------------------------------------------------------*/
uint32 OS_VxWorks_SigWait(osal_id_t timebase_id)
{
    OS_object_token_t                   token;
    OS_impl_timebase_internal_record_t *impl;
    uint32                              tick_time;
    int                                 signo;
    int                                 ret;

    tick_time = 0;

    if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id, &token) == OS_SUCCESS)
    {
        impl = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, token);

        /*
         * Pend for the tick arrival
         */
        ret = sigwait(&impl->timer_sigset, &signo);

        /*
         * The sigwait() can be interrupted....
         * Only return nonzero interval time if it is an actual timer signal.
         * This value will get added to the free-run counter.
         *
         * NOTE: This always returns the nominal interval time.
         *
         * This value will likely be wrong on the first tick
         * after starting a new timebase or reconfiguring an
         * existing one.  This is because interval times
         * are relative, and the exact instant that the reconfig
         * takes effect is not knowable.
         *
         * This is OK because free-run counter values
         * are only expected to be valid in steady-state
         * conditions.  Samples from before/after a reconfig
         * are generally not comparable.
         */
        if (ret == OK && signo == impl->assigned_signal)
        {
            if (impl->reset_flag)
            {
                /* first interval after reset, use start time */
                tick_time        = impl->configured_start_time;
                impl->reset_flag = false;
            }
            else
            {
                tick_time = impl->configured_interval_time;
            }
        }
    }

    return tick_time;
} /* end OS_VxWorks_SigWait */

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_RegisterTimer
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
void OS_VxWorks_RegisterTimer(osal_id_t obj_id)
{
    OS_impl_timebase_internal_record_t *local;
    OS_object_token_t                   token;
    struct sigevent                     evp;
    int                                 status;
    int32                               retcode;

    retcode = OS_ObjectIdGetById(OS_LOCK_MODE_RESERVED, OS_OBJECT_TYPE_OS_TIMEBASE, obj_id, &token);
    if (retcode == OS_SUCCESS)
    {
        local = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, token);

        if (local->assigned_signal == 0)
        {
            /* nothing to register in RTOS */
            status = 0;
        }
        else
        {
            memset(&evp, 0, sizeof(evp));
            evp.sigev_notify = SIGEV_SIGNAL;
            evp.sigev_signo  = local->assigned_signal;

            /*
            ** Create the timer
            **
            ** The result is not returned from this function, because
            ** this is a different task context from the original creator.
            **
            ** The registration status is returned through the OS_impl_timebase_table entry,
            ** which is checked by the creator before returning.
            **
            ** If set to ERROR, then this task will be subsequently deleted.
            */
            status = timer_create(OS_PREFERRED_CLOCK, &evp, &local->host_timerid);
        }

        if (status < 0)
        {
            OS_DEBUG("timer_create() failed: errno=%d\n", errno);
            local->timer_state = OS_TimerRegState_ERROR;
        }
        else
        {
            local->timer_state = OS_TimerRegState_SUCCESS;
        }

        OS_ObjectIdRelease(&token);
    }
    else
    {
        OS_DEBUG("OS_VxWorks_RegisterTimer() bad ID, code=%d\n", (int)retcode);
    }
} /* end OS_VxWorks_RegisterTimer */

/****************************************************************************************
                      Entry point for helper thread
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_TimeBaseTask
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int OS_VxWorks_TimeBaseTask(int arg)
{
    osal_id_t obj_id;

    obj_id = OS_ObjectIdFromInteger(arg);
    OS_VxWorks_RegisterTimer(obj_id);
    OS_TimeBase_CallbackThread(obj_id);

    return 0;
} /* end OS_VxWorks_TimeBaseTask */

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_TimeBaseAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void)
{
    int clockRate;

    /*
    ** sysClkRateGet returns ticks/second.
    */
    clockRate = sysClkRateGet();

    if (clockRate <= 0)
    {
        return OS_ERROR;
    }

    OS_SharedGlobalVars.TicksPerSecond = clockRate;

    /*
     * Store the clock accuracy for 1 tick.
     *
     * Compute the clock accuracy in Nanoseconds (ns per tick)
     * This really should be an exact/whole number result; otherwise this
     * will round to the nearest nanosecond.
     */
    OS_ClockAccuracyNsec = (1000000000 + (OS_SharedGlobalVars.TicksPerSecond / 2)) / OS_SharedGlobalVars.TicksPerSecond;

    /*
     * Finally compute the Microseconds per tick
     * This must further round again to the nearest microsecond (using the + 500 / 1000),
     * so it is undesirable to use this for time computations if the result is not exact.
     */
    OS_SharedGlobalVars.MicroSecPerTick = (OS_ClockAccuracyNsec + 500) / 1000;

    return (OS_SUCCESS);
} /* end OS_VxWorks_TimeBaseAPI_Impl_Init */

/****************************************************************************************
                                   Time Base API
****************************************************************************************/

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
    /*
     * The tick_sem is a simple semaphore posted by the ISR and taken by the
     * timebase helper task (created later).
     */
    int32                               return_code;
    OS_impl_timebase_internal_record_t *local;
    OS_timebase_internal_record_t *     timebase;
    int                                 signo;
    sigset_t                            inuse;
    osal_index_t                        idx;
    uint32                              i;

    return_code = OS_SUCCESS;

    local    = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, *token);

    sigemptyset(&local->timer_sigset);
    local->assigned_signal = 0;
    local->handler_task    = 0;
    local->handler_mutex   = (SEM_ID)0;
    local->host_timerid    = 0;
    local->timer_state     = OS_TimerRegState_INIT;
    local->reset_flag      = false;

    /*
     * Set up the necessary OS constructs
     *
     * If an external sync function is used then there is nothing to do here -
     * we simply call that function and it should synchronize to the time source.
     *
     * If no external sync function is provided then this will set up a VxWorks
     * timer to locally simulate the timer tick using the CPU clock.
     */
    if (timebase->external_sync == NULL)
    {
        /*
         * find an RT signal that is not used by another time base object.
         * This the global lock is held here so there is no chance of
         * the underlying tables changing.
         */
        sigemptyset(&inuse);

        for (idx = 0; idx < OS_MAX_TIMEBASES; ++idx)
        {
            if (OS_ObjectIdIsValid(OS_global_timebase_table[idx].active_id) &&
                OS_impl_timebase_table[idx].assigned_signal > 0)
            {
                /* mark signal as in-use */
                sigaddset(&inuse, OS_impl_timebase_table[idx].assigned_signal);
            }
        }

        for (signo = SIGRTMIN; signo <= SIGRTMAX; ++signo)
        {
            if (!sigismember(&inuse, signo))
            {
                /* signal is available, stop search */
                local->assigned_signal = signo;
                break;
            }
        }

        if (local->assigned_signal == 0)
        {
            /* no available signal for timer */
            OS_DEBUG("No free RT signals to use for simulated time base\n");
            return_code = OS_TIMER_ERR_UNAVAILABLE;
        }
        else
        {
            /*
             * Note that VxWorks appears to always send the timer signal
             * to the task that called timer_create().  This is different
             * than e.g. POSIX where the signal is sent to the process
             * and masks can be modified to direct the signal to the
             * correct task.
             *
             * Therefore, we choose the signal now, but defer calling
             * timer_create to the internal helper task.
             */
            sigaddset(&local->timer_sigset, signo);

            /*
             * Use local sigwait() wrapper as a sync function for the local task.
             */
            timebase->external_sync = OS_VxWorks_SigWait;
        }
    }

    if (return_code == OS_SUCCESS)
    {
        /*
         * Create the handler_mutex.
         * This controls access to the callback list for this timebase
         *
         * Note memory for this sem is statically allocated, so if a failure
         * occurs there is no need to free the memory later.
         */
        local->handler_mutex = semMInitialize(local->mmem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
        if (local->handler_mutex == (SEM_ID)0)
        {
            OS_DEBUG("Error: Handler Mutex could not be initialized: errno=%d\n", errno);
            return_code = OS_TIMER_ERR_INTERNAL;
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
        local->handler_task = taskSpawn(timebase->timebase_name, OSAL_TIMEBASE_TASK_PRIORITY, /* priority */
                                        OSAL_TIMEBASE_TASK_OPTION_WORD,                       /* task option word */
                                        OSAL_TIMEBASE_TASK_STACK_SIZE,    /* size (bytes) of stack needed */
                                        (FUNCPTR)OS_VxWorks_TimeBaseTask, /* Timebase helper task entry point */
                                        OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), /* 1st arg is ID */
                                        0, 0, 0, 0, 0, 0, 0, 0, 0);

        /* check if taskSpawn failed */
        if (local->handler_task == ((TASK_ID)ERROR))
        {
            OS_DEBUG("taskSpawn() - vxWorks errno: %d\n", errno);
            return_code = OS_TIMER_ERR_INTERNAL;
        }
        else
        {
            /*
             * Wait for the newly-spawned task to call timer_create().
             * If this is successful, then return success, otherwise
             * return failure.
             *
             * As the task runs with a high priority, it should preempt
             * this task and therefore it should probably already be
             * complete by the time execution gets here.  But for
             * multi-core machines it is possible that an extra delay
             * is necessary.
             */
            i = OSAL_TIMEBASE_REG_WAIT_LIMIT;
            while (local->timer_state == OS_TimerRegState_INIT && i > 0)
            {
                OS_TaskDelay(1);
                --i;
            }

            /*
             * If the timer wasn't fully created successfully,
             * then delete the task.
             */
            if (local->timer_state != OS_TimerRegState_SUCCESS)
            {
                OS_DEBUG("Error during timer registration\n");
                taskDelete(local->handler_task);
                return_code = OS_TIMER_ERR_INTERNAL;
            }
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
    OS_impl_timebase_internal_record_t *local;
    struct itimerspec                   timeout;
    int32                               return_code;
    int                                 status;

    local = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);

    /* There is only something to do here if we are generating a simulated tick */
    if (local->assigned_signal <= 0)
    {
        /* An externally synced timebase does not need to be set (noop) */
        return_code = OS_SUCCESS;
    }
    else
    {
        OS_VxWorks_UsecToTimespec(start_time, &timeout.it_value);
        OS_VxWorks_UsecToTimespec(interval_time, &timeout.it_interval);

        /*
        ** Program the real timer
        */
        status = timer_settime(local->host_timerid, 0, /* Flags field can be zero */
                               &timeout,               /* struct itimerspec */
                               NULL);                  /* Oldvalue */

        if (status == OK)
        {
            /*
             * VxWorks will round the interval up to the next higher
             * system tick interval.  Sometimes this can make a substantial
             * difference in the actual time, particularly as the error
             * accumulates over time.
             *
             * timer_gettime() will reveal the actual interval programmed,
             * after all rounding/adjustments, which can be used to determine
             * the actual start_time/interval_time that will be realized.
             *
             * If this actual interval is different than the intended value,
             * it may indicate the need for better tuning on the app/config/bsp
             * side, and so a DEBUG message is generated.
             */
            status = timer_gettime(local->host_timerid, &timeout);
            if (status == OK)
            {
                return_code = OS_SUCCESS;

                local->configured_start_time = (timeout.it_value.tv_sec * 1000000) + (timeout.it_value.tv_nsec / 1000);
                local->configured_interval_time =
                    (timeout.it_interval.tv_sec * 1000000) + (timeout.it_interval.tv_nsec / 1000);

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
            }
            else
            {
                return_code = OS_ERROR;

                OS_DEBUG("WARNING: timer %lu timer_gettime() failed - timer not configured properly?\n",
                         OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)));
            }
        }
        else
        {
            return_code = OS_TIMER_ERR_INVALID_ARGS;
        }
    }

    if (!local->reset_flag && return_code == OS_SUCCESS)
    {
        local->reset_flag = true;
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
    OS_impl_timebase_internal_record_t *local;
    int32                               return_code;

    local       = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    return_code = OS_SUCCESS;

    /* An assigned_signal value indicates the OS timer needs deletion too */
    if (local->assigned_signal > 0)
    {
        /* this also implies the sync sem needs delete too */
        timer_delete(local->host_timerid);
        local->host_timerid    = 0;
        local->assigned_signal = 0;
    }

    /*
    ** Delete the task associated with this timebase
    */
    taskDelete(local->handler_task);
    local->handler_task = 0;

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
