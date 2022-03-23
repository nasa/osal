/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains a symbol table implementation for systems
 * that do not use dynamic symbol lookups. It returns OS_ERR_NOT_IMPLEMENTED
 * for all calls.
 */

#include "osapi-module.h"
#include "os-shared-module.h"

/*----------------------------------------------------------------
 * Implementation for no dynamic loader configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Impl(cpuaddr *SymbolAddress, const char *SymbolName)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no dynamic loader configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_ModuleSymbolLookup_Impl(const OS_object_token_t *token, cpuaddr *SymbolAddress, const char *SymbolName)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 * Implementation for no dynamic loader configuration
 *
 * See prototype for argument/return detail
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl(const char *filename, size_t SizeLimit)
{
    return (OS_ERR_NOT_IMPLEMENTED);
}
