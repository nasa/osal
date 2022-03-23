/**
 * \file
 * \ingroup ut-stubs
 *
 * Declarations and prototypes for bsp-impl
 */

#ifndef OVERRIDE_BSP_IMPL_H
#define OVERRIDE_BSP_IMPL_H

#include "OCS_bsp-impl.h"

#define OS_BSP_CONSOLEMODE_NORMAL    OCS_OS_BSP_CONSOLEMODE_NORMAL
#define OS_BSP_CONSOLEMODE_RED       OCS_OS_BSP_CONSOLEMODE_RED
#define OS_BSP_CONSOLEMODE_GREEN     OCS_OS_BSP_CONSOLEMODE_GREEN
#define OS_BSP_CONSOLEMODE_BLUE      OCS_OS_BSP_CONSOLEMODE_BLUE
#define OS_BSP_CONSOLEMODE_HIGHLIGHT OCS_OS_BSP_CONSOLEMODE_HIGHLIGHT

#define OS_BSP_Lock_Impl           OCS_OS_BSP_Lock_Impl
#define OS_BSP_ConsoleOutput_Impl  OCS_OS_BSP_ConsoleOutput_Impl
#define OS_BSP_ConsoleSetMode_Impl OCS_OS_BSP_ConsoleSetMode_Impl
#define OS_BSP_Unlock_Impl         OCS_OS_BSP_Unlock_Impl

/*********************
   END bsp-impl.h
 *********************/

#endif /* OVERRIDE_BSP_IMPL_H */
