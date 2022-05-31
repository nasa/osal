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
#include "os-shared-mutex.h"

#include "OCS_string.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_MutexAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutexAPI_Init(), OS_SUCCESS);
}

void Test_OS_MutSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, "UT", 0), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(NULL, "UT", 0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, NULL, 0), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, "UT", 0), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, "UT", 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_MutSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete (uint32 sem_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_MutSemDelete(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemDelete(UT_OBJID_1), OS_ERROR);
}

void Test_OS_MutSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive ( uint32 sem_id )
     */
    OS_mutex_internal_record_t *mutex;

    /* Set up for "last owner" matching the calling task (nominal) */
    mutex             = &OS_mutex_table[1];
    mutex->last_owner = OS_TaskGetId();
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGive(UT_OBJID_1), OS_SUCCESS);

    /* owner should be unset */
    UtAssert_True(!OS_ObjectIdDefined(mutex->last_owner), "Mutex owner unset");

    /* Call again when not "owned".  This still works (or at least it calls the OS impl)
     * but should generate a debug message */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGive(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGive(UT_OBJID_1), OS_ERROR);
}

void Test_OS_MutSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake ( uint32 sem_id )
     */
    OS_mutex_internal_record_t *mutex;

    /* Set up for "last owner" being undefined (nominal) */
    mutex             = &OS_mutex_table[1];
    mutex->last_owner = OS_OBJECT_ID_UNDEFINED;
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake(UT_OBJID_1), OS_SUCCESS);

    /* owner should be set */
    UtAssert_True(OS_ObjectIdDefined(mutex->last_owner), "Mutex owner set");

    /* Call again when not already "owned".  This still works (or at least it calls the OS impl) */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake(UT_OBJID_1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OS_MutSemTake_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake(UT_OBJID_1), OS_ERROR);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_MutSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(&objid, "UT"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(&objid, NULL), OS_INVALID_POINTER);
}

void Test_OS_MutSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
    OS_mut_sem_prop_t prop;

    memset(&prop, 0, sizeof(prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_MUTEX, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo(UT_OBJID_1, &prop), OS_SUCCESS);

    OSAPI_TEST_OBJID(prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC", prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo(UT_OBJID_1, &prop), OS_ERR_INVALID_ID);
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
    ADD_TEST(OS_MutexAPI_Init);
    ADD_TEST(OS_MutSemCreate);
    ADD_TEST(OS_MutSemDelete);
    ADD_TEST(OS_MutSemGive);
    ADD_TEST(OS_MutSemTake);
    ADD_TEST(OS_MutSemGetIdByName);
    ADD_TEST(OS_MutSemGetInfo);
}
