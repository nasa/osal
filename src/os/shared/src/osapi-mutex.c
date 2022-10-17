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
#include "os-shared-mutex.h"

/*
 * Other OSAL public APIs used by this module
 */
#include "osapi-task.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_MUTEXES) || (OS_MAX_MUTEXES <= 0)
#error "osconfig.h must define OS_MAX_MUTEXES to a valid value"
#endif

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_MUTEXES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_MUTEX
};

OS_mutex_internal_record_t OS_mutex_table[LOCAL_NUM_OBJECTS];

/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_MutexAPI_Init(void)
{
    memset(OS_mutex_table, 0, sizeof(OS_mutex_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 options)
{
    int32                       return_code;
    OS_object_token_t           token;
    OS_mutex_internal_record_t *mutex;

    /* Check parameters */
    OS_CHECK_POINTER(sem_id);
    OS_CHECK_APINAME(sem_name);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, sem_name, &token);
    if (return_code == OS_SUCCESS)
    {
        mutex = OS_OBJECT_TABLE_GET(OS_mutex_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, mutex, obj_name, sem_name);

        /* Now call the OS-specific implementation.  This reads info from the table. */
        return_code = OS_MutSemCreate_Impl(&token, options);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, sem_id);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemDelete(osal_id_t sem_id)
{
    OS_object_token_t token;
    int32             return_code;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_MutSemDelete_Impl(&token);

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
int32 OS_MutSemGive(osal_id_t sem_id)
{
    OS_mutex_internal_record_t *mutex;
    OS_object_token_t           token;
    int32                       return_code;
    osal_id_t                   self_task;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        mutex = OS_OBJECT_TABLE_GET(OS_mutex_table, token);

        self_task = OS_TaskGetId();

        if (!OS_ObjectIdEqual(mutex->last_owner, self_task))
        {
            OS_DEBUG("WARNING: Task %lu giving mutex %lu while owned by task %lu\n", OS_ObjectIdToInteger(self_task),
                     OS_ObjectIdToInteger(sem_id), OS_ObjectIdToInteger(mutex->last_owner));
        }

        mutex->last_owner = OS_OBJECT_ID_UNDEFINED;

        return_code = OS_MutSemGive_Impl(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemTake(osal_id_t sem_id)
{
    OS_mutex_internal_record_t *mutex;
    OS_object_token_t           token;
    int32                       return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        mutex = OS_OBJECT_TABLE_GET(OS_mutex_table, token);

        return_code = OS_MutSemTake_Impl(&token);
        if (return_code == OS_SUCCESS)
        {
            /* Always set the owner if OS_MutSemTake_Impl() returned success */
            mutex->last_owner = OS_TaskGetId();
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
int32 OS_MutSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    int32 return_code;

    /* Check parameters */
    OS_CHECK_POINTER(sem_id);
    OS_CHECK_POINTER(sem_name);

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, sem_name, sem_id);

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemGetInfo(osal_id_t sem_id, OS_mut_sem_prop_t *mut_prop)
{
    OS_common_record_t *record;
    int32               return_code;
    OS_object_token_t   token;

    /* Check parameters */
    OS_CHECK_POINTER(mut_prop);

    memset(mut_prop, 0, sizeof(OS_mut_sem_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_mutex_table, token);

        strncpy(mut_prop->name, record->name_entry, sizeof(mut_prop->name) - 1);
        mut_prop->creator = record->creator;

        return_code = OS_MutSemGetInfo_Impl(&token, mut_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}
