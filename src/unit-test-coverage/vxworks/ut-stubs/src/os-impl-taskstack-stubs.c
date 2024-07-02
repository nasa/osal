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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-impl-taskstack header
 */

#include "os-impl-taskstack.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_VxWorks_TaskAPI_AcquireStackBlock(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_VxWorks_TaskAPI_StackBlockToAddr(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_VxWorks_TaskAPI_AcquireStackBlock()
 * ----------------------------------------------------
 */
void OS_VxWorks_TaskAPI_AcquireStackBlock(OS_impl_task_stack_mblock_t *bp, size_t block_size)
{
    UT_GenStub_SetupReturnBuffer(OS_VxWorks_TaskAPI_AcquireStackBlock, void *);

    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_AcquireStackBlock, OS_impl_task_stack_mblock_t *, bp);
    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_AcquireStackBlock, size_t, block_size);

    UT_GenStub_Execute(OS_VxWorks_TaskAPI_AcquireStackBlock, Basic,
                       UT_DefaultHandler_OS_VxWorks_TaskAPI_AcquireStackBlock);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_VxWorks_TaskAPI_ReleaseStackBlock()
 * ----------------------------------------------------
 */
void OS_VxWorks_TaskAPI_ReleaseStackBlock(OS_impl_task_stack_mblock_t *bp)
{
    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_ReleaseStackBlock, OS_impl_task_stack_mblock_t *, bp);

    UT_GenStub_Execute(OS_VxWorks_TaskAPI_ReleaseStackBlock, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_VxWorks_TaskAPI_StackBlockToAddr()
 * ----------------------------------------------------
 */
void OS_VxWorks_TaskAPI_StackBlockToAddr(OS_impl_task_stack_addr_t *addr, void *block_ptr, size_t block_size)
{
    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_StackBlockToAddr, OS_impl_task_stack_addr_t *, addr);
    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_StackBlockToAddr, void *, block_ptr);
    UT_GenStub_AddParam(OS_VxWorks_TaskAPI_StackBlockToAddr, size_t, block_size);

    UT_GenStub_Execute(OS_VxWorks_TaskAPI_StackBlockToAddr, Basic,
                       UT_DefaultHandler_OS_VxWorks_TaskAPI_StackBlockToAddr);
}
