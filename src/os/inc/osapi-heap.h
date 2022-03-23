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
 * Declarations and prototypes for heap functions
 */

#ifndef OSAPI_HEAP_H
#define OSAPI_HEAP_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @brief OSAL heap properties
 *
 * @sa OS_HeapGetInfo()
 */
typedef struct
{
    size_t            free_bytes;
    osal_blockcount_t free_blocks;
    size_t            largest_free_block;
} OS_heap_prop_t;

/** @defgroup OSAPIHeap OSAL Heap APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Return current info on the heap
 *
 * @param[out] heap_prop Storage buffer for heap info
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if the heap_prop argument is NULL
 */
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop);
/**@}*/

#endif /* OSAPI_HEAP_H */
