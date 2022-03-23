/**
 * \file
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "osapi.h"
#include "os-shared-module.h"

/*----------------------------------------------------------------
 *
 * Function: OS_Posix_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_ModuleAPI_Impl_Init(void)
{
    /* nothing to init in this mode */
    return (OS_SUCCESS);
} /* end OS_Posix_ModuleAPI_Impl_Init */
