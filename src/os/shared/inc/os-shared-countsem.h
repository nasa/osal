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
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_COUNTSEM_H
#define OS_SHARED_COUNTSEM_H

#include "osapi-countsem.h"
#include "os-shared-globaldefs.h"

/* other objects that have only an API name and no other data */
typedef struct
{
    char obj_name[OS_MAX_API_NAME];
} OS_count_sem_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_count_sem_internal_record_t OS_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

/****************************************************************************************
                                IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_CountSemAPI_Init

   Purpose: Initialize the OS-independent layer for counting semaphores

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_CountSemAPI_Init(void);

/*----------------------------------------------------------------

    Purpose: Prepare/allocate OS resources for a counting semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options);

/*----------------------------------------------------------------

    Purpose: Increment the semaphore value

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemGive_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Decrement the semaphore value
             Block the calling task if the semaphore is 0.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemTake_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Decrement the semaphore value, with a time limit

    Returns: OS_SUCCESS on success, or relevant error code
             OS_SEM_TIMEOUT must be returned if the time limit was reached
 ------------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs);

/*----------------------------------------------------------------

    Purpose: Free the OS resources associated with the counting semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Obtain OS-specific information about the semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CountSemGetInfo_Impl(const OS_object_token_t *token, OS_count_sem_prop_t *count_prop);

#endif /* OS_SHARED_COUNTSEM_H */
