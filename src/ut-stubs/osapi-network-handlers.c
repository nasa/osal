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

#include "osapi-network.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_NetworkGetHostName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_NetworkGetHostName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    char * host_name = UT_Hook_GetArgValueByName(Context, "host_name", char *);
    size_t name_len  = UT_Hook_GetArgValueByName(Context, "name_len", size_t);
    int32  status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && name_len > 0 &&
        UT_Stub_CopyToLocal(UT_KEY(OS_NetworkGetHostName), host_name, name_len) == 0)
    {
        strncpy(host_name, "ut", name_len - 1);
        host_name[name_len - 1] = 0;
    }
}
