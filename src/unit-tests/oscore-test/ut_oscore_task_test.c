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
** File:  ut_oscore_task_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_task_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_TASK_STACK_SIZE 0x2000
#define UT_TASK_PRIORITY   111

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern char *g_task_names[UT_OS_TASK_LIST_LEN];
extern char  g_long_task_name[UT_OS_NAME_BUFF_SIZE];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

uint32    g_task_result = 0;
bool      g_task_handler_called;
osal_id_t g_task_sync_sem;
osal_id_t g_task_ids[UT_OS_TASK_LIST_LEN];
osal_id_t g_task_get_id_result;
struct
{
    uint32 words[UT_TASK_STACK_SIZE];
} g_task_stacks[UT_OS_TASK_LIST_LEN];

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

void generic_test_task(void)
{
    osal_id_t      task_id;
    OS_task_prop_t task_prop;

    task_id = OS_TaskGetId();
    OS_TaskGetInfo(task_id, &task_prop);

    UtPrintf("Starting GenericTask: %s, id: %lx\n", task_prop.name, OS_ObjectIdToInteger(task_id));

    while (1)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskCreate
** Purpose: Creates a task
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_INVALID_PRIORITY if the priority passed in is bad
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there no more task can be created
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_create_test()
{
    int32 i = 0;
    char  task_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg-1 */

    UT_RETVAL(OS_TaskCreate(NULL, g_task_names[1], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                            sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
              OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Null-pointer-arg-2 */

    UT_RETVAL(OS_TaskCreate(&g_task_ids[2], NULL, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                            sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
              OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Null-pointer-arg-3 */

    UT_RETVAL(OS_TaskCreate(&g_task_ids[3], g_task_names[3], NULL, OSAL_STACKPTR_C(&g_task_stacks[3]),
                            sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
              OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* Bad stack size */

    UT_RETVAL(OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[3]), 0,
                            OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
              OS_ERR_INVALID_SIZE);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long */

    UT_RETVAL(OS_TaskCreate(&g_task_ids[4], g_long_task_name, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[4]),
                            sizeof(g_task_stacks[4]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
              OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #6 No-free-IDs */

    for (i = 0; i <= OS_MAX_TASKS; i++)
    {
        memset(task_name, '\0', sizeof(task_name));
        UT_os_sprintf(task_name, "CREATE_TASK%d", (int)i);
        if (i == OS_MAX_TASKS)
        {
            UT_RETVAL(OS_TaskCreate(&g_task_ids[i], task_name, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[i]),
                                    sizeof(g_task_stacks[i]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
                      OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_TaskCreate(&g_task_ids[i], task_name, generic_test_task,
                                         OSAL_STACKPTR_C(&g_task_stacks[i]), sizeof(g_task_stacks[i]),
                                         OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
        {
            break;
        }

        /* Delay to let child task run */
        OS_TaskDelay(200);
    }

    /* Reset test environment */
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[i]));
    }

    /*-----------------------------------------------------*/
    /* #7 Duplicate-name */

    if (UT_SETUP(OS_TaskCreate(&g_task_ids[7], g_task_names[7], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[7]),
                               sizeof(g_task_stacks[7]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        UT_RETVAL(OS_TaskCreate(&g_task_ids[8], g_task_names[7], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[8]),
                                sizeof(g_task_stacks[8]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0),
                  OS_ERR_NAME_TAKEN);

        /* Delay to let child task run */
        OS_TaskDelay(200);

        /* Reset test environment */
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[7]));
    }

    /*-----------------------------------------------------*/
    /* Nominal, fixed stack */

    UT_NOMINAL(OS_TaskCreate(&g_task_ids[9], g_task_names[9], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[9]),
                             sizeof(g_task_stacks[9]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0));

    /*-----------------------------------------------------*/
    /* Nominal, dynamic stack */

    UT_NOMINAL(OS_TaskCreate(&g_task_ids[8], g_task_names[8], generic_test_task, NULL, sizeof(g_task_stacks[8]),
                             OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0));

    /* Delay to let child task run */
    OS_TaskDelay(200);

    /* Reset test environment */
    UT_TEARDOWN(OS_TaskDelete(g_task_ids[8]));
    UT_TEARDOWN(OS_TaskDelete(g_task_ids[9]));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskDelete
** Purpose: Deletes a task
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_delete_test()
{
    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_TaskDelete(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TaskDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[3]),
                               sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        /* Delay to let child task run */
        OS_TaskDelay(200);

        UT_NOMINAL(OS_TaskDelete(g_task_ids[3]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskInstallDeleteHandler
** Purpose: Installs a callback function that is called when the task is deleted
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void delete_handler_callback(void)
{
    UtPrintf("Task delete callback...\n");
    g_task_handler_called = true;
}

/*--------------------------------------------------------------------------------*/

void delete_handler_test_task(void)
{
    osal_id_t      task_id;
    OS_task_prop_t task_prop;

    task_id = OS_TaskGetId();
    OS_TaskGetInfo(task_id, &task_prop);

    UtPrintf("Starting DeleteTest Task: %s, id: %lx\n", task_prop.name, OS_ObjectIdToInteger(task_id));

    g_task_result = OS_TaskInstallDeleteHandler(&delete_handler_callback);

    /*
    ** Release the semaphore so the main function can record the results of the test
    ** and clean up
    */
    OS_BinSemGive(g_task_sync_sem);

    for (;;)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*/

void UT_os_task_install_delete_handler_test(void)
{
    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    /*
    ** This test works because it is being called from the main task
    **  which should not be an official OSAL task
    */
    UT_RETVAL(OS_TaskInstallDeleteHandler(&delete_handler_callback), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Nominal */

    if (UT_SETUP(OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0)))
    {
        OS_BinSemTake(g_task_sync_sem);

        g_task_handler_called = false;

        if (UT_SETUP(OS_TaskCreate(&g_task_ids[2], g_task_names[2], delete_handler_test_task,
                                   OSAL_STACKPTR_C(&g_task_stacks[2]), sizeof(g_task_stacks[2]),
                                   OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
        {
            /* Wait for the task to finish the test */
            UT_SETUP(OS_BinSemTake(g_task_sync_sem));

            /* Delay to let child task run */
            OS_TaskDelay(500);

            UT_TEARDOWN(OS_TaskDelete(g_task_ids[2]));

            UtAssert_True(g_task_result == OS_SUCCESS, "OS_TaskInstallDeleteHandler() (%d) == OS_SUCCESS",
                          (int)g_task_result);
            UtAssert_True(g_task_handler_called, "OS_TaskInstallDeleteHandler() callback invoked");
        }

        UT_TEARDOWN(OS_BinSemDelete(g_task_sync_sem));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskExit
** Purpose: Allows a task to delete itself
** Parameters: To-be-filled-in
** Returns: Nothing
**--------------------------------------------------------------------------------*/
void exit_test_task(void)
{
    osal_id_t      task_id;
    OS_task_prop_t task_prop;

    task_id = OS_TaskGetId();
    OS_TaskGetInfo(task_id, &task_prop);

    UtPrintf("Starting ExitTest Task: %s, id: %lx\n", task_prop.name, OS_ObjectIdToInteger(task_id));

    /*
    ** The parent task will check to see if this task is valid.
    ** As long as it has not successfully called OS_TaskExit, it should
    ** be valid.
    */
    while (1)
    {
        OS_BinSemGive(g_task_sync_sem);

        OS_TaskExit();
    }
}

/*--------------------------------------------------------------------------------*/

void UT_os_task_exit_test(void)
{
    OS_task_prop_t task_prop;

    /*-----------------------------------------------------*/
    /* #1 Nominal */

    if (UT_SETUP(OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0)))
    {
        UT_SETUP(OS_BinSemTake(g_task_sync_sem));

        if (UT_SETUP(OS_TaskCreate(&g_task_ids[1], g_task_names[1], exit_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                                   sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
        {
            /* Wait for the task to finish the test */
            UT_SETUP(OS_BinSemTake(g_task_sync_sem));

            /* Delay to let the child task run */
            OS_TaskDelay(500);

            /*
            ** The only real way to tell if TaskExit ran is to check to see if the
            ** task ID is valid. It should not be valid
            */
            UT_RETVAL(OS_TaskGetInfo(g_task_ids[1], &task_prop), OS_ERR_INVALID_ID);
        }

        UT_TEARDOWN(OS_BinSemDelete(g_task_sync_sem));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskDelay
** Purpose: Suspends a given task for the given period of milliseconds
** Parameters: To-be-filled-in
** Returns: OS_ERROR if sleep failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_delay_test()
{
    OS_time_t before_time;
    OS_time_t after_time;
    int64     elapsed;

    /* Initialize to avoid static analysis warnings */
    memset(&before_time, 0, sizeof(before_time));
    memset(&after_time, 0, sizeof(after_time));

    /*
     * Note, if running under a VM/hypervisor, the real time clock may not
     * be very precise, depending on its implementation.  Therefore the allowed
     * ranges are slightly extended here.
     */

    /*-----------------------------------------------------*/
    /* Nominal, 100ms delay */
    UT_SETUP(OS_GetLocalTime(&before_time));
    UT_NOMINAL(OS_TaskDelay(100));
    UT_SETUP(OS_GetLocalTime(&after_time));

    elapsed = OS_TimeGetTotalMilliseconds(OS_TimeSubtract(after_time, before_time));
    UtAssert_True(elapsed >= 95, "Elapsed time %ld msec, expected 100", (long)elapsed);

    /*-----------------------------------------------------*/
    /* Nominal, 250ms delay */
    UT_SETUP(OS_GetLocalTime(&before_time));
    UT_NOMINAL(OS_TaskDelay(250));
    UT_SETUP(OS_GetLocalTime(&after_time));

    elapsed = OS_TimeGetTotalMilliseconds(OS_TimeSubtract(after_time, before_time));
    UtAssert_True(elapsed >= 245, "Elapsed time %ld msec, expected 250", (long)elapsed);
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskSetPriority
** Purpose: Sets task priority for the given task id
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_ERR_INVALID_PRIORITY if the priority passed in is greater than the max allowed
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_set_priority_test()
{
    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_TaskSetPriority(UT_OBJID_INCORRECT, OSAL_PRIORITY_C(100)), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TaskSetPriority(OS_OBJECT_ID_UNDEFINED, OSAL_PRIORITY_C(100)), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_TaskCreate(&g_task_ids[4], g_task_names[4], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[4]),
                               sizeof(g_task_stacks[4]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        UT_NOMINAL(OS_TaskSetPriority(g_task_ids[4], OSAL_PRIORITY_C(UT_TASK_PRIORITY - 10)));

        /* Delay to let child task run */
        OS_TaskDelay(500);

        UT_NOMINAL(OS_TaskSetPriority(g_task_ids[4], OSAL_PRIORITY_C(UT_TASK_PRIORITY)));

        /* Reset test environment */
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[4]));
    }
}

/*--------------------------------------------------------------------------------*/

void getid_test_task(void)
{
    OS_task_prop_t task_prop;

    g_task_get_id_result = OS_TaskGetId();
    OS_TaskGetInfo(g_task_get_id_result, &task_prop);

    while (1)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetId
** Purpose: Returns the task id for the task that calls this routine
** Parameters: To-be-filled-in
** Returns: Task id of the task that calls this routine
**--------------------------------------------------------------------------------*/
void UT_os_task_get_id_test()
{
    /*
     * Note this function does not return a normal status code,
     * there is no provision to return/check for OS_ERR_NOT_IMPLEMENTED.
     */

    /*-----------------------------------------------------*/
    /* #1 Nominal */

    if (UT_SETUP(OS_TaskCreate(&g_task_ids[1], g_task_names[1], getid_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                               sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        OS_TaskDelay(500);

        UtAssert_True(OS_ObjectIdEqual(g_task_get_id_result, g_task_ids[1]), "OS_TaskGetId() (%lu) == %lu",
                      OS_ObjectIdToInteger(g_task_get_id_result), OS_ObjectIdToInteger(g_task_ids[1]));

        UT_TEARDOWN(OS_TaskDelete(g_task_ids[1]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetIdByName
** Purpose: Returns the task id of the given task name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in is not in the task table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_get_id_by_name_test()
{
    /*-----------------------------------------------------*/
    /* #1 Invalid-pointer-arg-1 */

    UT_RETVAL(OS_TaskGetIdByName(NULL, "Name"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg-2 */

    UT_RETVAL(OS_TaskGetIdByName(&g_task_ids[2], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Name-too-long */

    UT_RETVAL(OS_TaskGetIdByName(&g_task_ids[3], g_long_task_name), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-not-found */

    UT_RETVAL(OS_TaskGetIdByName(&g_task_ids[4], "NotFound"), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    /* Setup */
    if (UT_SETUP(OS_TaskCreate(&g_task_ids[5], g_task_names[5], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[5]),
                               sizeof(g_task_stacks[5]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        UT_NOMINAL(OS_TaskGetIdByName(&g_task_ids[6], g_task_names[5]));

        UtAssert_True(OS_ObjectIdEqual(g_task_ids[5], g_task_ids[6]), "OS_TaskGetIdByName() ID (%lu) == %lu",
                      OS_ObjectIdToInteger(g_task_ids[5]), OS_ObjectIdToInteger(g_task_ids[6]));

        OS_TaskDelay(500); /* Delay to let task run */
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[5]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetInfo
** Purpose: Returns task information about the given task id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_get_info_test()
{
    OS_task_prop_t task_prop;

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_TaskGetInfo(UT_OBJID_INCORRECT, &task_prop), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TaskGetInfo(OS_OBJECT_ID_UNDEFINED, &task_prop), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-pointer-arg */

    /* Setup */
    if (UT_SETUP(OS_TaskCreate(&g_task_ids[2], g_task_names[2], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                               sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        UT_RETVAL(OS_TaskGetInfo(g_task_ids[2], NULL), OS_INVALID_POINTER);

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[2]));
    }

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[3]),
                               sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0)))
    {
        UT_NOMINAL(OS_TaskGetInfo(g_task_ids[3], &task_prop));

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        UT_TEARDOWN(OS_TaskDelete(g_task_ids[3]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskFindIdBySystemData
** Purpose: Finds the abstract OSAL task ID from the system ID data
**--------------------------------------------------------------------------------*/
void UT_os_task_getid_by_sysdata_test()
{
    uint8     sysdata = 0;
    osal_id_t task_id;

    /*
     * NOTE: OSAL does not provide a means to get the low level system ID data directly.
     * This API is intended to aid in exception processing in a PSP/BSP, where the
     * low level task information is obtained outside of OSAL in a platform-specific
     * manner.
     *
     * As a result this cannot check for nominal conditions, only validate the error checking.
     */
    UT_RETVAL(OS_TaskFindIdBySystemData(NULL, &sysdata, sizeof(sysdata)), OS_INVALID_POINTER);
    UT_RETVAL(OS_TaskFindIdBySystemData(&task_id, NULL, sizeof(sysdata)), OS_INVALID_POINTER);
    UT_RETVAL(OS_TaskFindIdBySystemData(&task_id, &sysdata, 0), OS_INVALID_POINTER);
}

/*================================================================================*
** End of File: ut_oscore_task_test.c
**================================================================================*/
