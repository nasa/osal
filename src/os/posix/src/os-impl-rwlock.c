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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-shared-rwlock.h"
#include "os-shared-idmap.h"
#include "os-impl-rwlock.h"

/* Tables where the OS object information is stored */
OS_impl_rwlock_internal_record_t OS_impl_rwlock_table[OS_MAX_RWLOCKS];

/****************************************************************************************
                                  RWLOCK API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Posix_RwLockAPI_Impl_Init(void)
{
    memset(OS_impl_rwlock_table, 0, sizeof(OS_impl_rwlock_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    int                               return_code;
    pthread_rwlockattr_t              rwlock_attr;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    /*
    ** initialize the attribute with default values
    */
    return_code = pthread_rwlockattr_init(&rwlock_attr);
    if (return_code != 0)
    {
        OS_DEBUG("Error: RwLock could not be created. pthread_rwlockattr_init failed ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), strerror(return_code));
        return OS_ERROR;
    }

    /*
    ** create the rwlock
    ** upon successful initialization, the state of the rwlock becomes initialized and unlocked
    */
    return_code = pthread_rwlock_init(&impl->id, &rwlock_attr);
    if (return_code != 0)
    {
        OS_DEBUG("Error: RwLock could not be created. ID = %lu: %s\n",
                 OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), strerror(return_code));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockDelete_Impl(const OS_object_token_t *token)
{
    int                               status;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    status = pthread_rwlock_destroy(&(impl->id)); /* 0 = success */

    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockReadGive_Impl(const OS_object_token_t *token)
{
    int                               status;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    /*
     ** Unlock the mutex
     */
    status = pthread_rwlock_unlock(&(impl->id));
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockWriteGive_Impl(const OS_object_token_t *token)
{
    int                               status;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    /*
     ** Unlock the mutex
     */
    status = pthread_rwlock_unlock(&(impl->id));
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockReadTake_Impl(const OS_object_token_t *token)
{
    int                               status;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    /*
    ** Lock the mutex
    */
    status = pthread_rwlock_rdlock(&(impl->id));
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockWriteTake_Impl(const OS_object_token_t *token)
{
    int                               status;
    OS_impl_rwlock_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_rwlock_table, *token);

    /*
    ** Lock the mutex
    */
    status = pthread_rwlock_wrlock(&(impl->id));
    if (status != 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_RwLockGetInfo_Impl(const OS_object_token_t *token, OS_rwlock_prop_t *rw_prop)
{
    return OS_SUCCESS;
}
