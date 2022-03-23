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
 * OSAL coverage stub basic data types
 */

#ifndef OCS_BLKIO_H
#define OCS_BLKIO_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* The module and blk_dev types are used in several headers */
typedef struct OCS_BLK_DEV
{
    int bd;
} OCS_BLK_DEV;
typedef OCS_BLK_DEV *OCS_BLK_DEV_ID;

#endif /* OCS_BLKIO_H */
