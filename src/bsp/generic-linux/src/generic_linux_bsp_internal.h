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
 *   Header file for internal data to the LINUX BSP
 */

#ifndef GENERIC_LINUX_BSP_INTERNAL_H
#define GENERIC_LINUX_BSP_INTERNAL_H

#include "osapi-common.h"
#include "osapi-bsp.h"
#include "osapi-error.h"
#include "bsp-impl.h"

#include <pthread.h>

/*
** BSP types
*/
typedef struct
{
    bool            EnableTermControl; /**< Will be set "true" when invoked from a TTY device, false otherwise */
    pthread_mutex_t AccessMutex;
    int             AccessCancelState;
} OS_BSP_GenericLinuxGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_GenericLinuxGlobalData_t OS_BSP_GenericLinuxGlobal;

#endif /* GENERIC_LINUX_BSP_INTERNAL_H */
