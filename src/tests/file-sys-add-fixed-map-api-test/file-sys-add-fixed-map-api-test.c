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

/*
 * Filename: file-sys-add-fixed-map-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-FileSysAddFixedMap"
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* *************************************** MAIN ************************************** */

void TestFileSysAddFixedMapApi(void)
{
    osal_id_t fs_id;
    char      translated_path[OS_MAX_LOCAL_PATH_LEN + 5];
    char      long_path[OS_MAX_PATH_LEN + 5];

    /* Test for nominal inputs */

    /*
     * This test case requires a fixed virtual dir for one test case.
     * Just map /test to a dir of the same name, relative to current dir.
     */

    UtAssert_INT32_EQ(OS_TranslatePath("/test/myfile.txt", translated_path), OS_FS_ERR_PATH_INVALID);
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, "./test", "/test"), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TranslatePath("/test/myfile.txt", translated_path), OS_SUCCESS);

    /* Test for invalid inputs */
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(NULL, "./test", "/test"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, NULL, "/test"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, "./test", NULL), OS_INVALID_POINTER);

    /* Test names too long (phys_path and virt_path have different limits) */
    memset(long_path, 'x', sizeof(long_path) - 1);
    long_path[sizeof(long_path) - 1] = 0;
    long_path[0]                     = '/';

    memset(translated_path, 'y', sizeof(translated_path) - 1);
    translated_path[sizeof(translated_path) - 1] = 0;
    translated_path[0]                           = '/';

    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, "./test", long_path), OS_FS_ERR_PATH_TOO_LONG);
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, translated_path, "/test"), OS_FS_ERR_PATH_TOO_LONG);

    /* create a path where only the "name" part is too long, but the overall path length is within limit */
    translated_path[OS_MAX_LOCAL_PATH_LEN - 1] = 0;
    UtAssert_INT32_EQ(OS_FileSysAddFixedMap(&fs_id, translated_path, "/test"), OS_ERR_NAME_TOO_LONG);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestFileSysAddFixedMapApi, NULL, NULL, "TestFileSysAddFixedMapApi");
}
