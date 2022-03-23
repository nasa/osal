/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-module header
 */

#include "os-shared-module.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleLoad_Static()
 * ----------------------------------------------------
 */
int32 OS_ModuleLoad_Static(const char *ModuleName)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleLoad_Static, int32);

    UT_GenStub_AddParam(OS_ModuleLoad_Static, const char *, ModuleName);

    UT_GenStub_Execute(OS_ModuleLoad_Static, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleLoad_Static, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolLookup_Static()
 * ----------------------------------------------------
 */
int32 OS_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolLookup_Static, int32);

    UT_GenStub_AddParam(OS_SymbolLookup_Static, cpuaddr *, SymbolAddress);
    UT_GenStub_AddParam(OS_SymbolLookup_Static, const char *, SymbolName);
    UT_GenStub_AddParam(OS_SymbolLookup_Static, const char *, ModuleName);

    UT_GenStub_Execute(OS_SymbolLookup_Static, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SymbolLookup_Static, int32);
}
