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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"

#include "os-impl-taskstack.h"
#include "OCS_memPartLib.h"

void Test_OS_VxWorks_TaskAPI_ReleaseStackBlock(void)
{
    /*
     * Test function for:
     * void OS_VxWorks_TaskAPI_ReleaseStackBlock(OS_impl_task_stack_mblock_t *bp);
     */

    OS_impl_task_stack_mblock_t blk;
    char                        my_block[100];

    memset(&blk, 0, sizeof(blk));

    /* nominal, no-op when blk.block_ptr is NULL */
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_ReleaseStackBlock(&blk));
    UtAssert_NULL(blk.block_ptr);
    UtAssert_STUB_COUNT(OCS_memPartFree, 0);

    /* nominal, should release when blk.block_ptr is non-NULL */
    blk.block_ptr  = my_block;
    blk.block_size = sizeof(my_block);
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_ReleaseStackBlock(&blk));
    UtAssert_NULL(blk.block_ptr);
    UtAssert_ZERO(blk.block_size);
    UtAssert_STUB_COUNT(OCS_memPartFree, 1);
}

void Test_OS_VxWorks_TaskAPI_AcquireStackBlock(void)
{
    /*
     * Test function for:
     * void OS_VxWorks_TaskAPI_AcquireStackBlock(OS_impl_task_stack_mblock_t *bp, size_t block_size);
     */
    OS_impl_task_stack_mblock_t blk;
    char                        my_block[100];

    memset(&blk, 0, sizeof(blk));

    /* memPartAlignedAlloc() returns NULL by default, so this is an error case */
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_AcquireStackBlock(&blk, sizeof(my_block)));
    UtAssert_STUB_COUNT(OCS_memPartAlignedAlloc, 1);
    UtAssert_NULL(blk.block_ptr);
    UtAssert_ZERO(blk.block_size);

    /* nominal, memPartAlignedAlloc() returns pointer to block */
    UT_SetDefaultReturnValue(UT_KEY(OCS_memPartAlignedAlloc), (UT_IntReturn_t)my_block);
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_AcquireStackBlock(&blk, sizeof(my_block)));
    UtAssert_STUB_COUNT(OCS_memPartAlignedAlloc, 2);
    UtAssert_ADDRESS_EQ(blk.block_ptr, my_block);
    UtAssert_GTEQ(size_t, blk.block_size, sizeof(my_block)); /* could be larger due to rounding */
}

void Test_OS_VxWorks_TaskAPI_StackBlockToAddr(void)
{
    /*
     * Test function for:
     * void OS_VxWorks_TaskAPI_StackBlockToAddr(OS_impl_task_stack_addr_t *addr, void *block_ptr, size_t block_size);
     */

    OS_impl_task_stack_addr_t addr;
    char                      my_block[100];

    memset(&addr, 0, sizeof(addr));

    /* nominal */
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_StackBlockToAddr(&addr, my_block, sizeof(my_block)));
    UtAssert_GTEQ(cpuaddr, addr.stackaddr, (cpuaddr)my_block); /* this could be the start or end of block */
    UtAssert_LTEQ(size_t, addr.usable_size, sizeof(my_block));
    UtAssert_GT(size_t, addr.usable_size, 0);

    /* block too small */
    UtAssert_VOIDCALL(OS_VxWorks_TaskAPI_StackBlockToAddr(&addr, my_block, 0));
    UtAssert_ZERO(addr.usable_size);
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
    ADD_TEST(OS_VxWorks_TaskAPI_ReleaseStackBlock);
    ADD_TEST(OS_VxWorks_TaskAPI_AcquireStackBlock);
    ADD_TEST(OS_VxWorks_TaskAPI_StackBlockToAddr);
}
