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
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-sockets.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketOpen' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketOpen(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sock_id = UT_Hook_GetArgValueByName(Context, "sock_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *sock_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_STREAM);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketRecvFrom' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketRecvFrom(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    void * buffer = UT_Hook_GetArgValueByName(Context, "buffer", void *);
    size_t buflen = UT_Hook_GetArgValueByName(Context, "buflen", size_t);
    int32  status;
    size_t CopySize;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketRecvFrom), buffer, buflen);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, buflen);
            status = buflen;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }

    UT_Stub_SetReturnValue(FuncKey, status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketSendTo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketSendTo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const void *buffer = UT_Hook_GetArgValueByName(Context, "buffer", const void *);
    size_t      buflen = UT_Hook_GetArgValueByName(Context, "buflen", size_t);
    int32       status;
    size_t      CopySize;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        CopySize = UT_Stub_CopyFromLocal(UT_KEY(OS_SocketSendTo), buffer, buflen);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = buflen;
        }
    }

    UT_Stub_SetReturnValue(FuncKey, status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sock_id = UT_Hook_GetArgValueByName(Context, "sock_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetIdByName), sock_id, sizeof(*sock_id)) < sizeof(*sock_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_STREAM, sock_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_socket_prop_t *sock_prop = UT_Hook_GetArgValueByName(Context, "sock_prop", OS_socket_prop_t *);
    int32             status;
    size_t            CopySize;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_SocketGetInfo), sock_prop, sizeof(*sock_prop));
        if (CopySize < sizeof(*sock_prop))
        {
            UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &sock_prop->creator);
            strncpy(sock_prop->name, "ut", sizeof(sock_prop->name) - 1);
            sock_prop->name[sizeof(sock_prop->name) - 1] = 0;
        }
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketAddrInit' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketAddrInit(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_SockAddr_t *Addr = UT_Hook_GetArgValueByName(Context, "Addr", OS_SockAddr_t *);
    int32          status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrInit), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketAddrToString' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketAddrToString(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    char * buffer = UT_Hook_GetArgValueByName(Context, "buffer", char *);
    size_t buflen = UT_Hook_GetArgValueByName(Context, "buflen", size_t);
    int32  status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && buflen > 0 && UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrToString), buffer, buflen) == 0)
    {
        strncpy(buffer, "UT-addr", buflen - 1);
        buffer[buflen - 1] = 0;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketAddrFromString' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketAddrFromString(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_SockAddr_t *Addr = UT_Hook_GetArgValueByName(Context, "Addr", OS_SockAddr_t *);
    int32          status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrFromString), Addr, sizeof(*Addr)) < sizeof(*Addr))
    {
        memset(Addr, 0, sizeof(*Addr));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SocketAddrGetPort' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketAddrGetPort(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint16 *PortNum = UT_Hook_GetArgValueByName(Context, "PortNum", uint16 *);
    int32   status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrGetPort), PortNum, sizeof(*PortNum)) < sizeof(*PortNum))
    {
        *PortNum = 0;
    }
}
