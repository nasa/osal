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
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains the OSAL Timebase API for QT systems.
 *
 * This implementation depends on the QT Timer API which may not be available
 * in older versions of the Linux kernel. It was developed and tested on
 * RHEL 5 ./ CentOS 5 with Linux kernel 2.6.18
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "os-impl-timebase.h"

#include "os-shared-timebase.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

static OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES] = {0};


/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/

static OS_impl_timebase_internal_record_t * GetTableById(osal_id_t id){
    if(id < 1 || OS_MAX_TIMEBASES < id)
        return 0x0;
    if(OS_impl_timebase_table[i -1]->timer == 0x0)
        return 0x0;
    return &OS_impl_timebase_table[i-1];

}

static int GetUnsuedEntryIndex(){
    for(int i=0;i<OS_MAX_TIMEBASES;i++){
        if(OS_impl_timebase_table[i]->timer == 0x0)
            return i;
    }
    return -1;
}


/****************************************************************************************
                                EXTERNAL FUNCTION PROTOTYPES
 ***************************************************************************************/


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the ID of an existing time base resource
 *
 * Given a time base name, find and output the ID associated with it.
 *
 * @param[out]  timebase_id     The timebase resource ID
 * @param[in]   timebase_name   The name of the timebase resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if timebase_id or timebase_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_TimeBaseGetIdByName(osal_id_t *timebase_id, const char *timebase_name){
    if(timebase_name == NULL)
        return OS_INVALID_POINTER;
    for(int i=0;i<OS_MAX_TIMEBASES;i++){
        OS_impl_timebase_internal_record_t * local = &OS_impl_timebase_table[i];
        if(local->timer == 0x0){
            continue;
        }
        if(strncmp(local->name, timebase_name, sizeof(local->name)) == 0){
            *timebase_id = i+1;
            return OS_SUCCESS;
        }

    }
    return OS_ERR_NAME_NOT_FOUND;
}
/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create an abstract Time Base resource
 *
 * An OSAL time base is an abstraction of a "timer tick" that can, in turn, be
 * used for measurement of elapsed time between events.
 *
 * Time bases can be simulated by the operating system using the OS kernel-provided
 * timing facilities, or based on a hardware timing source if provided by the BSP.
 *
 * A time base object has a servicing task associated with it, that runs at elevated
 * priority and will thereby interrupt user-level tasks when timing ticks occur.
 *
 * If the external_sync function is passed as NULL, the operating system kernel
 * timing resources will be utilized for a simulated timer tick.
 *
 * If the external_sync function is not NULL, this should point to a BSP-provided
 * function that will block the calling task until the next tick occurs.  This
 * can be used for synchronizing with hardware events.
 *
 * @note When provisioning a tunable RTOS kernel, such as RTEMS, the kernel should
 * be configured to support at least (OS_MAX_TASKS + OS_MAX_TIMEBASES) threads,
 * to account for the helper threads associated with time base objects.
 *
 * @param[out]  timebase_id     A non-zero ID corresponding to the timebase resource
 * @param[in]   timebase_name   The name of the time base
 * @param[in]   external_sync   A synchronization function for BSP hardware-based timer ticks
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TimeBaseCreate(osal_id_t *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync){
    return OS_ERR_NOT_IMPLEMENTED; /* todo */

}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the tick period for simulated time base objects
 *
 * This sets the actual tick period for timing ticks that are
 * simulated by the RTOS kernel (i.e. the "external_sync" parameter
 * on the call to OS_TimeBaseCreate() is NULL).
 *
 * The RTOS will be configured to wake up the helper thread at the
 * requested interval.
 *
 * This function has no effect for time bases that are using
 * a BSP-provided external_sync function.
 *
 * @param[in]   timebase_id     The timebase resource to configure
 * @param[in]   start_time      The amount of delay for the first tick, in microseconds.
 * @param[in]   interval_time   The amount of delay between ticks, in microseconds.
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TimeBaseSet(osal_id_t timebase_id, uint32 _start_time, uint32 _interval_time){
    OS_impl_timebase_internal_record_t * local = GetTableById(timebase_id);
    if(local == 0x0){
        return OS_ERROR;
    }

    int start_time = _start_time / 1000;
    int interval_time = _interval_time / 1000;

    if((start_time*1000) != _start_time){
        OS_DEBUG("WARNING: timer %lu start_time requested=%luus, configured=%luus\n",
                timebase_id, (unsigned long)_start_time,
                start_time*1000);
    }
    if((interval_time*1000) != _interval_time){
        OS_DEBUG("WARNING: timer %lu start_time requested=%luus, configured=%luus\n",
                timebase_id, (unsigned long)_interval_time,
                interval_time*1000);
    }


}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a time base object
 *
 * The helper task and any other resources associated with the time base
 * abstraction will be freed.
 *
 * @param[in]   timebase_id     The timebase resource to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TimeBaseDelete(osal_id_t timebase_id){
    OS_impl_timebase_internal_record_t * local = GetTableById(timebase_id);
    if(local == 0x0){
        return OS_ERROR;
    }
    local->timer.stop();
    delete local->timer;
    return OS_SUCCESS;
}


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a timebase resource
 *
 * Fills the buffer referred to by the timebase_prop parameter with
 * relevant information about the time base resource.
 *
 * This function will pass back a pointer to structure that contains
 *             all of the relevant info( name and creator) about the specified timebase.
 *
 * @param[in]   timebase_id     The timebase resource ID
 * @param[out]  timebase_prop   Buffer to store timebase properties
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * @retval #OS_INVALID_POINTER if the timebase_prop pointer is null
 */
int32 OS_TimeBaseGetInfo(osal_id_t timebase_id, OS_timebase_prop_t *timebase_prop){
    OS_impl_timebase_internal_record_t * local = GetTableById(timebase_id);
    if(local == 0x0){
        return OS_ERROR;
    }

    timebase_prop->creator = 0;
    memcpy(timebase_prop->name, local->name, sizeof(timebase_prop->name));
    timebase_prop->accuracy = 1000;
    timebase_prop->nominal_interval_time = local->interval_time * 1000;
    timebase_prop->freerun_time = 0;
    return OS_SUCCESS;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief  Read the value of the timebase free run counter
 *
 * Poll the timer free-running time counter in a lightweight fashion.
 *
 * The free run count is a monotonically increasing value reflecting the
 * total time elapsed since the timebase inception.  Units are the
 * same as the timebase itself, usually microseconds.
 *
 * Applications may quickly and efficiently calculate relative time
 * differences by polling this value and subtracting the previous
 * counter value.
 *
 * The absolute value of this counter is not relevant, because it
 * will "roll over" after 2^32 units of time.  For a timebase with
 * microsecond units, this occurs approximately every 4294 seconds,
 * or about 1.2 hours.
 *
 * @note To ensure consistency of results, the application should
 * sample the value at a minimum of two times the roll over frequency,
 * and calculate the difference between the consecutive samples.
 *
 * @param[in]   timebase_id The timebase to operate on
 * @param[out]  freerun_val Buffer to store the free run counter
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 */
int32 OS_TimeBaseGetFreeRun(osal_id_t timebase_id, uint32 *freerun_val){
    /* TODO */
    return OS_ERR_NOT_IMPLEMENTED;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create a timer object
 *
 * A timer object is a resource that invokes the specified application-provided function
 * upon timer expiration.  Timers may be one-shot or periodic in nature.
 *
 * This function creates a dedicated (hidden) time base object to service this timer,
 * which is created and deleted with the timer object itself.  The internal time base
 * is configured for an OS simulated timer tick at the same interval as the timer.
 *
 * @note clock_accuracy comes from the underlying OS tick value.  The nearest integer
 *       microsecond value is returned, so may not be exact.
 *
 * @warning Depending on the OS, the callback_ptr function may be similar to an
 *          interrupt service routine. Calls that cause the code to block or require
 *          an application context (like sending events) are generally not supported.
 *
 * @param[out]  timer_id        The non-zero resource ID of the timer object
 * @param[in]   timer_name      Name of the timer object
 * @param[out]  clock_accuracy  Expected precision of the timer, in microseconds. This
 *                              is the underlying tick value rounded to the nearest
 *                              microsecond integer.
 * @param[in]   callback_ptr    The function pointer of the timer callback or ISR that
 *                              will be called by the timer. The user’s function is
 *                              declared as follows: <tt> void timer_callback(uint32 timer_id) </tt>
 *                              Where the timer_id is passed in to the function by the OSAL
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if any parameters are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_TAKEN if the name is already in use by another timer.
 * @retval #OS_ERR_NO_FREE_IDS if all of the timers are already allocated.
 * @retval #OS_TIMER_ERR_INVALID_ARGS if the callback pointer is zero.
 * @retval #OS_TIMER_ERR_UNAVAILABLE if the timer cannot be created.
 */
int32 OS_TimerCreate(osal_id_t *timer_id, const char *timer_name, uint32 *clock_accuracy,
                     OS_TimerCallback_t callback_ptr){

    int index = GetUnsuedEntryIndex() + 1;
    if(index < 0){
        return OS_ERROR;
    }
    if(strlen(timebase_name) >= OS_MAX_API_NAME)
        return OS_ERR_NAME_TOO_LONG;

    OS_impl_timebase_internal_record_t * local = &OS_impl_timebase_table[index];
    local->timer = new QTimer();
    if(local->timer == 0x0)
        return OS_ERROR;

    memcpy(local->name, timebase_name, sizeof(local->name));
    local->callback = OS_TimerSync_t;
    *timebase_id = index + 1;


    *clock_accuracy = 1000;

    return OS_SUCCESS;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Add a timer object based on an existing TimeBase resource
 *
 * A timer object is a resource that invokes the specified application-provided function
 * upon timer expiration.  Timers may be one-shot or periodic in nature.
 *
 * This function uses an existing time base object to service this timer, which must
 * exist prior to adding the timer.  The precision of the timer is the same
 * as that of the underlying time base object. Multiple timer objects can be
 * created referring to a single time base object.
 *
 * This routine also uses a different callback function prototype from OS_TimerCreate(),
 * allowing a single opaque argument to be passed to the callback routine.
 * The OSAL implementation does not use this parameter, and may be set NULL.
 *
 * @warning Depending on the OS, the callback_ptr function may be similar to an
 *          interrupt service routine. Calls that cause the code to block or require
 *          an application context (like sending events) are generally not supported.
 *
 * @param[out]  timer_id        The non-zero resource ID of the timer object
 * @param[in]   timer_name      Name of the timer object
 * @param[in]   timebase_id     The time base resource to use as a reference
 * @param[in]   callback_ptr    Application-provided function to invoke
 * @param[in]   callback_arg    Opaque argument to pass to callback function
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TimerAdd(osal_id_t *timer_id, const char *timer_name, osal_id_t timebase_id, OS_ArgCallback_t callback_ptr,
                  void *callback_arg){

}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Configures a periodic or one shot timer
 *
 * This function programs the timer with a start time and an optional interval time.
 * The start time is the time in microseconds when the user callback function will be
 * called. If the interval time is non-zero, the timer will be reprogrammed with that
 * interval in microseconds to call the user callback function periodically. If the start
 * time and interval time are zero, the function will return an error.
 *
 * For a "one-shot" timer, the start_time configures the
 * expiration time, and the interval_time should be passed as
 * zero to indicate the timer is not to be automatically reset.
 *
 * @note The resolution of the times specified is limited to the clock accuracy
 *       returned in the OS_TimerCreate call. If the times specified in the start_msec
 *       or interval_msec parameters are less than the accuracy, they will be rounded
 *       up to the accuracy of the timer.
 *
 * @param[in] timer_id      The timer ID to operate on
 * @param[in] start_time    Time in microseconds to the first expiration
 * @param[in] interval_time Time in microseconds between subsequent intervals, value
 *                          of zero will only call the user callback function once
 *                          after the start_msec time.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the timer_id is not valid.
 * @retval #OS_TIMER_ERR_INTERNAL if there was an error programming the OS timer.
 * @retval #OS_ERROR if both start time and interval time are zero.
 */
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time){

}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a timer resource
 *
 * The application callback associated with the timer will be stopped,
 * and the resources freed for future use.
 *
 * @param[in] timer_id      The timer ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the timer_id is invalid.
 * @retval #OS_TIMER_ERR_INTERNAL if there was a problem deleting the timer in the host OS.
 */
int32 OS_TimerDelete(osal_id_t timer_id){

}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Locate an existing timer resource by name
 *
 * Outputs the ID associated with the given timer, if it exists.
 *
 * @param[out] timer_id      The timer ID corresponding to the name
 * @param[in]  timer_name    The timer name to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if timer_id or timer_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name){

}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets information about an existing timer
 *
 * This function takes timer_id, and looks it up in the OS table. It puts all of the
 * information known about that timer into a structure pointer to by timer_prop.
 *
 * @param[in]  timer_id      The timer ID to operate on
 * @param[out] timer_prop    Buffer containing timer properties
 *                           - creator: the OS task ID of the task that created this timer
 *                           - name: the string name of the timer
 *                           - start_time: the start time in microseconds, if any
 *                           - interval_time: the interval time in microseconds, if any
 *                           - accuracy: the accuracy of the timer in microseconds
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timer
 * @retval #OS_INVALID_POINTER if the timer_prop pointer is null
 */
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop){

}
/**@}*/