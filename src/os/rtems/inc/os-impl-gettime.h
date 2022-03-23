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
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_GETTIME_H
#define OS_IMPL_GETTIME_H

#include "osconfig.h"
#include <time.h>

/**
 * \brief Identifies the clock ID for OSAL clock operations on RTEMS
 *
 * This is the POSIX clock ID that will be used to implement
 * OS_GetLocalTime() and OS_SetLocalTime().
 */
#define OSAL_GETTIME_SOURCE_CLOCK CLOCK_REALTIME

#endif /* OS_IMPL_GETTIME_H */
