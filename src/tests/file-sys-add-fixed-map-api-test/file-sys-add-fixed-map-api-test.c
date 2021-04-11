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
    int32     expected;
    int32     actual;
    osal_id_t fs_id;
    char      translated_path[OS_MAX_LOCAL_PATH_LEN];

    /* Test for nominal inputs */

    /*
     * This test case requires a fixed virtual dir for one test case.
     * Just map /test to a dir of the same name, relative to current dir.
     */

    expected = OS_FS_ERR_PATH_INVALID;
    actual   = OS_TranslatePath("/test/myfile.txt", translated_path);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_SUCCESS;
    actual   = OS_FileSysAddFixedMap(&fs_id, "./test", "/test");
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_SUCCESS;
    actual   = OS_TranslatePath("/test/myfile.txt", translated_path);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    expected = OS_ERR_NAME_TAKEN;
    actual   = OS_FileSysAddFixedMap(NULL, "./test", "/test");
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysAddFixedMap(&fs_id, NULL, "/test");
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysAddFixedMap(&fs_id, "./test", NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysAddFixedMap(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysAddFixedMap(&fs_id, NULL, NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FileSysAddFixedMap(NULL, "./test", NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_INVALID_POINTER", (long)actual);

} /* end TestFileSysAddFixedMapApi */

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
