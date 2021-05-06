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
 * \file     os-impl-queues.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-queues.h"
#include "os-shared-queue.h"
#include "os-shared-timebase.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
OS_impl_queue_internal_record_t OS_impl_queue_table[OS_MAX_QUEUES];

/****************************************************************************************
                                MESSAGE QUEUE API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_QueueAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_QueueAPI_Impl_Init(void)
{
    memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));
    return (OS_SUCCESS);
} /* end OS_VxWorks_QueueAPI_Impl_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    MSG_Q_ID                         tmp_msgq_id;
    int                              queue_depth;
    int                              data_size;
    OS_impl_queue_internal_record_t *impl;
    OS_queue_internal_record_t *     queue;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    queue = OS_OBJECT_TABLE_GET(OS_queue_table, *token);

    queue_depth = queue->max_depth; /* maximum number of messages in queue (queue depth) */
    data_size   = queue->max_size;  /* maximum size in bytes of a message */

    /* Create VxWorks Message Queue */
    tmp_msgq_id = msgQCreate(queue_depth, data_size, MSG_Q_FIFO);

    /* check if message Q create failed */
    if (tmp_msgq_id == 0)
    {
        OS_DEBUG("msgQCreate() - vxWorks errno %d\n", errno);
        return OS_ERROR;
    }

    impl->vxid = tmp_msgq_id;
    return OS_SUCCESS;

} /* end OS_QueueCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    /* Try to delete the queue */
    if (msgQDelete(impl->vxid) != OK)
    {
        OS_DEBUG("msgQDelete() - vxWorks errno %d\n", errno);
        return OS_ERROR;
    }

    impl->vxid = 0;
    return OS_SUCCESS;

} /* end OS_QueueDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueGet_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGet_Impl(const OS_object_token_t *token, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    int32                            return_code;
    STATUS                           status;
    int                              ticks;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    /* Get Message From Message Queue */
    if (timeout == OS_PEND)
    {
        ticks = WAIT_FOREVER;
    }
    else if (timeout == OS_CHECK)
    {
        ticks = NO_WAIT;
    }
    else
    {
        /* msecs rounded to the closest system tick count if possible */
        if (OS_Milli2Ticks(timeout, &ticks) != OS_SUCCESS)
        {
            return OS_ERROR;
        }
    }

    status = msgQReceive(impl->vxid, data, size, ticks);

    if (status == ERROR)
    {
        *size_copied = 0;
        if (errno == S_objLib_OBJ_TIMEOUT)
        {
            return_code = OS_QUEUE_TIMEOUT;
        }
        else if (errno == S_objLib_OBJ_UNAVAILABLE)
        {
            return_code = OS_QUEUE_EMPTY;
        }
        else
        {
            OS_DEBUG("msgQReceive() - vxWorks errno %d\n", errno);
            return_code = OS_ERROR;
        }
    }
    else
    {
        *size_copied = OSAL_SIZE_C(status);
        return_code  = OS_SUCCESS;
    }

    return return_code;
} /* end OS_QueueGet_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_QueuePut_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueuePut_Impl(const OS_object_token_t *token, const void *data, size_t size, uint32 flags)
{
    int32                            return_code;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    if (msgQSend(impl->vxid, (void *)data, size, NO_WAIT, MSG_PRI_NORMAL) == OK)
    {
        return_code = OS_SUCCESS;
    }
    else if (errno == S_objLib_OBJ_UNAVAILABLE)
    {
        return_code = OS_QUEUE_FULL;
    }
    else
    {
        OS_DEBUG("msgQSend() - vxWorks errno %d\n", errno);
        return_code = OS_ERROR;
    }

    return return_code;

} /* end OS_QueuePut_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetInfo_Impl(const OS_object_token_t *token, OS_queue_prop_t *queue_prop)
{
    /* No extra info for queues in the OS implementation */
    return OS_SUCCESS;

} /* end OS_QueueGetInfo_Impl */
