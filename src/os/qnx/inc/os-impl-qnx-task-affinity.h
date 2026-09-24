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
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_QNX_TASK_AFFINITY_H
#define OS_IMPL_QNX_TASK_AFFINITY_H

#include <string.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>

#include "osconfig.h"
#include "common_types.h"
#include "osapi-printf.h"
#include "os-posix.h"

#define OS_QNX_MAX_CPUS 128

/**
 * @brief QNX cpuset affinity structure
 *
 * This is used to represent the cpuset/affinity required by the QNX
 * operating system (struct _thread_runmask) to set the runmask and inherit mask.
 *
 * Applications should not directly access fields within this structure,
 * as the pointer configuration may change based on the number of processors in
 * the running system.
 */
typedef struct
{
    int32  size; /**< The size of the QNX run/inherit mask */
    uint32 affinity_masks[((OS_QNX_MAX_CPUS + 31) / 32)
                          * 2]; /**< The QNX run and inherit masks storage (supports maximum number of processors) */
} OS_QNX_cpuset_t;

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initialize and zero the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetZero(OS_QNX_cpuset_t *cpuset)
{
    int32 MaskSize = RMSK_SIZE(_syspage_ptr->num_cpu);

    /* Initialize/zero cpuset structure */
    memset(cpuset, 0x0, sizeof(OS_QNX_cpuset_t));

    /* Set size of run mask and inherit mask */
    cpuset->size = MaskSize;

    /*  Verify that cpuset has enought storage for cpus in system */
    if ((sizeof(int32) + (sizeof(uint32) * MaskSize * 2)) > sizeof(OS_QNX_cpuset_t))
    {
        OS_printf(" Not enough storage for QNX cpuset run and inherit mask: %s:%i \n", __func__, __LINE__);
    }
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the corresponding cpu bit in the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetSetCore(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr     = &cpuset->affinity_masks[0];
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    RMSK_SET(cpu, RunMaskPtr);
    RMSK_SET(cpu, InheritMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear the corresponding cpu bit in the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetClearCore(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr     = &cpuset->affinity_masks[0];
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    RMSK_CLR(cpu, RunMaskPtr);
    RMSK_CLR(cpu, InheritMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Indicates if the corresponding cpu bit in the cpuset/affinity structure is set
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 * @return     uint32 Indicates if cpu number is set in cpuset/affinity structure
 */
static inline uint32 OS_QNX_CpusetIsSetCore(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr     = &cpuset->affinity_masks[0];
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    return (RMSK_ISSET(cpu, RunMaskPtr) & RMSK_ISSET(cpu, InheritMaskPtr));
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the corresponding cpu bit in the runmask of the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetSetCoreRunMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr = &cpuset->affinity_masks[0];

    RMSK_SET(cpu, RunMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear the corresponding cpu bit in the runmask of the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetClearCoreRunMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr = &cpuset->affinity_masks[0];

    RMSK_CLR(cpu, RunMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Indicates if the corresponding cpu bit in the runmask of the cpuset/affinity structure is set
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 * @return     uint32 Indicates if cpu number is set in cpuset/affinity structure
 */
static inline uint32 OS_QNX_CpusetIsSetCoreRunMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *RunMaskPtr;

    /* init affiniity mask pointers */
    RunMaskPtr = &cpuset->affinity_masks[0];

    return (RMSK_ISSET(cpu, RunMaskPtr));
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the corresponding cpu bit in the inherit mask of the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetSetCoreInheritMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    RMSK_SET(cpu, InheritMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear the corresponding cpu bit in the inherit mask of the cpuset/affinity structure
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 */
static inline void OS_QNX_CpusetClearCoreInheritMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    RMSK_CLR(cpu, InheritMaskPtr);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Indicates if the corresponding cpu bit in the inherit mask of the cpuset/affinity structure is set
 *
 * @param[in]  cpuset The cpuset/affinity structure
 * @param[in]  cpu    The cpu number to configure in cpuset/affinity structure
 *
 * @return     uint32 Indicates if cpu number is set in cpuset/affinity structure
 */
static inline uint32 OS_QNX_CpusetIsSetCoreInheritMask(OS_QNX_cpuset_t *cpuset, int cpu)
{
    uint32 *InheritMaskPtr;

    /* init affiniity mask pointers */
    InheritMaskPtr = &cpuset->affinity_masks[cpuset->size];

    return (RMSK_ISSET(cpu, InheritMaskPtr));
}

/* Define the function used by the OSAL initialization */
static inline uint32 OS_TaskAffinity_Proc_Conf(void)
{
    return (uint32)(_syspage_ptr->num_cpu);
}

#endif /* OS_IMPL_QNX_TASK_AFFINITY_H */
