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

#ifndef OS_IMPL_TASKS_H
#define OS_IMPL_TASKS_H

#include "osconfig.h"
#include <taskLib.h>

#if defined(VX_WIND_TCB_SIZE)
/**
 * vxworks >= 7.0 should provide this symbol via taskLib.h. WIND_TCB is an opaque type
 * NOTE: uint64 used to ensure sufficient alignment, and + 1 means it's at least big enough
 *       but might be slightly bigger than necessary if VX_WIND_TCB_SIZE mod 8 is zero
 */
typedef uint64 OS_VxWorks_TCB_t[(VX_WIND_TCB_SIZE / 8) + 1];
#else
/* older vxworks expose the definition of VX_WIND_TCB_SIZE */
typedef WIND_TCB OS_VxWorks_TCB_t;
#endif /* !defined(VX_WIND_TCB_SIZE) */

/*tasks */
typedef struct
{
    OS_VxWorks_TCB_t tcb; /* Must be first */
    TASK_ID          vxid;
    void *           heap_block; /* set non-null if the stack was obtained with malloc() */
    size_t           heap_block_size;
} OS_impl_task_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

#endif /* OS_IMPL_TASKS_H */
