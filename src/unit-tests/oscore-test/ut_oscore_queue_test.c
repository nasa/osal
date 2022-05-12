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

/*================================================================================*
** File:  ut_oscore_queue_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_queue_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueCreate
** Purpose: Creates a queue in the OS
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name has already been used on a created queue
**          OS_ERR_NO_FREE_IDS if there are no more free queue ids
**          OS_ERROR if the OS create call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_create_test()
{
    int       i         = 0;
    osal_id_t queue_id  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t queue_id2 = OS_OBJECT_ID_UNDEFINED;
    char      queue_name[UT_OS_NAME_BUFF_SIZE];
    char      long_queue_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg-1 */

    UT_RETVAL(OS_QueueCreate(NULL, "Queue1", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Null-pointer-arg-2 */

    UT_RETVAL(OS_QueueCreate(&queue_id, NULL, OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/

    /* Invalid item size */
    UT_RETVAL(OS_QueueCreate(&queue_id, "Queue1", OSAL_BLOCKCOUNT_C(10), 0, 0), OS_ERR_INVALID_SIZE);
    /* Invalid depth */
    UT_RETVAL(OS_QueueCreate(&queue_id, "Queue1", OSAL_BLOCKCOUNT_C(OS_QUEUE_MAX_DEPTH + 1), sizeof(uint32), 0),
              OS_QUEUE_INVALID_SIZE);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_queue_name, 'X', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name) - 1] = '\0';
    UT_RETVAL(OS_QueueCreate(&queue_id, long_queue_name, OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0),
              OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 No-free-IDs */

    /* Setup */
    for (i = 0; i <= OS_MAX_QUEUES; i++)
    {
        memset(queue_name, '\0', sizeof(queue_name));
        UT_os_sprintf(queue_name, "QUEUE%d", i);
        if (i == OS_MAX_QUEUES)
        {
            UT_RETVAL(OS_QueueCreate(&queue_id, "OneTooMany", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0),
                      OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_QueueCreate(&queue_id, queue_name, OSAL_BLOCKCOUNT_C(2), sizeof(uint32), 0)))
        {
            break;
        }
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    /* #5 Duplicate-name */

    if (UT_SETUP(OS_QueueCreate(&queue_id2, "DUPLICATE", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueueCreate(&queue_id, "DUPLICATE", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0), OS_ERR_NAME_TAKEN);

        /* Reset test environment */
        UT_TEARDOWN(OS_QueueDelete(queue_id2));
    }

    /*-----------------------------------------------------*/
    /* #7 Nominal */

    UT_NOMINAL(OS_QueueCreate(&queue_id, "Good", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0));

    /* Reset test environment */
    UT_TEARDOWN(OS_QueueDelete(queue_id));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueDelete
** Purpose: Deletes a queue in the OS
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_ERROR if the OS call to delete the queue failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_delete_test()
{
    osal_id_t queue_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_QueueDelete(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_QueueDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "DeleteTest", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_NOMINAL(OS_QueueDelete(queue_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGet
** Purpose: Retrieves a data item from an existing queue
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_QUEUE_EMPTY if the queue has no message on it to be received
**          OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
**          OS_QUEUE_INVALID_SIZE if the size copied from the queue is incorrect
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_test()
{
    osal_id_t queue_id = OS_OBJECT_ID_UNDEFINED;
    uint32    queue_data_out;
    uint32    queue_data_in;
    size_t    size_copied;
    size_t    data_size;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_QueueGet(UT_OBJID_INCORRECT, (void *)&queue_data_in, sizeof(uint32), &size_copied, OS_CHECK),
              OS_ERR_INVALID_ID);
    UT_RETVAL(OS_QueueGet(OS_OBJECT_ID_UNDEFINED, (void *)&queue_data_in, sizeof(uint32), &size_copied, OS_CHECK),
              OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-1 */
    /* #3 Invalid-pointer-arg-2 */

    /* Setup */
    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueGet", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueueGet(queue_id, NULL, sizeof(uint32), &size_copied, OS_CHECK), OS_INVALID_POINTER);
        UT_RETVAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), NULL, OS_CHECK), OS_INVALID_POINTER);
        UT_RETVAL(OS_QueueGet(queue_id, &queue_data_in, 0, &size_copied, OS_CHECK), OS_ERR_INVALID_SIZE);

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #4 Queue-empty */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueEmpty", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), &data_size, OS_CHECK), OS_QUEUE_EMPTY);

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #5 Queue-timed-out */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueTimeout", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), &data_size, 2), OS_QUEUE_TIMEOUT);

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #6 Invalid-queue-size */

    /* Setup */
    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueuePut", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        if (UT_SETUP(OS_QueuePut(queue_id, &queue_data_out, OSAL_SIZE_C(2), 0)))
        {
            UT_RETVAL(OS_QueueGet(queue_id, &queue_data_in, OSAL_SIZE_C(3), &data_size, OS_CHECK),
                      OS_QUEUE_INVALID_SIZE);
        }
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #8 Nominal Pend */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueGet", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        if (UT_SETUP(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0)))
        {
            UT_NOMINAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), &data_size, OS_PEND));
        }
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #9 Nominal timeout */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueGet", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        if (UT_SETUP(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0)))
        {
            UT_NOMINAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), &data_size, 20));
        }
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #10 Nominal check */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueGet", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        if (UT_SETUP(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0)))
        {
            UT_NOMINAL(OS_QueueGet(queue_id, &queue_data_in, sizeof(uint32), &data_size, OS_CHECK));
        }
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueuePut
** Purpose: Sends data on an existing queue
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_QUEUE_FULL if the queue could not accept another message
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_put_test()
{
    osal_id_t queue_id       = OS_OBJECT_ID_UNDEFINED;
    uint32    queue_data_out = 0;
    int       i;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_QueuePut(UT_OBJID_INCORRECT, (void *)&queue_data_out, sizeof(uint32), 0), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_QueuePut(OS_OBJECT_ID_UNDEFINED, (void *)&queue_data_out, sizeof(uint32), 0), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg */

    /* Setup */
    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueuePut", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueuePut(queue_id, NULL, sizeof(uint32), 0), OS_INVALID_POINTER);
        UT_RETVAL(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32) + 1, 0), OS_QUEUE_INVALID_SIZE);
        UT_RETVAL(OS_QueuePut(queue_id, &queue_data_out, 0, 0), OS_ERR_INVALID_SIZE);

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #4 Queue-full */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueuePut", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        for (i = 0; i <= 10; i++)
        {
            if (i == 10)
            {
                UT_RETVAL(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0), OS_QUEUE_FULL);
            }
            else if (!UT_SETUP(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0)))
            {
                break;
            }
        }

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "QueueGet", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        queue_data_out = 0x11223344;
        UT_NOMINAL(OS_QueuePut(queue_id, &queue_data_out, sizeof(uint32), 0));
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGetIdByName
** Purpose: Returns the queue id of a given queue name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the name of id pointers are null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name was not found in the queue table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_id_by_name_test()
{
    osal_id_t queue_id = OS_OBJECT_ID_UNDEFINED;
    char      long_queue_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg-1 */

    UT_RETVAL(OS_QueueGetIdByName(NULL, "Name"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-2 */

    UT_RETVAL(OS_QueueGetIdByName(&queue_id, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_queue_name, 'Y', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name) - 1] = '\0';
    UT_RETVAL(OS_QueueGetIdByName(&queue_id, long_queue_name), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-not-found */

    UT_RETVAL(OS_QueueGetIdByName(&queue_id, "NameNotFound"), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "GetIDByName", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_NOMINAL(OS_QueueGetIdByName(&queue_id, "GetIDByName"));

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGetInfo
** Purpose: Returns queue information about the given queue id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_info_test()
{
    osal_id_t       queue_id = OS_OBJECT_ID_UNDEFINED;
    OS_queue_prop_t queue_prop;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_QueueGetInfo(UT_OBJID_INCORRECT, &queue_prop), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_QueueGetInfo(OS_OBJECT_ID_UNDEFINED, &queue_prop), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "GetInfo", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_RETVAL(OS_QueueGetInfo(queue_id, NULL), OS_INVALID_POINTER);
        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_QueueCreate(&queue_id, "GetInfo", OSAL_BLOCKCOUNT_C(10), sizeof(uint32), 0)))
    {
        UT_NOMINAL(OS_QueueGetInfo(queue_id, &queue_prop));

        UT_TEARDOWN(OS_QueueDelete(queue_id));
    }
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
