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
    OSAPI_TEST_FUNCTION_RC(OS_DirAPI_Init(), OS_SUCCESS);
}

void Test_OS_mkdir(void)
{
    /*
     * Test Case For:
     * int32 OS_mkdir (const char *path, uint32 access)
     */
    OSAPI_TEST_FUNCTION_RC(OS_mkdir("Dir", OS_READ_WRITE), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_TranslatePath), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_mkdir("Dir", OS_READ_WRITE), OS_ERROR);
}

void Test_OS_DirectoryOpen(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(&objid, "Dir"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    /* Branch coverage for errors */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(&objid, "Dir"), OS_ERR_NO_FREE_IDS);

    UT_SetDefaultReturnValue(UT_KEY(OS_TranslatePath), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(&objid, "Dir"), OS_ERROR);

    /*
     * Note that the second arg (path) is validated by a separate unit (OS_TranslatePath),
     * so it should not be passed NULL here
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(NULL, "Dir"), OS_INVALID_POINTER);
}

void Test_OS_DirectoryClose(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryClose(uint32 dir_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryClose(UT_OBJID_1), OS_SUCCESS);

    /* Branch coverage for errors */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryClose(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_DirectoryRead(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRead(uint32 dir_id, OS_DirEntry_t *dirent)
     */
    os_dirent_t dirent;

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRead(UT_OBJID_1, &dirent), OS_SUCCESS);

    /* Branch coverage for errors */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRead(UT_OBJID_1, &dirent), OS_ERR_INVALID_ID);

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRead(UT_OBJID_1, NULL), OS_INVALID_POINTER);
}

void Test_OS_DirectoryRewind(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRewind(uint32 dir_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRewind(UT_OBJID_1), OS_SUCCESS);

    /* Branch coverage for errors */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRewind(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_rmdir(void)
{
    /*
     * Test Case For:
     * int32  OS_rmdir (const char *path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_rmdir("Dir"), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_TranslatePath), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_rmdir("Dir"), OS_ERROR);
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
