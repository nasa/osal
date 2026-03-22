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
 * Declarations and prototypes for rwlocks
 */

#ifndef OSAPI_RWLOCK_H
#define OSAPI_RWLOCK_H

#include "osconfig.h"
#include "common_types.h"

/** @brief OSAL rwlock properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_rwlock_prop_t;

/** @defgroup OSAPIRwLock OSAL RwLock APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates an readers-writer lock (rwlock)
 *
 * RwLocks are always created in the unlocked (full) state.
 *
 * @param[out]  rw_id will be set to the non-zero ID of the newly-created resource @nonnull
 * @param[in]   rw_name the name of the new resource to create @nonnull
 * @param[in]   options reserved for future use.  Should be passed as 0.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if sem_id or sem_name are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are no more free rwlock Ids
 * @retval #OS_ERR_NAME_TAKEN if there is already a rwlock with the same name
 * @retval #OS_ERROR if the OS call failed @covtest
 */
int32 OS_RwLockCreate(osal_id_t *rw_id, const char *rw_name, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Releases the reader lock on the rwlock object referenced by rw_id.
 *
 * If this function is called by a thread which holds a read lock on this rwlock object,
 * and there are other threads currently holding read locks, the rwlock will remain in
 * the read locked state. If the function is called by the last thread which held a
 * read lock, the rwlock becomes unlocked.
 *
 * If this function is called by a thread which holds a write lock on this rwlock object,
 * the behavior is undefined.
 *
 * If there are threads blocked on the rwlock object referenced by
 * rw_id when this function is called, resulting in the rwlock becoming
 * available, the scheduling policy shall determine which thread shall
 * acquire the rwlock.
 *
 * @param[in] rw_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid rwlock
 * @retval #OS_ERROR if an unspecified error occurs @covtest
 */
int32 OS_RwLockReadGive(osal_id_t rw_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Releases the rwlock object referenced by rw_id.
 *
 * If this function is called by a thread which holds a write lock on this rwlock object,
 * the rwlock becomes unlocked.
 *
 * If this function is called by a thread which holds a read lock on this rwlock object,
 * the behavior is undefined.
 *
 * If there are threads blocked on the rwlock object referenced by
 * rwlock when this function is called, resulting in the rwlock becoming
 * available, the scheduling policy shall determine which thread shall
 * acquire the rwlock.
 *
 * @param[in] rw_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid rwlock
 * @retval #OS_ERROR if an unspecified error occurs @covtest
 */
int32 OS_RwLockWriteGive(osal_id_t rw_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Acquire the rwlock object as a read lock as referenced by rw_id.
 *
 * If the rwlock is currently write locked, as in a thread holds a write lock on this
 * rwlock object, the current thread will block until the rwlock becomes available to
 * readers.
 *
 * If the rwlock is currently read locked and a thread intending to obtain a write lock
 * is waiting, whether or not the current thread is allowed to obtain a read lock depends
 * on the underlying OS rwlock implementation.
 *
 * If the rwlock is currently read locked and there are no threads waiting to obtain a write
 * lock, the current thread is allowed to obtain a read lock.
 *
 * If the rwlock is unlocked, the current thread will obtain a read lock
 * on this rwlock object.
 *
 * @param[in] rw_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid rwlock
 * @retval #OS_ERROR if an unspecified error occurs @covtest
 */
int32 OS_RwLockReadTake(osal_id_t rw_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Acquire the rwlock object as a write lock as referenced by rw_id.
 *
 * If the rwlock is currently write or read locked, the current thread will block until
 * the rwlock becomes available. If the rwlock is open, the current thread will acquire
 * a write lock on this rwlock object.
 *
 * @param[in] rw_id The object ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID the id passed in is not a valid rwlock
 * @retval #OS_ERROR if an unspecified error occurs @covtest
 */
int32 OS_RwLockWriteTake(osal_id_t rw_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified RwLock.
 *
 * Delete the rwlock.  This also frees the respective rw_id such that it can be
 * used again when another is created.
 *
 * @param[in] rw_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid rwlock
 * @retval #OS_ERROR if an unspecified error occurs @covtest
 */
int32 OS_RwLockDelete(osal_id_t rw_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing rwlock ID by name
 *
 * This function tries to find a rwlock Id given the name of a rwlock.
 * The id is returned through rw_id
 *
 * @param[out] rw_id will be set to the ID of the existing resource
 * @param[in]  rw_name the name of the existing resource to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER is rw_id or rw_name are NULL pointers
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_RwLockGetIdByName(osal_id_t *rw_id, const char *rw_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified rwlock.
 *
 * @param[in]  rw_id The object ID to operate on
 * @param[out] rw_prop The property object buffer to fill @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in is not a valid rwlock
 * @retval #OS_INVALID_POINTER if the rw_prop pointer is null
 */
int32 OS_RwLockGetInfo(osal_id_t rw_id, OS_rwlock_prop_t *rw_prop);
/**@}*/

#endif /* OSAPI_RWLOCK_H */
