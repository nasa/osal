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
#include "os-impl-loader.h"
#include "os-shared-module.h"

OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

/*----------------------------------------------------------------
 *
 * Function: OS_Posix_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_ModuleAPI_Impl_Init(void)
{
    memset(OS_impl_module_table, 0, sizeof(OS_impl_module_table));
    return (OS_SUCCESS);
} /* end OS_Posix_ModuleAPI_Impl_Init */
