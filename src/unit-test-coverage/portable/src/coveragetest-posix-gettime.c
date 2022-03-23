/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"
#include "os-shared-clock.h"

#include "OCS_time.h"

void Test_OS_GetLocalTime_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GetLocalTime_Impl(OS_time_t *time_struct)
     */
    OS_time_t timeval = {0};

    OSAPI_TEST_FUNCTION_RC(OS_GetLocalTime_Impl, (&timeval), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_clock_gettime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GetLocalTime_Impl, (&timeval), OS_ERROR);
}

void Test_OS_SetLocalTime_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct)
     */
    OS_time_t timeval = {0};

    OSAPI_TEST_FUNCTION_RC(OS_SetLocalTime_Impl, (&timeval), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_clock_settime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_SetLocalTime_Impl, (&timeval), OS_ERROR);
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

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GetLocalTime_Impl);
    ADD_TEST(OS_SetLocalTime_Impl);
}
