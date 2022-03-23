/*
 * File:  bsp_console.c
 *
 * Purpose:
 *   OSAL BSP debug console abstraction
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "generic_vxworks_bsp_internal.h"
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
    while (DataLen > 0)
    {
        putchar(*Str);
        ++Str;
        --DataLen;
    }
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    /* ignored; not implemented */
}
