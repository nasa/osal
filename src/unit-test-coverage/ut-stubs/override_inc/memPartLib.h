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
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for memPartLib.h
 */

#ifndef OVERRIDE_MEMPARTLIB_H
#define OVERRIDE_MEMPARTLIB_H

#include "OCS_memPartLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in memPartLib.h */
/* ----------------------------------------- */
#define PART_ID        OCS_PART_ID
#define MEM_PART_STATS OCS_MEM_PART_STATS

#define memAddToPool        OCS_memAddToPool
#define memPartAddToPool    OCS_memPartAddToPool
#define memPartAlignedAlloc OCS_memPartAlignedAlloc
#define memPartAlloc        OCS_memPartAlloc
#define memPartCreate       OCS_memPartCreate
#define memPartFree         OCS_memPartFree
#define memPartInfoGet      OCS_memPartInfoGet
#define memPartShow         OCS_memPartShow

#define memSysPartId OCS_memSysPartId

#endif /* OVERRIDE_MEMPARTLIB_H */
