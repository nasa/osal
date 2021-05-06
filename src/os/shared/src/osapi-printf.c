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
 * \file     osapi-printf.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *      Contains the abstraction for the OS_printf() call.
 *
 *      This top level contains only the master on/off switch for OS_printf(),
 *      that is the OS_printf_enable() and OS_printf_disable() API calls.
 *
 *      If enabled, this OS_printf() uses the C library "vsnprintf()" call
 *      to format the actual string for output.  As this is a C99 function
 *      it should be present on all compliant machines.  In the event that
 *      the machine's C library does not provide this function, the user
 *      would have to provide a compatible substitute to link to.
 *
 *      Once the string is formatted, it is passed to the lower level
 *      implementation to do the actual output.  This would typically write
 *      to a console device but may alternatively write to any other
 *      implementation-defined output interface, such as a system log or
 *      serial port.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 * User defined include files
 */
#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-printf.h"

/*
 * The choice of whether to run a separate utility task
 * comes from osal compile-time config
 */
#ifdef OSAL_CONFIG_CONSOLE_ASYNC
#define OS_CONSOLE_IS_ASYNC true
#else
#define OS_CONSOLE_IS_ASYNC false
#endif

/* reserve buffer memory for the printf console device */
static char OS_printf_buffer_mem[(sizeof(OS_PRINTF_CONSOLE_NAME) + OS_BUFFER_SIZE) * OS_BUFFER_MSG_DEPTH];

/* The global console state table */
OS_console_internal_record_t OS_console_table[OS_MAX_CONSOLES];

/*
 *********************************************************************************
 *          INITIALIZATION
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleAPI_Init(void)
{
    OS_console_internal_record_t *console;
    int32                         return_code;
    OS_object_token_t             token;

    memset(&OS_console_table, 0, sizeof(OS_console_table));

    /*
     * Configure a console device to be used for OS_printf() calls.
     */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_CONSOLE, OS_PRINTF_CONSOLE_NAME, &token);
    if (return_code == OS_SUCCESS)
    {
        console = OS_OBJECT_TABLE_GET(OS_console_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, console, device_name, OS_PRINTF_CONSOLE_NAME);

        /*
         * Initialize the ring buffer pointers
         */
        console->BufBase = OS_printf_buffer_mem;
        console->BufSize = sizeof(OS_printf_buffer_mem);
        console->IsAsync = OS_CONSOLE_IS_ASYNC;

        return_code = OS_ConsoleCreate_Impl(&token);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, &OS_SharedGlobalVars.PrintfConsoleId);

        /*
         * Printf can be enabled by default now that the buffer is configured.
         */
        OS_SharedGlobalVars.PrintfEnabled = true;
    }

    return return_code;
} /* end OS_ConsoleAPI_Init */

/*
 *********************************************************************************
 *          LOCAL HELPER FUNCTIONS
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 * Function: OS_Console_CopyOut
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *    Write into the console ring buffer
 *
 *    The NextWritePos is an input-output and contains the position
 *    in the ring buffer to start writing into.  This may or may not
 *    be the same as the value in the global.  It is only updated
 *    if the string is written in its entirety.
 *
 *    The intent is to avoid truncating a string if it does not fit.
 *    Either the entire string should be written, or none of it.
 *
 *-----------------------------------------------------------------*/
static int32 OS_Console_CopyOut(OS_console_internal_record_t *console, const char *Str, size_t *NextWritePos)
{
    const char *pmsg;
    size_t      WriteOffset;
    int32       return_code;

    return_code = OS_ERROR;
    pmsg        = Str;
    WriteOffset = *NextWritePos;
    while (true)
    {
        if (*pmsg == 0)
        {
            /* String is complete */
            *NextWritePos = WriteOffset;
            return_code   = OS_SUCCESS;
            break;
        }
        console->BufBase[WriteOffset] = *pmsg;
        WriteOffset                   = WriteOffset + 1;
        if (WriteOffset >= console->BufSize)
        {
            WriteOffset = 0;
        }

        if (WriteOffset == console->ReadPos)
        {
            /* out of space */
            return_code = OS_QUEUE_FULL;
            break;
        }

        ++pmsg;
    }

    return return_code;
} /* end OS_Console_CopyOut */

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleWrite
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Write into the console ring buffer
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleWrite(osal_id_t console_id, const char *Str)
{
    int32                         return_code;
    OS_object_token_t             token;
    OS_console_internal_record_t *console;
    size_t                        PendingWritePos;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_CONSOLE, console_id, &token);
    if (return_code == OS_SUCCESS)
    {
        console = OS_OBJECT_TABLE_GET(OS_console_table, token);

        /*
         * The entire string should be put to the ring buffer,
         * or none of it.  Therefore the WritePos in the table
         * is not updated until complete success.
         */
        PendingWritePos = console->WritePos;

        if (console->device_name[0] != 0)
        {
            return_code = OS_Console_CopyOut(console, console->device_name, &PendingWritePos);
        }
        if (return_code == OS_SUCCESS)
        {
            return_code = OS_Console_CopyOut(console, Str, &PendingWritePos);
        }

        if (return_code == OS_SUCCESS)
        {
            /* the entire message was successfully written */
            console->WritePos = PendingWritePos;
        }
        else
        {
            /* the message did not fit */
            ++console->OverflowEvents;
        }

        /*
         * Notify the underlying console implementation of new data.
         * This will forward the data to the actual console device.
         *
         * This is done while still locked, so it can support
         * either a synchronous or asynchronous implementation.
         */
        if (console->IsAsync)
        {
            /* post the sem for the utility task to run */
            OS_ConsoleWakeup_Impl(&token);
        }
        else
        {
            /* output directly */
            OS_ConsoleOutput_Impl(&token);
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_ConsoleWrite */

/*----------------------------------------------------------------
 *
 * Function: OS_printf
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf(const char *String, ...)
{
    va_list va;
    char    msg_buffer[OS_BUFFER_SIZE];
    int     actualsz;

    BUGCHECK((String) != NULL, )

    if (OS_SharedGlobalVars.GlobalState != OS_INIT_MAGIC_NUMBER)
    {
        /*
         * Catch some historical mis-use of the OS_printf() call.
         *
         * Typically OS_printf() should NOT be called before OS_API_Init().
         *
         * This was never guaranteed to work, particularly on a VxWorks
         * deployment where the utility task was enabled.
         *
         * However, some PSPs do this, particularly those that used POSIX
         * where it happened to work (because OS_printf just called printf).
         *
         * As a workaround, use the OS_DEBUG facility to dump the message,
         * along with a clue that this API is being used inappropriately.
         *
         * If debugging is not enabled, then this message will be silently
         * discarded.
         */
        OS_DEBUG("BUG: OS_printf() called when OSAL not initialized: %s", String);
    }
    else if (OS_SharedGlobalVars.PrintfEnabled)
    {
        /* Format and determine the size of string to write */
        va_start(va, String);
        actualsz = vsnprintf(msg_buffer, sizeof(msg_buffer), String, va);
        va_end(va);

        if (actualsz < 0)
        {
            /* unlikely: vsnprintf failed */
            actualsz = 0;
        }
        else if (actualsz >= OS_BUFFER_SIZE)
        {
            /* truncate */
            actualsz = OS_BUFFER_SIZE - 1;
        }

        msg_buffer[actualsz] = 0;

        OS_ConsoleWrite(OS_SharedGlobalVars.PrintfConsoleId, msg_buffer);
    }

} /* end OS_printf */

/*----------------------------------------------------------------
 *
 * Function: OS_printf_disable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf_disable(void)
{
    OS_SharedGlobalVars.PrintfEnabled = false;
} /* end OS_printf_disable */

/*----------------------------------------------------------------
 *
 * Function: OS_printf_enable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_printf_enable(void)
{
    OS_SharedGlobalVars.PrintfEnabled = true;
} /* end OS_printf_enable */
