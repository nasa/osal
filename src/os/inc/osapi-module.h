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
 *
 * Declarations and prototypes for module subsystem
 */

#ifndef OSAPI_MODULE_H
#define OSAPI_MODULE_H

#include "osconfig.h"
#include "common_types.h"

/*
** Defines
*/

/**
 * @brief Requests OS_ModuleLoad() to add the symbols to the global symbol table
 *
 * When supplied as the "flags" argument to OS_ModuleLoad(), this indicates
 * that the symbols in the loaded module should be added to the global symbol
 * table.  This will make symbols in this library available for use when
 * resolving symbols in future module loads.
 *
 * This is the default mode of operation for OS_ModuleLoad().
 *
 * @note On some operating systems, use of this option may make it difficult
 * to unload the module in the future, if the symbols are in use by other entities.
 *
 */
#define OS_MODULE_FLAG_GLOBAL_SYMBOLS 0x00

/**
 * @brief Requests OS_ModuleLoad() to keep the symbols local/private to this module
 *
 * When supplied as the "flags" argument to OS_ModuleLoad(), this indicates
 * that the symbols in the loaded module should NOT be added to the global
 * symbol table.  This means the symbols in the loaded library will not be
 * available for use by other modules.
 *
 * Use this option is recommended for cases where no other entities will need
 * to reference symbols within this module.  This helps ensure that the module
 * can be more safely unloaded in the future, by preventing other modules from
 * binding to it.  It also helps reduce the likelihood of symbol name conflicts
 * among modules.
 *
 * @note To look up symbols within a module loaded with this flag, use
 * OS_SymbolLookupInModule() instead of OS_SymbolLookup().  Also note that
 * references obtained using this method are not tracked by the OS; the
 * application must ensure that all references obtained in this manner have
 * been cleaned up/released before unloading the module.
 */
#define OS_MODULE_FLAG_LOCAL_SYMBOLS 0x01

/*
** Typedefs
*/

/** @brief OSAL module address properties */
typedef struct
{
    uint32  valid;
    uint32  flags;
    cpuaddr code_address;
    cpuaddr code_size;
    cpuaddr data_address;
    cpuaddr data_size;
    cpuaddr bss_address;
    cpuaddr bss_size;
} OS_module_address_t;

/** @brief OSAL module properties */
typedef struct
{
    cpuaddr             entry_point;
    cpuaddr             host_module_id;
    char                filename[OS_MAX_PATH_LEN];
    char                name[OS_MAX_API_NAME];
    OS_module_address_t addr;
} OS_module_prop_t;

/**
 * @brief Associates a single symbol name with a memory address.
 *
 * If the OS_STATIC_SYMBOL_TABLE feature is enabled, then
 * an array of these structures should be provided by the
 * application.  When the application needs to find a symbol
 * address, the static table will be checked in addition
 * to (or instead of) the OS/library-provided lookup function.
 *
 * This static symbol allows systems that do not implement
 * dynamic module loading to maintain the same semantics
 * as dynamically loaded modules.
 */
typedef const struct
{
    const char *Name;
    void (*Address)(void);
    const char *Module;
} OS_static_symbol_record_t;

/** @defgroup OSAPILoader OSAL Dynamic Loader and Symbol APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the Address of a Symbol
 *
 * This calls to the OS dynamic symbol lookup implementation,
 * and/or checks a static symbol table for a matching symbol name.
 *
 * The static table is intended to support embedded targets that do
 * not have module loading capability or have it disabled.
 *
 * @param[out] symbol_address Set to the address of the symbol @nonnull
 * @param[in]  symbol_name    Name of the symbol to look up @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the symbol could not be found
 * @retval #OS_INVALID_POINTER if one of the pointers passed in are NULL
 */
int32 OS_SymbolLookup(cpuaddr *symbol_address, const char *symbol_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find the Address of a Symbol within a module
 *
 * This is similar to OS_SymbolLookup() but for a specific module ID.
 * This should be used to look up a symbol in a module that has been
 * loaded with the #OS_MODULE_FLAG_LOCAL_SYMBOLS flag.
 *
 * @param[in]  module_id      Module ID that should contain the symbol
 * @param[out] symbol_address Set to the address of the symbol @nonnull
 * @param[in]  symbol_name    Name of the symbol to look up @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the symbol could not be found
 * @retval #OS_INVALID_POINTER if one of the pointers passed in are NULL
 */
int32 OS_ModuleSymbolLookup(osal_id_t module_id, cpuaddr *symbol_address, const char *symbol_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Dumps the system symbol table to a file
 *
 * Dumps the system symbol table to the specified filename
 *
 * @note Not all RTOS implementations support this API.  If the underlying
 * module subsystem does not provide a facility to iterate through the
 * symbol table, then the #OS_ERR_NOT_IMPLEMENTED status code is returned.
 *
 * @param[in] filename  File to write to @nonnull
 * @param[in] size_limit Maximum number of bytes to write
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS             @copybrief OS_SUCCESS
 * @retval #OS_ERR_NOT_IMPLEMENTED @copybrief OS_ERR_NOT_IMPLEMENTED
 * @retval #OS_INVALID_POINTER if the filename argument is NULL
 * @retval #OS_FS_ERR_PATH_INVALID if the filename argument is not valid
 * @retval #OS_ERR_NAME_TOO_LONG if any of the symbol names are too long @covtest
 * @retval #OS_ERR_OUTPUT_TOO_LARGE if the size_limit was reached before completing all symbols @covtest
 * @retval #OS_ERROR if an other/unspecified error occurs @covtest
 */
int32 OS_SymbolTableDump(const char *filename, size_t size_limit);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Loads an object file
 *
 * Loads an object file into the running operating system
 *
 * The "flags" parameter may influence how the loaded module symbols are made
 * available for use in the application.  See #OS_MODULE_FLAG_LOCAL_SYMBOLS
 * and #OS_MODULE_FLAG_GLOBAL_SYMBOLS for descriptions.
 *
 * @param[out] module_id    Non-zero OSAL ID corresponding to the loaded module
 * @param[in]  module_name  Name of module @nonnull
 * @param[in]  filename     File containing the object code to load @nonnull
 * @param[in]  flags        Options for the loaded module
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if one of the parameters is NULL
 * @retval #OS_ERR_NO_FREE_IDS if the module table is full
 * @retval #OS_ERR_NAME_TAKEN if the name is in use
 * @retval #OS_ERR_NAME_TOO_LONG if the module_name is too long
 * @retval #OS_FS_ERR_PATH_INVALID if the filename argument is not valid
 * @retval #OS_ERROR if an other/unspecified error occurs @covtest
 */
int32 OS_ModuleLoad(osal_id_t *module_id, const char *module_name, const char *filename, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unloads the module file
 *
 * Unloads the module file from the running operating system
 *
 * @param[in] module_id    OSAL ID of the previously the loaded module
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the module id invalid
 * @retval #OS_ERROR if an other/unspecified error occurs @covtest
 */
int32 OS_ModuleUnload(osal_id_t module_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a module
 *
 * Returns information about the loadable module
 *
 * @param[in]  module_id    OSAL ID of the previously the loaded module
 * @param[out] module_info  Buffer to store module information @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the module id invalid
 * @retval #OS_INVALID_POINTER if the pointer to the ModuleInfo structure is invalid
 * @retval #OS_ERROR if an other/unspecified error occurs @covtest
 */
int32 OS_ModuleInfo(osal_id_t module_id, OS_module_prop_t *module_info);
/**@}*/

#endif /* OSAPI_MODULE_H */
