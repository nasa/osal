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
 * Declarations and prototypes for general OSAL functions that are not part of a subsystem
 */

#ifndef OSAPI_COMMON_H
#define OSAPI_COMMON_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @brief A set of events that can be used with BSP event callback routines
 */
typedef enum
{
    OS_EVENT_RESERVED = 0, /**< no-op/reserved event id value */

    /**
     * resource/id has been newly allocated but not yet created.
     *
     * This event is invoked from WITHIN the locked region, in
     * the context of the task which is allocating the resource.
     *
     * If the handler returns non-success, the error will be returned
     * to the caller and the creation process is aborted.
     */
    OS_EVENT_RESOURCE_ALLOCATED,

    /**
     * resource/id has been fully created/finalized.
     *
     * Invoked outside locked region, in the context
     * of the task which created the resource.
     *
     * Data object is not used, passed as NULL.
     *
     * Return value is ignored - this is for information purposes only.
     */
    OS_EVENT_RESOURCE_CREATED,

    /**
     * resource/id has been deleted.
     *
     * Invoked outside locked region, in the context
     * of the task which deleted the resource.
     *
     * Data object is not used, passed as NULL.
     *
     * Return value is ignored - this is for information purposes only.
     */
    OS_EVENT_RESOURCE_DELETED,

    /**
     * New task is starting.
     *
     * Invoked outside locked region, in the context
     * of the task which is currently starting, before
     * the entry point is called.
     *
     * Data object is not used, passed as NULL.
     *
     * If the handler returns non-success, task startup is aborted
     * and the entry point is not called.
     */
    OS_EVENT_TASK_STARTUP,

    OS_EVENT_MAX /**< placeholder for end of enum, not used */
} OS_Event_t;

/**
 * @brief A callback routine for event handling.
 *
 * @param[in]    event      The event that occurred
 * @param[in]    object_id  The associated object_id, or 0 if not associated with an object
 * @param[inout] data       An abstract data/context object associated with the event, or NULL.
 * @return status Execution status, see @ref OSReturnCodes.
 */
typedef int32 (*OS_EventHandler_t)(OS_Event_t event, osal_id_t object_id, void *data);

/** @defgroup OSAPICore OSAL Core Operation APIs
 *
 * These are for OSAL core operations for startup/initialization, running, and shutdown.
 * Typically only used in bsps, unit tests, psps, etc.
 *
 * Not intended for user application use
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Application startup
 *
 * Startup abstraction such that the same BSP can be used for operations and testing.
 */
void OS_Application_Startup(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Application run
 *
 * Run abstraction such that the same BSP can be used for operations and testing.
 */
void OS_Application_Run(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initialization of API
 *
 * This function returns initializes the internal data structures of the OS
 * Abstraction Layer. It must be called in the application startup code before
 * calling any other OS routines.
 *
 * @return Execution status, see @ref OSReturnCodes. Any error code (negative)
 *         means the OSAL can not be initialized.  Typical platform specific response
 *         is to abort since additional OSAL calls will have undefined behavior.
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR   @copybrief OS_ERROR @covtest
 */
int32 OS_API_Init(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Teardown/de-initialization of OSAL API
 *
 * This is the inverse of OS_API_Init().  It will release all OS resources and
 * return the system to a state similar to what it was prior to invoking
 * OS_API_Init() initially.
 *
 * Normally for embedded applications, the OSAL is initialized after boot and will remain
 * initialized in memory until the processor is rebooted.  However for testing and
 * development purposes, it is potentially useful to reset back to initial conditions.
 *
 * For testing purposes, this API is designed/intended to be compatible with the
 * UtTest_AddTeardown() routine provided by the UT-Assert subsystem.
 *
 * @note This is a "best-effort" routine and it may not always be possible/guaranteed
 * to recover all resources, particularly in the case of off-nominal conditions, or if
 * a resource is used outside of OSAL.
 *
 * For example, while this will attempt to unload all dynamically-loaded modules, doing
 * so may not be possible and/or may induce undefined behavior if resources are in use by
 * tasks/functions outside of OSAL.
 */
void OS_API_Teardown(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Background thread implementation - waits forever for events to occur.
 *
 * This should be called from the BSP main routine or initial thread after all other
 * board and application initialization has taken place and all other tasks are running.
 *
 * Typically just waits forever until "OS_shutdown" flag becomes true.
 */
void OS_IdleLoop(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief delete all resources created in OSAL.
 *
 * provides a means to clean up all resources allocated by this
 * instance of OSAL.  It would typically be used during an orderly
 * shutdown but may also be helpful for testing purposes.
 */
void OS_DeleteAllObjects(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initiate orderly shutdown
 *
 * Indicates that the OSAL application should perform an orderly shutdown
 * of ALL tasks, clean up all resources, and exit the application.
 *
 * This allows the task currently blocked in OS_IdleLoop() to wake up, and
 * for that function to return to its caller.
 *
 * This is preferred over e.g. OS_ApplicationExit() which exits immediately and
 * does not provide for any means to clean up first.
 *
 * @param[in] flag set to true to initiate shutdown, false to cancel
 */
void OS_ApplicationShutdown(uint8 flag);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exit/Abort the application
 *
 * Indicates that the OSAL application should exit and return control to the OS
 * This is intended for e.g. scripted unit testing where the test needs to end
 * without user intervention.
 *
 * This function does not return.  Production code typically should not ever call this.
 *
 * @note This exits the entire process including tasks that have been created.
 */
void OS_ApplicationExit(int32 Status);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Callback routine registration
 *
 * This hook enables the application code to perform extra platform-specific
 * operations on various system events such as resource creation/deletion.
 *
 * @note Some events are invoked while the resource is "locked" and therefore
 * application-defined handlers for these events should not block or attempt
 * to access other OSAL resources.
 *
 * @param[in] handler The application-provided event handler @nonnull
 * @return Execution status, see @ref OSReturnCodes.
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if handler is NULL
 */
int32 OS_RegisterEventHandler(OS_EventHandler_t handler);

/**@}*/

#endif /* OSAPI_COMMON_H */
