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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-idmap header
 */

#include "os-shared-idmap.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Lock_Global_Impl()
 * ----------------------------------------------------
 */
void OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    UT_GenStub_AddParam(OS_Lock_Global_Impl, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_Lock_Global_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Unlock_Global_Impl()
 * ----------------------------------------------------
 */
void OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    UT_GenStub_AddParam(OS_Unlock_Global_Impl, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_Unlock_Global_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_WaitForStateChange_Impl()
 * ----------------------------------------------------
 */
void OS_WaitForStateChange_Impl(osal_objtype_t objtype, uint32 attempts)
{
    UT_GenStub_AddParam(OS_WaitForStateChange_Impl, osal_objtype_t, objtype);
    UT_GenStub_AddParam(OS_WaitForStateChange_Impl, uint32, attempts);

    UT_GenStub_Execute(OS_WaitForStateChange_Impl, Basic, NULL);
}
