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
 * Filename: select-test.c
 *
 * Purpose: This file contains functional tests for "osapi-select"
 * Single select test will create a server and client to stream data between them and the select watches that stream.
 * Multi select test will setup a second server and client also streaming data between them so that it can watch
 * multiple streams.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"
#define MAX_BUFFER_LOOP 1000000

osal_id_t     s_task_id;
osal_id_t     s2_task_id;
osal_id_t     s_socket_id;
osal_id_t     s2_socket_id;
osal_id_t     c_socket_id;
osal_id_t     c2_socket_id;
OS_SockAddr_t s_addr;
OS_SockAddr_t s2_addr;
OS_SockAddr_t c_addr;
OS_SockAddr_t c2_addr;
osal_id_t     bin_sem_id;
osal_id_t     bin_sem_id2;

/* *************************************** MAIN ************************************** */

char *           fsAddrPtr = NULL;
static osal_id_t setup_file(void)
{
    osal_id_t id;
    OS_mkfs(fsAddrPtr, "/ramdev0", "RAM", 512, 20);
    OS_mount("/ramdev0", "/drive0");
    OS_OpenCreate(&id, "/drive0/select_test.txt", OS_FILE_FLAG_CREATE, OS_READ_WRITE);
    return id;
}

void BinSemSetup(void)
{
    uint32            status;
    OS_bin_sem_prop_t bin_sem_prop;
    OS_bin_sem_prop_t bin_sem_prop2;

    /*
     * Create the binary semaphore
     * BinSem1 is used to control when the server can accept connections
     * BinSem2 is used to make sure all sub task finish before the main task does.
     */
    status = OS_BinSemCreate(&bin_sem_id, "BinSem1", 0, 0);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 create Id=%lx Rc=%d", OS_ObjectIdToInteger(bin_sem_id), (int)status);

    status = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 value=%d Rc=%d", (int)bin_sem_prop.value, (int)status);

    status = OS_BinSemCreate(&bin_sem_id2, "BinSem2", 0, 0);
    UtAssert_True(status == OS_SUCCESS, "BinSem2 create Id=%lx Rc=%d", OS_ObjectIdToInteger(bin_sem_id2), (int)status);

    status = OS_BinSemGetInfo(bin_sem_id2, &bin_sem_prop2);
    UtAssert_True(status == OS_SUCCESS, "BinSem2 value=%d Rc=%d", (int)bin_sem_prop2.value, (int)status);
}

void Setup_Server(void)
{
    int32 expected;
    int32 actual;

    /*
     * Set up a server
     */

    /* Open a server socket */
    s_socket_id = OS_OBJECT_ID_UNDEFINED;
    expected    = OS_SUCCESS;
    actual      = OS_SocketOpen(&s_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
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
}

void Setup_Client(void)
{
    int32 expected;
    int32 actual;

    /*
     * Set up a client
     */

    /* Open a client socket */
    expected    = OS_SUCCESS;
    c_socket_id = OS_OBJECT_ID_UNDEFINED;

    actual = OS_SocketOpen(&c_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_ObjectIdDefined(c_socket_id), "c_socket_id (%lu) != 0", OS_ObjectIdToInteger(c_socket_id));

    /* Initialize client address */
    actual = OS_SocketAddrInit(&c_addr, OS_SocketDomain_INET);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

    /* Set client port */
    actual = OS_SocketAddrSetPort(&c_addr, 9996);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

    /* Set client address */
    actual = OS_SocketAddrFromString(&c_addr, "127.0.0.1");
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);
}

void Server_Fn(void)
{
    osal_id_t     connsock_id = OS_OBJECT_ID_UNDEFINED;
    OS_SockAddr_t addr;
    uint32        status;

    /* Accept incoming connections */
    OS_SocketAccept(s_socket_id, &connsock_id, &addr, OS_PEND);

    status = OS_BinSemTake(bin_sem_id);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 Server 1 take Rc=%d", (int)status);

    status = OS_close(s_socket_id);
    UtAssert_True(status == OS_SUCCESS, "status after close s_socket_id = %d", (int)status);

    status = OS_close(connsock_id);
    UtAssert_True(status == OS_SUCCESS, "status after close connsock_id = %d", (int)status);

    status = OS_BinSemGive(bin_sem_id2);
    UtAssert_True(status == OS_SUCCESS, "BinSem2 Server 1 give Rc=%d", (int)status);
} /* end Server_Fn */

void Setup_Server2(void)
{
    int32 expected;
    int32 actual;

    /*
     * Set up a server
     */

    /* Open a server socket */
    s2_socket_id = OS_OBJECT_ID_UNDEFINED;
    expected     = OS_SUCCESS;
    actual       = OS_SocketOpen(&s2_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_ObjectIdDefined(s2_socket_id), "s2_socket_id (%lu) != 0", OS_ObjectIdToInteger(s2_socket_id));

    /* Initialize server address */
    actual = OS_SocketAddrInit(&s2_addr, OS_SocketDomain_INET);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

    /* Set server port */
    actual = OS_SocketAddrSetPort(&s2_addr, 9998);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

    /* Set server address */
    actual = OS_SocketAddrFromString(&s2_addr, "127.0.0.1");
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);

    /* Bind server socket to server address */
    actual = OS_SocketBind(s2_socket_id, &s2_addr);
    UtAssert_True(actual == expected, "OS_SocketBind() (%ld) == OS_SUCCESS", (long)actual);
}

void Setup_Client2(void)
{
    int32 expected;
    int32 actual;

    /*
     * Set up a client
     */

    /* Open a client socket */
    expected     = OS_SUCCESS;
    c2_socket_id = OS_OBJECT_ID_UNDEFINED;

    actual = OS_SocketOpen(&c2_socket_id, OS_SocketDomain_INET, OS_SocketType_STREAM);
    UtAssert_True(actual == expected, "OS_SocketOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_ObjectIdDefined(c2_socket_id), "c2_socket_id (%lu) != 0", OS_ObjectIdToInteger(c2_socket_id));

    /* Initialize client address */
    actual = OS_SocketAddrInit(&c2_addr, OS_SocketDomain_INET);
    UtAssert_True(actual == expected, "OS_SocketAddrInit() (%ld) == OS_SUCCESS", (long)actual);

    /* Set client port */
    actual = OS_SocketAddrSetPort(&c2_addr, 9995);
    UtAssert_True(actual == expected, "OS_SocketAddrSetPort() (%ld) == OS_SUCCESS", (long)actual);

    /* Set client address */
    actual = OS_SocketAddrFromString(&c2_addr, "127.0.0.1");
    UtAssert_True(actual == expected, "OS_SocketAddrFromString() (%ld) == OS_SUCCESS", (long)actual);
}

void Server_Fn2(void)
{
    osal_id_t     connsock_id = OS_OBJECT_ID_UNDEFINED;
    OS_SockAddr_t addr;
    uint32        status;

    /* Accept incoming connections */
    OS_SocketAccept(s2_socket_id, &connsock_id, &addr, OS_PEND);

    status = OS_close(s2_socket_id);
    UtAssert_True(status == OS_SUCCESS, "status after close s2_socket_id = %d", (int)status);

    status = OS_close(connsock_id);
    UtAssert_True(status == OS_SUCCESS, "status after close connsock_id = %d", (int)status);
} /* end Server_Fn */

void Setup_Single(void)
{
    Setup_Server();
    Setup_Client();
    BinSemSetup();
}

void Setup_Multi(void)
{
    Setup_Single();
    Setup_Server2();
    Setup_Client2();
}

void Teardown_Single(void)
{
    uint32 status;

    OS_close(c_socket_id);
    status = OS_BinSemTake(bin_sem_id2);
    UtAssert_True(status == OS_SUCCESS, "BinSem2 Teardown single take Rc=%d", (int)status);

    OS_BinSemDelete(bin_sem_id);
    OS_BinSemDelete(bin_sem_id2);
}

void Teardown_Multi(void)
{
    uint32 status;

    status = OS_BinSemFlush(bin_sem_id);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 Teardown multi flush Rc=%d", (int)status);

    OS_close(c2_socket_id);
    Teardown_Single();
}

void TestSelectSingleRead(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    int32 expected = OS_SUCCESS;
    int32 actual;

    /*
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    /* Create a server task/thread */
    int32 status = OS_TaskCreate(&s_task_id, "Server", Server_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                                 OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c_socket_id, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    uint32 StateFlags;
    expected   = OS_ERROR_TIMEOUT;
    StateFlags = OS_STREAM_STATE_READABLE;
    actual     = OS_SelectSingle(c_socket_id, &StateFlags, 100);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_ERROR_TIMEOUT", (long)actual);
    UtAssert_True(StateFlags == 0, "OS_SelectSingle() (%d) == None", StateFlags);

    status = OS_BinSemGive(bin_sem_id);

    expected   = OS_SUCCESS;
    StateFlags = OS_STREAM_STATE_READABLE;
    actual     = OS_SelectSingle(c_socket_id, &StateFlags, 100);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(StateFlags == OS_STREAM_STATE_READABLE, "OS_SelectSingle() (%d) == OS_STREAM_STATE_READABLE",
                  StateFlags);
}

void TestSelectMultipleRead(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;
    int32    expected = OS_SUCCESS;
    int32    actual;
    int32    status;

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdZero(&WriteSet);

    /*
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    /* Create a server task/thread */
    status = OS_TaskCreate(&s_task_id, "Server", Server_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                           OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c_socket_id, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    status = OS_TaskCreate(&s2_task_id, "Server2", Server_Fn2, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                           OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c2_socket_id, &s2_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    OS_SelectFdAdd(&ReadSet, c_socket_id);
    OS_SelectFdAdd(&ReadSet, c2_socket_id);

    UtAssert_True(OS_SelectFdIsSet(&ReadSet, c_socket_id), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(OS_SelectFdIsSet(&ReadSet, c2_socket_id), "OS_SelectFdIsSet(1) == true");

    actual = OS_SelectMultiple(&ReadSet, &WriteSet, 100);
    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectMultiple() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(!OS_SelectFdIsSet(&ReadSet, c_socket_id), "OS_SelectFdIsSet(1) == false");
    UtAssert_True(OS_SelectFdIsSet(&ReadSet, c2_socket_id), "OS_SelectFdIsSet(2) == true");
}

void TestSelectSingleWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */

    int32  actual;
    uint32 StateFlags;
    int32  expected          = OS_SUCCESS;
    int    count             = 0;
    char   Buf_send_c[16834] = {0};

    /*
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    /* Create a server task/thread */
    int32 status = OS_TaskCreate(&s_task_id, "Server", Server_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                                 OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c_socket_id, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    while (actual != OS_ERROR_TIMEOUT && count < MAX_BUFFER_LOOP)
    {
        strcpy(Buf_send_c, "16 KB buffer filler");
        actual = OS_TimedWrite(c_socket_id, Buf_send_c, sizeof(Buf_send_c), 10);

        StateFlags = OS_STREAM_STATE_WRITABLE;
        actual     = OS_SelectSingle(c_socket_id, &StateFlags, 100);

        count++;
    }

    status = OS_BinSemGive(bin_sem_id);

    if (count >= MAX_BUFFER_LOOP)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_MIR, __FILE__, __LINE__, "%s",
                   "Unable to cause OS_STREAM_STATE_WRITEABLE timeout with large looped writes, skipping verification");
    }
    else
    {
        expected = OS_ERROR_TIMEOUT;
        /* Verify Outputs */
        UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_ERROR_TIMEOUT", (long)actual);
        UtAssert_True(StateFlags == 0, "OS_SelectSingle() (%d) == None", StateFlags);

        expected   = OS_SUCCESS;
        StateFlags = OS_STREAM_STATE_WRITABLE;
        actual     = OS_SelectSingle(c_socket_id, &StateFlags, 100);

        /* Verify Outputs */
        UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
        UtAssert_True(StateFlags == OS_STREAM_STATE_WRITABLE, "OS_SelectSingle() (%d) == OS_STREAM_STATE_WRITABLE",
                      StateFlags);
    }
}

void TestSelectMultipleWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;
    int32    expected = OS_SUCCESS;
    int32    actual;
    int32    status;
    uint32   StateFlags;
    int      count             = 0;
    char     Buf_send_c[16834] = {0};

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdZero(&WriteSet);

    /*
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    /* Create a server task/thread */
    status = OS_TaskCreate(&s_task_id, "Server", Server_Fn, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                           OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c_socket_id, &s_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    status = OS_TaskCreate(&s2_task_id, "Server2", Server_Fn2, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                           OSAL_PRIORITY_C(50), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Connect to a server */
    actual = OS_SocketConnect(c2_socket_id, &s2_addr, 10);
    UtAssert_True(actual == expected, "OS_SocketConnect() (%ld) == OS_SUCCESS", (long)actual);

    OS_SelectFdAdd(&WriteSet, c_socket_id);
    OS_SelectFdAdd(&WriteSet, c2_socket_id);

    UtAssert_True(OS_SelectFdIsSet(&WriteSet, c_socket_id), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(OS_SelectFdIsSet(&WriteSet, c2_socket_id), "OS_SelectFdIsSet(1) == true");

    while (actual != OS_ERROR_TIMEOUT && count < MAX_BUFFER_LOOP)
    {
        strcpy(Buf_send_c, "16 KB buffer filler");
        actual = OS_TimedWrite(c_socket_id, Buf_send_c, sizeof(Buf_send_c), 10);

        StateFlags = OS_STREAM_STATE_WRITABLE;
        actual     = OS_SelectSingle(c_socket_id, &StateFlags, 100);

        count++;
    }

    if (count >= MAX_BUFFER_LOOP)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_MIR, __FILE__, __LINE__, "%s",
                   "Unable to cause OS_STREAM_STATE_WRITEABLE timeout with large looped writes, skipping verification");
    }
    else
    {
        actual = OS_SelectMultiple(&ReadSet, &WriteSet, 100);
        /* Verify Outputs */
        UtAssert_True(actual == expected, "OS_SelectMultiple() (%ld) == OS_SUCCESS", (long)actual);

        UtAssert_True(!OS_SelectFdIsSet(&WriteSet, c_socket_id), "OS_SelectFdIsSet(1) == false");
        UtAssert_True(OS_SelectFdIsSet(&WriteSet, c2_socket_id), "OS_SelectFdIsSet(2) == true");
    }
}

void TestSelectSingleFile(void)
{
    int32     expected = OS_SUCCESS;
    int32     actual;
    uint32    StateFlags;
    osal_id_t fd = setup_file();

    expected   = OS_SUCCESS;
    StateFlags = OS_STREAM_STATE_READABLE;
    actual     = OS_SelectSingle(fd, &StateFlags, 100);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(StateFlags == OS_STREAM_STATE_READABLE, "OS_SelectSingle() (%d) == OS_STREAM_STATE_READABLE",
                  StateFlags);

    StateFlags = OS_STREAM_STATE_WRITABLE;
    actual     = OS_SelectSingle(fd, &StateFlags, 100);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(StateFlags == OS_STREAM_STATE_WRITABLE, "OS_SelectSingle() (%d) == OS_STREAM_STATE_WRITABLE",
                  StateFlags);

    expected   = OS_ERROR_TIMEOUT;
    StateFlags = OS_STREAM_STATE_BOUND;
    actual     = OS_SelectSingle(fd, &StateFlags, 100);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_ERROR_TIMEOUT", (long)actual);
    UtAssert_True(StateFlags == 0, "OS_SelectSingle() (%d) == None", StateFlags);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */

    UtTest_Add(TestSelectSingleRead, Setup_Single, Teardown_Single, "TestSelectSingleRead");
    UtTest_Add(TestSelectMultipleRead, Setup_Multi, Teardown_Multi, "TestSelectMultipleRead");
    UtTest_Add(TestSelectSingleWrite, Setup_Single, Teardown_Single, "TestSelectSingleWrite");
    UtTest_Add(TestSelectMultipleWrite, Setup_Multi, Teardown_Multi, "TestSelectMultipleWrite");
    UtTest_Add(TestSelectSingleFile, NULL, NULL, "TestSelectSingleFile");
}