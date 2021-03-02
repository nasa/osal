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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-version.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

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
    static const char DEFAULT[] = "UT";
    void *            Buffer;
    const char *      RetVal;

    UT_GetDataBuffer(UT_KEY(OS_GetVersionString), &Buffer, NULL, NULL);
    if (Buffer == NULL)
    {
        RetVal = DEFAULT;
    }
    else
    {
        RetVal = Buffer;
    }

    return RetVal;
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
    static const char DEFAULT[] = "UT";
    void *            Buffer;
    const char *      RetVal;

    UT_GetDataBuffer(UT_KEY(OS_GetVersionCodeName), &Buffer, NULL, NULL);
    if (Buffer == NULL)
    {
        RetVal = DEFAULT;
    }
    else
    {
        RetVal = Buffer;
    }

    return RetVal;
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
    UT_Stub_RegisterContext(UT_KEY(OS_GetVersionNumber), VersionNumbers);
    UT_DEFAULT_IMPL(VersionNumbers);
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
    int32 status;

    status = UT_DEFAULT_IMPL(OS_GetBuildNumber);

    return status;
}
