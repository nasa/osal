/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-clock header
 */

#include "os-shared-clock.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetLocalTime_Impl()
 * ----------------------------------------------------
 */
int32 OS_GetLocalTime_Impl(OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_GetLocalTime_Impl, int32);

    UT_GenStub_AddParam(OS_GetLocalTime_Impl, OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_GetLocalTime_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_GetLocalTime_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SetLocalTime_Impl()
 * ----------------------------------------------------
 */
int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct)
{
    UT_GenStub_SetupReturnBuffer(OS_SetLocalTime_Impl, int32);

    UT_GenStub_AddParam(OS_SetLocalTime_Impl, const OS_time_t *, time_struct);

    UT_GenStub_Execute(OS_SetLocalTime_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SetLocalTime_Impl, int32);
}
