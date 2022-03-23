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
