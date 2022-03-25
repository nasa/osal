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
 */
#include "os-shared-coveragetest.h"
#include "os-shared-network.h"

void Test_OS_NetworkAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_NetworkAPI_Init(), OS_SUCCESS);
}

void Test_OS_NetworkGetHostName(void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkGetHostName(char *host_name, uint32 name_len)
     */
    char Buffer[32];

    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName(Buffer, sizeof(Buffer)), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_NetworkGetHostName_Impl), -4444);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName(Buffer, sizeof(Buffer)), -4444);

    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName(NULL, sizeof(Buffer)), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName(Buffer, 0), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName(Buffer, UINT32_MAX), OS_ERR_INVALID_SIZE);
}

void Test_OS_NetworkGetID(void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkGetID(void)
     */

    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID(), 42);

    UT_SetDefaultReturnValue(UT_KEY(OS_NetworkGetID_Impl), -5555);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID(), -1);
}

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

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_NetworkAPI_Init);
    ADD_TEST(OS_NetworkGetID);
    ADD_TEST(OS_NetworkGetHostName);
}
