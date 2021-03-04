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
 * \brief Coverage test for no network implementation
 * \ingroup  portable
 */

#include "os-portable-coveragetest.h"
#include "os-shared-sockets.h"
#include "os-shared-idmap.h"
#include "os-shared-file.h"

#include "OCS_sys_socket.h"

void Test_OS_SocketOpen_Impl(void)
{
    OS_object_token_t token = {0};

    /* Set up token for index 0 */
    token.obj_idx = UT_INDEX_0;

    /* Invalid socket type */
    OS_stream_table[0].socket_type = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERR_NOT_IMPLEMENTED);

    /* Invalid domain type */
    OS_stream_table[0].socket_type   = OS_SocketType_DATAGRAM;
    OS_stream_table[0].socket_domain = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERR_NOT_IMPLEMENTED);

    /* Socket error */
    OS_stream_table[0].socket_domain = OS_SocketDomain_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_socket), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERROR);

    /* Success case */
    OS_stream_table[0].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[0].socket_domain = OS_SocketDomain_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_SUCCESS);
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
    memset(OS_stream_table, 0, sizeof(OS_stream_table));
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
    ADD_TEST(OS_SocketOpen_Impl);
}
