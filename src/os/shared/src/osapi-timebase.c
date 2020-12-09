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
 * \file     osapi-timebase.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         A "timebase" provides the reference for which "timer" objects are based.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

/*
 * User defined include files
 */
#include "os-shared-timebase.h"
#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-task.h"
#include "os-shared-time.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_TIMEBASES) || (OS_MAX_TIMEBASES <= 0)
#error "osconfig.h must define OS_MAX_TIMEBASES to a valid value"
#endif

enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_TIMEBASES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_TIMEBASE
};

OS_timebase_internal_record_t OS_timebase_table[OS_MAX_TIMEBASES];

/*
 * Limit to the number of times that the OS timebase servicing thread
 * is allowed to spin without achieving external sync.
 */
#define OS_TIMEBASE_SPIN_LIMIT 4

/****************************************************************************************
                                   TimeBase API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseAPI_Init(void)
{
    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));
    return OS_SUCCESS;
} /* end OS_TimeBaseAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseCreate(osal_id_t *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    int32                          return_code;
    osal_objtype_t                 objtype;
    OS_object_token_t              token;
    OS_timebase_internal_record_t *timebase;

    /*
     * Specifying a NULL sync function means the timebase is not externally synchronized.
     * In this case an appropriate OS timer will be used to generate the simulated timer tick.
     */

    /*
     ** Check Parameters
     */
    if (timer_id == NULL || timebase_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     ** we don't want to allow names too long
     ** if truncated, two names might be the same
     */
    if (strlen(timebase_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_name, &token);
    if (return_code == OS_SUCCESS)
    {
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, timebase, timebase_name, timebase_name);

        timebase->external_sync = external_sync;
        if (external_sync == NULL)
        {
            timebase->accuracy_usec = OS_SharedGlobalVars.MicroSecPerTick;
        }
        else
        {
            timebase->accuracy_usec = 0;
        }

        /* Now call the OS-specific implementation.  This reads info from the timer table. */
        return_code = OS_TimeBaseCreate_Impl(&token);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, timer_id);
    }

    return return_code;
} /* end OS_TimeBaseCreate */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseSet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time)
{
    int32                          return_code;
    osal_objtype_t                 objtype;
    OS_object_token_t              token;
    OS_timebase_internal_record_t *timebase;

    /*
     * Internally the implementation represents the interval as a
     * signed 32-bit integer, but the parameter is unsigned because a negative interval
     * does not make sense.
     *
     * Note that the units are intentionally left unspecified.  The external sync period
     * could be measured in microseconds or hours -- it is whatever the application requires.
     */
    if (interval_time >= 1000000000 || start_time >= 1000000000)
    {
        return OS_TIMER_ERR_INVALID_ARGS;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMEBASE, timer_id, &token);
    if (return_code == OS_SUCCESS)
    {
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

        /* Need to take the time base lock to ensure that no ticks are currently being processed */
        OS_TimeBaseLock_Impl(&token);

        return_code = OS_TimeBaseSet_Impl(&token, start_time, interval_time);

        if (return_code == OS_SUCCESS)
        {
            /* Save the value since we were successful */
            timebase->nominal_start_time    = start_time;
            timebase->nominal_interval_time = interval_time;
        }

        OS_TimeBaseUnlock_Impl(&token);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_TimeBaseSet */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseDelete(osal_id_t timer_id)
{
    int32             return_code;
    osal_objtype_t    objtype;
    OS_object_token_t token;

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TIMEBASE, timer_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_TimeBaseDelete_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;
} /* end OS_TimeBaseDelete */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetIdByName(osal_id_t *timer_id, const char *timebase_name)
{
    int32          return_code;
    osal_objtype_t objtype;

    if (timer_id == NULL || timebase_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_name, timer_id);

    return return_code;
} /* end OS_TimeBaseGetIdByName */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo(osal_id_t timebase_id, OS_timebase_prop_t *timebase_prop)
{
    OS_common_record_t *           record;
    int32                          return_code;
    osal_objtype_t                 objtype;
    OS_object_token_t              token;
    OS_timebase_internal_record_t *timebase;

    /* Check parameters */
    if (timebase_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    memset(timebase_prop, 0, sizeof(OS_timebase_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, timebase_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record   = OS_OBJECT_TABLE_GET(OS_global_timebase_table, token);
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

        strncpy(timebase_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
        timebase_prop->creator               = record->creator;
        timebase_prop->nominal_interval_time = timebase->nominal_interval_time;
        timebase_prop->freerun_time          = timebase->freerun_time;
        timebase_prop->accuracy              = timebase->accuracy_usec;

        return_code = OS_TimeBaseGetInfo_Impl(&token, timebase_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_TimeBaseGetInfo */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBaseGetFreeRun
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimeBaseGetFreeRun(osal_id_t timebase_id, uint32 *freerun_val)
{
    int32                          return_code;
    OS_object_token_t              token;
    OS_timebase_internal_record_t *timebase;

    /* Check parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, timebase_id, &token);
    if (return_code == OS_SUCCESS)
    {
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

        *freerun_val = timebase->freerun_time;
    }

    return return_code;
} /* end OS_TimeBaseGetFreeRun */

/*----------------------------------------------------------------
 *
 * Function: OS_TimeBase_CallbackThread
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implementation of the time base "helper thread"
 *
 *           This is executed in a dedicated thread context (typically elevated priority)
 *           and performs two basic functions:
 *             1) call the BSP-specified delay routine to sync with the time reference (tick)
 *             2) process the requested Application callbacks each time the tick occurs
 *
 *    Returns: None.
 *
 *    Note: Application callbacks will be done under this thread context.
 *          Doing callbacks directly as an ISR or signal handler can be dangerous, as the
 *          available C library calls are very limited in that context.
 *
 *-----------------------------------------------------------------*/
void OS_TimeBase_CallbackThread(osal_id_t timebase_id)
{
    OS_TimerSync_t                 syncfunc;
    OS_timebase_internal_record_t *timebase;
    OS_timecb_internal_record_t *  timecb;
    OS_common_record_t *           record;
    OS_object_token_t              token;
    OS_object_token_t              cb_token;
    uint32                         tick_time;
    uint32                         spin_cycles;
    int32                          saved_wait_time;

    /*
     * Register this task as a time base handler.
     * Application code MUST NOT attempt to configure timers from the context of a
     * timer callback, otherwise deadlock is possible.  Additional checks prevent this.
     *
     */
    OS_TaskRegister_Impl(timebase_id);

    /* Grab the relevant info from the global structure */
    if (OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id, &token) != 0)
    {
        /* Something went wrong - abort this thread */
        return;
    }

    record   = OS_OBJECT_TABLE_GET(OS_global_timebase_table, token);
    timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, token);

    syncfunc    = timebase->external_sync;
    spin_cycles = 0;

    OS_ObjectIdRelease(&token);

    while (1)
    {
        /*
         * Call the sync function - this will pend for some period of time
         * and return the amount of elapsed time in units of "timebase ticks"
         */
        tick_time = (*syncfunc)(timebase_id);

        /*
         * The returned tick_time should be nonzero.  If the sync function
         * returns zero, then it means something went wrong and it is not
         * known how many ticks have elapsed.
         *
         * This can validly occur, for instance, if the underlying wait
         * operation was interrupted for some reason, e.g. EINTR or EAGAIN
         * on a POSIX-like OS.
         *
         * In some instances it is appropriate to simply call the
         * function again.  However, since this task typically runs as a
         * high-priority thread, it is prudent to limit such spinning.
         */
        if (tick_time != 0)
        {
            /* nominal case - reset counter */
            spin_cycles = 0;
        }
        else if (spin_cycles < OS_TIMEBASE_SPIN_LIMIT)
        {
            /* off-nominal but OK for now */
            ++spin_cycles;
        }
        else
        {
            /*
             * Spin-loop detected.
             * Just call OS_TaskDelay_Impl() to yield the CPU.
             * generate a debug warning only on the first time
             * so the operator knows this is happening.
             */
            OS_TaskDelay_Impl(10);

            if (spin_cycles == OS_TIMEBASE_SPIN_LIMIT)
            {
                ++spin_cycles;
                OS_DEBUG("WARNING: Timebase Sync Spin Loop detected\n");
            }
        }

        OS_TimeBaseLock_Impl(&token);

        /*
         * After waiting, check that our ID still matches
         * If not then it means this time base got deleted....
         */
        if (!OS_ObjectIdEqual(timebase_id, record->active_id))
        {
            OS_TimeBaseUnlock_Impl(&token);
            break;
        }

        timebase->freerun_time += tick_time;
        if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timebase->first_cb, &cb_token) == 0)
        {
            do
            {
                timecb          = OS_OBJECT_TABLE_GET(OS_timecb_table, cb_token);
                saved_wait_time = timecb->wait_time;
                timecb->wait_time -= tick_time;
                while (timecb->wait_time <= 0)
                {
                    timecb->wait_time += timecb->interval_time;

                    /*
                     * Only allow the "wait_time" underflow to go as far negative as one interval time
                     * This prevents a cb "interval_time" of less than the timebase interval_time from
                     * accumulating infinitely
                     */
                    if (timecb->wait_time < -timecb->interval_time)
                    {
                        ++timecb->backlog_resets;
                        timecb->wait_time = -timecb->interval_time;
                    }

                    /*
                     * Only give the callback if the wait_time actually transitioned from positive to negative.
                     * This allows one-shot operation where the API sets the "wait_time" positive but keeps
                     * the "interval_time" at zero.  With the interval_time at zero the wait time will never
                     * go positive again unless the API sets it again.
                     */
                    if (saved_wait_time > 0 && timecb->callback_ptr != NULL)
                    {
                        (*timecb->callback_ptr)(OS_ObjectIdFromToken(&cb_token), timecb->callback_arg);
                    }

                    /*
                     * Do not repeat the loop unless interval_time is configured.
                     */
                    if (timecb->interval_time <= 0)
                    {
                        break;
                    }
                }

            } while (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timecb->next_cb, &cb_token) == OS_SUCCESS &&
                    !OS_ObjectIdEqual(OS_ObjectIdFromToken(&cb_token), timebase->first_cb));
        }

        OS_TimeBaseUnlock_Impl(&token);
    }
} /* end OS_TimeBase_CallbackThread */

/*----------------------------------------------------------------
 *
 * Function: OS_Milli2Ticks
 *
 *  Purpose: Internal helper to convert milliseconds to ticks
 *
 *  Returns: OS_SUCCESS on success, OS_ERROR on failure (rollover)
 *
 *-----------------------------------------------------------------*/
int32 OS_Milli2Ticks(uint32 milli_seconds, int *ticks)
{
    uint64 num_of_ticks;
    int32  return_code = OS_SUCCESS;

    num_of_ticks = (((uint64)milli_seconds * OS_SharedGlobalVars.TicksPerSecond) + 999) / 1000;

    /* Check against maximum int32 (limit from some OS's) */
    if (num_of_ticks <= INT_MAX)
    {
        *ticks = (int)num_of_ticks;
    }
    else
    {
        return_code = OS_ERROR;
        *ticks      = 0;
    }

    return return_code;
} /* end OS_Milli2Ticks */
