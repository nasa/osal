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
#include "ut-adaptor-portable-posix-io.h"

#include "os-impl-io.h"

void UT_PortablePosixIOTest_ResetImpl(osal_index_t local_id)
{
    OS_impl_filehandle_table[local_id].fd         = -1;
    OS_impl_filehandle_table[local_id].selectable = false;
}

void UT_PortablePosixIOTest_Set_Selectable(osal_index_t local_id, bool is_selectable)
{
    OS_impl_filehandle_table[local_id].selectable = is_selectable;
}

void UT_PortablePosixIOTest_Set_FD(osal_index_t local_id, int fd)
{
    OS_impl_filehandle_table[local_id].fd = fd;
}

bool UT_PortablePosixIOTest_Get_Selectable(osal_index_t local_id)
{
    return OS_impl_filehandle_table[local_id].selectable;
}
