/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-task header
 */

#include "os-shared-task.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TaskAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_TaskAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_TaskAPI_Init, int32);

    UT_GenStub_Execute(OS_TaskAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TaskAPI_Init, int32);
}
