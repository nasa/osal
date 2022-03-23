/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-queue header
 */

#include "os-shared-queue.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_QueueAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueAPI_Init, int32);

    UT_GenStub_Execute(OS_QueueAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueueAPI_Init, int32);
}
