/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

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
    return OS_ERR_NOT_IMPLEMENTED;
}
