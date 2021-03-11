/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     osapi-version.c
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
