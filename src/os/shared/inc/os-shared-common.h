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

#ifndef OS_SHARED_COMMON_H
#define OS_SHARED_COMMON_H

#include "osapi-common.h"
#include "os-shared-globaldefs.h"

/*
 * Flag values for the "GlobalState" member the global state structure
 */
#define OS_INIT_MAGIC_NUMBER     0xBE57C0DE /**< Indicates that OS_API_Init() has been successfully run */
#define OS_SHUTDOWN_MAGIC_NUMBER 0xABADC0DE /**< Indicates that a system shutdown request is pending */

/* Global variables that are common between implementations */
struct OS_shared_global_vars
{
    /*
     * Tracks whether OS_API_Init() has been called or if
     * there is a shutdown request pending.
     *
     * After boot/first startup this should have 0 (from BSS clearing)
     * After OS_API_Init() is called this has OS_INIT_MAGIC_NUMBER
     * After OS_ApplicationShutdown() this has OS_SHUTDOWN_MAGIC_NUMBER
     */
    volatile uint32 GlobalState;

    /*
     * The console device ID used for OS_printf() calls
     */
    osal_id_t PrintfConsoleId;

    /*
     * PrintfEnabled and GlobalState are marked "volatile"
     * because they are updated and read by different threads
     */
    volatile bool PrintfEnabled;
    uint32        MicroSecPerTick;
    uint32        TicksPerSecond;

    /*
     * The event handler is an application-defined callback
     * that gets invoked as resources are created/configured/deleted.
     */
    OS_EventHandler_t EventHandler;

#ifdef OSAL_CONFIG_DEBUG_PRINTF
    uint8 DebugLevel;
#endif
};

/*
 * Shared data structure for global values
 */
extern OS_SharedGlobalVars_t OS_SharedGlobalVars;

/*---------------------------------------------------------------------------------------
   Name: OS_NotifyEvent

   Purpose: Notify the user application of a change in the state of an OSAL resource

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_NotifyEvent(OS_Event_t event, osal_id_t object_id, void *data);

/*---------------------------------------------------------------------------------------
   Name: OS_API_Impl_Init

   Purpose: Initialize the OS-specific layer for the given object type

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_API_Impl_Init(osal_objtype_t idtype);

/*
 * This functions implement a the OS-specific portion
 * of various OSAL functions.  They are defined in
 * OS-specific source files.
 */

/*----------------------------------------------------------------

   Function: OS_IdleLoop_Impl

   Purpose: Block the "idle" thread until woken up

   The "IdleLoop_Impl" is called by the main thread once
   everything is running and there is no more work to do.

   It should suspend the calling thread until a wakeup
   event happens.

 ------------------------------------------------------------------*/
void OS_IdleLoop_Impl(void);

/*----------------------------------------------------------------

   Function: OS_ApplicationShutdown_Impl

    Purpose: Wake up the idle task

   The "ApplicationShutdown_Impl" should wake up whatever
   task is currently suspended in "IdleLoop_Impl" and cause
   that thread to resume and return to its caller.

   NOTE: This should not block but it may cause the current
   thread to be preempted by the thread that was woken up,
   depending on priority levels.

 ------------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl(void);

/*----------------------------------------------------------------

   Function: OS_strnlen

    Purpose: Utility function to safely find the length of a string
             within a fixed-size array buffer.

             Provides a local OSAL routine to get the functionality
             of the (non-C99) "strnlen()" function, via the
             C89/C99 standard "memchr()" function instead.

 ------------------------------------------------------------------*/
static inline size_t OS_strnlen(const char *s, size_t maxlen)
{
    const char *end = memchr(s, 0, maxlen);
    if (end != NULL)
    {
        /* actual length of string is difference */
        maxlen = end - s;
    }
    return maxlen;
}

#endif /* OS_SHARED_COMMON_H */
