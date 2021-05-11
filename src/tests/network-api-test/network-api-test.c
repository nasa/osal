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
#define UT_TIMEOUT 1000

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

char ServerFn_ErrorString[128];

/*****************************************************************************
 *
 * Datagram Network Functional Test Setup
 *
 *****************************************************************************/

void TestDatagramNetworkApi_Setup(void)
{
    int32         expected;
    int32         actual;
    osal_id_t     socket_id;
    OS_SockAddr_t addr;
    OS_SockAddr_t inv_addr;

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

        /*
         * Test for invalid and other nominal input parameters
         * to the network functions being called above
         */

        /* OS_SocketOpen */
        actual = OS_SocketOpen(&socket_id, OS_SocketDomain_INET6, OS_SocketType_DATAGRAM);
        if (actual == OS_ERR_NOT_IMPLEMENTED)
        {
            UtAssert_NA("INET6 not supported");
        }
        else
        {
            UtAssert_True(actual == OS_SUCCESS, "OS_SocketOpen() (%ld) Passed", (long)actual);
        }
        OS_close(socket_id);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketOpen(NULL, OS_SocketDomain_INVALID, OS_SocketType_INVALID);
        UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_ERR_NOT_IMPLEMENTED;
        actual   = OS_SocketOpen(&socket_id, OS_SocketDomain_MAX, OS_SocketType_MAX);
        UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_ERR_NOT_IMPLEMENTED", (long)actual);

        /* OS_SocketAddrInit */
        actual = OS_SocketAddrInit(&addr, OS_SocketDomain_INET6);
        if (actual == OS_ERR_NOT_IMPLEMENTED)
        {
            UtAssert_NA("INET6 not supported");
        }
        else
        {
            UtAssert_True(actual == OS_SUCCESS, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);
        }

        actual = OS_SocketAddrInit(NULL, OS_SocketDomain_INET6);
        if (actual == OS_ERR_NOT_IMPLEMENTED)
        {
            UtAssert_NA("INET6 not supported");
        }
        else
        {
            UtAssert_True(actual == OS_INVALID_POINTER, "OS_SocketAddrInit() (%ld) == OS_INVALID_POINTER",
                          (long)actual);
        }

        expected = OS_ERR_NOT_IMPLEMENTED;
        actual   = OS_SocketAddrInit(&addr, OS_SocketDomain_INVALID);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_ERR_NOT_IMPLEMENTED", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrInit(NULL, OS_SocketDomain_INVALID);
        UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_INVALID_POINTER", (long)actual);

        /* OS_SocketAddrSetPort */
        expected = OS_ERR_BAD_ADDRESS;
        actual   = OS_SocketAddrSetPort(&addr, 0xFFFF);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_ERR_BAD_ADDRESS", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrSetPort(NULL, 1234);
        UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

        /* OS_SocketAddrFromString */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrFromString(NULL, "127.0.0.1");
        UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

        /* OS_SocketBind */
        expected  = OS_ERR_INVALID_ID;
        socket_id = OS_ObjectIdFromInteger(1);
        actual    = OS_SocketBind(socket_id, &addr);
        UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_ERR_INVALID_ID", (long)actual);

        expected = OS_ERR_INCORRECT_OBJ_STATE;
        memset(&inv_addr, 0, sizeof(inv_addr));
        actual = OS_SocketBind(p2_socket_id, &inv_addr);
        UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    }

} /* end TestDatagramNetworkApi_Setup */

/*****************************************************************************
 *
 * Datagram Network Functional Test
 *
 *****************************************************************************/
void TestDatagramNetworkApi(void)
{
    if (networkImplemented)
    {
        char             AddrBuffer1[32];
        char             AddrBuffer2[32];
        char             AddrBuffer3[32];
        char             AddrBuffer4[32];
        uint32           Buf1 = 111;
        uint32           Buf2 = 000;
        uint32           Buf3 = 222;
        uint32           Buf4 = 000;
        osal_id_t        objid;
        uint16           PortNum;
        OS_socket_prop_t prop;
        OS_SockAddr_t    l_addr;
        int32            expected;
        int32            actual;

        /*
         * Send data from peer1 to peer2 and verify
         */

        /* Send data from peer1 to peer2 */
        expected = sizeof(Buf1);
        actual   = OS_SocketSendTo(p1_socket_id, &Buf1, sizeof(Buf1), &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketSendTo() Passed. sizeof(Buf1) (%ld) == 1", (long)actual);

        /* Recieve data from peer1 to peer2 */
        expected = sizeof(Buf2);
        actual   = OS_SocketRecvFrom(p2_socket_id, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() Passed. sizeof(Buf2) (%ld) == 1", (long)actual);
        UtAssert_True(Buf1 == Buf2, "Buf1 (%ld) == Buf2 (%ld)", (long)Buf1, (long)Buf2);

        /* Convert addresses to string and verify data is being sent from the correct address */
        expected = OS_SUCCESS;

        actual = OS_SocketAddrToString(AddrBuffer1, sizeof(AddrBuffer1), &p1_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

        actual = OS_SocketAddrToString(AddrBuffer2, sizeof(AddrBuffer2), &l_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

        UtAssert_True(strcmp(AddrBuffer1, AddrBuffer2) == 0, "AddrBuffer1 (%s) == AddrBuffer2 (%s)", AddrBuffer1,
                      AddrBuffer2);

        /*
         * Send data from peer2 to peer1 and verify
         */

        /* Send data from peer2 to peer1 */
        expected = sizeof(Buf3);
        actual   = OS_SocketSendTo(p2_socket_id, &Buf3, sizeof(Buf3), &p1_addr);
        UtAssert_True(actual == expected, "OS_SocketSendTo() Passed. sizeof(Buf1) (%ld) == 1", (long)actual);

        /* Recieve data from peer2 to peer1 */
        expected = sizeof(Buf4);
        actual   = OS_SocketRecvFrom(p1_socket_id, &Buf4, sizeof(Buf4), &l_addr, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() Passed. sizeof(Buf3) (%ld) == 1", (long)actual);
        UtAssert_True(Buf3 == Buf4, "Buf3 (%ld) == Buf4 (%ld)", (long)Buf3, (long)Buf4);

        /* Convert addresses to string and verify data is being sent from the correct address */
        expected = OS_SUCCESS;

        actual = OS_SocketAddrToString(AddrBuffer3, sizeof(AddrBuffer3), &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

        actual = OS_SocketAddrToString(AddrBuffer4, sizeof(AddrBuffer4), &l_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

        UtAssert_True(strcmp(AddrBuffer3, AddrBuffer4) == 0, "AddrBuffer3 (%s) == AddrBuffer4 (%s)", AddrBuffer3,
                      AddrBuffer4);

        /* Get port from incoming address and verify */
        actual = OS_SocketAddrGetPort(&PortNum, &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(PortNum == 9998, "PortNum (%ld) == 9998", (long)actual);

        /* Get socket info and verify */
        actual = OS_SocketGetInfo(p1_socket_id, &prop);
        UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(!OS_ObjectIdDefined(prop.creator), "prop.creator (%lu) == 0", OS_ObjectIdToInteger(prop.creator));
        UtAssert_True(strcmp(prop.name, "127.0.0.1:9999") == 0, "prop.name (%s) == 127.0.0.1:9999", prop.name);

        actual = OS_SocketGetIdByName(&objid, "127.0.0.1:9999");
        UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(OS_ObjectIdEqual(objid, p1_socket_id), "objid (%lu) == p1_socket_id",
                      OS_ObjectIdToInteger(objid));

        /*
         * Test for invalid input parameters
         * to the network functions being called above
         */

        /* OS_SocketSendTo */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketSendTo(p1_socket_id, NULL, OSAL_SIZE_C(0), NULL);
        UtAssert_True(actual == expected, "OS_SocketSendTo(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketSendTo(p1_socket_id, NULL, OSAL_SIZE_C(1), &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketSendTo() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_ERR_INVALID_ID;
        objid    = OS_ObjectIdFromInteger(0xFFFFFFFF);
        actual   = OS_SocketSendTo(objid, &Buf1, sizeof(Buf1), &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketSendTo() (%ld) == OS_ERR_INVALID_ID", (long)actual);

        /* OS_SocketRecvFrom */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketRecvFrom(p2_socket_id, NULL, OSAL_SIZE_C(1), NULL, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketRecvFrom(p2_socket_id, NULL, OSAL_SIZE_C(0), NULL, 0);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_ERR_INVALID_ID;
        objid    = OS_ObjectIdFromInteger(0xFFFFFFFF);
        actual   = OS_SocketRecvFrom(objid, &Buf2, sizeof(Buf2), &l_addr, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_ERR_INVALID_ID", (long)actual);

        expected = OS_ERR_INVALID_SIZE;
        actual   = OS_SocketRecvFrom(p2_socket_id, &Buf2, OSAL_SIZE_C(0), &l_addr, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_ERR_INVALID_SIZE", (long)actual);

        expected = OS_ERR_INVALID_SIZE;
        actual   = OS_SocketRecvFrom(p2_socket_id, &Buf2, OSAL_SIZE_C(0), NULL, UT_TIMEOUT);
        UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_ERR_INVALID_SIZE", (long)actual);

        /* OS_SocketAddrToString */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrToString(NULL, OSAL_SIZE_C(0), NULL);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrToString(AddrBuffer1, sizeof(AddrBuffer1), NULL);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrToString(NULL, OSAL_SIZE_C(0), &p2_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

        /* OS_SocketAddrGetPort */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrGetPort(NULL, NULL);
        UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrGetPort(NULL, &l_addr);
        UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_INVALID_POINTER;
        actual   = OS_SocketAddrGetPort(&PortNum, NULL);
        UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

        /* OS_SocketGetIdByName */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketGetIdByName(NULL, NULL);
        UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_ERR_NAME_NOT_FOUND;
        actual   = OS_SocketGetIdByName(&objid, "NF");
        UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);

        /* OS_SocketGetInfo */
        expected = OS_INVALID_POINTER;
        actual   = OS_SocketGetInfo(p2_socket_id, NULL);
        UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

        expected = OS_ERR_INVALID_ID;
        objid    = OS_OBJECT_ID_UNDEFINED;
        actual   = OS_SocketGetInfo(objid, &prop);
        UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_ERR_INVALID_ID", (long)actual);
    }
    else
    {
        UtAssert_NA("Network API not implemented");
    }

} /* end TestDatagramNetworkApi */

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

} /* end TestDatagramNetworkApi_Teardown */

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
        Status = OS_SocketAccept(s_socket_id, &connsock_id, &addr, OS_PEND);
        if (Status != OS_SUCCESS)
        {
            snprintf(ServerFn_ErrorString, sizeof(ServerFn_ErrorString), "OS_SocketAccept() iter=%u, return code=%d",
                     (unsigned int)iter, (int)Status);
            break;
        }

        UtPrintf("SERVER: handling connection %u", (unsigned int)iter);

        /* Recieve incoming data from client -
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
             *   2. Original value recieved above (4 bytes)
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

} /* end Server_Fn */

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

            actual = OS_SocketConnect(c_socket_id, &s_addr, UT_TIMEOUT);
            UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

            /*
             * Test for invalid input parameters -
             * This is done after valid connection when the c_socket_id is valid,
             * but it only needs to be done once, so only do this on the first pass.
             */
            if (iter == UT_STREAM_CONNECTION_INITIAL)
            {
                /* OS_TimedRead */
                expected = OS_ERR_INVALID_ID;
                temp_id  = OS_ObjectIdFromInteger(0xFFFFFFFF);
                actual   = OS_TimedRead(temp_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);

                expected = OS_INVALID_POINTER;
                actual   = OS_TimedRead(c_socket_id, NULL, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);

                expected = OS_ERROR_TIMEOUT;
                actual   = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), 0);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);

                /* OS_TimedWrite */
                expected = OS_ERR_INVALID_ID;
                temp_id  = OS_ObjectIdFromInteger(0xFFFFFFFF);
                actual   = OS_TimedWrite(temp_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld", (long)actual, (long)expected);

                expected = OS_INVALID_POINTER;
                actual   = OS_TimedWrite(c_socket_id, NULL, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld", (long)actual, (long)expected);

                /* OS_SocketAccept */
                expected = OS_INVALID_POINTER;
                actual   = OS_SocketAccept(s_socket_id, NULL, NULL, 0);
                UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);

                expected = OS_INVALID_POINTER;
                actual   = OS_SocketAccept(s_socket_id, NULL, &temp_addr, UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);

                expected = OS_INVALID_POINTER;
                actual   = OS_SocketAccept(s_socket_id, &temp_id, NULL, UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);

                /* OS_SocketConnect */
                expected = OS_INVALID_POINTER;
                actual   = OS_SocketConnect(c_socket_id, NULL, UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_INVALID_POINTER", (long)actual);

                expected = OS_ERR_INCORRECT_OBJ_STATE;
                actual   = OS_SocketConnect(c_socket_id, &s_addr, 0);
                UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                              (long)actual);

                expected = OS_ERR_INVALID_ID;
                temp_id  = OS_ObjectIdFromInteger(0xFFFFFFFF);
                actual   = OS_SocketConnect(temp_id, &s_addr, UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_ERR_INVALID_ID", (long)actual);
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
                expected = OS_SUCCESS;
                actual   = OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_READ);
                UtAssert_True(actual == expected, "OS_SocketShutdown(SHUT_READ) (%ld) == %ld", (long)actual,
                              (long)expected);
            }

            if (iter == UT_STREAM_CONNECTION_RDWR_SHUTDOWN)
            {
                expected = OS_SUCCESS;
                actual   = OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_READWRITE);
                UtAssert_True(actual == expected, "OS_SocketShutdown(SHUT_READWRITE) (%ld) == %ld", (long)actual,
                              (long)expected);
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
                expected = OS_SUCCESS;
                actual   = OS_SocketShutdown(c_socket_id, OS_SocketShutdownMode_SHUT_WRITE);
                UtAssert_True(actual == expected, "OS_SocketShutdown(SHUT_WRITE) (%ld) == %ld", (long)actual,
                              (long)expected);
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
                /* Recieve back data from server, first is loop count */
                expected = sizeof(loopcnt);
                actual   = OS_TimedRead(c_socket_id, &loopcnt, sizeof(loopcnt), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
                UtAssert_UINT32_EQ(iter, loopcnt);

                /* Recieve back data from server, next is original string */
                expected = sizeof(Buf_rcv_c);
                actual   = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), UT_TIMEOUT);
                UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
                UtAssert_True(strcmp(Buf_send_c, Buf_rcv_c) == 0, "Buf_rcv_c (%s) == Buf_send_c (%s)", Buf_rcv_c,
                              Buf_send_c);

                /* Recieve back data from server, next is 8-bit charset */
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

        /* Check that the server function did NOT Report any errors */
        if (ServerFn_ErrorString[0] != 0)
        {
            UtAssert_Failed("Server_Fn(): %s", ServerFn_ErrorString);
        }
    }

} /* end TestStreamNetworkApi */

/*****************************************************************************
 *
 * Stream Network Teardown
 *
 *****************************************************************************/
void TestStreamNetworkApi_Teardown(void)
{
    /* Close sockets */
    OS_close(c_socket_id);

} /* end TestStreamNetworkApi_Teardown */

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
    UtTest_Add(TestDatagramNetworkApi, TestDatagramNetworkApi_Setup, TestDatagramNetworkApi_Teardown,
               "TestDatagramNetworkApi");
    UtTest_Add(TestStreamNetworkApi, NULL, TestStreamNetworkApi_Teardown, "TestStreamNetworkApi");
}
