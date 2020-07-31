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

#define UT_EXIT_LOOP_MAX    100

uint32 s_task_id;
uint32 p1_socket_id;
uint32 p2_socket_id;
uint32 s_socket_id;
uint32 c_socket_id;
OS_SockAddr_t p1_addr;
OS_SockAddr_t p2_addr;
OS_SockAddr_t s_addr;
OS_SockAddr_t c_addr;

/*****************************************************************************
 *
 * Datagram Network Functional Test Setup
 *
 *****************************************************************************/

void TestDatagramNetworkApi_Setup(void)
{
    int32 expected;
    int32 actual;
    uint32 socket_id;
    OS_SockAddr_t addr;
    OS_SockAddr_t inv_addr;

    /* Open a peer1 socket */
    expected = OS_SUCCESS;
    p1_socket_id = 0;

    actual = OS_SocketOpen(&p1_socket_id, OS_SocketDomain_INET, OS_SocketType_DATAGRAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(p1_socket_id != 0, "p1_socket_id (%lu) != 0", (unsigned long)p1_socket_id);

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
    expected = OS_SUCCESS;
    p2_socket_id = 0;

    actual = OS_SocketOpen(&p2_socket_id, OS_SocketDomain_INET, OS_SocketType_DATAGRAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(p2_socket_id != 0, "p2_socket_id (%lu) != 0", (unsigned long)p2_socket_id);

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
    expected = OS_SUCCESS;
    actual = OS_SocketOpen(&socket_id, OS_SocketDomain_INET6, OS_SocketType_DATAGRAM);
    UtAssert_True(actual == expected, "OS_SocketOpen(NULL) (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketOpen(NULL, OS_SocketDomain_INVALID, OS_SocketType_INVALID);
    UtAssert_True(actual == expected, "OS_SocketOpen(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_NOT_IMPLEMENTED;
    actual = OS_SocketOpen(&socket_id, OS_SocketDomain_MAX, OS_SocketType_MAX);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_ERR_NOT_IMPLEMENTED", (long)actual);

    /* OS_SocketAddrInit */
    expected = OS_SUCCESS;
    actual = OS_SocketAddrInit(&addr, OS_SocketDomain_INET6);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketAddrInit(NULL, OS_SocketDomain_INET6);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_NOT_IMPLEMENTED;
    actual = OS_SocketAddrInit(&addr, OS_SocketDomain_INVALID);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_ERR_NOT_IMPLEMENTED", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketAddrInit(NULL, OS_SocketDomain_INVALID);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* OS_SocketAddrSetPort */
    expected = OS_ERR_BAD_ADDRESS;
    actual = OS_SocketAddrSetPort(&addr, 0xFFFF);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_ERR_BAD_ADDRESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketAddrSetPort(NULL, 1234);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* OS_SocketAddrFromString */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketAddrFromString(NULL, "127.0.0.1");
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

    /* OS_SocketBind */
    expected = OS_ERR_INVALID_ID;
    actual = OS_SocketBind(1, &addr);
    UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_ERR_INCORRECT_OBJ_STATE;
    memset(&inv_addr,0,sizeof(inv_addr));
    actual = OS_SocketBind(p2_socket_id, &inv_addr);
    UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

    OS_close(socket_id);

} /* end TestDatagramNetworkApi_Setup */

/*****************************************************************************
 *
 * Datagram Network Functional Test
 *
 *****************************************************************************/
void TestDatagramNetworkApi(void)
{
    char AddrBuffer1[32];
    char AddrBuffer2[32];
    char AddrBuffer3[32];
    char AddrBuffer4[32];
    uint32 Buf1 = 111;
    uint32 Buf2 = 000;
    uint32 Buf3 = 222;
    uint32 Buf4 = 000;
    uint32 objid = 0;
    uint16 PortNum;  
    OS_socket_prop_t prop;
    OS_SockAddr_t l_addr;
    int32 expected;
    int32 actual;

    /*
     * Send data from peer1 to peer2 and verify
     */

    /* Send data from peer1 to peer2 */
    expected = sizeof(Buf1);
    actual = OS_SocketSendTo(p1_socket_id, &Buf1, sizeof(Buf1), &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo() Passed. sizeof(Buf1) (%ld) == 1", (long)actual);

    /* Recieve data from peer1 to peer2 */
    expected = sizeof(Buf2);
    actual = OS_SocketRecvFrom(p2_socket_id, &Buf2, sizeof(Buf2), &l_addr, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() Passed. sizeof(Buf2) (%ld) == 1", (long)actual);
    UtAssert_True(Buf1 == Buf2, "Buf1 (%ld) == Buf2 (%ld)", (long)Buf1, (long)Buf2);

    /* Convert addresses to string and verify data is being sent from the correct address */
    expected = OS_SUCCESS; 

    actual = OS_SocketAddrToString(AddrBuffer1, sizeof(AddrBuffer1), &p1_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrToString(AddrBuffer2, sizeof(AddrBuffer2), &l_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(strcmp(AddrBuffer1, AddrBuffer2) == 0, "AddrBuffer1 (%s) == AddrBuffer2 (%s)", AddrBuffer1, AddrBuffer2);

    /*
     * Send data from peer2 to peer1 and verify
     */

    /* Send data from peer2 to peer1 */
    expected = sizeof(Buf3);
    actual = OS_SocketSendTo(p2_socket_id, &Buf3,  sizeof(Buf3), &p1_addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo() Passed. sizeof(Buf1) (%ld) == 1", (long)actual);

    /* Recieve data from peer2 to peer1 */
    expected = sizeof(Buf4);
    actual = OS_SocketRecvFrom(p1_socket_id, &Buf4,  sizeof(Buf4), &l_addr, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() Passed. sizeof(Buf3) (%ld) == 1", (long)actual);
    UtAssert_True(Buf3 == Buf4, "Buf3 (%ld) == Buf4 (%ld)", (long)Buf3, (long)Buf4);

    /* Convert addresses to string and verify data is being sent from the correct address */
    expected = OS_SUCCESS; 

    actual = OS_SocketAddrToString(AddrBuffer3, sizeof(AddrBuffer3), &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrToString(AddrBuffer4, sizeof(AddrBuffer4), &l_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(strcmp(AddrBuffer3, AddrBuffer4) == 0, "AddrBuffer3 (%s) == AddrBuffer4 (%s)", AddrBuffer3, AddrBuffer4);

    /* Get port from incoming address and verify */
    actual = OS_SocketAddrGetPort(&PortNum, &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(PortNum == 9998, "PortNum (%ld) == 9998", (long)actual);

    /* Get socket info and verify */
    actual = OS_SocketGetInfo(p1_socket_id, &prop);
    UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(prop.creator == 0, "prop.creator (%lu) == 0",(unsigned long)prop.creator);
    UtAssert_True(strcmp(prop.name, "127.0.0.1:9999") == 0, "prop.name (%s) == 127.0.0.1:9999", prop.name);

    actual = OS_SocketGetIdByName(&objid,"127.0.0.1:9999");
    UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid == p1_socket_id, "objid (%ld) == p1_socket_id", (long)objid);

    /*
     * Test for invalid input parameters 
     * to the network functions being called above
     */

    /* OS_SocketSendTo */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketSendTo(1, NULL, 0, NULL);
    UtAssert_True(actual == expected, "OS_SocketSendTo(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketSendTo(p1_socket_id, NULL, 1, &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_SocketSendTo(0xFFFFFFF, &Buf1, 1, &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    /* OS_SocketRecvFrom */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketRecvFrom(p2_socket_id, NULL, 1, NULL, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketRecvFrom(1, NULL, 0, NULL, 0);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_SocketRecvFrom(1, &Buf2, 1, &l_addr, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketRecvFrom(p2_socket_id, &Buf2, 0, &l_addr, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketRecvFrom(p2_socket_id, &Buf2, 0, NULL, 100);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* OS_SocketAddrToString */
    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrToString(NULL, 0, NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrToString(AddrBuffer1, sizeof(AddrBuffer1), NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrToString(0, 0, &p2_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* OS_SocketAddrGetPort */
    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrGetPort(NULL, NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrGetPort(0, &l_addr);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER; 
    actual = OS_SocketAddrGetPort(&PortNum, NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* OS_SocketGetIdByName */
    expected = OS_INVALID_POINTER; 
    actual = OS_SocketGetIdByName(NULL,NULL);
    UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_NAME_NOT_FOUND; 
    actual = OS_SocketGetIdByName(&objid,"NF");
    UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);

    /* OS_SocketGetInfo */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketGetInfo(1, NULL);
    UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_SocketGetInfo(0, &prop);
    UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_ERR_INVALID_ID", (long)actual);

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
    OS_SockAddr_t addr;
    char Buf_send_s[4] = {0};
    char Buf_rcv_s[4] = {0};
    char Buf_trans[8] ={0};
    uint32 connsock_id = 0;

    /* Accept incoming connections */
    OS_SocketAccept(s_socket_id, &connsock_id, &addr, OS_PEND);

    /* Once connected, send data to client */
    strcpy(Buf_send_s, "abc");
    OS_TimedWrite(connsock_id, Buf_send_s, sizeof(Buf_send_s), 10);

   /* Recieve incoming data from client*/
    OS_TimedRead(connsock_id, Buf_rcv_s, sizeof(Buf_rcv_s), 10);

   /* Transform the incoming data and send it back to client */
    strcpy(Buf_trans, "uvw");
    strcat(Buf_trans, Buf_rcv_s);
    OS_TimedWrite(connsock_id, Buf_trans, sizeof(Buf_trans), 10);


    OS_close(s_socket_id);

} /* end Server_Fn */


/*****************************************************************************
 *
 * Stream Network Functional Test 
 *
 *****************************************************************************/
void TestStreamNetworkApi(void)
{
    int32 status; 
    int32 expected;
    int32 actual;
    uint32 loopcnt;
    uint32 temp_id;
    OS_SockAddr_t temp_addr;
    OS_task_prop_t taskprop;
    char Buf_rcv_c[4] = {0};
    char Buf_send_c[4] = {0};
    char Buf_expected[4] ={0};
    char Buf_rcv_trans[8] = {0};
    char Buf_expec_trans[8] ={0};


    /*
     * Set up a server
     */

    /* Open a server socket */
    s_socket_id = 0;
    expected = OS_SUCCESS;
    actual = OS_SocketOpen(&s_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(s_socket_id != 0, "s_socket_id (%lu) != 0", (unsigned long)s_socket_id);

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

   /* Open a client socket */
    expected = OS_SUCCESS;
    c_socket_id = 0;

    actual = OS_SocketOpen(&c_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(c_socket_id != 0, "c_socket_id (%lu) != 0", (unsigned long)c_socket_id);

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
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    /* Create a server task/thread */
    status = OS_TaskCreate( &s_task_id, "Server", Server_Fn, 0, 16384, 50, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c_socket_id, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

   /* Once connected, recieve incoming data from server*/
    expected = sizeof(Buf_rcv_c);
    strcpy(Buf_expected, "abc");

    actual = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",(long)actual, (long)expected);
    UtAssert_True(strcmp(Buf_rcv_c,Buf_expected) == 0, "Buf (%s) == Buf_expected (%s)", Buf_rcv_c, Buf_expected);

   /* Send data to server to be transformed and sent back */
    strcpy(Buf_send_c, "xyz");
    expected = sizeof(Buf_send_c);
    actual = OS_TimedWrite(c_socket_id, Buf_send_c, sizeof(Buf_send_c), 10);
    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld",(long)actual, (long)expected);

   /* Recieve back transformed data from server*/
    expected = sizeof(Buf_expec_trans);
    strcpy(Buf_expec_trans, "uvwxyz");

    actual = OS_TimedRead(c_socket_id, Buf_rcv_trans, sizeof(Buf_rcv_trans), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",(long)actual, (long)expected);
    UtAssert_True(strcmp(Buf_rcv_trans, Buf_expec_trans) == 0, "Buf_rcv_trans (%s) == Buf_expected (%s)", Buf_rcv_trans, Buf_expec_trans);

    /*
     * Test for invalid input parameters 
     * to the network functions being called above
     */

    /* OS_SocketConnect */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketConnect(c_socket_id, NULL, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_SocketConnect(c_socket_id, &s_addr, 0);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_SocketConnect(1, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    /* OS_TimedRead */
    expected = OS_ERR_INVALID_ID;
    actual = OS_TimedRead(1, Buf_rcv_c, sizeof(Buf_rcv_c), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",(long)actual, (long)expected);

    expected = OS_INVALID_POINTER;
    actual = OS_TimedRead(c_socket_id, NULL, sizeof(Buf_rcv_c), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",(long)actual, (long)expected);

    expected = OS_ERROR_TIMEOUT;
    actual = OS_TimedRead(c_socket_id, Buf_rcv_c, sizeof(Buf_rcv_c), 0);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",(long)actual, (long)expected);

    /* OS_TimedWrite */
    expected = OS_ERR_INVALID_ID;
    actual = OS_TimedWrite(1, Buf_rcv_c, sizeof(Buf_rcv_c), 10);
    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld",(long)actual, (long)expected);

    expected = OS_INVALID_POINTER;
    actual = OS_TimedWrite(c_socket_id, NULL, sizeof(Buf_rcv_c), 10);
    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld",(long)actual, (long)expected);

    /* OS_SocketAccept */
    expected = OS_INVALID_POINTER;
    actual = OS_SocketAccept(1, NULL, NULL, 0);
    UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketAccept(s_socket_id, NULL, &temp_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_SocketAccept(s_socket_id, &temp_id, NULL, 10);
    UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_INVALID_POINTER", (long)actual);


    /*
     * NOTE: Tests for invalid and other nominal input parameters 
     * to some of the network functions being called here are already 
     * tested in TestDatagramNetworkApi_Setup()
     */


    /* Looping delay in parent task to wait for child task to exit */
    loopcnt = 0;
    while ((OS_TaskGetInfo(s_task_id, &taskprop) == OS_SUCCESS) && (loopcnt < UT_EXIT_LOOP_MAX))
    {
       OS_TaskDelay(10);
       loopcnt++;
    }
    UtAssert_True(loopcnt < UT_EXIT_LOOP_MAX, "Task exited after %ld iterations", (long)loopcnt);

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

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestDatagramNetworkApi, TestDatagramNetworkApi_Setup, TestDatagramNetworkApi_Teardown, "TestDatagramNetworkApi");
    UtTest_Add(TestStreamNetworkApi, NULL, TestStreamNetworkApi_Teardown, "TestStreamNetworkApi");
}

