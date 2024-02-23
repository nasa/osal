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
 *   OSAL BSP command line implementation
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

#include "pcrtems_bsp_internal.h"

/*
 * BSP compile-time tuning
 */
#define RTEMS_MAX_USER_OPTIONS 4
#define RTEMS_MAX_CMDLINE      256

void OS_BSP_CmdLine(void)
{
    char        userargbuffer[RTEMS_MAX_CMDLINE];
    const char *cmdlinestr;
    const char *cmdp;
    char *      cmdi, *cmdo;

    cmdlinestr = bsp_cmdline();

    /*
     * Parse command line string (passed in from bootloader)
     *
     * Known arguments are handled here, and unknown args are
     * saved for the UT application.
     *
     * Batch mode is intended for non-interactive execution.
     *
     * It does two things:
     * - do not start the shell task
     * - when tests are complete, shutdown the executive
     *
     * The BSP should be configured with these options to
     * make this most useful:
     *   USE_COM1_AS_CONSOLE=1
     *   BSP_PRESS_KEY_FOR_RESET=0
     *   BSP_RESET_BOARD_AT_EXIT=1
     *
     * This way all the test output will be sent to COM1
     * and then immediately resets the CPU when done.
     *
     * When running under QEMU the "-no-reboot" flag is
     * also useful to shutdown QEMU rather than resetting.
     */
    if (cmdlinestr != NULL)
    {
        printf(" Bootloader Command Line: %s\n", cmdlinestr);

        cmdp = cmdlinestr;
        cmdo = NULL;
        cmdi = NULL;

        while (1)
        {
            if (isgraph((int)*cmdp))
            {
                if (cmdo == NULL)
                {
                    cmdo = userargbuffer;
                }
                else
                {
                    ++cmdo;
                }
                if (cmdi == NULL)
                {
                    cmdi = cmdo;
                }
                *cmdo = *cmdp;
            }
            else if (cmdi != NULL)
            {
                ++cmdo;
                *cmdo = 0;
                if (strcmp(cmdi, "--batch-mode") == 0)
                {
                    OS_BSP_PcRtemsGlobal.BatchMode = true;
                }
                else if (OS_BSP_Global.ArgC < RTEMS_MAX_USER_OPTIONS)
                {
                    /* save other args for app */
                    OS_BSP_Global.ArgV[OS_BSP_Global.ArgC] = cmdi;
                    ++OS_BSP_Global.ArgC;
                }
                cmdi = NULL;
            }

            if (*cmdp == 0)
            {
                break;
            }

            ++cmdp;
        }
    }
}
