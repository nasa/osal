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
 * Auto-Generated stub implementations for functions defined in os-shared-timebase header
 */

#include "os-shared-timebase.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Milli2Ticks()
 * ----------------------------------------------------
 */
int32 OS_Milli2Ticks(uint32 milli_seconds, int *ticks)
{
    UT_GenStub_SetupReturnBuffer(OS_Milli2Ticks, int32);

    UT_GenStub_AddParam(OS_Milli2Ticks, uint32, milli_seconds);
    UT_GenStub_AddParam(OS_Milli2Ticks, int *, ticks);

    UT_GenStub_Execute(OS_Milli2Ticks, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_Milli2Ticks, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBase_CallbackThread()
 * ----------------------------------------------------
 */
void OS_TimeBase_CallbackThread(osal_id_t timebase_id)
{
    UT_GenStub_AddParam(OS_TimeBase_CallbackThread, osal_id_t, timebase_id);

    UT_GenStub_Execute(OS_TimeBase_CallbackThread, Basic, NULL);
}
