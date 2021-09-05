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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-impl-queues.h"

#include "os-shared-queue.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_queue_internal_record_t OS_impl_queue_table[OS_MAX_QUEUES];

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_QueueAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_QueueAPI_Impl_Init(void)
{
    memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));
    return (OS_SUCCESS);
} /* end OS_Rtems_QueueAPI_Impl_Init */

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
    rtems_status_code                status;
    rtems_name                       r_name;
    OS_impl_queue_internal_record_t *impl;
    OS_queue_internal_record_t *     queue;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    queue = OS_OBJECT_TABLE_GET(OS_queue_table, *token);

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL queue ID in here, as we know it is already unique
    ** and trying to use the real queue name would be less than useful (only 4 chars)
    */
    r_name = OS_ObjectIdToInteger(OS_ObjectIdFromToken(token));

    /*
    ** Create the message queue.
    ** The queue attributes are set to default values; the waiting order
    ** (RTEMS_FIFO or RTEMS_PRIORITY) is irrelevant since only one task waits
    ** on each queue.
    */
    status = rtems_message_queue_create(r_name,           /* 32-bit RTEMS object name; not used */
                                        queue->max_depth, /* maximum number of messages in queue (queue depth) */
                                        queue->max_size,  /* maximum size in bytes of a message */
                                        RTEMS_FIFO | RTEMS_LOCAL, /* attributes (default) */
                                        &(impl->id)               /* object ID returned for queue */
    );

    /*
    ** If the operation failed, report the error
    */
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled queue_create error: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

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
    rtems_status_code                status;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    /* Try to delete the queue */
    status = rtems_message_queue_delete(impl->id);
    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled queue_delete error: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

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
    rtems_status_code                status;
    rtems_interval                   ticks;
    int                              tick_count;
    rtems_option                     option_set;
    size_t                           rtems_size;
    rtems_id                         rtems_queue_id;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    rtems_queue_id = impl->id;

    /* Get Message From Message Queue */
    if (timeout == OS_PEND)
    {
        option_set = RTEMS_WAIT;
        ticks      = RTEMS_NO_TIMEOUT;
    }
    else if (timeout == OS_CHECK)
    {
        option_set = RTEMS_NO_WAIT;
        ticks      = RTEMS_NO_TIMEOUT;
    }
    else
    {
        option_set = RTEMS_WAIT;

        /* msecs rounded to the closest system tick count */
        if (OS_Milli2Ticks(timeout, &tick_count) != OS_SUCCESS)
        {
            return OS_ERROR;
        }

        ticks = (rtems_interval)tick_count;
    }

    /*
     ** Pend until a message arrives.
     */
    status = rtems_message_queue_receive(rtems_queue_id, /* message queue descriptor */
                                         data,           /* pointer to message buffer */
                                         &rtems_size,    /* returned size of message */
                                         option_set,     /* wait option */
                                         ticks           /* timeout */
    );

    if (status == RTEMS_SUCCESSFUL)
    {
        return_code = OS_SUCCESS;
    }
    else if (status == RTEMS_TIMEOUT)
    {
        return_code = OS_QUEUE_TIMEOUT;
    }
    else if (status == RTEMS_UNSATISFIED)
    {
        return_code = OS_QUEUE_EMPTY;
    }
    else
    {
        /* Something else went wrong */
        return_code = OS_ERROR;
        OS_DEBUG("Unhandled queue_receive error: %s\n", rtems_status_text(status));
    }

    /*
    ** Check the size of the message.  If a valid message was
    ** obtained, indicate success.
    */
    if (status == RTEMS_SUCCESSFUL)
    {
        *size_copied = rtems_size;
        if (rtems_size != size)
        {
            /* Success, but the size was wrong */
            return_code = OS_QUEUE_INVALID_SIZE;
        }
    }
    else
    {
        *size_copied = 0;
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
    rtems_status_code                status;
    rtems_id                         rtems_queue_id;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    rtems_queue_id = impl->id;

    /* Write the buffer pointer to the queue.  If an error occurred, report it
    ** with the corresponding SB status code.
    */
    status = rtems_message_queue_send(rtems_queue_id, /* message queue descriptor */
                                      data,           /* pointer to message */
                                      size            /* length of message */
    );

    if (status == RTEMS_TOO_MANY)
    {
        /*
        ** Queue is full.
        */
        return OS_QUEUE_FULL;
    }

    if (status != RTEMS_SUCCESSFUL)
    {
        /*
        ** Unexpected error while writing to queue.
        */
        OS_DEBUG("Unhandled queue_send error: %s\n", rtems_status_text(status));
        return OS_ERROR;
    }

    return OS_SUCCESS;

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
