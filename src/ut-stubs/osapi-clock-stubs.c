/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-clock header
 */

#include "osapi-clock.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_GetLocalTime(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SetLocalTime(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetLocalTime()
 * ----------------------------------------------------
 */
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_GetLocalTime, int32);

    UT_GenStub_AddParam(OS_GetLocalTime, OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_GetLocalTime, Basic, UT_DefaultHandler_OS_GetLocalTime);

    return UT_GenStub_GetReturnValue(OS_GetLocalTime, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SetLocalTime()
 * ----------------------------------------------------
 */
int32 OS_SetLocalTime(const OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_SetLocalTime, int32);

    UT_GenStub_AddParam(OS_SetLocalTime, const OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_SetLocalTime, Basic, UT_DefaultHandler_OS_SetLocalTime);

    return UT_GenStub_GetReturnValue(OS_SetLocalTime, int32);
}
