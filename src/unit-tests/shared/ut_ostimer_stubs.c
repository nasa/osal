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

/*================================================================================*
** File:  ut_ostimer_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/* As defined in ostimer.c */
typedef struct
{
   uint32              free;
   char                name[OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;
   OS_TimerCallback_t  callback_ptr;
   uint32              host_timerid;
} OS_timer_record_t;

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsReturnCode_t  g_timerInit = {0,0};
UT_OsReturnCode_t  g_timerCreate = {0,0};
UT_OsReturnCode_t  g_timerDelete = {0,0};
UT_OsReturnCode_t  g_timerSet = {0,0};
UT_OsReturnCode_t  g_timerGetId = {0,0};
UT_OsReturnCode_t  g_timerGetInfo = {0,0};

/* As defined in ostimer.c */
OS_timer_record_t  OS_timer_table[OS_MAX_TIMERS];
uint32             os_clock_accuracy;

#ifdef _LINUX_OS_
pthread_mutex_t    OS_timer_table_mut;
#endif

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_TimerAPIInit(void)
{
    return (g_timerInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerCreate(uint32* timer_id, const char* timer_name,
                     uint32* clock_accuracy, OS_TimerCallback_t callback_ptr)
{
    return (g_timerCreate.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerDelete(uint32 timer_id)
{
    return (g_timerDelete.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerSet(uint32 timer_id, uint32 start_msec, uint32 interval_msec)
{
    return (g_timerSet.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerGetIdByName(uint32* timer_id, const char* timer_name)
{
    return (g_timerGetId.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t* timer_prop)
{
    return (g_timerGetInfo.value);
}

/*================================================================================*
** End of File: ut_ostimer_stubs.c
**================================================================================*/
