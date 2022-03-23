/*
 * File:  bsp_console.c
 *
 * Purpose:
 *   OSAL BSP debug console abstraction
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pcrtems_bsp_internal.h"
#include "bsp-impl.h"

/****************************************************************************************
                    BSP CONSOLE IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   OS_BSP_ConsoleOutput_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, size_t DataLen)
{
    /* writes the raw data directly to STDOUT_FILENO (unbuffered) */
    write(STDOUT_FILENO, Str, DataLen);
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    /* no-op on RTEMS */
}
