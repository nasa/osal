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
 * \file     coveragetest-bsd-select.c
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-portable-coveragetest.h"
#include "ut-adaptor-portable-posix-io.h"
#include "os-shared-select.h"
#include "os-shared-idmap.h"

#include "OCS_sys_select.h"
#include "OCS_errno.h"

void Test_OS_SelectSingle_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectSingle_Impl(uint32 stream_id, uint32 *SelectFlags, int32 msecs)
     */
    uint32              SelectFlags;
    OS_object_token_t   token;
    struct OCS_timespec nowtime;
    struct OCS_timespec latertime;
    struct OCS_timespec latertime2;

    memset(&token, 0, sizeof(token));

    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, false);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 0), OS_ERR_OPERATION_NOT_SUPPORTED);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 0), OS_SUCCESS);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, -1), OS_SUCCESS);
    SelectFlags = 0;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 0), OS_SUCCESS);

    /* try a case where select() needs to be repeated to achieve the desired wait time */
    UT_SetDefaultReturnValue(UT_KEY(OCS_select), -1);
    OCS_errno = OCS_EINTR;
    UT_SetDeferredRetcode(UT_KEY(OCS_select), 2, 0);
    SelectFlags        = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime.tv_sec     = 1;
    nowtime.tv_nsec    = 0;
    latertime.tv_sec   = 1;
    latertime.tv_nsec  = 800000000;
    latertime2.tv_sec  = 2;
    latertime2.tv_nsec = 200000000;
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &latertime, sizeof(latertime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &latertime2, sizeof(latertime2), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 1200), OS_ERROR_TIMEOUT);

    UT_SetDefaultReturnValue(UT_KEY(OCS_select), 0);
    SelectFlags       = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime.tv_sec    = 1;
    nowtime.tv_nsec   = 500000000;
    latertime.tv_sec  = 10;
    latertime.tv_nsec = 0;
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 999), OS_ERROR_TIMEOUT);

    UT_SetDefaultReturnValue(UT_KEY(OCS_select), -1);
    OCS_errno         = OCS_ETIMEDOUT;
    SelectFlags       = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime.tv_sec    = 1;
    nowtime.tv_nsec   = 0;
    latertime.tv_sec  = 2;
    latertime.tv_nsec = 600000000;
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 2100), OS_ERROR);

    /* Test cases where the FD exceeds FD_SETSIZE */
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, OCS_FD_SETSIZE);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (&token, &SelectFlags, 0), OS_ERR_OPERATION_NOT_SUPPORTED);

} /* end OS_SelectSingle_Impl */

void Test_OS_SelectMultiple_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;

    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, 0);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);

    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    WriteSet.object_ids[0] = 1;
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 0), OS_SUCCESS);

    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    ReadSet.object_ids[0] = 1;
    UT_SetDefaultReturnValue(UT_KEY(OCS_select), 0);
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 1), OS_ERROR_TIMEOUT);

    /* Test where the FD set is empty */
    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (NULL, NULL, 0), OS_ERR_INVALID_ID);

    /* Test cases where the FD exceeds FD_SETSIZE in the read set */
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, OCS_FD_SETSIZE);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    memset(&ReadSet, 0xff, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 0), OS_ERR_OPERATION_NOT_SUPPORTED);

    /* Test cases where the FD exceeds FD_SETSIZE in the write set */
    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0xff, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 0), OS_ERR_OPERATION_NOT_SUPPORTED);

    /* Test cases where additional bits are set in the OS_FdSet */
    UT_PortablePosixIOTest_Set_FD(UT_INDEX_0, 0);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    memset(&ReadSet, 0xff, sizeof(ReadSet));
    memset(&WriteSet, 0xff, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 0), OS_ERR_OPERATION_NOT_SUPPORTED);

} /* end OS_SelectMultiple_Impl */

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
