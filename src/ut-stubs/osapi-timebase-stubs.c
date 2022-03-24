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
 * Auto-Generated stub implementations for functions defined in osapi-timebase header
 */

#include "osapi-timebase.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_TimeBaseCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimeBaseDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimeBaseGetFreeRun(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimeBaseGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimeBaseGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseCreate()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseCreate(osal_id_t *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseCreate, int32);

    UT_GenStub_AddParam(OS_TimeBaseCreate, osal_id_t *, timebase_id);
    UT_GenStub_AddParam(OS_TimeBaseCreate, const char *, timebase_name);
    UT_GenStub_AddParam(OS_TimeBaseCreate, OS_TimerSync_t, external_sync);

    UT_GenStub_Execute(OS_TimeBaseCreate, Basic, UT_DefaultHandler_OS_TimeBaseCreate);

    return UT_GenStub_GetReturnValue(OS_TimeBaseCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseDelete()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseDelete(osal_id_t timebase_id)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseDelete, int32);

    UT_GenStub_AddParam(OS_TimeBaseDelete, osal_id_t, timebase_id);

    UT_GenStub_Execute(OS_TimeBaseDelete, Basic, UT_DefaultHandler_OS_TimeBaseDelete);

    return UT_GenStub_GetReturnValue(OS_TimeBaseDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseGetFreeRun()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseGetFreeRun(osal_id_t timebase_id, uint32 *freerun_val)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseGetFreeRun, int32);

    UT_GenStub_AddParam(OS_TimeBaseGetFreeRun, osal_id_t, timebase_id);
    UT_GenStub_AddParam(OS_TimeBaseGetFreeRun, uint32 *, freerun_val);

    UT_GenStub_Execute(OS_TimeBaseGetFreeRun, Basic, UT_DefaultHandler_OS_TimeBaseGetFreeRun);

    return UT_GenStub_GetReturnValue(OS_TimeBaseGetFreeRun, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseGetIdByName(osal_id_t *timebase_id, const char *timebase_name)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseGetIdByName, int32);

    UT_GenStub_AddParam(OS_TimeBaseGetIdByName, osal_id_t *, timebase_id);
    UT_GenStub_AddParam(OS_TimeBaseGetIdByName, const char *, timebase_name);

    UT_GenStub_Execute(OS_TimeBaseGetIdByName, Basic, UT_DefaultHandler_OS_TimeBaseGetIdByName);

    return UT_GenStub_GetReturnValue(OS_TimeBaseGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseGetInfo()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseGetInfo(osal_id_t timebase_id, OS_timebase_prop_t *timebase_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseGetInfo, int32);

    UT_GenStub_AddParam(OS_TimeBaseGetInfo, osal_id_t, timebase_id);
    UT_GenStub_AddParam(OS_TimeBaseGetInfo, OS_timebase_prop_t *, timebase_prop);

    UT_GenStub_Execute(OS_TimeBaseGetInfo, Basic, UT_DefaultHandler_OS_TimeBaseGetInfo);

    return UT_GenStub_GetReturnValue(OS_TimeBaseGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseSet()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseSet(osal_id_t timebase_id, uint32 start_time, uint32 interval_time)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseSet, int32);

    UT_GenStub_AddParam(OS_TimeBaseSet, osal_id_t, timebase_id);
    UT_GenStub_AddParam(OS_TimeBaseSet, uint32, start_time);
    UT_GenStub_AddParam(OS_TimeBaseSet, uint32, interval_time);

    UT_GenStub_Execute(OS_TimeBaseSet, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseSet, int32);
}
