/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-mutex header
 */

#include "os-shared-mutex.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutexAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_MutexAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_MutexAPI_Init, int32);

    UT_GenStub_Execute(OS_MutexAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutexAPI_Init, int32);
}
