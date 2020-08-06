/*
 *      Copyright (c) 2020, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
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
    int32 expected;
    int32 actual;
    uint32 fs_id;
    char virtual_path[OS_MAX_LOCAL_PATH_LEN];
    char translated_path[OS_MAX_LOCAL_PATH_LEN];

    /* Test for nominal inputs */
    /*
     * This test case requires a fixed virtual dir for one test case.
     * Just map /test to a dir of the same name, relative to current dir.
     */

    strcpy(virtual_path, "/test");
    expected = OS_SUCCESS;
    actual = OS_TranslatePath(virtual_path, translated_path);

    expected = OS_SUCCESS;
    actual = OS_FileSysAddFixedMap(&fs_id, "./test", virtual_path);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_SUCCESS;
    actual = OS_TranslatePath(virtual_path, translated_path);

    /* Test for invalid inputs */
    expected = OS_INVALID_POINTER;
    actual = OS_FileSysAddFixedMap(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_FileSysAddFixedMap(&fs_id, NULL, NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_FileSysAddFixedMap(&fs_id, "./test", NULL);
    UtAssert_True(actual == expected, "OS_FileSysAddFixedMap() (%ld) == OS_SUCCESS", (long)actual);


} /* end TestFileSysAddFixedMapApi */


void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestFileSysAddFixedMapApi, NULL, NULL, "TestFileSysAddFixedMapApi");
}

