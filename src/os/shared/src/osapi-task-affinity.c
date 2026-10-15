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

#include "common_types.h"

#include "osapi.h"
#include "os-shared-globaldefs.h"

#include "osapi-task-affinity.h"
#include "os-shared-task-affinity.h"

/*
 * User defined include files
 */
#include "os-shared-task.h"
#include "os-shared-common.h"
#include "os-shared-idmap.h"

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetCoresConfigured() is an api call  to obtain information from OS
 * for the number of configured cores
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityGetCoresConfigured(void)
{
    return OS_TaskAffinityGetCoresConfigured_Impl();
}

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityCoresConfigured() is an api call  to return global information
 * from OSAL set at init for the number of configured cores.
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityCoresConfigured(void)
{
    return OS_SharedGlobalVars.CoresConfigured;
}

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinitySetAffinity() is an api call to set affinity to a task
 *
 * Sets an affinity from cpuset to task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinitySetAffinity(osal_id_t task_id, const OS_cpuset_t cpuset)
{
    int32             OsStatus;
    OS_object_token_t token;

    OsStatus = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TASK, task_id, &token);
    if (OsStatus == OS_SUCCESS)
    {
        OsStatus = OS_TaskAffinitySetAffinity_Impl(&token, cpuset);
    }

    OS_ObjectIdRelease(&token);

    return OsStatus;
}

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetAffinity() is an api call to get affinity to a task
 *
 * Writes affinity to cpuset from task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinityGetAffinity(osal_id_t task_id, OS_cpuset_t *cpuset)
{
    int32             OsStatus;
    OS_object_token_t token;

    OsStatus = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_OBJECT_TYPE_OS_TASK, task_id, &token);
    if (OsStatus == OS_SUCCESS)
    {
        OsStatus = OS_TaskAffinityGetAffinity_Impl(&token, cpuset);
    }

    OS_ObjectIdRelease(&token);

    return OsStatus;
}
