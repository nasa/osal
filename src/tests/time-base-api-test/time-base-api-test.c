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

OS_timebase_prop_t SyncTimeBaseProp;

uint32 NumSyncs = 0;

static uint32 UT_TimerSync(osal_id_t timer_id)
{
    /*
     * Calls to time base configuration from the context of a sync function
     * should be rejected with OS_ERR_INCORRECT_OBJ_STATE.  Note that only the
     * POSIX provides the mechanism for this error check to actually work - On
     * other platforms the error checking may not be possible, depending on how
     * OS_TaskGetId_Impl() responds when called from a non-OSAL task.
     */
#ifdef _POSIX_OS_
    if (NumSyncs == 0)
    {
        UtAssert_INT32_EQ(OS_TimeBaseCreate(&timer_id, "sync", 0), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimeBaseDelete(timer_id), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimeBaseSet(timer_id, 100, 100), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&timer_id, "TimeBaseC"), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimeBaseGetInfo(timer_id, &SyncTimeBaseProp), OS_ERR_INCORRECT_OBJ_STATE);
    }
#endif

    ++NumSyncs;
    OS_TaskDelay(1);
    return 1;
}

/* *************************************** MAIN ************************************** */

void TestTimeBaseApi(void)
{
    uint32             freerun;
    osal_id_t          objid;
    osal_id_t          badid;
    osal_id_t          time_base_id  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t          time_base_id2 = OS_OBJECT_ID_UNDEFINED;
    osal_id_t          tb_id[OS_MAX_TIMEBASES];
    char               timebase_name[OS_MAX_API_NAME + 5];
    OS_timebase_prop_t timebase_prop;

    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */

    /* Test for invalid inputs */
    UtAssert_INT32_EQ(OS_TimeBaseCreate(NULL, "TimeBase6", 0), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, NULL, 0), OS_INVALID_POINTER);

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
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, "TimeBaseA", 0), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id2, "TimeBaseB", NULL), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TimeBaseCreate(&time_base_id, "TimeBaseC", UT_TimerSync), OS_SUCCESS);

    /* Let the TimeBaseC accumulate at least one sync, so it will
     * attempt to call timebase APIs from its own context. */
    while (NumSyncs == 0)
    {
        OS_TaskDelay(1);
    }

    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */

    /* Test for nominal inputs */
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, 1000, 1000), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, 0, 0), OS_SUCCESS);

    /* Test for invalid inputs */
    /* create a bad ID by flipping the bits of a good ID */
    badid = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(time_base_id2) ^ 0xFFFFFFFF);

    /* overflow on input */
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, UINT32_MAX, UINT32_MAX), OS_TIMER_ERR_INVALID_ARGS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, -1000, -1000), OS_TIMER_ERR_INVALID_ARGS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, 1000, -1000), OS_TIMER_ERR_INVALID_ARGS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(time_base_id, -1000, 1000), OS_TIMER_ERR_INVALID_ARGS);
    UtAssert_INT32_EQ(OS_TimeBaseSet(OS_OBJECT_ID_UNDEFINED, 1000, 1000), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseSet(badid, 1000, 1000), OS_ERR_INVALID_ID);

    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */

    /* Test for nominal inputs */
    UtAssert_INT32_EQ(OS_TimeBaseDelete(time_base_id), OS_SUCCESS);

    /* Test for invalid inputs */
    UtAssert_INT32_EQ(OS_TimeBaseDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseDelete(badid), OS_ERR_INVALID_ID);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate and id was set to time_base_id2 */
    objid = OS_OBJECT_ID_UNDEFINED;
    UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&objid, "TimeBaseB"), OS_SUCCESS);
    UtAssert_True(OS_ObjectIdEqual(objid, time_base_id2), "OS_TimeBaseGetIdByName() objid (%lu) Matches!",
                  OS_ObjectIdToInteger(objid));

    /* Test for invalid inputs */
    objid = OS_OBJECT_ID_UNDEFINED;
    UtAssert_INT32_EQ(OS_TimeBaseGetIdByName(&objid, "NF"), OS_ERR_NAME_NOT_FOUND);
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

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */

    /* Initializing timebase_prop values to something other than time_base_id2 to ensure they have changed */
    memset(&timebase_prop, 0x55, sizeof(timebase_prop));

    UtAssert_INT32_EQ(OS_TimeBaseGetInfo(time_base_id2, &timebase_prop), OS_SUCCESS);

    UtAssert_True(!OS_ObjectIdDefined(timebase_prop.creator), "timebase_prop.creator (%lu) undefined",
                  OS_ObjectIdToInteger(timebase_prop.creator));
    UtAssert_True(strcmp(timebase_prop.name, "TimeBaseB") == 0, "timebase_prop.name (%s) == TimeBaseB",
                  timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 0, "timebase_prop.nominal_interval_time (%lu) == 0",
                  (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 0, "timebase_prop.freerun_time (%lu) == 0",
                  (unsigned long)timebase_prop.freerun_time);

    /* Test for invalid inputs */
    UtAssert_INT32_EQ(OS_TimeBaseGetInfo(OS_OBJECT_ID_UNDEFINED, &timebase_prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseGetInfo(badid, &timebase_prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseGetInfo(time_base_id2, NULL), OS_INVALID_POINTER);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */
    freerun = 0xFFFFFFFF;
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(time_base_id2, &freerun), OS_SUCCESS);

    freerun = 0x0000000;
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(time_base_id2, &freerun), OS_SUCCESS);

    /* Test for invalid inputs */
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(OS_OBJECT_ID_UNDEFINED, &freerun), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(badid, &freerun), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_TimeBaseGetFreeRun(time_base_id2, NULL), OS_INVALID_POINTER);
}

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
