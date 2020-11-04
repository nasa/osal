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
 * \file   os-impl-posix-dl-symtab.c
 * \author joseph.p.hickey@nasa.gov
 *
 * This file contains a module loader implementation for systems
 * that implement a POSIX-style dynamic module loader.  This includes
 * RTEMS even if built without its native POSIX API.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * typically this must include dlfcn.h and whatever else is required
 * to get the prototypes of these functions:
 *
 *   dlsym()
 *   dlerror()
 *
 * In POSIX these functions are grouped with the loader (dl) library
 */
#include <string.h>
#include <stdlib.h>

#include "os-impl-loader.h"
#include "os-shared-module.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/*
 * Determine what to pass in for the first parameter of dlsym()
 *
 * If the "os-impl-loader.h" header already defined this, then use that.
 *
 * Otherwise, check if the C library provides an "RTLD_DEFAULT" symbol -
 * This symbol is not POSIX standard but many implementations do provide it.
 *
 * Lastly, if nothing no special handle that indicates the global symbol
 * table is defined, then OS_GlobalSymbolLookup_Impl() will return
 * OS_ERR_NOT_IMPLEMENTED rather than relying on undefined behavior.
 */
#if !defined(OSAL_DLSYM_GLOBAL_HANDLE) && defined(RTLD_DEFAULT)
#define OSAL_DLSYM_GLOBAL_HANDLE RTLD_DEFAULT
#endif

/****************************************************************************************
                                    Symbol table API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_GenericSymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericSymbolLookup_Impl(void *dl_handle, cpuaddr *SymbolAddress, const char *SymbolName)
{
    const char *dlError; /*  Pointer to error string   */
    void       *Function;
    int32       status;

    status = OS_ERROR;

    /*
     * call dlerror() to clear any prior error that might have occurred.
     */
    dlerror();
    Function = dlsym(dl_handle, SymbolName);
    dlError  = dlerror();

    /*
     * For the POSIX DL implementation, if the symbol does not exist
     * then dlerror() is supposed to return non-null.  This is intended
     * to differentiate between valid symbols which are actually 0/NULL
     * and invalid symbols that do not exist.
     *
     * Some implementations do _not_ implement this detail, and dlerror()
     * still returns NULL after looking up an invalid symbol name.
     *
     * In practice, this is expected to be used for looking up functions
     * and as such all valid symbols should be non-NULL, so NULL is considered
     * an error even if the C library doesn't consider this an error.
     */
    if (dlError != NULL)
    {
        OS_DEBUG("Error: %s: %s\n", SymbolName, dlError);
    }
    else if (Function == NULL)
    {
        /* technically not an error per POSIX, but in practice should not happen */
        OS_DEBUG("Error: %s: dlsym() returned NULL\n", SymbolName);
    }
    else
    {
        status = OS_SUCCESS;
    }

    *SymbolAddress = (cpuaddr)Function;

    return status;
}

/*----------------------------------------------------------------
 *
 * Function: OS_GlobalSymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GlobalSymbolLookup_Impl(cpuaddr *SymbolAddress, const char *SymbolName)
{
    int32 status;

#ifdef OSAL_DLSYM_DEFAULT_HANDLE
    status = OS_GenericSymbolLookup_Impl(OSAL_DLSYM_DEFAULT_HANDLE, SymbolAddress, SymbolName);
#else
    status = OS_ERR_NOT_IMPLEMENTED;
#endif

    return status;

} /* end OS_SymbolLookup_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleSymbolLookup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleSymbolLookup_Impl(uint32 local_id, cpuaddr *SymbolAddress, const char *SymbolName)
{
    int32 status;

    status = OS_GenericSymbolLookup_Impl(OS_impl_module_table[local_id].dl_handle, SymbolAddress, SymbolName);

    return status;

} /* end OS_ModuleSymbolLookup_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *  POSIX DL does not provide
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl(const char *filename, uint32 SizeLimit)
{
    /*
     * Limiting strictly to POSIX-defined API means there is no defined
     * method to get iterate over the symbol table.
     *
     * Some C libraries do offer an extension to provide this function, so
     * if this becomes a requirement on those platforms, this function
     * might need to move.
     *
     * Currently this is not a widely used/needed feature so it will report
     * unimplemented here.
     */

    return (OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */
