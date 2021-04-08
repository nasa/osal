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
 * Declarations and prototypes for task abstraction
 */

#ifndef OSAPI_TASK_H
#define OSAPI_TASK_H

#include "osconfig.h"
#include "common_types.h"

/** @brief Upper limit for OSAL task priorities */
#define OS_MAX_TASK_PRIORITY 255

/** @brief Floating point enabled state for a task */
#define OS_FP_ENABLED 1

/**
 * @brief Type to be used for OSAL task priorities.
 *
 * OSAL priorities are in reverse order, and range
 * from 0 (highest; will preempt all other tasks) to
 * 255 (lowest; will not preempt any other task).
 */
typedef uint8_t osal_priority_t;

#define OSAL_PRIORITY_C(X) ((osal_priority_t) {X})

/**
 * @brief Type to be used for OSAL stack pointer.
 */
typedef void *osal_stackptr_t;

#define OSAL_STACKPTR_C(X)       ((osal_stackptr_t) {X})
#define OSAL_TASK_STACK_ALLOCATE OSAL_STACKPTR_C(NULL)

/** @brief OSAL task properties */
typedef struct
{
    char            name[OS_MAX_API_NAME];
    osal_id_t       creator;
    size_t          stack_size;
    osal_priority_t priority;
} OS_task_prop_t;

/*
** These typedefs are for the task entry point
*/
typedef void osal_task;                      /**< @brief For task entry point */
typedef osal_task((*osal_task_entry)(void)); /**< @brief For task entry point */

/** @defgroup OSAPITask OSAL Task APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a task and starts running it.
 *
 * Creates a task and passes back the id of the task created. Task names must be unique;
 * if the name already exists this function fails. Names cannot be NULL.
 *
 * @param[out]  task_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   task_name the name of the new resource to create
 * @param[in]   function_pointer the entry point of the new task
 * @param[in]   stack_pointer pointer to the stack for the task, or NULL
 *              to allocate a stack from the system memory heap
 * @param[in]   stack_size the size of the stack, or 0 to use a default stack size.
 * @param[in]   priority initial priority of the new task
 * @param[in]   flags initial options for the new task
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if any of the necessary pointers are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_INVALID_PRIORITY if the priority is bad
 * @retval #OS_ERR_NO_FREE_IDS if there can be no more tasks created
 * @retval #OS_ERR_NAME_TAKEN if the name specified is already used by a task
 * @retval #OS_ERROR if an unspecified/other error occurs
 */
int32 OS_TaskCreate(osal_id_t *task_id, const char *task_name, osal_task_entry function_pointer,
                    osal_stackptr_t stack_pointer, size_t stack_size, osal_priority_t priority, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Task
 *
 * The task will be removed from the local tables. and the OS will
 * be configured to stop executing the task at the next opportunity.
 *
 * @param[in] task_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID given to it is invalid
 * @retval #OS_ERROR if the OS delete call fails
 */
int32 OS_TaskDelete(osal_id_t task_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exits the calling task
 *
 * The calling thread is terminated.  This function does not return.
 */
void OS_TaskExit(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Installs a handler for when the task is deleted.
 *
 * This function is used to install a callback that is called when the task is deleted.
 * The callback is called when OS_TaskDelete is called with the task ID. A task delete
 * handler is useful for cleaning up resources that a task creates, before the task is
 * removed from the system.
 *
 * @param[in]   function_pointer function to be called when task exits
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Delay a task for specified amount of milliseconds
 *
 * Causes the current thread to be suspended from execution for the period of millisecond.
 * This is a scheduled wait (clock_nanosleep/rtems_task_wake_after/taskDelay), not a "busy" wait.
 *
 * @param[in]   millisecond    Amount of time to delay
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if sleep fails or millisecond = 0
 */
int32 OS_TaskDelay(uint32 millisecond);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the given task to a new priority
 *
 * @param[in] task_id        The object ID to operate on
 *
 * @param[in] new_priority   Set the new priority
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID passed to it is invalid
 * @retval #OS_ERR_INVALID_PRIORITY if the priority is greater than the max allowed
 * @retval #OS_ERROR if the OS call to change the priority fails
 */
int32 OS_TaskSetPriority(osal_id_t task_id, osal_priority_t new_priority);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the task id of the calling task
 *
 * This function returns the task id of the calling task
 *
 * @return Task ID, or zero if the operation failed (zero is never a valid task ID)
 */
osal_id_t OS_TaskGetId(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing task ID by name
 *
 * This function tries to find a task Id given the name of a task
 *
 * @param[out]  task_id will be set to the ID of the existing resource
 * @param[in]   task_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if the pointers passed in are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name wasn't found in the table
 */
int32 OS_TaskGetIdByName(osal_id_t *task_id, const char *task_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (creator, stack size, priority, name) about the
 * specified task.
 *
 * @param[in]   task_id The object ID to operate on
 * @param[out]  task_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the ID passed to it is invalid
 * @retval #OS_INVALID_POINTER if the task_prop pointer is NULL
 */
int32 OS_TaskGetInfo(osal_id_t task_id, OS_task_prop_t *task_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Reverse-lookup the OSAL task ID from an operating system ID
 *
 * This provides a method by which an external entity may find the OSAL task
 * ID corresponding to a system-defined identifier (e.g. TASK_ID, pthread_t, rtems_id, etc).
 *
 * Normally OSAL does not expose the underlying OS-specific values to the application,
 * but in some circumstances, such as exception handling, the OS may provide this information
 * directly to a BSP handler outside of the normal OSAL API.
 *
 * @param[out]  task_id         The buffer where the task id output is stored
 * @param[in]   sysdata         Pointer to the system-provided identification data
 * @param[in]   sysdata_size    Size of the system-provided identification data
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 */
int32 OS_TaskFindIdBySystemData(osal_id_t *task_id, const void *sysdata, size_t sysdata_size);

/**@}*/

#endif /* OSAPI_TASK_H */
