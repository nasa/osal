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
osal_id_t g_task_sync_sem;
osal_id_t g_task_ids[UT_OS_TASK_LIST_LEN];
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
    int32       i = 0, res = 0;
    const char *testDesc;
    char        task_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskCreate(&g_task_ids[0], g_task_names[0], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[0]),
                        sizeof(g_task_stacks[0]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_create_test_exit_tag;
    }

    /* Delay to let child task run */
    OS_TaskDelay(200);

    /* Reset test environment */
    OS_TaskDelete(g_task_ids[0]);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_TaskCreate(NULL, g_task_names[1], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                        sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_TaskCreate(&g_task_ids[2], NULL, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                        sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Null-pointer-arg-3";

    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], NULL, OSAL_STACKPTR_C(&g_task_stacks[3]),
                        sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long";

    res = OS_TaskCreate(&g_task_ids[4], g_long_task_name, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[4]),
                        sizeof(g_task_stacks[4]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#6 No-free-IDs";

    /* Setup */
    for (i = 0; i <= OS_MAX_TASKS; i++)
    {
        memset(task_name, '\0', sizeof(task_name));
        UT_os_sprintf(task_name, "CREATE_TASK%d", (int)i);
        res = OS_TaskCreate(&g_task_ids[i], task_name, generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[i]),
                            sizeof(g_task_stacks[i]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
        if (res != OS_SUCCESS)
        {
            break;
        }
    }

    if ((res == OS_ERR_NO_FREE_IDS) && (i == OS_MAX_TASKS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Delay to let child tasks run */
    OS_TaskDelay(500);

    /* Reset test environment */
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        res = OS_TaskDelete(g_task_ids[i]); /* Ignore errors, does not matter here */
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 Duplicate-name";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[7], g_task_names[7], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[7]),
                        sizeof(g_task_stacks[7]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#7 Duplicate-name - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_TaskCreate(&g_task_ids[8], g_task_names[7], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[8]),
                            sizeof(g_task_stacks[8]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Delay to let child task run */
        OS_TaskDelay(200);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[7]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#8 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#9 Nominal";

    res = OS_TaskCreate(&g_task_ids[9], g_task_names[9], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[9]),
                        sizeof(g_task_stacks[9]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res == OS_SUCCESS)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Delay to let child task run */
    OS_TaskDelay(200);

    /* Reset test environment */
    res = OS_TaskDelete(g_task_ids[9]);

UT_os_task_create_test_exit_tag:
    return;
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
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskDelete(UT_OBJID_INCORRECT);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskDelete(UT_OBJID_INCORRECT);
    if (res == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[3]),
                        sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        /* Delay to let child task run */
        OS_TaskDelay(200);

        res = OS_TaskDelete(g_task_ids[3]);
        if (res == OS_SUCCESS)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_task_delete_test_exit_tag:
    return;
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
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskInstallDeleteHandler(&delete_handler_callback);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_install_delete_handler_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    /*
    ** This test works because it is being called from the main task
    **  which should not be an official OSAL task
    */
    res = OS_TaskInstallDeleteHandler(&delete_handler_callback);
    if (res == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Nominal - Bin-Sem-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        OS_BinSemTake(g_task_sync_sem);

        res =
            OS_TaskCreate(&g_task_ids[2], g_task_names[2], delete_handler_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                          sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
        if (res != OS_SUCCESS)
        {
            testDesc = "#2 Nominal - Task-Create-failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            /* Wait for the task to finish the test */
            OS_BinSemTake(g_task_sync_sem);
            /* Delay to let child task run */
            OS_TaskDelay(500);

            res = OS_TaskDelete(g_task_ids[2]);

            if (g_task_result == OS_SUCCESS)
                UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        }

        res = OS_BinSemDelete(g_task_sync_sem);
    }

UT_os_task_install_delete_handler_test_exit_tag:
    return;
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
    int32          res = 0;
    OS_task_prop_t task_prop;
    const char *   testDesc;

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#1 Nominal - Bin-Sem-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        OS_BinSemTake(g_task_sync_sem);

        res = OS_TaskCreate(&g_task_ids[1], g_task_names[1], exit_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                            sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
        if (res != OS_SUCCESS)
        {
            testDesc = "#1 Nominal - Task-Create failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            /* Wait for the task to finish the test */
            OS_BinSemTake(g_task_sync_sem);
            /* Delay to let the child task run */
            OS_TaskDelay(500);

            /*
            ** The only real way to tell if TaskExit ran is to check to see if the
            ** task ID is valid. It should not be valid
            */
            res = OS_TaskGetInfo(g_task_ids[1], &task_prop);
            if (res == OS_ERR_INVALID_ID)
                UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

            OS_TaskDelete(g_task_ids[1]); /* Won't hurt if its already deleted */
        }

        OS_BinSemDelete(g_task_sync_sem);
    }

    return;
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
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskDelay(100);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_delay_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_TaskDelay(100);
    if (res == OS_SUCCESS)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_task_delay_test_exit_tag:
    return;
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
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskCreate(&g_task_ids[0], g_task_names[0], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[0]),
                        sizeof(g_task_stacks[0]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#0 API not implemented - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_task_set_priority_test_exit_tag;
    }
    else
    {
        res = OS_TaskSetPriority(g_task_ids[0], OSAL_PRIORITY_C(UT_TASK_PRIORITY));
        if (res == OS_ERR_NOT_IMPLEMENTED)
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
            goto UT_os_task_set_priority_test_exit_tag;
        }

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Clean up */
        res = OS_TaskDelete(g_task_ids[0]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskSetPriority(UT_OBJID_INCORRECT, OSAL_PRIORITY_C(100));
    if (res == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-priority";

    res = OS_TaskCreate(&g_task_ids[2], g_task_names[2], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                        sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Invalid-priority - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[2]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TaskCreate(&g_task_ids[4], g_task_names[4], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[4]),
                        sizeof(g_task_stacks[4]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_TaskSetPriority(g_task_ids[4], OSAL_PRIORITY_C(UT_TASK_PRIORITY - 10));
        if (res == OS_SUCCESS)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        OS_TaskSetPriority(g_task_ids[4], OSAL_PRIORITY_C(UT_TASK_PRIORITY));
        OS_TaskDelete(g_task_ids[4]);
    }

UT_os_task_set_priority_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*/

void getid_test_task(void)
{
    osal_id_t      task_id;
    OS_task_prop_t task_prop;

    task_id = OS_TaskGetId();
    OS_TaskGetInfo(task_id, &task_prop);

    UtPrintf("OS_TaskGetId() - #1 Nominal [This is the returned task Id=%lx]\n", OS_ObjectIdToInteger(task_id));

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
    int32       res = 0;
    const char *testDesc;

    /*
     * Note this function does not return a normal status code,
     * there is no provision to return/check for OS_ERR_NOT_IMPLEMENTED.
     */

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[1], g_task_names[1], getid_test_task, OSAL_STACKPTR_C(&g_task_stacks[1]),
                        sizeof(g_task_stacks[1]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#1 Nominal - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        OS_TaskDelay(500);

        UtPrintf("OS_TaskGetId() - #1 Nominal [This is the expected task Id=%lx]\n",
                 OS_ObjectIdToInteger(g_task_ids[1]));

        res = OS_TaskDelete(g_task_ids[1]); /* Won't hurt if its already deleted */

        UT_OS_TEST_RESULT("#1 Nominal - Manual inspection required", UTASSERT_CASETYPE_MIR);
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
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_TaskGetIdByName(NULL, "InvalidName");
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_TaskGetIdByName(&g_task_ids[2], NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    res = OS_TaskGetIdByName(&g_task_ids[3], g_long_task_name);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_TaskGetIdByName(&g_task_ids[4], "NameNotFound");
    if (res == OS_ERR_NAME_NOT_FOUND)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[5], g_task_names[5], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[5]),
                        sizeof(g_task_stacks[5]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#5 Nominal - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_TaskGetIdByName(&g_task_ids[6], g_task_names[5]);
        if ((res == OS_SUCCESS) && OS_ObjectIdEqual(g_task_ids[5], g_task_ids[6]))
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        OS_TaskDelay(500); /* Delay to let task run */
        res = OS_TaskDelete(g_task_ids[5]);
    }

UT_os_task_get_id_by_name_test_exit_tag:
    return;
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
    int32          res = 0;
    OS_task_prop_t task_prop;
    const char *   testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskGetInfo(UT_OBJID_INCORRECT, &task_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_task_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskGetInfo(UT_OBJID_INCORRECT, &task_prop);
    if (res == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[2], g_task_names[2], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[2]),
                        sizeof(g_task_stacks[2]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Invalid-pointer-arg - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_TaskGetInfo(g_task_ids[2], NULL);
        if (res == OS_INVALID_POINTER)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[2]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, OSAL_STACKPTR_C(&g_task_stacks[3]),
                        sizeof(g_task_stacks[3]), OSAL_PRIORITY_C(UT_TASK_PRIORITY), 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Task-Create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_TaskGetInfo(g_task_ids[3], &task_prop);
        if (res == OS_SUCCESS)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[3]);
    }

UT_os_task_get_info_test_exit_tag:
    return;
}

/*================================================================================*
** End of File: ut_oscore_task_test.c
**================================================================================*/
