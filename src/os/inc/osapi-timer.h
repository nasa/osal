/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * The callback function should be declared according to the OS_TimerCallback_t
 * function pointer type.  The timer_id value is passed to the callback function.
 *
 * @note clock_accuracy reports the resolution of the underlying timer source,
 *       rounded to the nearest whole microsecond. On POSIX this comes from
 *       clock_getres(); a resolution below half a microsecond rounds to zero.
 *       This value does not guarantee the latency of callback execution.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @sa OS_TimerCallback_t
 *
 * @param[out]  timer_id        Will be set to the non-zero resource ID of the timer object @nonnull
 * @param[in]   timer_name      Name of the timer object @nonnull
 * @param[out]  clock_accuracy  Reported timer resolution in whole microseconds. @nonnull
 * @param[in]   callback_ptr    The function pointer of the timer callback @nonnull.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if any parameters are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_TAKEN if the name is already in use by another timer.
 * @retval #OS_ERR_NO_FREE_IDS if all of the timers are already allocated.
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if invoked from a timer context
 * @retval #OS_TIMER_ERR_INTERNAL if there was an error programming the OS timer @covtest
 */
int32 OS_TimerCreate(osal_id_t         *timer_id,
                     const char        *timer_name,
                     uint32            *clock_accuracy,
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
 * The callback function for this method should be declared according to the
 * OS_ArgCallback_t function pointer type.  The timer_id is passed in to the
 * function by the OSAL, and the arg parameter is passed through from the
 * callback_arg argument on this call.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @sa OS_ArgCallback_t
 *
 * @param[out]  timer_id        Will be set to the non-zero resource ID of the timer object @nonnull
 * @param[in]   timer_name      Name of the timer object @nonnull
 * @param[in]   timebase_id     The time base resource to use as a reference
 * @param[in]   callback_ptr    Application-provided function to invoke @nonnull
 * @param[in]   callback_arg    Opaque argument to pass to callback function, may be NULL
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if any parameters are NULL
 * @retval #OS_ERR_INVALID_ID if the timebase_id parameter is not valid
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_TAKEN if the name is already in use by another timer.
 * @retval #OS_ERR_NO_FREE_IDS if all of the timers are already allocated.
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if invoked from a timer context
 * @retval #OS_TIMER_ERR_INTERNAL if there was an error programming the OS timer @covtest
 */
int32 OS_TimerAdd(osal_id_t       *timer_id,
                  const char      *timer_name,
                  osal_id_t        timebase_id,
                  OS_ArgCallback_t callback_ptr,
                  void            *callback_arg);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Configures a periodic or one-shot timer
 *
 * Sets the requested delay before the first expiration and the interval between
 * later expirations, both in microseconds. Use a zero interval for a one-shot
 * timer. Passing zero for both parameters is invalid.
 *
 * @note Rounding of the requested times depends on the platform. OS_TimerSet()
 *       does not use the clock_accuracy output of OS_TimerCreate() as a minimum.
 *
 * @note On POSIX, passing a zero start_time with a nonzero interval_time disarms
 *       a timer created by OS_TimerCreate(). Use a nonzero start_time to arm it.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in] timer_id      The timer ID to operate on
 * @param[in] start_time    Requested delay before the first expiration, in microseconds
 * @param[in] interval_time Requested interval between expirations, in microseconds
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the timer_id is not valid.
 * @retval #OS_TIMER_ERR_INTERNAL if there was an error programming the OS timer @covtest
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 * @retval #OS_TIMER_ERR_INVALID_ARGS if the start_time or interval_time is out of range, or both 0
 */
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a timer resource
 *
 * The application callback associated with the timer will be stopped,
 * and the resources freed for future use.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in] timer_id      The timer ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the timer_id is invalid.
 * @retval #OS_TIMER_ERR_INTERNAL if there was a problem deleting the timer in the host OS @covtest
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimerDelete(osal_id_t timer_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Locate an existing timer resource by name
 *
 * Outputs the ID associated with the given timer, if it exists.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[out] timer_id      Will be set to the timer ID corresponding to the name @nonnull
 * @param[in]  timer_name    The timer name to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if timer_id or timer_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets information about an existing timer
 *
 * Looks up the timer by ID and fills the timer_prop buffer with its properties.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in]  timer_id      The timer ID to operate on
 * @param[out] timer_prop    Buffer containing timer properties @nonnull
 *                           - creator: the OS task ID of the task that created this timer
 *                           - name: the string name of the timer
 *                           - start_time: not populated; left at zero
 *                           - interval_time: the stored interval time in microseconds
 *                           - accuracy: the stored accuracy of the timer's time base, in microseconds.
 *                             This can differ from the clock_accuracy output of OS_TimerCreate().
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timer
 * @retval #OS_INVALID_POINTER if the timer_prop pointer is null
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop);
/**@}*/

#endif /* OSAPI_TIMER_H */
