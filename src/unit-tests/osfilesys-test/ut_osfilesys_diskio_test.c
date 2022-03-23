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

/*================================================================================*
** File:  ut_osfilesys_diskio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfilesys_diskio_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern char *g_fsAddrPtr;

extern size_t            g_blkSize;
extern osal_blockcount_t g_blkCnt;

extern char g_fsLongName[UT_OS_PATH_BUFF_SIZE];
extern char g_physDriveName[UT_OS_PHYS_NAME_BUFF_SIZE];

extern char g_volNames[UT_OS_FILESYS_LIST_LEN][UT_OS_NAME_BUFF_SIZE];
extern char g_devNames[UT_OS_FILESYS_LIST_LEN][UT_OS_FILE_BUFF_SIZE];
extern char g_mntNames[UT_OS_FILESYS_LIST_LEN][UT_OS_FILE_BUFF_SIZE];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_initfs(char *address, char *devname, char *volname, uint32 blocksize, uint32 numblocks)
** Purpose: Initializes (without re-formatting) a drive on the target without
**          erasing the existing file system
** Parameters: *address - a pointer to the address at which to start the new disk
**             *devname - a pointer to the name of the "generic" device
**             *volname - a pointer to the name of the volume (only used in vxWorks)
**             blocksize - size of a single block on the drive
**             numblocks - the number of blocks to be allocated for the drive
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the device name or volume name passed in is too long
**          OS_FS_ERR_DEVICE_NOT_FREE if the Volume table is full
**          OS_FS_ERR_DRIVE_NOT_CREATED if the volume is not FS-BASED
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_DRIVE_NOT_CREATED
** -----------------------------------------------------
** Test #4: Disk-full condition
**   1) Call this routine (OS_MAX_FILE_SYSTEMS+1) of times
**   2) Expect the returned value to be (except the last call)
**        (a) OS_SUCCESS
**   3) Expect the returned value of the last call to be
**        (a) OS_FS_ERR_DEVICE_NOT_FREE
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_SUCCESS
**   4) Call OS_rmfs with device name used in #1 as argument
**   5) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_initfs_test()
{
    int32 i = 0, j = 0;

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_initfs(NULL, NULL, NULL, g_blkSize, g_blkCnt)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_initfs(g_fsAddrPtr, NULL, g_volNames[1], OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);
    UT_RETVAL(OS_initfs(g_fsAddrPtr, g_devNames[1], NULL, OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_initfs(g_fsAddrPtr, g_fsLongName, g_volNames[2], g_blkSize, g_blkCnt), OS_FS_ERR_PATH_TOO_LONG);
    UT_RETVAL(OS_initfs(g_fsAddrPtr, g_devNames[2], g_fsLongName, g_blkSize, g_blkCnt), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 OS-call-failure */

    /*-----------------------------------------------------*/
    /* #4 Disk-full */

    for (i = 0; i <= OS_MAX_FILE_SYSTEMS; i++)
    {
        memset(g_devNames[i], '\0', sizeof(g_devNames[i]));
        UT_os_sprintf(g_devNames[i], "/ramdev%d", (int)i);
        memset(g_volNames[i], '\0', sizeof(g_volNames[i]));
        UT_os_sprintf(g_volNames[i], "RAM%d", (int)i);

        /* Only need to check the last call to OS_initfs() */
        if (i == OS_MAX_FILE_SYSTEMS)
        {
            UT_RETVAL(OS_initfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt),
                      OS_FS_ERR_DEVICE_NOT_FREE);
        }
        else if (!UT_SETUP(OS_initfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt)))
        {
            break;
        }
    }

    /* Reset test environment */
    for (j = 0; j < OS_MAX_FILE_SYSTEMS; j++)
    {
        UT_TEARDOWN(OS_rmfs(g_devNames[j]));
    }

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    UT_NOMINAL(OS_initfs(g_fsAddrPtr, g_devNames[5], g_volNames[5], g_blkSize, g_blkCnt));
    UT_TEARDOWN(OS_rmfs(g_devNames[5]));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mkfs(char *address, char *devname, char *volname, uint32 blocksize, uint32 numblocks)
** Purpose: Formats a volume with a file system
** Parameters: *address - a pointer to the address at which to start the new disk
**             *devname - a pointer to the name of the "generic" device
**             *volname - a pointer to the name of the volume (only used in vxWorks)
**             blocksize - the size of a single block on the drive
**             numblocks - the number of blocks to be allocated for the drive
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the name passed in is too long
**          OS_FS_ERR_DRIVE_NOT_CREATED if the OS call failed
**          OS_FS_ERR_DEVICE_NOT_FREE if the Volume table is full
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_mkfs() is implemented, then OS_rmfs() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_DRIVE_NOT_CREATED
** -----------------------------------------------------
** Test #4: Disk-full condition
**   1) Call this routine (OS_MAX_FILE_SYSTEMS+1) of times
**   2) Expect the returned value to be (except the last call)
**        (a) OS_SUCCESS
**   3) Expect the returned value of the last call to be
**        (a) OS_FS_ERR_DEVICE_NOT_FREE
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_SUCCESS
**   4) Call OS_rmfs with device name used in #1 as argument
**   5) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_makefs_test()
{
    int32 i = 0, j = 0;

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_mkfs(g_fsAddrPtr, NULL, NULL, g_blkSize, g_blkCnt)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_mkfs(g_fsAddrPtr, NULL, g_volNames[1], OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);
    UT_RETVAL(OS_mkfs(g_fsAddrPtr, g_devNames[1], NULL, OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_mkfs(g_fsAddrPtr, g_fsLongName, g_volNames[2], g_blkSize, g_blkCnt), OS_FS_ERR_PATH_TOO_LONG);
    UT_RETVAL(OS_mkfs(g_fsAddrPtr, g_devNames[2], g_fsLongName, g_blkSize, g_blkCnt), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Disk-full */

    for (i = 0; i <= OS_MAX_FILE_SYSTEMS; i++)
    {
        memset(g_devNames[i], '\0', sizeof(g_devNames[i]));
        UT_os_sprintf(g_devNames[i], "/ramdev%d", (int)i);
        memset(g_volNames[i], '\0', sizeof(g_volNames[i]));
        UT_os_sprintf(g_volNames[i], "RAM%d", (int)i);

        /* Only need to check the last call to OS_mkfs() */
        if (i == OS_MAX_FILE_SYSTEMS)
        {
            UT_RETVAL(OS_mkfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt),
                      OS_FS_ERR_DEVICE_NOT_FREE);
        }
        else if (!UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt)))
        {
            break;
        }
    }

    /* Reset test environment */
    for (j = 0; j < OS_MAX_FILE_SYSTEMS; j++)
    {
        UT_TEARDOWN(OS_rmfs(g_devNames[j]));
    }

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    UT_NOMINAL(OS_mkfs(g_fsAddrPtr, g_devNames[5], g_volNames[5], g_blkSize, g_blkCnt));

    UT_TEARDOWN(OS_rmfs(g_devNames[5]));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_rmfs(char *devname)
** Purpose: Removes or un-maps the target file system
** Parameters: *devname - a pointer to the name of the "generic" drive
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_ERR_NAME_NOT_FOUND if the given device is not found in the Volume table
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_rmfs() is implemented, then OS_mkfs() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-device-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some device name as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_mkfs to create a file system again exactly as in #1
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_removefs_test()
{
    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_rmfs(NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_rmfs(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-device-arg */

    UT_RETVAL(OS_rmfs(g_devNames[2]), OS_ERR_NAME_NOT_FOUND);
    UT_RETVAL(OS_rmfs(g_fsLongName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    UT_NOMINAL(OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt));

    UT_NOMINAL(OS_rmfs(g_devNames[3]));

    UT_NOMINAL(OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt));

    /* Reset test environment */
    UT_TEARDOWN(OS_rmfs(g_devNames[3]));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mount(const char *devname, char *mountpoint)
** Purpose: Mounts a disk volume to the file system tree
** Parameters: *devname - a pointer to the name of the drive to mount
**             *mountpoint - a pointer to the name to call this disk from now on
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_ERR_NAME_NOT_FOUND if the given device is not found in the Volume table
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_mount() is implemented, then
**            OS_mkfs(), OS_rmfs and OS_unmount should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-device-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some device name as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call this routine again exactly as in #3
**   6) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
**   7) Call OS_unmount with the mount-point used in #3 as argument
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_mount_test()
{
    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_mount(NULL, NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_mount(NULL, g_mntNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_mount(g_devNames[1], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-device-arg */

    UT_RETVAL(OS_mount("ramdev0", g_mntNames[2]), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt)))
    {
        UT_NOMINAL(OS_mount(g_devNames[3], g_mntNames[3]));
        UT_RETVAL(OS_mount(g_devNames[3], g_mntNames[3]), OS_ERR_NAME_NOT_FOUND);
        UT_RETVAL(OS_mount(g_devNames[3], g_fsLongName), OS_FS_ERR_PATH_TOO_LONG);

        /* Reset test environment */
        UT_TEARDOWN(OS_unmount(g_mntNames[3]));
        UT_TEARDOWN(OS_rmfs(g_devNames[3]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_unmount(const char *mountpoint)
** Purpose: Un-mounts a drive from the file system and makes all open file descriptors
**          obsolete
** Parameters: *mountpoint - a pointer to the name of the drive to unmount
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the absolute path passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the mount-point passed in is not found in the Volume table
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_unmount() is implemented, then
**            OS_mkfs(), OS_rmfs() and OS_mount() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some mount-point name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-mount-point-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some mount-point name as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_mount to mount the device to a mount-point
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call this routine with the mount-point used in #3 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call this routine again exactly as in #5
**   8) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
**--------------------------------------------------------------------------------*/
void UT_os_unmount_test()
{
    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_unmount(NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_unmount(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_unmount(g_fsLongName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-mount-point-arg */

    UT_RETVAL(OS_unmount(g_mntNames[3]), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt)))
    {
        UT_NOMINAL(OS_mount(g_devNames[4], g_mntNames[4]));
        UT_NOMINAL(OS_unmount(g_mntNames[4]));
        UT_RETVAL(OS_unmount(g_mntNames[4]), OS_ERR_NAME_NOT_FOUND);

        /* Reset test environment */
        UT_TEARDOWN(OS_rmfs(g_devNames[4]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FS_GetPhysDriveName(char *PhysDriveName, char *MountPoint)
** Purpose: Returns the name of the physical drive of a given mount-point
** Parameters: *PhysDriveName - a pointer that will hold the name of the physical drive
**             *MountPoint - a pointer to the name of the mount-point
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the mount-point passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the mount-point passed in is not found in the Volume table
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_unmount() is implemented, then
**            OS_mkfs(), OS_rmfs() and OS_mount() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some mount-point name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-mount-point-arg condition
**   1) Call OS_mkfs
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine
**   4) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_mkfs
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_mount with device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call this routine with the device name used in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
** --------------------------------------------------------------------------------*/
void UT_os_getphysdrivename_test()
{
    char physDevName[UT_OS_PHYS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_FS_GetPhysDriveName(NULL, NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_FS_GetPhysDriveName(NULL, g_mntNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_FS_GetPhysDriveName(physDevName, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_FS_GetPhysDriveName(physDevName, g_fsLongName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-mount-point-arg */

    UT_RETVAL(OS_FS_GetPhysDriveName(physDevName, g_mntNames[3]), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt)))
    {
        if (UT_SETUP(OS_mount(g_devNames[4], g_mntNames[4])))
        {
            memset(physDevName, '\0', sizeof(physDevName));
            UT_NOMINAL(OS_FS_GetPhysDriveName(physDevName, g_mntNames[4]));

            /* This just checks that the output string is not empty */
            UtAssert_True(physDevName[0] != 0, "physDevName (%s)", physDevName);

            UT_TEARDOWN(OS_unmount(g_mntNames[4]));
        }

        UT_TEARDOWN(OS_rmfs(g_devNames[4]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_GetFsInfo(os_fsinfo_t* filesys_info)
** Purpose: Returns information about the file system
** Parameters: filesys_info - out pointer contains info. about the file system
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine with a valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
** --------------------------------------------------------------------------------*/
void UT_os_getfsinfo_test(void)
{
    os_fsinfo_t fsInfo;

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_GetFsInfo(&fsInfo)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_GetFsInfo(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Nominal */

    UT_NOMINAL(OS_GetFsInfo(&fsInfo));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
** Purpose: Translates the given virtual path to the real path
** Parameters: *VirtualPath - pointer to the name of the virtual path or mount point
**             *LocalPath - pointer that will hold the name of the local path or
**                          physical device name
** Returns: OS_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the device name or volume name passed in is too long
**          OS_FS_ERR_PATH_INVALID if the virtual path passed in is not in correct format, or
**                                 virtual path name not found in the Volume table
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-virtual-path-arg condition
**   1) Call this routine with an incorrectly formatted virtual path name as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call OS_mkfs
**   3) Expect the returned value to be
**        (a) OS_SUCCESS
**   4) Call OS_mount with device name used in #2 as argument
**   5) Expect the returned value to be
**        (a) OS_SUCCESS
**   6) Call this routine with the mount-point used in #4 as argument
**   7) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) the returned local path to be ?
** --------------------------------------------------------------------------------*/
void UT_os_translatepath_test()
{
    char localPath[UT_OS_LOCAL_PATH_BUFF_SIZE];
    char virtPath[OS_MAX_PATH_LEN];

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_TranslatePath(NULL, localPath), OS_INVALID_POINTER);
    UT_RETVAL(OS_TranslatePath(g_mntNames[1], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_TranslatePath(g_fsLongName, localPath), OS_FS_ERR_PATH_TOO_LONG);

    /* create a path where only the name part is too long */
    memset(virtPath, 'z', sizeof(virtPath) - 1);
    virtPath[sizeof(virtPath) - 1] = 0;
    virtPath[0]                    = '/';
    UT_RETVAL(OS_TranslatePath(virtPath, localPath), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-virtual-path-arg */

    UT_RETVAL(OS_TranslatePath("cf", localPath), OS_FS_ERR_PATH_INVALID);
    UT_RETVAL(OS_TranslatePath("/foobar", localPath), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt)))
    {
        if (UT_SETUP(OS_mount(g_devNames[4], g_mntNames[4])))
        {
            memset(localPath, 0, sizeof(localPath));

            UT_NOMINAL(OS_TranslatePath(g_mntNames[4], localPath));

            /* This just checks that the output string is not empty */
            UtAssert_True(localPath[0] != 0, "localPath (%s)", localPath);

            /* Reset test environment */
            UT_TEARDOWN(OS_unmount(g_mntNames[4]));
        }
        /* Reset test environment */
        UT_TEARDOWN(OS_rmfs(g_devNames[4]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: os_fshealth_t OS_chkfs(const char *name, bool repair)
** Purpose: Checks the integrity of the file system, and may or may not repair it,
**          depending on repair
** Parameters: *name - the name of the drive to check
**             repair - bool flag to repair or not to repair
** Returns: OS_INVALID_POINTER if the pointer passed in is NULL
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some drive name of length greater than
**      maximum path length defined in macro
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Currently only applicable to vxworks platform
** --------------------------------------------------------------------------------*/
void UT_os_checkfs_test()
{
    char driveName[UT_OS_PATH_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_TranslatePath(NULL, NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_chkfs(NULL, 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    memset(driveName, 'A', sizeof(driveName));
    driveName[sizeof(driveName) - 1] = '\0';

    UT_RETVAL(OS_chkfs(driveName, 0), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 OS-call-failure */

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[5], g_volNames[5], g_blkSize, g_blkCnt)))
    {
        if (UT_SETUP(OS_mount(g_devNames[5], g_mntNames[5])))
        {
            UT_NOMINAL_OR_NOTIMPL(OS_chkfs(g_mntNames[5], 0));

            /* Reset test environment */
            UT_TEARDOWN(OS_unmount(g_mntNames[5]));
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_rmfs(g_devNames[5]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_fsstatvolume(const char *name)
** Purpose: Returns the number of blocks free in the file system
** Parameters: *name - a pointer to the name of the drive to check for free blocks
** Returns: OS_INVALID_POINTER if the pointer passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the path passed in is too long
**          OS_ERROR if the OS call failed
**          Number of blocks free in a volume if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a path name of length greater than Volume table's
**      name as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test#4: Nominal condition
**   1) Make sure no file system has been previously created
**   2) Call OS_mkfs
**   3) Expect the returned value to be
**        (a) OS_SUCCESS
**   4) Call OS_mount with device name used in #2
**   5) Expect the returned value to be
**        (a) OS_SUCCESS
**   6) Call this routine with mount-point used in #4
**   7) Expect the returned value to be
**        (a) greater than or equal to 0
** --------------------------------------------------------------------------------*/
void UT_os_fsstatvolume_test(void)
{
    OS_statvfs_t statbuf;

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_FileSysStatVolume("/cf", &statbuf)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1a Null-pointer-arg */

    UT_RETVAL(OS_FileSysStatVolume(NULL, &statbuf), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #1b Null-pointer-arg */

    UT_RETVAL(OS_FileSysStatVolume("/cf", NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_FileSysStatVolume(g_fsLongName, &statbuf), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 OS-call-failure */

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt)))
    {
        if (UT_SETUP(OS_mount(g_devNames[4], g_mntNames[4])))
        {
            UT_NOMINAL(OS_FileSysStatVolume(g_mntNames[4], &statbuf));

            /* Reset test environment */
            UT_TEARDOWN(OS_unmount(g_mntNames[4]));
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_rmfs(g_devNames[4]));
    }
}

/*================================================================================*
** End of File: ut_osfilesys_diskio_test.c
**================================================================================*/
