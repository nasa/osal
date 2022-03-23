/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-time header
 */

#include "os-shared-time.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerCbAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_TimerCbAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerCbAPI_Init, int32);

    UT_GenStub_Execute(OS_TimerCbAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimerCbAPI_Init, int32);
}
