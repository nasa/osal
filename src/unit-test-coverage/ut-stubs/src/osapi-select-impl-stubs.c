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
 * \file     osapi-select-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-select.h"

int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, int32 msecs)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectSingle_Impl), token);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectSingle_Impl), SelectFlags);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectSingle_Impl), msecs);

    return UT_DEFAULT_IMPL(OS_SelectSingle_Impl);
}

int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectMultiple_Impl), ReadSet);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectMultiple_Impl), WriteSet);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_SelectMultiple_Impl), msecs);

    return UT_DEFAULT_IMPL(OS_SelectMultiple_Impl);
}
