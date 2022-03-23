/**
 * \brief Coverage test for no symtab implementation
 * \ingroup  portable
 */

#include "os-portable-coveragetest.h"
#include "os-shared-module.h"

void Test_No_Symtab(void)
{
    OSAPI_TEST_FUNCTION_RC(OS_SymbolLookup_Impl, (NULL, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleSymbolLookup_Impl, (NULL, NULL, NULL), OS_ERR_NOT_IMPLEMENTED);
    OSAPI_TEST_FUNCTION_RC(OS_SymbolTableDump_Impl, (NULL, 0), OS_ERR_NOT_IMPLEMENTED);
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
    ADD_TEST(No_Symtab);
}
