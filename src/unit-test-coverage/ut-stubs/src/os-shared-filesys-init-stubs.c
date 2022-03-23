/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-filesys header
 */

#include "os-shared-filesys.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_FileSysAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysAPI_Init, int32);

    UT_GenStub_Execute(OS_FileSysAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysAPI_Init, int32);
}
