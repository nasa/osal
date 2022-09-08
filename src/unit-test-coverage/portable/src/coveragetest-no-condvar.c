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
 * \ingroup  portable
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"
#include "os-shared-condvar.h"

void Test_OS_CondVarCreate_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarCreate_Impl(const OS_object_token_t *token, uint32 options)
     */

    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate_Impl, (UT_INDEX_0, 0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarLock_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarLock_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarLock_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarUnlock_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarUnlock_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarUnlock_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarSignal_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarSignal_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarSignal_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarBroadcast_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarBroadcast_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarBroadcast_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarWait_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarWait_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarWait_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarTimedWait_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarTimedWait_Impl(const OS_object_token_t *token, const OS_time_t *abs_wakeup_time)
     */
    OS_time_t wakeup_time;

    wakeup_time = OS_TimeAssembleFromMilliseconds(100, 100);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarTimedWait_Impl, (UT_INDEX_0, &wakeup_time), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarDelete_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarDelete_Impl(const OS_object_token_t *token)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarDelete_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_CondVarGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_CondVarGetInfo_Impl(const OS_object_token_t *token, OS_condvar_prop_t *condvar_prop)
     */
    OS_condvar_prop_t cv_prop;

    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetInfo_Impl, (UT_INDEX_0, &cv_prop), OS_ERR_NOT_IMPLEMENTED);
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
    ADD_TEST(OS_CondVarCreate_Impl);
    ADD_TEST(OS_CondVarLock_Impl);
    ADD_TEST(OS_CondVarUnlock_Impl);
    ADD_TEST(OS_CondVarSignal_Impl);
    ADD_TEST(OS_CondVarBroadcast_Impl);
    ADD_TEST(OS_CondVarWait_Impl);
    ADD_TEST(OS_CondVarTimedWait_Impl);
    ADD_TEST(OS_CondVarDelete_Impl);
    ADD_TEST(OS_CondVarGetInfo_Impl);
}
