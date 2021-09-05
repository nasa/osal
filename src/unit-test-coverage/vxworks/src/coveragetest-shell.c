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
 * \file     coveragetest-shell.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-filetable-stub.h"
#include "os-shared-file.h"

#include "OCS_shellLib.h"
#include "OCS_taskLib.h"

void Test_OS_ShellOutputToFile_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
     */
    OS_object_token_t token = UT_TOKEN_0;

    /*
     * The ShellOutputToFile will loop until the
     * taskNameToId() function returns ERROR, so this
     * must be set to avoid getting into an endless loop.
     */
    UT_SetDeferredRetcode(UT_KEY(OCS_taskNameToId), 2, -1);
    OSAPI_TEST_FUNCTION_RC(OS_ShellOutputToFile_Impl(&token, "TestCmd"), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_shellGenericInit)) == 1, "shellGenericInit() called");

    /* failure to open the output file */
    UT_SetDeferredRetcode(UT_KEY(OS_OpenCreate), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ShellOutputToFile_Impl(&token, "TestCmd"), OS_ERROR);

    /* Shell failure */
    UT_SetDefaultReturnValue(UT_KEY(OCS_shellGenericInit), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ShellOutputToFile_Impl(&token, "TestCmd"), OS_ERROR);

    /* ID failure */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    UT_SetDefaultReturnValue(UT_KEY(OCS_taskNameToId), -1);
    OSAPI_TEST_FUNCTION_RC(OS_ShellOutputToFile_Impl(&token, "TestCmd"), OS_SUCCESS);
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
    memset(UT_FileTableTest_OS_impl_filehandle_table, 0, UT_FileTableTest_OS_impl_filehandle_table_SIZE);
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
    ADD_TEST(OS_ShellOutputToFile_Impl);
}
