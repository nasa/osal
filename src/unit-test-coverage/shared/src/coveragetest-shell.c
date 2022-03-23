/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-shell.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_ShellOutputToFile(void)
{
    /*
     * Test Case For:
     * int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_ShellOutputToFile("Cmd", 1);

    UtAssert_True(actual == expected, "OS_ShellOutputToFile() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_ShellOutputToFile(NULL, 1);

    UtAssert_True(actual == expected, "OS_ShellOutputToFile() (%ld) == OS_SUCCESS", (long)actual);
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
    ADD_TEST(OS_ShellOutputToFile);
}
