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
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "bsp-impl.h"
#include <QProcess>
#include "os-impl-queues.h"
#include "os-shared-queue.h"
#include "os-shared-idmap.h"

/* Tables where the OS object information is stored */
OS_impl_queue_internal_record_t OS_impl_queue_table[OS_MAX_QUEUES];

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_QT_QueueAPI_Impl_Init

   Purpose: Initialize the Queue data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_QT_QueueAPI_Impl_Init(void)
{
    memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));

    /*
     * Automatic truncation is dependent on the OSAL_CONFIG_DEBUG_PERMISSIVE_MODE compile-time define - so
     * creating a too-large message queue on a target without OSAL_CONFIG_DEBUG_PERMISSIVE_MODE will fail
     * with an OS error as intended.
     */
#ifdef OSAL_CONFIG_DEBUG_PERMISSIVE_MODE
    /*
     * Use the BSP-provided limit
     */
    QT_GlobalVars.TruncateQueueDepth = OS_BSP_Global.MaxQueueDepth;
#else
    /*
     * Initialize this to zero to indicate no limit
     */
    QT_GlobalVars.TruncateQueueDepth = OSAL_BLOCKCOUNT_C(0);
#endif

    return OS_SUCCESS;
} /* end OS_QT_QueueAPI_Impl_Init */

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
    int return_code = OS_SUCCESS;

    OS_impl_queue_internal_record_t *impl;
    OS_queue_internal_record_t *     queue;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    queue = OS_OBJECT_TABLE_GET(OS_queue_table, *token);

    /* set queue attributes */
    impl->max_depth = queue->max_depth;
    impl->max_size = queue->max_size;
    /*
     * The "TruncateQueueDepth" indicates a soft limit to the size of a queue.
     * If nonzero, anything larger than this will be silently truncated
     * (Supports running applications as non-root)
     */
    if (QT_GlobalVars.TruncateQueueDepth > 0 && QT_GlobalVars.TruncateQueueDepth < queue->max_size)
    {
        impl->max_size = QT_GlobalVars.TruncateQueueDepth;
    }

    /*
    ** Construct the queue name:
    ** The name will consist of "/<process_id>.queue_name"
    */
   int procId = QCoreApplication::applicationPid();
   snprintf(impl->name, sizeof(impl->name), "/%d.%s", (int)procId, queue->queue_name);

    /*
     ** create message queue
     */

    bool haveQueue = false;
    bool haveSem = false;
    do{
        impl->id = new QQueue<OS_imp_queue_msg*>();
        if (impl->id == NULL)
        {
            return_code = OS_QUEUE_ID_ERROR;
            OS_DEBUG("OS_QueueCreate Error.\n");
            break;
        }
        haveQueue = true;

        impl->sem = new QSemaphore();
        if(impl->sem == 0){
            return_code = OS_QUEUE_ID_ERROR;
            OS_DEBUG("OS_QueueCreate Error.\n");
            break;
        }
        haveSem = true;

    }while(0);

    if((return_code != OS_SUCCESS) && haveQueue)
    {
        delete impl->id;
    }
    if((return_code != OS_SUCCESS) && haveSem)
    {
        delete impl->sem;
    }

    return return_code;
} /* end OS_QueueCreate_Impl */

void OS_QueueObjDelete(OS_imp_queue_msg * obj){
    delete obj->message;
    delete obj;
}

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
    int32                            return_code;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    while (!impl->id->isEmpty()){
        OS_imp_queue_msg * obj = impl->id->dequeue();
        OS_QueueObjDelete(obj);
    }

    /* Try to delete and unlink the queue */
    delete impl->id;
    impl->id = NULL;

    return_code = OS_SUCCESS;


    return return_code;
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
    /* TODO This could be smarter if the get command
     * registers the request, and the next put command directly copies
     * data into the destination buffer, rather than using an
     * intermidate buffer.
    */
    int32 return_code = OS_SUCCESS;
    ssize_t                          sizeCopied = -1;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    OS_imp_queue_msg* msg = NULL;

    if(data == NULL)
        return OS_ERROR;
    /*
     ** Read the message queue for data
     */
    if (timeout == OS_PEND)
    {
        impl->sem->acquire();
    }
    else if (timeout == OS_CHECK)
    {
        if(impl->sem->tryAcquire() == false)
            return_code = OS_QUEUE_EMPTY;
    }else
    {
        if(impl->sem->tryAcquire(1,timeout) == false)
            return_code = OS_QUEUE_TIMEOUT;
    }

    if(return_code == OS_SUCCESS){
        msg = impl->id->dequeue();
        if(msg->size > size){
            return_code = OS_QUEUE_INVALID_SIZE;
            msg->size = size; /* TODO DEBUG message here ? */
        }
        sizeCopied = msg->size;
        memcpy(data, msg->message, *size_copied);
        OS_QueueObjDelete(msg);

    }else{
        return_code = OS_QUEUE_EMPTY;
    }

    if(size_copied != NULL){
        *size_copied = sizeCopied;
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
    int32                            return_code =OS_SUCCESS;
    int                              result;
    struct timespec                  ts;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    OS_imp_queue_msg* msg = new OS_imp_queue_msg();
    if(msg == NULL){
        return OS_ERROR;
    }
    /* TODO return / check OS_QUEUE_FULL */
    msg->size = size;
    msg->message = new char[size];
    if(msg->message == NULL){
        delete msg;
        return OS_ERROR;
    }

    impl->id->enqueue(msg);
    impl->sem->release(1);

    return return_code;

} /* end OS_QueuePut_Impl */
