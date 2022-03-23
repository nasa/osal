/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_HEAP_H
#define OS_SHARED_HEAP_H

#include "osapi-heap.h"
#include "os-shared-globaldefs.h"

/****************************************************************************************
                    MEMORY HEAP API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * This may also not be implementable on some platforms
 */

/*----------------------------------------------------------------
   Function: OS_HeapGetInfo_Impl

    Purpose: Return current info on the heap

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop);

#endif /* OS_SHARED_HEAP_H */
