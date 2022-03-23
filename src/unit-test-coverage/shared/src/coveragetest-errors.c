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

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(OS_ERROR, &err_name), OS_SUCCESS);
    UtAssert_True(strcmp(err_name, "OS_ERROR") == 0, "string(%s) == OS_ERROR", err_name);

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-4444, &err_name), OS_SUCCESS);
    UtAssert_True(strcmp(err_name, "UT_ERROR") == 0, "string(%s) == UT_ERROR", err_name);

    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-4445, &err_name), OS_ERROR);
    UtAssert_True(strcmp(err_name, "OS_UNKNOWN(-4445)") == 0, "string(%s) == OS_UNKNOWN(-4445)", err_name);
    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-555555, &err_name), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_GetErrorName(-555555, NULL), OS_INVALID_POINTER);
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
}
