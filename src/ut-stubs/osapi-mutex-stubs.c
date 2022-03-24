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
 * Auto-Generated stub implementations for functions defined in osapi-mutex header
 */

#include "osapi-mutex.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_MutSemCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_MutSemDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_MutSemGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_MutSemGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemCreate()
 * ----------------------------------------------------
 */
int32 OS_MutSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemCreate, int32);

    UT_GenStub_AddParam(OS_MutSemCreate, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_MutSemCreate, const char *, sem_name);
    UT_GenStub_AddParam(OS_MutSemCreate, uint32, options);

    UT_GenStub_Execute(OS_MutSemCreate, Basic, UT_DefaultHandler_OS_MutSemCreate);

    return UT_GenStub_GetReturnValue(OS_MutSemCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemDelete()
 * ----------------------------------------------------
 */
int32 OS_MutSemDelete(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemDelete, int32);

    UT_GenStub_AddParam(OS_MutSemDelete, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_MutSemDelete, Basic, UT_DefaultHandler_OS_MutSemDelete);

    return UT_GenStub_GetReturnValue(OS_MutSemDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_MutSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGetIdByName, int32);

    UT_GenStub_AddParam(OS_MutSemGetIdByName, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_MutSemGetIdByName, const char *, sem_name);

    UT_GenStub_Execute(OS_MutSemGetIdByName, Basic, UT_DefaultHandler_OS_MutSemGetIdByName);

    return UT_GenStub_GetReturnValue(OS_MutSemGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGetInfo()
 * ----------------------------------------------------
 */
int32 OS_MutSemGetInfo(osal_id_t sem_id, OS_mut_sem_prop_t *mut_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGetInfo, int32);

    UT_GenStub_AddParam(OS_MutSemGetInfo, osal_id_t, sem_id);
    UT_GenStub_AddParam(OS_MutSemGetInfo, OS_mut_sem_prop_t *, mut_prop);

    UT_GenStub_Execute(OS_MutSemGetInfo, Basic, UT_DefaultHandler_OS_MutSemGetInfo);

    return UT_GenStub_GetReturnValue(OS_MutSemGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGive()
 * ----------------------------------------------------
 */
int32 OS_MutSemGive(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGive, int32);

    UT_GenStub_AddParam(OS_MutSemGive, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_MutSemGive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemGive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemTake()
 * ----------------------------------------------------
 */
int32 OS_MutSemTake(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemTake, int32);

    UT_GenStub_AddParam(OS_MutSemTake, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_MutSemTake, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemTake, int32);
}
