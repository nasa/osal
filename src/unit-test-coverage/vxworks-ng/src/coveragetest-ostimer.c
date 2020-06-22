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
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "ostimer-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"
#include "ut-ostimer.h"

#include <overrides/taskLib.h>
#include <overrides/signal.h>


void Test_OS_VxWorks_TimeBaseAPI_Impl_Init(void)
{
    /* Test Case For:
     * int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_TimeBaseAPI_Impl_Init(), OS_SUCCESS);
}


void Test_OS_TimeBaseLock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseLock_Impl(uint32 local_id)
     */
    OS_TimeBaseLock_Impl(0);
}

void Test_OS_TimeBaseUnlock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseUnlock_Impl(uint32 local_id)
     */
    OS_TimeBaseUnlock_Impl(0);
}

static int32 Osapi_Internal_TimeBaseRegHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    Osapi_Internal_SetTimeBaseRegState(0, true);
    return 0;
}

void Test_OS_TimeBaseCreate_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseCreate_Impl(uint32 timer_id)
     */

    /* fail to spawn the task */
    UT_SetForceFail(UT_KEY(OCS_taskSpawn), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_INTERNAL);

    /*
     * this call to TimeBaseCreate_Impl should also fail, because
     * this mimics the situation where the reg global is never
     * set past OS_TimerRegState_INIT
     */
    UT_ClearForceFail(UT_KEY(OCS_taskSpawn));
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_INTERNAL);

    /*
     * Using the hook function, this sets the global state to
     * mimic registration success
     */
    UT_SetHookFunction(UT_KEY(OCS_taskSpawn), Osapi_Internal_TimeBaseRegHook, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_SUCCESS);

}

void Test_OS_VxWorks_SigWait(void)
{
    /* Test Case For:
     * static uint32 OS_VxWorks_SigWait(uint32 local_id)
     * (invocation of static function through a wrapper)
     */
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallSigWaitFunc(0), OS_SUCCESS);
}

void Test_OS_TimeBaseSet_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseSet_Impl(uint32 timer_id, int32 start_time, int32 interval_time)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(0,1,1), OS_ERR_NOT_IMPLEMENTED);
    Osapi_Internal_Setup(0, OCS_SIGRTMIN);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(0,1,1), OS_SUCCESS);
}

void Test_OS_TimeBaseDelete_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseDelete_Impl(uint32 timer_id)
     */
    Osapi_Internal_Setup(0, OCS_SIGRTMIN);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_TimeBaseGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseGetInfo_Impl (uint32 timer_id, OS_timebase_prop_t *timer_prop)
     */
    OS_timebase_prop_t timer_prop;
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo_Impl(0,&timer_prop), OS_SUCCESS);
}

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
    Osapi_Internal_ResetState();
}

/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_VxWorks_TimeBaseAPI_Impl_Init);
    ADD_TEST(OS_TimeBaseLock_Impl);
    ADD_TEST(OS_TimeBaseUnlock_Impl);
    ADD_TEST(OS_TimeBaseCreate_Impl);
    ADD_TEST(OS_VxWorks_SigWait);
    ADD_TEST(OS_TimeBaseSet_Impl);
    ADD_TEST(OS_TimeBaseDelete_Impl);
    ADD_TEST(OS_TimeBaseGetInfo_Impl);
}

