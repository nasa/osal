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
 * This file contains a module loader implementation for systems
 * that implement a POSIX-style dynamic module loader.  This includes
 * RTEMS even if built without its native POSIX API.
 *
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
 *   dlopen()
 *   dlclose()
 *   dlsym()
 *   dlerror()
 */
#include <string.h>
#include <stdlib.h>

#include "os-impl-loader.h"
#include "os-shared-module.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                    Module Loader API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl(const OS_object_token_t *token, const char *translated_path)
{
    int32                             status = OS_ERROR;
    int                               dl_mode;
    OS_impl_module_internal_record_t *impl;
    OS_module_internal_record_t *     module;

    impl   = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);
    module = OS_OBJECT_TABLE_GET(OS_module_table, *token);

    /*
     * RTLD_NOW should instruct dlopen() to resolve all the symbols in the
     * module immediately, as opposed to waiting until they are used.
     * The latter (lazy mode) is non-deterministic - a resolution error on
     * a rarely-used symbol could cause a random failure far in the future.
     */
    dl_mode = RTLD_NOW;

    if ((module->flags & OS_MODULE_FLAG_LOCAL_SYMBOLS) != 0)
    {
        /*
         * Do not add the symbols in this module to the global symbol table.
         * This mode helps prevent any unanticipated references into this
         * module, which can in turn prevent unloading via dlclose().
         */
        dl_mode |= RTLD_LOCAL;
    }
    else
    {
        /*
         * Default mode - add symbols to the global symbol table, so they
         * will be available to resolve symbols in future module loads.
         * However, any such references will prevent unloading of this
         * module via dlclose().
         */
        dl_mode |= RTLD_GLOBAL;
    }

    dlerror();
    impl->dl_handle = dlopen(translated_path, dl_mode);
    if (impl->dl_handle != NULL)
    {
        status = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error loading shared library: %s\n", dlerror());
    }

    return status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl(const OS_object_token_t *token)
{
    int32                             status = OS_ERROR;
    OS_impl_module_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);

    /*
    ** Attempt to close/unload the module
    */
    dlerror();
    if (dlclose(impl->dl_handle) == 0)
    {
        impl->dl_handle = NULL;
        status          = OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("Error unloading shared library: %s\n", dlerror());
    }

    return status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl(const OS_object_token_t *token, OS_module_prop_t *module_prop)
{
    /*
     * Limiting strictly to POSIX-defined API means there is no defined
     * method to get information about a module contents.
     *
     * The "dlinfo()" function might return some interesting information
     * but this is actually a non-posix extension / platform-defined API.
     *
     * This returns success - although there is no information to add here,
     * the parent/shared layer information is still valid.
     */
    return OS_SUCCESS;
}
