/**
 * \file
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-vxworks.h"

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_ModuleAPI_Impl_Init(void)
{
    /* nothing to init, but needs to return SUCCESS to allow the rest of OSAL to work */
    return (OS_SUCCESS);
} /* end OS_VxWorks_ModuleAPI_Impl_Init */
