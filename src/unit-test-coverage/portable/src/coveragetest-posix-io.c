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
 * \file     coveragetest-posix-io.c
 * \ingroup  portable
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"
#include "ut-adaptor-portable-posix-io.h"

#include "os-shared-file.h"
#include "os-shared-idmap.h"
#include "os-shared-select.h"

#include "OCS_stdlib.h"
#include "OCS_unistd.h"
#include "OCS_fcntl.h"
#include "OCS_errno.h"

/* OS_SelectSingle_Impl hook to clear SelectFlags */
static int32 UT_Hook_OS_SelectSingle_Impl(void *UserObj, int32 StubRetcode, uint32 CallCount,
                                          const UT_StubContext_t *Context)
{
    uint32 *SelectFlags;

    SelectFlags = UT_Hook_GetArgValueByName(Context, "SelectFlags", uint32 *);

    if (SelectFlags != NULL)
    {
        *SelectFlags = 0;
    }

    return 0;
}

void Test_OS_GenericClose_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericClose_Impl(uint32 local_id)
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_GenericClose_Impl, (&token), OS_SUCCESS);

    /*
     * Test path where underlying close() fails.
     * Should still return success.
     */
    UT_SetDefaultReturnValue(UT_KEY(OCS_close), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericClose_Impl, (&token), OS_SUCCESS);
}

void Test_OS_GenericSeek_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericSeek_Impl (uint32 local_id, int32 offset, uint32 whence)
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    /* note on success this wrapper returns the result of lseek(), not OS_SUCCESS */
    UT_SetDefaultReturnValue(UT_KEY(OCS_lseek), 111);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, OS_SEEK_CUR), 111);
    UT_SetDefaultReturnValue(UT_KEY(OCS_lseek), 222);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, OS_SEEK_SET), 222);
    UT_SetDefaultReturnValue(UT_KEY(OCS_lseek), 333);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, OS_SEEK_END), 333);

    /* bad whence */
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, 1234), OS_ERROR);

    /* generic failure of lseek() */
    UT_SetDefaultReturnValue(UT_KEY(OCS_lseek), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, OS_SEEK_END), OS_ERROR);

    /* The seek implementation also checks for this specific pipe errno */
    OCS_errno = OCS_ESPIPE;
    OSAPI_TEST_FUNCTION_RC(OS_GenericSeek_Impl, (&token, 0, OS_SEEK_END), OS_ERR_OPERATION_NOT_SUPPORTED);
}

void Test_OS_GenericRead_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericRead_Impl (uint32 local_id, void *buffer, uint32 nbytes, int32 timeout)
     */
    char              SrcData[]                 = "ABCDEFGHIJK";
    char              DestData[sizeof(SrcData)] = {0};
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    UT_SetDataBuffer(UT_KEY(OCS_read), SrcData, sizeof(SrcData), false);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, false);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, sizeof(DestData), 0), sizeof(DestData));
    UtAssert_MemCmp(SrcData, DestData, sizeof(SrcData), "read() data Valid");

    /* test invocation of select() in nonblocking mode */
    UT_ResetState(UT_KEY(OCS_read));
    UT_SetDataBuffer(UT_KEY(OCS_read), SrcData, sizeof(SrcData), false);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, sizeof(DestData), 0), sizeof(DestData));
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_SelectSingle_Impl)) == 1, "OS_SelectSingle() called");

    /* Read 0 bytes */
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, 0, 0), OS_SUCCESS);

    /* read() failure */
    UT_SetDefaultReturnValue(UT_KEY(OCS_read), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, sizeof(DestData), 0), OS_ERROR);

    /* Fail select */
    UT_SetDeferredRetcode(UT_KEY(OS_SelectSingle_Impl), 1, OS_ERROR_TIMEOUT);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, sizeof(DestData), 0), OS_ERROR_TIMEOUT);

    /* Not readable */
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), UT_Hook_OS_SelectSingle_Impl, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_GenericRead_Impl, (&token, DestData, sizeof(DestData), 0), OS_SUCCESS);
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), NULL, NULL);
}

void Test_OS_GenericWrite_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GenericWrite_Impl(uint32 local_id, const void *buffer, uint32 nbytes, int32 timeout)
     */
    char              SrcData[]                 = "ABCDEFGHIJKL";
    char              DestData[sizeof(SrcData)] = {0};
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    UT_SetDataBuffer(UT_KEY(OCS_write), DestData, sizeof(DestData), false);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, false);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, SrcData, sizeof(SrcData), 0), sizeof(SrcData));
    UtAssert_MemCmp(SrcData, DestData, sizeof(SrcData), "write() data valid");

    /* test invocation of select() in nonblocking mode */
    UT_ResetState(UT_KEY(OCS_write));
    UT_SetDataBuffer(UT_KEY(OCS_write), DestData, sizeof(DestData), false);
    UT_PortablePosixIOTest_Set_Selectable(UT_INDEX_0, true);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, SrcData, sizeof(SrcData), 0), sizeof(SrcData));
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_SelectSingle_Impl)) == 1, "OS_SelectSingle() called");

    /* Fail select */
    UT_SetDeferredRetcode(UT_KEY(OS_SelectSingle_Impl), 1, OS_ERROR_TIMEOUT);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, SrcData, sizeof(SrcData), 0), OS_ERROR_TIMEOUT);

    /* Write 0 bytes */
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, SrcData, 0, 0), OS_SUCCESS);

    /* write() failure */
    UT_SetDefaultReturnValue(UT_KEY(OCS_write), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, DestData, sizeof(DestData), 0), OS_ERROR);

    /* Not writeable */
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), UT_Hook_OS_SelectSingle_Impl, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_GenericWrite_Impl, (&token, SrcData, sizeof(SrcData), 0), OS_SUCCESS);
    UT_SetHookFunction(UT_KEY(OS_SelectSingle_Impl), NULL, NULL);
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
    memset(OS_stream_table, 0, sizeof(OS_stream_table));
    memset(OS_global_stream_table, 0, sizeof(OS_common_record_t) * OS_MAX_NUM_OPEN_FILES);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void) {}

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GenericClose_Impl);
    ADD_TEST(OS_GenericSeek_Impl);
    ADD_TEST(OS_GenericRead_Impl);
    ADD_TEST(OS_GenericWrite_Impl);
}
