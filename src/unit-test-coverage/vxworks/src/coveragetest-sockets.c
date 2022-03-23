/**
 * \file
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-sockets.h"

#include "OCS_ioLib.h"

void Test_OS_VxWorks_SetSocketFlags_Impl(void)
{
    UtAssert_VOIDCALL(UT_SocketTest_CallVxWorksSetFlags_Impl(0));

    UT_SetDefaultReturnValue(UT_KEY(OCS_ioctl), OCS_ERROR);
    UtAssert_VOIDCALL(UT_SocketTest_CallVxWorksSetFlags_Impl(0));
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
    ADD_TEST(OS_VxWorks_SetSocketFlags_Impl);
}
