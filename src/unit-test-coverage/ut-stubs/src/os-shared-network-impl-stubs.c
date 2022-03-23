/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-network header
 */

#include "os-shared-network.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_NetworkGetID_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetHostName_Impl()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetHostName_Impl(char *host_name, size_t name_len)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetHostName_Impl, int32);

    UT_GenStub_AddParam(OS_NetworkGetHostName_Impl, char *, host_name);
    UT_GenStub_AddParam(OS_NetworkGetHostName_Impl, size_t, name_len);

    UT_GenStub_Execute(OS_NetworkGetHostName_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_NetworkGetHostName_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetID_Impl()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetID_Impl(int32 *IdBuf)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetID_Impl, int32);

    UT_GenStub_AddParam(OS_NetworkGetID_Impl, int32 *, IdBuf);

    UT_GenStub_Execute(OS_NetworkGetID_Impl, Basic, UT_DefaultHandler_OS_NetworkGetID_Impl);

    return UT_GenStub_GetReturnValue(OS_NetworkGetID_Impl, int32);
}
