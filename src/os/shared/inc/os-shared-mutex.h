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

#ifndef OS_SHARED_MUTEX_H
#define OS_SHARED_MUTEX_H

#include "osapi-mutex.h"
#include "os-shared-globaldefs.h"

typedef struct
{
    char      obj_name[OS_MAX_API_NAME];
    osal_id_t last_owner;
} OS_mutex_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_mutex_internal_record_t OS_mutex_table[OS_MAX_MUTEXES];

/*---------------------------------------------------------------------------------------
   Name: OS_MutexAPI_Init

   Purpose: Initialize the OS-independent layer for mutex objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_MutexAPI_Init(void);

/*----------------------------------------------------------------

    Purpose: Prepare/allocate OS resources for a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options);

/*----------------------------------------------------------------

    Purpose: Release the mutex, which must be owned by the caller

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGive_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Acquire the mutex, blocking the caller as necessary

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemTake_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Free the OS resources associated with a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Obtain OS-specific information about the mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop);

#endif /* OS_SHARED_MUTEX_H */
