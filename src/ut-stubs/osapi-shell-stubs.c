/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-shell header
 */

#include "osapi-shell.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ShellOutputToFile()
 * ----------------------------------------------------
 */
int32 OS_ShellOutputToFile(const char *Cmd, osal_id_t filedes)
{
    UT_GenStub_SetupReturnBuffer(OS_ShellOutputToFile, int32);

    UT_GenStub_AddParam(OS_ShellOutputToFile, const char *, Cmd);
    UT_GenStub_AddParam(OS_ShellOutputToFile, osal_id_t, filedes);

    UT_GenStub_Execute(OS_ShellOutputToFile, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ShellOutputToFile, int32);
}
