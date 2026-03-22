/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
#include "os-shared-rwlock.h"

/*
 * Other OSAL public APIs used by this module
 */
#include "osapi-task.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_RWLOCKS) || (OS_MAX_RWLOCKS <= 0)
#error "osconfig.h must define OS_MAX_RWLOCKS to a valid value"
#endif

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_RWLOCKS,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_RWLOCK
};

OS_rwlock_internal_record_t OS_rwlock_table[LOCAL_NUM_OBJECTS];

/****************************************************************************************
                                  RWLOCK API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockAPI_Init(void)
{
    memset(OS_rwlock_table, 0, sizeof(OS_rwlock_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockCreate(osal_id_t *rw_id, const char *rw_name, uint32 options)
{
    int32                        return_code;
    OS_object_token_t            token;
    OS_rwlock_internal_record_t *rwlock;

    /* Check parameters */
    OS_CHECK_POINTER(rw_id);
    OS_CHECK_APINAME(rw_name);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, rw_name, &token);
    if (return_code == OS_SUCCESS)
    {
        rwlock = OS_OBJECT_TABLE_GET(OS_rwlock_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, rwlock, obj_name, rw_name);

        /* Now call the OS-specific implementation.  This reads info from the table. */
        return_code = OS_RwLockCreate_Impl(&token, options);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, rw_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockDelete(osal_id_t rw_id)
{
    OS_object_token_t token;
    int32             return_code;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_RwLockDelete_Impl(&token);

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
int32 OS_RwLockReadGive(osal_id_t rw_id)
{
    OS_rwlock_internal_record_t *rwlock;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        rwlock = OS_OBJECT_TABLE_GET(OS_rwlock_table, token);

        /* Ensure no write locks were held while this read lock was held */
        if (!OS_ObjectIdEqual(rwlock->last_writer, OS_OBJECT_ID_UNDEFINED))
        {
            OS_DEBUG("WARNING: Task %lu giving read lock on rwlock %lu while write lock held by task %lu\n",
                     OS_ObjectIdToInteger(OS_TaskGetId()), OS_ObjectIdToInteger(rw_id),
                     OS_ObjectIdToInteger(rwlock->last_writer));
        }

        return_code = OS_RwLockReadGive_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockWriteGive(osal_id_t rw_id)
{
    OS_rwlock_internal_record_t *rwlock;
    OS_object_token_t            token;
    int32                        return_code;
    osal_id_t                    self_task;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        rwlock = OS_OBJECT_TABLE_GET(OS_rwlock_table, token);

        self_task = OS_TaskGetId();

        /* Ensure multiple write locks weren't held */
        if (!OS_ObjectIdEqual(rwlock->last_writer, self_task))
        {
            OS_DEBUG("WARNING: Task %lu giving write lock on rwlock %lu while write lock held by task %lu\n",
                     OS_ObjectIdToInteger(self_task), OS_ObjectIdToInteger(rw_id),
                     OS_ObjectIdToInteger(rwlock->last_writer));
        }

        rwlock->last_writer = OS_OBJECT_ID_UNDEFINED;

        return_code = OS_RwLockWriteGive_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockReadTake(osal_id_t rw_id)
{
    OS_rwlock_internal_record_t *rwlock;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        rwlock = OS_OBJECT_TABLE_GET(OS_rwlock_table, token);

        return_code = OS_RwLockReadTake_Impl(&token);

        /* Ensure no write locks are being held */
        if (return_code == OS_SUCCESS && !OS_ObjectIdEqual(rwlock->last_writer, OS_OBJECT_ID_UNDEFINED))
        {
            OS_DEBUG("WARNING: Task %lu taking read lock on rwlock %lu while write lock held by task %lu\n",
                     OS_ObjectIdToInteger(OS_TaskGetId()), OS_ObjectIdToInteger(rw_id),
                     OS_ObjectIdToInteger(rwlock->last_writer));
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockWriteTake(osal_id_t rw_id)
{
    OS_rwlock_internal_record_t *rwlock;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        rwlock = OS_OBJECT_TABLE_GET(OS_rwlock_table, token);

        return_code = OS_RwLockWriteTake_Impl(&token);
        if (return_code == OS_SUCCESS)
        {
            rwlock->last_writer = OS_TaskGetId();
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockGetIdByName(osal_id_t *rw_id, const char *rw_name)
{
    int32 return_code;

    /* Check parameters */
    OS_CHECK_POINTER(rw_id);
    OS_CHECK_POINTER(rw_name);

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, rw_name, rw_id);

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockGetInfo(osal_id_t rw_id, OS_rwlock_prop_t *rw_prop)
{
    OS_common_record_t *record;
    int32               return_code;
    OS_object_token_t   token;

    /* Check parameters */
    OS_CHECK_POINTER(rw_prop);

    memset(rw_prop, 0, sizeof(OS_rwlock_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, rw_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_rwlock_table, token);

        strncpy(rw_prop->name, record->name_entry, sizeof(rw_prop->name) - 1);
        rw_prop->creator = record->creator;

        return_code = OS_RwLockGetInfo_Impl(&token, rw_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}
