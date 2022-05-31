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

#include "os-shared-timebase.h"
#include "os-shared-time.h"
#include "os-shared-task.h"
#include "os-shared-common.h"

#include "OCS_string.h"
#include <limits.h>

static uint32 TimerSyncCount  = 0;
static uint32 TimerSyncRetVal = 0;
static uint32 TimeCB          = 0;

static uint32 UT_TimerSync(osal_id_t timer_id)
{
    ++TimerSyncCount;
    return TimerSyncRetVal;
}

static int32 ClearObjectsHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    OS_common_record_t *recptr = UserObj;

    if (CallCount >= 10)
    {
        memset(recptr, 0, sizeof(*recptr));
    }
    return StubRetcode;
}

static void UT_TimeCB(osal_id_t object_id, void *arg)
{
    ++TimeCB;
}

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_TimeBaseAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseAPI_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseAPI_Init(), OS_SUCCESS);
}

void Test_OS_TimeBaseCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */
    osal_id_t objid;

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, "UT1", UT_TimerSync), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, "UT2", NULL), OS_SUCCESS);

    /* test error paths */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(NULL, "UT1", UT_TimerSync), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, NULL, UT_TimerSync), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, "UT", UT_TimerSync), OS_ERR_NAME_TOO_LONG);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, "UT", UT_TimerSync), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdAllocateNew), 1, OS_ERR_NO_FREE_IDS);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate(&objid, "UT1", UT_TimerSync), OS_ERR_NO_FREE_IDS);
}

void Test_OS_TimeBaseSet(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, 1000, 1000), OS_SUCCESS);

    /* test error paths: overflow on input */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, UINT32_MAX, 1000), OS_TIMER_ERR_INVALID_ARGS);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, 1000, UINT32_MAX), OS_TIMER_ERR_INVALID_ARGS);

    /* test error paths */
    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, 1000, 1000), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_TimeBaseSet_Impl), 1, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, 1000, 1000), OS_ERROR);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet(UT_OBJID_1, 1000, 1000), OS_ERR_INVALID_ID);
}

void Test_OS_TimeBaseDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete(UT_OBJID_1), OS_SUCCESS);

    /* test error paths */
    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete(UT_OBJID_1), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete(UT_OBJID_1), OS_ERR_INVALID_ID);
}

void Test_OS_TimeBaseGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */
    osal_id_t objid = OS_OBJECT_ID_UNDEFINED;

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdFindByName), 1, OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(&objid, "UT"), OS_SUCCESS);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);

    UT_SetDeferredRetcode(UT_KEY(OCS_memchr), 1, -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(&objid, "TL"), OS_ERR_NAME_TOO_LONG);

    /* test error paths */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(NULL, "UT"), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(&objid, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetIdByName(&objid, "NF"), OS_ERR_INCORRECT_OBJ_STATE);
}

void Test_OS_TimeBaseGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
     */
    OS_timebase_prop_t timebase_prop;

    memset(&timebase_prop, 0, sizeof(timebase_prop));

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OS_timebase_table[1].nominal_interval_time = 2222;
    OS_timebase_table[1].freerun_time          = 3333;
    OS_timebase_table[1].accuracy_usec         = 4444;

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo(UT_OBJID_1, &timebase_prop), OS_SUCCESS);

    OSAPI_TEST_OBJID(timebase_prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(timebase_prop.name, "ABC") == 0, "timebase_prop.name (%s) == ABC", timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 2222, "timebase_prop.nominal_interval_time (%lu) == 2222",
                  (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 3333, "timebase_prop.freerun_time (%lu) == 3333",
                  (unsigned long)timebase_prop.freerun_time);
    UtAssert_True(timebase_prop.accuracy == 4444, "timebase_prop.accuracy (%lu) == 4444",
                  (unsigned long)timebase_prop.accuracy);

    /* test error paths */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetId_Impl), 1, 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo(UT_OBJID_1, &timebase_prop), OS_ERR_INCORRECT_OBJ_STATE);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo(UT_OBJID_1, &timebase_prop), OS_ERR_INVALID_ID);
}

void Test_OS_TimeBaseGetFreeRun(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */
    uint32 freerun = 0xFFFFFFFF;

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetFreeRun(UT_OBJID_1, &freerun), OS_SUCCESS);

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetFreeRun(UT_OBJID_1, NULL), OS_INVALID_POINTER);

    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERR_INVALID_ID);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetFreeRun(UT_OBJID_1, &freerun), OS_ERR_INVALID_ID);
}

void Test_OS_TimeBase_CallbackThread(void)
{
    /*
     * Test Case For:
     * void OS_TimeBase_CallbackThread(uint32 timebase_id)
     */
    OS_common_record_t *recptr;
    OS_object_token_t   timecb_token;
    OS_object_token_t   timecb_token2;

    memset(&timecb_token, 0, sizeof(timecb_token));
    memset(&timecb_token2, 0, sizeof(timecb_token2));

    recptr = &OS_global_timebase_table[2];
    memset(recptr, 0, sizeof(*recptr));
    recptr->active_id = UT_OBJID_2;

    OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, UT_OBJID_1, &timecb_token);
    OS_timebase_table[2].external_sync = UT_TimerSync;
    OS_timebase_table[2].first_cb      = timecb_token.obj_id;
    OS_timecb_table[1].prev_cb         = timecb_token.obj_id;
    OS_timecb_table[1].next_cb         = timecb_token.obj_id;
    OS_timecb_table[1].wait_time       = 2000;
    OS_timecb_table[1].callback_ptr    = UT_TimeCB;
    TimerSyncCount                     = 0;
    TimerSyncRetVal                    = 0;
    TimeCB                             = 0;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_2);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_TimeBase_CallbackThread(UT_OBJID_2);

    UtAssert_True(TimerSyncCount == 11, "TimerSyncCount (%lu) == 11", (unsigned long)TimerSyncCount);

    /* No spin path, first timebase->first_cb token get will error */
    UT_ResetState(UT_KEY(OS_TimeBaseLock_Impl));
    TimerSyncCount    = 0;
    TimerSyncRetVal   = 1000;
    recptr->active_id = UT_OBJID_2;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_2);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_TimeBase_CallbackThread(UT_OBJID_2);

    /* Check that the TimeCB function was called */
    UtAssert_True(TimeCB > 0, "TimeCB (%lu) > 0", (unsigned long)TimeCB);

    /* Error on call to get timebase token */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERROR);
    OS_TimeBase_CallbackThread(UT_OBJID_2);

    /* Other paths for cb logic */
    OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TIMECB, UT_OBJID_2, &timecb_token2);
    OS_timecb_table[1].next_cb = timecb_token2.obj_id;
    OS_timecb_table[2].next_cb = timecb_token.obj_id;
    UT_ResetState(UT_KEY(OS_TimeBaseLock_Impl));
    recptr->active_id = UT_OBJID_2;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_2);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_timecb_table[1].interval_time = 1;
    OS_timecb_table[1].callback_ptr  = NULL;

    /* OS_UT_SetupTestTargetIndex sets the first OS_ObjectIdGetById return, so this sets the second and fourth */
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_ERROR);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 2, OS_ERROR);
    OS_TimeBase_CallbackThread(UT_OBJID_2);
}

void Test_OS_Milli2Ticks(void)
{
    /*
     * Test Case For:
     * int32 OS_Milli2Ticks(uint32 milli_seconds)
     */
    uint32 msec;
    int    ticks = 0;
    int    expected;

    msec                               = 5678;
    OS_SharedGlobalVars.TicksPerSecond = 500;
    UtAssert_INT32_EQ(OS_Milli2Ticks(msec, &ticks), OS_SUCCESS);
    UtAssert_INT32_EQ(ticks, 2839);

    /* Bigger than uint32 but valid case */
    msec     = UINT_MAX - 1;
    expected = (((uint64)msec * OS_SharedGlobalVars.TicksPerSecond) + 999) / 1000;
    UtAssert_INT32_EQ(OS_Milli2Ticks(msec, &ticks), OS_SUCCESS);
    UtAssert_INT32_EQ(ticks, expected);

    /* int rollover case */
    msec = UINT_MAX;
    UtAssert_INT32_EQ(OS_Milli2Ticks(msec, &ticks), OS_ERROR);
    UtAssert_INT32_EQ(ticks, 0);

    /* Max value rollover case */
    OS_SharedGlobalVars.TicksPerSecond = INT_MAX;
    UtAssert_INT32_EQ(OS_Milli2Ticks(msec, &ticks), OS_ERROR);
    UtAssert_INT32_EQ(ticks, 0);
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
    ADD_TEST(OS_TimeBaseAPI_Init);
    ADD_TEST(OS_TimeBaseCreate);
    ADD_TEST(OS_TimeBaseSet);
    ADD_TEST(OS_TimeBaseDelete);
    ADD_TEST(OS_TimeBaseGetIdByName);
    ADD_TEST(OS_TimeBaseGetInfo);
    ADD_TEST(OS_TimeBaseGetFreeRun);
    ADD_TEST(OS_TimeBase_CallbackThread);
    ADD_TEST(OS_Milli2Ticks);
}
