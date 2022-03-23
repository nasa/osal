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
 *
 * Declarations and prototypes for timebase abstraction
 */

#ifndef OSAPI_TIMEBASE_H
#define OSAPI_TIMEBASE_H

#include "osconfig.h"
#include "common_types.h"

/*
** Typedefs
*/
typedef uint32 (*OS_TimerSync_t)(osal_id_t timer_id); /**< @brief Timer sync */

/** @brief Time base properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
    uint32    nominal_interval_time;
    uint32    freerun_time;
    uint32    accuracy;
} OS_timebase_prop_t;

/** @defgroup OSAPITimebase OSAL Time Base APIs
 * @{
 */

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
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[out]  timebase_id     will be set to the non-zero ID of the newly-created resource @nonnull
 * @param[in]   timebase_name   The name of the time base @nonnull
 * @param[in]   external_sync   A synchronization function for BSP hardware-based timer ticks
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_NAME_TAKEN if the name specified is already used
 * @retval #OS_ERR_NO_FREE_IDS if there can be no more timebase resources created
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 * @retval #OS_ERR_NAME_TOO_LONG if the timebase_name is too long
 * @retval #OS_INVALID_POINTER if a pointer argument is NULL
 */
int32 OS_TimeBaseCreate(osal_id_t *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync);

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
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in]   timebase_id     The timebase resource to configure
 * @param[in]   start_time      The amount of delay for the first tick, in microseconds.
 * @param[in]   interval_time   The amount of delay between ticks, in microseconds.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 * @retval #OS_TIMER_ERR_INVALID_ARGS if start_time or interval_time are out of range
 */
int32 OS_TimeBaseSet(osal_id_t timebase_id, uint32 start_time, uint32 interval_time);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes a time base object
 *
 * The helper task and any other resources associated with the time base
 * abstraction will be freed.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in]   timebase_id     The timebase resource to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimeBaseDelete(osal_id_t timebase_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the ID of an existing time base resource
 *
 * Given a time base name, find and output the ID associated with it.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[out]  timebase_id     will be set to the non-zero ID of the matching resource @nonnull
 * @param[in]   timebase_name   The name of the timebase resource to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if timebase_id or timebase_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimeBaseGetIdByName(osal_id_t *timebase_id, const char *timebase_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a timebase resource
 *
 * Fills the buffer referred to by the timebase_prop parameter with
 * relevant information about the time base resource.
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified timebase.
 *
 * @note This configuration API must not be used from the context of a timer callback.
 * Timers should only be configured from the context of normal OSAL tasks.
 *
 * @param[in]   timebase_id     The timebase resource ID
 * @param[out]  timebase_prop   Buffer to store timebase properties @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * @retval #OS_INVALID_POINTER if the timebase_prop pointer is null
 * @retval #OS_ERR_INCORRECT_OBJ_STATE if called from timer/timebase context
 */
int32 OS_TimeBaseGetInfo(osal_id_t timebase_id, OS_timebase_prop_t *timebase_prop);

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
 * @param[out]  freerun_val Buffer to store the free run counter @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid timebase
 * @retval #OS_INVALID_POINTER if pointer argument is NULL
 */
int32 OS_TimeBaseGetFreeRun(osal_id_t timebase_id, uint32 *freerun_val);

/**@}*/

#endif /* OSAPI_TIMEBASE_H */
