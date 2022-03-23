/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-shell header
 */

#include "os-shared-shell.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ShellOutputToFile_Impl()
 * ----------------------------------------------------
 */
int32 OS_ShellOutputToFile_Impl(const OS_object_token_t *token, const char *Cmd)
{
    UT_GenStub_SetupReturnBuffer(OS_ShellOutputToFile_Impl, int32);

    UT_GenStub_AddParam(OS_ShellOutputToFile_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ShellOutputToFile_Impl, const char *, Cmd);

    UT_GenStub_Execute(OS_ShellOutputToFile_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ShellOutputToFile_Impl, int32);
}
