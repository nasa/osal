/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-error header
 */

#include "osapi-error.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetErrorName(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetErrorName()
 * ----------------------------------------------------
 */
int32 OS_GetErrorName(int32 error_num, os_err_name_t *err_name)
{
    UT_GenStub_SetupReturnBuffer(OS_GetErrorName, int32);

    UT_GenStub_AddParam(OS_GetErrorName, int32, error_num);
    UT_GenStub_AddParam(OS_GetErrorName, os_err_name_t *, err_name);

    UT_GenStub_Execute(OS_GetErrorName, Basic, UT_DefaultHandler_OS_GetErrorName);

    return UT_GenStub_GetReturnValue(OS_GetErrorName, int32);
}
