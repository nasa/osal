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
 * \file
 *
 * Declarations and prototypes for timer abstraction (app callbacks)
 */

#ifndef OSAPI_TIMER_H
#define OSAPI_TIMER_H

#include "osconfig.h"
#include "common_types.h"

/*
** Typedefs
*/
typedef void (*OS_TimerCallback_t)(osal_id_t timer_id); /**< @brief Timer callback */

/** @brief Timer properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
    uint32    start_time;
    uint32    interval_time;
    uint32    accuracy;

} OS_timer_prop_t;

/** @defgroup OSAPITimer OSAL Timer APIs
 * @{
 */

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
                     OS_TimerCallback_t callback_ptr);

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
                  void *callback_arg);

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
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time);

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
int32 OS_TimerDelete(osal_id_t timer_id);

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
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name);

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
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop);
/**@}*/

#endif /* OSAPI_TIMER_H */
