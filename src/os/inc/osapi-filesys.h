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
 * \file
 *
 * Declarations and prototypes for file systems
 */

#ifndef OSAPI_FILESYS_H
#define OSAPI_FILESYS_H

#include "osconfig.h"
#include "common_types.h"

#define OS_CHK_ONLY 0 /**< Unused, API takes bool */
#define OS_REPAIR   1 /**< Unused, API takes bool */

/** @brief OSAL file system info */
typedef struct
{
    uint32 MaxFds;      /**< @brief Total number of file descriptors */
    uint32 FreeFds;     /**< @brief Total number that are free */
    uint32 MaxVolumes;  /**< @brief Maximum number of volumes */
    uint32 FreeVolumes; /**< @brief Total number of volumes free */
} os_fsinfo_t;

/*
 * @brief The data type filled in by the OS_FileSysStatVolume() call.
 *
 * Encapsulates detail information about the size and available space
 * in a mounted file system volume.
 */
typedef struct
{
    size_t            block_size;   /**< Block size of underlying FS */
    osal_blockcount_t total_blocks; /**< Total blocks in underlying FS */
    osal_blockcount_t blocks_free;  /**< Available blocks in underlying FS */
} OS_statvfs_t;

/*
 * Exported Functions
 */

/** @defgroup OSAPIFileSys OSAL File System Level APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create a fixed mapping between an existing directory and a virtual OSAL mount point.
 *
 * This mimics the behavior of a "FS_BASED" entry in the VolumeTable but is registered
 * at runtime.  It is intended to be called by the PSP/BSP prior to starting the application.
 *
 * @note OSAL virtual mount points are required to be a single, non-empty top-level directory
 * name.  Virtual path names always follow the form /\<virt_mount_point\>/\<relative_path\>/\<file\>.
 * Only the relative path may be omitted/empty (i.e. /\<virt_mount_point\>/\<file\>) but the
 * virtual mount point must be present and not an empty string.  In particular this means
 * it is not possible to directly refer to files in the "root" of the native file system
 * from OSAL.  However it is possible to create a virtual map to the root, such as by calling:
 *
 *      OS_FileSysAddFixedMap(&fs_id, "/", "/root");
 *
 * @param[out]  filesys_id  A non-zero OSAL ID reflecting the file system
 * @param[in]   phys_path   The native system directory (an existing mount point)
 * @param[in]   virt_path   The virtual mount point of this filesystem
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_FileSysAddFixedMap(osal_id_t *filesys_id, const char *phys_path, const char *virt_path);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a file system on the target
 *
 * Makes a file system on the target.  Highly dependent on underlying OS and
 * dependent on OS volume table definition.
 *
 * @note The "volname" parameter of RAM disks should always begin with the string "RAM",
 *   e.g. "RAMDISK" or "RAM0","RAM1", etc if multiple devices are created.  The underlying
 *   implementation uses this to select the correct filesystem type/format, and this may
 *   also be used to differentiate between RAM disks and real physical disks.
 *
 * @param[in]   address   The address at which to start the new disk.  If address == 0
 *                        space will be allocated by the OS.
 * @param[in]   devname   The underlying kernel device to use, if applicable.
 * @param[in]   volname   The name of the volume (see note)
 * @param[in]   blocksize The size of a single block on the drive
 * @param[in]   numblocks The number of blocks to allocate for the drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if devname is NULL
 * @retval #OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
 * @retval #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * @retval #OS_SUCCESS on creating the disk
 */
int32 OS_mkfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks);
/*-------------------------------------------------------------------------------------*/
/**
 * @brief Mounts a file system
 *
 * Mounts a file system / block device at the given mount point.
 *
 * @param[in]   devname    The name of the drive to mount. devname is the same from #OS_mkfs
 * @param[in]   mountpoint The name to call this disk from now on
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_mount(const char *devname, const char *mountpoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initializes an existing file system
 *
 * Initializes a file system on the target.
 *
 * @note The "volname" parameter of RAM disks should always begin with the string "RAM",
 *   e.g. "RAMDISK" or "RAM0","RAM1", etc if multiple devices are created.  The underlying
 *   implementation uses this to select the correct filesystem type/format, and this may
 *   also be used to differentiate between RAM disks and real physical disks.
 *
 * @param[in]   address   The address at which to start the new disk.  If address == 0,
 *                        then space will be allocated by the OS
 * @param[in]   devname   The underlying kernel device to use, if applicable.
 * @param[in]   volname   The name of the volume (see note)
 * @param[in]   blocksize The size of a single block on the drive
 * @param[in]   numblocks The number of blocks to allocate for the drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if devname or volname are  NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * @retval #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * @retval #OS_FS_ERR_DRIVE_NOT_CREATED on error
 */
int32 OS_initfs(char *address, const char *devname, const char *volname, size_t blocksize, osal_blockcount_t numblocks);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file system
 *
 * This function will remove or un-map the target file system. Note that this is not
 * the same as un-mounting the file system.
 *
 * @param[in]   devname   The name of the "generic" drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if devname is NULL
 * @retval #OS_ERROR is the drive specified cannot be located
 */
int32 OS_rmfs(const char *devname);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unmounts a mounted file system
 *
 * This function will unmount a drive from the file system and make all open file
 * descriptors useless.
 *
 * @note Any open file descriptors referencing this file system should
 * be closed prior to unmounting a drive
 *
 * @param[in]   mountpoint The mount point to remove from #OS_mount
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if name is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the absolute path given is too long
 * @retval #OS_ERROR if the OS calls failed
 */
int32 OS_unmount(const char *mountpoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtains information about size and free space in a volume
 *
 * Populates the supplied OS_statvfs_t structure, which includes
 * the block size and total/free blocks in a file system volume.
 *
 * This replaces two older OSAL calls:
 *
 * OS_fsBlocksFree() is determined by reading the blocks_free
 *      output struct member
 * OS_fsBytesFree() is determined by multiplying blocks_free
 *      by the block_size member
 *
 * @param[in]  name       The device/path to operate on
 * @param[out] statbuf    Output structure to populate
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if name or statbuf is NULL
 * @retval #OS_ERROR if the OS call failed
 */
int32 OS_FileSysStatVolume(const char *name, OS_statvfs_t *statbuf);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Checks the health of a file system and repairs it if necessary
 *
 * Checks the drives for inconsistencies and optionally also repairs it
 *
 * @note not all operating systems implement this function
 *
 * @param[in]  name     The device/path to operate on
 * @param[in]  repair   Whether to also repair inconsistencies
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS             @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER     Name is NULL
 * @retval #OS_ERR_NOT_IMPLEMENTED @copybrief OS_ERR_NOT_IMPLEMENTED
 * @retval #OS_ERROR               @copybrief OS_ERROR
 */
int32 OS_chkfs(const char *name, bool repair);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtains the physical drive name associated with a mount point
 *
 * Returns the name of the physical volume associated with the drive,
 * when given the OSAL mount point of the drive
 *
 * @param[out] PhysDriveName    Buffer to store physical drive name
 * @param[in]  MountPoint       OSAL mount point
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if either parameter is NULL
 * @retval #OS_ERROR if the mountpoint could not be found
 */
int32 OS_FS_GetPhysDriveName(char *PhysDriveName, const char *MountPoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Translates a OSAL Virtual file system path to a host Local path
 *
 * Translates a virtual path to an actual system path name
 *
 * @param[in]  VirtualPath      OSAL virtual path name
 * @param[out] LocalPath        Buffer to store native/translated path name
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if either parameter is NULL
 */
int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Returns information about the file system
 *
 * Returns information about the file system in an os_fsinfo_t.
 * This includes the number of open files and file systems
 *
 * @param[out] filesys_info     Buffer to store filesystem information
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if filesys_info is NULL
 */
int32 OS_GetFsInfo(os_fsinfo_t *filesys_info);
/**@}*/

#endif /* OSAPI_FILESYS_H */
