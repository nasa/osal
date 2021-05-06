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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-timebase.h"
#include "os-shared-timebase.h"

#include "OCS_taskLib.h"
#include "OCS_signal.h"
#include "OCS_time.h"
#include "OCS_sysLib.h"
#include "OCS_semLib.h"

void Test_OS_VxWorks_TimeBaseAPI_Impl_Init(void)
{
    /* Test Case For:
     * int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TimeBaseAPI_Impl_Init(), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OCS_sysClkRateGet), -1);
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TimeBaseAPI_Impl_Init(), OS_ERROR);
}

void Test_OS_TimeBaseLock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseLock_Impl(uint32 local_id)
     */
    OS_object_token_t token = UT_TOKEN_0;
    OS_TimeBaseLock_Impl(&token);
}

void Test_OS_TimeBaseUnlock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseUnlock_Impl(uint32 local_id)
     */
    OS_object_token_t token = UT_TOKEN_0;
    OS_TimeBaseUnlock_Impl(&token);
}

static int32 UT_TimeBaseTest_TimeBaseRegHook(void *UserObj, int32 StubRetcode, uint32 CallCount,
                                             const UT_StubContext_t *Context)
{
    UT_TimeBaseTest_SetTimeBaseRegState(UT_INDEX_0, true);
    return 0;
}

void Test_OS_Impl_UsecToTimespec(void)
{
    /* Test Case For:
     * static void OS_Impl_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
     *
     * This is invoked through a wrapper, to get coverage on the function.
     *
     * Note: For some reason, the vxworks implementation does this differently for intervals
     * less than one second vs. one second or more, even though it does not need this
     * complexity/special case, because the "one-or-more" code works for all.  Nonetheless,
     * this needs to exist to test both ways.
     */

    struct OCS_timespec ts;

    memset(&ts, 255, sizeof(ts));
    UT_TimeBaseTest_UsecToTimespec(1, &ts);
    UtAssert_True(ts.tv_sec == 0, "ts.tv_sec (%ld) == 0", (long)ts.tv_sec);
    UtAssert_True(ts.tv_nsec == 1000, "ts.tv_nsec (%ld) == 1000", (long)ts.tv_sec);

    memset(&ts, 255, sizeof(ts));
    UT_TimeBaseTest_UsecToTimespec(1000000000, &ts);
    UtAssert_True(ts.tv_sec == 1000, "ts.tv_sec (%ld) == 1000", (long)ts.tv_sec);
    UtAssert_True(ts.tv_nsec == 0, "ts.tv_nsec (%ld) == 0", (long)ts.tv_sec);
}

void Test_OS_TimeBaseCreate_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseCreate_Impl(uint32 timer_id)
     */
    osal_id_t         id;
    OS_object_token_t token = UT_TOKEN_0;

    /*
     * Test paths though the signal number assignment.
     *
     * This should be done first as it will assign the "external_sync"
     * and therefore cause future calls to skip this block.
     */
    id = OS_ObjectIdFromInteger(OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT);

    OS_global_timebase_table[0].active_id = id;
    UT_TimeBaseTest_Setup(UT_INDEX_0, OCS_SIGRTMIN, false);

    id = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(id) + 1);

    OS_global_timebase_table[1].active_id = id;
    UT_TimeBaseTest_Setup(UT_INDEX_1, 1 + OCS_SIGRTMIN, false);
    UT_SetDefaultReturnValue(UT_KEY(OCS_sigismember), true);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(&token), OS_TIMER_ERR_UNAVAILABLE);
    UT_ResetState(UT_KEY(OCS_sigismember));

    /* fail to initialize the sem */
    UT_SetDefaultReturnValue(UT_KEY(OCS_semMInitialize), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(&token), OS_TIMER_ERR_INTERNAL);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_semMInitialize));

    /* fail to spawn the task */
    UT_SetDefaultReturnValue(UT_KEY(OCS_taskSpawn), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(&token), OS_TIMER_ERR_INTERNAL);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_taskSpawn));

    /*
     * this call to TimeBaseCreate_Impl should also fail, because
     * this mimics the situation where the reg global is never
     * set past OS_TimerRegState_INIT
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(&token), OS_TIMER_ERR_INTERNAL);

    /*
     * Do Nominal/success case now.
     * Using the hook function, this sets the global state to
     * mimic registration success
     */
    UT_SetHookFunction(UT_KEY(OCS_taskSpawn), UT_TimeBaseTest_TimeBaseRegHook, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(&token), OS_SUCCESS);

    /*
     * For coverage, call the  OS_VxWorks_TimeBaseTask() function.
     */
    UT_TimeBaseTest_CallHelperTaskFunc(0);

    /*
     * Check outputs of OS_VxWorks_RegisterTimer() function.
     */
    UT_TimeBaseTest_ClearTimeBaseRegState(UT_INDEX_1);
    UT_TimeBaseTest_CallRegisterTimer(OS_OBJECT_ID_UNDEFINED);
    UtAssert_True(UT_TimeBaseTest_CheckTimeBaseRegisteredState(UT_INDEX_0), "timer successfully registered");

    UT_TimeBaseTest_ClearTimeBaseRegState(UT_INDEX_1);
    UT_SetDefaultReturnValue(UT_KEY(OCS_timer_create), -1);
    UT_TimeBaseTest_CallRegisterTimer(OS_OBJECT_ID_UNDEFINED);
    UtAssert_True(UT_TimeBaseTest_CheckTimeBaseErrorState(UT_INDEX_0), "timer registration failure state");

    UT_TimeBaseTest_ClearTimeBaseRegState(UT_INDEX_1);
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    UT_TimeBaseTest_CallRegisterTimer(OS_OBJECT_ID_UNDEFINED);
    UtAssert_True(!UT_TimeBaseTest_CheckTimeBaseRegisteredState(UT_INDEX_0), "timer registration bad ID");
}

void Test_OS_VxWorks_SigWait(void)
{
    /* Test Case For:
     * static uint32 OS_VxWorks_SigWait(uint32 local_id)
     * (invocation of static function through a wrapper)
     */
    int                   signo = OCS_SIGRTMIN;
    struct OCS_itimerspec config_value;
    osal_id_t             id;
    OS_object_token_t     token = UT_TOKEN_0;

    memset(&id, 0x02, sizeof(id));
    OS_global_timebase_table[0].active_id      = id;
    OS_timebase_table[0].nominal_start_time    = 8888;
    OS_timebase_table[0].nominal_interval_time = 5555;

    memset(&config_value, 0, sizeof(config_value));
    UT_SetDataBuffer(UT_KEY(OCS_timer_settime), &config_value, sizeof(config_value), false);
    UT_SetDataBuffer(UT_KEY(OCS_timer_gettime), &config_value, sizeof(config_value), false);
    UT_TimeBaseTest_Setup(UT_INDEX_0, signo, true);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(&token, 1111111, 2222222), OS_SUCCESS);

    UT_SetDataBuffer(UT_KEY(OCS_timer_settime), &config_value, sizeof(config_value), false);
    UT_SetDeferredRetcode(UT_KEY(OCS_timer_gettime), 1, OCS_ERROR);
    UT_TimeBaseTest_Setup(UT_INDEX_0, signo, true);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(&token, 1111111, 2222222), OS_ERROR);

    UT_SetDataBuffer(UT_KEY(OCS_sigwait), &signo, sizeof(signo), false);
    OSAPI_TEST_FUNCTION_RC(UT_TimeBaseTest_CallSigWaitFunc(OS_OBJECT_ID_UNDEFINED), 1111111);
    UT_SetDataBuffer(UT_KEY(OCS_sigwait), &signo, sizeof(signo), false);
    OSAPI_TEST_FUNCTION_RC(UT_TimeBaseTest_CallSigWaitFunc(OS_OBJECT_ID_UNDEFINED), 2222222);
    UT_SetDataBuffer(UT_KEY(OCS_sigwait), &signo, sizeof(signo), false);
    OSAPI_TEST_FUNCTION_RC(UT_TimeBaseTest_CallSigWaitFunc(OS_OBJECT_ID_UNDEFINED), 2222222);

    UT_TimeBaseTest_Setup(UT_INDEX_0, 0, false);
    OS_global_timebase_table[0].active_id      = OS_OBJECT_ID_UNDEFINED;
    OS_timebase_table[0].nominal_interval_time = 0;
}

void Test_OS_TimeBaseSet_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseSet_Impl(uint32 timer_id, int32 start_time, int32 interval_time)
     */
    OS_object_token_t token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(&token, 1, 1), OS_ERR_NOT_IMPLEMENTED);

    UT_TimeBaseTest_Setup(UT_INDEX_0, OCS_SIGRTMIN, false);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(&token, 1, 1), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_timer_settime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(&token, 1, 1), OS_TIMER_ERR_INVALID_ARGS);
}

void Test_OS_TimeBaseDelete_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseDelete_Impl(uint32 timer_id)
     */
    OS_object_token_t token = UT_TOKEN_0;

    UT_TimeBaseTest_Setup(UT_INDEX_0, OCS_SIGRTMIN, false);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete_Impl(&token), OS_SUCCESS);
}

void Test_OS_TimeBaseGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseGetInfo_Impl (uint32 timer_id, OS_timebase_prop_t *timer_prop)
     */
    OS_timebase_prop_t timer_prop;
    OS_object_token_t  token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo_Impl(&token, &timer_prop), OS_SUCCESS);
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

    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));
    memset(OS_global_timebase_table, 0, sizeof(OS_common_record_t) * OS_MAX_BIN_SEMAPHORES);

    memset(UT_Ref_OS_impl_timebase_table, 0, UT_Ref_OS_impl_timebase_table_SIZE);
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
    ADD_TEST(OS_VxWorks_TimeBaseAPI_Impl_Init);
    ADD_TEST(OS_TimeBaseLock_Impl);
    ADD_TEST(OS_TimeBaseUnlock_Impl);
    ADD_TEST(OS_TimeBaseCreate_Impl);
    ADD_TEST(OS_VxWorks_SigWait);
    ADD_TEST(OS_TimeBaseSet_Impl);
    ADD_TEST(OS_TimeBaseDelete_Impl);
    ADD_TEST(OS_TimeBaseGetInfo_Impl);
    ADD_TEST(OS_Impl_UsecToTimespec);
}
