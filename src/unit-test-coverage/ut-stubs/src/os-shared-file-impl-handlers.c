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

#include "osapi-file.h" /* OSAL public API for this subsystem */
#include "os-shared-file.h"
#include "utstubs.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GenericRead_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GenericRead_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    void * buffer = UT_Hook_GetArgValueByName(Context, "buffer", void *);
    size_t nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    int32  status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        status = UT_Stub_CopyToLocal(UT_KEY(OS_GenericRead_Impl), buffer, nbytes);
        UT_Stub_SetReturnValue(FuncKey, status);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GenericWrite_Impl' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GenericWrite_Impl(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const void *buffer = UT_Hook_GetArgValueByName(Context, "buffer", const void *);
    size_t      nbytes = UT_Hook_GetArgValueByName(Context, "nbytes", size_t);
    int32       status;

    if (!UT_Stub_GetInt32StatusCode(Context, &status))
    {
        status = UT_Stub_CopyFromLocal(UT_KEY(OS_GenericWrite_Impl), buffer, nbytes);
        UT_Stub_SetReturnValue(FuncKey, status);
    }
}
