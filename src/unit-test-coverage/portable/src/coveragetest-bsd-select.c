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
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-portable-coveragetest.h"
#include "ut-adaptor-portable-posix-io.h"
#include "os-shared-clock.h"
#include "os-shared-select.h"
#include "os-shared-idmap.h"

#include "OCS_sys_select.h"
#include "OCS_errno.h"

void Test_OS_SelectSingle_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectSingle_Impl(uint32 stream_id, uint32 *SelectFlags, int32 msecs)
     */
    uint32            SelectFlags;
    OS_object_token_t token;
    OS_time_t         nowtime;
    OS_time_t         latertime;
    OS_time_t         timeout;

    memset(&token, 0, sizeof(token));

    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, false);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MIN), OS_ERR_OPERATION_NOT_SUPPORTED);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MIN), OS_SUCCESS);

    /* Cover FD_ISSET true branches and pend */
    UT_SetDeferredRetcode(UT_KEY(OCS_FD_ISSET), 1, true);
    UT_SetDeferredRetcode(UT_KEY(OCS_FD_ISSET), 1, true);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MAX), OS_SUCCESS);

    /* No flags and non-read/write flag branches */
    SelectFlags = 0;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MIN), OS_SUCCESS);
    SelectFlags = OS_STREAM_STATE_BOUND;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MIN), OS_SUCCESS);

    /* try a case where select() needs to be repeated to achieve the desired wait time */
    UT_ResetState(UT_KEY(OCS_select));
    UT_SetDefaultReturnValue(UT_KEY(OCS_select), -1);
    OCS_errno = OCS_EINTR;
    UT_SetDeferredRetcode(UT_KEY(OCS_select), 2, 0);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime     = OS_TimeFromTotalMilliseconds(1000);
    latertime   = OS_TimeFromTotalMilliseconds(1800);
    timeout     = OS_TimeFromTotalMilliseconds(1200);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, timeout), OS_ERROR_TIMEOUT);
    UtAssert_STUB_COUNT(OS_GetLocalTime_Impl, 2);
    UtAssert_STUB_COUNT(OCS_select, 2);

    /* Repeated select with alternate branches */
    OCS_errno   = OCS_EAGAIN;
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    UT_SetDeferredRetcode(UT_KEY(OCS_select), 2, 0);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, timeout), OS_ERROR_TIMEOUT);
    UtAssert_STUB_COUNT(OS_GetLocalTime_Impl, 4);
    UtAssert_STUB_COUNT(OCS_select, 4);

    UT_SetDefaultReturnValue(UT_KEY(OCS_select), 0);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime     = OS_TimeFromTotalMilliseconds(1500);
    latertime   = OS_TimeFromTotalMilliseconds(10000);
    timeout     = OS_TimeFromTotalMilliseconds(999);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, timeout), OS_ERROR_TIMEOUT);

    UT_SetDefaultReturnValue(UT_KEY(OCS_select), -1);
    OCS_errno   = OCS_ETIMEDOUT;
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime     = OS_TimeFromTotalMilliseconds(1000);
    latertime   = OS_TimeFromTotalMilliseconds(2600);
    timeout     = OS_TimeFromTotalMilliseconds(2100);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OS_GetLocalTime_Impl), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, timeout), OS_ERROR);

    /* Test cases where the FD exceeds FD_SETSIZE */
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, OCS_FD_SETSIZE);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, OS_TIME_MIN), OS_ERR_OPERATION_NOT_SUPPORTED);
}

void Test_OS_SelectMultiple_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, OS_time_t abs_timeout)
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;
    int      i;

    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, 0);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);

    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    WriteSet.object_ids[0] = 1;
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (NULL, &WriteSet, OS_TIME_MIN), OS_SUCCESS);
    ReadSet.object_ids[0] = 1;
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, NULL, OS_TIME_MIN), OS_SUCCESS);

    /* Branches for processing the set */
    UT_SetDeferredRetcode(UT_KEY(OCS_FD_ISSET), 1, true);
    WriteSet.object_ids[0] = 0x0D;
    UT_PortablePosixIOTest_Set_FD(OSAL_INDEX_C(2), -1);
    UT_PortablePosixIOTest_Set_FD(OSAL_INDEX_C(3), 0);
    UT_PortablePosixIOTest_Set_Selectable(OSAL_INDEX_C(3), true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MIN), OS_SUCCESS);

    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    ReadSet.object_ids[0] = 1;
    UT_SetDeferredRetcode(UT_KEY(OCS_select), 1, 0);
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MAX), OS_ERROR_TIMEOUT);

    /* Test where the FD set is empty */
    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (NULL, NULL, OS_TIME_MIN), OS_ERR_INVALID_ID);

    /* Test cases where the FD exceeds FD_SETSIZE in the read set */
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, OCS_FD_SETSIZE);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    memset(&ReadSet, 0xff, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MIN), OS_ERR_OPERATION_NOT_SUPPORTED);

    /* Test cases where the FD exceeds FD_SETSIZE in the write set */
    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0xff, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MIN), OS_ERR_OPERATION_NOT_SUPPORTED);

    /* Test cases where additional bits are set in the OS_FdSet */
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, 0);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    memset(&ReadSet, 0xff, sizeof(ReadSet));
    memset(&WriteSet, 0xff, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MIN), OS_ERR_OPERATION_NOT_SUPPORTED);

    /*
     * Cover OS_FdSet_ConvertOut_Impl for id < OS_MAX_NUM_OPEN_FILES, requires no errors from in conversion
     * NOTE - coverage only possible if OS_MAX_NUM_OPEN_FILES is not a multiple of 8 (exact fit)
     */
    for (i = 1; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        UT_PortablePosixIOTest_Set_FD(OSAL_INDEX_C(i), -1);
    }
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, OS_TIME_MIN), OS_SUCCESS);
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

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_SelectSingle_Impl);
    ADD_TEST(OS_SelectMultiple_Impl);
}
