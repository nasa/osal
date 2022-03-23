/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-console header
 */

#include "os-shared-console.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConsoleAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_ConsoleAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_ConsoleAPI_Init, int32);

    UT_GenStub_Execute(OS_ConsoleAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ConsoleAPI_Init, int32);
}
