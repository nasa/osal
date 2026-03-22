/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * \ingroup  portable
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"
#include "os-shared-idmap.h"
#include "os-shared-file.h"

#include "OCS_fcntl.h"
#include "OCS_errno.h"

void Test_OS_FileAllocate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileAllocate_Impl(const OS_object_token_t *token, osal_offset_t offset, osal_offset_t len);
     */
    OS_object_token_t token;

    memset(&token, 0, sizeof(token));

    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_SUCCESS);

    UT_SetDefaultReturnValue(UT_KEY(OCS_posix_fallocate), -1);

    OCS_errno = OCS_EOPNOTSUPP;
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERR_OPERATION_NOT_SUPPORTED);
    OCS_errno = OCS_EFBIG;
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERR_OUTPUT_TOO_LARGE);
    OCS_errno = OCS_ENOSPC;
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERR_OUTPUT_TOO_LARGE);
    OCS_errno = OCS_ESPIPE;
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERR_INCORRECT_OBJ_TYPE);
    OCS_errno = OCS_ENODEV;
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERR_INCORRECT_OBJ_TYPE);
    OCS_errno = OCS_EINVAL; /* something for the catch all */
    OSAPI_TEST_FUNCTION_RC(OS_FileAllocate_Impl, (&token, 0, 0), OS_ERROR);
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
    ADD_TEST(OS_FileAllocate_Impl);
}
