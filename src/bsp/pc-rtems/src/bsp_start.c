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

/*
 * \file
 *
 *   OSAL BSP main entry point.
 */

/*
**  Include Files
*/
/* TODO clean these */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <bsp.h>
#include <rtems.h>
#include <rtems/bdbuf.h>
#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/bdpart.h>
#include <rtems/error.h>
#include <rtems/ramdisk.h>
#include <rtems/dosfs.h>
#include <rtems/fsmount.h>

#include "pcrtems_bsp_internal.h"

/* TODO add bsp_setupfs.h */
void OS_BSP_SetupFS(void);

/* TODO add bsp_shell.h */
void OS_BSP_Shell(void);

/* TODO add bsp_cmdline.h */
void OS_BSP_CmdLine(void);

/*
** Global variables
*/
OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

void OS_BSP_Setup(void)
{
    int status;

    printf("\n\n*** RTEMS Info ***\n");
    printf("%s", OSAL_BSP_COPYRIGHT_NOTICE);
    printf("%s\n\n", rtems_get_version_string());
    printf(" Stack size=%d\n", (int)rtems_configuration_get_stack_space_size());
    printf(" Workspace size=%d\n", (int)rtems_configuration_get_work_space_size());

    /* Process command line based on selected implementation */
    // TODO uncomment    OS_BSP_CmdLine();

    printf("\n");
    printf("*** End RTEMS info ***\n\n");

    /*
     * Initialize the low level access sem
     */
    status = rtems_semaphore_create(rtems_build_name('B', 'S', 'P', '\0'), 1,
                                    RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY, 0,
                                    &OS_BSP_PcRtemsGlobal.AccessMutex);
    if (status != RTEMS_SUCCESSFUL)
    {
        BSP_DEBUG("rtems_semaphore_create: %s\n", rtems_status_text(status));
    }

    /* Set up file system based on selected implementation */
    // TODO uncomment    OS_BSP_SetupFS();

    /*
     * Start the shell now based on selected implementation
     *
     * This way, if there is an issue with the application startup,
     * the shell can still be used to debug the system.
     *
     * The shell is _NOT_ started if BatchMode global is TRUE
     * which supports completely autonomous execution
     */
    OS_BSP_Shell();

    printf("\n\n");
}

/*----------------------------------------------------------------
   OS_BSP_Lock_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_Lock_Impl(void)
{
    rtems_status_code status;
    status = rtems_semaphore_obtain(OS_BSP_PcRtemsGlobal.AccessMutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (status != RTEMS_SUCCESSFUL)
    {
        BSP_DEBUG("rtems_semaphore_obtain: %s\n", rtems_status_text(status));
    }
}

/*----------------------------------------------------------------
   OS_BSP_Unlock_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_Unlock_Impl(void)
{
    rtems_status_code status;
    status = rtems_semaphore_release(OS_BSP_PcRtemsGlobal.AccessMutex);
    if (status != RTEMS_SUCCESSFUL)
    {
        BSP_DEBUG("rtems_semaphore_release: %s\n", rtems_status_text(status));
    }
}

/* ---------------------------------------------------------
    OS_BSP_GetReturnStatus()

     Helper function to convert an OSAL status code into
     a code suitable for returning to the OS.
   --------------------------------------------------------- */
rtems_status_code OS_BSP_GetReturnStatus(void)
{
    rtems_status_code retcode;
    const char *      StatusStr;

    switch (OS_BSP_Global.AppStatus)
    {
        case OS_SUCCESS:
            /* translate OS_SUCCESS to the system RTEMS_SUCCESSFUL value */
            StatusStr = "SUCCESS";
            retcode   = RTEMS_SUCCESSFUL;
            break;

        default:
            /* translate anything else to a generic non-success code,
             * this basically just means the main task exited */
            StatusStr = "ERROR";
            retcode   = RTEMS_TASK_EXITTED;
            break;
    }

    printf("\nApplication exit status: %s (%d)\n\n", StatusStr, (int)OS_BSP_Global.AppStatus);
    rtems_task_wake_after(100);

    return retcode;
}

/* ---------------------------------------------------------
    OS_BSP_Shutdown_Impl()

     Helper function to abort the running task
   --------------------------------------------------------- */
void OS_BSP_Shutdown_Impl(void)
{
    /*
     * Not calling exit() under RTEMS, this simply shuts down the executive,
     * forcing the user to reboot the system.
     *
     * Calling suspend causes execution to get stuck here, but the RTEMS
     * shell thread will still be active so the user can poke around, read results,
     * then use a shell command to reboot when ready.
     */
    while (!OS_BSP_PcRtemsGlobal.BatchMode)
    {
        printf("\n\nInit thread idle.\nPress <enter> for shell or reset machine...\n\n");
        rtems_task_suspend(rtems_task_self());
    }

    rtems_shutdown_executive(OS_BSP_GetReturnStatus());
}

/*
 ** A simple entry point callable from Init or can be loaded and started within an rki
 */
void OS_BSPMain(void)
{
    /*
     * Initially clear the global object
     */
    memset(&OS_BSP_Global, 0, sizeof(OS_BSP_Global));
    memset(&OS_BSP_PcRtemsGlobal, 0, sizeof(OS_BSP_PcRtemsGlobal));

    /*
     * Perform BSP setup -
     * Initialize the root file system, create mount points, etc.
     */
    OS_BSP_Setup();

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

    /*
     * Enter the BSP default shutdown mode
     * depending on config, this may reset/reboot or suspend
     * so the operator can use the shell.
     */
    OS_BSP_Shutdown_Impl();
}
