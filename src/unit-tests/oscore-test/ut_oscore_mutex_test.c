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
** File:  ut_oscore_mutex_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_mutex_test.h"

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
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemCreate
** Purpose: Creates a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free mutex ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_create_test()
{
    int       i;
    osal_id_t mut_sem_id  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t mut_sem_id2 = OS_OBJECT_ID_UNDEFINED;
    char      sem_name[UT_OS_NAME_BUFF_SIZE];
    char      long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg-1 */

    UT_RETVAL(OS_MutSemCreate(NULL, "MutSem1", 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Null-pointer-arg-2 */

    UT_RETVAL(OS_MutSemCreate(&mut_sem_id, NULL, 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name) - 1] = '\0';
    UT_RETVAL(OS_MutSemCreate(&mut_sem_id, long_sem_name, 0), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 No-free-IDs */

    /* Setup */
    for (i = 0; i <= OS_MAX_MUTEXES; i++)
    {
        memset(sem_name, '\0', sizeof(sem_name));
        UT_os_sprintf(sem_name, "MUTSEM%d", i);
        if (i == OS_MAX_MUTEXES)
        {
            UT_RETVAL(OS_MutSemCreate(&mut_sem_id, "OneTooMany", 0), OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_MutSemCreate(&mut_sem_id, sem_name, 0)))
        {
            break;
        }
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    /* #5 Duplicate-name */

    /* Setup */
    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id2, "DUPLICATE", 0)))
    {
        UT_RETVAL(OS_MutSemCreate(&mut_sem_id, "DUPLICATE", 0), OS_ERR_NAME_TAKEN);

        /* Reset test environment */
        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id2));
    }

    /*-----------------------------------------------------*/
    /* #7 Nominal */

    UT_NOMINAL(OS_MutSemCreate(&mut_sem_id, "Good", 0));

    /* Reset test environment */
    UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemDelete
** Purpose: Deletes a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_ERR_SEM_NOT_FULL if the mutex is empty
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_delete_test()
{
    osal_id_t mut_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_MutSemDelete(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_MutSemDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "DeleteTest", 0)))
    {
        UT_NOMINAL(OS_MutSemDelete(mut_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGive
** Purpose: Releases a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_SEM_FAILURE if the mutex was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_give_test()
{
    osal_id_t mut_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_MutSemGive(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_MutSemGive(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "GiveTest", 0)))
    {
        if (UT_SETUP(OS_MutSemTake(mut_sem_id)))
        {
            UT_NOMINAL(OS_MutSemGive(mut_sem_id));
        }

        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemTake
** Purpose: Allocates a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_SEM_FAILURE if the mutex was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_take_test()
{
    osal_id_t mut_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_MutSemTake(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_MutSemTake(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "TakeTest", 0)))
    {
        UT_NOMINAL(OS_MutSemTake(mut_sem_id));

        UT_TEARDOWN(OS_MutSemGive(mut_sem_id));
        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGetIdByName
** Purpose: Returns the id of a given mutex semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name was not found in the mutex semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_get_id_by_name_test()
{
    osal_id_t mut_sem_id = OS_OBJECT_ID_UNDEFINED;
    char      long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg-1 */

    UT_RETVAL(OS_MutSemGetIdByName(NULL, "Name"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-2 */

    UT_RETVAL(OS_MutSemGetIdByName(&mut_sem_id, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name) - 1] = '\0';
    UT_RETVAL(OS_MutSemGetIdByName(&mut_sem_id, long_sem_name), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-not-found */

    UT_RETVAL(OS_MutSemGetIdByName(&mut_sem_id, "NotFound"), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "GetIDByName", 0)))
    {
        UT_NOMINAL(OS_MutSemGetIdByName(&mut_sem_id, "GetIDByName"));
        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGetInfo
** Purpose: Returns mutex semaphore information about a given mutex id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_get_info_test()
{
    osal_id_t         mut_sem_id = OS_OBJECT_ID_UNDEFINED;
    OS_mut_sem_prop_t mut_sem_prop;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_MutSemGetInfo(UT_OBJID_INCORRECT, &mut_sem_prop), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_MutSemGetInfo(OS_OBJECT_ID_UNDEFINED, &mut_sem_prop), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "InvalidPtr", 0)))
    {
        UT_RETVAL(OS_MutSemGetInfo(mut_sem_id, NULL), OS_INVALID_POINTER);

        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
    }

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_MutSemCreate(&mut_sem_id, "GetInfo", 0)))
    {
        UT_NOMINAL(OS_MutSemGetInfo(mut_sem_id, &mut_sem_prop));

        UT_TEARDOWN(OS_MutSemDelete(mut_sem_id));
    }
}

/*================================================================================*
** End of File: ut_oscore_mutex_test.c
**================================================================================*/
