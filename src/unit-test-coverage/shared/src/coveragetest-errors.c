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
#include "os-shared-errors.h"

void Test_OS_GetErrorName(void)
{
    /*
     * Test Case For:
     * int32 OS_GetErrorName(int32 error_num, os_err_name_t* err_name);
     */
    os_err_name_t err_name;

    memset(&err_name, 0, sizeof(err_name));

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(OS_ERROR, &err_name), OS_SUCCESS);
    UtAssert_True(strcmp(err_name, "OS_ERROR") == 0, "string(%s) == OS_ERROR", err_name);

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-4444, &err_name), OS_SUCCESS);
    UtAssert_True(strcmp(err_name, "UT_ERROR") == 0, "string(%s) == UT_ERROR", err_name);

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-4445, &err_name), OS_ERROR);
    UtAssert_True(strcmp(err_name, "OS_UNKNOWN(-4445)") == 0, "string(%s) == OS_UNKNOWN(-4445)", err_name);
    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-555555, &err_name), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-555555, NULL), OS_INVALID_POINTER);
}

/*--------------------------------------------------------------------------------*
** OS_StatusToString test helper function to avoid repeating logic
**--------------------------------------------------------------------------------*/
void Test_OS_StatusToString_Helper(osal_status_t status)
{
    os_status_string_t status_string;
    char *             rtn_addr;
    char               expected[OS_STATUS_STRING_LENGTH + 1];

    /* Used oversized string to test for truncation */
    snprintf(expected, sizeof(expected), "%ld", OS_StatusToInteger(status));
    rtn_addr = OS_StatusToString(status, &status_string);
    UtAssert_ADDRESS_EQ(rtn_addr, status_string);
    UtAssert_STRINGBUF_EQ(status_string, sizeof(status_string), expected, sizeof(expected));
}

/*--------------------------------------------------------------------------------*
** Functional OS_StatusToString test
**--------------------------------------------------------------------------------*/
void Test_OS_StatusToString(void)
{
    /* NULL test */
    UtAssert_ADDRESS_EQ(OS_StatusToString(OS_SUCCESS, NULL), NULL);

    /* Status value tests */
    Test_OS_StatusToString_Helper(OS_SUCCESS);
    Test_OS_StatusToString_Helper(OS_ERROR);
    Test_OS_StatusToString_Helper(OSAL_STATUS_C(INT32_MAX));
    Test_OS_StatusToString_Helper(OSAL_STATUS_C(INT32_MIN));
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
    ADD_TEST(OS_GetErrorName);
    ADD_TEST(OS_StatusToString);
}
