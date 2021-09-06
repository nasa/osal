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

/*
 * File:  bsp_console.c
 *
 * Purpose:
 *   OSAL BSP debug console abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "generic_linux_bsp_internal.h"
#include "bsp-impl.h"

/*----------------------------------------------------------------
   OS_BSP_ExecTput()

   Helper function: Use the system "tput" utility to set the given
   console capability.

   This uses a fork/exec to invoke the external command which outputs
   the control sequence directly to the controlling terminal.

   It is assumed that this will only be used during debug/testing.
   Otherwise it would be preferable to cache the control strings to
   avoid repetitive fork/exec operations.
 ------------------------------------------------------------------*/
static void OS_BSP_ExecTput(const char *cap, const char *param)
{
    pid_t cpid;
    int   status;

    cpid = fork();
    if (cpid < 0)
    {
        return;
    }
    if (cpid == 0)
    {
        execlp("tput", "tput", cap, param, NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(cpid, &status, 0);
}

/****************************************************************************************
                    BSP CONSOLE IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   OS_BSP_ConsoleOutput_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, size_t DataLen)
{
    ssize_t WriteLen;

    while (DataLen > 0)
    {
        /* writes the raw data directly to STDOUT_FILENO (unbuffered) */
        WriteLen = write(STDOUT_FILENO, Str, DataLen);
        if (WriteLen <= 0)
        {
            /* no recourse if this fails, just stop. */
            break;
        }
        Str += WriteLen;
        DataLen -= WriteLen;
    }
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    char param[32];

    if (OS_BSP_GenericLinuxGlobal.EnableTermControl)
    {
        if (ModeBits == OS_BSP_CONSOLEMODE_NORMAL)
        {
            OS_BSP_ExecTput("sgr0", NULL);
        }
        else
        {
            if ((ModeBits & OS_BSP_CONSOLEMODE_HIGHLIGHT) == 0)
            {
                /* no highlight (standout) text */
                OS_BSP_ExecTput("rmso", NULL);
            }
            else
            {
                /* set highlight (standout) text */
                OS_BSP_ExecTput("smso", NULL);
            }

            snprintf(param, sizeof(param), "%d", OS_BSP_CONSOLEMODE_TO_ANSICOLOR(ModeBits));
            OS_BSP_ExecTput("setaf", param);
        }
    }
}
