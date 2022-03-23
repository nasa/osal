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

#include "osapi-dir.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_DirectoryOpen' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_DirectoryOpen(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *dir_id = UT_Hook_GetArgValueByName(Context, "dir_id", osal_id_t *);
    int32      Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS)
    {
        *dir_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_DIR);
    }
    else
    {
        *dir_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_DirectoryClose' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_DirectoryClose(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t dir_id = UT_Hook_GetArgValueByName(Context, "dir_id", osal_id_t);
    int32     Status;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_DIR, dir_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_DirectoryRead' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_DirectoryRead(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    os_dirent_t *dirent = UT_Hook_GetArgValueByName(Context, "dirent", os_dirent_t *);
    int32        Status;
    size_t       CopySize;

    UT_Stub_GetInt32StatusCode(Context, &Status);

    if (Status == OS_SUCCESS)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_DirectoryRead), dirent, sizeof(*dirent));
        if (CopySize < sizeof(*dirent))
        {
            memset(dirent, 0, sizeof(*dirent));
        }
    }
}
