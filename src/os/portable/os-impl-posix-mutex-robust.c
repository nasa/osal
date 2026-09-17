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
 *
 * Purpose: Robust mutex support using the POSIX.1-2008 API.
 * This is used when OSAL_CONFIG_POSIX_ROBUST_MUTEX is enabled.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <errno.h>
#include <pthread.h>

#include "os-impl-mutex.h"

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int OS_Posix_MutexAttrSetRobust(pthread_mutexattr_t *attr)
{
    return pthread_mutexattr_setrobust(attr, PTHREAD_MUTEX_ROBUST);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int OS_Posix_MutexRecoverOwnerDead(pthread_mutex_t *mutex, int lock_status)
{
    if (lock_status == EOWNERDEAD)
    {
        /*
        ** The lock is held by the caller now; it must be marked consistent
        ** before use or the next unlock leaves it permanently unusable
        */
        lock_status = pthread_mutex_consistent(mutex);
    }

    return lock_status;
}
