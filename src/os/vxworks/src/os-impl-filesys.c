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
 * \file     os-impl-filesys.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include "os-impl-filesys.h"
#include "os-impl-dirs.h"
#include "os-shared-filesys.h"
#include "os-shared-idmap.h"

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stat.h>
#include <ioLib.h>
#include <errnoLib.h>
#include <ramDrv.h>
#include <xbdBlkDev.h>
#include <xbdRamDisk.h>
#include <dosFsLib.h>

#ifdef USE_VXWORKS_ATA_DRIVER
#include "drv/hdisk/ataDrv.h"
#endif

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   Data Types
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];

/****************************************************************************************
                                    Filesys API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStartVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStartVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *     local;
    OS_impl_filesys_internal_record_t *impl;
    int32                              return_code;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_filesys_table, *token);
    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    memset(impl, 0, sizeof(*impl));
    return_code = OS_ERR_NOT_IMPLEMENTED;
    switch (local->fstype)
    {
        case OS_FILESYS_TYPE_FS_BASED:
        {
            /* pass through for FS_BASED volumes, assume already mounted */
            OS_DEBUG("OSAL: Mapping an FS_BASED disk at: %s\n", (unsigned long)local->system_mountpt);
            return_code = OS_SUCCESS;
            break;
        }

        case OS_FILESYS_TYPE_VOLATILE_DISK:
        {
            OS_DEBUG("OSAL: Starting a RAM disk at: 0x%08lX\n", (unsigned long)local->address);

            /*
            ** Create the ram disk device
            ** The 32 is the number of blocks per track.
            **  Other values dont seem to work here
            */
            impl->blkDev           = ramDevCreate(local->address, local->blocksize, 32, local->numblocks, 0);
            impl->xbdMaxPartitions = 1;
            break;
        }

#ifdef USE_VXWORKS_ATA_DRIVER
        case OS_FILESYS_TYPE_NORMAL_DISK:
        {
            /*
            ** Create the Flash disk device
            ** This code requires an ATA driver in the BSP, so it must be
            ** left out of the compilation BSPs without.
            */
            OS_DEBUG("OSAL: Starting an ATA DISK: %s\n", local->volume_name);
            impl->xbdMaxPartitions = 4;
            impl->blkDev           = ataDevCreate(0, 0, 0, 0);
            break;
        }
#endif

        default:
            break;
    }

    if (impl->xbdMaxPartitions > 0)
    {
        /*
         * This code is common to RAM disks and ATA disks
         * (and anything else that relies on the xbd layer)
         */

        if (impl->blkDev == NULL)
        {
            /* there was an error calling the "DevCreate" function */
            OS_DEBUG("OSAL: Error creating low level block device\n");
            return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
        }
        else
        {
            /*
             * Connect the low level block device to the xbd device
             */
            impl->xbd = xbdBlkDevCreateSync(impl->blkDev, local->volume_name);
            if (impl->xbd == NULLDEV)
            {
                return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
            }
            else
            {
                /*
                 * Always using partition ":0"
                 *
                 * For ATA disks, this is different than the previous implementation
                 * which would try to open() all possible partitions in order until
                 * one was successful.
                 *
                 * From the original OS_GetPhysDeviceName() implementation comments:
                 *
                 *  The disk XBD code will add ":X" to the volume name you give to a disk, where
                 *  X is the partition number. While RAM disks are always 0 ( "RAM:0" ),
                 *  a physical disk such as a compact flash disk can be ":0", or ":1" etc,
                 *  depending on how the disk was partitioned.
                 *
                 * But there are two issues with trying all possible devices like that:
                 *  - Trying with open() actually mounts the filesystem, meaning it must
                 *    have a pre-existing dosFs on it
                 *  - It might not be consistent between devices/cards or even run-to-run.
                 *    (i.e. if a partition was formatted manually and then the software
                 *    restarted, a different block device might get mounted the second time)
                 */
                snprintf(local->system_mountpt, sizeof(local->system_mountpt), "%s:0", local->volume_name);

                return_code = OS_SUCCESS;
            }
        }
    }

    return return_code;

} /* end OS_FileSysStartVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStopVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStopVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *     local;
    OS_impl_filesys_internal_record_t *impl;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_filesys_table, *token);
    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    switch (local->fstype)
    {
        case OS_FILESYS_TYPE_VOLATILE_DISK:
        case OS_FILESYS_TYPE_NORMAL_DISK:
        {
            if (impl->xbdMaxPartitions > 0 && impl->xbd != NULLDEV)
            {
                xbdBlkDevDelete(impl->xbd, NULL);
                impl->xbd              = NULLDEV;
                impl->xbdMaxPartitions = 0;
            }
            break;
        }
        default:
            break;
    }

    /*
     * TBD: The VxWorks documentation does not seem to indicate any
     * "DevDelete" operation as the complement to ramDevCreate/ataDevCreate.
     */

    return OS_SUCCESS;

} /* end OS_FileSysStopVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysFormatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysFormatVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *local;
    int32                         return_code = OS_ERR_NOT_IMPLEMENTED;
    int                           status;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    switch (local->fstype)
    {
        case OS_FILESYS_TYPE_FS_BASED:
        {
            /*
             * The "format" operation is a no-op on FS_BASED types.
             * Return success to allow the operation to continue.
             */
            return_code = OS_SUCCESS;
            break;
        }
        case OS_FILESYS_TYPE_VOLATILE_DISK:
        case OS_FILESYS_TYPE_NORMAL_DISK:
        {
            /*
            ** Call the dos format routine
            */
            status = dosFsVolFormat(local->system_mountpt, DOS_OPT_BLANK, NULL);
            if (status == -1)
            {
                OS_DEBUG("OSAL: dosFsVolFormat failed. Errno = %d\n", errnoGet());
                return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
            }
            else
            {
                return_code = OS_SUCCESS;
            }
            break;
        }
        default:
            break;
    }

    return return_code;

} /* end OS_FileSysFormatVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysMountVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysMountVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *local;
    int32                         status;
    int                           fd;
    struct stat                   stat_buf;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    /*
     * For FS-based mounts, these are just a map to a some other
     * directory in the filesystem.
     *
     * If it does exist then make sure it is actually a directory.
     * If it does not exist then attempt to create it.
     */
    if (local->fstype == OS_FILESYS_TYPE_FS_BASED)
    {
        if (stat(local->system_mountpt, &stat_buf) == 0)
        {
            if (S_ISDIR(stat_buf.st_mode))
            {
                /* mount point exists */
                status = OS_SUCCESS;
            }
            else
            {
                OS_DEBUG("%s is not a directory\n", local->system_mountpt);
                status = OS_FS_ERR_PATH_INVALID;
            }
        }
        else
        {
            if (mkdir(local->system_mountpt, 0775) == 0)
            {
                /* directory created OK */
                status = OS_SUCCESS;
            }
            else
            {
                OS_DEBUG("mkdir(%s): errno=%d\n", local->system_mountpt, errnoGet());
                status = OS_FS_ERR_DRIVE_NOT_CREATED;
            }
        }
    }
    else
    {
        /*
         * For all other (non-FS_BASED) filesystem types,
         * Calling open() on the physical device path mounts the device.
         */
        fd = open(local->system_mountpt, O_RDONLY, 0644);
        if (fd < 0)
        {
            status = OS_ERROR;
        }
        else
        {
            status = OS_SUCCESS;
            close(fd);
        }
    }

    return status;

} /* end OS_FileSysMountVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysUnmountVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysUnmountVolume_Impl(const OS_object_token_t *token)
{
    OS_filesys_internal_record_t *local;
    int32                         status;
    int                           fd;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    if (local->fstype == OS_FILESYS_TYPE_FS_BASED)
    {
        /* unmount is a no-op on FS-based mounts - it is just a directory map */
        status = OS_SUCCESS;
    }
    else
    {
        /*
        ** vxWorks uses an ioctl to unmount
        */
        fd = open(local->system_mountpt, O_RDONLY, 0644);
        if (fd < 0)
        {
            status = OS_ERROR;
        }
        else
        {
            if (ioctl(fd, FIOUNMOUNT, 0) < 0)
            {
                status = OS_ERROR;
            }
            else
            {
                status = OS_SUCCESS;
            }

            close(fd);
        }
    }

    return status;

} /* end OS_FileSysUnmountVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStatVolume_Impl(const OS_object_token_t *token, OS_statvfs_t *result)
{
    OS_filesys_internal_record_t *local;
    struct statfs                 stat_buf;
    int                           return_code;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    if (statfs(local->system_mountpt, &stat_buf) != 0)
    {
        return_code = OS_ERROR;
        memset(result, 0, sizeof(*result));
    }
    else
    {
        result->block_size   = OSAL_SIZE_C(stat_buf.f_bsize);
        result->blocks_free  = OSAL_BLOCKCOUNT_C(stat_buf.f_bfree);
        result->total_blocks = OSAL_BLOCKCOUNT_C(stat_buf.f_blocks);
        return_code          = OS_SUCCESS;
    }

    return return_code;

} /* end OS_FileSysStatVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysCheckVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysCheckVolume_Impl(const OS_object_token_t *token, bool repair)
{
    OS_filesys_internal_record_t *local;
    STATUS                        chk_status;
    int                           flags;
    int                           fd;

    local = OS_OBJECT_TABLE_GET(OS_filesys_table, *token);

    fd = open(local->system_mountpt, O_RDONLY, 0);
    if (fd < 0)
    {
        return OS_ERROR;
    }

    /* Fix the disk if there are errors */
    if (repair)
    {
        flags = DOS_CHK_REPAIR;
    }
    else
    {
        flags = DOS_CHK_ONLY;
    }

    flags |= DOS_CHK_VERB_SILENT;

    chk_status = ioctl(fd, FIOCHKDSK, flags);

    close(fd);

    if (chk_status != OK)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_FileSysCheckVolume_Impl */
