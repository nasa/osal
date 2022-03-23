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
 * Declarations and prototypes for queue subsystem
 */

#ifndef OSAPI_QUEUE_H
#define OSAPI_QUEUE_H

#include "osconfig.h"
#include "common_types.h"

/** @brief OSAL queue properties */
typedef struct
{
    char      name[OS_MAX_API_NAME];
    osal_id_t creator;
} OS_queue_prop_t;

/** @defgroup OSAPIMsgQueue OSAL Message Queue APIs
 * @{
 */

/**
 * @brief Create a message queue
 *
 * This is the function used to create a queue in the operating system.
 * Depending on the underlying operating system, the memory for the queue
 * will be allocated automatically or allocated by the code that sets up
 * the queue. Queue names must be unique; if the name already exists this
 * function fails. Names cannot be NULL.
 *
 * @param[out]  queue_id will be set to the non-zero ID of the newly-created resource @nonnull
 * @param[in]   queue_name the name of the new resource to create @nonnull
 * @param[in]   queue_depth the maximum depth of the queue
 * @param[in]   data_size the size of each entry in the queue @nonzero
 * @param[in]   flags options for the queue (reserved for future use, pass as 0)
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if a pointer passed in is NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NO_FREE_IDS if there are already the max queues created
 * @retval #OS_ERR_NAME_TAKEN if the name is already being used on another queue
 * @retval #OS_ERR_INVALID_SIZE if data_size is 0
 * @retval #OS_QUEUE_INVALID_SIZE if the queue depth exceeds the limit
 * @retval #OS_ERROR if the OS create call fails
 */
int32 OS_QueueCreate(osal_id_t *queue_id, const char *queue_name, osal_blockcount_t queue_depth, size_t data_size,
                     uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified message queue.
 *
 * This is the function used to delete a queue in the operating system.
 * This also frees the respective queue_id to be used again when another queue is created.
 *
 * @note If There are messages on the queue, they will be lost and any subsequent
 * calls to QueueGet or QueuePut to this queue will result in errors
 *
 * @param[in] queue_id The object ID to delete
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the id passed in does not exist
 * @retval #OS_ERROR if the OS call returns an unexpected error @covtest
 */
int32 OS_QueueDelete(osal_id_t queue_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Receive a message on a message queue
 *
 * If a message is pending, it is returned immediately.  Otherwise the calling task
 * will block until a message arrives or the timeout expires.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  data The buffer to store the received message @nonnull
 * @param[in]   size The size of the data buffer @nonzero
 * @param[out]  size_copied Set to the actual size of the message @nonnull
 * @param[in]   timeout The maximum amount of time to block, or OS_PEND to wait forever
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the given ID does not exist
 * @retval #OS_INVALID_POINTER if a pointer passed in is NULL
 * @retval #OS_QUEUE_EMPTY if the Queue has no messages on it to be received
 * @retval #OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
 * @retval #OS_QUEUE_INVALID_SIZE if the size copied from the queue was not correct
 * @retval #OS_ERROR if the OS call returns an unexpected error @covtest
 */
int32 OS_QueueGet(osal_id_t queue_id, void *data, size_t size, size_t *size_copied, int32 timeout);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Put a message on a message queue.
 *
 * @param[in]  queue_id The object ID to operate on
 * @param[in]  data The buffer containing the message to put @nonnull
 * @param[in]  size The size of the data buffer @nonzero
 * @param[in]  flags Currently reserved/unused, should be passed as 0
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the queue id passed in is not a valid queue
 * @retval #OS_INVALID_POINTER if the data pointer is NULL
 * @retval #OS_QUEUE_INVALID_SIZE if the data message is too large for the queue
 * @retval #OS_QUEUE_FULL if the queue cannot accept another message
 * @retval #OS_ERROR if the OS call returns an unexpected error @covtest
 */
int32 OS_QueuePut(osal_id_t queue_id, const void *data, size_t size, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing queue ID by name
 *
 * This function tries to find a queue Id given the name of the queue. The
 * id of the queue is passed back in queue_id.
 *
 * @param[out]  queue_id will be set to the ID of the existing resource
 * @param[in]   queue_name the name of the existing resource to find @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if the name or id pointers are NULL
 * @retval #OS_ERR_NAME_TOO_LONG name length including null terminator greater than #OS_MAX_API_NAME
 * @retval #OS_ERR_NAME_NOT_FOUND the name was not found in the table
 */
int32 OS_QueueGetIdByName(osal_id_t *queue_id, const char *queue_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (name and creator) about the specified queue.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  queue_prop The property object buffer to fill @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if queue_prop is NULL
 * @retval #OS_ERR_INVALID_ID if the ID given is not  a valid queue
 */
int32 OS_QueueGetInfo(osal_id_t queue_id, OS_queue_prop_t *queue_prop);
/**@}*/

#endif /* OSAPI_QUEUE_H */
