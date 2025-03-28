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
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_TASKSTACK_H
#define OS_IMPL_TASKSTACK_H

#include "osconfig.h"
#include "common_types.h"

typedef struct OS_impl_task_stack_mblock
{
    void * block_ptr; /* set non-null if the stack was obtained from a memory pool */
    size_t block_size;
} OS_impl_task_stack_mblock_t;

typedef struct OS_impl_task_stack_addr
{
    cpuaddr stackaddr;
    size_t  usable_size;
} OS_impl_task_stack_addr_t;

/*
 * internal helper function to release a stack block
 *
 * This wrapper permits stack blocks to be obtained from something
 * other than the system heap.  This function is invoked by the Task
 * subsystem when it is finished using a block.
 */
void OS_VxWorks_TaskAPI_ReleaseStackBlock(OS_impl_task_stack_mblock_t *bp);

/*
 * internal helper function to acquire a stack block
 *
 * This wrapper permits stack blocks to be obtained from something
 * other than the system heap.  This function is invoked by the Task
 * subsystem when needs to obtain a new block for use as a task stack.
 */
void OS_VxWorks_TaskAPI_AcquireStackBlock(OS_impl_task_stack_mblock_t *bp, size_t block_size);

/*
 * internal helper function to convert a stack block to stack address
 *
 * VxWorks requires that the application pass in the actual CPU stack pointer (SP)
 * value to the task create call.  This function performs the necessary conversion
 * to accomodate for this.  The usable stack size may be less than the block size
 * due to alignment.
 */
void OS_VxWorks_TaskAPI_StackBlockToAddr(OS_impl_task_stack_addr_t *addr, void *block_ptr, size_t block_size);

#endif /* OS_IMPL_TASKSTACK_H */
