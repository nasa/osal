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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include "os-rtems.h"
#include "os-impl-loader.h"
#include "os-shared-module.h"
#include "os-shared-idmap.h"
#include <rtems/rtl/rtl.h>

/****************************************************************************************
                                   TYPEDEFS
 ***************************************************************************************/

#ifdef OS_RTEMS_4_DEPRECATED

typedef rtems_rtl_obj_t rtems_rtl_obj; /* Alias for RTEMS 4.11 */

#endif

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_ModuleAPI_Impl_Init(void)
{
    memset(OS_impl_module_table, 0, sizeof(OS_impl_module_table));
    return OS_SUCCESS;
}

/****************************************************************************************
                                HELPER ROUTINES
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.

 * This callback checks symbols in the RTL unresolved record list
 * NOTE: Right now this considers any unresolved items to be a failure.
 * This could be fine-tuned later.
 *
 *-----------------------------------------------------------------*/
static bool OS_rtems_rtl_check_unresolved(OSAL_UNRESOLV_REC_TYPE *rec, void *data)
{
    int32 *status = data;

    switch (rec->type)
    {
        case OSAL_UNRESOLVED_SYMBOL:
            OS_DEBUG("unresolved symbol: %s\n", rec->rec.name.name);
            *status = OS_ERROR;
            break;
        case rtems_rtl_unresolved_reloc:
            *status = OS_ERROR;
            break;
        default:
            break;
    }
    return false;
}

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
    int32                             status;
    int                               unresolved;
    void *                            dl_handle;
    OS_impl_module_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);

    dlerror();
    dl_handle = dlopen(translated_path, RTLD_NOW | RTLD_GLOBAL);
    if (dl_handle == NULL)
    {
        OS_DEBUG("Error loading shared library: %s\n", dlerror());
        status = OS_ERROR;
    }
    else if (dlinfo(dl_handle, RTLD_DI_UNRESOLVED, &unresolved) < 0)
    {
        /* should never happen */
        OS_DEBUG("dlinfo error checking unresolved status\n");
        status = OS_ERROR;
    }
    else if (unresolved)
    {
        /*
         * Note that RTEMS is a little different than traditional POSIX
         * in that even with RTLD_NOW specified, dlopen() will still return
         * success if there are unresolved symbols.  RTEMS has implemented
         * it this way to allow cross/circular dependencies.
         *
         * However, OSAL applications are not anticipated to have circular
         * dependencies such as this.  So this explicitly checks if unresolved
         * symbols are present after loading, and if so, do additional checks.
         *
         * It is possible that not every unresolved situation indicates failure.
         * The "check_unresolved" helper should verify if the condition
         * is acceptable.  If not acceptable, it sets the status back to an error.
         */

        OS_DEBUG("Module has unresolved externals\n");
        status = OS_SUCCESS; /* note - not final, probably overridden */
        OSAL_UNRESOLVED_ITERATE(OS_rtems_rtl_check_unresolved, &status);
    }
    else
    {
        status = OS_SUCCESS;
    }

    if (status == OS_SUCCESS)
    {
        /* success: save for future use */
        impl->dl_handle = dl_handle;
    }
    else if (dl_handle != NULL)
    {
        /*
         * When returning non-success based on one of the
         * subsequent verifications, also dlclose() the handle
         * to avoid leaking resources.
         */
        dlclose(dl_handle);
        dl_handle = NULL;
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
    rtems_rtl_obj *                   obj;
    OS_impl_module_internal_record_t *impl;
    int32                             status = OS_ERROR;

    impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);

    /* Lock RTEMS runtime loader */
    if (rtems_rtl_lock() != NULL)
    {
        /* Get RTL object from handle and populate section info */
        obj = rtems_rtl_check_handle(impl->dl_handle);

        if (obj != NULL)
        {
            module_prop->addr.valid        = true;
            module_prop->addr.code_address = (cpuaddr)obj->text_base;
            module_prop->addr.code_size    = (cpuaddr)rtems_rtl_obj_text_size(obj);
            module_prop->addr.data_address = (cpuaddr)obj->data_base;
            module_prop->addr.data_size    = (cpuaddr)rtems_rtl_obj_data_size(obj);
            module_prop->addr.bss_address  = (cpuaddr)obj->bss_base;
            module_prop->addr.bss_size     = (cpuaddr)rtems_rtl_obj_bss_size(obj);

            status = OS_SUCCESS;
        }

        /* Unlock RTEMS runtime loader, report error if applicable */
        rtems_rtl_unlock();

        if (obj == NULL)
        {
            OS_DEBUG("Error getting object information from handle\n");
            module_prop->addr.valid = false;
        }
    }
    else
    {
        OS_DEBUG("Error locking RTEMS runtime loader\n");
        module_prop->addr.valid = false;
    }

    return status;
}
