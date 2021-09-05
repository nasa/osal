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
 * \file     coveragetest-network.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"

#include "os-shared-network.h"

#include "OCS_unistd.h"
#include "OCS_hostLib.h"

/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void Test_OS_NetworkGetHostName_Impl(void)

{
    /*
     * Test Case For:
     * int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
     */
    char buffer[16];

    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName_Impl(buffer, sizeof(buffer)), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OCS_gethostname), -1);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName_Impl(buffer, sizeof(buffer)), OS_ERROR);

} /* end OS_NetworkGetHostName_Impl */

void Test_OS_NetworkGetID_Impl(void)

{
    /*
     * Test Case For:
     * int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
     */
    int32 IdBuf;

    UT_SetDefaultReturnValue(UT_KEY(OCS_hostGetByName), 1234);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl(&IdBuf), OS_SUCCESS);
    UtAssert_True(IdBuf == 1234, "IdBuf (%ld) == 1234", (long)IdBuf);

    UT_SetDefaultReturnValue(UT_KEY(OCS_hostGetByName), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl(&IdBuf), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OCS_gethostname), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl(&IdBuf), OS_ERROR);

} /* end OS_NetworkGetID_Impl */

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_NetworkGetID_Impl);
    ADD_TEST(OS_NetworkGetHostName_Impl);
}
