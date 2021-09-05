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
 * \file     coveragetest-common.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-common.h"
#include "os-shared-task.h"
#include "os-shared-timebase.h"

#include "OCS_stdlib.h"

/*
** OS_CleanUpObject() is an internal helper function.
** It is not exposed in the public API as it is not intended to be called directly.
** However the coverage test case needs to invoke it directly to test it.
*/
extern void OS_CleanUpObject(osal_id_t object_id, void *arg);

int32 Test_MicroSecPerTick = 0;
int32 Test_TicksPerSecond  = 0;

/*
**********************************************************************************
**          HOOK/CALLBACK FUNCTIONS
**********************************************************************************
*/

/* as a side effect, the OS_TimeBaseAPI_Init must initialize the globals */
static int32 TimeBaseInitGlobal(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    OS_SharedGlobalVars.MicroSecPerTick = Test_MicroSecPerTick;
    OS_SharedGlobalVars.TicksPerSecond  = Test_TicksPerSecond;
    return StubRetcode;
}

static int32 ObjectDeleteCountHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    uint32 *counter = UT_Hook_GetArgValueByName(Context, "callback_arg", uint32 *);

    if (CallCount < 2)
    {
        *counter = 1;
    }
    else
    {
        *counter = 0;
    }

    return StubRetcode;
}

/* Always returns 1 so TryCount will be exceeded */
static int32 ObjectDeleteFailHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{

    uint32 *counter = UT_Hook_GetArgValueByName(Context, "callback_arg", uint32 *);

    *counter = 1;

    return StubRetcode;
}

static int32 SetShutdownFlagHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    OS_ApplicationShutdown(true);
    return StubRetcode;
}

static int32 TestEventHandlerHook(OS_Event_t event, osal_id_t object_id, void *data)
{
    return UT_DEFAULT_IMPL(TestEventHandlerHook);
}

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/
void Test_OS_API_Init(void)
{
    /* Setup Inputs */
    UT_SetHookFunction(UT_KEY(OS_TimeBaseAPI_Init), TimeBaseInitGlobal, NULL);

    /* Execute Test */
    Test_MicroSecPerTick            = 0;
    Test_TicksPerSecond             = 0;
    OS_SharedGlobalVars.GlobalState = 0;
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), OS_ERROR);
    UtAssert_UINT32_EQ(OS_SharedGlobalVars.GlobalState, OS_SHUTDOWN_MAGIC_NUMBER);

    /* TicksPerSec == 0 branch */
    Test_MicroSecPerTick            = 1;
    Test_TicksPerSecond             = 0;
    OS_SharedGlobalVars.GlobalState = 0;
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), OS_ERROR);
    UtAssert_UINT32_EQ(OS_SharedGlobalVars.GlobalState, OS_SHUTDOWN_MAGIC_NUMBER);

    Test_MicroSecPerTick            = 1000;
    Test_TicksPerSecond             = 1000;
    OS_SharedGlobalVars.GlobalState = 0;
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), OS_SUCCESS);

    Test_MicroSecPerTick            = 1000;
    Test_TicksPerSecond             = 1001;
    OS_SharedGlobalVars.GlobalState = 0;
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), OS_SUCCESS);
    UtAssert_UINT32_EQ(OS_SharedGlobalVars.GlobalState, OS_INIT_MAGIC_NUMBER);

    /* Second call should return SUCCESS (but is a no-op) */
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), OS_SUCCESS);
    UtAssert_UINT32_EQ(OS_SharedGlobalVars.GlobalState, OS_INIT_MAGIC_NUMBER);

    /* other error paths */
    OS_SharedGlobalVars.GlobalState = 0;
    UT_SetDefaultReturnValue(UT_KEY(OS_ObjectIdInit), -222);
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), -222);
    UT_ResetState(UT_KEY(OS_ObjectIdInit));

    OS_SharedGlobalVars.GlobalState = 0;
    UT_SetDefaultReturnValue(UT_KEY(OS_API_Impl_Init), -333);
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), -333);
    UT_ResetState(UT_KEY(OS_API_Impl_Init));

    OS_SharedGlobalVars.GlobalState = 0;
    UT_SetDefaultReturnValue(UT_KEY(OS_TaskAPI_Init), -444);
    OSAPI_TEST_FUNCTION_RC(OS_API_Init(), -444);
    UT_ResetState(UT_KEY(OS_TaskAPI_Init));
}

void Test_OS_API_Teardown(void)
{
    /*
     * Test Case For:
     * void OS_API_Teardown(void);
     */

    /* Just need to call the API for coverage; there are no conditionals
     * and the internal functions are each tested separately */
    OS_API_Teardown();
}

void Test_OS_ApplicationExit(void)
{
    /*
     * Test Case For:
     * void OS_ApplicationExit(int32 Status);
     */
    uint32 CallCount = 0;
    OS_ApplicationExit(OS_SUCCESS);
    OS_ApplicationExit(OS_ERROR);
    CallCount = UT_GetStubCount(UT_KEY(OCS_exit));

    UtAssert_True(CallCount == 2, "exit() call count (%lu) == 2", (unsigned long)CallCount);
}

void Test_OS_CleanUpObject(void)
{
    uint32        objtype;
    uint32        CallCount;
    uint32        ActualObjs;
    uint32        ExpObjs;
    UT_EntryKey_t delhandler;

    ActualObjs = 0;
    ExpObjs    = 0;
    objtype    = OS_OBJECT_TYPE_UNDEFINED;
    while (objtype < OS_OBJECT_TYPE_USER)
    {
        UT_ResetState(0);
        UT_SetDefaultReturnValue(UT_KEY(OS_IdentifyObject), objtype);

        switch (objtype)
        {
            case OS_OBJECT_TYPE_OS_TASK:
                delhandler = UT_KEY(OS_TaskDelete);
                break;
            case OS_OBJECT_TYPE_OS_QUEUE:
                delhandler = UT_KEY(OS_QueueDelete);
                break;
            case OS_OBJECT_TYPE_OS_BINSEM:
                delhandler = UT_KEY(OS_BinSemDelete);
                break;
            case OS_OBJECT_TYPE_OS_COUNTSEM:
                delhandler = UT_KEY(OS_CountSemDelete);
                break;
            case OS_OBJECT_TYPE_OS_MUTEX:
                delhandler = UT_KEY(OS_MutSemDelete);
                break;
            case OS_OBJECT_TYPE_OS_MODULE:
                delhandler = UT_KEY(OS_ModuleUnload);
                break;
            case OS_OBJECT_TYPE_OS_TIMEBASE:
                delhandler = UT_KEY(OS_TimeBaseDelete);
                break;
            case OS_OBJECT_TYPE_OS_TIMECB:
                delhandler = UT_KEY(OS_TimerDelete);
                break;
            case OS_OBJECT_TYPE_OS_STREAM:
                delhandler = UT_KEY(OS_close);
                break;
            case OS_OBJECT_TYPE_OS_DIR:
                delhandler = UT_KEY(OS_DirectoryClose);
                break;
            default:
                delhandler = 0;
                break;
        }

        if (delhandler != 0)
        {
            /* note the return code here is ignored -
             * the goal is simply to defeat the default
             * check that the objid was valid (it isn't) */
            UT_SetDefaultReturnValue(delhandler, OS_ERROR);
            OS_CleanUpObject(OS_OBJECT_ID_UNDEFINED, &ActualObjs);

            CallCount = UT_GetStubCount(delhandler);
            UtAssert_True(CallCount == 1, "Objtype %lu call count (%lu) == 1", (unsigned long)objtype,
                          (unsigned long)CallCount);
        }
        else
        {
            OS_CleanUpObject(OS_OBJECT_ID_UNDEFINED, &ActualObjs);
        }
        ++objtype;
        ++ExpObjs;
    }

    UtAssert_True(ActualObjs == ExpObjs, "Total objects cleaned up (%lu) == %lu", (unsigned long)ActualObjs,
                  (unsigned long)ExpObjs);
}

void Test_OS_DeleteAllObjects(void)
{
    /*
     * Test Case For:
     * void OS_DeleteAllObjects(void);
     *
     * This function returns no status; it
     * just invokes OS_ForEachObject() with
     * the callback set to OS_CleanUpObject().
     *
     * OS_CleanUpObject() will be covered separately.
     */

    /*
     * The "ForEachObject" API will be invoked in a loop.
     * For the first pass, have it output nonzero
     * For the second pass, have it output zero top stop the loop
     */
    UT_SetHookFunction(UT_KEY(OS_ForEachObject), ObjectDeleteCountHook, NULL);

    /*
     * This gets coverage of the function but
     * there is nothing to assert/verify for postconditions here
     */
    OS_DeleteAllObjects();

    /* Exceed TryCount */
    UT_SetHookFunction(UT_KEY(OS_ForEachObject), ObjectDeleteFailHook, NULL);
    OS_DeleteAllObjects();
}

void Test_OS_IdleLoopAndShutdown(void)
{
    /*
     * Test Case For:
     * void OS_ApplicationShutdown(uint8 flag);
     * void OS_IdleLoop(void);
     */
    uint32 CallCount = 0;

    OS_SharedGlobalVars.GlobalState = OS_INIT_MAGIC_NUMBER;

    UT_SetHookFunction(UT_KEY(OS_IdleLoop_Impl), SetShutdownFlagHook, NULL);
    OS_IdleLoop();

    CallCount = UT_GetStubCount(UT_KEY(OS_ApplicationShutdown_Impl));

    UtAssert_True(CallCount == 1, "OS_ApplicationShutdown_Impl() call count (%lu) == 1", (unsigned long)CallCount);

    OS_ApplicationShutdown(false);
}

void Test_OS_NotifyEvent(void)
{
    /*
     * Test cases for:
     * int32 OS_NotifyEvent(OS_Event_t event, osal_id_t object_id, void *data)
     * int32 OS_RegisterEventHandler(OS_EventHandler_t handler)
     */

    OS_SharedGlobalVars.EventHandler = NULL;

    /* With no hook function registered OS_NotifyEvent() should return success */
    OSAPI_TEST_FUNCTION_RC(OS_NotifyEvent(OS_EVENT_RESERVED, OS_OBJECT_ID_UNDEFINED, NULL), OS_SUCCESS);

    /* Registering a NULL hook function should fail */
    OSAPI_TEST_FUNCTION_RC(OS_RegisterEventHandler(NULL), OS_INVALID_POINTER);

    /* Now Register the locally-defined hook function */
    OSAPI_TEST_FUNCTION_RC(OS_RegisterEventHandler(TestEventHandlerHook), OS_SUCCESS);

    /* Now this should invoke the test hook */
    OSAPI_TEST_FUNCTION_RC(OS_NotifyEvent(OS_EVENT_RESERVED, OS_OBJECT_ID_UNDEFINED, NULL), OS_SUCCESS);
    UtAssert_STUB_COUNT(TestEventHandlerHook, 1);

    /* Should also return whatever the hook returned */
    UT_SetDefaultReturnValue(UT_KEY(TestEventHandlerHook), -12345);
    OSAPI_TEST_FUNCTION_RC(OS_NotifyEvent(OS_EVENT_RESERVED, OS_OBJECT_ID_UNDEFINED, NULL), -12345);
    UtAssert_STUB_COUNT(TestEventHandlerHook, 2);

    OS_SharedGlobalVars.EventHandler = NULL;
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

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_API_Init);
    ADD_TEST(OS_DeleteAllObjects);
    ADD_TEST(OS_CleanUpObject);
    ADD_TEST(OS_IdleLoopAndShutdown);
    ADD_TEST(OS_ApplicationExit);
    ADD_TEST(OS_NotifyEvent);
    ADD_TEST(OS_API_Teardown);
}
