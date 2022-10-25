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

/*
** Binary Semaphore Flush Test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void CondVarSetup(void);
void CondVarCheck(void);

#define TASK_STACK_SIZE 4096
#define TASK_1_PRIORITY 100
#define TASK_2_PRIORITY 110
#define TASK_3_PRIORITY 120

#define NUM_TASKS 3

typedef struct condvar_task_state
{
    osal_id_t task_id;
    uint32    fail_count;
    uint32    run_count;
    uint32    mask;
} condvar_task_state_t;

typedef struct condvar_task_stack
{
    uint32 task_mem[TASK_STACK_SIZE];
} condvar_task_stack_t;

condvar_task_stack_t task_stacks[NUM_TASKS];
condvar_task_state_t task_states[NUM_TASKS];

osal_id_t condvar_id;

uint32 curr_condition;
uint32 total_work;

void condvar_worker(uint32 my_num)
{
    condvar_task_state_t *my_state;
    uint32                prev_condition;

    my_state       = &task_states[my_num];
    my_state->mask = 1 << my_num;

    UtPrintf("Starting task %u, mask=0x%x\n", (unsigned int)my_num, (unsigned int)my_state->mask);

    while (true)
    {
        /* Block here until the condition has a matching mask */
        UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);

        while ((curr_condition & my_state->mask) == 0)
        {
            if (!UtAssert_INT32_EQ(OS_CondVarWait(condvar_id), OS_SUCCESS))
            {
                break;
            }
        }

        prev_condition = curr_condition;
        curr_condition &= ~my_state->mask;
        ++total_work;

        UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

        ++my_state->run_count;
        UtPrintf("Doing Work task %u, mask=0x%x condition was=%x\n", (unsigned int)my_num, (unsigned int)my_state->mask,
                 (unsigned int)prev_condition);
        OS_TaskDelay(25 + (NUM_TASKS * 5));
    }
}

void task_1_entry(void)
{
    condvar_worker(0);
}

void task_2_entry(void)
{
    condvar_worker(1);
}

void task_3_entry(void)
{
    condvar_worker(2);
}

void CondVarTest_Setup(void)
{
    memset(&task_states, 0, sizeof(task_states));
    char            task_name[OS_MAX_API_NAME];
    osal_task_entry task_entry_points[NUM_TASKS] = {task_1_entry, task_2_entry, task_3_entry};
    uint32          i;

    curr_condition = 0;
    total_work     = 0;

    /*
    ** Create the condvar
    */
    UtAssert_INT32_EQ(OS_CondVarCreate(&condvar_id, "CondVar", 0), OS_SUCCESS);
    UtPrintf("CondVar create Id=%ld", OS_ObjectIdToInteger(condvar_id));

    /*
    ** Create the tasks
    */
    for (i = 0; i < NUM_TASKS; ++i)
    {
        snprintf(task_name, sizeof(task_name), "Task%u", (unsigned int)(i + 1));
        UtAssert_INT32_EQ(OS_TaskCreate(&task_states[i].task_id, task_name, task_entry_points[i],
                                        OSAL_STACKPTR_C(&task_stacks[i]), sizeof(task_stacks[i]),
                                        OSAL_PRIORITY_C(10 + (i * 10)), 0),
                          OS_SUCCESS);
        UtPrintf("%s create Id=%ld", task_name, OS_ObjectIdToInteger(task_states[i].task_id));
    }

    /* give a bit of time for those tasks to execute and block at the condvar */
    OS_TaskDelay(50);
}

void CondVarTest_Execute(void)
{
    static const uint32 ALL_RUN_CONDITION = (1 << NUM_TASKS) - 1;
    uint32              num_signals;
    uint32              i;

    num_signals = 0;

    /* now set the condvar so all tasks should run */
    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_UINT32_EQ(total_work, num_signals);
    curr_condition = ALL_RUN_CONDITION;
    UtAssert_INT32_EQ(OS_CondVarSignal(condvar_id), OS_SUCCESS);
    ++num_signals;
    OS_TaskDelay(5);
    /* this should still be true while holding the lock, other tasks are blocked, even though signaled */
    UtAssert_BITMASK_SET(curr_condition, ALL_RUN_CONDITION);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    /* Because it was signaled, not broadcast, only one task should have run */
    for (i = 0; i < (NUM_TASKS - 1); ++i)
    {
        OS_TaskDelay(5);
        UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
        UtAssert_UINT32_EQ(total_work, num_signals);
        UtAssert_UINT32_NEQ(curr_condition, ALL_RUN_CONDITION);
        UtAssert_UINT32_NEQ(curr_condition, 0);
        UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

        UtAssert_INT32_EQ(OS_CondVarSignal(condvar_id), OS_SUCCESS);
        ++num_signals;
    }

    OS_TaskDelay(5);

    /* after a the last signal, all the other tasks should have run */
    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    for (i = 0; i < NUM_TASKS; ++i)
    {
        UtAssert_UINT32_EQ(task_states[i].run_count, 1);
    }
    UtAssert_UINT32_EQ(total_work, num_signals);
    UtAssert_BITMASK_UNSET(curr_condition, ALL_RUN_CONDITION);
    curr_condition = ALL_RUN_CONDITION;
    UtAssert_INT32_EQ(OS_CondVarBroadcast(condvar_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    /* Give sufficient time for all tasks to wake up and do their work */
    OS_TaskDelay(100 + (NUM_TASKS * 10));

    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_BITMASK_UNSET(curr_condition, ALL_RUN_CONDITION);
    for (i = 0; i < NUM_TASKS; ++i)
    {
        UtAssert_UINT32_EQ(task_states[i].run_count, 2);
    }
    UtAssert_UINT32_EQ(total_work, NUM_TASKS * 2);
    curr_condition |= 0x1;
    UtAssert_INT32_EQ(OS_CondVarBroadcast(condvar_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    /* Give sufficient time for all tasks to wake up and do their work */
    OS_TaskDelay(100 + (NUM_TASKS * 10));

    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_UINT32_EQ(total_work, (NUM_TASKS * 2) + 1);
    UtAssert_UINT32_EQ(task_states[0].run_count, 3);
    UtAssert_BITMASK_UNSET(curr_condition, ALL_RUN_CONDITION);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);
}

void CondVarTest_Teardown(void)
{
    uint32 i;

    /*
     * Delete the tasks
     */
    for (i = 0; i < NUM_TASKS; ++i)
    {
        UtAssert_INT32_EQ(OS_TaskDelete(task_states[i].task_id), OS_SUCCESS);
        UtPrintf("Task%u delete Id=%ld", (unsigned int)(i + 1), OS_ObjectIdToInteger(task_states[i].task_id));
    }

    /* SANITY CHECK: confirm the condvar is still lockable (that is, deletion did not leave it in a deadlocked state) */
    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    UtAssert_INT32_EQ(OS_CondVarDelete(condvar_id), OS_SUCCESS);
}

OS_time_t testtm_ref1;
OS_time_t testtm_ref2;
OS_time_t testtm_ref3;

void condvar_timedtask_entry(void)
{
    OS_time_t endtm;
    OS_time_t elapsed;

    UtPrintf("Starting OS_CondVarTimedWait task\n");

    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);

    OS_GetLocalTime(&testtm_ref1);

    /* the first time this should time out */
    endtm = OS_TimeAdd(testtm_ref1, OS_TimeAssembleFromMilliseconds(0, 200));
    UtAssert_INT32_EQ(OS_CondVarTimedWait(condvar_id, &endtm), OS_ERROR_TIMEOUT);

    OS_GetLocalTime(&testtm_ref2);
    ++total_work;

    elapsed = OS_TimeSubtract(testtm_ref2, testtm_ref1);
    UtAssert_UINT32_GTEQ(OS_TimeGetTotalMilliseconds(elapsed), 200);

    /* thus time this should NOT time out */
    endtm = OS_TimeAdd(testtm_ref2, OS_TimeAssembleFromMilliseconds(0, 200));
    UtAssert_INT32_EQ(OS_CondVarTimedWait(condvar_id, &endtm), OS_SUCCESS);

    OS_GetLocalTime(&testtm_ref3);
    ++total_work;

    elapsed = OS_TimeSubtract(testtm_ref3, testtm_ref2);
    UtAssert_UINT32_LT(OS_TimeGetTotalMilliseconds(elapsed), 200);

    /* all done now, wait here for the task to be deleted */
    endtm = OS_TimeAdd(testtm_ref3, OS_TimeAssembleFromMilliseconds(1800, 0));
    UtAssert_INT32_EQ(OS_CondVarTimedWait(condvar_id, &endtm), OS_SUCCESS);

    UtAssert_Abort("Should not reach this point");
}

void CondVarTimedWait_Setup(void)
{
    total_work = 0;

    UtAssert_INT32_EQ(OS_CondVarCreate(&condvar_id, "CondVar", 0), OS_SUCCESS);

    UtAssert_INT32_EQ(OS_TaskCreate(&task_states[0].task_id, "timedwait", condvar_timedtask_entry,
                                    OSAL_STACKPTR_C(&task_stacks[0]), sizeof(task_stacks[0]), OSAL_PRIORITY_C(10), 0),
                      OS_SUCCESS);
}

void CondVarTimedWait_Execute(void)
{
    /* TimedWait Subcase 1 - nothing signals the condvar, should get a timeout */
    /* Small delay to ensure that the task gets into OS_CondVarTimedWait() */
    OS_TaskDelay(5);
    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    while (total_work == 0)
    {
        /* note this is NOT using condvar signalling here, just a simple poll, but be sure to unlock before waiting */
        /* Also note, not using the Assert macro here since it clutters the log significantly */
        OS_CondVarUnlock(condvar_id);
        OS_TaskDelay(5);
        OS_CondVarLock(condvar_id);
    }

    /* there should have been at least 200ms delay before/after the call */
    UtAssert_UINT32_EQ(total_work, 1);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    /* TimedWait Subcase 2 - signal the condvar, should not get a timeout */
    OS_TaskDelay(5);
    UtAssert_INT32_EQ(OS_CondVarSignal(condvar_id), OS_SUCCESS);

    /* Another small delay to let the task run */
    OS_TaskDelay(5);

    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_UINT32_EQ(total_work, 2);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);
}

void CondVarTimedWait_Teardown(void)
{
    UtAssert_INT32_EQ(OS_TaskDelete(task_states[0].task_id), OS_SUCCESS);

    /* SANITY CHECK: confirm the condvar is still lockable (that is, deletion did not leave it in a deadlocked state) */
    UtAssert_INT32_EQ(OS_CondVarLock(condvar_id), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_CondVarUnlock(condvar_id), OS_SUCCESS);

    UtAssert_INT32_EQ(OS_CondVarDelete(condvar_id), OS_SUCCESS);
}

void CondVarTest_Ops(void)
{
    uint32            i;
    char              cv_name[OS_MAX_API_NAME];
    osal_id_t         cv_id[OS_MAX_CONDVARS];
    osal_id_t         cv_extra;
    OS_condvar_prop_t cv_prop;

    UtAssert_INT32_EQ(OS_CondVarCreate(NULL, "cvex", 0), OS_INVALID_POINTER);
    UtAssert_INT32_EQ(OS_CondVarCreate(&cv_extra, NULL, 0), OS_INVALID_POINTER);

    for (i = 0; i < OS_MAX_CONDVARS; ++i)
    {
        snprintf(cv_name, sizeof(cv_name), "cv%03u", (unsigned int)i);
        UtAssert_INT32_EQ(OS_CondVarCreate(&cv_id[i], cv_name, 0), OS_SUCCESS);
    }

    UtAssert_INT32_EQ(OS_CondVarCreate(&cv_extra, "cvex", 0), OS_ERR_NO_FREE_IDS);
    UtAssert_INT32_EQ(OS_CondVarDelete(cv_id[OS_MAX_CONDVARS - 1]), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_CondVarCreate(&cv_extra, "cv000", 0), OS_ERR_NAME_TAKEN);

    UtAssert_INT32_EQ(OS_CondVarGetIdByName(&cv_extra, "cv000"), OS_SUCCESS);
    UtAssert_True(OS_ObjectIdEqual(cv_extra, cv_id[0]), "objid (%lu) == cv_id[0] (%lu)", OS_ObjectIdToInteger(cv_extra),
                  OS_ObjectIdToInteger(cv_id[0]));
    UtAssert_INT32_EQ(OS_CondVarGetIdByName(&cv_extra, "cvex"), OS_ERR_NAME_NOT_FOUND);

    UtAssert_INT32_EQ(OS_CondVarGetInfo(cv_id[0], &cv_prop), OS_SUCCESS);
    UtAssert_STRINGBUF_EQ(cv_prop.name, sizeof(cv_prop.name), "cv000", UTASSERT_STRINGBUF_NULL_TERM);
    UtAssert_INT32_EQ(OS_CondVarGetInfo(cv_id[OS_MAX_CONDVARS - 1], &cv_prop), OS_ERR_INVALID_ID);
    UtAssert_INT32_EQ(OS_CondVarGetInfo(OS_OBJECT_ID_UNDEFINED, &cv_prop), OS_ERR_INVALID_ID);

    for (i = 0; i < (OS_MAX_CONDVARS - 1); ++i)
    {
        snprintf(cv_name, sizeof(cv_name), "cv%03u", (unsigned int)i);
        UtAssert_INT32_EQ(OS_CondVarDelete(cv_id[i]), OS_SUCCESS);
    }
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(CondVarTest_Ops, NULL, NULL, "CondVarOps");
    UtTest_Add(CondVarTest_Execute, CondVarTest_Setup, CondVarTest_Teardown, "CondVarBasic");
    UtTest_Add(CondVarTimedWait_Execute, CondVarTimedWait_Setup, CondVarTimedWait_Teardown, "CondVarTimed");
}
