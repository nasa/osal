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
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-taskstack.h"

#include "os-shared-task.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"
#include "osapi-bsp.h"

#include <errnoLib.h>
#include <taskLib.h>
#include <memPartLib.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/*
 * macros for stack size manipulation.
 * These are normally provided by vxWorks.h if relevant for the platform.
 * If they are not defined, use a reasonable default/substitute.
 */
#if defined(_STACK_ALIGN_SIZE)
#define VX_IMPL_STACK_ALIGN_SIZE _STACK_ALIGN_SIZE
#else
#define VX_IMPL_STACK_ALIGN_SIZE ((size_t)16)
#endif

#if defined(STACK_ROUND_DOWN)
#define VX_IMPL_STACK_ROUND_DOWN(x) STACK_ROUND_DOWN(x)
#else
#define VX_IMPL_STACK_ROUND_DOWN(x) ((x) & ~(VX_IMPL_STACK_ALIGN_SIZE - 1))
#endif

#if defined(STACK_ROUND_UP)
#define VX_IMPL_STACK_ROUND_UP(x) STACK_ROUND_UP(x)
#else
#define VX_IMPL_STACK_ROUND_UP(x) (((x) + (VX_IMPL_STACK_ALIGN_SIZE - 1)) & ~(VX_IMPL_STACK_ALIGN_SIZE - 1))
#endif

/* Use the VxWorks system partition by default */
#if !defined(VX_IMPL_STACK_MEMPARTID)
extern PART_ID memSysPartId;
#define VX_IMPL_STACK_MEMPARTID memSysPartId
#endif

/* Define a reasonable minimum stack size for sanity check */
#if !defined(VX_IMPL_STACK_MIN)
#define VX_IMPL_STACK_MIN (VX_IMPL_STACK_ALIGN_SIZE * 16)
#endif

/*
 * internal helper function to release a stack block
 *
 * This wrapper permits stack blocks to be obtained from something
 * other than the system heap.  This function is invoked by the Task
 * subsystem when it is finished using a block.
 */
void OS_VxWorks_TaskAPI_ReleaseStackBlock(OS_impl_task_stack_mblock_t *bp)
{
    if (bp->block_ptr != NULL)
    {
        memPartFree(VX_IMPL_STACK_MEMPARTID, bp->block_ptr);
    }

    bp->block_size = 0;
    bp->block_ptr  = NULL;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 * internal helper function to acquire a stack block
 *
 *-----------------------------------------------------------------*/
void OS_VxWorks_TaskAPI_AcquireStackBlock(OS_impl_task_stack_mblock_t *bp, size_t block_size)
{
    size_t actualsz;
    void * mempool_block;

    /*
     * NOTE: The stack-rounding/alignment macros are normally supplied from
     * vxWorks.h on relevant platforms.  If not provided then it is
     * assumed that no specific alignment is needed on this platform.
     */

    /* add a little extra in case the base address needs alignment too.
     * this helps ensure that the final aligned stack is not less
     * than what was originally requested (but might be a bit more)  */
    actualsz = block_size + VX_IMPL_STACK_ALIGN_SIZE;
    actualsz = VX_IMPL_STACK_ROUND_UP(actualsz);

    /* this call should always return an aligned block */
    mempool_block = memPartAlignedAlloc(VX_IMPL_STACK_MEMPARTID, /* memory partition to allocate from */
                                        actualsz,                /* number of bytes to allocate */
                                        VX_IMPL_STACK_ALIGN_SIZE /* boundary to align to */
    );

    bp->block_ptr = mempool_block;

    if (mempool_block == NULL)
    {
        bp->block_size = 0;
    }
    else
    {
        bp->block_size = actualsz;
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 * internal helper function to acquire a stack block
 *
 *-----------------------------------------------------------------*/
void OS_VxWorks_TaskAPI_StackBlockToAddr(OS_impl_task_stack_addr_t *addr, void *block_ptr, size_t block_size)
{
    cpuaddr start_addr;
    cpuaddr end_addr;
    size_t  usable_size;

    /* got a block, now adjust for cpu-specific requirements */

    /* convert to integer memory address */
    start_addr = (cpuaddr)block_ptr;
    end_addr   = start_addr + block_size - 1;

    start_addr += (VX_IMPL_STACK_ALIGN_SIZE - 1);
    start_addr &= ~(VX_IMPL_STACK_ALIGN_SIZE - 1);
    end_addr &= ~(VX_IMPL_STACK_ALIGN_SIZE - 1);

    if (end_addr <= start_addr)
    {
        /* This should not happen */
        usable_size = 0;
    }
    else
    {
        usable_size = end_addr - start_addr;
    }

    /*
     * export the address to the caller.  This is the value that
     * should be used for the actual VxWorks task stack pointer
     *
     * VxWorks uses the address directly as the CPU stack pointer register,
     * this needs to compensate for downward-growing stacks.
     *
     * On most CPUs the stack grows downward, so assume that to be
     * the case in the event that _STACK_DIR is not defined/known
     */
#if !defined(_STACK_DIR) || (_STACK_DIR != _STACK_GROWS_UP)
    addr->stackaddr = end_addr; /* stack grows down: SP set to upper address bound */
#else
    addr->stackaddr = start_addr; /* stack grows up: SP set to lower address bound */
#endif

    addr->usable_size = usable_size;
}