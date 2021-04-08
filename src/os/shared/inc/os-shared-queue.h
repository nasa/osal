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
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_QUEUE_H
#define OS_SHARED_QUEUE_H

#include "osapi-queue.h"
#include "os-shared-globaldefs.h"

typedef struct
{
    char              queue_name[OS_MAX_API_NAME];
    size_t            max_size;
    osal_blockcount_t max_depth;
} OS_queue_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_queue_internal_record_t OS_queue_table[OS_MAX_QUEUES];

/****************************************************************************************
                 MESSAGE QUEUE API LOW-LEVEL IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_QueueAPI_Init

   Purpose: Initialize the OS-independent layer for queues

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_QueueAPI_Init(void);

/*----------------------------------------------------------------
   Function: OS_QueueCreate_Impl

    Purpose: Prepare/Allocate OS resources for a message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueCreate_Impl(const OS_object_token_t *token, uint32 flags);

/*----------------------------------------------------------------
   Function: OS_QueueDelete_Impl

    Purpose: Free the OS resources associated with the message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_QueueGet_Impl

    Purpose: Receive a message on a message queue.
             The calling task will be blocked if no message is immediately available

    Returns: OS_SUCCESS on success, or relevant error code
             OS_QUEUE_TIMEOUT must be returned if the timeout expired and no message was received
             OS_QUEUE_EMPTY must be returned if the queue is empty when polled (OS_CHECK)
             OS_QUEUE_INVALID_SIZE must be returned if the supplied buffer is too small
 ------------------------------------------------------------------*/
int32 OS_QueueGet_Impl(const OS_object_token_t *token, void *data, size_t size, size_t *size_copied, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_QueuePut_Impl

    Purpose: Put a message into a message queue

    Returns: OS_SUCCESS on success, or relevant error code
             OS_QUEUE_FULL must be returned if the queue is full.
 ------------------------------------------------------------------*/
int32 OS_QueuePut_Impl(const OS_object_token_t *token, const void *data, size_t size, uint32 flags);

/*----------------------------------------------------------------
   Function: OS_QueueGetInfo_Impl

    Purpose: Obtain OS-specific information about a message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueGetInfo_Impl(const OS_object_token_t *token, OS_queue_prop_t *queue_prop);

#endif /* OS_SHARED_QUEUE_H */
