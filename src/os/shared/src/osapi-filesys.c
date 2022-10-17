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
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/*
 * User defined include files
 */
#include "os-shared-filesys.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_FILE_SYSTEMS,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_FILESYS
};

/*
 * Internal filesystem state table entries
 */
OS_filesys_internal_record_t OS_filesys_table[LOCAL_NUM_OBJECTS];

/*
 * A string that should be the prefix of RAM disk volume names, which
 * provides a hint that the file system refers to a RAM disk.
 *
 * If multiple RAM disks are required then these can be numbered,
 * e.g. RAM0, RAM1, etc.
 */
const char OS_FILESYS_RAMDISK_VOLNAME_PREFIX[] = "RAM";

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Iterator function to match only the free/open entries
 *
 *  Returns: true if the entry is free, false if it is in use
 *
 *-----------------------------------------------------------------*/
bool OS_FileSysFilterFree(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return !OS_ObjectIdDefined(obj->active_id);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks if the filesys table index matches the "virtual_mountpt" field.
 *           Function is Compatible with the Search object lookup routine
 *
 *  Returns: true if the entry matches, false if it does not match
 *
 *-----------------------------------------------------------------*/
bool OS_FileSys_FindVirtMountPoint(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    OS_filesys_internal_record_t *filesys;
    const char *                  target = (const char *)ref;
    size_t                        mplen;

    filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    if ((filesys->flags & OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL) == 0)
    {
        return false;
    }

    mplen = OS_strnlen(filesys->virtual_mountpt, sizeof(filesys->virtual_mountpt));

    /*
     * The virtual_mountpt member should be a substring of the search target.
     * If this matches a basic substring check then it may be match
     */
    if (mplen == 0 || mplen >= sizeof(filesys->virtual_mountpt) ||
        strncmp(target, filesys->virtual_mountpt, mplen) != 0)
    {
        /* not a substring, so not a match */
        return false;
    }

    /*
     * Confirm that the substring ends at either a directory separator
     * or the end of string  (so exact mount points also match).
     *
     * For instance consider a virtual_mountpt of /mnt/abc and searching
     * for target=/mnt/abcd - this should return false in that case.
     */
    return (target[mplen] == '/' || target[mplen] == 0);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implements Common code between the mkfs and initfs calls -
 *           mkfs passes the "should_format" as true and initfs passes as false.
 *
 *  Returns: OS_SUCCESS on creating the disk, or appropriate error code.
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSys_Initialize(char *address, const char *fsdevname, const char *fsvolname, size_t blocksize,
                            osal_blockcount_t numblocks, bool should_format)
{
    OS_filesys_internal_record_t *filesys;
    int32                         return_code;
    OS_object_token_t             token;

    /*
     * Check parameters
     *
     * Note "address" is not checked, because in certain configurations it can be validly null.
     */
    OS_CHECK_STRING(fsdevname, sizeof(filesys->device_name), OS_FS_ERR_PATH_TOO_LONG);
    OS_CHECK_STRING(fsvolname, sizeof(filesys->volume_name), OS_FS_ERR_PATH_TOO_LONG);

    /* check names are not empty strings */
    if (fsdevname[0] == 0 || fsvolname[0] == 0)
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, fsdevname, &token);
    if (return_code == OS_SUCCESS)
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, filesys, device_name, fsdevname);

        /* populate the VolumeName and BlockSize ahead of the Impl call,
         * so the implementation can reference this info if necessary */
        filesys->blocksize = blocksize;
        filesys->numblocks = numblocks;
        filesys->address   = address;
        strncpy(filesys->volume_name, fsvolname, sizeof(filesys->volume_name) - 1);

        /*
         * Determine basic type of filesystem, if not already known
         *
         * if either an address was supplied, or if the volume name
         * contains the string "RAM" then it is a RAM disk. Otherwise
         * leave the type as UNKNOWN and let the implementation decide.
         */
        if (filesys->fstype == OS_FILESYS_TYPE_UNKNOWN &&
            (filesys->address != NULL || strncmp(filesys->volume_name, OS_FILESYS_RAMDISK_VOLNAME_PREFIX,
                                                 sizeof(OS_FILESYS_RAMDISK_VOLNAME_PREFIX) - 1) == 0))
        {
            filesys->fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
        }

        return_code = OS_FileSysStartVolume_Impl(&token);

        if (return_code == OS_SUCCESS)
        {
            /*
             * The "mkfs" call also formats the device.
             * this is the primary difference between mkfs and initfs.
             */
            if (should_format)
            {
                return_code = OS_FileSysFormatVolume_Impl(&token);
            }

            if (return_code == OS_SUCCESS)
            {
                filesys->flags |= OS_FILESYS_FLAG_IS_READY;
            }
            else
            {
                /*
                 * To avoid leaving in an intermediate state,
                 * this also stops the volume if formatting failed.
                 * Cast to void to repress analysis warnings for
                 * ignored return value.
                 */
                (void)OS_FileSysStopVolume_Impl(&token);
            }
        }

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, NULL);
    }

    return return_code;
}

/****************************************************************************************
                                  INITIALIZATION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysAPI_Init(void)
{
    int32 return_code = OS_SUCCESS;

    memset(OS_filesys_table, 0, sizeof(OS_filesys_table));

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysAddFixedMap(osal_id_t *filesys_id, const char *phys_path, const char *virt_path)
{
    OS_filesys_internal_record_t *filesys;
    int32                         return_code;
    OS_object_token_t             token;
    const char *                  dev_name;

    /*
     * Validate inputs
     */
    OS_CHECK_POINTER(filesys_id);
    OS_CHECK_STRING(phys_path, sizeof(filesys->system_mountpt), OS_FS_ERR_PATH_TOO_LONG);
    OS_CHECK_PATHNAME(virt_path);

    /*
     * Generate a dev name by taking the basename of the phys_path.
     */
    dev_name = strrchr(phys_path, '/');
    if (dev_name == NULL)
    {
        dev_name = phys_path;
    }
    else
    {
        ++dev_name;
    }

    if (memchr(dev_name, 0, sizeof(filesys->volume_name)) == NULL)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, dev_name, &token);
    if (return_code == OS_SUCCESS)
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, filesys, device_name, dev_name);

        strncpy(filesys->volume_name, dev_name, sizeof(filesys->volume_name) - 1);
        strncpy(filesys->system_mountpt, phys_path, sizeof(filesys->system_mountpt) - 1);
        strncpy(filesys->virtual_mountpt, virt_path, sizeof(filesys->virtual_mountpt) - 1);

        /*
         * mark the entry that it is a fixed disk
         */
        filesys->fstype = OS_FILESYS_TYPE_FS_BASED;
        filesys->flags  = OS_FILESYS_FLAG_IS_FIXED;

        /*
         * The "mount" implementation is required as it will
         * create the mountpoint if it does not already exist
         */
        return_code = OS_FileSysStartVolume_Impl(&token);

        if (return_code == OS_SUCCESS)
        {
            filesys->flags |= OS_FILESYS_FLAG_IS_READY;
            return_code = OS_FileSysMountVolume_Impl(&token);
        }

        if (return_code == OS_SUCCESS)
        {
            /*
             * mark the entry that it is a fixed disk
             */
            filesys->flags |= OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
        }

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, filesys_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mkfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks)
{
    int32 return_code;

    return_code = OS_FileSys_Initialize(address, devname, volname, blocksize, numblocks, true);

    if (return_code == OS_ERR_INCORRECT_OBJ_STATE || return_code == OS_ERR_NO_FREE_IDS)
    {
        /*
         * This is the historic filesystem-specific error code generated when
         * attempting to mkfs()/initfs() on a filesystem that was
         * already initialized, or if there were no free slots in the table.
         *
         * This code preserved just in case application code was checking for it.
         */
        return_code = OS_FS_ERR_DEVICE_NOT_FREE;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_rmfs(const char *devname)
{
    int32             return_code;
    OS_object_token_t token;

    /* Check parameters */
    OS_CHECK_PATHNAME(devname);

    return_code = OS_ObjectIdGetByName(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, devname, &token);
    if (return_code == OS_SUCCESS)
    {
        /*
         * NOTE: It is likely that if the file system is mounted,
         * this call to stop the volume will fail.
         *
         * It would be prudent to first check the flags to ensure that
         * the filesystem is unmounted first, but this would break
         * compatibility with the existing unit tests.
         */
        return_code = OS_FileSysStopVolume_Impl(&token);

        /* Free the entry in the master table  */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }
    else
    {
        return_code = OS_ERR_NAME_NOT_FOUND;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_initfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks)
{
    int32 return_code;

    return_code = OS_FileSys_Initialize(address, devname, volname, blocksize, numblocks, false);

    if (return_code == OS_ERR_INCORRECT_OBJ_STATE || return_code == OS_ERR_NO_FREE_IDS)
    {
        /*
         * This is the historic filesystem-specific error code generated when
         * attempting to mkfs()/initfs() on a filesystem that was
         * already initialized, or if there were no free slots in the table.
         *
         * This code preserved just in case application code was checking for it.
         */
        return_code = OS_FS_ERR_DEVICE_NOT_FREE;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mount(const char *devname, const char *mountpoint)
{
    int32                         return_code;
    OS_object_token_t             token;
    OS_filesys_internal_record_t *filesys;

    /* Check parameters */
    OS_CHECK_STRING(devname, sizeof(filesys->device_name), OS_FS_ERR_PATH_TOO_LONG);
    OS_CHECK_STRING(mountpoint, sizeof(filesys->virtual_mountpt), OS_FS_ERR_PATH_TOO_LONG);

    return_code = OS_ObjectIdGetByName(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, devname, &token);
    if (return_code == OS_SUCCESS)
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        /*
         * READY flag should be set (mkfs/initfs must have been called on this FS)
         * MOUNTED SYSTEM/VIRTUAL should always be unset.
         *
         * FIXED flag _should_ always be unset (these don't support mount/unmount)
         * but to support abstraction this is not enforced.
         */
        if ((filesys->flags & ~OS_FILESYS_FLAG_IS_FIXED) != OS_FILESYS_FLAG_IS_READY)
        {
            /* mount() cannot be used on this file system at this time */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else if (filesys->system_mountpt[0] == 0)
        {
            /*
             * The system mount point should be a non-empty string.
             */
            return_code = OS_FS_ERR_PATH_INVALID;
        }
        else
        {
            return_code = OS_FileSysMountVolume_Impl(&token);
        }

        if (return_code == OS_SUCCESS)
        {
            /* mark as mounted in the local table.
             * For now this does both sides (system and virtual) */
            filesys->flags |= OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
            strncpy(filesys->virtual_mountpt, mountpoint, sizeof(filesys->virtual_mountpt) - 1);
            filesys->virtual_mountpt[sizeof(filesys->virtual_mountpt) - 1] = 0;
        }

        OS_ObjectIdRelease(&token);
    }

    if (return_code != OS_SUCCESS)
    {
        return_code = OS_ERR_NAME_NOT_FOUND;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_unmount(const char *mountpoint)
{
    int32                         return_code;
    OS_object_token_t             token;
    OS_filesys_internal_record_t *filesys;

    /* Check parameters */
    OS_CHECK_STRING(mountpoint, sizeof(filesys->virtual_mountpt), OS_FS_ERR_PATH_TOO_LONG);

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, OS_FileSys_FindVirtMountPoint,
                                         (void *)mountpoint, &token);

    if (return_code == OS_SUCCESS)
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        /*
         * FIXED flag should always be unset (these don't support mount/unmount at all)
         * READY flag should be set (mkfs/initfs must have been called on this FS)
         * MOUNTED SYSTEM/VIRTUAL should always be unset.
         *
         * The FIXED flag is not enforced to support abstraction.
         */
        if ((filesys->flags & ~OS_FILESYS_FLAG_IS_FIXED) !=
            (OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL))
        {
            /* unmount() cannot be used on this file system at this time */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else
        {
            return_code = OS_FileSysUnmountVolume_Impl(&token);
        }

        if (return_code == OS_SUCCESS)
        {
            /* mark as mounted in the local table.
             * For now this does both sides (system and virtual) */
            filesys->flags &= ~(OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL);
        }

        OS_ObjectIdRelease(&token);
    }

    if (return_code != OS_SUCCESS)
    {
        return_code = OS_ERR_NAME_NOT_FOUND;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStatVolume(const char *name, OS_statvfs_t *statbuf)
{
    int32             return_code;
    OS_object_token_t token;

    /* Check parameters */
    OS_CHECK_PATHNAME(name);
    OS_CHECK_POINTER(statbuf);

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, OS_FileSys_FindVirtMountPoint,
                                         (void *)name, &token);

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileSysStatVolume_Impl(&token, statbuf);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_chkfs(const char *name, bool repair)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check parameters */
    OS_CHECK_PATHNAME(name);

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, OS_FileSys_FindVirtMountPoint,
                                         (void *)name, &token);

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileSysCheckVolume_Impl(&token, repair);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FS_GetPhysDriveName(char *PhysDriveName, const char *MountPoint)
{
    OS_object_token_t             token;
    int32                         return_code;
    OS_filesys_internal_record_t *filesys;

    /* Check parameters */
    OS_CHECK_PATHNAME(MountPoint);
    OS_CHECK_POINTER(PhysDriveName);

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, OS_FileSys_FindVirtMountPoint,
                                         (void *)MountPoint, &token);

    if (return_code == OS_SUCCESS)
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        if ((filesys->flags & OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM) != 0)
        {
            strncpy(PhysDriveName, filesys->system_mountpt, OS_FS_PHYS_NAME_LEN - 1);
            PhysDriveName[OS_FS_PHYS_NAME_LEN - 1] = 0;
        }
        else
        {
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }

        OS_ObjectIdRelease(&token);
    }
    else
    {
        return_code = OS_ERR_NAME_NOT_FOUND;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetFsInfo(os_fsinfo_t *filesys_info)
{
    OS_object_iter_t iter;

    /* Check parameters */
    OS_CHECK_POINTER(filesys_info);

    memset(filesys_info, 0, sizeof(*filesys_info));

    filesys_info->MaxFds     = OS_MAX_NUM_OPEN_FILES;
    filesys_info->MaxVolumes = OS_MAX_FILE_SYSTEMS;

    OS_ObjectIdIteratorInit(OS_FileSysFilterFree, NULL, OS_OBJECT_TYPE_OS_STREAM, &iter);
    while (OS_ObjectIdIteratorGetNext(&iter))
    {
        ++filesys_info->FreeFds;
    }
    OS_ObjectIdIteratorDestroy(&iter);

    OS_ObjectIdIteratorInit(OS_FileSysFilterFree, NULL, OS_OBJECT_TYPE_OS_FILESYS, &iter);
    while (OS_ObjectIdIteratorGetNext(&iter))
    {
        ++filesys_info->FreeVolumes;
    }
    OS_ObjectIdIteratorDestroy(&iter);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    OS_object_token_t             token;
    int32                         return_code;
    const char *                  name_ptr;
    OS_filesys_internal_record_t *filesys;
    size_t                        SysMountPointLen;
    size_t                        VirtPathLen;
    size_t                        VirtPathBegin;

    /*
    ** Check to see if the path pointers are NULL
    */
    /* Check parameters */
    OS_CHECK_POINTER(VirtualPath);
    OS_CHECK_POINTER(LocalPath);

    /*
    ** Check length
    */
    VirtPathLen = OS_strnlen(VirtualPath, OS_MAX_PATH_LEN);
    if (VirtPathLen >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* checks to see if there is a '/' somewhere in the path */
    name_ptr = strrchr(VirtualPath, '/');
    if (name_ptr == NULL)
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /* strrchr returns a pointer to the last '/' char, so we advance one char */
    name_ptr = name_ptr + 1;
    if (memchr(name_ptr, 0, OS_MAX_FILE_NAME) == NULL)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    SysMountPointLen = 0;
    VirtPathBegin    = VirtPathLen;

    /*
    ** All valid Virtual paths must start with a '/' character
    */
    if (VirtualPath[0] != '/')
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, OS_FileSys_FindVirtMountPoint,
                                         (void *)VirtualPath, &token);

    if (return_code != OS_SUCCESS)
    {
        return_code = OS_FS_ERR_PATH_INVALID;
    }
    else
    {
        filesys = OS_OBJECT_TABLE_GET(OS_filesys_table, token);

        if ((filesys->flags & OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM) != 0)
        {
            SysMountPointLen = OS_strnlen(filesys->system_mountpt, sizeof(filesys->system_mountpt));
            VirtPathBegin    = OS_strnlen(filesys->virtual_mountpt, sizeof(filesys->virtual_mountpt));
            if (SysMountPointLen < OS_MAX_LOCAL_PATH_LEN)
            {
                memcpy(LocalPath, filesys->system_mountpt, SysMountPointLen);
            }
        }
        else
        {
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }

        OS_ObjectIdRelease(&token);
    }

    if (return_code == OS_SUCCESS)
    {
        if (VirtPathLen < VirtPathBegin)
        {
            return_code = OS_FS_ERR_PATH_INVALID;
        }
        else
        {
            VirtPathLen -= VirtPathBegin;
            if ((SysMountPointLen + VirtPathLen) < OS_MAX_LOCAL_PATH_LEN)
            {
                memcpy(&LocalPath[SysMountPointLen], &VirtualPath[VirtPathBegin], VirtPathLen);
                LocalPath[SysMountPointLen + VirtPathLen] = 0;
            }
            else
            {
                return_code = OS_FS_ERR_PATH_TOO_LONG;
            }
        }
    }

    return return_code;
}
