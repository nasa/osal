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

#ifndef OS_SHARED_TIMEBASE_H
#define OS_SHARED_TIMEBASE_H

#include "osapi-timebase.h"
#include "os-shared-globaldefs.h"

typedef struct
{
    char           timebase_name[OS_MAX_API_NAME];
    OS_TimerSync_t external_sync;
    uint32         accuracy_usec;
    osal_id_t      first_cb;
    uint32         freerun_time;
    uint32         nominal_start_time;
    uint32         nominal_interval_time;
} OS_timebase_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_timebase_internal_record_t OS_timebase_table[OS_MAX_TIMEBASES];

/****************************************************************************************
                 CLOCK / TIME API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_TimeBaseAPI_Init

   Purpose: Initialize the OS-independent layer for timebase objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_TimeBaseAPI_Init(void);

/*
 * TimeBase routines implement a low-level timer tick/interrupt;
 * no callbacks to user code here.  All application callbacks are
 * done in the shared layer timer API.
 */

/*----------------------------------------------------------------

    Purpose: Prepare OS resources for a time base

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseCreate_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Configure the OS resources to provide a timer tick

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseSet_Impl(const OS_object_token_t *token, uint32 start_time, uint32 interval_time);

/*----------------------------------------------------------------

    Purpose: Free the OS resources associated with the time base

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseDelete_Impl(const OS_object_token_t *token);

/****************************************************************************************
                                INTERNAL FUNCTIONS
****************************************************************************************/

/*----------------------------------------------------------------

    Purpose: Get exclusive access to the given timebase
             Add/remove of application callbacks is prevented
 ------------------------------------------------------------------*/
void OS_TimeBaseLock_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Release exclusive access to the given timebase
             Add/remove of application callbacks is allowed
 ------------------------------------------------------------------*/
void OS_TimeBaseUnlock_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Obtain the OS-specific time base information, if any

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_TimeBaseGetInfo_Impl(const OS_object_token_t *token, OS_timebase_prop_t *timer_prop);

/*----------------------------------------------------------------

    Purpose: Implement the time base helper thread
             This is the context for providing application callbacks
 ------------------------------------------------------------------*/
void OS_TimeBase_CallbackThread(osal_id_t timebase_id);

/*----------------------------------------------------------------

    Purpose: Convert milliseconds to ticks
 ------------------------------------------------------------------*/
int32 OS_Milli2Ticks(uint32 milli_seconds, int *ticks);

#endif /* OS_SHARED_TIMEBASE_H */
