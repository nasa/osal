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
 */
#include "os-shared-coveragetest.h"
#include "os-shared-filesys.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/
void Test_OS_FileSysAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual;

    actual = OS_FileSysAPI_Init();
    UtAssert_True(actual == expected, "OS_FileSysAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_FileSysAddFixedMap(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path, const char *virt_path)
     */
    osal_id_t id;

    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", NULL), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, NULL, "/virt"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(NULL, "/phys", "/virt"), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_FS_ERR_PATH_TOO_LONG);
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 2, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_FS_ERR_PATH_TOO_LONG);

    UT_SetDefaultReturnValue(UT_KEY(OCS_strrchr), -1);
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 3, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERR_NAME_TOO_LONG);
    UT_ResetState(UT_KEY(OCS_memchr));
    UT_ResetState(UT_KEY(OCS_strrchr));

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdAllocateNew), 1, OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERR_NO_FREE_IDS);
    UT_SetDeferredRetcode(UT_KEY(OS_FileSysStartVolume_Impl), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERROR);
    UT_SetDeferredRetcode(UT_KEY(OS_FileSysMountVolume_Impl), 1, OS_ERROR - 1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERROR - 1);

    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
}

void Test_OS_mkfs(void)
{
    /*
     * Test Case For:
     * int32 OS_mkfs (char *address, const char *devname, const char * volname,
     *          uint32 blocksize, uint32 numblocks)
     */
    char TestBuffer[128];

    /* Success case for existing entry */
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_SUCCESS);

    /*
     * Test an entry NOT found in the OS_VolumeTable
     */
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/rd1", "vol1", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_SUCCESS);

    /* NULL addr with RAM volume to cover branches */
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(NULL, "/rd1", "RAM1", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, NULL, "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", NULL, OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_INVALID_POINTER);

    /* First string check error */
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_PATH_TOO_LONG);

    /* Second string check error */
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 2, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_PATH_TOO_LONG);

    /* failure due to empty strings */
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_PATH_INVALID);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_PATH_INVALID);

    /* set up for failure due to formatting */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysFormatVolume_Impl), OS_FS_ERR_DRIVE_NOT_CREATED);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_DRIVE_NOT_CREATED);

    /* Start failure */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysStartVolume_Impl), OS_ERR_INCORRECT_OBJ_STATE);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_DEVICE_NOT_FREE);

    /* set up for failure due to no free slots */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_mkfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_DEVICE_NOT_FREE);
}

void Test_OS_rmfs(void)
{
    /*
     * Test Case For:
     * int32 OS_rmfs (const char *devname)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    actual = OS_rmfs("/ramdev5");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_SUCCESS", (long)actual);

    /* check error paths */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetByName), OS_ERR_NAME_NOT_FOUND);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_rmfs("/ramdev4");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdGetByName));

    expected = OS_INVALID_POINTER;
    actual   = OS_rmfs(NULL);
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual   = OS_rmfs("/ramdev4");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
}

void Test_OS_initfs(void)
{
    /*
     * Test Case For:
     * int32 OS_initfs (char *address,const char *devname, const char *volname,
     *          uint32 blocksize, uint32 numblocks)
     */
    char TestBuffer[128];

    OSAPI_TEST_FUNCTION_RC(OS_initfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_initfs(NULL, "/hda2", "vol2", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_initfs(NULL, NULL, NULL, OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_initfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_PATH_TOO_LONG);

    /* set up for failure */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdAllocateNew), 1, OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_initfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_DEVICE_NOT_FREE);

    /* Start failure */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysStartVolume_Impl), OS_ERR_INCORRECT_OBJ_STATE);
    OSAPI_TEST_FUNCTION_RC(OS_initfs(TestBuffer, "/ramdev0", "vol", OSAL_SIZE_C(0), OSAL_BLOCKCOUNT_C(0)),
                           OS_FS_ERR_DEVICE_NOT_FREE);
}

void Test_OS_mount(void)
{
    /*
     * Test Case For:
     * int32 OS_mount (const char *devname, const char* mountpoint)
     */
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev5", "/ram5"), OS_ERR_NAME_NOT_FOUND);

    /* Test unknown/unset system mountpoint */
    OS_filesys_table[1].flags             = OS_FILESYS_FLAG_IS_READY;
    OS_filesys_table[1].system_mountpt[0] = 0;
    /* should be OS_FS_ERR_PATH_INVALID, but compat return overwrites */
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev5", "/ram5"), OS_ERR_NAME_NOT_FOUND);

    /* set up so record is in the right state for mounting */
    snprintf(OS_filesys_table[1].system_mountpt, sizeof(OS_filesys_table[1].system_mountpt), "/ut");
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev5", "/ram5"), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_mount(NULL, "/ram5"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev5", NULL), OS_INVALID_POINTER);

    /* Path too long errors */
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev0", "/ram0"), OS_FS_ERR_PATH_TOO_LONG);
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 2, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev0", "/ram0"), OS_FS_ERR_PATH_TOO_LONG);

    /* Fail OS_ObjectIdGetByName */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetByName), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mount("/ramdev5", "/ram5"), OS_ERR_NAME_NOT_FOUND);
}

void Test_OS_unmount(void)
{
    /*
     * Test Case For:
     * int32 OS_unmount (const char *mountpoint)
     */
    OSAPI_TEST_FUNCTION_RC(OS_unmount("/ram0"), OS_ERR_NAME_NOT_FOUND);

    /* set up so record is in the right state for mounting */
    OS_filesys_table[1].flags =
        OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    OSAPI_TEST_FUNCTION_RC(OS_unmount("/ram0"), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_unmount(NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetBySearch), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_unmount("/ram0"), OS_ERR_NAME_NOT_FOUND);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_unmount("/ram0"), OS_FS_ERR_PATH_TOO_LONG);
}

void Test_OS_FileSysStatVolume(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysStatVolume(const char *name, OS_statvfs_t *statbuf)
     */

    OS_statvfs_t statbuf;
    OS_statvfs_t statref;
    int32        expected;
    int32        actual;

    memset(&statbuf, 0, sizeof(statbuf));

    statref.block_size   = OSAL_SIZE_C(1024);
    statref.blocks_free  = OSAL_BLOCKCOUNT_C(1111);
    statref.total_blocks = OSAL_BLOCKCOUNT_C(2222);
    UT_SetDataBuffer(UT_KEY(OS_FileSysStatVolume_Impl), &statref, sizeof(statref), false);
    OS_filesys_table[1].flags =
        OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

    expected = OS_SUCCESS;
    actual   = OS_FileSysStatVolume("/cf", &statbuf);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(statbuf.block_size == statref.block_size, "blocks_size (%lu) == %lu",
                  (unsigned long)statbuf.block_size, (unsigned long)statref.block_size);
    UtAssert_True(statbuf.total_blocks == statref.total_blocks, "total_blocks (%lu) == %lu",
                  (unsigned long)statbuf.total_blocks, (unsigned long)statref.total_blocks);
    UtAssert_True(statbuf.blocks_free == statref.blocks_free, "blocks_free (%lu) == %lu",
                  (unsigned long)statbuf.blocks_free, (unsigned long)statref.blocks_free);

    /* validate error checking */
    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysStatVolume(NULL, &statbuf);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_INVALID_POINTER", (long)actual);
    actual = OS_FileSysStatVolume("/cf", NULL);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* Test Fail due to no matching VolTab entry */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_FileSysStatVolume("/cf", &statbuf);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
    UT_ResetState(UT_KEY(OS_ObjectIdGetBySearch));

    /* Verify pass through of impl error */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysStatVolume_Impl), OS_ERR_OPERATION_NOT_SUPPORTED);
    expected = OS_ERR_OPERATION_NOT_SUPPORTED;
    actual   = OS_FileSysStatVolume("/cf", &statbuf);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_ERR_OPERATION_NOT_SUPPORTED", (long)actual);
    UT_ResetState(UT_KEY(OS_FileSysStatVolume_Impl));

    /* Verify OS_FS_ERR_PATH_TOO_LONG */
    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), -1);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual   = OS_FileSysStatVolume("/cf", &statbuf);
    UtAssert_True(actual == expected, "OS_FileSysStatVolume() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ResetState(UT_KEY(OCS_memchr));
}

void Test_OS_chkfs(void)
{
    /*
     * Test Case For:
     * int32 OS_chkfs (const char *name, bool repair)
     */
    int32 expected;
    int32 actual;

    /* Setup for success by having strings match */
    expected = OS_SUCCESS;
    actual   = OS_chkfs("/cf", true);
    UtAssert_True(actual == expected, "OS_chkfs() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_chkfs(NULL, false);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual   = OS_chkfs("/cf", false);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    /* Test Fail due to no matching VolTab entry */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_chkfs("none", true);
    UtAssert_True(actual == expected, "OS_chkfs() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
}

void Test_OS_FS_GetPhysDriveName(void)
{
    /*
     * Test Case For:
     * int32 OS_FS_GetPhysDriveName(char * PhysDriveName, const char * MountPoint)
     */
    char NameBuf[OS_FS_PHYS_NAME_LEN];

    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NULL, "none"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NameBuf, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NameBuf, "none"), OS_FS_ERR_PATH_TOO_LONG);

    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NameBuf, "none"), OS_ERR_INCORRECT_OBJ_STATE);

    OS_filesys_table[1].flags =
        OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NameBuf, "none"), OS_SUCCESS);

    /* Test Fail due to no matching VolTab entry */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    OSAPI_TEST_FUNCTION_RC(OS_FS_GetPhysDriveName(NameBuf, "none"), OS_ERR_NAME_NOT_FOUND);
}

void Test_OS_GetFsInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_GetFsInfo(OS_FsInfo_t  *filesys_info)
     */
    int32              expected = OS_SUCCESS;
    int32              actual   = ~OS_SUCCESS;
    os_fsinfo_t        filesys_info;
    OS_common_record_t rec;

    memset(&filesys_info, 0, sizeof(filesys_info));

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdIteratorGetNext), 1);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdIteratorGetNext), 3, 0);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdIteratorGetNext), 4, 0);

    actual = OS_GetFsInfo(&filesys_info);

    UtAssert_True(actual == expected, "OS_FileSysInfo() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(filesys_info.MaxFds == OS_MAX_NUM_OPEN_FILES, "filesys_info.MaxFds (%lu) == OS_MAX_NUM_OPEN_FILES",
                  (unsigned long)filesys_info.MaxFds);
    UtAssert_True(filesys_info.MaxVolumes == OS_MAX_FILE_SYSTEMS,
                  "filesys_info.MaxVolumes (%lu) == OS_MAX_FILE_SYSTEMS", (unsigned long)filesys_info.MaxVolumes);

    /* since there are no open files, the free fd count should match the max */
    UtAssert_True(filesys_info.FreeFds == 2, "filesys_info.FreeFds (%lu) == 2", (unsigned long)filesys_info.FreeFds);

    UtAssert_True(filesys_info.FreeVolumes == 3, "filesys_info.FreeVolumes (%lu) == 3",
                  (unsigned long)filesys_info.FreeVolumes);

    expected = OS_INVALID_POINTER;
    actual   = OS_GetFsInfo(NULL);
    UtAssert_True(actual == expected, "OS_GetFsInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* This function uses a helper OS_FileSysFilterFree() that needs to be called for coverage. */
    /* It is just a wrapper around OS_ObjectIdDefined() for the record ID */
    memset(&rec, 0, sizeof(rec));
    UtAssert_True(OS_FileSysFilterFree(NULL, NULL, &rec), "OS_FileSysFilterFree() (unused record)");
    rec.active_id = UT_OBJID_1;
    UtAssert_True(!OS_FileSysFilterFree(NULL, NULL, &rec), "!OS_FileSysFilterFree() (used record)");
}

void Test_OS_TranslatePath(void)
{
    /*
     * Test Case For:
     * int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
     */
    char  LocalBuffer[OS_MAX_PATH_LEN];
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    memset(LocalBuffer, 0, sizeof(LocalBuffer));

    /* Set up the local record for success */
    OS_filesys_table[1].flags =
        OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    strcpy(OS_filesys_table[1].virtual_mountpt, "/cf");
    strcpy(OS_filesys_table[1].system_mountpt, "/mnt/cf");

    actual = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(LocalBuffer, "/mnt/cf/test") == 0, "OS_TranslatePath(/cf/test) (%s)  == /mnt/cf/test",
                  LocalBuffer);

    /* Check various error paths */
    UtAssert_INT32_EQ(OS_TranslatePath("/cf/test", NULL), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_TranslatePath(NULL, LocalBuffer), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual   = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    /* Invalid no '/' */
    expected = OS_FS_ERR_PATH_INVALID;
    actual   = OS_TranslatePath("invalid", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 2, OS_ERROR);
    expected = OS_FS_ERR_NAME_TOO_LONG;
    actual   = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_FS_ERR_NAME_TOO_LONG", (long)actual);

    /* Invalid no leading '/' */
    expected = OS_FS_ERR_PATH_INVALID;
    actual   = OS_TranslatePath("invalid/", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    actual = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdGetBySearch));

    /* VirtPathLen < VirtPathBegin */
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 4, OS_ERROR);
    expected = OS_FS_ERR_PATH_INVALID;
    actual   = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    /* (SysMountPointLen + VirtPathLen) > OS_MAX_LOCAL_PATH_LEN */
    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 3, OS_ERROR);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual   = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);

    OS_filesys_table[1].flags = 0;
    expected                  = OS_ERR_INCORRECT_OBJ_STATE;
    actual                    = OS_TranslatePath("/cf/test", LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_FileSys_FindVirtMountPoint(void)
{
    /*
     * Test Case For:
     * static bool OS_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj)
     *
     * This is a static internal function and must be invoked through a UT-specific wrapper in
     * order to get coverage on it.
     */
    bool               result;
    OS_common_record_t refobj;
    const char         refstr[]  = "/ut";
    const char         refstr1[] = "/ut/";
    OS_object_token_t  token;

    memset(&token, 0, sizeof(token));
    token.obj_idx  = UT_INDEX_1;
    token.obj_type = OS_OBJECT_TYPE_OS_FILESYS;

    memset(&refobj, 0, sizeof(refobj));
    OS_filesys_table[1].flags              = 0;
    OS_filesys_table[1].virtual_mountpt[0] = 0;

    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (unmounted) == false", refstr);

    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

    /* Branch coverage for mismatches */
    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr,
                  OS_filesys_table[1].virtual_mountpt);

    memset(OS_filesys_table[1].virtual_mountpt, 'a', sizeof(OS_filesys_table[1].virtual_mountpt));
    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr,
                  OS_filesys_table[1].virtual_mountpt);

    /* Verify cases where one is a substring of the other -
     * these should also return false */
    strncpy(OS_filesys_table[1].virtual_mountpt, "/ut11", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr,
                  OS_filesys_table[1].virtual_mountpt);

    strncpy(OS_filesys_table[1].virtual_mountpt, "/u", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr,
                  OS_filesys_table[1].virtual_mountpt);

    strncpy(OS_filesys_table[1].virtual_mountpt, "/ut", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = OS_FileSys_FindVirtMountPoint((void *)refstr, &token, &refobj);
    UtAssert_True(result, "OS_FileSys_FindVirtMountPoint(%s) (nominal) == true", refstr);

    /* Passing case with reference ending in "/" */
    strncpy(OS_filesys_table[1].virtual_mountpt, "/ut", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = OS_FileSys_FindVirtMountPoint((void *)refstr1, &token, &refobj);
    UtAssert_True(result, "OS_FileSys_FindVirtMountPoint(%s) (nominal) == true", refstr);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(OS_filesys_table, 0, sizeof(OS_filesys_table));
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_FileSysAPI_Init);
    ADD_TEST(OS_FileSysAddFixedMap);
    ADD_TEST(OS_mkfs);
    ADD_TEST(OS_rmfs);
    ADD_TEST(OS_initfs);
    ADD_TEST(OS_mount);
    ADD_TEST(OS_unmount);
    ADD_TEST(OS_chkfs);
    ADD_TEST(OS_FS_GetPhysDriveName);
    ADD_TEST(OS_GetFsInfo);
    ADD_TEST(OS_TranslatePath);
    ADD_TEST(OS_FileSys_FindVirtMountPoint);
    ADD_TEST(OS_FileSysStatVolume);
}
