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

#include "osapi-filesys.h" /* OSAL public API for this subsystem */
#include "os-shared-filesys.h"
#include "utstubs.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_FileSysStatVolume_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_FileSysStatVolume_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_statvfs_t *result = UT_Hook_GetArgValueByName(Context, "result", OS_statvfs_t *);
    int32         status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_FileSysStatVolume_Impl), result, sizeof(*result)) < sizeof(*result))
    {
        memset(result, 0, sizeof(*result));
    }
}
