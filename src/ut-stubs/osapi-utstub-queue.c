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

#include "osapi-queue.h" /* OSAL public API for this subsystem */
#include "osapi-idmap.h"
#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_QueueAPI_Init, (void))

/*****************************************************************************/
/**
** \brief OS_QueueCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueCreate.  The user can adjust the response by setting
**        the values in the QueueCreateRtn structure prior to this function
**        being called.  If the value QueueCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueCreateRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        creating the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to the real call, doesn't care about creator or do any
**           mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_INVALID_POINTER,
**        OS_ERR_NAME_TOO_LONG, OS_ERR_NO_FREE_IDS, OS_ERR_NAME_TAKEN,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueCreate(osal_id_t *queue_id, const char *queue_name, osal_blockcount_t queue_depth, size_t data_size,
                     uint32 flags)
{
    UT_Stub_RegisterContext(UT_KEY(OS_QueueCreate), queue_id);
    UT_Stub_RegisterContext(UT_KEY(OS_QueueCreate), queue_name);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueCreate), queue_depth);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueCreate), data_size);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueCreate), flags);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueCreate);

    if (status == OS_SUCCESS)
    {
        *queue_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_QUEUE);
    }
    else
    {
        *queue_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueDelete.  The user can adjust the response by setting
**        the values in the QueueDelRtn structure prior to this function
**        being called.  If the value QueueDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueDelRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        deleting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to real code without mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueDelete(osal_id_t queue_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueDelete), queue_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_QUEUE, queue_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGet stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGet.  The user can adjust the response by setting
**        the values in the QueueGetRtn structure prior to this function
**        being called.  If the value QueueGetRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueGetRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        getting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Works similar to real function; note that pend on empty queue
**           doesn't block\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_EMPTY, OS_QUEUE_INVALID_SIZE,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGet(osal_id_t queue_id, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueGet), queue_id);
    UT_Stub_RegisterContext(UT_KEY(OS_QueueGet), data);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueGet), size);
    UT_Stub_RegisterContext(UT_KEY(OS_QueueGet), size_copied);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueGet), timeout);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGet);

    if (status == OS_SUCCESS)
    {
        *size_copied = UT_Stub_CopyToLocal((UT_EntryKey_t)OS_ObjectIdToInteger(queue_id), data, size);
        if (*size_copied == 0)
        {
            status = OS_QUEUE_EMPTY;
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueuePut stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueuePut.  The user can adjust the response by setting
**        the values in the QueuePutRtn structure prior to this function
**        being called.  If the value QueuePutRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueuePutRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        putting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Same as real function\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_FULL, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueuePut(osal_id_t queue_id, const void *data, size_t size, uint32 flags)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueuePut), queue_id);
    UT_Stub_RegisterContext(UT_KEY(OS_QueuePut), data);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueuePut), size);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueuePut), flags);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueuePut);

    if (status == OS_SUCCESS)
    {
        UT_SetDataBuffer((UT_EntryKey_t)OS_ObjectIdToInteger(queue_id), (void *)data, size, true);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_QueueGetIdByName()
 *
 *****************************************************************************/
int32 OS_QueueGetIdByName(osal_id_t *queue_id, const char *queue_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_QueueGetIdByName), queue_id);
    UT_Stub_RegisterContext(UT_KEY(OS_QueueGetIdByName), queue_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetIdByName), queue_id, sizeof(*queue_id)) < sizeof(*queue_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_QUEUE, queue_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGetInfo.  It sets the queue structure variables to fixed
**        values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGetInfo(osal_id_t queue_id, OS_queue_prop_t *queue_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_QueueGetInfo), queue_id);
    UT_Stub_RegisterContext(UT_KEY(OS_QueueGetInfo), queue_prop);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGetInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetInfo), queue_prop, sizeof(*queue_prop)) < sizeof(*queue_prop))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &queue_prop->creator);
        strncpy(queue_prop->name, "Name", sizeof(queue_prop->name) - 1);
        queue_prop->name[sizeof(queue_prop->name) - 1] = '\0';
    }

    return status;
}
