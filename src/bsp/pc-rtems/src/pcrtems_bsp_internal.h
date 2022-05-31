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
 *
 * Purpose:
 *   Header file for internal data to the PC-RTEMS BSP
 */

#ifndef PCRTEMS_BSP_INTERNAL_H
#define PCRTEMS_BSP_INTERNAL_H

/*
** OSAL includes
*/
#include "bsp-impl.h"

#include <rtems.h>

/*
 * BSP compile-time tuning
 */
#define RTEMS_MAX_USER_OPTIONS 4
#define RTEMS_MAX_CMDLINE      256

/*
 * Handle the differences between RTEMS 5 and 4.11 copyright notice
 */
#ifdef OS_RTEMS_4_DEPRECATED
#define OSAL_BSP_COPYRIGHT_NOTICE _Copyright_Notice
#else
#define OSAL_BSP_COPYRIGHT_NOTICE rtems_get_copyright_notice()
#endif

/*
 * The location which the general purpose file system will be mounted
 */
#define RTEMS_USER_FS_MOUNTPOINT "/mnt"

/*
 * By default put the shell at the same priority
 * as the utility task which handles OS_printf()
 */
#define RTEMS_SHELL_PRIORITY OS_UTILITYTASK_PRIORITY

/*
** BSP types
*/
typedef struct
{
    char     UserArgBuffer[RTEMS_MAX_CMDLINE];
    bool     BatchMode;
    rtems_id AccessMutex;
} OS_BSP_PcRtemsGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

#endif /* PCRTEMS_BSP_INTERNAL_H */
