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
** RwLock read timeout test
*/
#include <stdio.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

#define STACK_SIZE 4096

/* Define setup and check functions for UT assert */
void RwLockSetup(void);
void RwLockCheck(void);

uint32    orchestrator_stack[STACK_SIZE];
osal_id_t orchestrator_id;
uint32    orchestrator_failures;

uint32    reader_1_stack[STACK_SIZE];
osal_id_t reader_1_id;
uint32    reader_1_failures;

uint32    reader_2_stack[STACK_SIZE];
osal_id_t reader_2_id;
uint32    reader_2_failures;

osal_id_t rw_id;

bool reader_1_ready;
bool reader_2_ready;

bool should_run;
bool start_count;

uint32 counter;

void orchestrator(void)
{
    int    i;
    uint32 status;

    OS_printf("Starting ORCHESTRATOR...\n");

    while (should_run)
    {
        /* First, acquire the write lock and reset flags */
        start_count    = false;
        reader_1_ready = false;
        reader_2_ready = false;

        /* Wait for other tasks to try to acquire the read lock */
        status = OS_TaskDelay(500);
        if (reader_1_ready == true || reader_2_ready == true)
        {
            ++orchestrator_failures;
            OS_printf("ORCHESTRATOR:Error while holding write lock, readers read during rwlock write phase\n");
        }
        else
        {
            OS_printf("Single-writer only check passed!\n");
        }

        /* Release the write lock to allow readers to take the read lock */
        status = OS_RwLockWriteGive(rw_id);
        if (status != OS_SUCCESS)
        {
            ++orchestrator_failures;
            OS_printf("ORCHESTRATOR:Error calling OS_RwLockWriteGive\n");
        }

        /* Wait for readers to set their ready flags, once they've acquire the read lock */
        status = OS_TaskDelay(500);
        if (reader_1_ready != true || reader_2_ready != true)
        {
            ++orchestrator_failures;
            OS_printf("ORCHESTRATOR:Error, readers unable to simultaneously read\n");
        }
        else
        {
            OS_printf("Multiple-reader check passed!\n");
        }

        /* Reset reader flags and signal that they should release their read locks and start counting */
        reader_1_ready = false;
        reader_2_ready = false;
        start_count    = true;

        /* Increment the counter by 100 */
        for (i = 0; i < 100; ++i)
        {
            status = OS_RwLockWriteTake(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("ORCHESTRATOR:Error unable to take write lock\n");
                ++orchestrator_failures;
            }

            ++counter;
            status = OS_RwLockWriteGive(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("ORCHESTRATOR:Error unable to give write lock\n");
                ++orchestrator_failures;
            }
        }

        /* Wait for the readers to finish counting 100 as well */
        while (reader_1_ready != true || reader_2_ready != true)
        {
            OS_TaskDelay(5);
            if (should_run == false)
            {
                return;
            }
        }

        /* If the counter wasn't incremented correctly due to a data race, report the failure */
        if (counter % 300 != 0)
        {
            OS_printf("ORCHESTRATOR:Error counter did not match expected value: %d\n", counter);
            ++orchestrator_failures;
        }
        else
        {
            OS_printf("One parallel unit of work completed!\n");
        }

        if (counter >= 1500)
        {
            return;
        }

        /* Time limited execution */
        status = OS_RwLockWriteTake(rw_id);
        if (status != OS_SUCCESS)
        {
            ++orchestrator_failures;
            OS_printf("ORCHESTRATOR:Error calling OS_RwLockWriteTake\n");
        }
    }
}

void reader_1(void)
{
    int    i;
    uint32 status;

    OS_printf("Starting READER 1...");

    while (should_run)
    {
        /* Taking the read lock will take a while as the orchestrator will hold onto the write lock */
        status = OS_RwLockReadTake(rw_id);
        if (status != OS_SUCCESS)
        {
            ++reader_1_failures;
            OS_printf("READER 1:Error calling OS_RwLockReadTake");
        }

        /* Once the read lock is acquired, set the read flag to show the reader read it */
        reader_1_ready = true;

        /* Wait for the command to start counting */
        while (start_count != true)
        {
            OS_TaskDelay(5);
            if (should_run == false)
            {
                return;
            }
        }

        /* Once the command has been received return the read lock */
        status = OS_RwLockReadGive(rw_id);
        if (status != OS_SUCCESS)
        {
            ++reader_1_failures;
            OS_printf("READER 1:Error calling OS_RwLockReadGive");
        }

        /* Increment the counter by 100 */
        for (i = 0; i < 100; ++i)
        {
            status = OS_RwLockWriteTake(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("READER 1:Error unable to take write lock\n");
                ++reader_1_failures;
            }

            ++counter;
            status = OS_RwLockWriteGive(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("READER 1:Error unable to give write lock\n");
                ++reader_1_failures;
            }
        }

        /* Signal that the reader is ready to repeat the test */
        reader_1_ready = true;
        while (reader_1_ready == true)
        {
            OS_TaskDelay(5);
            if (should_run == false)
            {
                return;
            }
        }
    }
}

void reader_2(void)
{
    int    i;
    uint32 status;

    OS_printf("Starting READER 2...");

    while (should_run)
    {
        /* Taking the read lock will take a while as the orchestrator will hold onto the write lock */
        status = OS_RwLockReadTake(rw_id);
        if (status != OS_SUCCESS)
        {
            ++reader_2_failures;
            OS_printf("READER 2:Error calling OS_RwLockReadTake");
        }

        /* Once the read lock is acquired, set the read flag to show the reader read it */
        reader_2_ready = true;

        /* Wait for the command to start counting */
        while (start_count != true)
        {
            OS_TaskDelay(5);
            if (should_run == false)
            {
                return;
            }
        }

        /* Once the command has been received return the read lock */
        status = OS_RwLockReadGive(rw_id);
        if (status != OS_SUCCESS)
        {
            ++reader_2_failures;
            OS_printf("READER 2:Error calling OS_RwLockReadGive");
        }

        /* Increment the counter by 100 */
        for (i = 0; i < 100; ++i)
        {
            status = OS_RwLockWriteTake(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("READER 2:Error unable to take write lock\n");
                ++reader_2_failures;
            }

            ++counter;
            status = OS_RwLockWriteGive(rw_id);
            if (status != OS_SUCCESS)
            {
                OS_printf("READER 2:Error unable to give write lock\n");
                ++reader_2_failures;
            }
        }

        /* Signal that the reader is ready to repeat the test */
        reader_2_ready = true;
        while (reader_2_ready == true)
        {
            OS_TaskDelay(5);
            if (should_run == false)
            {
                return;
            }
        }
    }
}

bool RwLockTest_CheckImpl(void)
{
    int32     status;
    osal_id_t rwid;

    status = OS_RwLockCreate(&rwid, "ut", 0);
    if (status == OS_SUCCESS)
    {
        OS_RwLockDelete(rwid);
    }

    return (status != OS_ERR_NOT_IMPLEMENTED);
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
    if (RwLockTest_CheckImpl())
    {
        UtTest_Add(RwLockCheck, RwLockSetup, NULL, "RwLockTest");
    }
    else
    {
        UtAssert_MIR("Readers-writer locks not implemented; skipping tests");
    }
}

void RwLockSetup(void)
{
    uint32 status;

    orchestrator_failures = 0;
    reader_1_failures     = 0;
    reader_2_failures     = 0;
    counter               = 0;
    reader_1_ready        = false;
    reader_2_ready        = false;
    start_count           = false;
    should_run            = true;

    /* Create the RwLock */
    status = OS_RwLockCreate(&rw_id, "rwlock", 0);
    UtAssert_True(status == OS_SUCCESS, "RwLock ID=%lx, status=%d\n", OS_ObjectIdToInteger(rw_id), (int)status);

    /* Test for RwLock read nesting */
    status = OS_RwLockReadTake(rw_id);
    UtAssert_True(status == OS_SUCCESS, "OS_RwLockReadTake 1, status=%d\n", (int)status);

    status = OS_RwLockReadTake(rw_id);
    UtAssert_True(status == OS_SUCCESS, "OS_RwLockReadTake 2, status=%d\n", (int)status);

    status = OS_RwLockReadGive(rw_id);
    UtAssert_True(status == OS_SUCCESS, "OS_RwLockReadGive 2, status=%d\n", (int)status);

    status = OS_RwLockReadGive(rw_id);
    UtAssert_True(status == OS_SUCCESS, "OS_RwLockReadGive 1, status=%d\n", (int)status);

    /* Ensure the orchestrator has the write lock before the test begins. */
    status = OS_RwLockWriteTake(rw_id);
    UtAssert_True(status == OS_SUCCESS, "OS_RwLockWriteTake, status=%d\n", (int)status);

    /* Create the tasks */
    status = OS_TaskCreate(&reader_1_id, "Reader 1", reader_1, OSAL_STACKPTR_C(reader_1_stack), sizeof(reader_1_stack),
                           OSAL_PRIORITY_C(100), 0);
    UtAssert_True(status == OS_SUCCESS, "Reader ID=%lx, status=%d", OS_ObjectIdToInteger(reader_1_id), (int)status);

    status = OS_TaskCreate(&reader_2_id, "Reader 2", reader_2, OSAL_STACKPTR_C(reader_2_stack), sizeof(reader_2_stack),
                           OSAL_PRIORITY_C(100), 0);
    UtAssert_True(status == OS_SUCCESS, "Reader ID=%lx, status=%d", OS_ObjectIdToInteger(reader_2_id), (int)status);

    orchestrator();

    /* Delete the tasks */
    should_run = false;
    OS_TaskDelete(reader_1_id);
    OS_TaskDelete(reader_2_id);
}

void RwLockCheck(void)
{
    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(orchestrator_failures == 0, "Orchestrator failures = %u", (unsigned int)orchestrator_failures);
    UtAssert_True(reader_1_failures == 0, "Reader 1 failures = %u", (unsigned int)reader_1_failures);
    UtAssert_True(reader_2_failures == 0, "Reader 2 failures = %u", (unsigned int)reader_2_failures);
}
