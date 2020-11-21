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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "osal-core-test.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Defines */
#define UT_EXIT_LOOP_MAX 100 /* Used to limit wait for self-exiting task to exit */

/* OS Constructs */
#define OSAL_UT_MAX_CALLBACKS 5
typedef struct
{
    uint32    NumInvocations;
    osal_id_t ObjList[OSAL_UT_MAX_CALLBACKS];
} TestCallbackState_t;

void TestTasks(void);
void InitializeTaskIds(void);
void InitializeQIds(void);
void InitializeBinIds(void);
void InitializeMutIds(void);
void TestQueues(void);
void TestBinaries(void);
void TestMutexes(void);
void TestGetInfos(void);
void TestGenericQueries(void);

/* helper function for "OS_ForEachObject" test cases */
static void TestForEachCallback(osal_id_t object_id, void *arg)
{
    TestCallbackState_t *State = (TestCallbackState_t *)arg;
    if (State->NumInvocations < OSAL_UT_MAX_CALLBACKS)
    {
        State->ObjList[State->NumInvocations] = object_id;
    }
    ++State->NumInvocations;
}

/* *************************************** MAIN ************************************** */
void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    UtTest_Add(TestTasks, NULL, NULL, "TASK");
    UtTest_Add(TestQueues, NULL, NULL, "MSGQ");
    UtTest_Add(TestBinaries, NULL, NULL, "BSEM");
    UtTest_Add(TestMutexes, NULL, NULL, "MSEM");
    UtTest_Add(TestGetInfos, NULL, NULL, "INFO");
    UtTest_Add(TestGenericQueries, NULL, NULL, "QUERIES");

} /* end OS_Application Startup */

/* **************** A TASK THAT RUNS FOREVER **************************** */

void task_generic_no_exit(void)
{
    OS_TaskRegister();

    while (1)
    {
        OS_TaskDelay(100);
    }

    return;
} /* end task_0 */

/* **************** A TASK THAT EXITS ITSELF **************************** */

void task_generic_with_exit(void)
{
    return;
} /* end task_0 */

typedef struct
{
    osal_id_t task_id;
    uint32    task_stack[TASK_0_STACK_SIZE];
} TestTaskData;
/* ********************************************** TASKS******************************* */
void TestTasks(void)
{
    int32               status;
    char                taskname[OS_MAX_API_NAME];
    int                 tasknum;
    osal_id_t           saved_task0_id;
    static TestTaskData TaskData[OS_MAX_TASKS + 1];
    OS_task_prop_t      taskprop;
    int                 loopcnt;

    /* OS_TaskRegister(); */

    /* Testing Creating up to OS_MAX_TASKS, plus one more */
    memset(TaskData, 0xFF, sizeof(TaskData));
    for (tasknum = 0; tasknum < (OS_MAX_TASKS + 1); ++tasknum)
    {
        snprintf(taskname, sizeof(taskname), "Task %d", tasknum);
        status = OS_TaskCreate(&TaskData[tasknum].task_id, taskname, task_generic_no_exit,
                               OSAL_STACKPTR_C(TaskData[tasknum].task_stack), sizeof(TaskData[tasknum].task_stack),
                               OSAL_PRIORITY_C(250 - OS_MAX_TASKS + tasknum), 0);

        UtDebug("Create %s Status = %d, Id = %lx\n", taskname, (int)status,
                OS_ObjectIdToInteger(TaskData[tasknum].task_id));

        UtAssert_True((tasknum < OS_MAX_TASKS && status == OS_SUCCESS) ||
                          (tasknum >= OS_MAX_TASKS && status != OS_SUCCESS),
                      "OS_TaskCreate, nominal");
    }

    /* Save off the task 0 ID for later */
    saved_task0_id = TaskData[0].task_id;

    /* Testing the Deletions of all the tasks we have created */
    for (tasknum = 0; tasknum < (OS_MAX_TASKS + 1); ++tasknum)
    {
        snprintf(taskname, sizeof(taskname), "Task %d", tasknum);
        status = OS_TaskDelete(TaskData[tasknum].task_id);

        UtDebug("Delete Status = %d, Id = %lx\n", (int)status, OS_ObjectIdToInteger(TaskData[tasknum].task_id));

        UtAssert_True((tasknum < OS_MAX_TASKS && status == OS_SUCCESS) ||
                          (tasknum >= OS_MAX_TASKS && status != OS_SUCCESS),
                      "OS_TaskDelete, nominal");
    }

    /* These next few tasks were deleted already, testing a "redelete" */
    UtAssert_True(OS_TaskDelete(TaskData[1].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 1");
    UtAssert_True(OS_TaskDelete(TaskData[2].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 2");
    UtAssert_True(OS_TaskDelete(TaskData[3].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 3");

    /* Testing tasks that exit themselves by calling OS_TaskExit() */
    memset(TaskData, 0xFF, sizeof(TaskData));
    for (tasknum = 0; tasknum < (OS_MAX_TASKS + 1); ++tasknum)
    {
        snprintf(taskname, sizeof(taskname), "Task %d", tasknum);
        status = OS_TaskCreate(&TaskData[tasknum].task_id, taskname, task_generic_with_exit,
                               OSAL_STACKPTR_C(TaskData[tasknum].task_stack), sizeof(TaskData[tasknum].task_stack),
                               OSAL_PRIORITY_C((250 - OS_MAX_TASKS) + tasknum), 0);

        UtDebug("Create %s Status = %d, Id = %lx\n", taskname, (int)status,
                OS_ObjectIdToInteger(TaskData[tasknum].task_id));

        UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, self exiting task");

        /* Looping delay in parent task to wait for child task to exit */
        loopcnt = 0;
        while ((OS_TaskGetInfo(TaskData[tasknum].task_id, &taskprop) == OS_SUCCESS) && (loopcnt < UT_EXIT_LOOP_MAX))
        {
            OS_TaskDelay(10);
            loopcnt++;
        }
        UtDebug("Looped %d times waiting for child task Id %lx to exit\n", loopcnt,
                OS_ObjectIdToInteger(TaskData[tasknum].task_id));
        UtAssert_True(loopcnt < UT_EXIT_LOOP_MAX, "Looped %d times without self-exiting task exiting", loopcnt);

        /*
         * Attempting to delete the task that exited itself should always fail
         */
        status = OS_TaskDelete(TaskData[tasknum].task_id);

        UtDebug("Delete Status = %d, Id = %lx\n", (int)status, OS_ObjectIdToInteger(TaskData[tasknum].task_id));

        UtAssert_True(status != OS_SUCCESS, "OS_TaskDelete, self exiting task");
    }

    /* Creating some more tasks for testing name functions */

    InitializeTaskIds();
    /* Create Task 0 again */
    status = OS_TaskCreate(&task_0_id, "Task 0", task_0, OSAL_STACKPTR_C(task_0_stack), sizeof(task_0_stack),
                           OSAL_PRIORITY_C(TASK_0_PRIORITY), 0);
    /*UtDebug("Create Status = %d, Id = %d\n",status,task_0_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 0");

    /* Try and create another "Task 0", should fail as we already have one named "Task 0" */
    status = OS_TaskCreate(&task_1_id, "Task 0", task_0, OSAL_STACKPTR_C(task_0_stack), sizeof(task_0_stack),
                           OSAL_PRIORITY_C(TASK_0_PRIORITY), 0);
    UtAssert_True(status != OS_SUCCESS, "OS_TaskCreate, dupe name 0");

    status = OS_TaskCreate(&task_2_id, "Task 2", task_2, OSAL_STACKPTR_C(task_2_stack), sizeof(task_2_stack),
                           OSAL_PRIORITY_C(TASK_2_PRIORITY), 0);
    /*  UtDebug("Create Status = %d, Id = %d\n",status,task_2_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 2");

    status = OS_TaskCreate(&task_3_id, "Task 3", task_3, OSAL_STACKPTR_C(task_3_stack), sizeof(task_3_stack),
                           OSAL_PRIORITY_C(TASK_3_PRIORITY), 0);
    /*  UtDebug("Create Status = %d, Id = %d\n",status,task_3_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 3");

    status = OS_TaskGetIdByName(&task_0_id, "Task 0");
    /* UtDebug("Satus after Getting the id of \"Task 0\":%d,%d \n\n",status,task_0_id); */
    /*first newly created task should have id == 0*/
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 0");

    status = OS_TaskGetIdByName(&task_1_id, "Task 1");
    /*UtDebug("Satus after Getting the id of \"Task 1\":%d,%d \n\n",status,task_1_id);*/
    UtAssert_True(status != OS_SUCCESS, "OS_TaskGetIdByName, Task 1");

    status = OS_TaskGetIdByName(&task_2_id, "Task 2");
    /* UtDebug("Satus after Getting the id of \"Task 2\":%d,%d \n\n",status,task_2_id);*/
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 2");

    status = OS_TaskGetIdByName(&task_3_id, "Task 3");
    /* UtDebug("Satus after Getting the id of \"Task 3\":%d,%d \n\n",status,task_3_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 3");

    /*
     * This should fail - verifies that ID re-use protections are working.
     * This only works with the new OSALs
     */
    UtAssert_True(OS_TaskDelete(saved_task0_id) != OS_SUCCESS, "OS_TaskDelete, Old ID");

    UtAssert_True(OS_TaskDelete(task_0_id) == OS_SUCCESS, "OS_TaskDelete, Task 0");
    UtAssert_True(OS_TaskDelete(task_1_id) != OS_SUCCESS, "OS_TaskDelete, Task 1");
    UtAssert_True(OS_TaskDelete(task_2_id) == OS_SUCCESS, "OS_TaskDelete, Task 2");
    UtAssert_True(OS_TaskDelete(task_3_id) == OS_SUCCESS, "OS_TaskDelete, Task 3");

} /* end TestTasks */

/* ************************************************************************************ */

void TestQueues(void)
{
    int32            status;
    char             qname[OS_MAX_API_NAME];
    int              qnum;
    osal_id_t        saved_queue0_id;
    static osal_id_t msgq_ids[OS_MAX_QUEUES + 1];

    InitializeQIds();
    memset(msgq_ids, 0xFF, sizeof(msgq_ids));

    for (qnum = 0; qnum < (OS_MAX_QUEUES + 1); ++qnum)
    {
        snprintf(qname, sizeof(qname), "q %d", qnum);
        status = OS_QueueCreate(&msgq_ids[qnum], qname, OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);

        UtAssert_True((qnum < OS_MAX_QUEUES && status == OS_SUCCESS) || (qnum >= OS_MAX_QUEUES && status != OS_SUCCESS),
                      "OS_QueueCreate, nominal");
    }

    saved_queue0_id = msgq_ids[0];
    /*     Trying now to Delete all of the Queues created. */

    for (qnum = 0; qnum < (OS_MAX_QUEUES + 1); ++qnum)
    {
        status = OS_QueueDelete(msgq_ids[qnum]);

        UtAssert_True((qnum < OS_MAX_QUEUES && status == OS_SUCCESS) || (qnum >= OS_MAX_QUEUES && status != OS_SUCCESS),
                      "OS_QueueDelete, nominal");
    }

    /*     Create Some more Queues for trying to get the id by name */

    InitializeQIds();
    status = OS_QueueCreate(&msgq_0, "q 0", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    /* UtDebug("Status after Creating q 0: %d,%d\n",status,msgq_0);*/
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 0");

    /* This one should fail */
    status = OS_QueueCreate(&msgq_1, "q 0", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    /* UtDebug("Status after Creating q 0 again: %d,%d\n",status,msgq_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_QueueCreate, dupe name 0");

    status = OS_QueueCreate(&msgq_2, "q 2", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    /* UtDebug("Status after Creating q 2: %d,%d\n",status,msgq_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 2");

    status = OS_QueueCreate(&msgq_3, "q 3", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    /* UtDebug("Status after Creating q 3: %d,%d\n",status,msgq_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 3");

    /*
     * Now that the Queues are created, its time to see if we can find
     * the propper ID by the name of the queue;
     */
    status = OS_QueueGetIdByName(&msgq_0, "q 0");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 0");

    status = OS_QueueGetIdByName(&msgq_1, "q 1");
    UtAssert_True(status != OS_SUCCESS, "OS_QueueGetIdByName, q 1");

    status = OS_QueueGetIdByName(&msgq_2, "q 2");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 2");

    status = OS_QueueGetIdByName(&msgq_3, "q 3");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 3");

    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_QueueDelete(saved_queue0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_QueueDelete, Old ID");

    /* Time to Delete the Queues we just created */

    status = OS_QueueDelete(msgq_0);
    /* UtDebug("Status after Deleting q 0 : %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 0");

    status = OS_QueueDelete(msgq_1);
    /* UtDebug("Status after Deleting q 1: %d\n",status); */
    UtAssert_True(status != OS_SUCCESS, "OS_QueueDelete, q 1");

    status = OS_QueueDelete(msgq_2);
    /* UtDebug("Status after Deleting q 2: %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 2");

    status = OS_QueueDelete(msgq_3);
    /* UtDebug("Status after Deleting q 3: %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 3");

} /* end TestQueues */

/* *************************************************************************** */
void TestBinaries(void)
{
    int32            status;
    char             bname[OS_MAX_API_NAME];
    int              bnum;
    osal_id_t        saved_bin0_id;
    static osal_id_t binsem_ids[OS_MAX_BIN_SEMAPHORES + 1];

    memset(binsem_ids, 0xFF, sizeof(binsem_ids));

    for (bnum = 0; bnum < (OS_MAX_BIN_SEMAPHORES + 1); ++bnum)
    {
        snprintf(bname, sizeof(bname), "Bin %d", bnum);
        status = OS_BinSemCreate(&binsem_ids[bnum], bname, 1, 0);

        UtAssert_True((bnum < OS_MAX_BIN_SEMAPHORES && status == OS_SUCCESS) ||
                          (bnum >= OS_MAX_BIN_SEMAPHORES && status != OS_SUCCESS),
                      "OS_BinSemCreate, nominal");
    }

    saved_bin0_id = binsem_ids[0];

    for (bnum = 0; bnum < (OS_MAX_BIN_SEMAPHORES + 1); ++bnum)
    {
        status = OS_BinSemDelete(binsem_ids[bnum]);

        UtAssert_True((bnum < OS_MAX_BIN_SEMAPHORES && status == OS_SUCCESS) ||
                          (bnum >= OS_MAX_BIN_SEMAPHORES && status != OS_SUCCESS),
                      "OS_BinSemDelete, nominal");
    }

    /*
     * Now Create a few extra semaphores
     *  to test  GetIdByName
     */
    InitializeBinIds();
    status = OS_BinSemCreate(&bin_0, "Bin 0", OS_SEM_FULL, 0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 0");

    status = OS_BinSemCreate(&bin_1, "Bin 0", OS_SEM_FULL, 0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemCreate, dupe name 0");

    status = OS_BinSemCreate(&bin_2, "Bin 2", OS_SEM_EMPTY, 0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_2);  */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 2");

    status = OS_BinSemCreate(&bin_3, "Bin 3", OS_SEM_EMPTY, 0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 3");

    status = OS_BinSemGetIdByName(&bin_0, "Bin 0");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 0");

    status = OS_BinSemGetIdByName(&bin_1, "Bin 1");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemGetIdByName, Bin 1");

    status = OS_BinSemGetIdByName(&bin_2, "Bin 2");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 2");

    status = OS_BinSemGetIdByName(&bin_3, "Bin 3");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 3");

    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_BinSemDelete(saved_bin0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemDelete, Old ID");

    status = OS_BinSemDelete(bin_0);
    /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 0");

    /* this one was never created */
    status = OS_BinSemDelete(bin_1);
    /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemDelete, Bin 1");

    status = OS_BinSemDelete(bin_2);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 2");

    status = OS_BinSemDelete(bin_3);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 3");

} /* end TestBinaries */

/* ************************************************************************************ */
void TestMutexes(void)
{
    int32            status;
    char             mname[OS_MAX_API_NAME];
    int              mnum;
    osal_id_t        saved_mut0_id;
    static osal_id_t mutex_ids[OS_MAX_MUTEXES + 1];

    memset(mutex_ids, 0xFF, sizeof(mutex_ids));

    for (mnum = 0; mnum < (OS_MAX_MUTEXES + 1); ++mnum)
    {
        snprintf(mname, sizeof(mname), "Mut %d", mnum);
        status = OS_MutSemCreate(&mutex_ids[mnum], mname, 0);

        UtAssert_True((mnum < OS_MAX_MUTEXES && status == OS_SUCCESS) ||
                          (mnum >= OS_MAX_MUTEXES && status != OS_SUCCESS),
                      "OS_MutSemCreate, nominal");
    }

    saved_mut0_id = mutex_ids[0];

    for (mnum = 0; mnum < (OS_MAX_MUTEXES + 1); ++mnum)
    {
        status = OS_MutSemDelete(mutex_ids[mnum]);

        UtAssert_True((mnum < OS_MAX_MUTEXES && status == OS_SUCCESS) ||
                          (mnum >= OS_MAX_MUTEXES && status != OS_SUCCESS),
                      "OS_MutSemDelete, nominal");
    }

    /*
     * Now Create a few extra semaphores
     *  to test  GetIdByName
     */
    InitializeMutIds();
    status = OS_MutSemCreate(&mut_0, "Mut 0", 0);
    /*  UtDebug("Status after creating Mut 0: %d,%d\n",status,mut_0);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 0");

    status = OS_MutSemCreate(&mut_1, "Mut 0", 0);
    /*  UtDebug("Status after creating Mut 0 again: %d,%d\n",status,mut_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemCreate, dupe name 0");

    status = OS_MutSemCreate(&mut_2, "Mut 2", 0);
    /*  UtDebug("Status after creating Mut 2: %d,%d\n",status,mut_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 2");

    status = OS_MutSemCreate(&mut_3, "Mut 3", 0);
    /*  UtDebug("Status after creating Mut 3: %d,%d\n",status,mut_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 3");

    status = OS_MutSemGetIdByName(&mut_0, "Mut 0");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 0");

    status = OS_MutSemGetIdByName(&mut_1, "Mut 1");
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemGetIdByName, Mut 1");

    status = OS_MutSemGetIdByName(&mut_2, "Mut 2");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 2");

    status = OS_MutSemGetIdByName(&mut_3, "Mut 3");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 3");

    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_MutSemDelete(saved_mut0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemDelete, Old ID");

    status = OS_MutSemDelete(mut_0);
    /*  UtDebug("Status after deleteing Mut 0:%d\n",status);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 0");

    /* this one was never created*/
    status = OS_MutSemDelete(mut_1);
    /*  UtDebug("Status after deleteing Mut 1:%d\n",status);  */
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemDelete, Mut 1");

    status = OS_MutSemDelete(mut_2);
    /*  UtDebug("Status after deleteing Mut 2:%d\n",status);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 2");

    status = OS_MutSemDelete(mut_3);
    /*  UtDebug("Status after deleteing Mut 3:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 3");

} /* end TestMutexes */

/* These next several tasks simply initialize the ids to a number which
 * cannot occur in the system itself. This helps avoid confusion when a create
 * fails, and the previous value of the id is displayed
 * */

/* ************************************************************************** */
void InitializeTaskIds(void)
{

    task_0_id  = OS_OBJECT_ID_UNDEFINED;
    task_1_id  = OS_OBJECT_ID_UNDEFINED;
    task_2_id  = OS_OBJECT_ID_UNDEFINED;
    task_3_id  = OS_OBJECT_ID_UNDEFINED;
    task_4_id  = OS_OBJECT_ID_UNDEFINED;
    task_5_id  = OS_OBJECT_ID_UNDEFINED;
    task_6_id  = OS_OBJECT_ID_UNDEFINED;
    task_7_id  = OS_OBJECT_ID_UNDEFINED;
    task_8_id  = OS_OBJECT_ID_UNDEFINED;
    task_9_id  = OS_OBJECT_ID_UNDEFINED;
    task_10_id = OS_OBJECT_ID_UNDEFINED;
    task_11_id = OS_OBJECT_ID_UNDEFINED;
    task_12_id = OS_OBJECT_ID_UNDEFINED;
    task_13_id = OS_OBJECT_ID_UNDEFINED;
    task_14_id = OS_OBJECT_ID_UNDEFINED;
    task_15_id = OS_OBJECT_ID_UNDEFINED;
    task_16_id = OS_OBJECT_ID_UNDEFINED;
    task_17_id = OS_OBJECT_ID_UNDEFINED;
    task_18_id = OS_OBJECT_ID_UNDEFINED;
    task_19_id = OS_OBJECT_ID_UNDEFINED;
    task_20_id = OS_OBJECT_ID_UNDEFINED;

    return;
} /* end InitializeTaskIds */

/* **************************************************************************** */
void InitializeQIds(void)
{
    msgq_0  = OS_OBJECT_ID_UNDEFINED;
    msgq_1  = OS_OBJECT_ID_UNDEFINED;
    msgq_2  = OS_OBJECT_ID_UNDEFINED;
    msgq_3  = OS_OBJECT_ID_UNDEFINED;
    msgq_4  = OS_OBJECT_ID_UNDEFINED;
    msgq_5  = OS_OBJECT_ID_UNDEFINED;
    msgq_6  = OS_OBJECT_ID_UNDEFINED;
    msgq_7  = OS_OBJECT_ID_UNDEFINED;
    msgq_8  = OS_OBJECT_ID_UNDEFINED;
    msgq_9  = OS_OBJECT_ID_UNDEFINED;
    msgq_10 = OS_OBJECT_ID_UNDEFINED;
    msgq_id = OS_OBJECT_ID_UNDEFINED;

    return;
} /* end InitializeQIds */

/* ***************************************************************************** */
void InitializeBinIds(void)
{
    bin_0  = OS_OBJECT_ID_UNDEFINED;
    bin_1  = OS_OBJECT_ID_UNDEFINED;
    bin_2  = OS_OBJECT_ID_UNDEFINED;
    bin_3  = OS_OBJECT_ID_UNDEFINED;
    bin_4  = OS_OBJECT_ID_UNDEFINED;
    bin_5  = OS_OBJECT_ID_UNDEFINED;
    bin_6  = OS_OBJECT_ID_UNDEFINED;
    bin_7  = OS_OBJECT_ID_UNDEFINED;
    bin_8  = OS_OBJECT_ID_UNDEFINED;
    bin_9  = OS_OBJECT_ID_UNDEFINED;
    bin_10 = OS_OBJECT_ID_UNDEFINED;
    return;
} /* end InitializeBinIds */

/* ***************************************************************************** */
void InitializeMutIds(void)
{
    mut_0  = OS_OBJECT_ID_UNDEFINED;
    mut_1  = OS_OBJECT_ID_UNDEFINED;
    mut_2  = OS_OBJECT_ID_UNDEFINED;
    mut_3  = OS_OBJECT_ID_UNDEFINED;
    mut_4  = OS_OBJECT_ID_UNDEFINED;
    mut_5  = OS_OBJECT_ID_UNDEFINED;
    mut_6  = OS_OBJECT_ID_UNDEFINED;
    mut_7  = OS_OBJECT_ID_UNDEFINED;
    mut_8  = OS_OBJECT_ID_UNDEFINED;
    mut_9  = OS_OBJECT_ID_UNDEFINED;
    mut_10 = OS_OBJECT_ID_UNDEFINED;
    return;
} /* end InitializeMutIds */
/* ***************************************************************************** */
void TestGetInfos(void)
{
    int               status;
    OS_task_prop_t    task_prop;
    OS_queue_prop_t   queue_prop;
    OS_bin_sem_prop_t bin_prop;
    OS_mut_sem_prop_t mut_prop;

    /* first step is to create an object to to get the properties of */

    status = OS_TaskCreate(&task_0_id, "Task 0", task_0, OSAL_STACKPTR_C(task_0_stack), sizeof(task_0_stack),
                           OSAL_PRIORITY_C(TASK_0_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate");

    status = OS_QueueCreate(&msgq_0, "q 0", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    /* UtDebug("Status after Creating q 0: %d,%d\n",status,msgq_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate");

    status = OS_BinSemCreate(&bin_0, "Bin 0", 1, 0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate");

    status = OS_MutSemCreate(&mut_0, "Mut 0", 0);
    /* UtDebug("Status after creating: %d,%d\n",status,mut_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate");

    /* Next Step is to get the properties of the objects */

    status = OS_TaskGetInfo(task_0_id, &task_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetInfo");

    status = OS_QueueGetInfo(msgq_0, &queue_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetInfo");

    status = OS_BinSemGetInfo(bin_0, &bin_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetInfo");

    status = OS_MutSemGetInfo(mut_0, &mut_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetInfo");

    status = OS_TaskDelete(task_0_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskDelete");

    status = OS_QueueDelete(msgq_0);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete");
    /* UtDebug("Status after Deleting q 0: %d\n",status); */

    status = OS_BinSemDelete(bin_0);
    /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete");

    status = OS_MutSemDelete(mut_0);
    /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete");
}

/*
 * Validate generic query functions such as OS_ForEachObject, OS_GetResourceName,
 * and any other function that operates on any generic/unspecified object type.
 */
void TestGenericQueries(void)
{
    int                 status;
    TestCallbackState_t State;
    char                ResourceName[OS_MAX_API_NAME];

    status = OS_TaskCreate(&task_0_id, "Task 0", task_0, OSAL_STACKPTR_C(task_0_stack), sizeof(task_0_stack),
                           OSAL_PRIORITY_C(TASK_0_PRIORITY), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate (%ld) == OS_SUCCESS", (long)status);

    status = OS_QueueCreate(&msgq_0, "q 0", OSAL_BLOCKCOUNT_C(MSGQ_DEPTH), OSAL_SIZE_C(MSGQ_SIZE), 0);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate (%ld) == OS_SUCCESS", (long)status);

    status = OS_BinSemCreate(&bin_0, "Bin 0", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate (%ld) == OS_SUCCESS", (long)status);

    status = OS_MutSemCreate(&mut_0, "Mut 0", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate (%ld) == OS_SUCCESS", (long)status);

    /* The "OS_ForEachObjectOfType()" should callback for only a specific object type -
     * spot check for Tasks and Bin Sem */
    memset(&State, 0, sizeof(State));
    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_TASK, OS_OBJECT_CREATOR_ANY, TestForEachCallback, &State);
    UtAssert_True(State.NumInvocations == 1, "Task NumInvocations (%lu) == 1", (unsigned long)State.NumInvocations);
    UtAssert_True(OS_ObjectIdEqual(State.ObjList[0], task_0_id), "Task ObjList[0] (%lx) == %lx",
                  OS_ObjectIdToInteger(State.ObjList[0]), OS_ObjectIdToInteger(task_0_id));

    memset(&State, 0, sizeof(State));
    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_BINSEM, OS_OBJECT_CREATOR_ANY, TestForEachCallback, &State);
    UtAssert_True(State.NumInvocations == 1, "BinSem NumInvocations (%lu) == 1", (unsigned long)State.NumInvocations);
    UtAssert_True(OS_ObjectIdEqual(State.ObjList[0], bin_0), "BinSem ObjList[0] (%lx) == %lx",
                  OS_ObjectIdToInteger(State.ObjList[0]), OS_ObjectIdToInteger(bin_0));

    memset(&State, 0, sizeof(State));
    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_COUNTSEM, OS_OBJECT_CREATOR_ANY, TestForEachCallback, &State);
    UtAssert_True(State.NumInvocations == 0, "CountSem NumInvocations (%lu) == 0", (unsigned long)State.NumInvocations);

    /*
     * The generic "OS_ForEachObject()" should callback for all object types.
     *
     * Note there are internally-generated object IDs that also may get returned here,
     * in addition to the ones which are explicitly created within this test.  Therefore
     * it is only verified that the callback was invoked for _at least_ the resources
     * created here, but OK if it is more than that.
     */
    memset(&State, 0, sizeof(State));
    OS_ForEachObject(OS_OBJECT_CREATOR_ANY, TestForEachCallback, &State);
    UtAssert_True(State.NumInvocations >= 4, "State.NumInvocations (%lu) >= 4", (unsigned long)State.NumInvocations);

    /* Test the OS_GetResourceName() API function */
    status = OS_GetResourceName(mut_0, ResourceName, OSAL_SIZE_C(0));
    UtAssert_True(status == OS_INVALID_POINTER, "OS_GetResourceName (%lx,%ld) == OS_INVALID_POINTER",
                  OS_ObjectIdToInteger(mut_0), (long)status);

    status = OS_GetResourceName(msgq_0, ResourceName, sizeof(ResourceName));
    UtAssert_True(status == OS_SUCCESS, "OS_GetResourceName (%lx,%ld) == OS_SUCCESS", OS_ObjectIdToInteger(msgq_0),
                  (long)status);
    UtAssert_StrCmp(ResourceName, "q 0", "Output value correct");

    status = OS_GetResourceName(OS_OBJECT_ID_UNDEFINED, ResourceName, sizeof(ResourceName));
    UtAssert_True(status == OS_ERR_INVALID_ID, "OS_GetResourceName (%lx,%ld) == OS_ERR_INVALID_ID",
                  OS_ObjectIdToInteger(OS_OBJECT_ID_UNDEFINED), (long)status);

    status = OS_GetResourceName(bin_0, ResourceName, OSAL_SIZE_C(1));
    UtAssert_True(status == OS_ERR_NAME_TOO_LONG, "OS_GetResourceName (%lx,%ld) == OS_ERR_NAME_TOO_LONG",
                  OS_ObjectIdToInteger(bin_0), (long)status);

    /* The OS_DeleteAllObjects() should clean up every object created here. */
    OS_DeleteAllObjects();
}
