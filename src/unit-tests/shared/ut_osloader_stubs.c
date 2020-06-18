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

/*================================================================================*
** File:  ut_osloader_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsReturnCode_t  g_moduleTblInit = {0,0};
UT_OsReturnCode_t  g_moduleLoad = {0,0};
UT_OsReturnCode_t  g_moduleUnload = {0,0};
UT_OsReturnCode_t  g_moduleInfo = {0,0};

UT_OsReturnCode_t  g_symbolTblLookup = {0,0};
UT_OsReturnCode_t  g_symbolTblDump = {0,0};


/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_ModuleTableInit()
{
    return (g_moduleTblInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_ModuleLoad(uint32* module_id, char* module_name, char* filename)
{
    return (g_moduleLoad.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_ModuleUnload(uint32 module_id)
{
    return (g_moduleUnload.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_ModuleInfo(uint32 module_id, OS_module_prop_t* module_info)
{
    return (g_moduleInfo.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_SymbolLookup(uint32* SymbolAddress, char* SymbolName)
{
    return (g_symbolTblLookup.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_SymbolTableDump(char* filename, uint32 SizeLimit)
{
    return (g_symbolTblDump.value);
}

/*================================================================================*
** End of File: ut_osloader_stubs.c
**================================================================================*/
