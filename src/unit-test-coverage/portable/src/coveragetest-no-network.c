/**
 * \brief Coverage test for no network implementation
 * \ingroup  portable
 */

#include "os-portable-coveragetest.h"
#include "os-shared-network.h"

void Test_No_Network(void)
{
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetID_Impl, (NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_NetworkGetHostName_Impl, (NULL, 0), OS_ERR_NOT_IMPLEMENTED);
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

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(No_Network);
}
