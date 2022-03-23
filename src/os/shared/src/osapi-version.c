/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *  Defines functions that return version information
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include "osapi-version.h"

/*----------------------------------------------------------------
 *
 * Function: OS_GetVersionString
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
const char *OS_GetVersionString(void)
{
    return OS_VERSION;
}

/*----------------------------------------------------------------
 *
 * Function: OS_GetVersionCodeName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
const char *OS_GetVersionCodeName(void)
{
    return OS_VERSION_CODENAME;
}

/*----------------------------------------------------------------
 *
 * Function: OS_GetVersionNumber
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_GetVersionNumber(uint8 VersionNumbers[4])
{
    VersionNumbers[0] = OS_MAJOR_VERSION;
    VersionNumbers[1] = OS_MINOR_VERSION;
    VersionNumbers[2] = OS_REVISION;
    VersionNumbers[3] = OS_MISSION_REV;
}

/*----------------------------------------------------------------
 *
 * Function: OS_GetBuildNumber
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
uint32 OS_GetBuildNumber(void)
{
    return OS_BUILD_NUMBER;
}
