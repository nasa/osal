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
** File:  ut_oscore_countsem_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_countsem_test.h"

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
** Syntax: OS_CountSemCreate
** Purpose: Creates a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free counting semaphore ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_create_test()
{
    int       i;
    osal_id_t count_sem_ids[OS_MAX_COUNT_SEMAPHORES + 1];
    char      sem_name[UT_OS_NAME_BUFF_SIZE];
    char      long_sem_name[UT_OS_NAME_BUFF_SIZE];

    memset(count_sem_ids, 0, sizeof(count_sem_ids));

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg-1 */

    UT_RETVAL(OS_CountSemCreate(NULL, "CountSem1", 1, 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Null-pointer-arg-2 */

    UT_RETVAL(OS_CountSemCreate(&count_sem_ids[0], NULL, 1, 0), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name) - 1] = '\0';
    UT_RETVAL(OS_CountSemCreate(&count_sem_ids[0], long_sem_name, 1, 0), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Initial-count-too-high */

    /*
     * The intent with this test case is to call OS_CountSemCreate() with an initial
     * value greater than SEM_VALUE_MAX and confirm it returns OS_INVALID_SEM_VALUE.
     *
     * However, none of the currently available test platforms are able to produce
     * this condition, because SEM_VALUE_MAX is either not defined/exposed or it
     * is equal to UINT32_MAX and thus impossible to pass a value greater than this.
     *
     * Therefore a placeholder is here in case a platform in the future does permit
     * it to be tested.  Note that the check and return value is still tested in the
     * coverage test for this function.
     */
    UtAssert_NA("#4 Initial-count-too-high");

    /*-----------------------------------------------------*/
    /* #5 No-free-IDs */

    /* Setup */
    for (i = 0; i <= OS_MAX_COUNT_SEMAPHORES; i++)
    {
        memset(sem_name, '\0', sizeof(sem_name));
        UT_os_sprintf(sem_name, "COUNTSEM%d", i);

        if (i == OS_MAX_COUNT_SEMAPHORES)
        {
            UT_RETVAL(OS_CountSemCreate(&count_sem_ids[i], "OneTooMany", 1, 0), OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_CountSemCreate(&count_sem_ids[i], sem_name, 1, 0)))
        {
            break;
        }
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    /* #6 Duplicate-name */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_ids[0], "DUPLICATE", 1, 0)))
    {
        UT_RETVAL(OS_CountSemCreate(&count_sem_ids[1], "DUPLICATE", 1, 0), OS_ERR_NAME_TAKEN);

        /* Reset test environment */
        UT_TEARDOWN(OS_CountSemDelete(count_sem_ids[0]));
    }

    /*-----------------------------------------------------*/
    /* #8 Nominal */

    UT_NOMINAL(OS_CountSemCreate(&count_sem_ids[0], "Good", 1, 0));

    /* Reset test environment */
    UT_TEARDOWN(OS_CountSemDelete(count_sem_ids[0]));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemDelete
** Purpose: Deletes a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_delete_test()
{
    osal_id_t count_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_CountSemDelete(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_CountSemDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "DeleteTest", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemDelete(count_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGive
** Purpose: Gives back a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_give_test()
{
    osal_id_t count_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_CountSemGive(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_CountSemGive(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "GiveTest", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemGive(count_sem_id));

        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemTake
** Purpose: Reserves a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_take_test()
{
    osal_id_t count_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_CountSemTake(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_CountSemTake(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "TakeTest", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemTake(count_sem_id));
        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemTimedWait
** Purpose: Reserves a counting semaphore with a timeout
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SEM_TIMEOUT if the semaphore was not relinquished in time
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_timed_wait_test()
{
    osal_id_t count_sem_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_CountSemTimedWait(UT_OBJID_INCORRECT, 1000), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_CountSemTimedWait(OS_OBJECT_ID_UNDEFINED, 1000), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Sem-take-timed-out */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "TimedWait", 1, 0)))
    {
        if (UT_SETUP(OS_CountSemTake(count_sem_id)))
        {
            UT_RETVAL(OS_CountSemTimedWait(count_sem_id, 1000), OS_SEM_TIMEOUT);
        }
        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "TimedWait", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemTimedWait(count_sem_id, 1000));
        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGetIdByName
** Purpose: Returns the id of a given counting semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in was not found in the counting
**                                semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_get_id_by_name_test()
{
    osal_id_t count_sem_id = OS_OBJECT_ID_UNDEFINED;
    char      long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg-1 */

    UT_RETVAL(OS_CountSemGetIdByName(NULL, "Name"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-2 */

    UT_RETVAL(OS_CountSemGetIdByName(&count_sem_id, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name) - 1] = '\0';
    UT_RETVAL(OS_CountSemGetIdByName(&count_sem_id, long_sem_name), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-not-found */

    UT_RETVAL(OS_CountSemGetIdByName(&count_sem_id, "NotFound"), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "GetIDByName", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemGetIdByName(&count_sem_id, "GetIDByName"));

        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGetInfo
** Purpose: Returns semaphore information about the given binary semaphore id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_get_info_test()
{
    osal_id_t           count_sem_id = OS_OBJECT_ID_UNDEFINED;
    OS_count_sem_prop_t count_sem_prop;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_CountSemGetInfo(UT_OBJID_INCORRECT, &count_sem_prop), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_CountSemGetInfo(OS_OBJECT_ID_UNDEFINED, &count_sem_prop), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg */

    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0)))
    {
        UT_RETVAL(OS_CountSemGetInfo(count_sem_id, NULL), OS_INVALID_POINTER);

        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0)))
    {
        UT_NOMINAL(OS_CountSemGetInfo(count_sem_id, &count_sem_prop));

        UT_TEARDOWN(OS_CountSemDelete(count_sem_id));
    }
}

/*================================================================================*
** End of File: ut_oscore_countsem_test.c
**================================================================================*/
