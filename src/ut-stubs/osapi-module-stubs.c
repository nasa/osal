/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-module header
 */

#include "osapi-module.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_ModuleInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ModuleLoad(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ModuleSymbolLookup(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ModuleUnload(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_SymbolLookup(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleInfo()
 * ----------------------------------------------------
 */
int32 OS_ModuleInfo(osal_id_t module_id, OS_module_prop_t *module_info)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleInfo, int32);

    UT_GenStub_AddParam(OS_ModuleInfo, osal_id_t, module_id);
    UT_GenStub_AddParam(OS_ModuleInfo, OS_module_prop_t *, module_info);

    UT_GenStub_Execute(OS_ModuleInfo, Basic, UT_DefaultHandler_OS_ModuleInfo);

    return UT_GenStub_GetReturnValue(OS_ModuleInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleLoad()
 * ----------------------------------------------------
 */
int32 OS_ModuleLoad(osal_id_t *module_id, const char *module_name, const char *filename, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleLoad, int32);

    UT_GenStub_AddParam(OS_ModuleLoad, osal_id_t *, module_id);
    UT_GenStub_AddParam(OS_ModuleLoad, const char *, module_name);
    UT_GenStub_AddParam(OS_ModuleLoad, const char *, filename);
    UT_GenStub_AddParam(OS_ModuleLoad, uint32, flags);

    UT_GenStub_Execute(OS_ModuleLoad, Basic, UT_DefaultHandler_OS_ModuleLoad);

    return UT_GenStub_GetReturnValue(OS_ModuleLoad, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleSymbolLookup()
 * ----------------------------------------------------
 */
int32 OS_ModuleSymbolLookup(osal_id_t module_id, cpuaddr *symbol_address, const char *symbol_name)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleSymbolLookup, int32);

    UT_GenStub_AddParam(OS_ModuleSymbolLookup, osal_id_t, module_id);
    UT_GenStub_AddParam(OS_ModuleSymbolLookup, cpuaddr *, symbol_address);
    UT_GenStub_AddParam(OS_ModuleSymbolLookup, const char *, symbol_name);

    UT_GenStub_Execute(OS_ModuleSymbolLookup, Basic, UT_DefaultHandler_OS_ModuleSymbolLookup);

    return UT_GenStub_GetReturnValue(OS_ModuleSymbolLookup, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleUnload()
 * ----------------------------------------------------
 */
int32 OS_ModuleUnload(osal_id_t module_id)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleUnload, int32);

    UT_GenStub_AddParam(OS_ModuleUnload, osal_id_t, module_id);

    UT_GenStub_Execute(OS_ModuleUnload, Basic, UT_DefaultHandler_OS_ModuleUnload);

    return UT_GenStub_GetReturnValue(OS_ModuleUnload, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolLookup()
 * ----------------------------------------------------
 */
int32 OS_SymbolLookup(cpuaddr *symbol_address, const char *symbol_name)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolLookup, int32);

    UT_GenStub_AddParam(OS_SymbolLookup, cpuaddr *, symbol_address);
    UT_GenStub_AddParam(OS_SymbolLookup, const char *, symbol_name);

    UT_GenStub_Execute(OS_SymbolLookup, Basic, UT_DefaultHandler_OS_SymbolLookup);

    return UT_GenStub_GetReturnValue(OS_SymbolLookup, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SymbolTableDump()
 * ----------------------------------------------------
 */
int32 OS_SymbolTableDump(const char *filename, size_t size_limit)
{
    UT_GenStub_SetupReturnBuffer(OS_SymbolTableDump, int32);

    UT_GenStub_AddParam(OS_SymbolTableDump, const char *, filename);
    UT_GenStub_AddParam(OS_SymbolTableDump, size_t, size_limit);

    UT_GenStub_Execute(OS_SymbolTableDump, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SymbolTableDump, int32);
}
