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

    /* Hit off-nominal branches, nominal covered by use of helper in code */
    UT_SetDefaultReturnValue(UT_KEY(OS_SocketAddrToString_Impl), OS_ERROR);
    OS_CreateSocketName(&token, &testaddr, "ut");
    UT_SetDefaultReturnValue(UT_KEY(OS_SocketAddrGetPort_Impl), OS_ERROR);
    OS_CreateSocketName(&token, &testaddr, "ut");

    /*
     * The function should have called snprintf() to create the name
     */
    UtAssert_STUB_COUNT(OCS_snprintf, 3);
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
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen(&objid, OS_SocketDomain_INET, OS_SocketType_STREAM), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen(NULL, OS_SocketDomain_INVALID, OS_SocketType_INVALID), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketOpen(&objid, OS_SocketDomain_INET, OS_SocketType_STREAM), OS_ERROR);
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
    OS_SockAddr_t Addr;

    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    memset(&Addr, 0, sizeof(Addr));

    /* Fail implementation */
    UT_SetDeferredRetcode(UT_KEY(OS_SocketBind_Impl), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, &Addr), OS_ERROR);

    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, &Addr), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    /*
     * Should fail if not a socket domain
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, &Addr), OS_ERR_INCORRECT_OBJ_TYPE);

    /*
     * Should fail if already bound
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[1].stream_state  = OS_STREAM_STATE_BOUND;
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, &Addr), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketBind(UT_OBJID_1, &Addr), OS_ERROR);
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
    osal_index_t  local_id;
    osal_id_t     connsock_id;
    OS_SockAddr_t Addr;

    local_id    = UT_INDEX_0;
    connsock_id = OS_OBJECT_ID_UNDEFINED;

    OS_stream_table[local_id].socket_type  = OS_SocketType_STREAM;
    OS_stream_table[local_id].stream_state = OS_STREAM_STATE_BOUND;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, local_id);
    memset(&Addr, 0, sizeof(Addr));

    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, NULL, &Addr, 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, NULL, 0), OS_INVALID_POINTER);

    /*
     * Should fail if not a stream socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), OS_ERR_INCORRECT_OBJ_TYPE);

    /*
     * Should fail if already connected
     */
    OS_stream_table[1].socket_type  = OS_SocketType_STREAM;
    OS_stream_table[1].stream_state = OS_STREAM_STATE_BOUND | OS_STREAM_STATE_CONNECTED;
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), OS_ERR_INCORRECT_OBJ_STATE);

    /*
     * Underlying implementation failure test
     */
    OS_stream_table[1].stream_state = OS_STREAM_STATE_BOUND;
    UT_SetDefaultReturnValue(UT_KEY(OS_SocketAccept_Impl), -1234);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), -1234);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAccept(UT_OBJID_1, &connsock_id, &Addr, 0), OS_ERR_INVALID_ID);
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
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[idbuf].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[idbuf].stream_state  = 0;

    /* Fail implementation */
    UT_SetDeferredRetcode(UT_KEY(OS_SocketConnect_Impl), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_ERROR);

    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, NULL, 0), OS_INVALID_POINTER);

    /*
     * Should fail if not a stream socket
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_ERR_INCORRECT_OBJ_TYPE);

    /*
     * Should fail if already connected
     */
    OS_stream_table[1].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[1].stream_state  = OS_STREAM_STATE_CONNECTED;
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_ERR_INCORRECT_OBJ_STATE);

    /* Only stream type will fail if connected */
    OS_stream_table[1].socket_type = OS_SocketType_DATAGRAM;
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketConnect(UT_OBJID_1, &Addr, 0), OS_ERR_INVALID_ID);
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
    osal_index_t idbuf;

    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INET;
    OS_stream_table[idbuf].socket_type   = OS_SocketType_STREAM;
    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;

    /* nominal */
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READ), OS_SUCCESS);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) == 0, "Stream bits cleared");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_WRITE), OS_SUCCESS);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) == 0, "Stream bits cleared");

    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE), OS_SUCCESS);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) == 0, "Stream bits cleared");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) == 0, "Stream bits cleared");

    /* Invalid Argument */
    OS_stream_table[idbuf].stream_state =
        OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_NONE), OS_ERR_INVALID_ARGUMENT);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    /* Implementation failure */
    UT_SetDeferredRetcode(UT_KEY(OS_SocketShutdown_Impl), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE), OS_ERROR);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    /* Invalid ID */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE), OS_ERR_INVALID_ID);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) != 0, "Stream bits unchanged");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) != 0, "Stream bits unchanged");

    /* Unconnected socket */
    OS_stream_table[idbuf].stream_state = 0;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE),
                           OS_ERR_INCORRECT_OBJ_STATE);

    /* Unconnected datagram should clear bits */
    OS_stream_table[idbuf].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[idbuf].stream_state = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE), OS_SUCCESS);
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_READABLE) == 0, "Stream bits cleared");
    UtAssert_True((OS_stream_table[idbuf].stream_state & OS_STREAM_STATE_WRITABLE) == 0, "Stream bits cleared");

    /* Invalid socket type */
    OS_stream_table[idbuf].socket_domain = OS_SocketDomain_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketShutdown(UT_OBJID_1, OS_SocketShutdownMode_SHUT_READWRITE),
                           OS_ERR_INCORRECT_OBJ_TYPE);
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
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[idbuf].stream_state = OS_STREAM_STATE_BOUND;

    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, 1, &Addr, 0), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, NULL, OSAL_SIZE_C(0), NULL, 0), OS_INVALID_POINTER);

    /*
     * Should fail if not a datagram socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0), OS_ERR_INCORRECT_OBJ_TYPE);

    /*
     * Should fail if not bound
     */
    OS_stream_table[1].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[1].stream_state = 0;
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0), OS_ERR_INCORRECT_OBJ_STATE);

    /* Fail w/OS_ERR_INVALID_SIZE */
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, OSAL_SIZE_C(0), &Addr, 0), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, OSAL_SIZE_C(UINT32_MAX), &Addr, 0), OS_ERR_INVALID_SIZE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketRecvFrom(UT_OBJID_1, &Buf, sizeof(Buf), &Addr, 0), OS_ERR_INVALID_ID);
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
    char          Buf = 'A';
    OS_SockAddr_t Addr;
    osal_index_t  idbuf;

    memset(&Addr, 0, sizeof(Addr));
    idbuf = UT_INDEX_1;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_STREAM, idbuf);
    OS_stream_table[idbuf].socket_type  = OS_SocketType_DATAGRAM;
    OS_stream_table[idbuf].stream_state = OS_STREAM_STATE_BOUND;

    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), &Addr), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, NULL, sizeof(Buf), &Addr), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), NULL), OS_INVALID_POINTER);

    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, OSAL_SIZE_C(0), &Addr), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, OSAL_SIZE_C(UINT32_MAX), &Addr), OS_ERR_INVALID_SIZE);

    /*
     * Should fail if not a datagram socket
     */
    OS_stream_table[1].socket_type = OS_SocketType_INVALID;
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), &Addr), OS_ERR_INCORRECT_OBJ_TYPE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketSendTo(UT_OBJID_1, &Buf, sizeof(Buf), &Addr), OS_ERR_INVALID_ID);
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
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdFindByName), 1, OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketGetIdByName(&objid, "UT"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_SocketGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    OSAPI_TEST_FUNCTION_RC(OS_SocketGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketGetIdByName(&objid, NULL), OS_INVALID_POINTER);
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
    OS_socket_prop_t prop;

    memset(&prop, 0, sizeof(prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OSAPI_TEST_FUNCTION_RC(OS_SocketGetInfo(UT_OBJID_1, &prop), OS_SUCCESS);
    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_SocketGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_SocketGetInfo(UT_OBJID_1, &prop), OS_ERR_INVALID_ID);
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

    /* First verify nominal case for each function */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit(&Addr, OS_SocketDomain_INVALID), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString(Buffer, sizeof(Buffer), &Addr), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString(&Addr, Buffer), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort(&Addr, 1234), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort(&PortNum, &Addr), OS_SUCCESS);

    /* Verify invalid pointer checking in each function */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrInit(NULL, OS_SocketDomain_INVALID), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString(Buffer, 1, NULL), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString(NULL, 1, &Addr), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString(&Addr, NULL), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrFromString(NULL, Buffer), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrSetPort(NULL, 1234), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort(&PortNum, NULL), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrGetPort(NULL, &Addr), OS_INVALID_POINTER);

    /* Invalid sizes */
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString(Buffer, OSAL_SIZE_C(0), &Addr), OS_ERR_INVALID_SIZE);
    OSAPI_TEST_FUNCTION_RC(OS_SocketAddrToString(Buffer, OSAL_SIZE_C(UINT32_MAX), &Addr), OS_ERR_INVALID_SIZE);
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
