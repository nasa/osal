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
 * Declarations and prototypes for mutexes
 */

#ifndef OSAPI_MUTEX_H
#define OSAPI_MUTEX_H

#include "osconfig.h"
#include "common_types.h"

/** @brief OSAL mutex properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_mut_sem_prop_t;

/** @defgroup OSAPIMutex OSAL Mutex APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a mutex semaphore
 *
 * Mutex semaphores are always created in the unlocked (full) state.
 *
 * @param[out]  sem_id will be set to the non-zero ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   options reserved for future use.  Should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sem_id or sem_name are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are no more free mutex Ids
 * @retval #OS_ERR_NAME_TAKEN if there is already a mutex with the same name
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Releases the mutex object referenced by sem_id.
 *
 * If there are threads blocked on the mutex object referenced by
 * mutex when this function is called, resulting in the mutex becoming
 * available, the scheduling policy shall determine which thread shall
 * acquire the mutex.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * @retval #OS_SEM_FAILURE if an unspecified error occurs
 */
int32 OS_MutSemGive(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Acquire the mutex object referenced by sem_id.
 *
 * If the mutex is already locked, the calling thread shall
 * block until the mutex becomes available. This operation shall
 * return with the mutex object referenced by mutex in the locked state
 * with the calling thread as its owner.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_SEM_FAILURE if the semaphore was not previously initialized or is
 * not in the array of semaphores defined by the system
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid mutex
 */
int32 OS_MutSemTake(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Mutex Semaphore.
 *
 * Delete the semaphore.  This also frees the respective sem_id such that it can be
 * used again when another is created.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * @retval #OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemDelete(osal_id_t sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing mutex ID by name
 *
 * This function tries to find a mutex sem Id given the name of a mut_sem.
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_MutSemGetIdByName(osal_id_t *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified mutex
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  mut_prop The property object buffer to fill
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * @retval #OS_INVALID_POINTER if the mut_prop pointer is null
 */
int32 OS_MutSemGetInfo(osal_id_t sem_id, OS_mut_sem_prop_t *mut_prop);
/**@}*/

#endif /* OSAPI_MUTEX_H */
