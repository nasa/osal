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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/vfs.h>

#include "os-posix.h"
#include "os-shared-filesys.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
const char OS_POSIX_DEVICEFILE_PREFIX[] = "/dev/";

/****************************************************************************************
                                Filesys API
 ***************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_FileSysAPI_Impl_Init

    Purpose: Filesystem API global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Posix_FileSysAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

/*
 * System Level API
 */

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStartVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *local;
    struct stat                   stat_buf;
    const char *                  tmpdir;
    size_t                        mplen;
    size_t                        vollen;
    uint32                        i;
    enum
    {
        VOLATILE_DISK_LOC_DEV_SHM,
        VOLATILE_DISK_LOC_ENV,
        VOLATILE_DISK_LOC_VARTMP,
        VOLATILE_DISK_LOC_TMP,
        VOLATILE_DISK_LOC_MAX
    };

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    /*
     * Determine basic type of filesystem, if not already known
     */
    if (local->fstype == OS_FILESYS_TYPE_UNKNOWN &&
        strncmp(local->device_name, OS_POSIX_DEVICEFILE_PREFIX, sizeof(OS_POSIX_DEVICEFILE_PREFIX) - 1) == 0)
    {
        /*
         * If referring to a real device in the /dev filesystem,
         * then assume it is a normal disk.
         */
        local->fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    }

    /*
     * For VOLATILE volumes, there are two options:
     *  - The /dev/shm filesystem, if it exists
     *  - The /tmp filesystem
     *
     * The /dev/shm is preferable because it should actually be a ramdisk, but
     * it is system-specific - should exist on Linux if it is mounted.
     * The /tmp file system might be a regular persistent disk, but should always exist
     * on any POSIX-compliant OS.
     */

    tmpdir = NULL;
    if (local->fstype == OS_FILESYS_TYPE_VOLATILE_DISK)
    {
        /* find a suitable location to keep the volatile disk */
        for (i = 0; i <= VOLATILE_DISK_LOC_MAX; ++i)
        {
            switch (i)
            {
                case VOLATILE_DISK_LOC_DEV_SHM:
                    /* This is most preferable because it should actually be a ramdisk */
                    tmpdir = "/dev/shm";
                    break;
                case VOLATILE_DISK_LOC_ENV:
                    /* try the TMPDIR environment variable, if set */
                    tmpdir = getenv("TMPDIR");
                    break;
                case VOLATILE_DISK_LOC_VARTMP:
                    /* try /var/tmp directory */
                    tmpdir = "/var/tmp";
                    break;
                case VOLATILE_DISK_LOC_TMP:
                    /* use /tmp directory as a last resort */
                    tmpdir = "/tmp";
                    break;
                default:
                    tmpdir = NULL;
                    break;
            }

            if (tmpdir != NULL && stat(tmpdir, &stat_buf) == 0)
            {
                /* check if the user has write permission to the directory */
                if ((stat_buf.st_mode & S_IWOTH) != 0 ||
                    ((stat_buf.st_mode & S_IWGRP) != 0 && stat_buf.st_gid == getegid()) ||
                    ((stat_buf.st_mode & S_IWUSR) != 0 && stat_buf.st_uid == geteuid()))
                {
                    break;
                }
            }
        }

        if (tmpdir == NULL)
        {
            /* OS provides no place to put the volume */
            OS_DEBUG("No storage location for volatile volumes");
            return OS_FS_ERR_DRIVE_NOT_CREATED;
        }

        /*
         * Note - performing the concatenation in a single snprintf() call seems
         * to trigger a (false) pointer overlap warning, because volume_name should
         * always be null terminated.  To get around this, calculate the
         * string size and check that it is within the expected size, and do the
         * append of volume_name explicitly.
         */
        mplen = snprintf(local->system_mountpt, sizeof(local->system_mountpt), "%s/osal:", tmpdir);
        if (mplen < sizeof(local->system_mountpt))
        {
            vollen = OS_strnlen(local->volume_name, sizeof(local->volume_name));
            if ((vollen + mplen) >= sizeof(local->system_mountpt))
            {
                vollen = sizeof(local->system_mountpt) - mplen - 1;
            }
            memcpy(&local->system_mountpt[mplen], local->volume_name, vollen);
            local->system_mountpt[mplen + vollen] = 0;
        }
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStopVolume_Impl(const OS_object_token_t *token)
{
    /*
     * This is a no-op.
     *
     * Volatile volumes are just directories created in the temp dir,
     * and this will not remove the directories just in case something
     * went wrong.
     *
     * If the volume is started again, the directory will be re-used.
     */
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysFormatVolume_Impl(const OS_object_token_t *token)
{
    /*
     * In theory, this should wipe any existing files in the ramdisk,
     * but since ramdisks here are implemented using a directory within a tmpfs,
     * removal of such files could be risky if something goes wrong,
     * because it might remove files that were important.
     *
     * So the safest option is just a no-op.
     * (this is also backward compatible since POSIX mkfs was always a no-op)
     */
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysMountVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *local;
    struct stat                   stat_buf;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    /*
     * This will do a mkdir() for the mount point if it does
     * not already exist.
     */
    if (stat(local->system_mountpt, &stat_buf) != 0)
    {
        if (mkdir(local->system_mountpt, 0700) < 0)
        {
            OS_DEBUG("ERROR: Cannot create mount point %s: %s", local->system_mountpt, strerror(errno));
            return OS_FS_ERR_DRIVE_NOT_CREATED;
        }
    }
    else if (!S_ISDIR(stat_buf.st_mode))
    {
        OS_DEBUG("ERROR: Volume %s exists and is not a directory", local->system_mountpt);
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }

    /*
     * NOTE: The mount() system call could be used here to actually
     * mount a disk, if warranted.  For all current POSIX-based PSPs,
     * this is not needed, because the volumes are all pre-mounted
     * through the system init before OSAL starts.
     *
     * For volatile filesystems (ramdisk) these were created within
     * a temp filesystem, so all that is needed is to ensure the
     * mount point exists.  For any other FS type, trigger an
     * error to indicate that it is not implemented in this OSAL.
     */
    if (local->fstype != OS_FILESYS_TYPE_VOLATILE_DISK && local->fstype != OS_FILESYS_TYPE_FS_BASED)
    {
        /* the mount command is not implemented for this FS type */
        return OS_ERR_NOT_IMPLEMENTED;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysUnmountVolume_Impl(const OS_object_token_t *token)
{
    /*
     * NOTE: Mounting/Unmounting on POSIX is not implemented.
     * For backward compatibility this call must return success.
     *
     * This is a no-op.  The mount point that was created during
     * the mount process can stay for the next mount.
     */
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStatVolume_Impl(const OS_object_token_t *token, OS_statvfs_t *result)
{
    OS_filesys_internal_record_t *local;
    struct statvfs                stat_buf;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    if (statvfs(local->system_mountpt, &stat_buf) != 0)
    {
        return OS_ERROR;
    }

    result->block_size   = OSAL_SIZE_C(stat_buf.f_bsize);
    result->blocks_free  = OSAL_BLOCKCOUNT_C(stat_buf.f_bfree);
    result->total_blocks = OSAL_BLOCKCOUNT_C(stat_buf.f_blocks);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysCheckVolume_Impl(const OS_object_token_t *token, bool repair)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
