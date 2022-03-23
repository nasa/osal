/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-console header
 */

#include "os-shared-console.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConsoleCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ConsoleCreate_Impl, int32);

    UT_GenStub_AddParam(OS_ConsoleCreate_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ConsoleCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ConsoleCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConsoleWakeup_Impl()
 * ----------------------------------------------------
 */
void OS_ConsoleWakeup_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_ConsoleWakeup_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ConsoleWakeup_Impl, Basic, NULL);
}
