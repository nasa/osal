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
 *
 * This file Contains all of the api calls for manipulating the task affinity
 * on the QNX operating system.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 */

#include "os-impl-qnx-task-affinity.h"
#include "os-impl-tasks.h"
#include "os-shared-idmap.h"
#include "osapi-task-affinity.h"

/*
**  System Include Files
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <process.h>
#include <errno.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>
#include <unistd.h>

int pthread_setname_np(pthread_t thread, const char *name);

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                 Named File API
 ***************************************************************************************/
/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetCoresConfigured() is an api call  to obtain information from OS
 * for the number of configured cores
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityGetCoresConfigured_Impl(void)
{
    return OS_TaskAffinity_Proc_Conf();
}

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinitySetAffinity() is an api call to set affinity to a task
 *
 * Sets an affinity from cpuset to task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinitySetAffinity_Impl(const OS_object_token_t *token, const OS_cpuset_t cpuset)
{
    OS_QNX_cpuset_t                 lcl_cpuset;
    OS_impl_task_internal_record_t *impl;
    int                             return_value;
    uint32                          i;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /* Intitialize and zero cpuset */
    OS_QNX_CpusetZero(&lcl_cpuset);

    /* Populate POSIX cpuset from OSAL CPU Set using the macro */
    for (i = 0; (i < OS_MAX_CPUS) && (i < OS_TaskAffinityGetCoresConfigured()); i++)
    {
        /* Check if the bit is set in the OSAL structure */
        if (OS_CPUSET_ISSET(i, &cpuset))
        {
            /* If it is, set the corresponding bit in the POSIX structure */
            OS_QNX_CpusetSetCore(&lcl_cpuset, i);
        }
    }

    /* Set the runmask and get inherit mask */
    return_value = ThreadCtlExt(0, (int)impl->id, _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT, (void *)&lcl_cpuset);

    if (return_value != -1)
    {
        return OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("ThreadCtlExt: _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT Failed [(%i) %s ]\n", errno, strerror(errno));
        return OS_ERROR;
    }
}

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetAffinity() is an api call to get affinity to a task
 *
 * Writes affinity to cpuset from task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinityGetAffinity_Impl(const OS_object_token_t *token, OS_cpuset_t *cpuset)
{
    OS_QNX_cpuset_t                 local_cpuset;
    OS_impl_task_internal_record_t *impl;
    int                             return_value;
    uint32                          i;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /* Intitialize and zero cpuset */
    OS_QNX_CpusetZero(&local_cpuset);

    /* Get the runmask and inherit mask when passing cpuset set to zero */
    return_value = ThreadCtlExt(0, (int)impl->id, _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT, (void *)&local_cpuset);

    if (return_value != -1)
    {
        /* Clear the OSAL affinity mask using the macro */
        OS_CPUSET_ZERO(cpuset);

        /* Populate the OSAL affinity mask from the POSIX mask */
        for (i = 0; (i < OS_MAX_CPUS) && (i < OS_TaskAffinityGetCoresConfigured()); i++)
        {
            /* If the bit is set in the POSIX structure */
            if (OS_QNX_CpusetIsSetCore(&local_cpuset, i))
            {
                /* set the corresponding bit in the OSAL structure */
                OS_CPUSET_SET(i, cpuset);
            }
        }

        return OS_SUCCESS;
    }
    else
    {
        OS_DEBUG("ThreadCtlExt: _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT Failed [(%i) %s ]\n", errno, strerror(errno));
    }

    return OS_ERROR;
}
