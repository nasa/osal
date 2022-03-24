/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 * Exercise the "GetVersion" functions.  These functions do not have any actual
 * logic, they just directly return fixed strings, but they should be called as
 * part of unit test for coverage reasons.
 *
 * The strings are free-form and no specific format is checked, the only real
 * requirement is that they are not NULL.
 */
#include "os-shared-coveragetest.h"

#include "osapi-version.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_GetVersionString(void)
{
    /*
     * Test Case For:
     * const char *OS_GetVersionString(void)
     */
    const char *Result;

    Result = OS_GetVersionString();
    UtAssert_NOT_NULL(Result);

    /*
     * Display the version description string, just for informational purposes
     */
    UtPrintf("OS_GetVersionString() Returned: %s\n", Result);
}

void Test_OS_GetVersionCodeName(void)
{
    /*
     * Test Case For:
     * const char *OS_GetVersionCodeName(void)
     */
    const char *Result;

    Result = OS_GetVersionCodeName();
    UtAssert_NOT_NULL(Result);

    /*
     * Display the code name string, just for informational purposes
     */
    UtPrintf("OS_GetVersionCodeName() Returned: %s\n", Result);
}

void Test_OS_GetVersionNumber(void)
{
    /*
     * Test Case For:
     * void OS_GetVersionNumber(uint8 VersionNumbers[4])
     */
    uint8 VersionNum[4] = {0};

    OS_GetVersionNumber(VersionNum);

    /*
     * This should output the same info as the version macros
     */
    UtAssert_INT32_EQ(VersionNum[0], OS_MAJOR_VERSION);
    UtAssert_INT32_EQ(VersionNum[1], OS_MINOR_VERSION);
    UtAssert_INT32_EQ(VersionNum[2], OS_REVISION);
    UtAssert_INT32_EQ(VersionNum[3], OS_MISSION_REV);

    /*
     * Display the version number, just for informational purposes
     */
    UtPrintf("OS_GetVersionNumber() Returned: %u.%u.%u.%u\n", (unsigned int)VersionNum[0], (unsigned int)VersionNum[1],
             (unsigned int)VersionNum[2], (unsigned int)VersionNum[3]);
}

void Test_OS_GetBuildNumber(void)
{
    /*
     * Test Case For:
     * uint32 OS_GetBuildNumber(void)
     */
    uint32 Result;

    Result = OS_GetBuildNumber();
    UtAssert_NONZERO(Result);

    /*
     * Display the build number, just for informational purposes
     */
    UtPrintf("Test_OS_GetBuildNumber() Returned: %lu\n", (unsigned long)Result);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void) {}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GetVersionString);
    ADD_TEST(OS_GetVersionCodeName);
    ADD_TEST(OS_GetVersionNumber);
    ADD_TEST(OS_GetBuildNumber);
}
