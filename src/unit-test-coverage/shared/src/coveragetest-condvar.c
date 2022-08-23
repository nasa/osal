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
 */
#include "os-shared-coveragetest.h"
#include "os-shared-condvar.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_CondVarAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarAPI_Init(), OS_SUCCESS);
}

void Test_OS_CondVarCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarCreate (uint32 *sem_id, const char *sem_name, uint32 options)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate(&objid, "UT", 0), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate(NULL, "UT", 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate(&objid, NULL, 0), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate(&objid, "UT", 0), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarCreate(&objid, "UT", 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_CondVarDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarDelete (uint32 sem_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_CondVarDelete(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarDelete(UT_OBJID_1), OS_ERROR);
}

void Test_OS_CondVarUnlock(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarUnlock ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarUnlock(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarUnlock_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarUnlock(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarUnlock(UT_OBJID_1), OS_ERROR);
}

void Test_OS_CondVarLock(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarLock ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarLock(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarLock_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarLock(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarLock(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_CondVarSignal(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarSignal(osal_id_t var_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarSignal(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarSignal_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarSignal(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarSignal(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_CondVarBroadcast(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarBroadcast(osal_id_t var_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarBroadcast(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarBroadcast_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarBroadcast(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarBroadcast(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_CondVarWait(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarWait(osal_id_t var_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CondVarWait(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarWait_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarWait(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarWait(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_CondVarTimedWait(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarTimedWait(osal_id_t var_id, const OS_time_t *abs_wakeup_time)
     */
    OS_time_t wakeup_time;

    wakeup_time = OS_TimeAssembleFromMilliseconds(100, 100);

    OSAPI_TEST_FUNCTION_RC(OS_CondVarTimedWait(UT_OBJID_1, &wakeup_time), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarTimedWait(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_CondVarTimedWait_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarTimedWait(UT_OBJID_1, &wakeup_time), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarTimedWait(UT_OBJID_1, &wakeup_time), OS_ERR_INVALID_ID);
}

void Test_OS_CondVarGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetIdByName(&objid, "UT"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetIdByName(&objid, NULL), OS_INVALID_POINTER);
}

void Test_OS_CondVarGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_CondVarGetInfo (uint32 sem_id, OS_condvar_prop_t *mut_prop)
     */
    OS_condvar_prop_t prop;

    memset(&prop, 0, sizeof(prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_CONDVAR, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetInfo(UT_OBJID_1, &prop), OS_SUCCESS);

    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_CondVarGetInfo(UT_OBJID_1, &prop), OS_ERR_INVALID_ID);
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
    ADD_TEST(OS_CondVarAPI_Init);
    ADD_TEST(OS_CondVarCreate);
    ADD_TEST(OS_CondVarDelete);
    ADD_TEST(OS_CondVarUnlock);
    ADD_TEST(OS_CondVarLock);
    ADD_TEST(OS_CondVarSignal);
    ADD_TEST(OS_CondVarBroadcast);
    ADD_TEST(OS_CondVarWait);
    ADD_TEST(OS_CondVarTimedWait);
    ADD_TEST(OS_CondVarGetIdByName);
    ADD_TEST(OS_CondVarGetInfo);
}
