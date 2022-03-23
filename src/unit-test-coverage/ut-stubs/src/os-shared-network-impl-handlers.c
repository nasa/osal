/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-network.h" /* OSAL public API for this subsystem */
#include "os-shared-network.h"
#include "utstubs.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_NetworkGetID_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_NetworkGetID_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    int32 *IdBuf = UT_Hook_GetArgValueByName(Context, "IdBuf", int32 *);
    int32  status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == 0)
    {
        *IdBuf = 42;
    }
}
