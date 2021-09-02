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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains the OSAL Timebase API for POSIX systems.
 *
 * This implementation depends on the POSIX Timer API which may not be available
 * in older versions of the Linux kernel. It was developed and tested on
 * RHEL 5 ./ CentOS 5 with Linux kernel 2.6.18
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-impl-timebase.h"
#include "os-impl-tasks.h"

#include "os-shared-timebase.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

/****************************************************************************************
                                EXTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

static void OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec);

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
                                     GLOBALS
 ***************************************************************************************/

OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];

/****************************************************************************************
                                INTERNAL FUNCTIONS
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_UsecToTimespec
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Convert Microseconds to a POSIX timespec structure.
 *
 *-----------------------------------------------------------------*/
static void OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
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
} /* end OS_UsecToTimespec */

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

    pthread_mutex_lock(&impl->handler_mutex);
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

    pthread_mutex_unlock(&impl->handler_mutex);
} /* end OS_TimeBaseUnlock_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_SoftWaitImpl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static uint32 OS_TimeBase_SigWaitImpl(osal_id_t obj_id)
{
    int                                 ret;
    OS_object_token_t                   token;
    OS_impl_timebase_internal_record_t *impl;
    OS_timebase_internal_record_t *     timebase;
    uint32                              interval_time;
    int                                 sig;

    interval_time = 0;

    if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMEBASE, obj_id, &token) == OS_SUCCESS)
    {
        impl     = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, token);
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

        ret = sigwait(&impl->sigset, &sig);

        if (ret != 0)
        {
            /*
             * the sigwait call failed.
             * returning 0 will cause the process to repeat.
             */
        }
        else if (impl->reset_flag == 0)
        {
            /*
             * Normal steady-state behavior.
             * interval_time reflects the configured interval time.
             */
            interval_time = timebase->nominal_interval_time;
        }
        else
        {
            /*
             * Reset/First interval behavior.
             * timer_set() was invoked since the previous interval occurred (if any).
             * interval_time reflects the configured start time.
             */
            interval_time    = timebase->nominal_start_time;
            impl->reset_flag = 0;
        }
    }

    return interval_time;
} /* end OS_TimeBase_SoftWaitImpl */

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/******************************************************************************
 *  Function:  OS_Posix_TimeBaseAPI_Impl_Init
 *
 *  Purpose:  Initialize the timer implementation layer
 *
 *  Arguments:
 *
 *  Return:
 */
int32 OS_Posix_TimeBaseAPI_Impl_Init(void)
{
    int                 status;
    osal_index_t        idx;
    pthread_mutexattr_t mutex_attr;
    struct timespec     clock_resolution;
    int32               return_code;

    return_code = OS_SUCCESS;

    do
    {
        /*
        ** Mark all timers as available
        */
        memset(OS_impl_timebase_table, 0, sizeof(OS_impl_timebase_table));

        /*
        ** get the resolution of the selected clock
        */
        status = clock_getres(OS_PREFERRED_CLOCK, &clock_resolution);
        if (status != 0)
        {
            OS_DEBUG("failed in clock_getres: %s\n", strerror(errno));
            return_code = OS_ERROR;
            break;
        }

        /*
        ** Convert to microseconds
        ** Note that the resolution MUST be in the sub-second range, if not then
        ** it looks like the POSIX timer API in the C library is broken.
        ** Note for any flavor of RTOS we would expect <= 1ms.  Even a "desktop"
        ** linux or development system should be <= 100ms absolute worst-case.
        */
        if (clock_resolution.tv_sec > 0)
        {
            return_code = OS_TIMER_ERR_INTERNAL;
            break;
        }

        /* Round to the nearest microsecond */
        POSIX_GlobalVars.ClockAccuracyNsec = (uint32)(clock_resolution.tv_nsec);

        /*
        ** initialize the attribute with default values
        */
        status = pthread_mutexattr_init(&mutex_attr);
        if (status != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_init failed: %s\n", strerror(status));
            return_code = OS_ERROR;
            break;
        }

        /*
        ** Allow the mutex to use priority inheritance
        */
        status = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
        if (status != 0)
        {
            OS_DEBUG("Error: pthread_mutexattr_setprotocol failed: %s\n", strerror(status));
            return_code = OS_ERROR;
            break;
        }

        for (idx = 0; idx < OS_MAX_TIMEBASES; ++idx)
        {
            /*
            ** create the timebase sync mutex
            ** This gives a mechanism to synchronize updates to the timer chain with the
            ** expiration of the timer and processing the chain.
            */
            status = pthread_mutex_init(&OS_impl_timebase_table[idx].handler_mutex, &mutex_attr);
            if (status != 0)
            {
                OS_DEBUG("Error: Mutex could not be created: %s\n", strerror(status));
                return_code = OS_ERROR;
                break;
            }
        }

        /*
         * Pre-calculate the clock tick to microsecond conversion factor.
         */
        OS_SharedGlobalVars.TicksPerSecond = sysconf(_SC_CLK_TCK);
        if (OS_SharedGlobalVars.TicksPerSecond <= 0)
        {
            OS_DEBUG("Error: Unable to determine OS ticks per second: %s\n", strerror(errno));
            return_code = OS_ERROR;
            break;
        }

        /*
         * Calculate microseconds per tick
         *  - If the ratio is not an integer, this will round to the nearest integer value
         *  - This is used internally for reporting accuracy,
         *  - TicksPerSecond values over 2M will return zero
         */
        OS_SharedGlobalVars.MicroSecPerTick =
            (1000000 + (OS_SharedGlobalVars.TicksPerSecond / 2)) / OS_SharedGlobalVars.TicksPerSecond;
    } while (0);

    return (return_code);
} /* end OS_Posix_TimeBaseAPI_Impl_Init */

/****************************************************************************************
                                   Time Base API
 ***************************************************************************************/

static void *OS_TimeBasePthreadEntry(void *arg)
{
    OS_VoidPtrValueWrapper_t local_arg;

    local_arg.opaque_arg = arg;
    OS_TimeBase_CallbackThread(local_arg.id);
    return NULL;
}

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
    int                                 status;
    int                                 i;
    osal_index_t                        idx;
    struct sigevent                     evp;
    struct timespec                     ts;
    OS_impl_timebase_internal_record_t *local;
    OS_timebase_internal_record_t *     timebase;
    OS_VoidPtrValueWrapper_t            arg;

    local    = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, *token);

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
    arg.opaque_arg = NULL;
    arg.id         = OS_ObjectIdFromToken(token);
    return_code    = OS_Posix_InternalTaskCreate_Impl(&local->handler_thread, OSAL_PRIORITY_C(0), 0,
                                                   OS_TimeBasePthreadEntry, arg.opaque_arg);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    local->assigned_signal = 0;
    clock_gettime(OS_PREFERRED_CLOCK, &local->softsleep);

    /*
     * Set up the necessary OS constructs
     *
     * If an external sync function is used then there is nothing to do here -
     * we simply call that function and it should synchronize to the time source.
     *
     * If no external sync function is provided then this will set up a POSIX
     * timer to locally simulate the timer tick using the CPU clock.
     */
    if (timebase->external_sync == NULL)
    {
        sigemptyset(&local->sigset);

        /*
         * find an RT signal that is not used by another time base object.
         * This is all done while the global lock is held so no chance of the
         * underlying tables changing
         */
        for (idx = 0; idx < OS_MAX_TIMEBASES; ++idx)
        {
            if (OS_ObjectIdIsValid(OS_global_timebase_table[idx].active_id) &&
                OS_impl_timebase_table[idx].assigned_signal != 0)
            {
                sigaddset(&local->sigset, OS_impl_timebase_table[idx].assigned_signal);
            }
        }

        for (i = SIGRTMIN; i <= SIGRTMAX; ++i)
        {
            if (!sigismember(&local->sigset, i))
            {
                local->assigned_signal = i;
                break;
            }
        }

        do
        {
            if (local->assigned_signal == 0)
            {
                OS_DEBUG("No free RT signals to use for simulated time base\n");
                return_code = OS_TIMER_ERR_UNAVAILABLE;
                break;
            }

            sigemptyset(&local->sigset);
            sigaddset(&local->sigset, local->assigned_signal);

            /*
             * Ensure that the chosen signal is NOT already pending.
             *
             * Perform a "sigtimedwait" with a zero timeout to poll the
             * status of the selected signal.  RT signals are also queued,
             * so this needs to be called in a loop to until sigtimedwait()
             * returns an error.
             *
             * The max number of signals that can be queued is available
             * via sysconf() as the _SC_SIGQUEUE_MAX value.
             *
             * The output is irrelevant here; the objective is to just ensure
             * that the signal is not already pending.
             */
            i = sysconf(_SC_SIGQUEUE_MAX);
            do
            {
                ts.tv_sec  = 0;
                ts.tv_nsec = 0;
                if (sigtimedwait(&local->sigset, NULL, &ts) < 0)
                {
                    /* signal is NOT pending */
                    break;
                }
                --i;
            } while (i > 0);

            /*
            **  Initialize the sigevent structures for the handler.
            */
            memset((void *)&evp, 0, sizeof(evp));
            evp.sigev_notify = SIGEV_SIGNAL;
            evp.sigev_signo  = local->assigned_signal;

            /*
             * Pass the Timer Index value of the object ID to the signal handler --
             *  Note that the upper bits can be safely assumed as a timer ID to recreate the original,
             *  and doing it this way should still work on a system where sizeof(sival_int) < sizeof(uint32)
             *  (as long as sizeof(sival_int) >= number of bits in OS_OBJECT_INDEX_MASK)
             */
            evp.sigev_value.sival_int = (int)OS_ObjectIdToSerialNumber_Impl(OS_ObjectIdFromToken(token));

            /*
            ** Create the timer
            ** Note using the "MONOTONIC" clock here as this will still produce consistent intervals
            ** even if the system clock is stepped (e.g. clock_settime).
            */
            status = timer_create(OS_PREFERRED_CLOCK, &evp, &local->host_timerid);
            if (status < 0)
            {
                return_code = OS_TIMER_ERR_UNAVAILABLE;
                break;
            }

            timebase->external_sync = OS_TimeBase_SigWaitImpl;
        } while (0);
    }

    if (return_code != OS_SUCCESS)
    {
        /*
         * NOTE about the thread cancellation -- this technically is just a backup,
         * we should not need to cancel it because the handler thread will exit automatically
         * if the active ID does not match the expected value.  This check would fail
         * if this function returns non-success (the ID in the global will be set zero)
         */
        pthread_cancel(local->handler_thread);
        local->assigned_signal = 0;
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
    OS_timebase_internal_record_t *     timebase;

    local       = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);
    timebase    = OS_OBJECT_TABLE_GET(OS_timebase_table, *token);
    return_code = OS_SUCCESS;

    /* There is only something to do here if we are generating a simulated tick */
    if (local->assigned_signal != 0)
    {
        /*
        ** Convert from Microseconds to timespec structures
        */
        memset(&timeout, 0, sizeof(timeout));
        OS_UsecToTimespec(start_time, &timeout.it_value);
        OS_UsecToTimespec(interval_time, &timeout.it_interval);

        /*
        ** Program the real timer
        */
        status = timer_settime(local->host_timerid, 0, /* Flags field can be zero */
                               &timeout,               /* struct itimerspec */
                               NULL);                  /* Oldvalue */

        if (status < 0)
        {
            OS_DEBUG("Error in timer_settime: %s\n", strerror(errno));
            return_code = OS_TIMER_ERR_INTERNAL;
        }
        else if (interval_time > 0)
        {
            timebase->accuracy_usec = (uint32)((timeout.it_interval.tv_nsec + 999) / 1000);
        }
        else
        {
            timebase->accuracy_usec = (uint32)((timeout.it_value.tv_nsec + 999) / 1000);
        }
    }

    local->reset_flag = (return_code == OS_SUCCESS);
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
    int                                 status;

    local = OS_OBJECT_TABLE_GET(OS_impl_timebase_table, *token);

    pthread_cancel(local->handler_thread);

    /*
    ** Delete the timer
    */
    if (local->assigned_signal != 0)
    {
        status = timer_delete(local->host_timerid);
        if (status < 0)
        {
            OS_DEBUG("Error deleting timer: %s\n", strerror(errno));
            return (OS_TIMER_ERR_INTERNAL);
        }

        local->assigned_signal = 0;
    }

    return OS_SUCCESS;
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
