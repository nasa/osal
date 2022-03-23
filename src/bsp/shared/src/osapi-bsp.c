/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file  contains some of the OS APIs abstraction layer code
 * that is shared/common across all BSP implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "bsp-impl.h"

/*
 * BSP Global Data Object
 */
OS_BSP_GlobalData_t OS_BSP_Global;

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */

/*----------------------------------------------------------------
   OS_BSP_GetArgC
   See full description in header
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetArgC(void)
{
    return OS_BSP_Global.ArgC;
}

/*----------------------------------------------------------------
   OS_BSP_GetArgV
   See full description in header
 ------------------------------------------------------------------*/
char *const *OS_BSP_GetArgV(void)
{
    return OS_BSP_Global.ArgV;
}

/*----------------------------------------------------------------
   OS_BSP_SetExitCode
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_SetExitCode(int32 code)
{
    OS_BSP_Global.AppStatus = code;
}
