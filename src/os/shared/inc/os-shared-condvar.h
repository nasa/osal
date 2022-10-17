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

#ifndef OS_SHARED_CONDVAR_H
#define OS_SHARED_CONDVAR_H

#include "osapi-condvar.h"
#include "os-shared-globaldefs.h"

typedef struct
{
    char obj_name[OS_MAX_API_NAME];
} OS_condvar_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_condvar_internal_record_t OS_condvar_table[OS_MAX_CONDVARS];

/*---------------------------------------------------------------------------------------
   Name: OS_CondVarAPI_Init

   Purpose: Initialize the OS-independent layer for condvar objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_CondVarAPI_Init(void);

/*----------------------------------------------------------------

    Purpose: Prepare/allocate OS resources for a condvar object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarCreate_Impl(const OS_object_token_t *token, uint32 options);

/*----------------------------------------------------------------

    Purpose: Acquires the underlying mutex

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarLock_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Release the underlying mutex

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarUnlock_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Wake up one blocked task

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarSignal_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Wake up all blocked tasks

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarBroadcast_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Wait indefinitely for the condvar to be signaled

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarWait_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Time-Limited wait for the condvar to be signaled

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarTimedWait_Impl(const OS_object_token_t *token, const OS_time_t *abs_wakeup_time);

/*----------------------------------------------------------------

    Purpose: Free the OS resources associated with a condvar object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Obtain OS-specific information about the condvar object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_CondVarGetInfo_Impl(const OS_object_token_t *token, OS_condvar_prop_t *condvar_prop);

#endif /* OS_SHARED_CONDVAR_H */
