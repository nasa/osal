/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*================================================================================*
** File:  ut_oscore_misc_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_misc_test.h"

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
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_API_Init(void)
** Purpose: Initializes the tables that the OS API uses to keep track of information
**          about objects
** Parameters: None
** Returns: OS_ERROR on an unsuccessful inits
**          OS_SUCCESS on a successful inits
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Init-not-call-first condition
**   1) Don't call this routine first
**   2) Call OS_QueueCreate, OS_BinSemCreate, OS_CountSemCreate, OS_MutSemCreate
**   3) Expect the returned value from those routines to be
**       (a) __not__ OS_SUCCESS
*** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**       (a) OS_SUCCESS (although results are not directly observable)
**   3) Call OS_QueueCreate, OS_BinSemCreate, OS_CountSemCreate, OS_MutSemCreate
**   4) Expect the returned value from those routines to be
**       (a) OS_SUCCESS
*--------------------------------------------------------------------------------*/
void UT_os_apiinit_test()
{
    int32             res = 0;
    const char *      testDesc;
    osal_id_t         qId;
    osal_blockcount_t qDepth = OSAL_BLOCKCOUNT_C(10);
    size_t            qSize  = OSAL_SIZE_C(4);
    uint32            qFlags = 0;
    osal_id_t         semIds[3];
    uint32            semInitValue = 1, semOptions = 0;

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first";

    if ((OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags) != OS_SUCCESS) &&
        (OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions) != OS_SUCCESS) &&
        (OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions) != OS_SUCCESS) &&
        (OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions) != OS_SUCCESS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_QueueDelete(qId);
    OS_BinSemDelete(semIds[0]);
    OS_CountSemDelete(semIds[1]);
    OS_MutSemDelete(semIds[2]);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_API_Init();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API not implemented";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_apiinit_test_exit_tag;
    }
    else if ((res == OS_SUCCESS) && (OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags) == OS_SUCCESS) &&
             (OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions) == OS_SUCCESS) &&
             (OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions) == OS_SUCCESS) &&
             (OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions) == OS_SUCCESS))
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    }
    else
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    OS_QueueDelete(qId);
    OS_BinSemDelete(semIds[0]);
    OS_CountSemDelete(semIds[1]);
    OS_MutSemDelete(semIds[2]);

UT_os_apiinit_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf(const char String, ...)
** Purpose: Provides a printing utility similar to printf
** Parameters: String - text portion of the print
**             ellipsis - the other parameters to print
** Returns: Nothing
** -----------------------------------------------------
** Test #1: Nominal condition
**   1) Call OS_printf_enable
**   2) Call this routine to print a text string.
**   3) Visually observe that the text string in #2 did get print.
**--------------------------------------------------------------------------------*/
void UT_os_printf_test()
{
    OS_printf_enable();
    UtPrintf("OS_printf() - #1 Nominal [This is the expected stdout output after API call]\n");
    OS_printf("OS_printf() - #1 Nominal [ This is the expected stdout output after API call]\n");

    UT_OS_TEST_RESULT("#1 Nominal - Manual inspection required", UTASSERT_CASETYPE_MIR);
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf_enable(void)
** Purpose: Enables the UART or console output of OS_printf
** Parameters: None
** Returns: Nothing
** Test #1: Nominal condition
**   1) Call OS_printf_disable
**   2) Call OS_printf to print a text string.
**   3) Call this routine
**   4) Call OS_printf to print a text string that's different than #2
**   5) Visually observe that text string in #2 did not print, but text string in #4 did.
**--------------------------------------------------------------------------------*/
void UT_os_printfenable_test()
{
    OS_printf_disable();

    OS_printf_enable();
    UtPrintf("OS_printf_enable() - #1 Nominal [This is the expected stdout output after API call]\n");
    OS_printf("OS_printf_enable() - #1 Nominal [This is the expected stdout output after API call]\n");

    UT_OS_TEST_RESULT("#1 Nominal - Manual inspection required", UTASSERT_CASETYPE_MIR);
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf_disable(void)
** Purpose: Disables the UART or console output of OS_printf
** Parameters: None
** Returns: Nothing
** Test #1: Nominal condition
**   1) Call OS_printf_enable
**   2) Call OS_printf() to print a text string.
**   3) Call this routine
**   4) Call OS_printf() to print a text string that's different than #2
**   5) Visually observe that text string in #2 did get print, but text string in #4 did not.
**--------------------------------------------------------------------------------*/
void UT_os_printfdisable_test()
{
    OS_printf_enable();
    UtPrintf("OS_printf_disable() - #1 Nominal [This is the expected stdout output before API call]\n");
    OS_printf("OS_printf_disable() - #1 Nominal [This is the expected stdout output before API call]\n");

    OS_printf_disable();
    UtPrintf("OS_printf_disable() - #1 Nominal [This is NOT the expected stdout output after API call]\n");
    OS_printf("OS_printf_disable() - #1 Nominal [This is NOT the expected stdout output after API call]\n");

    UT_OS_TEST_RESULT("#1 Nominal - Manual inspection required", UTASSERT_CASETYPE_MIR);

    /* Reset test environment */
    OS_printf_enable();
    UtPrintf("OS_printf_disable() - #1 Nominal [This is the expected stdout output after test reset]\n");
    OS_printf("OS_printf_disable() - #1 Nominal [This is the expected stdout output after test reset]\n");
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_GetLocalTime(OS_time_t *time_struct)
** Purpose: Returns the local time of the machine it is on
** Parameters: *time_struct - a pointer to an OS_time_t structure that will hold
**                            the current time in seconds and milliseconds
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS (although results are not directly observable)
**--------------------------------------------------------------------------------*/
void UT_os_getlocaltime_test()
{
    OS_time_t   time_struct;
    const char *testDesc;
    int32       res = 0, i = 0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_GetLocalTime(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_getlocaltime_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";
    res      = OS_GetLocalTime(NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_GetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
        UtPrintf("\n");
        for (i = 0; i < 5; i++)
        {
            UtPrintf("OS_GetLocalTime() - #3 Nominal ");
            UtPrintf("[Expecting output after API call to increase over time: %ld.%ld]\n",
                     (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

            OS_TaskDelay(20);
            OS_GetLocalTime(&time_struct);
        }

        testDesc = "#3 Nominal - Manual inspection required";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_MIR);
    }
    else
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_getlocaltime_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SetLocalTime(OS_time_t *time_struct)
** Purpose: Allows user to set the local time of the machine it is on
** Parameters: *time_struct - a pointer to an OS_time_t structure that holds the
**                            target time in seconds and milliseconds
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   - Call this routine with valid argument
**   - Expect the returned value to be
**       (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_setlocaltime_test()
{
    OS_time_t   time_struct;
    const char *testDesc;
    int32       res = 0, i = 0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_SetLocalTime(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_setlocaltime_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_GetLocalTime(NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_GetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
        for (i = 0; i < 5; i++)
        {
            UtPrintf("OS_SetLocalTime() - #3 Nominal ");
            UtPrintf("[Expecting output before API call to increase over time: %ld.%ld]\n",
                     (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

            OS_TaskDelay(20);
            OS_GetLocalTime(&time_struct);
        }
    }

    time_struct = OS_TimeAssembleFromNanoseconds(20000, 123000);

    res = OS_SetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
        UtPrintf("OS_SetLocalTime() - #3 Nominal [New time set at %ld.%ld]\n",
                 (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

        res = OS_GetLocalTime(&time_struct);
        if (res == OS_SUCCESS)
        {
            for (i = 0; i < 5; i++)
            {
                UtPrintf("OS_SetLocalTime() - #3 Nominal ");
                UtPrintf("[Expecting output after API call to increase over time: %ld.%ld]\n",
                         (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

                OS_TaskDelay(20);
                OS_GetLocalTime(&time_struct);
            }
        }

        testDesc = "#3 Nominal - Manual inspection required";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_MIR);
    }
    else
    {
        /* Most likely it is a permission issue - no way to fix - but OK to ignore this failure */
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
    }

UT_os_setlocaltime_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_GetErrorName(int32 error_num, os_err_name_t * err_name)
** Purpose: Returns the string name of the error value
** Parameters: error_num - an error value
**             *err_name - pointer to hold the returned string name
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_geterrorname_test(void)
{
    int32         res = 0;
    os_err_name_t errNames[4];
    const char *  testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_GetErrorName(OS_SUCCESS, &errNames[0]);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_geterrorname_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_GetErrorName(OS_ERROR, NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Undefined Error";

    if (OS_GetErrorName(12345, &errNames[0]) == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    if ((OS_GetErrorName(OS_ERR_NAME_TOO_LONG, &errNames[0]) == OS_SUCCESS) &&
        (strcmp(errNames[0], "OS_ERR_NAME_TOO_LONG") == 0) &&
        (OS_GetErrorName(OS_ERR_NAME_TAKEN, &errNames[1]) == OS_SUCCESS) &&
        (strcmp(errNames[1], "OS_ERR_NAME_TAKEN") == 0) &&
        (OS_GetErrorName(OS_ERR_NO_FREE_IDS, &errNames[2]) == OS_SUCCESS) &&
        (strcmp(errNames[2], "OS_ERR_NO_FREE_IDS") == 0))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_geterrorname_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
** Purpose: Returns current info on the heap
** Parameters: prop - out pointer to heap data
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**          OS_ERROR if OS call failed
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_heapgetinfo_test(void)
{
    int32          res = 0;
    OS_heap_prop_t heapProp;
    const char *   testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_HeapGetInfo(&heapProp);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_heapgetinfo_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_HeapGetInfo(NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_HeapGetInfo(&heapProp);
    if (res == OS_SUCCESS)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_heapgetinfo_test_exit_tag:
    return;
}

/*================================================================================*
** End of File: ut_oscore_misc_test.c
**================================================================================*/
