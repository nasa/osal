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
 * \brief Coverage test for no socket implementation
 * \ingroup  portable
 */

#include "os-portable-coveragetest.h"
#include "os-shared-sockets.h"

void Test_No_Sockets(void)
{
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (NULL, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (NULL, NULL, 0), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept_Impl, (NULL, NULL, NULL, 0), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown_Impl, (NULL, 0), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (NULL, NULL, 0, NULL, 0), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo_Impl, (NULL, NULL, 0, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketGetInfo_Impl, (NULL, NULL), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit_Impl, (NULL, 0), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (NULL, 0, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (NULL, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort_Impl, (NULL, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort_Impl, (NULL, 0), OS_ERR_NOT_IMPLEMENTED);
}

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
    ADD_TEST(No_Sockets);
}
