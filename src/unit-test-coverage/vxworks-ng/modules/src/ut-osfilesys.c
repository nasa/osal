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

/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"


#include "ut-osfilesys.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osfilesys.c"

/* For proper linkage this variable must be instantiated here */
OS_filesys_internal_record_t        OS_filesys_table[OS_MAX_FILE_SYSTEMS];


OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };


void Osapi_Internal_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts)
{
    OS_impl_filesys_table[id].blkDev = blkdev;
    OS_impl_filesys_table[id].xbd = xbddev;
    OS_impl_filesys_table[id].xbdMaxPartitions = MaxParts;
}

