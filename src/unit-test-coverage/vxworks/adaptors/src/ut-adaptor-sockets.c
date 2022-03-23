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
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-sockets.h"

#include "os-vxworks.h"
#include "os-shared-idmap.h"
#include "os-impl-sockets.h"

/*
 * A UT-specific wrapper function to invoke the VxWorks "SetFlag" helper
 */
void UT_SocketTest_CallVxWorksSetFlags_Impl(uint32 index)
{
    OS_object_token_t token = {.obj_idx = index};

    OS_VxWorks_SetSocketFlags_Impl(&token);
}
