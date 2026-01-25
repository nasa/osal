/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * \brief Coverage test for no network implementation
 * \ingroup  portable
 */

#include "os-portable-coveragetest.h"
#include "os-shared-sockets.h"
#include "os-shared-idmap.h"
#include "os-shared-file.h"
#include "os-shared-select.h"

#include "OCS_sys_socket.h"
#include "OCS_errno.h"
#include "OCS_fcntl.h"
#include "OCS_arpa_inet.h"

#include "ut-adaptor-portable-posix-io.h"

/* Unique error code for return testing */
#define UT_ERR_UNIQUE 0xDEADBEEF

/* Buffer size */
#define UT_BUFFER_SIZE 16

void Test_OS_SocketAddrToString_Impl(void)
{
    char                 buffer[UT_BUFFER_SIZE];
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERR_BAD_ADDRESS);

    /* AF_INET6 success */
    sa->sa_family = OCS_AF_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_SUCCESS);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_SUCCESS);

    /* AF_INET failed inet_ntop */
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_ntop), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERROR);
}

void Test_OS_SocketAddrFromString_Impl(void)
{
    const char           buffer[UT_BUFFER_SIZE] = "UT";
    OS_SockAddr_t        addr                   = {0};
    struct OCS_sockaddr *sa                     = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_BAD_ADDRESS);

    /* AF_INET6 success */
    sa->sa_family = OCS_AF_INET6;
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_pton), 1, 1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_SUCCESS);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_pton), 1, 1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_SUCCESS);

    /* AF_INET, unable to convert (note inet_pton returns 0 if it failed) */
    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_pton), 1, 0);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERROR);
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
    ADD_TEST(OS_SocketAddrToString_Impl);
    ADD_TEST(OS_SocketAddrFromString_Impl);
}
