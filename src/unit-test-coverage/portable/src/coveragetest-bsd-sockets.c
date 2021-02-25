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
#include "os-shared-select.h"
#include "os-impl-io.h"

#include "OCS_sys_socket.h"
#include "OCS_errno.h"
#include "OCS_fcntl.h"
#include "OCS_arpa_inet.h"

#include "ut-adaptor-portable-posix-io.h"

/* Unique error code for return testing */
#define UT_ERR_UNIQUE 0xDEADBEEF

/* Buffer size */
#define UT_BUFFER_SIZE 16

/* OS_SelectSingle_Impl hook to set SelectFlags per input */
static int32 UT_Hook_OS_SelectSingle_Impl(void *UserObj, int32 StubRetcode, uint32 CallCount,
                                          const UT_StubContext_t *Context)
{
    uint32 *SelectFlags;

    SelectFlags = UT_Hook_GetArgValueByName(Context, "SelectFlags", uint32 *);

    if (SelectFlags != NULL)
    {
        *SelectFlags = *((uint32 *)UserObj);
    }

    return 0;
}

/* OCS_getsockopt hook to set sockopt per input */
static int32 UT_Hook_OCS_getsockopt(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    int *optval;

    optval = UT_Hook_GetArgValueByName(Context, "optval", int *);

    if (optval != NULL)
    {
        *optval = *((int *)UserObj);
    }

    return 0;
}

void Test_OS_SocketOpen_Impl(void)
{
    OS_object_token_t token = {0};

    /* Set up token for index 0 */
    token.obj_idx = UT_INDEX_0;
    UT_PortablePosixIOTest_ResetImpl(token.obj_idx);

    /* Invalid socket type */
    OS_stream_table[0].socket_type = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERR_NOT_IMPLEMENTED);

    /* Invalid domain type */
    OS_stream_table[0].socket_type   = OS_SocketType_DATAGRAM;
    OS_stream_table[0].socket_domain = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERR_NOT_IMPLEMENTED);

    /* Fail socket */
    OS_stream_table[0].socket_domain = OS_SocketDomain_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_socket), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_ERROR);

    /* Success case */
    OS_stream_table[0].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[0].socket_domain = OS_SocketDomain_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_SUCCESS);
    UtAssert_True(UT_PortablePosixIOTest_Get_Selectable(token.obj_idx), "Socket is selectable");

    /* Failure in fcntl() GETFL */
    UT_PortablePosixIOTest_ResetImpl(token.obj_idx);
    UT_ResetState(UT_KEY(OCS_fcntl));
    UT_SetDeferredRetcode(UT_KEY(OCS_fcntl), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_fcntl, 1);
    UtAssert_True(!UT_PortablePosixIOTest_Get_Selectable(token.obj_idx),
                  "Socket not selectable without O_NONBLOCK flag");

    /* Failure in fcntl() SETFL */
    UT_PortablePosixIOTest_ResetImpl(token.obj_idx);
    UT_ResetState(UT_KEY(OCS_fcntl));
    UT_SetDeferredRetcode(UT_KEY(OCS_fcntl), 2, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen_Impl, (&token), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_fcntl, 2);
    UtAssert_True(!UT_PortablePosixIOTest_Get_Selectable(token.obj_idx),
                  "Socket not selectable without O_NONBLOCK flag");
}

void Test_OS_SocketBind_Impl(void)
{
    OS_object_token_t    token = {0};
    OS_SockAddr_t        addr  = {0};
    struct OCS_sockaddr *sa    = (struct OCS_sockaddr *)&addr.AddrData;

    /* Set up token for index 0 */
    token.obj_idx = UT_INDEX_0;

    /* Default family case */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (&token, &addr), OS_ERR_BAD_ADDRESS);

    /* Note - not attempting to hit addrlen > OS_SOCKADDR_MAX_LEN at this point (NOT MC/DC)
     * would require compiling with a small OS_SOCKADDR_MAX_LEN or bigger structure */

    /* Fail bind */
    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_bind), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (&token, &addr), OS_ERROR);

    /* Fail listen */
    sa->sa_family                  = OCS_AF_INET6;
    OS_stream_table[0].socket_type = OS_SocketType_STREAM;
    UT_SetDeferredRetcode(UT_KEY(OCS_listen), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (&token, &addr), OS_ERROR);

    /* Success with OS_SocketType_STREAM */
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (&token, &addr), OS_SUCCESS);

    /* Success with ~OS_SocketType_STREAM */
    OS_stream_table[0].socket_type = ~OS_SocketType_STREAM;
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind_Impl, (&token, &addr), OS_SUCCESS);
}

void Test_OS_SocketConnect_Impl(void)
{
    OS_object_token_t    token = {0};
    OS_SockAddr_t        addr  = {0};
    struct OCS_sockaddr *sa    = (struct OCS_sockaddr *)&addr.AddrData;
    int32                selectflags;
    int                  sockopt;

    /* Set up token for index 0 */
    token.obj_idx = UT_INDEX_0;

    /* Default family case */
    sa->sa_family     = -1;
    addr.ActualLength = sizeof(struct OCS_sockaddr_in);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_ERR_BAD_ADDRESS);

    /* Sucessful connect */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_SUCCESS);

    /* Fail connect, errno ! EINPROGRESS */
    OCS_errno = ~OCS_EINPROGRESS;
    UT_SetDefaultReturnValue(UT_KEY(OCS_connect), -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_ERROR);

    /* Fail OS_SelectSingle_Impl, errno == EINPROGRESS */
    OCS_errno                              = OCS_EINPROGRESS;
    sa->sa_family                          = OCS_AF_INET6;
    addr.ActualLength                      = sizeof(struct OCS_sockaddr_in6);
    OS_impl_filehandle_table[0].selectable = true;
    UT_SetDeferredRetcode(UT_KEY(OS_SelectSingle_Impl), 1, UT_ERR_UNIQUE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), UT_ERR_UNIQUE);

    /* Timout error by clearing select flags with hook */
    selectflags = 0;
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), UT_Hook_OS_SelectSingle_Impl, &selectflags);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_ERROR_TIMEOUT);
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), NULL, NULL);

    /* Fail getsockopt status */
    UT_SetDeferredRetcode(UT_KEY(OCS_getsockopt), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_ERROR);

    /* Nonzero getsockopt sockopt */
    sockopt = 1;
    UT_SetHookFunction(UT_KEY(OCS_getsockopt), UT_Hook_OCS_getsockopt, &sockopt);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_ERROR);
    UT_SetHookFunction(UT_KEY(OCS_getsockopt), NULL, NULL);

    /* Success case with selectable */
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_SUCCESS);

    /* Success case with not selectable */
    OS_impl_filehandle_table[0].selectable = false;
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect_Impl, (&token, &addr, 0), OS_SUCCESS);
}

void Test_OS_SocketAccept_Impl(void)
{
    OS_object_token_t sock_token = {0};
    OS_object_token_t conn_token = {0};
    OS_SockAddr_t     addr       = {0};
    int32             selectflags;

    /* Set up tokens */
    sock_token.obj_idx = UT_INDEX_0;
    conn_token.obj_idx = UT_INDEX_1;

    /* Fail OS_SelectSingle_Impl with sock_token selectable */
    OS_impl_filehandle_table[0].selectable = true;
    UT_SetDeferredRetcode(UT_KEY(OS_SelectSingle_Impl), 1, UT_ERR_UNIQUE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept_Impl, (&sock_token, &conn_token, &addr, 0), UT_ERR_UNIQUE);

    /* Timeout by clearing select flags with hook */
    selectflags = 0;
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), UT_Hook_OS_SelectSingle_Impl, &selectflags);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept_Impl, (&sock_token, &conn_token, &addr, 0), OS_ERROR_TIMEOUT);
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), NULL, NULL);

    /* Clear selectable and fail accept */
    OS_impl_filehandle_table[0].selectable = false;
    UT_SetDeferredRetcode(UT_KEY(OCS_accept), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept_Impl, (&sock_token, &conn_token, &addr, 0), OS_ERROR);

    /* Success case */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept_Impl, (&sock_token, &conn_token, &addr, 0), OS_SUCCESS);
}

void Test_OS_SocketRecvFrom_Impl(void)
{
    OS_object_token_t token = {0};
    uint8             buffer[UT_BUFFER_SIZE];
    OS_SockAddr_t     addr = {0};
    int32             selectflags;

    /* Set up token */
    token.obj_idx = UT_INDEX_0;

    /* NULL RemoteAddr, selectable, fail OS_SelectSingle_Impl */
    OS_impl_filehandle_table[0].selectable = true;
    UT_SetDeferredRetcode(UT_KEY(OS_SelectSingle_Impl), 1, UT_ERR_UNIQUE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), NULL, 0), UT_ERR_UNIQUE);

    /* Timeout by clearing select flags with hook */
    selectflags = 0;
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), UT_Hook_OS_SelectSingle_Impl, &selectflags);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), &addr, 0), OS_ERROR_TIMEOUT);
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), NULL, NULL);

    /* Not selectable, 0 timeout, EAGAIN error from recvfrom error */
    OS_impl_filehandle_table[0].selectable = false;
    OCS_errno                              = OCS_EAGAIN;
    UT_SetDeferredRetcode(UT_KEY(OCS_recvfrom), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), &addr, 0), OS_QUEUE_EMPTY);

    /* With timeout, other error from recvfrom error */
    OCS_errno = 0;
    UT_SetDeferredRetcode(UT_KEY(OCS_recvfrom), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), &addr, 1), OS_ERROR);

    /* With timeout, EWOULDBLOCK error from recvfrom error */
    OCS_errno = OCS_EWOULDBLOCK;
    UT_SetDeferredRetcode(UT_KEY(OCS_recvfrom), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), &addr, 1), OS_QUEUE_EMPTY);

    /* Success with NULL RemoteAddr */
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), NULL, 0), OS_SUCCESS);

    /* Success with non-NULL RemoteAddr */
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom_Impl, (&token, buffer, sizeof(buffer), &addr, 0), OS_SUCCESS);
}

void Test_OS_SocketSendTo_Impl(void)
{
    OS_object_token_t    token = {0};
    uint8                buffer[UT_BUFFER_SIZE];
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Set up token */
    token.obj_idx = UT_INDEX_0;

    /* Bad adderss length */
    sa->sa_family     = -1;
    addr.ActualLength = sizeof(struct OCS_sockaddr_in);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo_Impl, (&token, buffer, sizeof(buffer), &addr), OS_ERR_BAD_ADDRESS);

    /* AF_INET, failed sendto */
    sa->sa_family = OCS_AF_INET;
    UT_SetDeferredRetcode(UT_KEY(OCS_sendto), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo_Impl, (&token, buffer, sizeof(buffer), &addr), OS_ERROR);

    /* AF_INET6, success */
    sa->sa_family     = OCS_AF_INET6;
    addr.ActualLength = sizeof(struct OCS_sockaddr_in6);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo_Impl, (&token, buffer, sizeof(buffer), &addr), OS_SUCCESS);
}

void Test_OS_SocketGetInfo_Impl(void)
{
    OSAPI_TEST_FUNCTION_RC(OS_SocketGetInfo_Impl, (NULL, NULL), OS_SUCCESS);
}

void Test_OS_SocketAddrInit_Impl(void)
{
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Unknown domain */
    sa->sa_family     = -1;
    addr.ActualLength = 5;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit_Impl, (&addr, -1), OS_ERR_NOT_IMPLEMENTED);
    UtAssert_INT32_EQ(sa->sa_family, 0);
    UtAssert_INT32_EQ(addr.ActualLength, 0);

    /* Note - not attempting to hit addrlen > OS_SOCKADDR_MAX_LEN at this point (NOT MC/DC)
     * would require compiling with a small OS_SOCKADDR_MAX_LEN or bigger structure */

    /* INET, success */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit_Impl, (&addr, OS_SocketDomain_INET), OS_SUCCESS);
    UtAssert_INT32_EQ(sa->sa_family, OCS_AF_INET);
    UtAssert_INT32_EQ(addr.ActualLength, sizeof(struct OCS_sockaddr_in));

    /* AF_INET6, success */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit_Impl, (&addr, OS_SocketDomain_INET6), OS_SUCCESS);
    UtAssert_INT32_EQ(sa->sa_family, OCS_AF_INET6);
    UtAssert_INT32_EQ(addr.ActualLength, sizeof(struct OCS_sockaddr_in6));
}

void Test_OS_SocketAddrToString_Impl(void)
{
    char                 buffer[UT_BUFFER_SIZE];
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERR_BAD_ADDRESS);

    /* AF_INET6 failed inet_ntop */
    sa->sa_family = OCS_AF_INET6;
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_ntop), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_ERROR);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString_Impl, (buffer, sizeof(buffer), &addr), OS_SUCCESS);
}

void Test_OS_SocketAddrFromString_Impl(void)
{
    char                 buffer[UT_BUFFER_SIZE];
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERR_BAD_ADDRESS);

    /* AF_INET6 failed inet_ntop */
    sa->sa_family = OCS_AF_INET6;
    UT_SetDeferredRetcode(UT_KEY(OCS_inet_pton), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_ERROR);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString_Impl, (&addr, buffer), OS_SUCCESS);
}

void Test_OS_SocketAddrGetPort_Impl(void)
{
    uint16               port;
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort_Impl, (&port, &addr), OS_ERR_BAD_ADDRESS);

    /* AF_INET6, success */
    sa->sa_family = OCS_AF_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort_Impl, (&port, &addr), OS_SUCCESS);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort_Impl, (&port, &addr), OS_SUCCESS);
}

void Test_OS_SocketAddrSetPort_Impl(void)
{
    uint16               port = 1;
    OS_SockAddr_t        addr = {0};
    struct OCS_sockaddr *sa   = (struct OCS_sockaddr *)&addr.AddrData;

    /* Bad family */
    sa->sa_family = -1;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort_Impl, (&addr, port), OS_ERR_BAD_ADDRESS);

    /* AF_INET6, success */
    sa->sa_family = OCS_AF_INET6;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort_Impl, (&addr, port), OS_SUCCESS);

    /* AF_INET, success */
    sa->sa_family = OCS_AF_INET;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort_Impl, (&addr, port), OS_SUCCESS);
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
    ADD_TEST(OS_SocketBind_Impl);
    ADD_TEST(OS_SocketConnect_Impl);
    ADD_TEST(OS_SocketAccept_Impl);
    ADD_TEST(OS_SocketRecvFrom_Impl);
    ADD_TEST(OS_SocketSendTo_Impl);
    ADD_TEST(OS_SocketGetInfo_Impl);
    ADD_TEST(OS_SocketAddrInit_Impl);
    ADD_TEST(OS_SocketAddrToString_Impl);
    ADD_TEST(OS_SocketAddrFromString_Impl);
    ADD_TEST(OS_SocketAddrGetPort_Impl);
    ADD_TEST(OS_SocketAddrSetPort_Impl);
}
