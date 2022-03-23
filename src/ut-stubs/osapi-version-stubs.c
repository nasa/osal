/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-version header
 */

#include "osapi-version.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetVersionCodeName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_GetVersionString(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetBuildNumber()
 * ----------------------------------------------------
 */
uint32 OS_GetBuildNumber(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetBuildNumber, uint32);

    UT_GenStub_Execute(OS_GetBuildNumber, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_GetBuildNumber, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionCodeName()
 * ----------------------------------------------------
 */
const char *OS_GetVersionCodeName(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetVersionCodeName, const char *);

    UT_GenStub_Execute(OS_GetVersionCodeName, Basic, UT_DefaultHandler_OS_GetVersionCodeName);

    return UT_GenStub_GetReturnValue(OS_GetVersionCodeName, const char *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionNumber()
 * ----------------------------------------------------
 */
void OS_GetVersionNumber(uint8 VersionNumbers[4])
{

    UT_GenStub_Execute(OS_GetVersionNumber, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetVersionString()
 * ----------------------------------------------------
 */
const char *OS_GetVersionString(void)
{
    UT_GenStub_SetupReturnBuffer(OS_GetVersionString, const char *);

    UT_GenStub_Execute(OS_GetVersionString, Basic, UT_DefaultHandler_OS_GetVersionString);

    return UT_GenStub_GetReturnValue(OS_GetVersionString, const char *);
}
