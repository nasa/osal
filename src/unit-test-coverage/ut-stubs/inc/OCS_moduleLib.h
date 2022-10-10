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
 * OSAL coverage stub replacement for moduleLib.h
 */

#ifndef OCS_MODULELIB_H
#define OCS_MODULELIB_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* ----------------------------------------- */
/* constants normally defined in moduleLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in moduleLib.h */
/* ----------------------------------------- */
typedef struct OCS_MODULE
{
    int m;
} OCS_MODULE;
typedef OCS_MODULE *OCS_MODULE_ID;

typedef struct OCS_MODULE_INFO
{
    struct
    {
        unsigned long textAddr;
        unsigned long textSize;
        unsigned long dataAddr;
        unsigned long dataSize;
        unsigned long bssAddr;
        unsigned long bssSize;
    } segInfo;
} OCS_MODULE_INFO;

/* ----------------------------------------- */
/* prototypes normally declared in moduleLib.h */
/* ----------------------------------------- */

extern OCS_STATUS OCS_moduleInfoGet(OCS_MODULE_ID moduleId, OCS_MODULE_INFO *pModuleInfo);

#endif /* OCS_MODULELIB_H */
