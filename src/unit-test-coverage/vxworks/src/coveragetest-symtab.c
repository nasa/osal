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
 * \file     coveragetest-symtab.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-symtab.h"
#include "os-shared-module.h"

#include "OCS_string.h"
#include "OCS_unistd.h"
#include "OCS_fcntl.h"
#include "OCS_symLib.h"

void Test_OS_GlobalSymbolLookup_Impl(void)
{
    /* Test Case For:
     * int32 OS_GlobalSymbolLookup_Impl( cpuaddr *SymbolAddress, const char *SymbolName )
     */
    cpuaddr SymAddr;

    OSAPI_TEST_FUNCTION_RC(OS_GlobalSymbolLookup_Impl(&SymAddr, "symname"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_GlobalSymbolLookup_Impl(NULL, NULL), OS_INVALID_POINTER);
    UT_SetDefaultReturnValue(UT_KEY(OCS_symFind), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_GlobalSymbolLookup_Impl(&SymAddr, "symname"), OS_ERROR);
}

void Test_OS_ModuleSymbolLookup_Impl(void)
{
    /* Test Case For:
     * int32 OS_ModuleSymbolLookup_Impl( uint32 local_id, cpuaddr *SymbolAddress, const char *SymbolName )
     */
    cpuaddr           SymAddr;
    OS_object_token_t token = UT_TOKEN_0;

    OSAPI_TEST_FUNCTION_RC(OS_ModuleSymbolLookup_Impl(&token, &SymAddr, "symname"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleSymbolLookup_Impl(&token, NULL, NULL), OS_INVALID_POINTER);
    UT_SetDefaultReturnValue(UT_KEY(OCS_symFind), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ModuleSymbolLookup_Impl(&token, &SymAddr, "symname"), OS_ERROR);
}

void Test_OS_SymTableIterator_Impl(void)
{
    /* Test Case For:
     * BOOL OS_SymTableIterator_Impl(char *name, SYM_VALUE val, SYM_TYPE type, int arg, SYM_GROUP group)
     */
    uint32 Data = 0;

    /* nominal case - nothing goes wrong */
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_CallIteratorFunc("ut", &Data, 100, 1000), true);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_GetIteratorStatus(), OS_SUCCESS);

    /* Check case where next entry will exceed size limit */
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_CallIteratorFunc("ut", &Data, 100, 101), false);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_GetIteratorStatus(), OS_ERR_OUTPUT_TOO_LARGE);

    /* Check case where entry has a name that is too long */
    UT_SetDefaultReturnValue(UT_KEY(OCS_memchr), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_CallIteratorFunc("ut", &Data, 100, 1000), false);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_GetIteratorStatus(), OS_ERR_NAME_TOO_LONG);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_memchr));

    /* Check case where writing to file fails */
    UT_SetDefaultReturnValue(UT_KEY(OCS_write), -1);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_CallIteratorFunc("ut", &Data, 100, 1000), false);
    OSAPI_TEST_FUNCTION_RC(UT_SymTabTest_GetIteratorStatus(), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_write));
}

static int32 UT_symEachHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    uint32 Data = 0;
    UT_SymTabTest_CallIteratorFunc("ut", &Data, 100, 1000);
    return StubRetcode;
}

void Test_OS_SymbolTableDump_Impl(void)
{
    /* Test Case For:
     * int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
     */

    /* With no action in symEach(), this will yield an empty file, which is an error */
    OSAPI_TEST_FUNCTION_RC(OS_SymbolTableDump_Impl("file", 10000), OS_ERROR);

    /* Check failure in open() */
    UT_SetDefaultReturnValue(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_SymbolTableDump_Impl("file", 10000), OS_ERROR);
    UT_ClearDefaultReturnValue(UT_KEY(OCS_open));

    /* Set up a hook function for symEach() to provide at least one entry */
    UT_SetHookFunction(UT_KEY(OCS_symEach), UT_symEachHook, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_SymbolTableDump_Impl("file", 10000), OS_SUCCESS);
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
    ADD_TEST(OS_SymTableIterator_Impl);
    ADD_TEST(OS_GlobalSymbolLookup_Impl);
    ADD_TEST(OS_ModuleSymbolLookup_Impl);
    ADD_TEST(OS_SymbolTableDump_Impl);
}
