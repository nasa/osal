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
 * Table implementation and calls related to the console buffer.
 *
 * This is a simple ring buffer that decouples
 * the OS_printf() call from actual console output.
 *
 * The implementation layer may optionally spawn a
 * "utility task" or equivalent to forward data, or
 * it may process data immediately.
 */

#ifndef OS_SHARED_CONSOLE_H
#define OS_SHARED_CONSOLE_H

#include "osapi-printf.h"
#include "os-shared-printf.h"
#include "os-shared-globaldefs.h"

/**
 * The generic console data record
 */
typedef struct
{
    char device_name[OS_MAX_API_NAME];

    char *          BufBase;        /**< Start of the buffer memory */
    size_t          BufSize;        /**< Total size of the buffer */
    volatile size_t ReadPos;        /**< Offset of next byte to read */
    volatile size_t WritePos;       /**< Offset of next byte to write */
    uint32          OverflowEvents; /**< Number of lines dropped due to overflow */
    bool            IsAsync;        /**< Whether to write data via deferred utility task */

} OS_console_internal_record_t;

extern OS_console_internal_record_t OS_console_table[OS_MAX_CONSOLES];

/****************************************************************************************
                 CONSOLE / DEBUG API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_ConsoleAPI_Init

   Purpose: Initialize the OS-independent layer for console service

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_ConsoleAPI_Init(void);

/*----------------------------------------------------------------
   Function: OS_ConsoleCreate_Impl

    Purpose: Prepare a console device for use
             For Async devices, this sets up the background writer task
 ------------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ConsoleWakeup_Impl

    Purpose: Console output data notification

   This is a notification API that is invoked whenever there
   is new data available in the console output buffer.

   It is only used if the console is configured for async operation,
   and it should wakeup the actual console servicing thread.
 ------------------------------------------------------------------*/
void OS_ConsoleWakeup_Impl(const OS_object_token_t *token);

#endif /* OS_SHARED_CONSOLE_H */
