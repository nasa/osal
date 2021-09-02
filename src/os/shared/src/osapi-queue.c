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
 * \file     osapi-queue.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         This code only uses very basic C library calls that are expected
 *         to be available on every sane C-language compiler.  For everything else,
 *         a platform-specific implementation function is used.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-queue.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevant OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_QUEUES) || (OS_MAX_QUEUES <= 0)
#error "osconfig.h must define OS_MAX_QUEUES to a valid value"
#endif

enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_QUEUES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_QUEUE
};

OS_queue_internal_record_t OS_queue_table[LOCAL_NUM_OBJECTS];

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_QueueAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueAPI_Init(void)
{
    memset(OS_queue_table, 0, sizeof(OS_queue_table));
    return OS_SUCCESS;
} /* end OS_QueueAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueCreate(osal_id_t *queue_id, const char *queue_name, osal_blockcount_t queue_depth, size_t data_size,
                     uint32 flags)
{
    int32                       return_code;
    OS_object_token_t           token;
    OS_queue_internal_record_t *queue;

    /* validate inputs */
    OS_CHECK_POINTER(queue_id);
    OS_CHECK_APINAME(queue_name);
    OS_CHECK_SIZE(data_size);
    ARGCHECK(queue_depth <= OS_QUEUE_MAX_DEPTH, OS_QUEUE_INVALID_SIZE);

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, queue_name, &token);
    if (return_code == OS_SUCCESS)
    {
        queue = OS_OBJECT_TABLE_GET(OS_queue_table, token);

        /* Reset the table entry and save the name */
        OS_OBJECT_INIT(token, queue, queue_name, queue_name);

        queue->max_depth = queue_depth;
        queue->max_size  = data_size;

        /* Now call the OS-specific implementation.  This reads info from the queue table. */
        return_code = OS_QueueCreate_Impl(&token, flags);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, queue_id);
    }

    return return_code;

} /* end OS_QueueCreate */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueDelete(osal_id_t queue_id)
{
    OS_object_token_t token;
    int32             return_code;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, queue_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_QueueDelete_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;

} /* end OS_QueueDelete */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueGet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGet(osal_id_t queue_id, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    OS_object_token_t           token;
    int32                       return_code;
    OS_queue_internal_record_t *queue;

    /* Check Parameters */
    OS_CHECK_POINTER(data);
    OS_CHECK_POINTER(size_copied);
    OS_CHECK_SIZE(size);

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, queue_id, &token);
    if (return_code == OS_SUCCESS)
    {
        queue = OS_OBJECT_TABLE_GET(OS_queue_table, token);

        if (size < queue->max_size)
        {
            /*
            ** The buffer that the user is passing in is potentially too small
            */
            *size_copied = 0;
            return_code  = OS_QUEUE_INVALID_SIZE;
        }
        else
        {
            return_code = OS_QueueGet_Impl(&token, data, size, size_copied, timeout);
        }
    }

    return return_code;
} /* end OS_QueueGet */

/*----------------------------------------------------------------
 *
 * Function: OS_QueuePut
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueuePut(osal_id_t queue_id, const void *data, size_t size, uint32 flags)
{
    OS_object_token_t           token;
    int32                       return_code;
    OS_queue_internal_record_t *queue;

    /* Check Parameters */
    OS_CHECK_POINTER(data);
    OS_CHECK_SIZE(size);

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, queue_id, &token);
    if (return_code == OS_SUCCESS)
    {
        queue = OS_OBJECT_TABLE_GET(OS_queue_table, token);

        if (size > queue->max_size)
        {
            /*
            ** The buffer that the user is passing in is too large
            */
            return_code = OS_QUEUE_INVALID_SIZE;
        }
        else
        {
            return_code = OS_QueuePut_Impl(&token, data, size, flags);
        }
    }

    return return_code;
} /* end OS_QueuePut */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetIdByName(osal_id_t *queue_id, const char *queue_name)
{
    int32 return_code;

    /* Check Parameters */
    OS_CHECK_POINTER(queue_id);
    OS_CHECK_POINTER(queue_name);

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, queue_name, queue_id);

    return return_code;

} /* end OS_QueueGetIdByName */

/*----------------------------------------------------------------
 *
 * Function: OS_QueueGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetInfo(osal_id_t queue_id, OS_queue_prop_t *queue_prop)
{
    OS_common_record_t *record;
    int32               return_code;
    OS_object_token_t   token;

    /* Check parameters */
    OS_CHECK_POINTER(queue_prop);

    memset(queue_prop, 0, sizeof(OS_queue_prop_t));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, queue_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_queue_table, token);

        strncpy(queue_prop->name, record->name_entry, sizeof(queue_prop->name) - 1);
        queue_prop->creator = record->creator;

        /*
         * Currently there are no additional queue details provided by the impl layer -
         * But this could be added in the future (i.e. current/max depth, msg size, etc)
         */

        OS_ObjectIdRelease(&token);
    }

    return return_code;

} /* end OS_QueueGetInfo */
