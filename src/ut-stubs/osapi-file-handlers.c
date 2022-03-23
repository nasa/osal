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
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-file.h" /* OSAL public API for this subsystem */
#include "osapi-idmap.h"
#include "utstub-helpers.h"

/*****************************************************************************
 *
 * Local Stub helper function for reading
 *
 *****************************************************************************/
static void UT_GenericReadStub(UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    void * buffer = UT_Hook_GetArgValueByName(Context, "buffer", void *);
    size_t nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    size_t CopySize;
    int32  status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        CopySize = UT_Stub_CopyToLocal(FuncKey, buffer, nbytes);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, nbytes);
            status = nbytes;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }

    UT_Stub_SetReturnValue(FuncKey, status);
}

/*****************************************************************************
 *
 * Local Stub helper function for writing
 *
 *****************************************************************************/
static void UT_GenericWriteStub(UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const void *buffer = UT_Hook_GetArgValueByName(Context, "buffer", const void *);
    size_t      nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    size_t      CopySize;
    int32       status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        CopySize = UT_Stub_CopyFromLocal(FuncKey, buffer, nbytes);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = nbytes;
        }
    }

    UT_Stub_SetReturnValue(FuncKey, status);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_OpenCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_OpenCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *filedes = UT_Hook_GetArgValueByName(Context, "filedes", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *filedes = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_STREAM);
    }
    else
    {
        *filedes = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_close' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_close(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t filedes = UT_Hook_GetArgValueByName(Context, "filedes", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_STREAM, filedes);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_read' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_read(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_GenericReadStub(FuncKey, Context);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_write' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_write(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_GenericWriteStub(FuncKey, Context);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimedRead' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimedRead(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_GenericReadStub(FuncKey, Context);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimedWrite' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimedWrite(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_GenericWriteStub(FuncKey, Context);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_stat' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_stat(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    os_fstat_t *filestats = UT_Hook_GetArgValueByName(Context, "filestats", os_fstat_t *);
    size_t      CopySize;
    int32       Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_stat), filestats, sizeof(*filestats));

        /* Ensure memory is set if not provided by test */
        if (CopySize < sizeof(*filestats))
        {
            memset(filestats, 0, sizeof(*filestats));
        }
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_lseek' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_lseek(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    int32 offset = UT_Hook_GetArgValueByName(Context, "offset", int32);
    int32 Status;

    if (UT_Stub_GetInt32StatusCode(Context, &Status))
    {
        /* Use the configured status code directly as the returned offset */
        offset = Status;
    }

    UT_Stub_SetReturnValue(FuncKey, offset);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_FDGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_FDGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_file_prop_t *fd_prop = UT_Hook_GetArgValueByName(Context, "fd_prop", OS_file_prop_t *);
    int32           status;
    size_t          CopySize;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_FDGetInfo), fd_prop, sizeof(*fd_prop));
        if (CopySize < sizeof(*fd_prop))
        {
            memset(fd_prop, 0, sizeof(*fd_prop));
            fd_prop->IsValid = true;
            UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &fd_prop->User);
        }
    }
}
