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
#include "ut-adaptor-console.h"
#include "os-shared-printf.h"
#include "os-shared-common.h"

#include "OCS_unistd.h"
#include "OCS_semLib.h"
#include "OCS_taskLib.h"
#include "OCS_errnoLib.h"
#include "OCS_stdio.h"

void Test_OS_ConsoleWakeup_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ConsoleWakeup_Impl(const char *string)
     */
    OS_object_token_t token = UT_TOKEN_0;

    /* this just gives the sem, only called in async mode */
    OS_ConsoleWakeup_Impl(&token);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semGive)) == 1, "semGive() called in async mode");

    /* Failure only causes a debug message to be generated, no error handling here */
    UT_SetDefaultReturnValue(UT_KEY(OCS_semGive), -1);
    OS_ConsoleWakeup_Impl(&token);
}

void Test_OS_ConsoleCreate_Impl(void)
{
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    /* Verify coverage when configured for sync mode */
    OS_console_table[0].IsAsync = false;
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(&token), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_taskSpawn, 0); /* Task _was not_ spawned */

    /* Verify coverage when configured for async mode */
    OS_console_table[0].IsAsync = true;
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(&token), OS_SUCCESS);
    UtAssert_STUB_COUNT(OCS_taskSpawn, 1); /* Task _was_ spawned */

    UT_SetDefaultReturnValue(UT_KEY(OCS_semCInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(&token), OS_SEM_FAILURE);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_semCInitialize));

    UT_SetDefaultReturnValue(UT_KEY(OCS_taskSpawn), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(&token), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_taskSpawn));

    token.obj_idx = OS_MAX_CONSOLES + 1;
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(&token), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_VxWorks_ConsoleTask_Entry(void)
{
    /* This task has an infinite loop, which only exits if semTake fails */
    UT_SetDeferredRetcode(UT_KEY(OCS_semTake), 2, OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(UT_ConsoleTest_TaskEntry(0), OCS_OK);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(UT_ConsoleTest_TaskEntry(0), OCS_OK);

    OS_SharedGlobalVars.GlobalState = OS_SHUTDOWN_MAGIC_NUMBER;
    OSAPI_TEST_FUNCTION_RC(UT_ConsoleTest_TaskEntry(0), OCS_OK);
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
    memset(OS_console_table, 0, sizeof(OS_console_table));
    memset(UT_Ref_OS_impl_console_table, 0, UT_Ref_OS_impl_console_table_SIZE);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ConsoleCreate_Impl);
    ADD_TEST(OS_ConsoleWakeup_Impl);
    ADD_TEST(OS_VxWorks_ConsoleTask_Entry);
}
