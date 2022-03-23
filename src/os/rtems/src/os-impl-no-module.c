/**
 * \file
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#define _USING_RTEMS_INCLUDES_

#include "os-rtems.h"

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_ModuleAPI_Impl_Init(void)
{
    /* nothing to init, but needs to return SUCCESS to allow the rest of OSAL to work */
    return (OS_SUCCESS);
} /* end OS_Rtems_ModuleAPI_Impl_Init */
