/**
 * \file
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *      Uses the BSP-provided "console write" function
 *      Note this only supports a single console
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <string.h>
#include <errno.h>

#include "osapi-printf.h"

#include "bsp-impl.h"

#include "os-impl-console.h"
#include "os-shared-printf.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                CONSOLE OUTPUT
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleOutput_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ConsoleOutput_Impl(const OS_object_token_t *token)
{
    size_t                        StartPos;
    size_t                        EndPos;
    size_t                        WriteSize;
    OS_console_internal_record_t *console;

    console  = OS_OBJECT_TABLE_GET(OS_console_table, *token);
    StartPos = console->ReadPos;
    EndPos   = console->WritePos;

    OS_BSP_Lock_Impl();

    while (StartPos != EndPos)
    {
        if (StartPos > EndPos)
        {
            /* handle wrap */
            WriteSize = console->BufSize - StartPos;
        }
        else
        {
            WriteSize = EndPos - StartPos;
        }

        OS_BSP_ConsoleOutput_Impl(&console->BufBase[StartPos], WriteSize);

        StartPos += WriteSize;
        if (StartPos >= console->BufSize)
        {
            /* handle wrap */
            StartPos = 0;
        }
    }

    OS_BSP_Unlock_Impl();

    /* Update the global with the new read location */
    console->ReadPos = StartPos;
} /* end OS_ConsoleOutput_Impl */
