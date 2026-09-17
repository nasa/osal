/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 * Mutex robustness test (nasa/cFE#2433)
 *
 * A task takes a mutex and is then deleted while it still holds it, as
 * happens to any task cancelled at a cancellation point during shutdown.
 * With a robust mutex the next OS_MutSemTake() from another task succeeds
 * and the mutex remains fully usable; without it the take blocks forever.
 *
 * This test is only built when OSAL_CONFIG_POSIX_ROBUST_MUTEX is TRUE.
 */

#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

#define TASK_STACK_SIZE 4096
#define TASK_1_PRIORITY 100

uint32    task_1_stack[TASK_STACK_SIZE];
osal_id_t task_1_id;
osal_id_t mut_sem_id;

volatile bool task_1_holds_mutex;

void task_1(void)
{
    int32 status;

    status = OS_MutSemTake(mut_sem_id);
    if (status != OS_SUCCESS)
    {
        OS_printf("TASK 1: OS_MutSemTake failed Rc=%d\n", (int)status);
        return;
    }

    task_1_holds_mutex = true;

    /* Hold the mutex until deleted; OS_TaskDelay is a cancellation point */
    while (1)
    {
        OS_TaskDelay(10);
    }
}

void MutexRobustSetup(void)
{
    int32 status;

    task_1_holds_mutex = false;

    status = OS_MutSemCreate(&mut_sem_id, "MutSemRobust", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate Id=%lx Rc=%d", OS_ObjectIdToInteger(mut_sem_id), (int)status);

    status = OS_TaskCreate(&task_1_id,
                           "Task 1",
                           task_1,
                           OSAL_STACKPTR_C(task_1_stack),
                           sizeof(task_1_stack),
                           OSAL_PRIORITY_C(TASK_1_PRIORITY),
                           0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate Id=%lx Rc=%d", OS_ObjectIdToInteger(task_1_id), (int)status);
}

void MutexRobustCheck(void)
{
    int32  status;
    uint32 wait_count;

    /* Wait for task 1 to take the mutex */
    for (wait_count = 0; wait_count < 100 && !task_1_holds_mutex; ++wait_count)
    {
        OS_TaskDelay(10);
    }
    UtAssert_True(task_1_holds_mutex, "Task 1 holds the mutex after %u waits", (unsigned int)wait_count);

    /* Delete the owner while it holds the mutex */
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskDelete Rc=%d", (int)status);

    /* Let the cancellation complete; deletion is not synchronous on every platform */
    OS_TaskDelay(100);

    /* A robust mutex is granted to the next taker instead of deadlocking it */
    status = OS_MutSemTake(mut_sem_id);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemTake after owner deleted Rc=%d", (int)status);

    status = OS_MutSemGive(mut_sem_id);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGive after recovery Rc=%d", (int)status);

    /* The recovered mutex stays usable */
    status = OS_MutSemTake(mut_sem_id);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemTake second Rc=%d", (int)status);

    status = OS_MutSemGive(mut_sem_id);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGive second Rc=%d", (int)status);

    status = OS_MutSemDelete(mut_sem_id);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete Rc=%d", (int)status);
}

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    UtTest_Add(MutexRobustCheck, MutexRobustSetup, NULL, "MutexRobustTest");
}
