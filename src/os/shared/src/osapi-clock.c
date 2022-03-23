/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to clock getting / setting.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
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
#include "os-shared-clock.h"

/*----------------------------------------------------------------
 *
 * Function: OS_GetLocalTime
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    /* Check parameters */
    OS_CHECK_POINTER(time_struct);

    return OS_GetLocalTime_Impl(time_struct);

} /* end OS_GetLocalTime */

/*----------------------------------------------------------------
 *
 * Function: OS_SetLocalTime
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SetLocalTime(const OS_time_t *time_struct)
{
    /* Check parameters */
    OS_CHECK_POINTER(time_struct);

    return OS_SetLocalTime_Impl(time_struct);

} /* end OS_SetLocalTime */
