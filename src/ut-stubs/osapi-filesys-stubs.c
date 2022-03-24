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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-filesys header
 */

#include "osapi-filesys.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_FS_GetPhysDriveName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_FileSysAddFixedMap(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_FileSysStatVolume(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_GetFsInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TranslatePath(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FS_GetPhysDriveName()
 * ----------------------------------------------------
 */
int32 OS_FS_GetPhysDriveName(char *PhysDriveName, const char *MountPoint)
{
    UT_GenStub_SetupReturnBuffer(OS_FS_GetPhysDriveName, int32);

    UT_GenStub_AddParam(OS_FS_GetPhysDriveName, char *, PhysDriveName);
    UT_GenStub_AddParam(OS_FS_GetPhysDriveName, const char *, MountPoint);

    UT_GenStub_Execute(OS_FS_GetPhysDriveName, Basic, UT_DefaultHandler_OS_FS_GetPhysDriveName);

    return UT_GenStub_GetReturnValue(OS_FS_GetPhysDriveName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysAddFixedMap()
 * ----------------------------------------------------
 */
int32 OS_FileSysAddFixedMap(osal_id_t *filesys_id, const char *phys_path, const char *virt_path)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysAddFixedMap, int32);

    UT_GenStub_AddParam(OS_FileSysAddFixedMap, osal_id_t *, filesys_id);
    UT_GenStub_AddParam(OS_FileSysAddFixedMap, const char *, phys_path);
    UT_GenStub_AddParam(OS_FileSysAddFixedMap, const char *, virt_path);

    UT_GenStub_Execute(OS_FileSysAddFixedMap, Basic, UT_DefaultHandler_OS_FileSysAddFixedMap);

    return UT_GenStub_GetReturnValue(OS_FileSysAddFixedMap, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysStatVolume()
 * ----------------------------------------------------
 */
int32 OS_FileSysStatVolume(const char *name, OS_statvfs_t *statbuf)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysStatVolume, int32);

    UT_GenStub_AddParam(OS_FileSysStatVolume, const char *, name);
    UT_GenStub_AddParam(OS_FileSysStatVolume, OS_statvfs_t *, statbuf);

    UT_GenStub_Execute(OS_FileSysStatVolume, Basic, UT_DefaultHandler_OS_FileSysStatVolume);

    return UT_GenStub_GetReturnValue(OS_FileSysStatVolume, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetFsInfo()
 * ----------------------------------------------------
 */
int32 OS_GetFsInfo(os_fsinfo_t *filesys_info)
{
    UT_GenStub_SetupReturnBuffer(OS_GetFsInfo, int32);

    UT_GenStub_AddParam(OS_GetFsInfo, os_fsinfo_t *, filesys_info);

    UT_GenStub_Execute(OS_GetFsInfo, Basic, UT_DefaultHandler_OS_GetFsInfo);

    return UT_GenStub_GetReturnValue(OS_GetFsInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TranslatePath()
 * ----------------------------------------------------
 */
int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    UT_GenStub_SetupReturnBuffer(OS_TranslatePath, int32);

    UT_GenStub_AddParam(OS_TranslatePath, const char *, VirtualPath);
    UT_GenStub_AddParam(OS_TranslatePath, char *, LocalPath);

    UT_GenStub_Execute(OS_TranslatePath, Basic, UT_DefaultHandler_OS_TranslatePath);

    return UT_GenStub_GetReturnValue(OS_TranslatePath, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_chkfs()
 * ----------------------------------------------------
 */
int32 OS_chkfs(const char *name, bool repair)
{
    UT_GenStub_SetupReturnBuffer(OS_chkfs, int32);

    UT_GenStub_AddParam(OS_chkfs, const char *, name);
    UT_GenStub_AddParam(OS_chkfs, bool, repair);

    UT_GenStub_Execute(OS_chkfs, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_chkfs, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_initfs()
 * ----------------------------------------------------
 */
int32 OS_initfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks)
{
    UT_GenStub_SetupReturnBuffer(OS_initfs, int32);

    UT_GenStub_AddParam(OS_initfs, char *, address);
    UT_GenStub_AddParam(OS_initfs, const char *, devname);
    UT_GenStub_AddParam(OS_initfs, const char *, volname);
    UT_GenStub_AddParam(OS_initfs, size_t, blocksize);
    UT_GenStub_AddParam(OS_initfs, osal_blockcount_t, numblocks);

    UT_GenStub_Execute(OS_initfs, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_initfs, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_mkfs()
 * ----------------------------------------------------
 */
int32 OS_mkfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks)
{
    UT_GenStub_SetupReturnBuffer(OS_mkfs, int32);

    UT_GenStub_AddParam(OS_mkfs, char *, address);
    UT_GenStub_AddParam(OS_mkfs, const char *, devname);
    UT_GenStub_AddParam(OS_mkfs, const char *, volname);
    UT_GenStub_AddParam(OS_mkfs, size_t, blocksize);
    UT_GenStub_AddParam(OS_mkfs, osal_blockcount_t, numblocks);

    UT_GenStub_Execute(OS_mkfs, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_mkfs, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_mount()
 * ----------------------------------------------------
 */
int32 OS_mount(const char *devname, const char *mountpoint)
{
    UT_GenStub_SetupReturnBuffer(OS_mount, int32);

    UT_GenStub_AddParam(OS_mount, const char *, devname);
    UT_GenStub_AddParam(OS_mount, const char *, mountpoint);

    UT_GenStub_Execute(OS_mount, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_mount, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_rmfs()
 * ----------------------------------------------------
 */
int32 OS_rmfs(const char *devname)
{
    UT_GenStub_SetupReturnBuffer(OS_rmfs, int32);

    UT_GenStub_AddParam(OS_rmfs, const char *, devname);

    UT_GenStub_Execute(OS_rmfs, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_rmfs, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_unmount()
 * ----------------------------------------------------
 */
int32 OS_unmount(const char *mountpoint)
{
    UT_GenStub_SetupReturnBuffer(OS_unmount, int32);

    UT_GenStub_AddParam(OS_unmount, const char *, mountpoint);

    UT_GenStub_Execute(OS_unmount, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_unmount, int32);
}
