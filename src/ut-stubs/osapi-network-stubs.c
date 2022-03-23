/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-network header
 */

#include "osapi-network.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_NetworkGetHostName(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetHostName()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetHostName(char *host_name, size_t name_len)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetHostName, int32);

    UT_GenStub_AddParam(OS_NetworkGetHostName, char *, host_name);
    UT_GenStub_AddParam(OS_NetworkGetHostName, size_t, name_len);

    UT_GenStub_Execute(OS_NetworkGetHostName, Basic, UT_DefaultHandler_OS_NetworkGetHostName);

    return UT_GenStub_GetReturnValue(OS_NetworkGetHostName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkGetID()
 * ----------------------------------------------------
 */
int32 OS_NetworkGetID(void)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkGetID, int32);

    UT_GenStub_Execute(OS_NetworkGetID, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_NetworkGetID, int32);
}
