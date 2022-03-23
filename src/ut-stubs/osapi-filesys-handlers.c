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

#include "osapi-filesys.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_FileSysAddFixedMap' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_FileSysAddFixedMap(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *filesys_id = UT_Hook_GetArgValueByName(Context, "filesys_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *filesys_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_FILESYS);
    }
    else
    {
        *filesys_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_FileSysStatVolume' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_FileSysStatVolume(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_statvfs_t *statbuf = UT_Hook_GetArgValueByName(Context, "statbuf", OS_statvfs_t *);
    int32         status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_FileSysStatVolume), statbuf, sizeof(*statbuf)) < sizeof(*statbuf))
    {
        memset(statbuf, 0, sizeof(*statbuf));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_FS_GetPhysDriveName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_FS_GetPhysDriveName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    char *      PhysDriveName = UT_Hook_GetArgValueByName(Context, "PhysDriveName", char *);
    const char *MountPoint    = UT_Hook_GetArgValueByName(Context, "MountPoint", const char *);

    strncpy(PhysDriveName, MountPoint, OS_FS_PHYS_NAME_LEN - 1);
    PhysDriveName[OS_FS_PHYS_NAME_LEN - 1] = 0;
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetFsInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetFsInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    os_fsinfo_t *filesys_info = UT_Hook_GetArgValueByName(Context, "filesys_info", os_fsinfo_t *);

    memset(filesys_info, 0, sizeof(*filesys_info));
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TranslatePath' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TranslatePath(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const char *VirtualPath = UT_Hook_GetArgValueByName(Context, "VirtualPath", const char *);
    char *      LocalPath   = UT_Hook_GetArgValueByName(Context, "LocalPath", char *);
    int32       status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && VirtualPath != NULL && LocalPath != NULL &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TranslatePath), LocalPath, OS_MAX_LOCAL_PATH_LEN) == 0)
    {
        strncpy(LocalPath, VirtualPath, OS_MAX_LOCAL_PATH_LEN - 1);
        LocalPath[OS_MAX_LOCAL_PATH_LEN - 1] = 0;
    }
}
