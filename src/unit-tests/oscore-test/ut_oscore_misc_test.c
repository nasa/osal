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
    osal_id_t         qId          = OS_OBJECT_ID_UNDEFINED;
    osal_blockcount_t qDepth       = OSAL_BLOCKCOUNT_C(10);
    size_t            qSize        = OSAL_SIZE_C(4);
    uint32            qFlags       = 0;
    osal_id_t         semIds[3]    = {OS_OBJECT_ID_UNDEFINED, OS_OBJECT_ID_UNDEFINED, OS_OBJECT_ID_UNDEFINED};
    uint32            semInitValue = 1;
    uint32            semOptions   = 0;

    /*-----------------------------------------------------*/
    /* #1 Init-not-call-first */

    /*
     * Note that OS_API_Init() is supposed to be the first function invoked,
     * calling any other OSAL API before this is technically undefined behavior.
     * There is code to check for errors in this regard so this just tests that
     * the result is _not_ success.  The specific status code if called before
     * OS_API_Init is not documented.
     */
    UT_NOT_SUCCESS(OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags));
    UT_NOT_SUCCESS(OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions));
    UT_NOT_SUCCESS(OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions));
    UT_NOT_SUCCESS(OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions));

    /*-----------------------------------------------------*/
    /* #2 Nominal */

    UT_NOMINAL(OS_API_Init());

    UT_RETVAL(OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags), OS_SUCCESS);
    UT_RETVAL(OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions), OS_SUCCESS);
    UT_RETVAL(OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions), OS_SUCCESS);
    UT_RETVAL(OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions), OS_SUCCESS);

    /* Reset test environment */
    UT_TEARDOWN(OS_QueueDelete(qId));
    UT_TEARDOWN(OS_BinSemDelete(semIds[0]));
    UT_TEARDOWN(OS_CountSemDelete(semIds[1]));
    UT_TEARDOWN(OS_MutSemDelete(semIds[2]));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_RegisterEventHandler(OS_EventHandler_t handler)
** See header file for list of return values
**--------------------------------------------------------------------------------*/
int32 UT_os_eventhandler(OS_Event_t event, osal_id_t object_id, void *data)
{
    return OS_SUCCESS;
}

void UT_os_registereventhandler_test()
{
    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */
    UT_RETVAL(OS_RegisterEventHandler(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Nominal */
    UT_NOMINAL(OS_RegisterEventHandler(UT_os_eventhandler));
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
    UT_MIR_VOID(OS_printf("OS_printf() - #1 Nominal [This is the expected stdout output after API call]\n"));
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
    UT_MIR_VOID(OS_printf("OS_printf_enable() - #1 Nominal [This is the expected stdout output after API call]\n"));
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
    UT_MIR_VOID(OS_printf("OS_printf_disable() - #1 Nominal [This is the expected stdout output before API call]\n"));

    OS_printf_disable();
    UT_MIR_VOID(
        OS_printf("OS_printf_disable() - #1 Nominal [This is NOT the expected stdout output after API call]\n"));

    /* Reset test environment */
    OS_printf_enable();
    UT_MIR_VOID(OS_printf("OS_printf_disable() - #1 Nominal [This is the expected stdout output after test reset]\n"));
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
    OS_time_t time_struct;
    int32     i = 0;

    memset(&time_struct, 0, sizeof(time_struct));

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_GetLocalTime(NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */
    UT_RETVAL(OS_GetLocalTime(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    for (i = 0; i < 5; i++)
    {
        UT_NOMINAL(OS_GetLocalTime(&time_struct));
        UtPrintf("[Expecting output after API call to increase over time: %ld.%ld]\n",
                 (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

        OS_TaskDelay(20);
    }

    /* #3 Nominal - Manual inspection required */
    UT_MIR_STATUS(OS_GetLocalTime(&time_struct));
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
    OS_time_t time_struct;
    int32     i = 0;

    memset(&time_struct, 0, sizeof(time_struct));

    /*-----------------------------------------------------*/
    /* API not implemented */

    if (!UT_IMPL(OS_SetLocalTime(NULL)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_SetLocalTime(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    for (i = 0; i < 5; i++)
    {
        UT_NOMINAL(OS_GetLocalTime(&time_struct));
        UtPrintf("[Expecting output before API call to increase over time: %ld.%ld]\n",
                 (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

        OS_TaskDelay(20);
    }

    time_struct = OS_TimeAssembleFromNanoseconds(20000, 123000);

    /*
     * This case is MIR because on some systems this requires permission,
     * failure is expected if user does not have the required permission
     */
    if (UT_MIR_STATUS(OS_SetLocalTime(&time_struct)))
    {
        UtPrintf("OS_SetLocalTime() - #3 Nominal [New time set at %ld.%ld]\n",
                 (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

        for (i = 0; i < 5; i++)
        {
            UT_NOMINAL(OS_GetLocalTime(&time_struct));
            UtPrintf("[Expecting output before API call to increase over time: %ld.%ld]\n",
                     (long)OS_TimeGetTotalSeconds(time_struct), (long)OS_TimeGetMicrosecondsPart(time_struct));

            OS_TaskDelay(20);
        }

        UT_MIR_STATUS(OS_GetLocalTime(&time_struct));
    }
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
    os_err_name_t errNames[4];

    memset(errNames, 0, sizeof(errNames));

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_GetErrorName(OS_ERROR, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Undefined Error */

    UT_RETVAL(OS_GetErrorName(12345, &errNames[0]), OS_ERROR);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    UT_NOMINAL(OS_GetErrorName(OS_ERR_NAME_TOO_LONG, &errNames[0]));
    UtAssert_StrCmp(errNames[0], "OS_ERR_NAME_TOO_LONG", "%s == %s", errNames[0], "OS_ERR_NAME_TOO_LONG");

    UT_NOMINAL(OS_GetErrorName(OS_ERR_NAME_TAKEN, &errNames[1]));
    UtAssert_StrCmp(errNames[1], "OS_ERR_NAME_TAKEN", "%s == %s", errNames[1], "OS_ERR_NAME_TAKEN");

    UT_NOMINAL(OS_GetErrorName(OS_ERR_NO_FREE_IDS, &errNames[2]));
    UtAssert_StrCmp(errNames[2], "OS_ERR_NO_FREE_IDS", "%s == %s", errNames[2], "OS_ERR_NO_FREE_IDS");
}

/*--------------------------------------------------------------------------------*
** OS_StatusToString test helper function to avoid repeating logic
**--------------------------------------------------------------------------------*/
void UT_os_statustostring_test_helper(osal_status_t status)
{
    os_status_string_t status_string;
    char *             rtn_addr;
    char               expected[OS_STATUS_STRING_LENGTH + 1];

    /* Used oversized string to test for truncation */
    snprintf(expected, sizeof(expected) - 1, "%ld", OS_StatusToInteger(status));
    rtn_addr = OS_StatusToString(status, &status_string);
    UtAssert_ADDRESS_EQ(rtn_addr, status_string);
    UtAssert_STRINGBUF_EQ(status_string, sizeof(status_string), expected, sizeof(expected));
}

/*--------------------------------------------------------------------------------*
** Functional OS_StatusToString test
**--------------------------------------------------------------------------------*/
void UT_os_statustostring_test(void)
{
    /* NULL test */
    UtAssert_ADDRESS_EQ(OS_StatusToString(OS_SUCCESS, NULL), NULL);

    /* Status value tests */
    UT_os_statustostring_test_helper(OS_SUCCESS);
    UT_os_statustostring_test_helper(OS_ERROR);
    UT_os_statustostring_test_helper(OSAL_STATUS_C(INT32_MAX));
    UT_os_statustostring_test_helper(OSAL_STATUS_C(INT32_MIN));
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
    OS_heap_prop_t heapProp;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_HeapGetInfo(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Nominal (allows for not implemented) */

    UT_NOMINAL_OR_NOTIMPL(OS_HeapGetInfo(&heapProp));
}

/*================================================================================*
** End of File: ut_oscore_misc_test.c
**================================================================================*/
