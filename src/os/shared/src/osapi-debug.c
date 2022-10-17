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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *      Contains the implementation for OS_DEBUG().
 *
 *      This is only compiled when OSAL_CONFIG_DEBUG_PRINTF is enabled.
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
#include "os-shared-globaldefs.h"
#include "os-shared-common.h"
#include "bsp-impl.h"

#define OS_DEBUG_OUTPUT_STREAM stdout
#define OS_DEBUG_MAX_LINE_LEN  132

/*----------------------------------------------------------------
 *
 *  Purpose: Outputs a single debug statement to the console
 *
 *-----------------------------------------------------------------*/
void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...)
{
    char    buffer[OS_DEBUG_MAX_LINE_LEN];
    va_list va;

    if (OS_SharedGlobalVars.DebugLevel >= Level)
    {
        /*
         * Lock the console so this appears coherently,
         * not mixed with other chars from other tasks
         */
        OS_BSP_Lock_Impl();

        snprintf(buffer, sizeof(buffer), "%s():%lu:", Func, (unsigned long)Line);
        OS_BSP_ConsoleOutput_Impl(buffer, strlen(buffer));

        va_start(va, Format);
        vsnprintf(buffer, sizeof(buffer), Format, va);
        va_end(va);

        OS_BSP_ConsoleOutput_Impl(buffer, strlen(buffer));

        OS_BSP_Unlock_Impl();
    }
}
