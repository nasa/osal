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
 * Declarations and prototypes for file objects
 */

#ifndef OSAPI_TASK_AFFINITY_H
#define OSAPI_TASK_AFFINITY_H

#include "common_types.h"
#include "osconfig.h"

/**
 * @brief An abstract structure capable of holding a cpuset
 * Because standard integer types max out at 64 bits
 * OSAL uses an array of 8-bit bytes to store the mask so it can scale
 * to support any number of cores. Currently the max is 64 but this can
 * be used for more in the future.
 * if the number of bits doesn't divide perfectly by 8,
 * add one extra byte to the end to hold the remainders
 */
typedef struct
{
    uint8 affinity_mask[(OS_MAX_CPUS + 7) / 8];
} OS_cpuset_t;

/**
 * @name OSAL CPU Set Manipulation Macros
 * @{
 */

/** @brief Initializes the CPU set to empty (clears all CPUs) */
#define OS_CPUSET_ZERO(cpusetptr) memset((cpusetptr)->affinity_mask, 0, sizeof((cpusetptr)->affinity_mask))

/** @brief Adds a specific CPU to the CPU set */
#define OS_CPUSET_SET(cpu, cpusetptr)                                     \
    do                                                                    \
    {                                                                     \
        if ((cpu) < OS_MAX_CPUS)                                          \
        {                                                                 \
            (cpusetptr)->affinity_mask[(cpu) / 8] |= (1U << ((cpu) % 8)); \
        }                                                                 \
    } while (0)

/** @brief Removes a specific CPU from the CPU set */
#define OS_CPUSET_CLR(cpu, cpusetptr)                                      \
    do                                                                     \
    {                                                                      \
        if ((cpu) < OS_MAX_CPUS)                                           \
        {                                                                  \
            (cpusetptr)->affinity_mask[(cpu) / 8] &= ~(1U << ((cpu) % 8)); \
        }                                                                  \
    } while (0)

/** @brief Checks if a specific CPU is in the CPU set (Evaluates to true/false) */
#define OS_CPUSET_ISSET(cpu, cpusetptr) \
    (((cpu) < OS_MAX_CPUS) ? (((cpusetptr)->affinity_mask[(cpu) / 8] & (1U << ((cpu) % 8))) != 0) : 0)

/** @} */
/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetCoresConfigured() is an api call  to obtain information from OS
 * for the number of configured cores
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityGetCoresConfigured(void);

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityCoresConfigured() is an api call  to return global information
 * from OSAL set at init for the number of configured cores.
 *
 * Returns the number of configured cores
 * ----------------------------------------------------------------------
 */
uint32 OS_TaskAffinityCoresConfigured(void);

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinitySetAffinity() is an api call to set affinity to a task
 *
 * Sets an affinity from cpuset to task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinitySetAffinity(osal_id_t task_id, const OS_cpuset_t cpuset);

/*
 * ----------------------------------------------------------------------
 * The OS_TaskAffinityGetAffinity() is an api call to get affinity to a task
 *
 * Writes affinity to cpuset from task with provided task_id
 * ----------------------------------------------------------------------
 */
int32 OS_TaskAffinityGetAffinity(osal_id_t task_id, OS_cpuset_t *cpuset);

#endif /* OSAPI_TASK_AFFINITY_H */
