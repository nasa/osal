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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-filesys.h"

#include "os-shared-filesys.h"

#include "OCS_stdlib.h"
#include "OCS_sys_ioctl.h"
#include "OCS_stat.h"
#include "OCS_fcntl.h"
#include "OCS_unistd.h"
#include "OCS_ramDrv.h"
#include "OCS_dosFsLib.h"
#include "OCS_xbdBlkDev.h"

void Test_OS_FileSysStartVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysStartVolume_Impl (uint32 filesys_id)
     */
    int32             expected;
    OS_object_token_t token;

    token = UT_TOKEN_0;

    /* Emulate an UNKNOWN entry */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_UNKNOWN;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), OS_ERR_NOT_IMPLEMENTED);

    /* Emulate an FS_BASED entry */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_FS_BASED;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), OS_SUCCESS);

    /* Emulate a VOLATILE_DISK entry (ramdisk) */
    OS_filesys_table[1].fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
    token.obj_idx              = UT_INDEX_1;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), OS_SUCCESS);

    /* Emulate a NORMAL_DISK entry (ATA) */
    OS_filesys_table[2].fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    /* the expected value depends on whether it is compiled with USE_VXWORKS_ATA_DRIVER */
#ifdef USE_VXWORKS_ATA_DRIVER
    expected = OS_SUCCESS;
#else
    expected = OS_ERR_NOT_IMPLEMENTED;
#endif
    token = UT_TOKEN_2;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), expected);

    /* Failure to create XBD layer */
    UT_SetDefaultReturnValue(UT_KEY(OCS_xbdBlkDevCreateSync), -1);
    token = UT_TOKEN_1;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), OS_FS_ERR_DRIVE_NOT_CREATED);

    /* Failure to create low level block dev */
    UT_SetDefaultReturnValue(UT_KEY(OCS_ramDevCreate), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(&token), OS_FS_ERR_DRIVE_NOT_CREATED);
}

void Test_OS_FileSysStopVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysStopVolume_Impl (uint32 filesys_id)
     */
    OS_object_token_t token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(&token), OS_SUCCESS);

    /* Failure to delete XBD layer */
    OS_filesys_table[1].fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
    UT_FileSysTest_SetupFileSysEntry(1, NULL, 1, 4);
    token = UT_TOKEN_1;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(&token), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_xbdBlkDevDelete)) == 1, "xbdBlkDevDelete() called");

    /* Test alternative branches */
    UT_ResetState(UT_KEY(OCS_xbdBlkDevDelete));
    UT_FileSysTest_SetupFileSysEntry(1, NULL, OCS_NULLDEV, 4);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(&token), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_xbdBlkDevDelete)) == 0, "xbdBlkDevDelete() not called");

    UT_FileSysTest_SetupFileSysEntry(1, NULL, 1, 0);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(&token), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_xbdBlkDevDelete)) == 0, "xbdBlkDevDelete() not called");
}

void Test_OS_FileSysFormatVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysFormatVolume_Impl (uint32 filesys_id)
     */
    OS_object_token_t token = UT_TOKEN_0;

    /* test unimplemented fs type */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_UNKNOWN;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(&token), OS_ERR_NOT_IMPLEMENTED);

    /* fs-based should be noop */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_FS_BASED;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(&token), OS_SUCCESS);

    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(&token), OS_SUCCESS);

    /* Failure of the dosFsVolFormat() call */
    UT_SetDefaultReturnValue(UT_KEY(OCS_dosFsVolFormat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(&token), OS_FS_ERR_DRIVE_NOT_CREATED);
}

void Test_OS_FileSysMountVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysMountVolume_Impl (uint32 filesys_id)
     */
    OS_object_token_t token = UT_TOKEN_0;
    struct OCS_stat   statbuf;

    memset(&OS_filesys_table[0], 0, sizeof(OS_filesys_table[0]));
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    strcpy(OS_filesys_table[0].system_mountpt, "/ut");

    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_open));

    /* Additional cases for the FS_BASED handling */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_FS_BASED;

    /* Mount dir does not exist but can be created */
    UT_SetDefaultReturnValue(UT_KEY(OCS_stat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_SUCCESS);

    /* Mount dir does not exist and cannot be created */
    UT_SetDeferredRetcode(UT_KEY(OCS_mkdir), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_FS_ERR_DRIVE_NOT_CREATED);

    /* Mount dir does exist but not a directory */
    UT_ClearDefaultReturnValue(UT_KEY(OCS_stat));
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_FS_ERR_PATH_INVALID);

    /* Mount dir does exist and is a directory */
    memset(&statbuf, 0, sizeof(statbuf));
    statbuf.st_mode = OCS_S_IFDIR;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &statbuf, sizeof(statbuf), false);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(&token), OS_SUCCESS);
}

void Test_OS_FileSysUnmountVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id)
     */
    OS_object_token_t token = UT_TOKEN_0;

    memset(&OS_filesys_table[0], 0, sizeof(OS_filesys_table[0]));
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    strcpy(OS_filesys_table[0].system_mountpt, "/ut");

    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(&token), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(&token), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_open));

    UT_SetDefaultReturnValue(UT_KEY(OCS_ioctl), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(&token), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_ioctl));

    /* Additional cases for the FS_BASED handling (no op on unmount) */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_FS_BASED;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(&token), OS_SUCCESS);
}

void Test_OS_FileSysStatVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result)
     */
    OS_statvfs_t      stat;
    OS_object_token_t token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_FileSysStatVolume_Impl(&token, &stat), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_statvfs), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStatVolume_Impl(&token, &stat), OS_ERROR);
}

void Test_OS_FileSysCheckVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair)
     */
    OS_object_token_t token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(&token, true), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(&token, false), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_open));

    UT_SetDefaultReturnValue(UT_KEY(OCS_ioctl), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(&token, false), OS_ERROR);
}

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    memset(OS_filesys_table, 0, sizeof(OS_filesys_table));
    memset(OS_global_filesys_table, 0, sizeof(OS_common_record_t) * OS_MAX_FILE_SYSTEMS);

    memset(UT_Ref_OS_impl_filesys_table, 0, UT_Ref_OS_impl_filesys_table_SIZE);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_FileSysStartVolume_Impl);
    ADD_TEST(OS_FileSysStopVolume_Impl);
    ADD_TEST(OS_FileSysFormatVolume_Impl);
    ADD_TEST(OS_FileSysCheckVolume_Impl);
    ADD_TEST(OS_FileSysStatVolume_Impl);
    ADD_TEST(OS_FileSysMountVolume_Impl);
    ADD_TEST(OS_FileSysUnmountVolume_Impl);
}
