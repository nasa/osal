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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-common.h"

#include "os-shared-common.h"

#include "OCS_errno.h"
#include "OCS_objLib.h"

OCS_SEM TestGlobalSem;

void Test_OS_API_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_API_Impl_Init(uint32 idtype)
     */
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(0), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_StubKey_OS_VxWorks_TableMutex_Init, OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_StubKey_OS_VxWorks_TableMutex_Init);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_QUEUE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_BINSEM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_COUNTSEM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_MUTEX), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_MODULE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TIMEBASE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_STREAM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_DIR), OS_SUCCESS);
}

void Test_OS_IdleLoop_Impl(void)
{
    /*
     * Test Case For:
     * void OS_IdleLoop_Impl()
     */
    /* just call the function for coverage */
    OS_IdleLoop_Impl();
}

void Test_OS_ApplicationShutdown_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ApplicationShutdown_Impl()
     */
    /* just call the function for coverage */
    OS_ApplicationShutdown_Impl();
}

void Test_OS_VxWorks_GenericSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_GenericSemGive(SEM_ID vxid)
     */
    OCS_SEM_ID SemID = NULL;

    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemGive(SemID), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OCS_semGive), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemGive(SemID), OS_SEM_FAILURE);
} /* end OS_VxWorks_GenericSemGive */

void Test_OS_VxWorks_GenericSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_GenericSemTake(SEM_ID vxid, int sys_ticks)
     */
    OCS_SEM_ID SemID = NULL;

    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemTake(SemID, 10), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OCS_semTake), OCS_ERROR);
    OCS_errno = OCS_S_objLib_OBJ_TIMEOUT;
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemTake(SemID, 0), OS_SEM_TIMEOUT);
    OCS_errno = OCS_S_objLib_OBJ_ID_ERROR;
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemTake(SemID, 0), OS_SEM_FAILURE);
    OCS_errno = OCS_S_objLib_OBJ_UNAVAILABLE;
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemTake(SemID, 0), OS_SEM_TIMEOUT);
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_GenericSemTake(SemID, 1), OS_SEM_FAILURE);

} /* end OS_VxWorks_GenericSemTake */

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(&OS_SharedGlobalVars, 0, sizeof(OS_SharedGlobalVars));
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
    ADD_TEST(OS_API_Impl_Init);
    ADD_TEST(OS_IdleLoop_Impl);
    ADD_TEST(OS_ApplicationShutdown_Impl);
    ADD_TEST(OS_VxWorks_GenericSemGive);
    ADD_TEST(OS_VxWorks_GenericSemTake);
}
