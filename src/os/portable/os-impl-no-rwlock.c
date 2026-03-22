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
 * Purpose: All functions return OS_ERR_NOT_IMPLEMENTED.
 * This is used when rwlock functionality is disabled by config.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <osapi.h>
#include "os-shared-rwlock.h"

int32 OS_RwLockCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockReadGive_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockWriteGive_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockReadTake_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockWriteTake_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_RwLockGetInfo_Impl(const OS_object_token_t *token, OS_rwlock_prop_t *rw_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
