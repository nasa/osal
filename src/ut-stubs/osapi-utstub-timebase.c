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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-timebase.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_TimeBaseAPI_Init, (void))

/*****************************************************************************
 *
 * Stub for OS_TimeBaseCreate() function
 *
 *****************************************************************************/
int32 OS_TimeBaseCreate(osal_id_t *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseCreate), timebase_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseCreate), timebase_name);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseCreate), external_sync);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseCreate);

    if (status == OS_SUCCESS)
    {
        *timebase_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMEBASE);
    }
    else
    {
        *timebase_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseSet() function
 *
 *****************************************************************************/
int32 OS_TimeBaseSet(osal_id_t timebase_id, uint32 start_time, uint32 interval_time)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseSet), timebase_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseSet), start_time);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseSet), interval_time);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseSet);

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseDelete() function
 *
 *****************************************************************************/
int32 OS_TimeBaseDelete(osal_id_t timebase_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseDelete), timebase_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetIdByName() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetIdByName(osal_id_t *timebase_id, const char *timebase_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseGetIdByName), timebase_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseGetIdByName), timebase_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetIdByName), timebase_id, sizeof(*timebase_id)) < sizeof(*timebase_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TIMEBASE, timebase_id);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetInfo() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetInfo(osal_id_t timebase_id, OS_timebase_prop_t *timebase_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseGetInfo), timebase_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseGetInfo), timebase_prop);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetInfo), timebase_prop, sizeof(*timebase_prop)) < sizeof(*timebase_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &timebase_prop->creator);
        strncpy(timebase_prop->name, "Name", sizeof(timebase_prop->name) - 1);
        timebase_prop->name[sizeof(timebase_prop->name) - 1] = '\0';
    }

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetFreeRun() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetFreeRun(osal_id_t timebase_id, uint32 *freerun_val)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimeBaseGetFreeRun), timebase_id);
    UT_Stub_RegisterContext(UT_KEY(OS_TimeBaseGetFreeRun), freerun_val);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetFreeRun);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetFreeRun), freerun_val, sizeof(*freerun_val)) < sizeof(*freerun_val))
    {
        int32 tempcount;
        int32 temprc;

        /*
         * Use the call count such that the value increases with each successive call.
         * If that doesn't work then just return a constant nonzero value.
         */
        if (UT_GetStubRetcodeAndCount(UT_KEY(OS_TimeBaseGetFreeRun), &temprc, &tempcount))
        {
            *freerun_val = tempcount;
        }
        else
        {
            *freerun_val = 1;
        }
    }

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBase_CallbackThread() function
 *
 *****************************************************************************/
void OS_TimeBase_CallbackThread(uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBase_CallbackThread);
}

/*****************************************************************************
 *
 * Stub for OS_Milli2Ticks() function
 *
 *****************************************************************************/
int32 OS_Milli2Ticks(uint32 milli_seconds, int *ticks)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_Milli2Ticks), milli_seconds);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_Milli2Ticks), ticks);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_Milli2Ticks);

    if (status >= 0)
    {
        UT_Stub_CopyToLocal(UT_KEY(OS_Milli2Ticks), (uint8 *)ticks, sizeof(*ticks));
    }

    return status;
}
