/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-heap header
 */

#include "osapi-heap.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_HeapGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_HeapGetInfo()
 * ----------------------------------------------------
 */
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_HeapGetInfo, int32);

    UT_GenStub_AddParam(OS_HeapGetInfo, OS_heap_prop_t *, heap_prop);

    UT_GenStub_Execute(OS_HeapGetInfo, Basic, UT_DefaultHandler_OS_HeapGetInfo);

    return UT_GenStub_GetReturnValue(OS_HeapGetInfo, int32);
}
