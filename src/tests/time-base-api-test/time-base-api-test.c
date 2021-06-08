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

/*
 * Filename: time-base-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-timebase"
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

static int32 SyncTimeBaseCreateRc    = 0;
static int32 SyncTimeBaseDeleteRc    = 0;
static int32 SyncTimeBaseSetRc       = 0;
static int32 SyncTimeBaseGetByNameRc = 0;
static int32 SyncTimeBaseGetInfoRc   = 0;

static OS_timebase_prop_t SyncTimeBaseProp;

static uint32 NumSyncs = 0;

static uint32 UT_TimerSync(osal_id_t timer_id)
{
    /*
     * Calls to time base configuration from the context of a sync function
     * should be rejected with OS_ERR_INCORRECT_OBJ_STATE.  However because
     * UtAssert is not fully thread-safe, this does not assert here, it just
     * calls the various functions on the first time through and stores the
     * result, which is checked/asserted in the main task.
     */
    if (NumSyncs == 0)
    {
        SyncTimeBaseCreateRc    = OS_TimeBaseCreate(&timer_id, "sync", 0);
        SyncTimeBaseDeleteRc    = OS_TimeBaseDelete(timer_id);
        SyncTimeBaseSetRc       = OS_TimeBaseSet(timer_id, 100, 100);
        SyncTimeBaseGetByNameRc = OS_TimeBaseGetIdByName(&timer_id, "TimeBaseC");
        SyncTimeBaseGetInfoRc   = OS_TimeBaseGetInfo(timer_id, &SyncTimeBaseProp);
    }
    ++NumSyncs;
    OS_TaskDelay(1);
    return 1;
}

/* *************************************** MAIN ************************************** */

void TestTimeBaseApi(void)
{
    int32              expected;
    int32              actual;
    uint32             freerun;
    osal_id_t          objid;
    osal_id_t          time_base_id;
    osal_id_t          time_base_id2;
    osal_id_t          tb_id[OS_MAX_TIMEBASES];
    char               timebase_name[OS_MAX_API_NAME + 5];
    OS_timebase_prop_t timebase_prop;

    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */

    /* Test for invalid inputs */
    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseCreate(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseCreate(NULL, "TimeBase6", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseCreate(&time_base_id, NULL, 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    memset(timebase_name, 'x', sizeof(timebase_name));
    timebase_name[sizeof(timebase_name) - 1] = 0;
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, timebase_name, 0), OS_ERR_NAME_TOO_LONG);

    /* Checking for OS_MAX_TIMEBASES */
    for (int i = 0; i < OS_MAX_TIMEBASES; i++)
    {
        /* On the final setup pass, while there is still one free slot,
         * check attempting to create a duplicate name (index 0) - this
         * should be rejected and _not_ consume the last empty slot */
        if (i == (OS_MAX_TIMEBASES - 1))
        {
            UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, "TimeBase0", 0), OS_ERR_NAME_TAKEN);
        }

        snprintf(timebase_name, sizeof(timebase_name), "TimeBase%d", i);
        UtAssert_INT32_EQ(OS_TimeBaseCreate(&tb_id[i], timebase_name, 0), OS_SUCCESS);
    }

    /* Checking OS_MAX_TIMEBASES + 1 */
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, "overMaxTimeBase", 0), OS_ERR_NO_FREE_IDS);

    /* reset test environment */
    OS_DeleteAllObjects();

    /* Test for nominal inputs - these resources are used for the remainder of test */
    expected = OS_SUCCESS;

    actual = OS_TimeBaseCreate(&time_base_id, "TimeBaseA", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&time_base_id2, "TimeBaseB", NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&time_base_id, "TimeBaseC", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    /* Let the TimeBaseC accumulate at least one sync, so it will
     * attempt to call timebase APIs from its own context. */
    while (NumSyncs == 0)
    {
        OS_TaskDelay(1);
    }

    /* Check that those configuration attempts all returned OS_ERR_INCORRECT_OBJ_STATE */
    UtAssert_True(SyncTimeBaseCreateRc == OS_ERR_INCORRECT_OBJ_STATE,
                  "OS_TimeBaseCreate(&timer_id, \"sync\", 0) (%d) == OS_ERR_INCORRECT_OBJ_STATE",
                  (int)SyncTimeBaseCreateRc);
    UtAssert_True(SyncTimeBaseDeleteRc == OS_ERR_INCORRECT_OBJ_STATE,
                  "OS_TimeBaseDelete(timer_id) (%d) == OS_ERR_INCORRECT_OBJ_STATE", (int)SyncTimeBaseDeleteRc);
    UtAssert_True(SyncTimeBaseSetRc == OS_ERR_INCORRECT_OBJ_STATE,
                  "OS_TimeBaseSet(timer_id, 100, 100) (%d) == OS_ERR_INCORRECT_OBJ_STATE", (int)SyncTimeBaseSetRc);
    UtAssert_True(SyncTimeBaseGetByNameRc == OS_ERR_INCORRECT_OBJ_STATE,
                  "OS_TimeBaseGetIdByName(&timer_id, \"TimeBaseC\") (%d) == OS_ERR_INCORRECT_OBJ_STATE",
                  (int)SyncTimeBaseGetByNameRc);
    UtAssert_True(SyncTimeBaseGetInfoRc == OS_ERR_INCORRECT_OBJ_STATE,
                  "OS_TimeBaseGetInfo(timer_id, &SyncTimeBaseProp) (%d) == OS_ERR_INCORRECT_OBJ_STATE",
                  (int)SyncTimeBaseGetInfoRc);

    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */

    /* Test for nominal inputs */
    expected = OS_SUCCESS;
    actual   = OS_TimeBaseSet(time_base_id, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_SUCCESS;
    actual   = OS_TimeBaseSet(time_base_id, 0, 0);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    /* overflow on input */
    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual   = OS_TimeBaseSet(time_base_id, UINT32_MAX, UINT32_MAX);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual   = OS_TimeBaseSet(time_base_id, -1000, -1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual   = OS_TimeBaseSet(time_base_id, 1000, -1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual   = OS_TimeBaseSet(time_base_id, -1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual   = OS_TimeBaseSet(OS_OBJECT_ID_UNDEFINED, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */

    /* Test for nominal inputs */
    expected = OS_SUCCESS;
    actual   = OS_TimeBaseDelete(time_base_id);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    actual   = OS_TimeBaseDelete(OS_OBJECT_ID_UNDEFINED);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate and id was set to time_base_id2 */
    expected = OS_SUCCESS;
    objid    = OS_OBJECT_ID_UNDEFINED;
    actual   = OS_TimeBaseGetIdByName(&objid, "TimeBaseB");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_ObjectIdEqual(objid, time_base_id2), "OS_TimeBaseGetIdByName() objid (%lu) Matches!",
                  OS_ObjectIdToInteger(objid));

    /* Test for invalid inputs */
    expected = OS_ERR_NAME_NOT_FOUND;
    objid    = OS_OBJECT_ID_UNDEFINED;
    actual   = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
    UtAssert_True(!OS_ObjectIdDefined(objid), "OS_TimeBaseGetIdByName() objid (%lu) still OS_OBJECT_ID_UNDEFINED",
                  OS_ObjectIdToInteger(objid));

    /* check each pointer input individually */
    UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(NULL, "NF"), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&objid, NULL), OS_INVALID_POINTER);

    memset(timebase_name, 'x', sizeof(timebase_name));
    timebase_name[sizeof(timebase_name) - 1] = 0;
    UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&objid, timebase_name), OS_ERR_NAME_TOO_LONG);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
     */
    expected = OS_SUCCESS;

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */

    /* Initializing timebase_prop values to something other than time_base_id2 to ensure they have changed */
    memset(&timebase_prop, 0x55, sizeof(timebase_prop));

    actual = OS_TimeBaseGetInfo(time_base_id2, &timebase_prop);

    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(!OS_ObjectIdDefined(timebase_prop.creator), "timebase_prop.creator (%lu) undefined",
                  OS_ObjectIdToInteger(timebase_prop.creator));
    UtAssert_True(strcmp(timebase_prop.name, "TimeBaseB") == 0, "timebase_prop.name (%s) == TimeBaseB",
                  timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 0, "timebase_prop.nominal_interval_time (%lu) == 0",
                  (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 0, "timebase_prop.freerun_time (%lu) == 0",
                  (unsigned long)timebase_prop.freerun_time);
    UtAssert_True(timebase_prop.accuracy >= 0, "timebase_prop.accuracy (%lu) >= 0",
                  (unsigned long)timebase_prop.accuracy);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    actual   = OS_TimeBaseGetInfo(OS_OBJECT_ID_UNDEFINED, &timebase_prop);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_TimeBaseGetInfo(time_base_id2, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */
    expected = OS_SUCCESS;

    freerun = 0xFFFFFFFF;
    actual  = OS_TimeBaseGetFreeRun(time_base_id2, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

    freerun = 0x0000000;
    actual  = OS_TimeBaseGetFreeRun(time_base_id2, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(OS_OBJECT_ID_UNDEFINED, &freerun), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(time_base_id2, NULL), OS_INVALID_POINTER);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    freerun  = 0xFFFFFFFF;
    actual   = OS_TimeBaseGetFreeRun(OS_OBJECT_ID_UNDEFINED, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

} /* end TestTimeBaseApi */

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestTimeBaseApi, NULL, NULL, "TestTimeBaseApi");
}
