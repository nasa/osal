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
#include "os-shared-select.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_SelectSingle(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    uint32 StateFlags = 0;

    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle(UT_OBJID_1, &StateFlags, 0), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle(UT_OBJID_1, NULL, 0), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle(UT_OBJID_1, &StateFlags, 0), OS_ERROR);
}

void Test_OS_SelectMultiple(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdZero(&WriteSet);
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple(&ReadSet, &WriteSet, 0), OS_SUCCESS);
}

void Test_OS_SelectFdAddClearOps(void)
{
    /*
     * Test Cases For:
     * int32 OS_SelectFdZero(OS_FdSet *Set);
     * int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid);
     * int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid);
     * bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid);
     */
    OS_FdSet UtSet;

    /* Error cases */
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdZero(NULL), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdAdd(NULL, UT_OBJID_1), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdClear(NULL, UT_OBJID_1), OS_INVALID_POINTER);
    UtAssert_True(!OS_SelectFdIsSet(NULL, UT_OBJID_1), "!OS_SelectFdIsSet(NULL, UT_OBJID_1)");

    OSAPI_TEST_FUNCTION_RC(OS_SelectFdZero(&UtSet), OS_SUCCESS);

    /* Verify Outputs */
    UtAssert_STUB_COUNT(OCS_memset, 1);
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == false");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == false");

    OSAPI_TEST_FUNCTION_RC(OS_SelectFdAdd(&UtSet, UT_OBJID_1), OS_SUCCESS);

    UtAssert_True(OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == false");
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdAdd(&UtSet, UT_OBJID_2), OS_SUCCESS);

    UtAssert_True(OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == true");

    OSAPI_TEST_FUNCTION_RC(OS_SelectFdClear(&UtSet, UT_OBJID_2), OS_SUCCESS);
    UtAssert_True(OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == false");

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdToArrayIndex), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdAdd(&UtSet, UT_OBJID_2), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_SelectFdClear(&UtSet, UT_OBJID_1), OS_ERROR);
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == false");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == false");

    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdToArrayIndex));
    UtAssert_True(OS_SelectFdIsSet(&UtSet, UT_OBJID_1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, UT_OBJID_2), "OS_SelectFdIsSet(2) == false");
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
    ADD_TEST(OS_SelectFdAddClearOps);
    ADD_TEST(OS_SelectSingle);
    ADD_TEST(OS_SelectMultiple);
}
