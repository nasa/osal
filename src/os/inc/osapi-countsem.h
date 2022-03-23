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
 * Declarations and prototypes for counting semaphores
 */

#ifndef OSAPI_COUNTSEM_H
#define OSAPI_COUNTSEM_H

#include "osconfig.h"
#include "common_types.h"

/** @brief OSAL counting semaphore properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
    int32     value;
} OS_count_sem_prop_t;

/** @defgroup OSAPICountSem OSAL Counting Semaphore APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a counting semaphore
 *
 * Creates a counting semaphore with initial value specified by
 * sem_initial_value and name specified by sem_name. sem_id will be
 * returned to the caller.
 *
 * @note Underlying RTOS implementations may or may not impose a specific upper limit
 * to the value of a counting semaphore.  If the OS has a specific limit and the
 * sem_initial_value exceeds this limit, then #OS_INVALID_SEM_VALUE is returned.  On
 * other implementations, any 32-bit integer value may be acceptable.  For maximum
 * portability, it is recommended to keep counting semaphore values within the
 * range of a "short int" (i.e. between 0 and 32767).  Many platforms do accept
 * larger values, but may not be guaranteed.
 *
 * @param[out]  sem_id will be set to the non-zero ID of the newly-created resource @nonnull
 * @param[in]   sem_name the name of the new resource to create @nonnull
 * @param[in]   sem_initial_value the initial value of the counting semaphore
 * @param[in]   options Reserved for future use, should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sen name or sem_id are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
 * @retval #OS_ERR_NAME_TAKEN if this is already the name of a counting semaphore
 * @retval #OS_INVALID_SEM_VALUE if the semaphore value is too high @covtest
 * @retval #OS_SEM_FAILURE if an unspecified implementation error occurs @covtest
 */
int32 OS_CountSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Increment the semaphore value
 *
 * The function  unlocks the semaphore referenced by sem_id by performing
 * a semaphore unlock operation on that semaphore.  If the semaphore value
 * resulting from this operation is positive, then no threads were blocked
 * waiting for the semaphore to become unlocked; the semaphore value is
 * simply incremented for this semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore
 * @retval #OS_SEM_FAILURE if an unspecified implementation error occurs @covtest
 */
int32 OS_CountSemGive(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value
 *
 * The locks the semaphore referenced by sem_id by performing a
 * semaphore lock operation on that semaphore.  If the semaphore value
 * is currently zero, then the calling thread shall not return from
 * the call until it either locks the semaphore or the call is
 * interrupted.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the Id passed in is not a valid counting semaphore
 * @retval #OS_SEM_FAILURE if an unspecified implementation error occurs @covtest
 */
int32 OS_CountSemTake(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value with timeout
 *
 * The function locks the semaphore referenced by sem_id. However,
 * if the semaphore cannot be locked without waiting for another process
 * or thread to unlock the semaphore, this wait shall be terminated when
 * the specified timeout, msecs, expires.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[in] msecs  The maximum amount of time to block, in milliseconds
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_TIMEOUT if semaphore was not relinquished in time
 * @retval #OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
 * @retval #OS_SEM_FAILURE if an unspecified implementation error occurs @covtest
 */
int32 OS_CountSemTimedWait(osal_id_t sem_id, uint32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified counting Semaphore.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore
 * @retval #OS_SEM_FAILURE if an unspecified implementation error occurs @covtest
 */
int32 OS_CountSemDelete(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing semaphore ID by name
 *
 * This function tries to find a counting sem Id given the name of a count_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]  sem_name the name of the existing resource to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_CountSemGetIdByName(osal_id_t *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified counting
 * semaphore.
 *
 * @param[in]  sem_id The object ID to operate on
 * @param[out] count_prop The property object buffer to fill @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the count_prop pointer is null
 */
int32 OS_CountSemGetInfo(osal_id_t sem_id, OS_count_sem_prop_t *count_prop);

/**@}*/

#endif /* OSAPI_COUNTSEM_H */
