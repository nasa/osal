/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to heap.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * User defined include files
 */
#include "os-shared-heap.h"

/*----------------------------------------------------------------
 *
 * Function: OS_HeapGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
{
    /* Check parameters */
    OS_CHECK_POINTER(heap_prop);

    return OS_HeapGetInfo_Impl(heap_prop);
} /* end OS_HeapGetInfo */
