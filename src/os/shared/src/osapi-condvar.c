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
#include "os-shared-idmap.h"
#include "os-shared-condvar.h"

/*
 * Other OSAL public APIs used by this module
 */
#include "osapi-task.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_CONDVARS) || (OS_MAX_CONDVARS <= 0)
#error "osconfig.h must define OS_MAX_CONDVARS to a valid value"
#endif

OS_condvar_internal_record_t OS_condvar_table[OS_MAX_CONDVARS];

/****************************************************************************************
                                  CONDITION VARIABLE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarAPI_Init(void)
{
    memset(OS_condvar_table, 0, sizeof(OS_condvar_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarCreate(osal_id_t *var_id, const char *var_name, uint32 options)
{
    int32                         return_code;
    OS_object_token_t             token;
    OS_condvar_internal_record_t *condvar;

    /* Check parameters */
    OS_CHECK_POINTER(var_id);
    OS_CHECK_APINAME(var_name);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_CONDVAR, var_name, &token);
    if (return_code == OS_SUCCESS)
    {
        condvar = OS_OBJECT_TABLE_GET(OS_condvar_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, condvar, obj_name, var_name);

        /* Now call the OS-specific implementation.  This reads info from the table. */
        return_code = OS_CondVarCreate_Impl(&token, options);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, var_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarDelete(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarDelete_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarLock(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarLock_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarUnlock(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarUnlock_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarSignal(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarSignal_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarBroadcast(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarBroadcast_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarWait(osal_id_t var_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarWait_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarTimedWait(osal_id_t var_id, const OS_time_t *abs_wakeup_time)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check parameters */
    OS_CHECK_POINTER(abs_wakeup_time);

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CondVarTimedWait_Impl(&token, abs_wakeup_time);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarGetIdByName(osal_id_t *var_id, const char *var_name)
{
    int32 return_code;

    /* Check parameters */
    OS_CHECK_POINTER(var_id);
    OS_CHECK_POINTER(var_name);

    return_code = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_CONDVAR, var_name, var_id);

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CondVarGetInfo(osal_id_t var_id, OS_condvar_prop_t *condvar_prop)
{
    OS_common_record_t *record;
    int32               return_code;
    OS_object_token_t   token;

    /* Check parameters */
    OS_CHECK_POINTER(condvar_prop);

    memset(condvar_prop, 0, sizeof(OS_condvar_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_CONDVAR, var_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_condvar_table, token);

        strncpy(condvar_prop->name, record->name_entry, sizeof(condvar_prop->name) - 1);
        condvar_prop->creator = record->creator;

        return_code = OS_CondVarGetInfo_Impl(&token, condvar_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}
