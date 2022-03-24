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
 * Auto-Generated stub implementations for functions defined in osapi-binsem header
 */

#include "osapi-binsem.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_BinSemCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_BinSemDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_BinSemGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_BinSemGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemCreate()
 * ----------------------------------------------------
 */
int32 OS_BinSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemCreate, int32);

    UT_GenStub_AddParam(OS_BinSemCreate, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_BinSemCreate, const char *, sem_name);
    UT_GenStub_AddParam(OS_BinSemCreate, uint32, sem_initial_value);
    UT_GenStub_AddParam(OS_BinSemCreate, uint32, options);

    UT_GenStub_Execute(OS_BinSemCreate, Basic, UT_DefaultHandler_OS_BinSemCreate);

    return UT_GenStub_GetReturnValue(OS_BinSemCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemDelete()
 * ----------------------------------------------------
 */
int32 OS_BinSemDelete(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemDelete, int32);

    UT_GenStub_AddParam(OS_BinSemDelete, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_BinSemDelete, Basic, UT_DefaultHandler_OS_BinSemDelete);

    return UT_GenStub_GetReturnValue(OS_BinSemDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemFlush()
 * ----------------------------------------------------
 */
int32 OS_BinSemFlush(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemFlush, int32);

    UT_GenStub_AddParam(OS_BinSemFlush, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_BinSemFlush, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BinSemFlush, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_BinSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemGetIdByName, int32);

    UT_GenStub_AddParam(OS_BinSemGetIdByName, osal_id_t *, sem_id);
    UT_GenStub_AddParam(OS_BinSemGetIdByName, const char *, sem_name);

    UT_GenStub_Execute(OS_BinSemGetIdByName, Basic, UT_DefaultHandler_OS_BinSemGetIdByName);

    return UT_GenStub_GetReturnValue(OS_BinSemGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemGetInfo()
 * ----------------------------------------------------
 */
int32 OS_BinSemGetInfo(osal_id_t sem_id, OS_bin_sem_prop_t *bin_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemGetInfo, int32);

    UT_GenStub_AddParam(OS_BinSemGetInfo, osal_id_t, sem_id);
    UT_GenStub_AddParam(OS_BinSemGetInfo, OS_bin_sem_prop_t *, bin_prop);

    UT_GenStub_Execute(OS_BinSemGetInfo, Basic, UT_DefaultHandler_OS_BinSemGetInfo);

    return UT_GenStub_GetReturnValue(OS_BinSemGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemGive()
 * ----------------------------------------------------
 */
int32 OS_BinSemGive(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemGive, int32);

    UT_GenStub_AddParam(OS_BinSemGive, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_BinSemGive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BinSemGive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemTake()
 * ----------------------------------------------------
 */
int32 OS_BinSemTake(osal_id_t sem_id)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemTake, int32);

    UT_GenStub_AddParam(OS_BinSemTake, osal_id_t, sem_id);

    UT_GenStub_Execute(OS_BinSemTake, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BinSemTake, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BinSemTimedWait()
 * ----------------------------------------------------
 */
int32 OS_BinSemTimedWait(osal_id_t sem_id, uint32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_BinSemTimedWait, int32);

    UT_GenStub_AddParam(OS_BinSemTimedWait, osal_id_t, sem_id);
    UT_GenStub_AddParam(OS_BinSemTimedWait, uint32, msecs);

    UT_GenStub_Execute(OS_BinSemTimedWait, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BinSemTimedWait, int32);
}
