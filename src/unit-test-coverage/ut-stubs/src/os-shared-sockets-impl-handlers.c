/**
 * \file
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-sockets.h" /* OSAL public API for this subsystem */
#include "os-shared-sockets.h"
#include "utstubs.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SocketAddrGetPort_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint16 *PortNum = UT_Hook_GetArgValueByName(Context, "PortNum", uint16 *);
    int32   status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_SocketAddrGetPort_Impl), PortNum, sizeof(*PortNum)) < sizeof(*PortNum))
    {
        *PortNum = 0;
    }
}
