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
 * \file     osapi-countsem.c
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
#include "os-shared-countsem.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_COUNT_SEMAPHORES) || (OS_MAX_COUNT_SEMAPHORES <= 0)
#error "osconfig.h must define OS_MAX_COUNT_SEMAPHORES to a valid value"
#endif

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_COUNT_SEMAPHORES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_COUNTSEM
};

OS_count_sem_internal_record_t OS_count_sem_table[LOCAL_NUM_OBJECTS];

/****************************************************************************************
                                  SEMAPHORE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemAPI_Init(void)
{
    memset(OS_count_sem_table, 0, sizeof(OS_count_sem_table));
    return OS_SUCCESS;
} /* end OS_CountSemAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    int32                           return_code;
    OS_object_token_t               token;
    OS_count_sem_internal_record_t *countsem;

    /* Check parameters */
    OS_CHECK_POINTER(sem_id);
    OS_CHECK_APINAME(sem_name);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, sem_name, &token);
    if (return_code == OS_SUCCESS)
    {
        countsem = OS_OBJECT_TABLE_GET(OS_count_sem_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, countsem, obj_name, sem_name);

        /* Now call the OS-specific implementation.  This reads info from the table. */
        return_code = OS_CountSemCreate_Impl(&token, sem_initial_value, options);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, sem_id);
    }

    return return_code;

} /* end OS_CountSemCreate */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemDelete(osal_id_t sem_id)
{
    OS_object_token_t token;
    int32             return_code;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CountSemDelete_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;

} /* end OS_CountSemDelete */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGive
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGive(osal_id_t sem_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CountSemGive_Impl(&token);
    }

    return return_code;

} /* end OS_CountSemGive */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTake
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake(osal_id_t sem_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CountSemTake_Impl(&token);
    }

    return return_code;
} /* end OS_CountSemTake */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTimedWait
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait(osal_id_t sem_id, uint32 msecs)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_CountSemTimedWait_Impl(&token, msecs);
    }

    return return_code;
} /* end OS_CountSemTimedWait */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    int32 return_code;

    /* Check parameters */
    OS_CHECK_POINTER(sem_id);
    OS_CHECK_POINTER(sem_name);

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, sem_name, sem_id);

    return return_code;
} /* end OS_CountSemGetIdByName */

/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetInfo(osal_id_t sem_id, OS_count_sem_prop_t *count_prop)
{
    OS_common_record_t *record;
    OS_object_token_t   token;
    int32               return_code;

    /* Check parameters */
    OS_CHECK_POINTER(count_prop);

    memset(count_prop, 0, sizeof(OS_count_sem_prop_t));

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sem_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_count_sem_table, token);

        strncpy(count_prop->name, record->name_entry, sizeof(count_prop->name) - 1);
        count_prop->creator = record->creator;

        return_code = OS_CountSemGetInfo_Impl(&token, count_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_CountSemGetInfo */
