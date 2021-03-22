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
 * \file     coveragetest-file.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-file.h"
#include "os-shared-idmap.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_FileAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_FileAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FileAPI_Init();

    UtAssert_True(actual == expected, "OS_FileAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_OpenCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_OpenCreate(osal_id_t *filedes, const char *path, int32 flags, int32 access)
     */
    int32     expected;
    int32     actual;
    osal_id_t filedes;

    /* Test in OS_creat mode */
    expected = OS_SUCCESS;
    actual   = OS_OpenCreate(&filedes, "/cf/file", OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(actual == expected, "OS_OpenCreate() (%ld) == OS_SUCCESS (create mode)", (long)actual);

    /* Test in OS_open mode */
    actual = OS_OpenCreate(&filedes, "/cf/file", OS_FILE_FLAG_NONE, OS_READ_WRITE);
    UtAssert_True(actual == expected, "OS_OpenCreate() (%ld) == OS_SUCCESS (open mode)", (long)actual);

    /* Test with bad descriptor buffer */
    expected = OS_INVALID_POINTER;
    actual   = OS_OpenCreate(NULL, "/cf/file", OS_FILE_FLAG_NONE, OS_READ_WRITE);
    UtAssert_True(actual == expected, "OS_OpenCreate() (%ld) == OS_INVALID_POINTER (bad buffer)", (long)actual);

    /* Test with bad access flags */
    expected = OS_ERROR;
    actual   = OS_OpenCreate(&filedes, "/cf/file", OS_FILE_FLAG_NONE, 9999);
    UtAssert_True(actual == expected, "OS_OpenCreate() (%ld) == OS_ERROR (bad flags)", (long)actual);

    /* Test failure to convert path */
    UT_SetDefaultReturnValue(UT_KEY(OS_TranslatePath), OS_ERROR);
    expected = OS_ERROR;
    actual   = OS_OpenCreate(&filedes, "/cf/file", OS_FILE_FLAG_NONE, OS_READ_WRITE);
    UtAssert_True(actual == OS_ERROR, "OS_OpenCreate() (%ld) == OS_ERROR (bad path)", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OS_TranslatePath));
}

void Test_OS_close(void)
{
    /*
     * Test Case For:
     * int32 OS_close (uint32 filedes)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_close(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_close() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_TimedRead(void)
{
    /*
     * Test Case For:
     * int32 OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout)
     */
    char  Buf[4]    = "zzz";
    char  SrcBuf[8] = "rrrrrrr";
    int32 expected  = sizeof(Buf);
    int32 actual    = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), SrcBuf, sizeof(SrcBuf), false);
    actual = OS_TimedRead(UT_OBJID_1, Buf, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf, SrcBuf, actual) == 0, "buffer content match");

    expected = OS_INVALID_POINTER;
    actual   = OS_TimedRead(UT_OBJID_1, NULL, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld", (long)actual, (long)expected);
}

void Test_OS_TimedWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout)
     */
    const char Buf[4]    = "www";
    char       DstBuf[8] = "zzz";
    int32      expected  = sizeof(Buf);
    int32      actual    = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), DstBuf, sizeof(DstBuf), false);
    actual = OS_TimedWrite(UT_OBJID_1, Buf, sizeof(Buf), 10);

    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf, DstBuf, actual) == 0, "buffer content match");

    expected = OS_INVALID_POINTER;
    actual   = OS_TimedWrite(UT_OBJID_1, NULL, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld", (long)actual, (long)expected);
}

void Test_OS_read(void)
{
    /*
     * Test Case For:
     * int32 OS_read  (uint32  filedes, void *buffer, uint32 nbytes)
     */
    char  Buf[4]    = "zzz";
    char  SrcBuf[8] = "RRRRRRR";
    int32 expected  = sizeof(Buf);
    int32 actual    = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), SrcBuf, sizeof(SrcBuf), false);
    actual = OS_read(UT_OBJID_1, Buf, sizeof(Buf));
    UtAssert_True(actual == expected, "OS_read() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf, SrcBuf, actual) == 0, "buffer content match");
}

void Test_OS_write(void)
{
    /*
     * Test Case For:
     * int32 OS_write (uint32  filedes, const void *buffer, uint32 nbytes)
     */
    const char Buf[4]    = "WWW";
    char       DstBuf[8] = "zzz";
    int32      expected  = sizeof(Buf);
    int32      actual    = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), DstBuf, sizeof(DstBuf), false);
    actual = OS_write(UT_OBJID_1, Buf, sizeof(Buf));

    UtAssert_True(actual == expected, "OS_write() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf, DstBuf, actual) == 0, "buffer content match");
}

void Test_OS_chmod(void)
{
    /*
     * Test Case For:
     * int32 OS_chmod  (const char *path, uint32 access)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_chmod("/cf/file", 0);

    UtAssert_True(actual == expected, "OS_chmod() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_stat(void)
{
    /*
     * Test Case For:
     * int32 OS_stat   (const char *path, OS_FileStat_t *filestats)
     */
    int32      expected = OS_SUCCESS;
    os_fstat_t statbuf;
    int32      actual = OS_stat("/cf/file", &statbuf);

    UtAssert_True(actual == expected, "OS_stat() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_stat("/cf/file", NULL);
    UtAssert_True(actual == expected, "OS_stat() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_lseek(void)
{
    /*
     * Test Case For:
     * int32 OS_lseek  (uint32  filedes, int32 offset, uint32 whence)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_lseek(UT_OBJID_1, 0, 0);

    UtAssert_True(actual == expected, "OS_lseek() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_remove(void)
{
    /*
     * Test Case For:
     * int32 OS_remove (const char *path)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_remove("/cf/file");

    UtAssert_True(actual == expected, "OS_remove() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_rename(void)
{
    /*
     * Test Case For:
     * int32 OS_rename (const char *old, const char *new)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    OS_UT_SetupIterator(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, 1);
    strncpy(OS_stream_table[1].stream_name, "/cf/file1", sizeof(OS_stream_table[1].stream_name));
    actual = OS_rename("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_rename() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(OS_stream_table[1].stream_name, "/cf/file2") == 0,
                  "OS_stream_table[1].stream_name (%s) == /cf/file2", OS_stream_table[1].stream_name);
}

void Test_OS_cp(void)
{
    /*
     * Test Case For:
     * int32 OS_cp (const char *src, const char *dest)
     */
    int32 expected                  = OS_INVALID_POINTER;
    int32 actual                    = OS_cp(NULL, NULL);
    char  ReadBuf[]                 = "cpcpcpcp";
    char  WriteBuf[sizeof(ReadBuf)] = "";

    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_INVALID_POINTER", (long)actual);

    /* setup to make internal copy loop execute at least once */
    expected = OS_SUCCESS;
    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), ReadBuf, sizeof(ReadBuf), false);
    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), WriteBuf, sizeof(WriteBuf), false);
    actual = OS_cp("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_SUCCESS", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_GenericRead_Impl), -444);
    expected = -444;
    actual   = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == -444", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OS_GenericRead_Impl));

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), ReadBuf, sizeof(ReadBuf), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_GenericWrite_Impl), -555);
    expected = -555;
    actual   = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == -555", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_TranslatePath), OS_INVALID_POINTER);
    expected = OS_INVALID_POINTER;
    actual   = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_INVALID_POINTER", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OS_TranslatePath));
}

void Test_OS_mv(void)
{
    /*
     * Test Case For:
     * int32 OS_mv (const char *src, const char *dest)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_mv("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_mv() (%ld) == OS_SUCCESS", (long)actual);

    /* In the default case, the implementation tries to rename first.
     * Force rename to fail so it does a full copy and remove */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileRename_Impl), OS_ERROR);
    actual = OS_mv("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_mv() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_FDGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop)
     */
    int32          expected = OS_SUCCESS;
    int32          actual   = ~OS_SUCCESS;
    OS_file_prop_t file_prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    actual = OS_FDGetInfo(UT_OBJID_1, &file_prop);

    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(file_prop.Path, "ABC") == 0, "file_prop.Path (%s) == ABC", file_prop.Path);

    expected = OS_INVALID_POINTER;
    actual   = OS_FDGetInfo(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_FileOpenCheck(void)
{
    /*
     * Test Case For:
     * int32 OS_FileOpenCheck(const char *Filename)
     */
    int32 expected = OS_ERROR;
    int32 actual   = OS_FileOpenCheck("/cf/file");

    UtAssert_True(actual == expected, "OS_FileOpenCheck() (%ld) == OS_ERROR", (long)actual);

    OS_UT_SetupIterator(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, 1);
    UT_SetDefaultReturnValue(UT_KEY(OCS_strcmp), 0);
    expected = OS_SUCCESS;
    actual   = OS_FileOpenCheck("/cf/file");

    UtAssert_True(actual == expected, "OS_FileOpenCheck() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileOpenCheck(NULL);
    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_CloseFileByName(void)
{
    /*
     * Test Case For:
     * int32 OS_CloseFileByName(const char *Filename)
     */
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = OS_CloseFileByName("/cf/file");

    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    /* setup for success */
    OS_UT_SetupIterator(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, 1);
    expected = OS_SUCCESS;
    UT_SetDefaultReturnValue(UT_KEY(OCS_strcmp), 0);
    actual = OS_CloseFileByName("/cf/file");
    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_CloseFileByName(NULL);
    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_CloseAllFiles(void)
{
    /*
     * Test Case For:
     * int32 OS_CloseAllFiles(void)
     */
    int32 expected = -222;
    int32 actual;

    OS_UT_SetupIterator(OS_OBJECT_TYPE_OS_STREAM, UT_INDEX_1, 2);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdIteratorProcessEntry), 1, expected);
    actual = OS_CloseAllFiles();

    UtAssert_True(actual == expected, "OS_CloseAllFiles() (%ld) == -222", (long)actual);

    /* This uses a helper function OS_FileIteratorClose() with the iterator,
     * which needs to be called for coverage - it just invokes OS_close() */
    expected = OS_SUCCESS;
    actual   = OS_FileIteratorClose(UT_OBJID_1, NULL);

    UtAssert_True(actual == expected, "OS_FileIteratorClose() (%ld) == OS_SUCCESS", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(OS_stream_table, 0, sizeof(OS_stream_table));
    memset(OS_global_stream_table, 0, sizeof(OS_common_record_t) * OS_MAX_NUM_OPEN_FILES);
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
    ADD_TEST(OS_FileAPI_Init);
    ADD_TEST(OS_OpenCreate);
    ADD_TEST(OS_close);
    ADD_TEST(OS_TimedRead);
    ADD_TEST(OS_TimedWrite);
    ADD_TEST(OS_read);
    ADD_TEST(OS_write);
    ADD_TEST(OS_chmod);
    ADD_TEST(OS_stat);
    ADD_TEST(OS_lseek);
    ADD_TEST(OS_remove);
    ADD_TEST(OS_rename);
    ADD_TEST(OS_cp);
    ADD_TEST(OS_mv);
    ADD_TEST(OS_FDGetInfo);
    ADD_TEST(OS_FileOpenCheck);
    ADD_TEST(OS_CloseFileByName);
    ADD_TEST(OS_CloseAllFiles);
}
