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
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_MODULE_H
#define OS_SHARED_MODULE_H

#include "osapi-module.h"
#include "os-shared-globaldefs.h"

typedef enum
{
    OS_MODULE_TYPE_UNKNOWN = 0, /**< Default/unspecified (reserved value) */
    OS_MODULE_TYPE_DYNAMIC = 1, /**< Module is dynamically loaded via the OS loader */
    OS_MODULE_TYPE_STATIC  = 2  /**< Module is statically linked and is a placeholder */
} OS_module_type_t;

typedef struct
{
    char             module_name[OS_MAX_API_NAME];
    char             file_name[OS_MAX_PATH_LEN];
    OS_module_type_t module_type;
    uint32           flags;
    cpuaddr          entry_point;
} OS_module_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_module_internal_record_t OS_module_table[OS_MAX_MODULES];

/****************************************************************************************
                 MODULE LOADER API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_ModuleAPI_Init

   Purpose: Initialize the OS-independent layer for modules

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_ModuleAPI_Init(void);

/*----------------------------------------------------------------
   Function: OS_ModuleLoad_Impl

    Purpose: Loads an object file into the running operating system

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl(const OS_object_token_t *token, const char *translated_path);

/*----------------------------------------------------------------

   Function: OS_ModuleUnload_Impl

    Purpose: Unloads the module file from the running operating system

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ModuleGetInfo_Impl

    Purpose: Returns information about the loadable module

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl(const OS_object_token_t *token, OS_module_prop_t *module_prop);

/*----------------------------------------------------------------
   Function: OS_GlobalSymbolLookup_Impl

    Purpose: Find the Address of a Symbol in the global symbol table.
             The address of the symbol will be stored in the pointer that is passed in.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_GlobalSymbolLookup_Impl(cpuaddr *SymbolAddress, const char *SymbolName);

/*----------------------------------------------------------------
   Function: OS_SymbolLookup_Impl

    Purpose: Find the Address of a Symbol within a specific module.
             The address of the symbol will be stored in the pointer that is passed in.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ModuleSymbolLookup_Impl(const OS_object_token_t *token, cpuaddr *SymbolAddress, const char *SymbolName);

/*----------------------------------------------------------------
   Function: OS_SymbolTableDump_Impl

    Purpose: Dumps the system symbol table to a file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl(const char *filename, size_t size_limit);

/*
 * Helper functions within the shared layer that are not normally invoked outside the local module
 * These need to be exposed for unit testing
 */
int32 OS_ModuleLoad_Static(const char *ModuleName);
int32 OS_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName);

#endif /* OS_SHARED_MODULE_H */
