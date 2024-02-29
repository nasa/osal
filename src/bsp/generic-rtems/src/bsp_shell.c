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
 *   OSAL BSP shell implementation
 */

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <bsp.h>
#include <rtems.h>
#include <rtems/shell.h>
#include <rtems/rtl/dlfcn-shell.h>

#include "genericrtems_bsp_internal.h"

#include "bsp_shell.h"

/*
 * By default put the shell at the same priority
 * as the utility task which handles OS_printf()
 */
#define RTEMS_SHELL_PRIORITY OS_UTILITYTASK_PRIORITY

/*
**  External Declarations
*/
extern int rtems_rtl_shell_command(int argc, char *argv[]);

/*
 * Additional shell commands for the RTL functionality
 */
rtems_shell_cmd_t rtems_shell_RTL_Command = {
    .name = "rtl", .usage = "rtl COMMAND...", .topic = "misc", .command = rtems_rtl_shell_command};
rtems_shell_cmd_t rtems_shell_dlopen_Command = {
    .name = "dlopen", .usage = "dlopen COMMAND...", .topic = "misc", .command = shell_dlopen};
rtems_shell_cmd_t rtems_shell_dlsym_Command = {
    .name = "dlsym", .usage = "dlsym COMMAND...", .topic = "misc", .command = shell_dlsym};

void OS_BSP_Shell(void)
{
    int status;

    /* Supports command line enable/disable of batch mode */
    if (!OS_BSP_GenericRtemsGlobal.BatchMode)
    {
        printf("RTEMS_NO_SHELL:FALSE, BatchMode:FALSE, shell implemented and initialized");

        status = rtems_shell_init("SHLL", RTEMS_MINIMUM_STACK_SIZE * 4, RTEMS_SHELL_PRIORITY, "/dev/console", false,
                                  false, NULL);
        if (status < 0)
        {
            printf("shell init failed: %d / %s\n", status, strerror(errno));
        }

        /* give a small delay to let the shell start,
           avoids having the login prompt show up mid-test,
           and gives a little time for pending output to actually
           be sent to the console in case of a slow port */
        rtems_task_wake_after(50);
    }
    else
    {
        printf("RTEMS_NO_SHELL:FALSE, BatchMode:TRUE, shell implemented but not initialized");
    }
}

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS

#define CONFIGURE_SHELL_USER_COMMANDS &rtems_shell_RTL_Command, &rtems_shell_dlopen_Command, &rtems_shell_dlsym_Command

#include <rtems/shellconfig.h>
