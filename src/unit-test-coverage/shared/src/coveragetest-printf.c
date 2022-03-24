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
#include "os-shared-printf.h"
#include "os-shared-common.h"

#include "OCS_stdio.h"

char TestConsoleBuffer[16];

void Test_OS_ConsoleAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_ConsoleAPI_Init(void)
     */
    OS_object_token_t token;

    /* make a custom token to force use of array index 0 */
    token.lock_mode = OS_LOCK_MODE_NONE;
    token.obj_type  = OS_OBJECT_TYPE_OS_CONSOLE;
    token.obj_idx   = UT_INDEX_0;
    token.obj_id    = UT_OBJID_1;

    UT_SetDataBuffer(UT_KEY(OS_ObjectIdAllocateNew), &token, sizeof(token), false);

    /* call for coverage */
    OS_ConsoleAPI_Init();
    UtAssert_STUB_COUNT(OS_ConsoleCreate_Impl, 1);
    UT_ResetState(UT_KEY(OS_ConsoleCreate_Impl));

    /* Fail OS_ObjectIdAllocateNew */
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OS_ConsoleAPI_Init();
    UtAssert_STUB_COUNT(OS_ConsoleCreate_Impl, 0);
}

void Test_OS_printf(void)
{
    /*
     * Test Case For:
     * void OS_printf( const char *String, ...);
     * void OS_printf_disable(void);
     * void OS_printf_enable(void);
     */

    /* catch case where OS_printf called before init */
    OS_SharedGlobalVars.PrintfConsoleId = OS_OBJECT_ID_UNDEFINED;
    OS_SharedGlobalVars.GlobalState     = 0;
    OS_printf("UnitTest1");
    UtAssert_True(OS_console_table[0].WritePos == 0, "WritePos (%lu) >= 0",
                  (unsigned long)OS_console_table[0].WritePos);

    /* because printf is disabled, the call count should _not_ increase here */
    OS_SharedGlobalVars.GlobalState = OS_INIT_MAGIC_NUMBER;
    OS_printf_disable();
    OS_printf("UnitTest2");
    UtAssert_True(OS_console_table[0].WritePos == 0, "WritePos (%lu) >= 0",
                  (unsigned long)OS_console_table[0].WritePos);

    /* normal case - sync mode */
    OS_console_table[0].IsAsync = false;
    OS_printf_enable();
    OS_printf("UnitTest3s");
    UtAssert_STUB_COUNT(OS_ConsoleWakeup_Impl, 0);
    UtAssert_STUB_COUNT(OS_ConsoleOutput_Impl, 1);
    UtAssert_True(OS_console_table[0].WritePos >= 10, "WritePos (%lu) >= 10",
                  (unsigned long)OS_console_table[0].WritePos);

    /* normal case - async mode */
    OS_console_table[0].IsAsync  = true;
    OS_console_table[0].WritePos = 0;
    OS_printf("UnitTest3a");
    UtAssert_STUB_COUNT(OS_ConsoleWakeup_Impl, 1);
    UtAssert_STUB_COUNT(OS_ConsoleOutput_Impl, 1);
    UtAssert_True(OS_console_table[0].WritePos >= 10, "WritePos (%lu) >= 10",
                  (unsigned long)OS_console_table[0].WritePos);

    /* print a long string that does not fit in the 16-char buffer */
    OS_printf_enable();
    OS_printf("UnitTest4BufferLengthExceeded");
    UtAssert_UINT32_EQ(OS_console_table[0].OverflowEvents, 1);

    /* test writing with a non-empty console name */
    strncpy(OS_console_table[0].device_name, "ut", sizeof(OS_console_table[0].device_name) - 1);
    OS_printf("UnitTest5");

    /* Cover branch for console name overflowing buffer*/
    OS_console_table[0].WritePos = 0;
    OS_console_table[0].ReadPos  = 1;
    OS_printf("UnitTest5.5");
    UtAssert_UINT32_EQ(OS_console_table[0].OverflowEvents, 3);

    /*
     * For coverage, exercise different paths depending on the return value
     */
    UT_SetDefaultReturnValue(UT_KEY(OCS_vsnprintf), -1);
    OS_printf("UnitTest6");

    UT_SetDefaultReturnValue(UT_KEY(OCS_vsnprintf), OS_BUFFER_SIZE + 10);
    OS_printf("UnitTest7");

    /* Null case */
    OS_printf(NULL);

    /* OS_ObjectIdGetById failure */
    UT_ResetState(UT_KEY(OS_ConsoleWakeup_Impl));
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OS_printf("a");
    UtAssert_STUB_COUNT(OS_ConsoleWakeup_Impl, 0);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(OS_console_table, 0, sizeof(OS_console_table));
    memset(&OS_SharedGlobalVars, 0, sizeof(OS_SharedGlobalVars));
    OS_console_table[0].BufBase = TestConsoleBuffer;
    OS_console_table[0].BufSize = sizeof(TestConsoleBuffer);
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
    ADD_TEST(OS_ConsoleAPI_Init);
    ADD_TEST(OS_printf);
}
