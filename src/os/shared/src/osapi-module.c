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
 * \file     osapi-module.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-module.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined
 *
 * OS_MAX_MODULES is allowed to be zero in which case the
 * table is not instantiated.
 */
#if !defined(OS_MAX_MODULES) || (OS_MAX_MODULES <= 0)
#error "osconfig.h must define OS_MAX_MODULES to a valid value"
#endif

enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_MODULES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_MODULE
};

OS_module_internal_record_t OS_module_table[OS_MAX_MODULES];

/*
 * If the "OS_STATIC_LOADER" directive is enabled,
 * then the user application/BSP must provide a symbol
 * called "OS_STATIC_SYMBOL_TABLE" which will provide
 * user-defined mappings of symbol names to addresses.
 *
 * Note - when compiling unit tests, the UT code will
 * supply a custom definition for OS_STATIC_SYMTABLE_SOURCE
 */
#if !defined(OS_STATIC_SYMTABLE_SOURCE) && defined(OSAL_CONFIG_INCLUDE_STATIC_LOADER)
/* use default symbol name for static table */
#define OS_STATIC_SYMTABLE_SOURCE OS_STATIC_SYMBOL_TABLE
#endif

#ifdef OS_STATIC_SYMTABLE_SOURCE
/* the BSP should supply the static symbol table when this is set */
extern OS_static_symbol_record_t OS_STATIC_SYMTABLE_SOURCE[];
#else
/* there is no static symbol table, use NULL */
#define OS_STATIC_SYMTABLE_SOURCE NULL
#endif /* OS_STATIC_SYMTABLE_SOURCE */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup_Static
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks for a symbol name in the static symbol table
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName)
{
    int32                      return_code = OS_ERR_NOT_IMPLEMENTED;
    OS_static_symbol_record_t *StaticSym   = OS_STATIC_SYMTABLE_SOURCE;

    while (StaticSym != NULL)
    {
        if (StaticSym->Name == NULL)
        {
            /* end of list --
             * Return "OS_ERROR" to indicate that an actual search was done
             * with a not-found result, vs. not searching at all. */
            return_code = OS_ERROR;
            break;
        }
        if (strcmp(StaticSym->Name, SymbolName) == 0 &&
                (ModuleName == NULL || strcmp(StaticSym->Module, ModuleName) == 0))
        {
            /* found matching symbol */
            *SymbolAddress = (cpuaddr)StaticSym->Address;
            return_code    = OS_SUCCESS;
            break;
        }

        ++StaticSym;
    }

    return return_code;
} /* end OS_SymbolLookup_Static */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Static
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks for a module name in the static symbol table
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Static(const char *ModuleName)
{
    int32                      return_code = OS_ERR_NAME_NOT_FOUND;
    OS_static_symbol_record_t *StaticSym   = OS_STATIC_SYMTABLE_SOURCE;

    while (StaticSym != NULL)
    {
        if (StaticSym->Name == NULL)
        {
            /* end of list  */
            break;
        }
        if (StaticSym->Module != NULL && strcmp(StaticSym->Module, ModuleName) == 0)
        {
            /* found matching module name */
            return_code = OS_SUCCESS;
            break;
        }

        ++StaticSym;
    }

    return return_code;
} /* end OS_ModuleLoad_Static */

/****************************************************************************************
                                   Module API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleAPI_Init(void)
{
    memset(OS_module_table, 0, sizeof(OS_module_table));
    return OS_SUCCESS;
} /* end OS_ModuleAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad(osal_id_t *module_id, const char *module_name, const char *filename, uint32 flags)
{
    char                         translated_path[OS_MAX_LOCAL_PATH_LEN];
    int32                        return_code;
    int32                        filename_status;
    OS_object_token_t            token;
    OS_module_internal_record_t *module;

    /*
     ** Check parameters
     **
     ** Note "filename" is not checked, because in certain configurations it can be validly
     ** null.  filename is checked for NULL-ness by the OS_TranslatePath() later.
     */
    if ((module_id == NULL) || (module_name == NULL))
    {
        return (OS_INVALID_POINTER);
    }

    if (strlen(module_name) >= OS_MAX_API_NAME)
    {
        return (OS_ERR_NAME_TOO_LONG);
    }

    /*
     * Preemptively translate the filename, and hold it in a temporary buffer.
     *
     * This should be done before allocating a new object ID because it also
     * locks the global table, and this prevents double-locking.
     *
     * The status of this operation is stored separately, because it may or
     * may not be relevant, depending on whether the static module table is enabled.
     */
    filename_status = OS_TranslatePath(filename, translated_path);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, module_name, &token);
    if (return_code == OS_SUCCESS)
    {
        module = OS_OBJECT_TABLE_GET(OS_module_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, module, module_name, module_name);

        module->flags = flags; /* save user-supplied flags */

        /*
         * Check the statically-linked module list.
         * If a matching entry is found, this means its
         * already effectively "loaded" through static linkage.
         * Return success without any more action.
         *
         * If the OSAL_CONFIG_INCLUDE_STATIC_LOADER feature is disabled,
         * then the list of static modules is empty and this always
         * returns OS_ERR_NAME_NOT_FOUND.
         */
        return_code = OS_ModuleLoad_Static(module_name);
        if (return_code == OS_SUCCESS)
        {
            /* mark this as a statically loaded module */
            module->module_type = OS_MODULE_TYPE_STATIC;
        }
        else
        {
            /*
             * If this is NOT a static module, then the module file must be loaded by normal
             * means using the dynamic loader, if available.  This also means the filename
             * must be valid, so this is when the "filename_status" is checked/enforced.
             */
            if (filename_status != OS_SUCCESS)
            {
                /* supplied filename was not valid */
                return_code = filename_status;
            }
            else
            {
                /* supplied filename was valid, so store a copy for future reference */
                strncpy(module->file_name, filename, OS_MAX_PATH_LEN);
                module->module_type = OS_MODULE_TYPE_DYNAMIC;

                /* Now call the OS-specific implementation.  This reads info from the module table. */
                return_code = OS_ModuleLoad_Impl(&token, translated_path);
            }
        }

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, module_id);
    }

    return (return_code);

} /* end OS_ModuleLoad */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload(osal_id_t module_id)
{
    OS_module_internal_record_t *module;
    int32                        return_code;
    OS_object_token_t            token;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, module_id, &token);
    if (return_code == OS_SUCCESS)
    {
        module = OS_OBJECT_TABLE_GET(OS_module_table, token);

        /*
         * Only call the implementation if the file was actually loaded.
         * If this is a static module, then this is just a placeholder and
         * it means there was no file actually loaded.
         */
        if (module->module_type == OS_MODULE_TYPE_DYNAMIC)
        {
            return_code = OS_ModuleUnload_Impl(&token);
        }

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;
} /* end OS_ModuleUnload */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleInfo(osal_id_t module_id, OS_module_prop_t *module_prop)
{
    OS_common_record_t *         record;
    OS_module_internal_record_t *module;
    int32                        return_code;
    OS_object_token_t            token;

    /* Check parameters */
    if (module_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    memset(module_prop, 0, sizeof(OS_module_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, module_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_module_table, token);
        module = OS_OBJECT_TABLE_GET(OS_module_table, token);

        strncpy(module_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
        strncpy(module_prop->filename, module->file_name, OS_MAX_API_NAME - 1);

        return_code = OS_ModuleGetInfo_Impl(&token, module_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;

} /* end OS_ModuleInfo */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolLookup
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup(cpuaddr *SymbolAddress, const char *SymbolName)
{
    int32 return_code;
    int32 staticsym_status;

    /*
    ** Check parameters
    */
    if ((SymbolAddress == NULL) || (SymbolName == NULL))
    {
        return (OS_INVALID_POINTER);
    }

    /*
     * attempt to find the symbol in the global symbol table.
     */
    return_code = OS_GlobalSymbolLookup_Impl(SymbolAddress, SymbolName);

    /*
     * If the OS call did not find the symbol or the loader is
     * disabled, then check if a static symbol table is present
     */
    if (return_code != OS_SUCCESS)
    {
        staticsym_status = OS_SymbolLookup_Static(SymbolAddress, SymbolName, NULL);

        /*
         * Only overwrite the return code if static lookup was successful.
         * Otherwise keep the error code from the low level implementation.
         */
        if (staticsym_status == OS_SUCCESS)
        {
            return_code = staticsym_status;
        }
    }

    return (return_code);

} /* end OS_SymbolLookup */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleSymbolLookup
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleSymbolLookup(osal_id_t module_id, cpuaddr *symbol_address, const char *symbol_name)
{
    int32               return_code;
    int32               staticsym_status;
    OS_common_record_t *record;
    OS_object_token_t   token;

    /*
    ** Check parameters
    */
    if ((symbol_address == NULL) || (symbol_name == NULL))
    {
        return (OS_INVALID_POINTER);
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, module_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_module_table, token);

        return_code = OS_ModuleSymbolLookup_Impl(&token, symbol_address, symbol_name);
        if (return_code != OS_SUCCESS)
        {
            /* look for a static symbol that also matches this module name */
            staticsym_status = OS_SymbolLookup_Static(symbol_address, symbol_name, record->name_entry);

            /*
             * Only overwrite the return code if static lookup was successful.
             * Otherwise keep the error code from the low level implementation.
             */
            if (staticsym_status == OS_SUCCESS)
            {
                return_code = staticsym_status;
            }
        }

        OS_ObjectIdRelease(&token);
    }

    return (return_code);

} /* end OS_ModuleSymbolLookup */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump(const char *filename, size_t SizeLimit)
{
    int32 return_code;
    char  translated_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check parameters
    */
    if (filename == NULL)
    {
        return (OS_INVALID_POINTER);
    }

    /*
     ** Translate the filename to the Host System
     */
    return_code = OS_TranslatePath(filename, translated_path);
    if (return_code != OS_SUCCESS)
    {
        return (return_code);
    }

    /*
     * Locking the global ensures only one symbol table dump
     * can be executing.  It also prevents module loading/unloading
     * while the dump is occurring.
     *
     * Because calls to this function are serialized, the
     * underlying implementation may safely use globals for
     * state storage.
     */
    OS_Lock_Global(LOCAL_OBJID_TYPE);

    return_code = OS_SymbolTableDump_Impl(translated_path, SizeLimit);

    OS_Unlock_Global(LOCAL_OBJID_TYPE);

    return (return_code);

} /* end OS_SymbolTableDump */
