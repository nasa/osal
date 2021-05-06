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
 * \file     coveragetest-sockets.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-sockets.h"
#include "os-shared-idmap.h"
#include "os-shared-file.h"

#include "OCS_stdio.h"

void Test_OS_SocketAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_SocketAPI_Init();

    UtAssert_True(actual == expected, "OS_SocketAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_CreateSocketName()
 * This is a static helper function invoked via a wrapper
 *
 *****************************************************************************/
void Test_OS_CreateSocketName(void)
{
    /*
     * Test Case For:
     * static void OS_CreateSocketName(OS_stream_internal_record_t *sock,
     *      const OS_SockAddr_t *Addr, const char *parent_name)
     *
     * This focuses on coverage paths, as this function does not return a value
     */
    OS_SockAddr_t     testaddr;
    OS_object_token_t token;

    OS_stream_table[0].stream_name[0] = 'x';

    token.lock_mode = OS_LOCK_MODE_NONE;
    token.obj_idx   = UT_INDEX_0;
    token.obj_id    = UT_OBJID_1;
    token.obj_type  = OS_OBJECT_TYPE_OS_STREAM;

    UT_SetDefaultReturnValue(UT_KEY(OS_SocketAddrToString_Impl), OS_ERROR);
    OS_CreateSocketName(&token, &testaddr, "ut");

    /*
     * The function should have called snprintf() to create the name
     */
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_snprintf)) == 2, "OS_CreateSocketName() invoked snprintf()");
    UtAssert_True(OS_stream_table[0].stream_name[0] != 'x', "OS_CreateSocketName() set stream name");
}

/*****************************************************************************
 *
 * Test case for OS_SocketOpen()
 *
 *****************************************************************************/
void Test_OS_SocketOpen(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketOpen(uint32 *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual = OS_SocketOpen(&objid, OS_SocketDomain_INET, OS_SocketType_STREAM);

    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketOpen(NULL, OS_SocketDomain_INVALID, OS_SocketType_INVALID);
    UtAssert_True(actual == expected, "OS_SocketOpen(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketBind()
 *
 *****************************************************************************/
void Test_OS_SocketBind(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketBind(uint32 sock_id, const OS_SockAddr_t *Addr)
     */
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;
    OS_SockAddr_t Addr;

    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    memset(&Addr, 0, sizeof(Addr));
    actual = OS_SocketBind(UT_OBJID_1, &Addr);

    UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketBind(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_SocketBind(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Should fail if not a socket domain
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INVALID;
    expected                         = OS_ERR_INCORRECT_OBJ_TYPE;
    actual                           = OS_SocketBind(UT_OBJID_1, &Addr);
    UtAssert_True(actual == expected, "OS_SocketBind() non-socket (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);

    /*
     * Should fail if already bound
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[1].stream_state  = OS_STREAM_STATE_BOUND;
    expected                         = OS_ERR_INCORRECT_OBJ_STATE;
    actual                           = OS_SocketBind(UT_OBJID_1, &Addr);
    UtAssert_True(actual == expected, "OS_SocketBind() already bound (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                  (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketAccept()
 *
 *****************************************************************************/
void Test_OS_SocketAccept(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketAccept(uint32 sock_id, uint32 *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
     */
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;
    osal_index_t  local_id;
    osal_id_t     connsock_id;
    OS_SockAddr_t Addr;

    local_id    = UT_INDEX_0;
    connsock_id = OS_OBJECT_ID_UNDEFINED;

    OS_stream_table[local_id].socket_type  = OS_SocketType_STREAM;
    OS_stream_table[local_id].stream_state = OS_STREAM_STATE_BOUND;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, local_id);
    memset(&Addr, 0, sizeof(Addr));
    actual = OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0);

    UtAssert_True(actual == expected, "OS_SocketAccept() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketAccept(UT_OBJID_1, NULL, NULL, 0);
    UtAssert_True(actual == expected, "OS_SocketAccept(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Should fail if not a stream socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    expected                       = OS_ERR_INCORRECT_OBJ_TYPE;
    actual                         = OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketAccept() non-stream (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);

    /*
     * Should fail if already connected
     */
    OS_stream_table[1].socket_type  = OS_SocketType_STREAM;
    OS_stream_table[1].stream_state = OS_STREAM_STATE_BOUND | OS_STREAM_STATE_CONNECTED;
    expected                        = OS_ERR_INCORRECT_OBJ_STATE;
    actual                          = OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketAccept() already bound (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                  (long)actual);

    /*
     * Underlying implementation failure test
     */
    OS_stream_table[1].stream_state = OS_STREAM_STATE_BOUND;
    expected                        = -1234;
    UT_SetDefaultReturnValue(UT_KEY(OS_SocketAccept_Impl), -1234);
    actual = OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketAccept() underlying failure (%ld) == -1234", (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketConnect()
 *
 *****************************************************************************/
void Test_OS_SocketConnect(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketConnect(uint32 sock_id, const OS_SockAddr_t *Addr, int32 Timeout)
     */
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[idbuf].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[idbuf].stream_state  = 0;

    actual = OS_SocketConnect(UT_OBJID_1, &Addr, 0);

    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketConnect(UT_OBJID_1, NULL, 0);
    UtAssert_True(actual == expected, "OS_SocketConnect(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Should fail if not a stream socket
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INVALID;
    OS_stream_table[1].socket_type   = OS_SocketType_INVALID;
    expected                         = OS_ERR_INCORRECT_OBJ_TYPE;
    actual                           = OS_SocketConnect(UT_OBJID_1, &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketConnect() non-stream (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);

    /*
     * Should fail if already connected
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[1].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[1].stream_state  = OS_STREAM_STATE_CONNECTED;
    expected                         = OS_ERR_INCORRECT_OBJ_STATE;
    actual                           = OS_SocketConnect(UT_OBJID_1, &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketConnect() already connected (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                  (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketShutdown()
 *
 *****************************************************************************/
void Test_OS_SocketShutdown(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketShutdown(osal_id_t sock_id, OS_SocketShutdownMode_t Mode)
     */
    int32        expected = OS_SUCCESS;
    int32        actual   = ~OS_SUCCESS;
    osal_index_t idbuf;

    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[idbuf].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;

    /* nominal */
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READ);
    UtAssert_True(actual == expected, "OS_SocketShutdown() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) == 0, "Stream bits cleared");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_WRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) == 0, "Stream bits cleared");

    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) == 0, "Stream bits cleared");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) == 0, "Stream bits unchanged");

    /* Invalid Argument */
    expected = OS_ERR_INVALID_ARGUMENT;
    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_NONE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() (%ld) == OS_ERR_INVALID_ARGUMENT", (long)actual);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    /* Implementation failure */
    expected = -1234;
    UT_SetDefaultReturnValue(UT_KEY(OS_SocketShutdown_Impl), expected);
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() impl failure (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");
    UT_ResetState(UT_KEY(OS_SocketShutdown_Impl));

    /* Invalid ID */
    expected = OS_ERR_INVALID_ID;
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), expected);
    actual = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() invalid ID (%ld) == OS_ERR_INVALID_ID", (long)actual);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");
    UT_ResetState(UT_KEY(OS_ObjectIdGetById));

    /* Unconnected socket */
    expected                            = OS_ERR_INCORRECT_OBJ_STATE;
    OS_stream_table[idbuf].stream_state = 0;
    actual                              = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() unconnected (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                  (long)actual);

    /* Invalid socket type */
    expected                             = OS_ERR_INCORRECT_OBJ_TYPE;
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INVALID;
    actual                               = OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE);
    UtAssert_True(actual == expected, "OS_SocketShutdown() unconnected (%ld) == OS_ERR_INCORRECT_OBJ_TYPE",
                  (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketRecvFrom()
 *
 *****************************************************************************/
void Test_OS_SocketRecvFrom(void)
{
    /*
     * Test Case For:
     * OS_SocketRecvFrom(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
     */
    char          Buf;
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[idbuf].stream_state = OS_STREAM_STATE_BOUND;
    actual                              = OS_SocketRecvFrom(UT_OBJID_1, &Buf, 1, &Addr, 0);
    actual                              = OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0);

    UtAssert_True(actual == expected, "OS_SocketRecvFrom() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketRecvFrom(UT_OBJID_1, NULL, OSAL_SIZE_C(0), NULL, 0);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Should fail if not a datagram socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    expected                       = OS_ERR_INCORRECT_OBJ_TYPE;
    actual                         = OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() non-datagram (%ld) == OS_ERR_INCORRECT_OBJ_TYPE",
                  (long)actual);

    /*
     * Should fail if not bound
     */
    OS_stream_table[1].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[1].stream_state = 0;
    expected                        = OS_ERR_INCORRECT_OBJ_STATE;
    actual                          = OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0);
    UtAssert_True(actual == expected, "OS_SocketRecvFrom() non-bound (%ld) == OS_ERR_INCORRECT_OBJ_STATE",
                  (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketSendTo()
 *
 *****************************************************************************/
void Test_OS_SocketSendTo(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketSendTo(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr)
     */
    char          Buf      = 'A';
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[idbuf].stream_state = OS_STREAM_STATE_BOUND;
    actual                              = OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), &Addr);

    UtAssert_True(actual == expected, "OS_SocketSendTo() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketSendTo(UT_OBJID_1, NULL, sizeof(Buf), &Addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), NULL);
    UtAssert_True(actual == expected, "OS_SocketSendTo(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INVALID_SIZE;
    actual   = OS_SocketSendTo(UT_OBJID_1, &Buf, OSAL_SIZE_C(0), &Addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo(0) (%ld) == OS_ERR_INVALID_SIZE", (long)actual);

    /*
     * Should fail if not a datagram socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    expected                       = OS_ERR_INCORRECT_OBJ_TYPE;
    actual                         = OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), &Addr);
    UtAssert_True(actual == expected, "OS_SocketSendTo() non-datagram (%ld) == OS_ERR_INCORRECT_OBJ_TYPE",
                  (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketGetIdByName()
 *
 *****************************************************************************/
void Test_OS_SocketGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketGetIdByName (uint32 *sock_id, const char *sock_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_SocketGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_SocketGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_SocketGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketGetIdByName(NULL, NULL);
    UtAssert_True(actual == expected, "Test_OS_SocketGetIdByName(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);
}

/*****************************************************************************
 *
 * Test case for OS_SocketGetInfo()
 *
 *****************************************************************************/
void Test_OS_SocketGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketGetInfo (uint32 sock_id, OS_socket_prop_t *sock_prop)
     */
    int32            expected = OS_SUCCESS;
    int32            actual   = ~OS_SUCCESS;
    OS_socket_prop_t prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, "ABC", UT_OBJID_OTHER);
    actual = OS_SocketGetInfo(UT_OBJID_1, &prop);

    UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    expected = OS_INVALID_POINTER;
    actual   = OS_SocketGetInfo(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_SocketGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_SocketAddr(void)
{
    /*
     * Test Case For:
     * int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
     * int32 OS_SocketAddrToString(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr)
     * int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
     * int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
     */
    OS_SockAddr_t Addr;
    char          Buffer[32];
    uint16        PortNum;
    int32         expected = OS_SUCCESS;
    int32         actual   = ~OS_SUCCESS;

    /* First verify nominal case for each function */
    actual = OS_SocketAddrInit(&Addr, OS_SocketDomain_INVALID);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrToString(Buffer, sizeof(Buffer), &Addr);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrFromString(&Addr, Buffer);
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrSetPort(&Addr, 1234);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_SocketAddrGetPort(&PortNum, &Addr);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_SUCCESS", (long)actual);

    /* Verify invalid pointer checking in each function */
    expected = OS_INVALID_POINTER;

    actual = OS_SocketAddrInit(NULL, OS_SocketDomain_INVALID);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_INVALID_POINTER", (long)actual);

    actual = OS_SocketAddrToString(NULL, OSAL_SIZE_C(0), NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrToString() (%ld) == OS_INVALID_POINTER", (long)actual);

    actual = OS_SocketAddrFromString(NULL, NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_INVALID_POINTER", (long)actual);

    actual = OS_SocketAddrSetPort(NULL, 1234);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_INVALID_POINTER", (long)actual);

    actual = OS_SocketAddrGetPort(NULL, NULL);
    UtAssert_True(actual == expected, "OS_SocketAddrGetPort() (%ld) == OS_INVALID_POINTER", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(OS_stream_table, 0, sizeof(OS_stream_table));
    memset(OS_global_stream_table, 0, sizeof(OS_common_record_t) * OS_MAX_NUM_OPEN_FILES);
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
    ADD_TEST(OS_SocketAPI_Init);
    ADD_TEST(OS_SocketAddr);
    ADD_TEST(OS_SocketOpen);
    ADD_TEST(OS_SocketBind);
    ADD_TEST(OS_SocketAccept);
    ADD_TEST(OS_SocketConnect);
    ADD_TEST(OS_SocketRecvFrom);
    ADD_TEST(OS_SocketSendTo);
    ADD_TEST(OS_SocketShutdown);
    ADD_TEST(OS_SocketGetIdByName);
    ADD_TEST(OS_SocketGetInfo);
    ADD_TEST(OS_CreateSocketName);
}
