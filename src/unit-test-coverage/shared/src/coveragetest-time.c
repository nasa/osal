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
#include "os-shared-time.h"
#include "os-shared-timebase.h"
#include "os-shared-task.h"

#include "OCS_string.h"

static uint32 UT_TimerCount    = 0;
static uint32 UT_TimerArgCount = 0;

void UT_TimerCallback(osal_id_t timer_id)
{
    ++UT_TimerCount;
}

void UT_TimerArgCallback(osal_id_t object_id, void *arg)
{
    ++UT_TimerArgCount;
}

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_TimerCbAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerCbAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimerCbAPI_Init(), OS_SUCCESS);
}

void Test_OS_TimerAdd(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t
     * callback_ptr, void *callback_arg)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;
    char      arg   = 'a';

    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_SUCCESS);

    /* test error cases */
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(NULL, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, NULL, UT_OBJID_1, UT_TimerArgCallback, &arg), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_ERR_NAME_TOO_LONG);

    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, NULL, &arg), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg),
                           OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_ERR_INVALID_ID);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdAllocateNew), 1, OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_ERR_NO_FREE_IDS);

    /* Callback ring paths (still success) */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 2, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 3, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerAdd(&objid, "UT", UT_OBJID_1, UT_TimerArgCallback, &arg), OS_SUCCESS);
}

void Test_OS_TimerCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *accuracy, OS_TimerCallback_t callback_ptr)
     */
    osal_id_t    objid    = OS_OBJECT_ID_UNDEFINED;
    osal_index_t local_id = OSAL_INDEX_C(0);
    uint32       accuracy = 0xFFFFFFFF;

    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback), OS_SUCCESS);
    OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMECB, objid, &local_id);
    UtAssert_True(OS_timecb_table[local_id].callback_ptr != NULL, "OS_timecb_table[%lu].callback_ptr (%lx) != NULL",
                  (unsigned long)local_id, (unsigned long)OS_timecb_table[local_id].callback_ptr);
    UT_TimerCount = 0;
    OS_timecb_table[local_id].callback_ptr(objid, OS_timecb_table[local_id].callback_arg);
    UtAssert_True(UT_TimerCount == 1, "UT_TimerCount (%lu) == 1", (unsigned long)UT_TimerCount);

    /* additional cases to exercise error paths */
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(NULL, "UT", &accuracy, UT_TimerCallback), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, NULL, &accuracy, UT_TimerCallback), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", NULL, UT_TimerCallback), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", &accuracy, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TimeBaseCreate), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback), OS_ERROR);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback), OS_ERR_NAME_TOO_LONG);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    /* This function creates its own timebase.  If OS_DoTimerAdd() fails this timebase needs to be deleted */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback), OS_ERR_INVALID_ID);
    UtAssert_STUB_COUNT(OS_TimeBaseDelete, 1);
}

void Test_OS_TimerSet(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_1, 0, 0), OS_TIMER_ERR_INVALID_ARGS);

    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_1, 0, 1), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_1, 1, 0), OS_SUCCESS);

    OS_timecb_table[2].timebase_token.obj_type = OS_OBJECT_TYPE_OS_TIMEBASE;
    OS_timecb_table[2].timebase_token.obj_id   = UT_OBJID_2;
    OS_timecb_table[2].timebase_token.obj_idx  = UT_INDEX_0;
    OS_timecb_table[2].flags                   = TIMECB_FLAG_DEDICATED_TIMEBASE;

    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, 0, 1), OS_SUCCESS);
    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));

    /* Call again without dedicated timebase */
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, 0, 1), OS_SUCCESS);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, 0, 1), OS_ERR_INVALID_ID);

    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, UINT32_MAX, 1), OS_TIMER_ERR_INVALID_ARGS);
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, 1, UINT32_MAX), OS_TIMER_ERR_INVALID_ARGS);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimerSet(UT_OBJID_2, 0, 1), OS_ERR_INCORRECT_OBJ_STATE);
}

void Test_OS_TimerDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerDelete(uint32 timer_id)
     */
    osal_id_t                      timebase_id   = OS_OBJECT_ID_UNDEFINED;
    osal_id_t                      timer_objid_1 = OS_OBJECT_ID_UNDEFINED;
    osal_id_t                      timer_objid_2 = OS_OBJECT_ID_UNDEFINED;
    OS_timebase_internal_record_t *timebase;
    OS_object_token_t              timebase_token;
    uint32                         accuracy;

    memset(&timebase_token, 0, sizeof(timebase_token));

    /* The ObjIds in the ring need to match what will be in the token */
    /* Get a "timebase" from the stub so the objid will validate */
    OS_TimeBaseCreate(&timebase_id, "ut", NULL);
    OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id, &timebase_token);
    timebase = OS_OBJECT_TABLE_GET(OS_timebase_table, timebase_token);

    /* Add and confirm timebase and cb references */
    OS_TimerAdd(&timer_objid_1, "UT1", timebase_id, UT_TimerArgCallback, NULL);
    UtAssert_True(OS_ObjectIdEqual(timebase->first_cb, timer_objid_1), "1st timer add - First CB at timer 1");

    OS_TimerAdd(&timer_objid_2, "UT2", timebase_id, UT_TimerArgCallback, NULL);
    UtAssert_True(OS_ObjectIdEqual(timebase->first_cb, timer_objid_2), "2nd timer add - First CB at timer 2");

    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(timer_objid_2), OS_SUCCESS);

    /* After deleting timer 2 the "first_cb" should be pointing at timer 1 */
    UtAssert_True(OS_ObjectIdEqual(timebase->first_cb, timer_objid_1), "First CB at timer 1");

    /* Re-add timer 2 again */
    OS_TimerAdd(&timer_objid_2, "UT2", timebase_id, UT_TimerArgCallback, NULL);

    /* Sanity check: the "first_cb" should be pointing at timer 2 again */
    UtAssert_True(OS_ObjectIdEqual(timebase->first_cb, timer_objid_2), "First CB at timer 2");

    /* delete timer 1 */
    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(timer_objid_1), OS_SUCCESS);

    /* The "first_cb" should be still pointing at timer 2 */
    UtAssert_True(OS_ObjectIdEqual(timebase->first_cb, timer_objid_2), "First CB at timer 2");

    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(timer_objid_2), OS_SUCCESS);

    /* The "first_cb" should be undefined */
    UtAssert_True(!OS_ObjectIdDefined(timebase->first_cb), "First CB at OS_OBJECT_ID_UNDEFINED");

    /* verify deletion of the dedicated timebase objects
     * these are implicitly created as part of timer creation for API compatibility */
    OS_TimerCreate(&timer_objid_1, "UT1", &accuracy, UT_TimerCallback);
    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(timer_objid_1), OS_SUCCESS);
    UtAssert_STUB_COUNT(OS_TimeBaseDelete, 1);

    /* Cover OS_ObjectIdGetById failure cases for callback ring which can't occur nominally */
    OS_TimerAdd(&timer_objid_1, "UT1", timebase_id, UT_TimerArgCallback, NULL);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 2, OS_ERR_INVALID_ID);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(UT_OBJID_1), OS_SUCCESS);

    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));
    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(UT_OBJID_2), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerDelete(UT_OBJID_2), OS_ERR_INVALID_ID);
}

void Test_OS_TimerGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerGetIdByName (uint32 *timer_id, const char *timer_name)
     */
    osal_id_t objid;

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdFindByName), 1, OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetIdByName(&objid, "UT"), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_TimerGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetIdByName(NULL, "NF"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetIdByName(&objid, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetIdByName(&objid, "NF"), OS_ERR_INCORRECT_OBJ_STATE);
}

void Test_OS_TimerGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerGetInfo (uint32 timer_id, OS_timer_prop_t *timer_prop)
     */
    OS_timer_prop_t timer_prop;

    memset(&timer_prop, 0, sizeof(timer_prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_TIMECB, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OS_timecb_table[1].interval_time           = 2222;
    OS_timecb_table[1].timebase_token.obj_type = OS_OBJECT_TYPE_OS_TIMEBASE;
    OS_timecb_table[1].timebase_token.obj_id   = UT_OBJID_1;
    OS_timecb_table[1].timebase_token.obj_idx  = UT_INDEX_0;
    OS_timebase_table[0].accuracy_usec         = 3333;

    OSAPI_TEST_FUNCTION_RC(OS_TimerGetInfo(UT_OBJID_1, &timer_prop), OS_SUCCESS);
    UtAssert_True(strcmp(timer_prop.name, "ABC") == 0, "timer_prop.name (%s) == ABC", timer_prop.name);
    UtAssert_True(timer_prop.interval_time == 2222, "timer_prop.interval_time (%lu) == 2222",
                  (unsigned long)timer_prop.interval_time);
    UtAssert_True(timer_prop.accuracy == 3333, "timer_prop.accuracy (%lu) == 3333", (unsigned long)timer_prop.accuracy);

    OSAPI_TEST_FUNCTION_RC(OS_TimerGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetInfo(UT_OBJID_1, &timer_prop), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimerGetInfo(UT_OBJID_1, &timer_prop), OS_ERR_INVALID_ID);
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
    ADD_TEST(OS_TimerCbAPI_Init);
    ADD_TEST(OS_TimerAdd);
    ADD_TEST(OS_TimerCreate);
    ADD_TEST(OS_TimerSet);
    ADD_TEST(OS_TimerDelete);
    ADD_TEST(OS_TimerGetIdByName);
    ADD_TEST(OS_TimerGetInfo);
}
