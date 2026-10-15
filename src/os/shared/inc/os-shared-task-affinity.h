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
#ifndef OS_SHARED_TASK_AFFINITY_H
#define OS_SHARED_TASK_AFFINITY_H

/*
**  System Include Files
*/
#include <stdlib.h>

#include "osapi-task-affinity.h"
/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetCoresConfigured() is an api call  to obtain information from OS
 * for the number of configured cores
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityGetCoresConfigured_Impl(void);

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinitySetAffinity() is an api call to set affinity to a task
 *
 * Sets an affinity from cpuset to task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinitySetAffinity_Impl(const OS_object_token_t *token, const OS_cpuset_t cpuset);

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetAffinity() is an api call to get affinity to a task
 *
 * Writes affinity to cpuset from task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinityGetAffinity_Impl(const OS_object_token_t *token, OS_cpuset_t *cpuset);

#endif /* OS_SHARED_TASK_AFFINITY_H */
