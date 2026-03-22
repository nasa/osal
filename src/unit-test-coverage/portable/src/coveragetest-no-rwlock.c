/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
#include "os-shared-rwlock.h"

void Test_OS_RwLockCreate_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockCreate_Impl(const OS_object_token_t *token, uint32 options)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockCreate_Impl, (UT_INDEX_0, 0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockDelete_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockDelete_Impl(const OS_object_token_t *token, uint32 options)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockDelete_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockReadGive_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockReadGive_Impl(const OS_object_token_t *token)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadGive_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockWriteGive_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockWriteGive_Impl(const OS_object_token_t *token)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteGive_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockReadTake_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockReadTake_Impl(const OS_object_token_t *token)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockReadTake_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockWriteTake_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockWriteTake_Impl(const OS_object_token_t *token)
     */

    OSAPI_TEST_FUNCTION_RC(OS_RwLockWriteTake_Impl, (UT_INDEX_0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_RwLockGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_RwLockGetInfo_Impl(const OS_object_token_t *token, OS_rwlock_prop_t *rw_prop)
     */
    OS_rwlock_prop_t rw_prop;

    OSAPI_TEST_FUNCTION_RC(OS_RwLockGetInfo_Impl, (UT_INDEX_0, &rw_prop), OS_ERR_NOT_IMPLEMENTED);
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
    ADD_TEST(OS_RwLockCreate_Impl);
    ADD_TEST(OS_RwLockDelete_Impl);
    ADD_TEST(OS_RwLockReadGive_Impl);
    ADD_TEST(OS_RwLockWriteGive_Impl);
    ADD_TEST(OS_RwLockReadTake_Impl);
    ADD_TEST(OS_RwLockWriteTake_Impl);
    ADD_TEST(OS_RwLockGetInfo_Impl);
}
