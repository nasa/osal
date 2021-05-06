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
 * \file     vxworks-os-impl-common-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-common.h"
#include "OCS_semLib.h"

UT_DEFAULT_STUB(OS_API_Impl_Init, (osal_objtype_t idtype))

int OS_VxWorks_GenericSemTake(OCS_SEM_ID vxid, int sys_ticks)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemTake);
}

int OS_VxWorks_GenericSemGive(OCS_SEM_ID vxid)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemGive);
}
