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
 * Auto-Generated stub implementations for functions defined in osapi-queue header
 */

#include "osapi-queue.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_QueueCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_QueueDelete(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_QueueGet(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_QueueGetIdByName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_QueueGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_QueuePut(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueCreate()
 * ----------------------------------------------------
 */
int32 OS_QueueCreate(osal_id_t *queue_id, const char *queue_name, osal_blockcount_t queue_depth, size_t data_size,
                     uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueCreate, int32);

    UT_GenStub_AddParam(OS_QueueCreate, osal_id_t *, queue_id);
    UT_GenStub_AddParam(OS_QueueCreate, const char *, queue_name);
    UT_GenStub_AddParam(OS_QueueCreate, osal_blockcount_t, queue_depth);
    UT_GenStub_AddParam(OS_QueueCreate, size_t, data_size);
    UT_GenStub_AddParam(OS_QueueCreate, uint32, flags);

    UT_GenStub_Execute(OS_QueueCreate, Basic, UT_DefaultHandler_OS_QueueCreate);

    return UT_GenStub_GetReturnValue(OS_QueueCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueDelete()
 * ----------------------------------------------------
 */
int32 OS_QueueDelete(osal_id_t queue_id)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueDelete, int32);

    UT_GenStub_AddParam(OS_QueueDelete, osal_id_t, queue_id);

    UT_GenStub_Execute(OS_QueueDelete, Basic, UT_DefaultHandler_OS_QueueDelete);

    return UT_GenStub_GetReturnValue(OS_QueueDelete, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueGet()
 * ----------------------------------------------------
 */
int32 OS_QueueGet(osal_id_t queue_id, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueGet, int32);

    UT_GenStub_AddParam(OS_QueueGet, osal_id_t, queue_id);
    UT_GenStub_AddParam(OS_QueueGet, void *, data);
    UT_GenStub_AddParam(OS_QueueGet, size_t, size);
    UT_GenStub_AddParam(OS_QueueGet, size_t *, size_copied);
    UT_GenStub_AddParam(OS_QueueGet, int32, timeout);

    UT_GenStub_Execute(OS_QueueGet, Basic, UT_DefaultHandler_OS_QueueGet);

    return UT_GenStub_GetReturnValue(OS_QueueGet, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueGetIdByName()
 * ----------------------------------------------------
 */
int32 OS_QueueGetIdByName(osal_id_t *queue_id, const char *queue_name)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueGetIdByName, int32);

    UT_GenStub_AddParam(OS_QueueGetIdByName, osal_id_t *, queue_id);
    UT_GenStub_AddParam(OS_QueueGetIdByName, const char *, queue_name);

    UT_GenStub_Execute(OS_QueueGetIdByName, Basic, UT_DefaultHandler_OS_QueueGetIdByName);

    return UT_GenStub_GetReturnValue(OS_QueueGetIdByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueGetInfo()
 * ----------------------------------------------------
 */
int32 OS_QueueGetInfo(osal_id_t queue_id, OS_queue_prop_t *queue_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueGetInfo, int32);

    UT_GenStub_AddParam(OS_QueueGetInfo, osal_id_t, queue_id);
    UT_GenStub_AddParam(OS_QueueGetInfo, OS_queue_prop_t *, queue_prop);

    UT_GenStub_Execute(OS_QueueGetInfo, Basic, UT_DefaultHandler_OS_QueueGetInfo);

    return UT_GenStub_GetReturnValue(OS_QueueGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueuePut()
 * ----------------------------------------------------
 */
int32 OS_QueuePut(osal_id_t queue_id, const void *data, size_t size, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_QueuePut, int32);

    UT_GenStub_AddParam(OS_QueuePut, osal_id_t, queue_id);
    UT_GenStub_AddParam(OS_QueuePut, const void *, data);
    UT_GenStub_AddParam(OS_QueuePut, size_t, size);
    UT_GenStub_AddParam(OS_QueuePut, uint32, flags);

    UT_GenStub_Execute(OS_QueuePut, Basic, UT_DefaultHandler_OS_QueuePut);

    return UT_GenStub_GetReturnValue(OS_QueuePut, int32);
}
