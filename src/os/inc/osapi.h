/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, Core OS module
 */

#ifndef OSAPI_H
#define OSAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Note - the "osapi-os-filesys.h" file previously included these system headers
 * plus a couple others.  Some existing code used stdio/stdlib functions but did
 * not #include them, relying on the implicit inclusion that osapi-os-filesys
 * provided.
 *
 * osapi-os-filesys.h now does a more complete abstraction and does not base its
 * API on the system API anymore.  However, in order to not break other code that
 * relied on the implicit inclusion of these system files, these are added here.
 *
 * (The reasoning is that stdio/stdlib are widely used and well standardized
 * across all C implementations, even embedded/lightweight C libraries should
 * have some usable version of these includes)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common_types.h"

#include "osapi-version.h"

/*
** Include the configuration file
*/
#include "osconfig.h"

/*
** Include the OS API modules
*/
#include "osapi-binsem.h"
#include "osapi-clock.h"
#include "osapi-common.h"
#include "osapi-constants.h"
#include "osapi-countsem.h"
#include "osapi-dir.h"
#include "osapi-error.h"
#include "osapi-file.h"
#include "osapi-filesys.h"
#include "osapi-heap.h"
#include "osapi-macros.h"
#include "osapi-idmap.h"
#include "osapi-module.h"
#include "osapi-mutex.h"
#include "osapi-network.h"
#include "osapi-printf.h"
#include "osapi-queue.h"
#include "osapi-select.h"
#include "osapi-shell.h"
#include "osapi-sockets.h"
#include "osapi-task.h"
#include "osapi-timebase.h"
#include "osapi-timer.h"

    /*
     ******************************************************************************
     * Items below here are internal OSAL-use definitions and are not part of the
     * OSAL API
     *****************************************************************************
     */

#include "osapi-bsp.h"

#ifdef __cplusplus
}
#endif

#endif /* OSAPI_H */
