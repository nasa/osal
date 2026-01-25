/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * Auto-Generated stub implementations for functions defined in osapi-rwlock header
 */

#include "osapi-rwlock.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_RwLockCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_RwLockDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_RwLockGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_RwLockGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockCreate()
 * ----------------------------------------------------
 */
int32 OS_RwLockCreate(osal_id_t *rw_id, const char *rw_name, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockCreate, int32);

    UT_GenStub_AddParam(OS_RwLockCreate, osal_id_t *, rw_id);
    UT_GenStub_AddParam(OS_RwLockCreate, const char *, rw_name);
    UT_GenStub_AddParam(OS_RwLockCreate, uint32, options);

    UT_GenStub_Execute(OS_RwLockCreate, Basic, UT_DefaultHandler_OS_RwLockCreate);

    return UT_GenStub_GetReturnValue(OS_RwLockCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockDelete()
 * ----------------------------------------------------
 */
int32 OS_RwLockDelete(osal_id_t rw_id)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockDelete, int32);

    UT_GenStub_AddParam(OS_RwLockDelete, osal_id_t, rw_id);

    UT_GenStub_Execute(OS_RwLockDelete, Basic, UT_DefaultHandler_OS_RwLockDelete);

    return UT_GenStub_GetReturnValue(OS_RwLockDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_RwLockGetIdByName(osal_id_t *rw_id, const char *rw_name)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockGetIdByName, int32);

    UT_GenStub_AddParam(OS_RwLockGetIdByName, osal_id_t *, rw_id);
    UT_GenStub_AddParam(OS_RwLockGetIdByName, const char *, rw_name);

    UT_GenStub_Execute(OS_RwLockGetIdByName, Basic, UT_DefaultHandler_OS_RwLockGetIdByName);

    return UT_GenStub_GetReturnValue(OS_RwLockGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockGetInfo()
 * ----------------------------------------------------
 */
int32 OS_RwLockGetInfo(osal_id_t rw_id, OS_rwlock_prop_t *rw_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockGetInfo, int32);

    UT_GenStub_AddParam(OS_RwLockGetInfo, osal_id_t, rw_id);
    UT_GenStub_AddParam(OS_RwLockGetInfo, OS_rwlock_prop_t *, rw_prop);

    UT_GenStub_Execute(OS_RwLockGetInfo, Basic, UT_DefaultHandler_OS_RwLockGetInfo);

    return UT_GenStub_GetReturnValue(OS_RwLockGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockReadGive()
 * ----------------------------------------------------
 */
int32 OS_RwLockReadGive(osal_id_t rw_id)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockReadGive, int32);

    UT_GenStub_AddParam(OS_RwLockReadGive, osal_id_t, rw_id);

    UT_GenStub_Execute(OS_RwLockReadGive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockReadGive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockReadTake()
 * ----------------------------------------------------
 */
int32 OS_RwLockReadTake(osal_id_t rw_id)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockReadTake, int32);

    UT_GenStub_AddParam(OS_RwLockReadTake, osal_id_t, rw_id);

    UT_GenStub_Execute(OS_RwLockReadTake, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockReadTake, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockWriteGive()
 * ----------------------------------------------------
 */
int32 OS_RwLockWriteGive(osal_id_t rw_id)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockWriteGive, int32);

    UT_GenStub_AddParam(OS_RwLockWriteGive, osal_id_t, rw_id);

    UT_GenStub_Execute(OS_RwLockWriteGive, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockWriteGive, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RwLockWriteTake()
 * ----------------------------------------------------
 */
int32 OS_RwLockWriteTake(osal_id_t rw_id)
{
    UT_GenStub_SetupReturnBuffer(OS_RwLockWriteTake, int32);

    UT_GenStub_AddParam(OS_RwLockWriteTake, osal_id_t, rw_id);

    UT_GenStub_Execute(OS_RwLockWriteTake, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RwLockWriteTake, int32);
}
