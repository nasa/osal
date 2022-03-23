/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-module header
 */

#include "os-shared-module.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolLookup_Impl()
 * ----------------------------------------------------
 */
int32 OS_SymbolLookup_Impl(cpuaddr *SymbolAddress, const char *SymbolName)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolLookup_Impl, int32);

    UT_GenStub_AddParam(OS_SymbolLookup_Impl, cpuaddr *, SymbolAddress);
    UT_GenStub_AddParam(OS_SymbolLookup_Impl, const char *, SymbolName);

    UT_GenStub_Execute(OS_SymbolLookup_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SymbolLookup_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_ModuleGetInfo_Impl(const OS_object_token_t *token, OS_module_prop_t *module_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_ModuleGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ModuleGetInfo_Impl, OS_module_prop_t *, module_prop);

    UT_GenStub_Execute(OS_ModuleGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleLoad_Impl()
 * ----------------------------------------------------
 */
int32 OS_ModuleLoad_Impl(const OS_object_token_t *token, const char *translated_path)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleLoad_Impl, int32);

    UT_GenStub_AddParam(OS_ModuleLoad_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ModuleLoad_Impl, const char *, translated_path);

    UT_GenStub_Execute(OS_ModuleLoad_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleLoad_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleSymbolLookup_Impl()
 * ----------------------------------------------------
 */
int32 OS_ModuleSymbolLookup_Impl(const OS_object_token_t *token, cpuaddr *SymbolAddress, const char *SymbolName)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleSymbolLookup_Impl, int32);

    UT_GenStub_AddParam(OS_ModuleSymbolLookup_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ModuleSymbolLookup_Impl, cpuaddr *, SymbolAddress);
    UT_GenStub_AddParam(OS_ModuleSymbolLookup_Impl, const char *, SymbolName);

    UT_GenStub_Execute(OS_ModuleSymbolLookup_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleSymbolLookup_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleUnload_Impl()
 * ----------------------------------------------------
 */
int32 OS_ModuleUnload_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleUnload_Impl, int32);

    UT_GenStub_AddParam(OS_ModuleUnload_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_ModuleUnload_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleUnload_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolTableDump_Impl()
 * ----------------------------------------------------
 */
int32 OS_SymbolTableDump_Impl(const char *filename, size_t size_limit)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolTableDump_Impl, int32);

    UT_GenStub_AddParam(OS_SymbolTableDump_Impl, const char *, filename);
    UT_GenStub_AddParam(OS_SymbolTableDump_Impl, size_t, size_limit);

    UT_GenStub_Execute(OS_SymbolTableDump_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SymbolTableDump_Impl, int32);
}
