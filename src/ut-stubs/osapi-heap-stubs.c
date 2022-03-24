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
 * Auto-Generated stub implementations for functions defined in osapi-heap header
 */

#include "osapi-heap.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_HeapGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_HeapGetInfo()
 * ----------------------------------------------------
 */
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_HeapGetInfo, int32);

    UT_GenStub_AddParam(OS_HeapGetInfo, OS_heap_prop_t *, heap_prop);

    UT_GenStub_Execute(OS_HeapGetInfo, Basic, UT_DefaultHandler_OS_HeapGetInfo);

    return UT_GenStub_GetReturnValue(OS_HeapGetInfo, int32);
}
