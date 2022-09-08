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
 * Auto-Generated stub implementations for functions defined in osapi-condvar header
 */

#include "osapi-condvar.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarBroadcast()
 * ----------------------------------------------------
 */
int32 OS_CondVarBroadcast(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarBroadcast, int32);

    UT_GenStub_AddParam(OS_CondVarBroadcast, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarBroadcast, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarBroadcast, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarCreate()
 * ----------------------------------------------------
 */
int32 OS_CondVarCreate(osal_id_t *var_id, const char *var_name, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarCreate, int32);

    UT_GenStub_AddParam(OS_CondVarCreate, osal_id_t *, var_id);
    UT_GenStub_AddParam(OS_CondVarCreate, const char *, var_name);
    UT_GenStub_AddParam(OS_CondVarCreate, uint32, options);

    UT_GenStub_Execute(OS_CondVarCreate, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarDelete()
 * ----------------------------------------------------
 */
int32 OS_CondVarDelete(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarDelete, int32);

    UT_GenStub_AddParam(OS_CondVarDelete, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarDelete, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_CondVarGetIdByName(osal_id_t *var_id, const char *var_name)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarGetIdByName, int32);

    UT_GenStub_AddParam(OS_CondVarGetIdByName, osal_id_t *, var_id);
    UT_GenStub_AddParam(OS_CondVarGetIdByName, const char *, var_name);

    UT_GenStub_Execute(OS_CondVarGetIdByName, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarGetInfo()
 * ----------------------------------------------------
 */
int32 OS_CondVarGetInfo(osal_id_t var_id, OS_condvar_prop_t *condvar_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarGetInfo, int32);

    UT_GenStub_AddParam(OS_CondVarGetInfo, osal_id_t, var_id);
    UT_GenStub_AddParam(OS_CondVarGetInfo, OS_condvar_prop_t *, condvar_prop);

    UT_GenStub_Execute(OS_CondVarGetInfo, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarLock()
 * ----------------------------------------------------
 */
int32 OS_CondVarLock(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarLock, int32);

    UT_GenStub_AddParam(OS_CondVarLock, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarLock, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarLock, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarSignal()
 * ----------------------------------------------------
 */
int32 OS_CondVarSignal(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarSignal, int32);

    UT_GenStub_AddParam(OS_CondVarSignal, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarSignal, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarSignal, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarTimedWait()
 * ----------------------------------------------------
 */
int32 OS_CondVarTimedWait(osal_id_t var_id, const OS_time_t *abs_wakeup_time)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarTimedWait, int32);

    UT_GenStub_AddParam(OS_CondVarTimedWait, osal_id_t, var_id);
    UT_GenStub_AddParam(OS_CondVarTimedWait, const OS_time_t *, abs_wakeup_time);

    UT_GenStub_Execute(OS_CondVarTimedWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarTimedWait, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarUnlock()
 * ----------------------------------------------------
 */
int32 OS_CondVarUnlock(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarUnlock, int32);

    UT_GenStub_AddParam(OS_CondVarUnlock, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarUnlock, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarUnlock, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CondVarWait()
 * ----------------------------------------------------
 */
int32 OS_CondVarWait(osal_id_t var_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CondVarWait, int32);

    UT_GenStub_AddParam(OS_CondVarWait, osal_id_t, var_id);

    UT_GenStub_Execute(OS_CondVarWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CondVarWait, int32);
}
