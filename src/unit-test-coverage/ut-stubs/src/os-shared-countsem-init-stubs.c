/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-countsem header
 */

#include "os-shared-countsem.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_CountSemAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemAPI_Init, int32);

    UT_GenStub_Execute(OS_CountSemAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemAPI_Init, int32);
}
