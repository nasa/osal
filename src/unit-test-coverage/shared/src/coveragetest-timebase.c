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

/**
 * \file     coveragetest-timebase.c
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
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TimeBaseAPI_Init();

    UtAssert_True(actual == expected, "OS_TimeBaseAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_TimeBaseCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */
    int32     expected = OS_SUCCESS;
    osal_id_t objid;
    int32     actual;

    actual = OS_TimeBaseCreate(&objid, "UT1", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&objid, "UT2", NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseCreate(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    expected = OS_ERR_NAME_TOO_LONG;
    actual   = OS_TimeBaseCreate(&objid, "UT", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual   = OS_TimeBaseCreate(&objid, "UT", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseSet(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TimeBaseSet(UT_OBJID_1, 1000, 1000);

    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths: overflow on input */
    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual   = OS_TimeBaseSet(UT_OBJID_1, UINT32_MAX, UINT32_MAX);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    /* test error paths */
    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual   = OS_TimeBaseSet(UT_OBJID_1, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_TimeBaseDelete(UT_OBJID_1);

    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths */
    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual   = OS_TimeBaseDelete(UT_OBJID_1);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */
    int32     expected = OS_SUCCESS;
    int32     actual   = ~OS_SUCCESS;
    osal_id_t objid;

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_TimeBaseGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    OSAPI_TEST_OBJID(objid, !=, OS_OBJECT_ID_UNDEFINED);
    UT_ClearDefaultReturnValue(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == %ld", (long)actual, (long)expected);

    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseGetIdByName(NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual   = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
     */
    int32              expected = OS_SUCCESS;
    int32              actual   = ~OS_SUCCESS;
    OS_timebase_prop_t timebase_prop;

    OS_UT_SetupBasicInfoTest(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_1, "ABC", UT_OBJID_OTHER);

    OS_timebase_table[1].nominal_interval_time = 2222;
    OS_timebase_table[1].freerun_time          = 3333;
    OS_timebase_table[1].accuracy_usec         = 4444;

    actual = OS_TimeBaseGetInfo(UT_OBJID_1, &timebase_prop);

    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    OSAPI_TEST_OBJID(timebase_prop.creator, ==, UT_OBJID_OTHER);
    UtAssert_True(strcmp(timebase_prop.name, "ABC") == 0, "timebase_prop.name (%s) == ABC", timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 2222, "timebase_prop.nominal_interval_time (%lu) == 2222",
                  (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 3333, "timebase_prop.freerun_time (%lu) == 3333",
                  (unsigned long)timebase_prop.freerun_time);
    UtAssert_True(timebase_prop.accuracy == 4444, "timebase_prop.accuracy (%lu) == 4444",
                  (unsigned long)timebase_prop.accuracy);

    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseGetInfo(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetDefaultReturnValue(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual   = OS_TimeBaseGetInfo(UT_OBJID_1, &timebase_prop);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseGetFreeRun(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */
    int32  expected = OS_SUCCESS;
    uint32 freerun  = 0xFFFFFFFF;
    int32  actual   = OS_TimeBaseGetFreeRun(UT_OBJID_1, &freerun);

    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseGetFreeRun(UT_OBJID_1, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_TimeBase_CallbackThread(void)
{
    /*
     * Test Case For:
     * void OS_TimeBase_CallbackThread(uint32 timebase_id)
     */
    OS_common_record_t *recptr;
    OS_object_token_t   timecb_token;

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

    UT_ResetState(UT_KEY(OS_TimeBaseLock_Impl));
    TimerSyncCount    = 0;
    TimerSyncRetVal   = 1000;
    recptr->active_id = UT_OBJID_2;
    OS_UT_SetupTestTargetIndex(OS_OBJECT_TYPE_OS_TIMEBASE, UT_INDEX_2);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_TimeBase_CallbackThread(UT_OBJID_2);

    /* Check that the TimeCB function was called */
    UtAssert_True(TimeCB > 0, "TimeCB (%lu) > 0", (unsigned long)TimeCB);

    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OS_TimeBase_CallbackThread(UT_OBJID_2);
}

void Test_OS_Milli2Ticks(void)
{
    /*
     * Test Case For:
     * int32 OS_Milli2Ticks(uint32 milli_seconds)
     */
    uint32 msec;
    int    ticks;
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
