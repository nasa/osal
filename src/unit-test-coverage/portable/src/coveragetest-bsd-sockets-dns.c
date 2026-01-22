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
#include "os-impl-io.h"

#include "OCS_sys_socket.h"
#include "OCS_errno.h"
#include "OCS_fcntl.h"
#include "OCS_arpa_inet.h"
#include "OCS_netdb.h"

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

    sa->sa_family = OCS_AF_INET6;
    UT_SetDeferredRetcode(UT_KEY(OCS_getnameinfo), 1, OCS_EAI_NONAME);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERR_EMPTY_SET);

    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_getnameinfo), 1, OCS_EAI_AGAIN);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERR_TRY_AGAIN);

    /* other error */
    UT_SetDeferredRetcode(UT_KEY(OCS_getnameinfo), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERROR);

    /* AF_INET6, success */
    sa->sa_family = OCS_AF_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_SUCCESS);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_SUCCESS);
}

void Test_OS_SocketAddrFromString_Impl(void)
{
    const char           buffer[UT_BUFFER_SIZE] = "UT";
    OS_SockAddr_t        addr                   = {0};
    struct OCS_sockaddr *sa                     = (struct OCS_sockaddr *)&addr.AddrData;
    struct OCS_addrinfo  ai;
    struct OCS_addrinfo *aip;

    union
    {
        struct OCS_sockaddr     sa;
        struct OCS_sockaddr_in  sa_in4;
        struct OCS_sockaddr_in6 sa_in6;
    } sa_buf;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_BAD_ADDRESS);

    sa->sa_family = OCS_AF_INET6;
    UT_SetDeferredRetcode(UT_KEY(OCS_getaddrinfo), 1, OCS_EAI_NONAME);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_EMPTY_SET);

    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_getaddrinfo), 1, OCS_EAI_AGAIN);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_TRY_AGAIN);

    /* other error */
    UT_SetDeferredRetcode(UT_KEY(OCS_getaddrinfo), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERROR);

    /* AF_INET6, empty result, but still needs freeing */
    sa->sa_family = OCS_AF_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_EMPTY_SET);
    UtAssert_STUB_COUNT(OCS_freeaddrinfo, 1);

    /* AF_INET, empty result, but still needs freeing */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_EMPTY_SET);
    UtAssert_STUB_COUNT(OCS_freeaddrinfo, 2);

    /* set up stub for successful return */
    memset(&ai, 0, sizeof(ai));
    memset(&sa_buf, 0, sizeof(sa_buf));

    sa->sa_family = OCS_AF_INET;
    ai.ai_family  = sa->sa_family;
    ai.ai_addrlen = sizeof(struct OCS_sockaddr_in);
    ai.ai_addr    = &sa_buf.sa;

    aip = &ai;
    UT_SetDataBuffer(UT_KEY(OCS_getaddrinfo), &aip, sizeof(aip), false);

    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_freeaddrinfo, 3);
    UtAssert_EQ(size_t, addr.ActualLength, ai.ai_addrlen);

    sa->sa_family = OCS_AF_INET6;
    ai.ai_family  = sa->sa_family;
    ai.ai_addrlen = sizeof(struct OCS_sockaddr_in6);
    ai.ai_addr    = &sa_buf.sa;

    aip = &ai;
    UT_SetDataBuffer(UT_KEY(OCS_getaddrinfo), &aip, sizeof(aip), false);

    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_freeaddrinfo, 4);
    UtAssert_EQ(size_t, addr.ActualLength, ai.ai_addrlen);

    /* wrong family */
    ai.ai_family = OCS_AF_INET;
    UT_SetDataBuffer(UT_KEY(OCS_getaddrinfo), &aip, sizeof(aip), false);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_EMPTY_SET);
    UtAssert_STUB_COUNT(OCS_freeaddrinfo, 5);
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
