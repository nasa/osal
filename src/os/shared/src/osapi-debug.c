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
 * Function: OS_DebugPrintf
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

} /* end OS_DebugPrintf */
