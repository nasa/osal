/**
 * \file
 *
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-heap.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_HeapGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_HeapGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_heap_prop_t *heap_prop = UT_Hook_GetArgValueByName(Context, "heap_prop", OS_heap_prop_t *);
    int32           status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_HeapGetInfo), heap_prop, sizeof(*heap_prop)) < sizeof(*heap_prop))
    {
        /* Return some random data */
        heap_prop->free_bytes         = OSAL_SIZE_C(12345);
        heap_prop->free_blocks        = OSAL_BLOCKCOUNT_C(6789);
        heap_prop->largest_free_block = OSAL_SIZE_C(100);
    }
}
