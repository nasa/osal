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

/*
 * Timeouts for various socket ops in the test cases
 *
 * Note that the act of calling any "assert" routine causes console output, which
 * can easily take tens or even hundreds of milliseconds to execute on platforms
 * where the console is on a slow serial port.  Therefore this timeout must
 * not be too short.
 */
#define UT_TIMEOUT 500

osal_id_t s1_task_id;
osal_id_t s2_task_id;
osal_id_t s1_socket_id;
osal_id_t s2_socket_id;
osal_id_t c1_socket_id;
osal_id_t c2_socket_id;
osal_id_t bin_sem_id;
bool      networkImplemented = true;

char filldata[16834];

/* *************************************** MAIN ************************************** */

void BinSemSetup(void)
{
    OS_bin_sem_prop_t bin_sem_prop;

    memset(&bin_sem_prop, 0, sizeof(bin_sem_prop));

    /*
     * Create the binary semaphore
     * BinSem1 is used to control when the server can accept connections
     */
    UtAssert_INT32_EQ(OS_BinSemCreate(&bin_sem_id, "BinSem1", 0, 0), OS_SUCCESS);
    UtAssert_True(OS_ObjectIdDefined(bin_sem_id), "bin_sem_id (%lu) != UNDEFINED", OS_ObjectIdToInteger(bin_sem_id));

    UtAssert_INT32_EQ(OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop), OS_SUCCESS);
    UtPrintf("BinSem1 value=%d", (int)bin_sem_prop.value);
}

/*
 * Helper function to open a server socket, bind server to a port,
 * creates a server task, and create a client socket.
 * Then finally connects the client to the server and leave sockets open.
 */
void Setup_SocketPair(osal_id_t *server_sockid_ptr, osal_id_t *client_sockid_ptr, uint16 portnum, osal_id_t *taskid_ptr,
                      const char *SrvName, void (*SrvFunc)(void))
{
    OS_SockAddr_t addr;
    int           sock_status;

    /* Open a server socket, if OS_ERR_NOT_IMPLEMENTED then give up now */
    sock_status = OS_SocketOpen(server_sockid_ptr, OS_SocketDomain_INET, OS_SocketType_STREAM);
    if (sock_status == OS_ERR_NOT_IMPLEMENTED)
    {
        networkImplemented = false;
        return;
    }

    UtAssert_True(sock_status == OS_SUCCESS,
                  "OS_SocketOpen(s1_socketid_ptr, OS_SocketDomain_INET, OS_SocketType_STREAM) (%d) == OS_SUCCESS",
                  (int)sock_status);

    /* Open a client socket */
    UtAssert_INT32_EQ(OS_SocketOpen(client_sockid_ptr, OS_SocketDomain_INET, OS_SocketType_STREAM), OS_SUCCESS);

    /* Initialize server address */
    UtAssert_INT32_EQ(OS_SocketAddrInit(&addr, OS_SocketDomain_INET), OS_SUCCESS);

    /* Set server port */
    UtAssert_INT32_EQ(OS_SocketAddrSetPort(&addr, portnum), OS_SUCCESS);

    /* Set server address */
    UtAssert_INT32_EQ(OS_SocketAddrFromString(&addr, "127.0.0.1"), OS_SUCCESS);

    /* Bind server socket to server address */
    UtAssert_INT32_EQ(OS_SocketBind(*server_sockid_ptr, &addr), OS_SUCCESS);

    /* Print the sockets for informational purposes - should both be valid/defined */
    UtAssert_True(OS_ObjectIdDefined(*server_sockid_ptr), "s1_socket_id (%lu) != UNDEFINED (port %u)",
                  OS_ObjectIdToInteger(*server_sockid_ptr), (unsigned int)portnum);
    UtAssert_True(OS_ObjectIdDefined(*client_sockid_ptr), "c1_socket_id (%lu) != UNDEFINED",
                  OS_ObjectIdToInteger(*client_sockid_ptr));

    /*
     * Create a server thread, and connect client from
     * this thread to server thread
     */

    /* Create a server task/thread */
    UtAssert_INT32_EQ(OS_TaskCreate(taskid_ptr, SrvName, SrvFunc, OSAL_TASK_STACK_ALLOCATE, OSAL_SIZE_C(16384),
                                    OSAL_PRIORITY_C(50), 0),
                      OS_SUCCESS);

    /* Connect to a server */
    UtAssert_INT32_EQ(OS_SocketConnect(*client_sockid_ptr, &addr, UT_TIMEOUT), OS_SUCCESS);
}

void Delete_SocketPair(osal_id_t server_sockid, osal_id_t client_sockid, osal_id_t task_id)
{
    OS_task_prop_t task_prop;

    while (OS_TaskGetInfo(task_id, &task_prop) == OS_SUCCESS)
    {
        OS_TaskDelay(10);
    }

    UtAssert_INT32_EQ(OS_close(server_sockid), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_close(client_sockid), OS_SUCCESS);
}

bool FillOutputBuffer(osal_id_t conn_id)
{
    uint32 count;

    UtMemFill(filldata, sizeof(filldata));

    for (count = 0; count < MAX_BUFFER_LOOP; ++count)
    {
        if (OS_TimedWrite(conn_id, filldata, sizeof(filldata), UT_TIMEOUT) == OS_ERROR_TIMEOUT)
        {
            break;
        }
    }

    return (count < MAX_BUFFER_LOOP);
}

void Server_Fn(void)
{
    osal_id_t     connsock_id = OS_OBJECT_ID_UNDEFINED;
    OS_SockAddr_t addr;

    /* Accept incoming connections */
    UtAssert_INT32_EQ(OS_SocketAccept(s1_socket_id, &connsock_id, &addr, OS_PEND), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_BinSemTake(bin_sem_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_close(connsock_id), OS_SUCCESS);
}

void Server_Fn2(void)
{
    osal_id_t     connsock_id = OS_OBJECT_ID_UNDEFINED;
    OS_SockAddr_t addr;

    /* Accept incoming connections */
    UtAssert_INT32_EQ(OS_SocketAccept(s2_socket_id, &connsock_id, &addr, OS_PEND), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_close(connsock_id), OS_SUCCESS);
}

void Setup_Single(void)
{
    BinSemSetup();
    Setup_SocketPair(&s1_socket_id, &c1_socket_id, 9994, &s1_task_id, "Server1", Server_Fn);
}

void Setup_Multi(void)
{
    Setup_Single();
    if (networkImplemented)
    {
        Setup_SocketPair(&s2_socket_id, &c2_socket_id, 9995, &s2_task_id, "Server2", Server_Fn2);
    }
}

void Teardown_Single(void)
{
    if (networkImplemented)
    {
        Delete_SocketPair(s1_socket_id, c1_socket_id, s1_task_id);
    }

    UtAssert_INT32_EQ(OS_BinSemDelete(bin_sem_id), OS_SUCCESS);
}

void Teardown_Multi(void)
{
    if (networkImplemented)
    {
        Delete_SocketPair(s2_socket_id, c2_socket_id, s2_task_id);
    }

    Teardown_Single();
}

void TestSelectSingleRead(void)
{
    uint32 StateFlags;

    if (!networkImplemented)
    {
        UtAssert_NA("Network API not implemented");
        return;
    }

    StateFlags = OS_STREAM_STATE_READABLE;
    UtAssert_INT32_EQ(OS_SelectSingle(c1_socket_id, &StateFlags, UT_TIMEOUT), OS_ERROR_TIMEOUT);

    /* Verify Outputs */
    UtAssert_True(StateFlags == 0, "StateFlags after OS_SelectSingle (0x%x) == None", (unsigned int)StateFlags);

    UtAssert_INT32_EQ(OS_BinSemGive(bin_sem_id), OS_SUCCESS);
    OS_TaskDelay(10); /* Give server time to run and close the socket */

    StateFlags = OS_STREAM_STATE_READABLE;
    UtAssert_INT32_EQ(OS_SelectSingle(c1_socket_id, &StateFlags, UT_TIMEOUT), OS_SUCCESS);

    /* Verify Outputs */
    UtAssert_True(StateFlags == OS_STREAM_STATE_READABLE,
                  "StateFlags after OS_SelectSingle() (0x%x) == OS_STREAM_STATE_READABLE", (unsigned int)StateFlags);
}

void TestSelectMultipleRead(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);
     */
    OS_FdSet ReadSet;

    if (!networkImplemented)
    {
        UtAssert_NA("Network API not implemented");
        return;
    }

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, c1_socket_id);

    /*
     * Check for readability on socket 1 should time out, as server1 is waiting on Sem
     */
    UtAssert_INT32_EQ(OS_SelectMultiple(&ReadSet, NULL, UT_TIMEOUT), OS_ERROR_TIMEOUT);

    /*
     * NOTE: NOT checking sets, because after OS_SelectMultiple fails, sets are not defined.
     * (because it timed out, by definition it means all sets are considered empty)
     */

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, c1_socket_id);
    OS_SelectFdAdd(&ReadSet, c2_socket_id);

    UtAssert_INT32_EQ(OS_SelectMultiple(&ReadSet, NULL, UT_TIMEOUT), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&ReadSet, c1_socket_id), false);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&ReadSet, c2_socket_id), true);

    UtAssert_INT32_EQ(OS_BinSemGive(bin_sem_id), OS_SUCCESS);
    OS_TaskDelay(10); /* Give server time to run and close the socket */

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, c1_socket_id);
    OS_SelectFdAdd(&ReadSet, c2_socket_id);

    UtAssert_INT32_EQ(OS_SelectMultiple(&ReadSet, NULL, UT_TIMEOUT), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&ReadSet, c1_socket_id), true);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&ReadSet, c2_socket_id), true);
}

void TestSelectSingleWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    uint32 StateFlags;

    if (!networkImplemented)
    {
        UtAssert_NA("Network API not implemented");
        return;
    }

    /*
     * In order to get the "write" to block, data must be written to the socket
     * until the OS buffer fills.  Note the server function is waiting on a sem,
     * and not actually reading this data, so writes here will accumulate.
     */

    StateFlags = OS_STREAM_STATE_WRITABLE;
    UtAssert_INT32_EQ(OS_SelectSingle(c1_socket_id, &StateFlags, UT_TIMEOUT), OS_SUCCESS);
    UtAssert_True(StateFlags == OS_STREAM_STATE_WRITABLE,
                  "StateFlags after OS_SelectSingle() (0x%x) == OS_STREAM_STATE_WRITABLE", (unsigned int)StateFlags);

    if (!FillOutputBuffer(c1_socket_id))
    {
        UtAssert_MIR("%s", "Unable to fill buffer with large looped writes, skipping verification");
    }
    else
    {
        UtAssert_INT32_EQ(OS_SelectSingle(c1_socket_id, &StateFlags, UT_TIMEOUT), OS_ERROR_TIMEOUT);
        /* Verify Outputs */
        UtAssert_True(StateFlags == 0, "StateFlags after OS_SelectSingle() (0x%x) == None", (unsigned int)StateFlags);
    }

    /*
     * Giving the sem should cause the server to close the socket,
     * which will discard all written data.  The OS should then consider
     * it writable again, due to EOF condition.
     */
    UtAssert_INT32_EQ(OS_BinSemGive(bin_sem_id), OS_SUCCESS);
    OS_TaskDelay(10); /* Give server time to run and close the socket */

    UtAssert_INT32_EQ(OS_SelectSingle(c1_socket_id, &StateFlags, UT_TIMEOUT), OS_SUCCESS);
    /* Verify Outputs */
    UtAssert_True(StateFlags == 0, "StateFlags after OS_SelectSingle() (0x%x) == OS_STREAM_STATE_WRITABLE",
                  (unsigned int)StateFlags);
}

void TestSelectMultipleWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    OS_FdSet WriteSet;

    if (!networkImplemented)
    {
        return;
    }

    /*
     * Create a server thread, and connect client from
     * this thread to server thread and verify connection
     */

    OS_SelectFdZero(&WriteSet);
    OS_SelectFdAdd(&WriteSet, c1_socket_id);
    OS_SelectFdAdd(&WriteSet, c2_socket_id);
    UtAssert_INT32_EQ(OS_SelectMultiple(NULL, &WriteSet, UT_TIMEOUT), OS_SUCCESS);

    /*
     * Both sockets should initially be writable
     */
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&WriteSet, c1_socket_id), true);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&WriteSet, c2_socket_id), true);

    if (!FillOutputBuffer(c1_socket_id))
    {
        UtAssert_MIR("Unable to fill buffer with large looped writes, skipping verification");
    }
    else
    {
        /* only add the first socket, to get a timeout */
        OS_SelectFdZero(&WriteSet);
        OS_SelectFdAdd(&WriteSet, c1_socket_id);
        UtAssert_INT32_EQ(OS_SelectMultiple(NULL, &WriteSet, UT_TIMEOUT), OS_ERROR_TIMEOUT);

        /*
         * NOTE: NOT checking sets, because after OS_SelectMultiple fails, sets are not defined.
         * (because it timed out, by definition it means all sets are considered empty)
         */
    }

    /*
     * Giving the sem should cause the server to close the socket,
     * which will discard all written data.  The OS should then consider
     * it writable again, due to EOF condition.
     */
    UtAssert_INT32_EQ(OS_BinSemGive(bin_sem_id), OS_SUCCESS);
    OS_TaskDelay(10); /* Give server time to run and close the socket */

    OS_SelectFdZero(&WriteSet);
    OS_SelectFdAdd(&WriteSet, c1_socket_id);
    OS_SelectFdAdd(&WriteSet, c2_socket_id);
    UtAssert_INT32_EQ(OS_SelectMultiple(NULL, &WriteSet, UT_TIMEOUT), OS_SUCCESS);

    UtAssert_INT32_EQ(OS_SelectFdIsSet(&WriteSet, c1_socket_id), true);
    UtAssert_INT32_EQ(OS_SelectFdIsSet(&WriteSet, c2_socket_id), true);
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

    UtTest_Add(TestSelectSingleRead, Setup_Single, Teardown_Single, "TestSelectSingleRead");
    UtTest_Add(TestSelectMultipleRead, Setup_Multi, Teardown_Multi, "TestSelectMultipleRead");
    UtTest_Add(TestSelectSingleWrite, Setup_Single, Teardown_Single, "TestSelectSingleWrite");
    UtTest_Add(TestSelectMultipleWrite, Setup_Multi, Teardown_Multi, "TestSelectMultipleWrite");
}
