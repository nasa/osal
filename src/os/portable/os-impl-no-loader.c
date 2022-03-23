/**
 * \file
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains a module loader implementation for systems
 * that do not use dynamic modules. It returns OS_ERR_NOT_IMPLEMENTED
 * for all calls.
 */

#include "os-shared-module.h"

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleLoad_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleLoad_Impl(const OS_object_token_t *token, const char *translated_path)
{
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_ModuleLoad_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleUnload_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleUnload_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_ModuleUnload_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl(const OS_object_token_t *token, OS_module_prop_t *module_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_ModuleGetInfo_Impl */
