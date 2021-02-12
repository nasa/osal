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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
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

UT_DEFAULT_STUB(OS_DirAPI_Init, (void))

/*****************************************************************************
 *
 * Stub for OS_mkdir() function
 *
 *****************************************************************************/
int32 OS_mkdir(const char *path, uint32 access)
{
    UT_Stub_RegisterContext(UT_KEY(OS_mkdir), path);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_mkdir), access);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_mkdir);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_rmdir() function
 *
 *****************************************************************************/
int32 OS_rmdir(const char *path)
{
    UT_Stub_RegisterContext(UT_KEY(OS_rmdir), path);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_rmdir);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryOpen() function
 *
 *****************************************************************************/
int32 OS_DirectoryOpen(osal_id_t *dir_id, const char *path)
{
    UT_Stub_RegisterContext(UT_KEY(OS_DirectoryOpen), dir_id);
    UT_Stub_RegisterContext(UT_KEY(OS_DirectoryOpen), path);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryOpen);

    if (Status == OS_SUCCESS)
    {
        *dir_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_DIR);
    }
    else
    {
        *dir_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryClose() function
 *
 *****************************************************************************/
int32 OS_DirectoryClose(osal_id_t dir_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_DirectoryClose), dir_id);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryClose);

    if (Status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_DIR, dir_id);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryRewind() function
 *
 *****************************************************************************/
int32 OS_DirectoryRewind(osal_id_t dir_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_DirectoryRewind), dir_id);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryRewind);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryRead() function
 *
 *****************************************************************************/
int32 OS_DirectoryRead(osal_id_t dir_id, os_dirent_t *dirent)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_DirectoryRead), dir_id);
    UT_Stub_RegisterContext(UT_KEY(OS_DirectoryRead), dirent);

    int32  Status;
    size_t CopySize;

    Status = UT_DEFAULT_IMPL(OS_DirectoryRead);

    if (Status == OS_SUCCESS)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_DirectoryRead), dirent, sizeof(*dirent));
        if (CopySize < sizeof(*dirent))
        {
            memset(dirent, 0, sizeof(*dirent));
        }
    }

    return Status;
}
