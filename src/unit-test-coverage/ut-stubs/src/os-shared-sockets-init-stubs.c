/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-sockets header
 */

#include "os-shared-sockets.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SocketAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_SocketAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_SocketAPI_Init, int32);

    UT_GenStub_Execute(OS_SocketAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SocketAPI_Init, int32);
}
