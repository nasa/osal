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

/*================================================================================*
** File:  ut_osloader_symtable_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_symtable_test.h"
#include "ut_osloader_test_platforms.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/**
 * The size limit to pass for OS_SymbolTableDump nominal test
 *
 * This must be large enough to actually accommodate all of the symbols
 * in the target system.
 */
#define UT_SYMTABLE_SIZE_LIMIT 1048576

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_SymbolLookup
** Purpose: Returns the memory address of a symbol
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERROR if the symbol name is not found
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/

void UT_os_symbol_lookup_test()
{
    cpuaddr   symbol_addr;
    osal_id_t module_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* API Not implemented */

    if (!UT_IMPL(OS_SymbolLookup(&symbol_addr, "main")))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg-1 */

    UT_RETVAL(OS_SymbolLookup(0, "Sym"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-2 */

    UT_RETVAL(OS_SymbolLookup(&symbol_addr, 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* Setup for global symbol test */
    if (UT_SETUP(OS_ModuleLoad(&module_id, "Mod1", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_GLOBAL_SYMBOLS)))
    {
        /*-----------------------------------------------------*/
        /* #3 Symbol-not-found */

        UT_RETVAL(OS_SymbolLookup(&symbol_addr, "NotFound"), OS_ERROR);

        /*-----------------------------------------------------*/
        /* #4 Nominal, Global Symbols */

        UT_NOMINAL(OS_SymbolLookup(&symbol_addr, "module1"));

        /* Reset test environment */
        UT_TEARDOWN(OS_ModuleUnload(module_id));
    }

    /*-----------------------------------------------------*/
    /* Setup for local symbol test */
    if (UT_SETUP(OS_ModuleLoad(&module_id, "Mod1", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        /*-----------------------------------------------------*/
        /* #5 Nominal, Local Symbols */

        UT_NOMINAL(OS_SymbolLookup(&symbol_addr, "module1"));

        /* Reset test environment */
        UT_TEARDOWN(OS_ModuleUnload(module_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleSymbolLookup
** Purpose: Returns the memory address of a symbol
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERROR if the symbol name is not found
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/

void UT_os_module_symbol_lookup_test()
{
    cpuaddr   symbol_addr;
    osal_id_t module_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* API Not implemented */

    if (!UT_IMPL(OS_ModuleSymbolLookup(OS_OBJECT_ID_UNDEFINED, &symbol_addr, "main")))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* Invalid object ID */

    UT_RETVAL(OS_ModuleSymbolLookup(OS_OBJECT_ID_UNDEFINED, &symbol_addr, "Sym"), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_ModuleSymbolLookup(UT_OBJID_INCORRECT, &symbol_addr, "Sym"), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* Setup for remainder of tests */
    if (UT_SETUP(OS_ModuleLoad(&module_id, "Mod1", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        /*-----------------------------------------------------*/
        /* #1 Invalid-pointer-arg */

        UT_RETVAL(OS_ModuleSymbolLookup(module_id, NULL, "Sym"), OS_INVALID_POINTER);
        UT_RETVAL(OS_ModuleSymbolLookup(module_id, &symbol_addr, NULL), OS_INVALID_POINTER);

        /*-----------------------------------------------------*/
        /* #3 Symbol-not-found */
        UT_RETVAL(OS_ModuleSymbolLookup(module_id, &symbol_addr, "NotFound"), OS_ERROR);

        /*-----------------------------------------------------*/
        /* #4 Nominal, Local Symbols */

        UT_NOMINAL(OS_ModuleSymbolLookup(module_id, &symbol_addr, "module1"));

        /* Reset test environment */
        UT_TEARDOWN(OS_ModuleUnload(module_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_SymbolTableDump
** Purpose: Dumps the system symbol table to the given filename
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_INVALID if the filename is invalid
**          OS_ERROR if there was any problem writing the symbol table to the file
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_symbol_table_dump_test()
{
    int32 status;
    /*
     * Note that even if the functionality is not implemented,
     * the API still validates the input pointers (not null) and
     * the validity of the file name.
     */

    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg */

    UT_RETVAL(OS_SymbolTableDump(0, 10000), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path */

    UT_RETVAL(OS_SymbolTableDump("/this/path/is/invalid.dat", 10000), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    status = OS_SymbolTableDump(UT_OS_GENERIC_MODULE_DIR "SymbolReal.dat", UT_SYMTABLE_SIZE_LIMIT);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SymbolTableDump API not implemented");
    }
    else if (status == OS_ERR_OUTPUT_TOO_LARGE)
    {
        UtAssert_MIR("UT_SYMTABLE_SIZE_LIMIT too small for OS_SymbolTableDump");
    }
    else if (status == OS_ERR_NAME_TOO_LONG)
    {
        UtAssert_MIR("OSAL_CONFIG_MAX_SYM_LEN too small for OS_SymbolTableDump");
    }
    else
    {
        UtAssert_True(status == OS_SUCCESS, "status after 128k OS_SymbolTableDump = %d", (int)status);
    }

    if (status == OS_SUCCESS)
    {
        UT_RETVAL(OS_SymbolTableDump(UT_OS_GENERIC_MODULE_DIR "SymbolZero.dat", 0), OS_ERR_OUTPUT_TOO_LARGE);
    }
}

/*================================================================================*
** End of File: ut_osloader_symtable_test.c
**================================================================================*/
