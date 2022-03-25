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
 * Auto-Generated stub implementations for functions defined in osapi-timer header
 */

#include "osapi-timer.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_TimerAdd(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimerCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimerDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimerGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimerGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerAdd()
 * ----------------------------------------------------
 */
int32 OS_TimerAdd(osal_id_t *timer_id, const char *timer_name, osal_id_t timebase_id, OS_ArgCallback_t callback_ptr,
                  void *callback_arg)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerAdd, int32);

    UT_GenStub_AddParam(OS_TimerAdd, osal_id_t *, timer_id);
    UT_GenStub_AddParam(OS_TimerAdd, const char *, timer_name);
    UT_GenStub_AddParam(OS_TimerAdd, osal_id_t, timebase_id);
    UT_GenStub_AddParam(OS_TimerAdd, OS_ArgCallback_t, callback_ptr);
    UT_GenStub_AddParam(OS_TimerAdd, void *, callback_arg);

    UT_GenStub_Execute(OS_TimerAdd, Basic, UT_DefaultHandler_OS_TimerAdd);

    return UT_GenStub_GetReturnValue(OS_TimerAdd, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerCreate()
 * ----------------------------------------------------
 */
int32 OS_TimerCreate(osal_id_t *timer_id, const char *timer_name, uint32 *clock_accuracy,
                     OS_TimerCallback_t callback_ptr)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerCreate, int32);

    UT_GenStub_AddParam(OS_TimerCreate, osal_id_t *, timer_id);
    UT_GenStub_AddParam(OS_TimerCreate, const char *, timer_name);
    UT_GenStub_AddParam(OS_TimerCreate, uint32 *, clock_accuracy);
    UT_GenStub_AddParam(OS_TimerCreate, OS_TimerCallback_t, callback_ptr);

    UT_GenStub_Execute(OS_TimerCreate, Basic, UT_DefaultHandler_OS_TimerCreate);

    return UT_GenStub_GetReturnValue(OS_TimerCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerDelete()
 * ----------------------------------------------------
 */
int32 OS_TimerDelete(osal_id_t timer_id)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerDelete, int32);

    UT_GenStub_AddParam(OS_TimerDelete, osal_id_t, timer_id);

    UT_GenStub_Execute(OS_TimerDelete, Basic, UT_DefaultHandler_OS_TimerDelete);

    return UT_GenStub_GetReturnValue(OS_TimerDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_TimerGetIdByName(osal_id_t *timer_id, const char *timer_name)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerGetIdByName, int32);

    UT_GenStub_AddParam(OS_TimerGetIdByName, osal_id_t *, timer_id);
    UT_GenStub_AddParam(OS_TimerGetIdByName, const char *, timer_name);

    UT_GenStub_Execute(OS_TimerGetIdByName, Basic, UT_DefaultHandler_OS_TimerGetIdByName);

    return UT_GenStub_GetReturnValue(OS_TimerGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerGetInfo()
 * ----------------------------------------------------
 */
int32 OS_TimerGetInfo(osal_id_t timer_id, OS_timer_prop_t *timer_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerGetInfo, int32);

    UT_GenStub_AddParam(OS_TimerGetInfo, osal_id_t, timer_id);
    UT_GenStub_AddParam(OS_TimerGetInfo, OS_timer_prop_t *, timer_prop);

    UT_GenStub_Execute(OS_TimerGetInfo, Basic, UT_DefaultHandler_OS_TimerGetInfo);

    return UT_GenStub_GetReturnValue(OS_TimerGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimerSet()
 * ----------------------------------------------------
 */
int32 OS_TimerSet(osal_id_t timer_id, uint32 start_time, uint32 interval_time)
{
    UT_GenStub_SetupReturnBuffer(OS_TimerSet, int32);

    UT_GenStub_AddParam(OS_TimerSet, osal_id_t, timer_id);
    UT_GenStub_AddParam(OS_TimerSet, uint32, start_time);
    UT_GenStub_AddParam(OS_TimerSet, uint32, interval_time);

    UT_GenStub_Execute(OS_TimerSet, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimerSet, int32);
}
