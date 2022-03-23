/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "osconfig.h"
#include "ut-adaptor-module.h"
#include "os-shared-module.h"

int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName)
{
    return OS_SymbolLookup_Static(SymbolAddress, SymbolName, ModuleName);
}

int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName)
{
    return OS_ModuleLoad_Static(ModuleName);
}
