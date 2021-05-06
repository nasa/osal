/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     ut-adaptor-module.c
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
