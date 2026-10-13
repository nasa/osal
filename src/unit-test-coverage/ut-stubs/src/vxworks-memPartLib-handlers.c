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
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "OCS_memPartLib.h"
#include "utstubs.h"

#include <string.h>

/* instance of the global (OK here for now, may need to move to a separate unit) */
OCS_PART_ID OCS_memSysPartId;

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OCS_memPartInfoGet' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OCS_memPartInfoGet(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OCS_MEM_PART_STATS *ppartStats = UT_Hook_GetArgValueByName(Context, "ppartStats", OCS_MEM_PART_STATS *);

    memset(ppartStats, 0, sizeof(*ppartStats));
}
