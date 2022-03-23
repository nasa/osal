/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-network header
 */

#include "os-shared-network.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NetworkAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_NetworkAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_NetworkAPI_Init, int32);

    UT_GenStub_Execute(OS_NetworkAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_NetworkAPI_Init, int32);
}
