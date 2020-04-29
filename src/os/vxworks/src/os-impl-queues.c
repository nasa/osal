/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
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


/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
OS_impl_queue_internal_record_t OS_impl_queue_table     [OS_MAX_QUEUES];

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
int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
{
    MSG_Q_ID tmp_msgq_id;
    int queue_depth = OS_queue_table[queue_id].max_depth; /* maximum number of messages in queue (queue depth) */
    int data_size = OS_queue_table[queue_id].max_size;    /* maximum size in bytes of a message */

    /* Create VxWorks Message Queue */
    tmp_msgq_id = msgQCreate(queue_depth, data_size, MSG_Q_FIFO);

    /* check if message Q create failed */
    if(tmp_msgq_id == 0)
    {
        OS_DEBUG("msgQCreate() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    OS_impl_queue_table[queue_id].vxid = tmp_msgq_id;
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
int32 OS_QueueDelete_Impl (uint32 queue_id)
{
    /* Try to delete the queue */
    if (msgQDelete(OS_impl_queue_table[queue_id].vxid) != OK)
    {
        OS_DEBUG("msgQDelete() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    OS_impl_queue_table[queue_id].vxid = 0;
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
int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied,
                    int32 timeout)
{
    int32              return_code;
    STATUS             status;
    int                ticks;

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
        /* msecs rounded to the closest system tick count */
        ticks = OS_Milli2Ticks(timeout);
    }

    status = msgQReceive(OS_impl_queue_table[queue_id].vxid, data, size, ticks);

    if(status == ERROR)
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
            OS_DEBUG("msgQReceive() - vxWorks errno %d\n",errno);
            return_code = OS_ERROR;
        }
    }
    else
    {
        *size_copied = status;
        return_code = OS_SUCCESS;
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
int32 OS_QueuePut_Impl (uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    int32              return_code;

    if(msgQSend(OS_impl_queue_table[queue_id].vxid, (void*)data, size, NO_WAIT, MSG_PRI_NORMAL) == OK)
    {
        return_code = OS_SUCCESS;
    }
    else if(errno == S_objLib_OBJ_UNAVAILABLE)
    {
        return_code = OS_QUEUE_FULL;
    }
    else
    {
        OS_DEBUG("msgQSend() - vxWorks errno %d\n",errno);
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
int32 OS_QueueGetInfo_Impl (uint32 queue_id, OS_queue_prop_t *queue_prop)
{
    /* No extra info for queues in the OS implementation */
    return OS_SUCCESS;

} /* end OS_QueueGetInfo_Impl */

