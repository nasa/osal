/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-heap header
 */

#include "os-shared-heap.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_HeapGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_HeapGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_HeapGetInfo_Impl, OS_heap_prop_t *, heap_prop);

    UT_GenStub_Execute(OS_HeapGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_HeapGetInfo_Impl, int32);
}
