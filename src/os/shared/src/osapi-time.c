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

/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
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
#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"
#include "os-shared-time.h"
#include "os-shared-task.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_TIMERS) || (OS_MAX_TIMERS <= 0)
#error "osconfig.h must define OS_MAX_TIMERS to a valid value"
#endif

OS_timecb_internal_record_t OS_timecb_table[OS_MAX_TIMERS];

typedef union
{
    OS_TimerCallback_t timer_callback_func;
    void *             opaque_arg;
} OS_Timer_ArgWrapper_t;

/****************************************************************************************
                                   Timer API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerCbAPI_Init(void)
{
    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Adds new OSAL Timer based on an existing timebase
 *           Internal function used by TimerCreate and TimerAdd API calls
 *
 *  Arguments:  flags to specify the internal bits to set in the created record
 *
 *  Return:     OS_SUCCESS or error code
 *
 *-----------------------------------------------------------------*/
static int32 OS_DoTimerAdd(osal_id_t *timer_id, const char *timer_name, osal_id_t timebase_ref_id,
                           OS_ArgCallback_t callback_ptr, void *callback_arg, uint32 flags)
{
    int32                          return_code;
    osal_objtype_t                 objtype;
    OS_object_token_t              timebase_token;
    OS_object_token_t              timecb_token;
    OS_object_token_t              listcb_token;
    OS_timecb_internal_record_t *  timecb;
    OS_timecb_internal_record_t *  list_timecb;
    OS_timebase_internal_record_t *timebase;

    /*
     * Check parameters
     *
     * Note "callback_arg" is not checked, because in certain configurations it can be validly null.
     */
    OS_CHECK_POINTER(timer_id);
    OS_CHECK_APINAME(timer_name);
    OS_CHECK_POINTER(callback_ptr);

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    /*
     * Check that the timebase reference is valid
     * If successful, then after this statement, we MUST decrement the refcount
     * if we leave this routine with an error.
     */
    return_code =
        OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_ref_id, &timebase_token);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TIMECB, timer_name, &timecb_token);
    if (return_code == OS_SUCCESS)
    {
        timecb   = OS_OBJECT_TABLE_GET(OS_timecb_table, timecb_token);
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, timebase_token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(timecb_token, timecb, timer_name, timer_name);

        /*
         * transfer ownership so the refcount obtained earlier is now
         * associated with the timecb object, and will be retained until
         * the object is deleted.
         */
        OS_ObjectIdTransferToken(&timebase_token, &timecb->timebase_token);

        timecb->callback_ptr = callback_ptr;
        timecb->callback_arg = callback_arg;
        timecb->flags        = flags;
        timecb->prev_cb      = OS_ObjectIdFromToken(&timecb_token);
        timecb->next_cb      = OS_ObjectIdFromToken(&timecb_token);

        /*
         * Now we need to add it to the time base callback ring, so take the
         * timebase-specific lock to prevent a tick from being processed at this moment.
         */
        OS_TimeBaseLock_Impl(&timebase_token);

        if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timebase->first_cb, &listcb_token) ==
            OS_SUCCESS)
        {
            list_timecb = OS_OBJECT_TABLE_GET(OS_timecb_table, listcb_token);

            timecb->next_cb = OS_ObjectIdFromToken(&listcb_token);
            timecb->prev_cb = list_timecb->prev_cb;

            if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timecb->prev_cb, &listcb_token) ==
                OS_SUCCESS)
            {
                list_timecb->prev_cb = OS_ObjectIdFromToken(&timecb_token);
                list_timecb          = OS_OBJECT_TABLE_GET(OS_timecb_table, listcb_token);
                list_timecb->next_cb = OS_ObjectIdFromToken(&timecb_token);
            }
        }

        timebase->first_cb = OS_ObjectIdFromToken(&timecb_token);

        OS_TimeBaseUnlock_Impl(&timebase_token);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &timecb_token, timer_id);
    }
    else
    {
        OS_ObjectIdRelease(&timebase_token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerAdd(osal_id_t *timer_id, const char *timer_name, osal_id_t timebase_ref_id, OS_ArgCallback_t callback_ptr,
                  void *callback_arg)
{
    return (OS_DoTimerAdd(timer_id, timer_name, timebase_ref_id, callback_ptr, callback_arg, 0));
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static void OS_Timer_NoArgCallback(osal_id_t objid, void *arg)
{
    OS_Timer_ArgWrapper_t Conv;

    /*
     * Note - did not write this as simply *((OS_SimpleCallback_t)arg) because
     * technically you cannot cast a void * to a function pointer.
     */
    Conv.opaque_arg = arg;
    (*Conv.timer_callback_func)(objid);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerCreate(osal_id_t *timer_id, const char *timer_name, uint32 *accuracy, OS_TimerCallback_t callback_ptr)
{
    int32                 return_code;
    osal_id_t             timebase_ref_id;
    OS_Timer_ArgWrapper_t Conv;

    /*
    ** Check Parameters.  Although DoTimerAdd will also
    ** check this stuff, also doing it here avoids unnecessarily
    ** creating and deleting a timebase object in case something is bad.
    */
    OS_CHECK_POINTER(timer_id);
    OS_CHECK_APINAME(timer_name);
    OS_CHECK_POINTER(accuracy);
    OS_CHECK_POINTER(callback_ptr);

    /*
     * Create our dedicated time base object to drive this timer
     */
    return_code = OS_TimeBaseCreate(&timebase_ref_id, timer_name, NULL);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    /*
     * Create the actual timer object based off our dedicated time base
     * The TIMECB_FLAG_DEDICATED_TIMEBASE flag is used to mark this object
     * that the time base object attached to it was automatically created for it
     *
     * Although this passes a function pointer through the "void*" argument
     * to the callback function (technically a no-no), this should be safe
     * because it is already verified by a CompileTimeAssert that
     * sizeof(OS_TimerCallback_t) <= sizeof(void*) on this platform.
     */
    Conv.opaque_arg          = NULL;
    Conv.timer_callback_func = callback_ptr;

    return_code = OS_DoTimerAdd(timer_id, timer_name, timebase_ref_id, OS_Timer_NoArgCallback, Conv.opaque_arg,
                                TIMECB_FLAG_DEDICATED_TIMEBASE);

    /*
     * If returning from this call unsuccessfully, then we need to delete the
     * dedicated timebase object that we just created earlier.
     */
    if (return_code != OS_SUCCESS)
    {
        OS_TimeBaseDelete(timebase_ref_id);
    }
    else
    {
        *accuracy = OS_SharedGlobalVars.MicroSecPerTick;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time)
{
    OS_timecb_internal_record_t *timecb;
    int32                        return_code;
    osal_objtype_t               objtype;
    osal_id_t                    dedicated_timebase_id;
    OS_object_token_t            token;

    dedicated_timebase_id = OS_OBJECT_ID_UNDEFINED;

    ARGCHECK(start_time < (UINT32_MAX / 2), OS_TIMER_ERR_INVALID_ARGS);
    ARGCHECK(interval_time < (UINT32_MAX / 2), OS_TIMER_ERR_INVALID_ARGS);
    ARGCHECK(start_time != 0 || interval_time != 0, OS_TIMER_ERR_INVALID_ARGS);

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMECB, timer_id, &token);
    if (return_code == OS_SUCCESS)
    {
        timecb = OS_OBJECT_TABLE_GET(OS_timecb_table, token);

        OS_TimeBaseLock_Impl(&timecb->timebase_token);

        if ((timecb->flags & TIMECB_FLAG_DEDICATED_TIMEBASE) != 0)
        {
            dedicated_timebase_id = OS_ObjectIdFromToken(&timecb->timebase_token);
        }

        timecb->wait_time     = (int32)start_time;
        timecb->interval_time = (int32)interval_time;

        OS_TimeBaseUnlock_Impl(&timecb->timebase_token);

        OS_ObjectIdRelease(&token);
    }

    /*
     * If the timer uses a dedicated time base, then also change that to the same interval.
     *
     * This is not perfect in the sense that the update is not all done atomically.  If this
     * is not the first call to TimerSet, then weirdness can happen:
     *  - a timer tick could occur between setting the callback times above and now
     *  - the call to OS_TimeBaseSet() could fail leaving us in a mixed state
     *
     * However, the notion of multiple TimerSet() calls is a gray area to begin with, since
     * timer ticks can occur at any moment - there is never a guarantee your new TimerSet values
     * will get applied before the old timer expires.  Therefore by definition an application
     * MUST be able to handle a possible "spurious" callback in these circumstances.
     */
    if (return_code == OS_SUCCESS && OS_ObjectIdDefined(dedicated_timebase_id))
    {
        return_code = OS_TimeBaseSet(dedicated_timebase_id, start_time, interval_time);
    }
    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerDelete(osal_id_t timer_id)
{
    int32                          return_code;
    osal_objtype_t                 objtype;
    osal_id_t                      dedicated_timebase_id;
    OS_object_token_t              timecb_token;
    OS_object_token_t              timebase_token;
    OS_object_token_t              listcb_token;
    OS_timebase_internal_record_t *timebase;
    OS_timecb_internal_record_t *  timecb;
    OS_timecb_internal_record_t *  list_timecb;

    dedicated_timebase_id = OS_OBJECT_ID_UNDEFINED;
    memset(&timebase_token, 0, sizeof(timebase_token));

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TIMECB, timer_id, &timecb_token);
    if (return_code == OS_SUCCESS)
    {
        timecb   = OS_OBJECT_TABLE_GET(OS_timecb_table, timecb_token);
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, timecb->timebase_token);

        OS_ObjectIdTransferToken(&timecb->timebase_token, &timebase_token);

        OS_TimeBaseLock_Impl(&timecb->timebase_token);

        /*
         * If the timer uses a dedicated time base, then also delete that.
         */
        if ((timecb->flags & TIMECB_FLAG_DEDICATED_TIMEBASE) != 0)
        {
            dedicated_timebase_id = OS_ObjectIdFromToken(&timecb->timebase_token);
        }

        /*
         * Now we need to remove it from the time base callback ring
         */
        if (OS_ObjectIdEqual(timebase->first_cb, OS_ObjectIdFromToken(&timecb_token)))
        {
            if (OS_ObjectIdEqual(OS_ObjectIdFromToken(&timecb_token), timecb->next_cb))
            {
                timebase->first_cb = OS_OBJECT_ID_UNDEFINED;
            }
            else
            {
                timebase->first_cb = timecb->next_cb;
            }
        }

        if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timecb->prev_cb, &listcb_token) ==
            OS_SUCCESS)
        {
            list_timecb          = OS_OBJECT_TABLE_GET(OS_timecb_table, listcb_token);
            list_timecb->next_cb = timecb->next_cb;
        }
        if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, timecb->next_cb, &listcb_token) ==
            OS_SUCCESS)
        {
            list_timecb          = OS_OBJECT_TABLE_GET(OS_timecb_table, listcb_token);
            list_timecb->prev_cb = timecb->prev_cb;
        }

        timecb->next_cb = OS_ObjectIdFromToken(&timecb_token);
        timecb->prev_cb = OS_ObjectIdFromToken(&timecb_token);

        OS_TimeBaseUnlock_Impl(&timecb->timebase_token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &timecb_token);
    }

    /*
     * Remove the reference count against the timebase
     */
    OS_ObjectIdRelease(&timebase_token);

    /*
     * If the timer uses a dedicated time base, then also delete it.
     */
    if (return_code == OS_SUCCESS && OS_ObjectIdDefined(dedicated_timebase_id))
    {
        OS_TimeBaseDelete(dedicated_timebase_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name)
{
    int32          return_code;
    osal_objtype_t objtype;

    /* Check parameters */
    OS_CHECK_POINTER(timer_id);
    OS_CHECK_POINTER(timer_name);

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TIMECB, timer_name, timer_id);

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop)
{
    OS_common_record_t *           record;
    int32                          return_code;
    osal_objtype_t                 objtype;
    OS_object_token_t              token;
    OS_timecb_internal_record_t *  timecb;
    OS_timebase_internal_record_t *timebase;

    /* Check parameters */
    OS_CHECK_POINTER(timer_prop);

    /*
     * Check our context.  Not allowed to use the timer API from a timer callback.
     * Just interested in the object type returned.
     */
    objtype = OS_ObjectIdToType_Impl(OS_TaskGetId_Impl());
    if (objtype == OS_OBJECT_TYPE_OS_TIMEBASE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    memset(timer_prop, 0, sizeof(OS_timer_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TIMECB, timer_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record   = OS_OBJECT_TABLE_GET(OS_global_timecb_table, token);
        timecb   = OS_OBJECT_TABLE_GET(OS_timecb_table, token);
        timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, timecb->timebase_token);

        strncpy(timer_prop->name, record->name_entry, sizeof(timer_prop->name) - 1);
        timer_prop->creator       = record->creator;
        timer_prop->interval_time = (uint32)timecb->interval_time;
        timer_prop->accuracy      = timebase->accuracy_usec;

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}
