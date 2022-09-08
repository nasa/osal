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
 * Declarations and prototypes for condition variables
 */

#ifndef OSAPI_CONDVAR_H
#define OSAPI_CONDVAR_H

#include "osconfig.h"
#include "common_types.h"

#include "osapi-clock.h" /* required for OS_time_t definition */

/** @brief OSAL condition variable properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_condvar_prop_t;

/** @defgroup OSAPICondVar OSAL Condition Variable APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a condition variable resource
 *
 * A condition variable adds a more sophisticated synchronization option for mutexes,
 * such that it can operate on arbitrary user-defined conditions rather than simply a
 * counter or boolean (as in the case of simple semaphores).
 *
 * Creating a condition variable resource in OSAL will in turn create both a basic mutex
 * as well as a synchronization overlay.  The underlying mutex is similar to the mutex
 * functionality provided by the OSAL mutex subsystem, and can be locked and unlocked
 * normally.
 *
 * This mutex is intended to protect access to any arbitrary user-defined data object that
 * serves as the condition being tested.
 *
 * A task that needs a particular state of the object should follow this general flow:
 *  - Lock the underlying mutex
 *  - Test for the condition being waited for (a user-defined check on user-defined data)
 *  - If condition IS NOT met, then call OS_CondVarWait() to wait, then repeat test
 *  - If condition IS met, then unlock the underlying mutex and continue
 *
 * A task that changes the state of the object should follow this general flow:
 *  - Lock the underlying mutex
 *  - Change the state as necessary
 *  - Call either OS_CondVarSignal() or OS_CondVarBroadcast()
 *  - Unlock the underlying mutex
 *
 * @param[out]  var_id will be set to the non-zero ID of the newly-created resource @nonnull
 * @param[in]   var_name the name of the new resource to create @nonnull
 * @param[in]   options reserved for future use.  Should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if var_id or var_name are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are no more free condition variable Ids
 * @retval #OS_ERR_NAME_TAKEN if there is already a condition variable with the same name
 */
int32 OS_CondVarCreate(osal_id_t *var_id, const char *var_name, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Locks/Acquires the underlying mutex associated with a condition variable
 *
 * The mutex should always be locked by a task before reading or modifying the
 * data object associated with a condition variable.
 *
 * @note This lock must be acquired by a task before invoking OS_CondVarWait() or
 * OS_CondVarTimedWait() on the same condition variable.
 *
 * @param[in] var_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid condition variable
 */
int32 OS_CondVarLock(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unlocks/Releases the underlying mutex associated with a condition variable
 *
 * The mutex should be unlocked by a task once reading or modifying the
 * data object associated with a condition variable is complete.
 *
 * @param[in] var_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid condition variable
 */
int32 OS_CondVarUnlock(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Signals the condition variable resource referenced by var_id.
 *
 * This function may be used to indicate when the state of a data object has been
 * changed.
 *
 * If there are threads blocked on the condition variable object referenced by
 * var_id when this function is called, one of those threads will be unblocked,
 * as determined by the scheduling policy.
 *
 * @param[in] var_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid condition variable
 */
int32 OS_CondVarSignal(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Broadcasts the condition variable resource referenced by var_id.
 *
 * This function may be used to indicate when the state of a data object has been
 * changed.
 *
 * If there are threads blocked on the condition variable object referenced by
 * var_id when this function is called, all threads will be unblocked.
 *
 * Note that although all threads are unblocked, because the mutex is re-acquired
 * before the wait function returns, only a single task will be testing the condition
 * at a given time.  The order with which each blocked task runs is determined by the
 * scheduling policy.
 *
 * @param[in] var_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid condition variable
 */
int32 OS_CondVarBroadcast(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Waits on the condition variable object referenced by var_id.
 *
 * The calling task will be blocked until another task calls the function
 * OS_CondVarSignal() or OS_CondVarBroadcast() on the same condition variable.
 *
 * The underlying mutex associated with the condition variable must be locked and
 * owned by the calling task at the time this function is invoked.  As part of this
 * call, the mutex will be unlocked as the task blocks.  This is done in such a way
 * that there is no possibility that another task could aquire the mutex before the
 * calling task has actually blocked.
 *
 * This atomicity with respect to blocking the task and unlocking the mutex is a
 * critical difference between condition variables and other synchronization
 * primitives.  It avoids a window of opportunity where inherent in the simpler
 * synchronization resource types where the state of the data could change between
 * the time that the calling task tested the state and the time that the task
 * actually blocks on the sync resource.
 *
 * @param[in] var_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid condvar
 */
int32 OS_CondVarWait(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Time-limited wait on the condition variable object referenced by var_id.
 *
 * Identical in operation to OS_CondVarWait(), except that the maximum amount of
 * time that the task will be blocked is limited.
 *
 * The abs_wakeup_time refers to the absolute time of the system clock at which the
 * task should be unblocked to run, regardless of the state of the condition variable.
 * This refers to the same system clock that is the subject of the OS_GetLocalTime() API.
 *
 * @param[in] var_id The object ID to operate on
 * @param[in] abs_wakeup_time The system time at which the task should be unblocked @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid condvar
 */
int32 OS_CondVarTimedWait(osal_id_t var_id, const OS_time_t *abs_wakeup_time);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified condition variable.
 *
 * Delete the condition variable and releases any related system resources.
 *
 * @param[in] var_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid condvar
 */
int32 OS_CondVarDelete(osal_id_t var_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing condition variable ID by name
 *
 * This function tries to find an existing condition variable ID given the name.
 * The id is returned through var_id.
 *
 * @param[out] var_id will be set to the ID of the existing resource
 * @param[in]  var_name the name of the existing resource to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is var_id or var_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_CondVarGetIdByName(osal_id_t *var_id, const char *var_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will fill a structure to contain the information
 * (name and creator) about the specified condition variable.
 *
 * @param[in]  var_id The object ID to operate on
 * @param[out] condvar_prop The property object buffer to fill @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the mut_prop pointer is null
 */
int32 OS_CondVarGetInfo(osal_id_t var_id, OS_condvar_prop_t *condvar_prop);
/**@}*/

#endif /* OSAPI_CONDVAR_H */
