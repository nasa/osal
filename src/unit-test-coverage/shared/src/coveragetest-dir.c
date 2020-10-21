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
 * \file     coveragetest-dir.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-dir.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_DirAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_DirAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_DirAPI_Init();

    UtAssert_True(actual == expected, "OS_DirAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_mkdir(void)
{
    /*
     * Test Case For:
     * int32 OS_mkdir (const char *path, uint32 access)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_mkdir("Dir", 0);

    UtAssert_True(actual == expected, "OS_mkdir() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_DirectoryOpen(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual = OS_DirectoryOpen(&objid, "Dir");

    UtAssert_True(actual == expected, "OS_DirectoryOpen() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_DirectoryClose(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryClose(uint32 dir_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_DirectoryClose(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_DirectoryClose() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_DirectoryRead(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRead(uint32 dir_id, OS_DirEntry_t *dirent)
     */
    int32       expected = OS_SUCCESS;
    os_dirent_t dirent;

    int32 actual = OS_DirectoryRead(UT_OBJID_1, &dirent);

    UtAssert_True(actual == expected, "OS_DirectoryRead() (%ld) == OS_SUCCESS", (long)actual);

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRead(UT_OBJID_1, NULL), OS_INVALID_POINTER);
}

void Test_OS_DirectoryRewind(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRewind(uint32 dir_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_DirectoryRewind(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_DirectoryRewind() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_rmdir(void)
{
    /*
     * Test Case For:
     * int32  OS_rmdir (const char *path)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_rmdir("Dir");

    UtAssert_True(actual == expected, "OS_rmdir() (%ld) == OS_SUCCESS", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
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
    ADD_TEST(OS_DirAPI_Init);
    ADD_TEST(OS_mkdir);
    ADD_TEST(OS_DirectoryOpen);
    ADD_TEST(OS_DirectoryClose);
    ADD_TEST(OS_DirectoryRead);
    ADD_TEST(OS_DirectoryRewind);
    ADD_TEST(OS_rmdir);
}
