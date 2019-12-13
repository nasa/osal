/*================================================================================*
** File:  ut_osnetwork_misc_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osnetwork_misc_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    const char* testDesc;

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    return;
    
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_NetworkGetID(void)
** Purpose: Returns network ID of the machine it is on
** Parameters: None
** Returns: OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: OS-call-failure condition
**   1) Cause the OS call to fail
**   2) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**        (a) OS_ERR_NOT_IMPLEMENTED __or__
**        (b) host id of value greater than 0
**--------------------------------------------------------------------------------*/
void UT_os_networkgetid_test()
{
    int32 res = 0;
    const char* testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_NetworkGetID();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_networkgetid_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_NetworkGetID();

    /* NOTE: This API does not return error codes.
     * Any return value could be valid */
    UT_OS_LOG("OS_NetworkGetID() return value=%ld", (long)res);
    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_MIR);

UT_os_networkgetid_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_NetworkGetHostName(char *host_name, uint32 name_len)
** Purpose: Returns the network name of the machine it is on
** Parameters: *host_name - pointer that will hold the network name of the machine it is on
**             name_len - the maximum length of the memory address host_name points to
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the OS call failed or name length passed in is 0
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   - Call this routine with a null pointer as one of the arguments
**   - Expect the returned value to be
**       (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Zero-name-length-arg condition
**   - Call this routine with 0 as the argument for name length
**   - Expect the returned value to be
**       (a) OS_ERROR
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   - Cause the OS call to fail
**   - Expect the returned value to be
**       (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   - Call this routine with all valid arguments
**   - Expect the returned value to be
**       (a) OS_SUCCESS, _and_
**       (b) the returned buffer to be non-empty
**--------------------------------------------------------------------------------*/
void UT_os_networkgethostname_test()
{
    int32 res = 0;
    const char* testDesc;
    char buffer[UT_OS_IO_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_NetworkGetHostName(buffer, sizeof(buffer));
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_networkgethostname_test_exit_tag;
    }


    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_NetworkGetHostName(NULL, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Zero-name-length-arg";

    res = OS_NetworkGetHostName(buffer, 0);
    if (res == OS_ERROR)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_NetworkGetHostName(buffer, sizeof(buffer));
    if (res == OS_SUCCESS)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_networkgethostname_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_osnetwork_misc_test.c
**================================================================================*/
