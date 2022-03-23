/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-binsem header
 */

#include "os-shared-binsem.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_BinSemAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemAPI_Init, int32);

    UT_GenStub_Execute(OS_BinSemAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BinSemAPI_Init, int32);
}
