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
 * Auto-Generated stub implementations for functions defined in osapi-countsem header
 */

#include "osapi-countsem.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_CountSemCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_CountSemDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_CountSemGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_CountSemGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemCreate()
 * ----------------------------------------------------
 */
int32 OS_CountSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemCreate, int32);

    UT_GenStub_AddParam(OS_CountSemCreate, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_CountSemCreate, const char *, sem_name);
    UT_GenStub_AddParam(OS_CountSemCreate, uint32, sem_initial_value);
    UT_GenStub_AddParam(OS_CountSemCreate, uint32, options);

    UT_GenStub_Execute(OS_CountSemCreate, Basic, UT_DefaultHandler_OS_CountSemCreate);

    return UT_GenStub_GetReturnValue(OS_CountSemCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemDelete()
 * ----------------------------------------------------
 */
int32 OS_CountSemDelete(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemDelete, int32);

    UT_GenStub_AddParam(OS_CountSemDelete, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_CountSemDelete, Basic, UT_DefaultHandler_OS_CountSemDelete);

    return UT_GenStub_GetReturnValue(OS_CountSemDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_CountSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemGetIdByName, int32);

    UT_GenStub_AddParam(OS_CountSemGetIdByName, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_CountSemGetIdByName, const char *, sem_name);

    UT_GenStub_Execute(OS_CountSemGetIdByName, Basic, UT_DefaultHandler_OS_CountSemGetIdByName);

    return UT_GenStub_GetReturnValue(OS_CountSemGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemGetInfo()
 * ----------------------------------------------------
 */
int32 OS_CountSemGetInfo(osal_id_t sem_id, OS_count_sem_prop_t *count_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemGetInfo, int32);

    UT_GenStub_AddParam(OS_CountSemGetInfo, osal_id_t, sem_id);
    UT_GenStub_AddParam(OS_CountSemGetInfo, OS_count_sem_prop_t *, count_prop);

    UT_GenStub_Execute(OS_CountSemGetInfo, Basic, UT_DefaultHandler_OS_CountSemGetInfo);

    return UT_GenStub_GetReturnValue(OS_CountSemGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemGive()
 * ----------------------------------------------------
 */
int32 OS_CountSemGive(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemGive, int32);

    UT_GenStub_AddParam(OS_CountSemGive, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_CountSemGive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemGive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemTake()
 * ----------------------------------------------------
 */
int32 OS_CountSemTake(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemTake, int32);

    UT_GenStub_AddParam(OS_CountSemTake, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_CountSemTake, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemTake, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CountSemTimedWait()
 * ----------------------------------------------------
 */
int32 OS_CountSemTimedWait(osal_id_t sem_id, uint32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_CountSemTimedWait, int32);

    UT_GenStub_AddParam(OS_CountSemTimedWait, osal_id_t, sem_id);
    UT_GenStub_AddParam(OS_CountSemTimedWait, uint32, msecs);

    UT_GenStub_Execute(OS_CountSemTimedWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CountSemTimedWait, int32);
}
