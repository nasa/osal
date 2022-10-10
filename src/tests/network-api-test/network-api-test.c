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

/*
 * Filename: network-api-test.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

#define UT_EXIT_LOOP_MAX 100

/*
 * Timeouts for various socket ops in the test cases
 *
 * Note that the act of calling any "assert" routine causes console output, which
 * can easily take tens or even hundreds of milliseconds to execute on platforms
 * where the console is on a slow serial port.  Therefore this timeout must
 * not be too short.
 */
#define UT_TIMEOUT 4000

/*
 * Variations of client->server connections to create.
 * This tests that the server socket can accept multiple connections,
 * and the various combinations of socket shutdown/closure work as expected.
 */
enum
{
    UT_STREAM_CONNECTION_INITIAL,        /* On first pass, just check basic read/writes */
    UT_STREAM_CONNECTION_REUSE_SERVER,   /* Second pass is the same, confirms server socket can be re-used */
    UT_STREAM_CONNECTION_READ_SHUTDOWN,  /* Third pass confirms that read shutdown works correctly */
    UT_STREAM_CONNECTION_WRITE_SHUTDOWN, /* Fourth pass confirms that write shutdown works correctly */
    UT_STREAM_CONNECTION_RDWR_SHUTDOWN,  /* Fifth pass confirms that read/write shutdown works correctly */
    UT_STREAM_CONNECTION_MAX
};

osal_id_t     fs_id;
osal_id_t     regular_file_id;
osal_id_t     s_task_id;
osal_id_t     p1_socket_id;
osal_id_t     p2_socket_id;
osal_id_t     s_socket_id;
osal_id_t     c_socket_id;
OS_SockAddr_t p1_addr;
OS_SockAddr_t p2_addr;
OS_SockAddr_t s_addr;
OS_SockAddr_t c_addr;
bool          networkImplemented = true;

int32 ServerFn_AcceptStatus;
char  ServerFn_ErrorString[128];
char  LongName[OS_MAX_API_NAME + 5];

/*****************************************************************************
 *
 * Datagram Network Functional Test Setup
 *
 *****************************************************************************/

void TestNetworkApiBadArgs(void)
{
    OS_SockAddr_t addr;
    osal_id_t     socket_id;
    uint16        port;
    char          addr_string[20];

    memset(&addr, 0, sizeof(addr));

    UtAssert_INT32_EQ(OS_SocketOpen(NULL, OS_SocketDomain_INVALID, OS_SocketType_INVALID), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketOpen(&socket_id, OS_SocketDomain_MAX, OS_SocketType_MAX), OS_ERR_NOT_IMPLEMENTED);

    UtAssert_INT32_EQ(OS_SocketAddrInit(&addr, OS_SocketDomain_INVALID), OS_ERR_NOT_IMPLEMENTED);
    UtAssert_INT32_EQ(OS_SocketAddrInit(NULL, OS_SocketDomain_INVALID), OS_INVALID_POINTER);

    /* OS_SocketAddrSetPort */
    UtAssert_INT32_EQ(OS_SocketAddrSetPort(&addr, 0xFFFF), OS_ERR_BAD_ADDRESS);
    UtAssert_INT32_EQ(OS_SocketAddrSetPort(NULL, 1234), OS_INVALID_POINTER);

    /* OS_SocketAddrGetPort */
    UtAssert_INT32_EQ(OS_SocketAddrGetPort(&port, &addr), OS_ERR_BAD_ADDRESS);
    UtAssert_INT32_EQ(OS_SocketAddrGetPort(NULL, &addr), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketAddrGetPort(&port, NULL), OS_INVALID_POINTER);

    /* init the addr value to be INET for to/from string tests */
    UtAssert_INT32_EQ(OS_SocketAddrInit(&addr, OS_SocketDomain_INET), OS_SUCCESS);

    /* OS_SocketAddrFromString */
    UtAssert_INT32_EQ(OS_SocketAddrFromString(NULL, "127.0.0.1"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketAddrFromString(&addr, NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketAddrFromString(&addr, "invalid"), OS_ERROR);

    /* OS_SocketAddrToString */
    UtAssert_INT32_EQ(OS_SocketAddrToString(addr_string, 0, &addr), OS_ERR_INVALID_SIZE);
    UtAssert_INT32_EQ(OS_SocketAddrToString(NULL, sizeof(addr_string), &addr), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketAddrToString(addr_string, sizeof(addr_string), NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketAddrToString(addr_string, 1, &addr), OS_ERROR);
}

void TestNetworkApiInet6(void)
{
    osal_id_t     socket_id = OS_OBJECT_ID_UNDEFINED;
    OS_SockAddr_t addr;
    int32         actual;

    /*
     * The INET6 protocol is an optional component -
     * Check for general support of this protocol by trying to init a
     * socket address of that type
     */
    actual = OS_SocketAddrInit(&addr, OS_SocketDomain_INET6);
    if (actual == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SocketAddrInit(&addr, OS_SocketDomain_INET6) - INET6 not supported");
    }
    else
    {
        UtAssert_True(actual == OS_SUCCESS, "OS_SocketAddrInit(&addr, OS_SocketDomain_INET6) (%ld) == OS_SUCCESS",
                      (long)actual);

        /* Try OS_SocketOpen - should work if INET6 is supported per above test */
        UtAssert_INT32_EQ(OS_SocketOpen(&socket_id, OS_SocketDomain_INET6, OS_SocketType_DATAGRAM), OS_SUCCESS);
        UtAssert_INT32_EQ(OS_close(socket_id), OS_SUCCESS);
    }
}

void TestDatagramNetworkApi_Setup(void)
{
    int32 expected;
    int32 actual;

    memset(LongName, 'x', sizeof(LongName) - 1);
    LongName[sizeof(LongName) - 1] = 0;

    /* Open a peer1 socket */
    expected     = OS_SUCCESS;
    p1_socket_id = OS_OBJECT_ID_UNDEFINED;

    actual = OS_SocketOpen(&p1_socket_id, OS_SocketDomain_INET, OS_SocketType_DATAGRAM);
    if (actual == OS_ERR_NOT_IMPLEMENTED)
    {
        networkImplemented = false;
    }
    else
    {
        UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(OS_ObjectIdDefined(p1_socket_id), "p1_socket_id (%lu) != 0", OS_ObjectIdToInteger(p1_socket_id));

        /* Initialize peer1 address */
        actual = OS_SocketAddrInit(&p1_addr, OS_SocketDomain_INET);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

        /* Set peer1 port */
        actual = OS_SocketAddrSetPort(&p1_addr, 9999);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

        /* Set peer1 address */
        actual = OS_SocketAddrFromString(&p1_addr, "127.0.0.1");
        UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

        /* Bind peer1 socket to address */
        actual = OS_SocketBind(p1_socket_id, &p1_addr);
        UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_SUCCESS", (long)actual);

        /* Open a peer2 socket */
        expected     = OS_SUCCESS;
        p2_socket_id = OS_OBJECT_ID_UNDEFINED;

        actual = OS_SocketOpen(&p2_socket_id, OS_SocketDomain_INET, OS_SocketType_DATAGRAM);
        UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(OS_ObjectIdDefined(p2_socket_id), "p2_socket_id (%lu) != 0", OS_ObjectIdToInteger(p2_socket_id));

        /* Initialize peer2 address */
        actual = OS_SocketAddrInit(&p2_addr, OS_SocketDomain_INET);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

        /* Set peer2 port */
        actual = OS_SocketAddrSetPort(&p2_addr, 9998);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

        /* Set peer2 address */
        actual = OS_SocketAddrFromString(&p2_addr, "127.0.0.1");
        UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

        /* Bind peer2 socket to address */
        actual = OS_SocketBind(p2_socket_id, &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_SUCCESS", (long)actual);

        /* Finally, open a regular file handle to test rejection if passed to a socket API call */
        UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, "./test", "/test"), OS_SUCCESS);
        UtAssert_INT32_EQ(OS_OpenCreate(&regular_file_id, "/test/test.txt", OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE,
                                        OS_READ_WRITE),
                          OS_SUCCESS);
    }
}

/*****************************************************************************
 *
 * Datagram Network Functional Test
 *
 *****************************************************************************/
void TestDatagramNetworkApi(void)
{
    char             AddrBuffer1[32];
    char             AddrBuffer2[32];
    char             AddrBuffer3[32];
    char             AddrBuffer4[32];
    uint32           Buf1  = 111;
    uint32           Buf2  = 0;
    uint32           Buf3  = 222;
    uint32           Buf4  = 0;
    osal_id_t        objid = OS_OBJECT_ID_UNDEFINED;
    osal_id_t        invalid_fd;
    uint16           PortNum = 0;
    OS_socket_prop_t prop;
    OS_SockAddr_t    l_addr;

    memset(AddrBuffer1, 0, sizeof(AddrBuffer1));
    memset(AddrBuffer2, 0, sizeof(AddrBuffer2));
    memset(AddrBuffer3, 0, sizeof(AddrBuffer3));
    memset(AddrBuffer4, 0, sizeof(AddrBuffer4));
    memset(&prop, 0, sizeof(prop));

    if (!networkImplemented)
    {
        UtAssert_NA("Network API not implemented");
        return;
    }

    /*
     * Check various error inputs
     * This set must be done after setting up a valid ID.
     */

    /* make a bad object ID by flipping the bits of a good object ID */
    invalid_fd = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(p2_socket_id) ^ 0xFFFFFFFF);

    /* OS_SocketBind */
    UtAssert_INT32_EQ(OS_SocketBind(OS_OBJECT_ID_UNDEFINED, &p2_addr), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketBind(invalid_fd, &p2_addr), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketBind(regular_file_id, &p2_addr), OS_ERR_INCORRECT_OBJ_TYPE);
    UtAssert_INT32_EQ(OS_SocketBind(p2_socket_id, &p2_addr), OS_ERR_INCORRECT_OBJ_STATE);
    UtAssert_INT32_EQ(OS_SocketBind(p2_socket_id, NULL), OS_INVALID_POINTER);

    /* OS_SocketRecvFrom */
    UtAssert_INT32_EQ(OS_SocketRecvFrom(OS_OBJECT_ID_UNDEFINED, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT),
                      OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketRecvFrom(invalid_fd, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketRecvFrom(regular_file_id, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT),
                      OS_ERR_INCORRECT_OBJ_TYPE);
    UtAssert_INT32_EQ(OS_SocketRecvFrom(p2_socket_id, NULL, sizeof(Buf2), &l_addr, UT_TIMEOUT), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketRecvFrom(p2_socket_id, &Buf2, 0, &l_addr, UT_TIMEOUT), OS_ERR_INVALID_SIZE);

    /* OS_SocketSendTo */
    UtAssert_INT32_EQ(OS_SocketSendTo(OS_OBJECT_ID_UNDEFINED, &Buf2, sizeof(Buf2), &l_addr), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketSendTo(invalid_fd, &Buf2, sizeof(Buf2), &l_addr), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketSendTo(regular_file_id, &Buf2, sizeof(Buf2), &l_addr), OS_ERR_INCORRECT_OBJ_TYPE);
    UtAssert_INT32_EQ(OS_SocketSendTo(p2_socket_id, NULL, sizeof(Buf2), &l_addr), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketSendTo(p2_socket_id, &Buf2, 0, &l_addr), OS_ERR_INVALID_SIZE);
    UtAssert_INT32_EQ(OS_SocketSendTo(p2_socket_id, &Buf2, sizeof(Buf2), NULL), OS_INVALID_POINTER);

    /* OS_SocketGetInfo */
    UtAssert_INT32_EQ(OS_SocketGetInfo(OS_OBJECT_ID_UNDEFINED, &prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketGetInfo(invalid_fd, &prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_SocketGetInfo(p2_socket_id, NULL), OS_INVALID_POINTER);

    /* OS_SocketGetIdByName */
    UtAssert_INT32_EQ(OS_SocketGetIdByName(NULL, "NF"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketGetIdByName(&objid, NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_SocketGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);
    UtAssert_INT32_EQ(OS_SocketGetIdByName(&objid, LongName), OS_ERR_NAME_TOO_LONG);

    /*
     * Send data from peer1 to peer2 and verify
     */

    /* Send data from peer1 to peer2 */
    UtAssert_INT32_EQ(OS_SocketSendTo(p1_socket_id, &Buf1, sizeof(Buf1), &p2_addr), sizeof(Buf1));

    /* Receive data from peer1 to peer2 */
    UtAssert_INT32_EQ(OS_SocketRecvFrom(p2_socket_id, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT), sizeof(Buf2));
    UtAssert_True(Buf1 == Buf2, "Buf1 (%ld) == Buf2 (%ld)", (long)Buf1, (long)Buf2);

    /* Convert addresses to string and verify data is being sent from the correct address */
    UtAssert_INT32_EQ(OS_SocketAddrToString(AddrBuffer1, sizeof(AddrBuffer1), &p1_addr), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_SocketAddrToString(AddrBuffer2, sizeof(AddrBuffer2), &l_addr), OS_SUCCESS);

    UtAssert_True(strcmp(AddrBuffer1, AddrBuffer2) == 0, "AddrBuffer1 (%s) == AddrBuffer2 (%s)", AddrBuffer1,
                  AddrBuffer2);

    /*
     * Send data from peer2 to peer1 and verify
     */

    /* Send data from peer2 to peer1 */
    UtAssert_INT32_EQ(OS_SocketSendTo(p2_socket_id, &Buf3, sizeof(Buf3), &p1_addr), sizeof(Buf3));

    /* Receive data from peer2 to peer1 */
    UtAssert_INT32_EQ(OS_SocketRecvFrom(p1_socket_id, &Buf4, sizeof(Buf4), &l_addr, UT_TIMEOUT), sizeof(Buf4));
    UtAssert_True(Buf3 == Buf4, "Buf3 (%ld) == Buf4 (%ld)", (long)Buf3, (long)Buf4);

    /* Convert addresses to string and verify data is being sent from the correct address */
    UtAssert_INT32_EQ(OS_SocketAddrToString(AddrBuffer3, sizeof(AddrBuffer3), &p2_addr), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_SocketAddrToString(AddrBuffer4, sizeof(AddrBuffer4), &l_addr), OS_SUCCESS);
    UtAssert_True(strcmp(AddrBuffer3, AddrBuffer4) == 0, "AddrBuffer3 (%s) == AddrBuffer4 (%s)", AddrBuffer3,
                  AddrBuffer4);

    /* Get port from incoming address and verify */
    UtAssert_INT32_EQ(OS_SocketAddrGetPort(&PortNum, &p2_addr), OS_SUCCESS);
    UtAssert_True(PortNum == 9998, "PortNum (%u) == 9998", (unsigned int)PortNum);

    /* Get socket info and verify */
    UtAssert_INT32_EQ(OS_SocketGetInfo(p1_socket_id, &prop), OS_SUCCESS);
    UtAssert_True(!OS_ObjectIdDefined(prop.creator), "prop.creator (%lu) == 0", OS_ObjectIdToInteger(prop.creator));
    UtAssert_True(strcmp(prop.name, "127.0.0.1:9999") == 0, "prop.name (%s) == 127.0.0.1:9999", prop.name);

    UtAssert_INT32_EQ(OS_SocketGetIdByName(&objid, "127.0.0.1:9999"), OS_SUCCESS);
    UtAssert_True(OS_ObjectIdEqual(objid, p1_socket_id), "objid (%lu) == p1_socket_id", OS_ObjectIdToInteger(objid));
}

/*****************************************************************************
 *
 * Datagram Network Teardown
 *
 *****************************************************************************/
void TestDatagramNetworkApi_Teardown(void)
{
    /* Close sockets */
    OS_close(p1_socket_id);
    OS_close(p2_socket_id);
}

/*****************************************************************************
 *
 * Server Function for Stream Network Functional Test
 *
 *****************************************************************************/
void Server_Fn(void)
{
    osal_id_t     connsock_id = OS_OBJECT_ID_UNDEFINED;
    uint32        iter;
    OS_SockAddr_t addr;
    char          Buf_trans[8]         = {0};
    uint8         Buf_each_char_s[256] = {0};
    int32         Status;
    int32         ExpectedStatus;

    /* Fill the memory with a count pattern */
    UtMemFill(Buf_each_char_s, sizeof(Buf_each_char_s));

    for (iter = UT_STREAM_CONNECTION_INITIAL; iter < UT_STREAM_CONNECTION_MAX; ++iter)
    {
        UtPrintf("SERVER: accepting connection %u", (unsigned int)iter);

        /* Accept incoming connections */
        ServerFn_AcceptStatus = OS_SocketAccept(s_socket_id, &connsock_id, &addr, OS_PEND);
        if (ServerFn_AcceptStatus != OS_SUCCESS)
        {
            break;
        }

        UtPrintf("SERVER: handling connection %u", (unsigned int)iter);

        /* Receive incoming data from client -
         * should be exactly 4 bytes on most cycles, but 0 bytes on the cycle
         * where write shutdown was done by client side prior to initial write. */
        if (iter == UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
        {
            ExpectedStatus = 0;
        }
        else
        {
            ExpectedStatus = 4;
        }
        Status = OS_TimedRead(connsock_id, Buf_trans, sizeof(Buf_trans), UT_TIMEOUT);
        if (Status != ExpectedStatus)
        {
            snprintf(ServerFn_ErrorString, sizeof(ServerFn_ErrorString), "OS_TimedRead() iter=%u, return code=%d/%d",
                     (unsigned int)iter, (int)Status, (int)ExpectedStatus);
            break;
        }

        /*
         * on iterations where the client is doing a read/readwrite shutdown, it will close the socket,
         * and the write calls may return -1 depending on what happens first.  So skip the writes.
         */
        if (iter != UT_STREAM_CONNECTION_READ_SHUTDOWN && iter != UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
        {
            /* Send back to client:
             *   1. uint32 value indicating number of connections so far (4 bytes)
             *   2. Original value received above (4 bytes)
             *   3. String of all possible 8-bit chars [0-255]  (256 bytes)
             */
            Status = OS_TimedWrite(connsock_id, &iter, sizeof(iter), UT_TIMEOUT);
            if (Status != sizeof(iter))
            {
                snprintf(ServerFn_ErrorString, sizeof(ServerFn_ErrorString),
                         "OS_TimedWrite(uint32) iter=%u, return code=%d", (unsigned int)iter, (int)Status);
                break;
            }

            Status = OS_TimedWrite(connsock_id, Buf_trans, 4, UT_TIMEOUT);
            if (Status != 4)
            {
                snprintf(ServerFn_ErrorString, sizeof(ServerFn_ErrorString),
                         "OS_TimedWrite(Buf_trans) iter=%u, return code=%d", (unsigned int)iter, (int)Status);
                break;
            }

            Status = OS_TimedWrite(connsock_id, Buf_each_char_s, sizeof(Buf_each_char_s), UT_TIMEOUT);
            if (Status != sizeof(Buf_each_char_s))
            {
                snprintf(ServerFn_ErrorString, sizeof(ServerFn_ErrorString),
                         "OS_TimedWrite(Buf_each_char_s) return code=%d", (int)Status);
                break;
            }
        }

        OS_close(connsock_id);
    }

    OS_close(s_socket_id);
}

/*****************************************************************************
 *
 * Stream Network Functional Test
 *
 *****************************************************************************/
void TestStreamNetworkApi(void)
{
    int32          status;
    int32          expected;
    int32          actual;
    uint32         iter;
    uint32         loopcnt;
    osal_id_t      temp_id;
    osal_id_t      invalid_fd;
    OS_SockAddr_t  temp_addr;
    OS_task_prop_t taskprop;
    char           Buf_rcv_c[4]           = {0};
    char           Buf_send_c[4]          = {0};
    uint8          Buf_each_char_rcv[256] = {0};

    /*
     * NOTE: The server cannot directly use UtAssert because the library is not thread-safe
     * If the server task encounters an error, it will write the string to this buffer, and it
     * will be reported at the end of this routine.
     *
     * Be sure it is empty to start with.
     */
    memset(ServerFn_ErrorString, 0, sizeof(ServerFn_ErrorString));

    /* Open a server socket */
    s_socket_id = OS_OBJECT_ID_UNDEFINED;
    expected    = OS_SUCCESS;
    actual      = OS_SocketOpen(&s_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    if (actual == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Network API not implemented");
    }
    else
    {
        UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(OS_ObjectIdDefined(s_socket_id), "s_socket_id (%lu) != 0", OS_ObjectIdToInteger(s_socket_id));

        /* Initialize server address */
        actual = OS_SocketAddrInit(&s_addr, OS_SocketDomain_INET);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

        /* Set server port */
        actual = OS_SocketAddrSetPort(&s_addr, 9997);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

        /* Set server address */
        actual = OS_SocketAddrFromString(&s_addr, "127.0.0.1");
        UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

        /* OS_SocketAccept error conditions - check before binding */
        /* create a bad ID by flipping the bits of a good ID */
        invalid_fd = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(s_socket_id) ^ 0xFFFFFFFF);
        UtAssert_INT32_EQ(OS_SocketAccept(invalid_fd, &temp_id, &temp_addr, 0), OS_ERR_INVALID_ID);
        UtAssert_INT32_EQ(OS_SocketAccept(OS_OBJECT_ID_UNDEFINED, &temp_id, &temp_addr, 0), OS_ERR_INVALID_ID);
        UtAssert_INT32_EQ(OS_SocketAccept(s_socket_id, NULL, &temp_addr, UT_TIMEOUT), OS_INVALID_POINTER);
        UtAssert_INT32_EQ(OS_SocketAccept(s_socket_id, &temp_id, NULL, UT_TIMEOUT), OS_INVALID_POINTER);
        UtAssert_INT32_EQ(OS_SocketAccept(s_socket_id, &temp_id, &temp_addr, UT_TIMEOUT), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_SocketAccept(regular_file_id, &temp_id, &temp_addr, UT_TIMEOUT),
                          OS_ERR_INCORRECT_OBJ_TYPE);

        /* Bind server socket to server address */
        actual = OS_SocketBind(s_socket_id, &s_addr);
        UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_SUCCESS", (long)actual);

        /*
         * Set up a client
         */

        /*
         * Create a server thread, and connect client from
         * this thread to server thread and verify connection
         */

        /* Create a server task/thread */
        status = OS_TaskCreate(&s_task_id, "Server", Server_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                               OSAL_PRIORITY_C(50), 0);
        UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

        /* Initialize client address */
        actual = OS_SocketAddrInit(&c_addr, OS_SocketDomain_INET);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

        /* Set client port */
        actual = OS_SocketAddrSetPort(&c_addr, 9996);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

        /* Set client address */
        actual = OS_SocketAddrFromString(&c_addr, "127.0.0.1");
        UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

        /*
         * Connect to a server - this is done in a loop
         * to confirm a server socket can be re-used for multiple clients
         */
        for (iter = UT_STREAM_CONNECTION_INITIAL; iter < UT_STREAM_CONNECTION_MAX; ++iter)
        {
            /* An extra delay here to increases the chance that the server task has reached the "accept" call */
            OS_TaskDelay(UT_TIMEOUT);
            UtPrintf("CLIENT: initiating connection %u", (unsigned int)iter);

            /* Open a client socket */
            expected    = OS_SUCCESS;
            c_socket_id = OS_OBJECT_ID_UNDEFINED;

            actual = OS_SocketOpen(&c_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
            UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
            UtAssert_True(OS_ObjectIdDefined(c_socket_id), "c_socket_id (%lu) != 0", OS_ObjectIdToInteger(c_socket_id));

            /*
             * Test for invalid input parameters - BEFORE connection
             * This is done before valid connection when the c_socket_id is created but not connected.
             * only needs to be done once, so only do this on the first pass.
             */
            if (iter == UT_STREAM_CONNECTION_INITIAL)
            {
                /* create a bad ID by flipping the bits of a good ID */
                invalid_fd = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(c_socket_id) ^ 0xFFFFFFFF);

                /* OS_SocketShutdown */
                UtAssert_INT32_EQ(OS_SocketShutdown(invalid_fd, OS_SocketShutdownMode_SHUT_READ), OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_SocketShutdown(OS_OBJECT_ID_UNDEFINED, OS_SocketShutdownMode_SHUT_READ),
                                  OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_SocketShutdown(regular_file_id, OS_SocketShutdownMode_SHUT_READ),
                                  OS_ERR_INCORRECT_OBJ_TYPE);
                UtAssert_INT32_EQ(OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_READ),
                                  OS_ERR_INCORRECT_OBJ_STATE);
            }

            actual = OS_SocketConnect(c_socket_id, &s_addr, UT_TIMEOUT);
            UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

            /*
             * Test for invalid input parameters - AFTER connection
             * This is done after valid connection when the c_socket_id is fully connected.
             * only needs to be done once, so only do this on the first pass.
             */
            if (iter == UT_STREAM_CONNECTION_INITIAL)
            {
                /* create a bad ID by flipping the bits of a good ID */
                invalid_fd = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(c_socket_id) ^ 0xFFFFFFFF);

                /* OS_SocketShutdown */
                UtAssert_INT32_EQ(OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_NONE), OS_ERR_INVALID_ARGUMENT);

                /* OS_TimedRead */
                UtAssert_INT32_EQ(OS_TimedRead(invalid_fd, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT),
                                  OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_TimedRead(OS_OBJECT_ID_UNDEFINED, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT),
                                  OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_TimedRead(c_socket_id, NULL, sizeof(Buf_rcv_c), UT_TIMEOUT), OS_INVALID_POINTER);
                UtAssert_INT32_EQ(OS_TimedRead(c_socket_id, Buf_rcv_c, 0, UT_TIMEOUT), OS_ERR_INVALID_SIZE);
                UtAssert_INT32_EQ(OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), 0), OS_ERROR_TIMEOUT);

                /* OS_TimedWrite */
                UtAssert_INT32_EQ(OS_TimedWrite(invalid_fd, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT),
                                  OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_TimedWrite(OS_OBJECT_ID_UNDEFINED, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT),
                                  OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_TimedWrite(c_socket_id, NULL, sizeof(Buf_rcv_c), UT_TIMEOUT), OS_INVALID_POINTER);
                UtAssert_INT32_EQ(OS_TimedWrite(c_socket_id, Buf_rcv_c, 0, UT_TIMEOUT), OS_ERR_INVALID_SIZE);

                /* OS_SocketConnect */
                UtAssert_INT32_EQ(OS_SocketConnect(invalid_fd, &s_addr, UT_TIMEOUT), OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_SocketConnect(OS_OBJECT_ID_UNDEFINED, &s_addr, UT_TIMEOUT), OS_ERR_INVALID_ID);
                UtAssert_INT32_EQ(OS_SocketConnect(regular_file_id, &s_addr, UT_TIMEOUT), OS_ERR_INCORRECT_OBJ_TYPE);
                UtAssert_INT32_EQ(OS_SocketConnect(c_socket_id, NULL, UT_TIMEOUT), OS_INVALID_POINTER);
                UtAssert_INT32_EQ(OS_SocketConnect(c_socket_id, &s_addr, 0), OS_ERR_INCORRECT_OBJ_STATE);
            }

            /*
             * Once connection is made between
             * server and client, transfer data
             */
            snprintf(Buf_send_c, sizeof(Buf_send_c), "%03x", (unsigned int)((iter + 0xabc) & 0xfff));

            /*
             * On designated iterations, use "shutdown" to indicate this is the end of the read data
             */
            if (iter == UT_STREAM_CONNECTION_READ_SHUTDOWN)
            {
                UtAssert_INT32_EQ(OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_READ), OS_SUCCESS);
            }

            if (iter == UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                UtAssert_INT32_EQ(OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_READWRITE), OS_SUCCESS);
            }

            if (iter == UT_STREAM_CONNECTION_READ_SHUTDOWN || iter == UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                /* Attempt to read data, would block/timeout normally, but
                 * due to read shutdown it should immediately return instead. */
                expected = 0;
                actual   = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() after read shutdown (%ld) == %ld", (long)actual,
                              (long)expected);
            }

            if (iter != UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                /* Send data to server - this should still work after read shutdown, but not after write shutdown */
                expected = sizeof(Buf_send_c);
                actual   = OS_TimedWrite(c_socket_id, Buf_send_c, sizeof(Buf_send_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld", (long)actual, (long)expected);
            }

            /* On the designated iteration, use shutdown to indicate this is the end of the written data */
            if (iter == UT_STREAM_CONNECTION_WRITE_SHUTDOWN)
            {
                UtAssert_INT32_EQ(OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_WRITE), OS_SUCCESS);
            }

            if (iter == UT_STREAM_CONNECTION_WRITE_SHUTDOWN || iter == UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                /* If write shutdown worked as expected, write should return an error */
                expected = OS_ERROR;
                actual   = OS_TimedWrite(c_socket_id, Buf_send_c, sizeof(Buf_send_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedWrite() after SHUT_WRITE (%ld) == %ld", (long)actual,
                              (long)expected);
            }

            /* On iterations where read was shutdown, skip the rest (reads after shutdown are unclear, may or may not
             * work) */
            if (iter != UT_STREAM_CONNECTION_READ_SHUTDOWN && iter != UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                /* Receive back data from server, first is loop count */
                expected = sizeof(loopcnt);
                actual   = OS_TimedRead(c_socket_id, &loopcnt, sizeof(loopcnt), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
                UtAssert_UINT32_EQ(iter, loopcnt);

                /* Receive back data from server, next is original string */
                expected = sizeof(Buf_rcv_c);
                actual   = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
                UtAssert_True(strcmp(Buf_send_c, Buf_rcv_c) == 0, "Buf_rcv_c (%s) == Buf_send_c (%s)", Buf_rcv_c,
                              Buf_send_c);

                /* Receive back data from server, next is 8-bit charset */
                expected = sizeof(Buf_each_char_rcv);
                actual   = OS_TimedRead(c_socket_id, Buf_each_char_rcv, sizeof(Buf_each_char_rcv), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
                UtAssert_MemCmpCount(Buf_each_char_rcv, sizeof(Buf_each_char_rcv), "Verify byte count pattern");

                /* Server should close the socket, reads will return 0 indicating EOF */
                expected = 0;
                actual   = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
            }

            OS_close(c_socket_id);

            /* Check that the server function did NOT Report any errors */
            UtAssert_True(ServerFn_AcceptStatus == OS_SUCCESS,
                          "OS_SocketAccept(s_socket_id, &connsock_id, &addr, OS_PEND) (%d) == OS_SUCCESS",
                          (int)ServerFn_AcceptStatus);
            if (ServerFn_ErrorString[0] != 0)
            {
                UtAssert_Failed("Server_Fn(): %s", ServerFn_ErrorString);
            }
        }

        /*
         * NOTE: Tests for invalid and other nominal input parameters
         * to some of the network functions being called here are already
         * tested in TestDatagramNetworkApi_Setup()
         */

        /* Looping delay in parent task to wait for child task to exit */
        loopcnt = 0;
        while ((OS_TaskGetInfo(s_task_id, &taskprop) == OS_SUCCESS) && (loopcnt < UT_EXIT_LOOP_MAX))
        {
            OS_TaskDelay(UT_TIMEOUT);
            loopcnt++;
        }
        UtAssert_True(loopcnt < UT_EXIT_LOOP_MAX, "Task exited after %ld iterations", (long)loopcnt);
    }
}

/*****************************************************************************
 *
 * Stream Network Teardown
 *
 *****************************************************************************/
void TestStreamNetworkApi_Teardown(void)
{
    /* Close sockets */
    OS_close(c_socket_id);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestNetworkApiBadArgs, NULL, NULL, "NetworkApiBadArgs");
    UtTest_Add(TestNetworkApiInet6, NULL, NULL, "TestNetworkApiInet6");
    UtTest_Add(TestDatagramNetworkApi, TestDatagramNetworkApi_Setup, TestDatagramNetworkApi_Teardown,
               "TestDatagramNetworkApi");
    UtTest_Add(TestStreamNetworkApi, NULL, TestStreamNetworkApi_Teardown, "TestStreamNetworkApi");
}
