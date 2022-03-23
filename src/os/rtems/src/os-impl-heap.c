/**
 * \file
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-heap.h"

/****************************************************************************************
                                     HEAP API
 ***************************************************************************************/

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
    OSAL_HEAP_INFO_BLOCK info;
    int                  status;

    status = malloc_info(&info);

    if (status != 0)
    {
        return (OS_ERROR);
    }

    heap_prop->free_bytes         = (uint32)info.Free.total;
    heap_prop->free_blocks        = (uint32)info.Free.number;
    heap_prop->largest_free_block = (uint32)info.Free.largest;

    return (OS_SUCCESS);
} /* end OS_HeapGetInfo_Impl */
