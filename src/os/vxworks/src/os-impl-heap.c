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
#include "os-shared-heap.h"

#include <memPartLib.h>

/****************************************************************************************
                                     HEAP API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_HeapGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
{
    MEM_PART_STATS stats;
    STATUS         status;

    status = memPartInfoGet(memSysPartId, &stats);

    if (status != OK)
    {
        return OS_ERROR;
    }

    heap_prop->free_bytes         = OSAL_SIZE_C(stats.numBytesFree);
    heap_prop->free_blocks        = OSAL_BLOCKCOUNT_C(stats.numBlocksFree);
    heap_prop->largest_free_block = OSAL_SIZE_C(stats.maxBlockSizeFree);

    return (OS_SUCCESS);
} /* end OS_HeapGetInfo_Impl */
