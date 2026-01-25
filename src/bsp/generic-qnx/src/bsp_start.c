/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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

/*
 * File:  bsp_start.c
 *
 * Purpose:
 *   OSAL BSP main entry point.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "generic_qnx_bsp_internal.h"

OS_BSP_GenericQnxGlobalData_t OS_BSP_GenericQnxGlobal;

/* ---------------------------------------------------------
    OS_BSP_Initialize()

     Helper function to auto-create any missing FS_BASED mount
     points listed in OS_VolumeTable.  If these do not actually
     exist then app code may fail.
   --------------------------------------------------------- */
void OS_BSP_Initialize(void)
{
    pthread_mutexattr_t mutex_attr;
    int                 status;

    /* Initialize the low level access mutex (w/priority inheritance) */
    status = pthread_mutexattr_init(&mutex_attr);
    if (status < 0)
    {
        BSP_DEBUG("pthread_mutexattr_init: %s\n", strerror(status));
    }
    status = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    if (status < 0)
    {
        BSP_DEBUG("pthread_mutexattr_setprotocol: %s\n", strerror(status));
    }
    status = pthread_mutex_init(&OS_BSP_GenericQnxGlobal.AccessMutex, &mutex_attr);
    if (status < 0)
    {
        BSP_DEBUG("pthread_mutex_init: %s\n", strerror(status));
    }
}

/*----------------------------------------------------------------
   OS_BSP_Lock_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_Lock_Impl(void)
{
    int status;

    status = pthread_mutex_lock(&OS_BSP_GenericQnxGlobal.AccessMutex);
    if (status < 0)
    {
        BSP_DEBUG("pthread_mutex_lock: %s\n", strerror(status));
    }
    else
    {
        /*
         * Temporarily Disable/Defer thread cancellation.
         * Note that OS_BSP_ConsoleOutput_Impl() calls write() which is a cancellation point.
         * So if this calling task is canceled, it risks leaving the BSP locked.
         */
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &OS_BSP_GenericQnxGlobal.AccessCancelState);
    }
}

/*----------------------------------------------------------------
   OS_BSP_Unlock_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_Unlock_Impl(void)
{
    int status;

    status = pthread_mutex_unlock(&OS_BSP_GenericQnxGlobal.AccessMutex);
    if (status < 0)
    {
        BSP_DEBUG("pthread_mutex_unlock: %s\n", strerror(status));
    }
    else
    {
        /* Restore previous cancellability state */
        pthread_setcancelstate(OS_BSP_GenericQnxGlobal.AccessCancelState, NULL);
    }
}

/* ---------------------------------------------------------
    OS_BSP_GetReturnStatus()

     Helper function to convert an OSAL status code into
     a code suitable for returning to the OS.
   --------------------------------------------------------- */
int OS_BSP_GetReturnStatus(void)
{
    int retcode;

    switch (OS_BSP_Global.AppStatus)
    {
        case OS_SUCCESS:
            /* translate OS_SUCCESS to the system EXIT_SUCCESS value (usually 0) */
            retcode = EXIT_SUCCESS;
            break;

        case OS_ERROR:
            /* translate OS_ERROR to the system EXIT_FAILURE value (usually 1) */
            retcode = EXIT_FAILURE;
            break;

        default:
            /* any other value will be passed through (implementation-defined) */
            /* Range is limited to 0-127, however */
            retcode = OS_BSP_Global.AppStatus & 0x7F;
            break;
    }

    return retcode;
}

/* ---------------------------------------------------------
    OS_BSP_Shutdown_Impl()

     Helper function to abort the running task
   --------------------------------------------------------- */
void OS_BSP_Shutdown_Impl(void)
{
    abort();
}

/******************************************************************************
**
**  Purpose:
**    BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int main(int argc, char *argv[])
{
    /*
     * Initially clear the global objects
     */
    memset(&OS_BSP_Global, 0, sizeof(OS_BSP_Global));
    memset(&OS_BSP_GenericQnxGlobal, 0, sizeof(OS_BSP_GenericQnxGlobal));

    /*
     * Save the argc/argv arguments for future use.
     * In particular the UT-specific logic uses this
     * to control verbosity.
     *
     * Note that the first argument (0) is the command name.  The
     * first "real" argument is at position 1.
     *
     * However this still needs to pass it through as the application
     * might still want to use library "getopt" and this expects the
     * first parameter to be this way.
     */
    OS_BSP_Global.ArgC = argc;
    OS_BSP_Global.ArgV = argv;

    /*
     * Only attempt terminal control if the stdout is a TTY
     * and the TERM environment variable is set
     */
    if (getenv("TERM") != NULL)
    {
        OS_BSP_GenericQnxGlobal.EnableTermControl = isatty(STDOUT_FILENO);
    }

    /*
     * Perform any other BSP-specific initialization
     */
    OS_BSP_Initialize();

    /*
     * Call application specific entry point.
     * This should set up all user tasks and resources, then return
     */
    OS_Application_Startup();

    /*
     * OS_Application_Run() implements the background task.
     * The user application may provide this, or a default implementation
     * is used which just calls OS_IdleLoop().
     */
    OS_Application_Run();

    /* Should typically never get here */
    return OS_BSP_GetReturnStatus();
}
