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
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"

#include "os-shared-dir.h"
#include "os-shared-idmap.h"

#include "OCS_stdlib.h"
#include "OCS_dirent.h"
#include "OCS_unistd.h"
#include "OCS_fcntl.h"
#include "OCS_stat.h"
#include "OCS_errno.h"

void Test_OS_DirCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
     */
    struct OCS_stat statbuf;

    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl, ("dir", 0), OS_SUCCESS);

    /* With errno other than EEXIST it should return OS_ERROR */
    OCS_errno = OCS_EROFS;
    UT_SetDefaultReturnValue(UT_KEY(OCS_mkdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl, ("dir", 0), OS_ERROR);

    /* If the errno is EEXIST it should return success */
    OCS_errno = OCS_EEXIST;
    memset(&statbuf, 0, sizeof(statbuf));
    statbuf.st_mode = OCS_S_IFDIR;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &statbuf, sizeof(statbuf), false);
    UT_SetDefaultReturnValue(UT_KEY(OCS_mkdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl, ("dir", 0), OS_SUCCESS);

    /* EEXIST but not a dir */
    statbuf.st_mode = 0;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &statbuf, sizeof(statbuf), false);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl, ("dir", 0), OS_ERROR);

    /* stat failure */
    UT_SetDefaultReturnValue(UT_KEY(OCS_stat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl, ("dir", 0), OS_ERROR);
}

void Test_OS_DirOpen_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl, (&token, "dir"), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OCS_opendir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl, (&token, "dir"), OS_ERROR);
}

void Test_OS_DirClose_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirClose_Impl(uint32 local_id)
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_DirClose_Impl, (&token), OS_SUCCESS);
}

void Test_OS_DirRead_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
     */
    os_dirent_t       dirent_buff;
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl, (&token, &dirent_buff), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_readdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl, (&token, &dirent_buff), OS_ERROR);
}

void Test_OS_DirRewind_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRewind_Impl(uint32 local_id)
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_DirRewind_Impl, (&token), OS_SUCCESS);
}

void Test_OS_DirRemove_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRemove_Impl(const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl, ("dir"), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_rmdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl, ("dir"), OS_ERROR);
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
    ADD_TEST(OS_DirCreate_Impl);
    ADD_TEST(OS_DirOpen_Impl);
    ADD_TEST(OS_DirClose_Impl);
    ADD_TEST(OS_DirRead_Impl);
    ADD_TEST(OS_DirRewind_Impl);
    ADD_TEST(OS_DirRemove_Impl);
}
