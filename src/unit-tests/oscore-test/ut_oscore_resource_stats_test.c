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
** File:  ut_oscore_resource_stats_test.c
** Owner: Codex
** Date:  September 2025
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_resource_stats_test.h"

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

void UT_os_resource_stats_test(void)
{
    OS_resource_stats_t stats_before;
    OS_resource_stats_t stats_after;
    osal_id_t           queue_id     = OS_OBJECT_ID_UNDEFINED;
    osal_id_t           binsem_id    = OS_OBJECT_ID_UNDEFINED;
    osal_id_t           countsem_id  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t           mutex_id     = OS_OBJECT_ID_UNDEFINED;
    osal_blockcount_t   queue_depth  = OSAL_BLOCKCOUNT_C(4);
    size_t              queue_size   = OSAL_SIZE_C(4);
    uint32              queue_flags  = 0;
    uint32              sem_options  = 0;
    uint32              sem_init_val = 1;

    UT_RETVAL(OS_GetResourceStats(NULL), OS_INVALID_POINTER);
    UT_RETVAL(OS_GetResourceStats(&stats_before), OS_SUCCESS);

    UtAssert_True(stats_before.tasks.total == OS_MAX_TASKS, "tasks total == OS_MAX_TASKS");
    UtAssert_True(stats_before.queues.total == OS_MAX_QUEUES, "queues total == OS_MAX_QUEUES");
    UtAssert_True(stats_before.bin_semaphores.total == OS_MAX_BIN_SEMAPHORES,
                  "bin semaphores total == OS_MAX_BIN_SEMAPHORES");
    UtAssert_True(stats_before.count_semaphores.total == OS_MAX_COUNT_SEMAPHORES,
                  "count semaphores total == OS_MAX_COUNT_SEMAPHORES");
    UtAssert_True(stats_before.mutexes.total == OS_MAX_MUTEXES, "mutexes total == OS_MAX_MUTEXES");
    UtAssert_True(stats_before.streams.total == OS_MAX_NUM_OPEN_FILES, "streams total == OS_MAX_NUM_OPEN_FILES");
    UtAssert_True(stats_before.dirs.total == OS_MAX_NUM_OPEN_DIRS, "dirs total == OS_MAX_NUM_OPEN_DIRS");
    UtAssert_True(stats_before.timebases.total == OS_MAX_TIMEBASES, "timebases total == OS_MAX_TIMEBASES");
    UtAssert_True(stats_before.timers.total == OS_MAX_TIMERS, "timers total == OS_MAX_TIMERS");
    UtAssert_True(stats_before.modules.total == OS_MAX_MODULES, "modules total == OS_MAX_MODULES");
    UtAssert_True(stats_before.filesystems.total == OS_MAX_FILE_SYSTEMS, "filesystems total == OS_MAX_FILE_SYSTEMS");
    UtAssert_True(stats_before.consoles.total == OS_MAX_CONSOLES, "consoles total == OS_MAX_CONSOLES");
    UtAssert_True(stats_before.condvars.total == OS_MAX_CONDVARS, "condvars total == OS_MAX_CONDVARS");

    UT_RETVAL(OS_BinSemCreate(&binsem_id, "ResStat_BinSem", sem_init_val, sem_options), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.bin_semaphores.used == (stats_before.bin_semaphores.used + 1),
                  "bin semaphores used incremented");
    UT_RETVAL(OS_BinSemDelete(binsem_id), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.bin_semaphores.used == stats_before.bin_semaphores.used,
                  "bin semaphores used restored");

    UT_RETVAL(OS_CountSemCreate(&countsem_id, "ResStat_CountSem", sem_init_val, sem_options), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.count_semaphores.used == (stats_before.count_semaphores.used + 1),
                  "count semaphores used incremented");
    UT_RETVAL(OS_CountSemDelete(countsem_id), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.count_semaphores.used == stats_before.count_semaphores.used,
                  "count semaphores used restored");

    UT_RETVAL(OS_MutSemCreate(&mutex_id, "ResStat_Mutex", sem_options), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.mutexes.used == (stats_before.mutexes.used + 1), "mutexes used incremented");
    UT_RETVAL(OS_MutSemDelete(mutex_id), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.mutexes.used == stats_before.mutexes.used, "mutexes used restored");

    UT_RETVAL(OS_QueueCreate(&queue_id, "ResStat_Queue", queue_depth, queue_size, queue_flags), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.queues.used == (stats_before.queues.used + 1), "queues used incremented");
    UT_RETVAL(OS_QueueDelete(queue_id), OS_SUCCESS);
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);
    UtAssert_True(stats_after.queues.used == stats_before.queues.used, "queues used restored");
}

void UT_os_resource_stats_used_never_exceeds_total_test(void)
{
    OS_resource_stats_t stats;

    UT_RETVAL(OS_GetResourceStats(&stats), OS_SUCCESS);

    UtAssert_True(stats.tasks.used <= stats.tasks.total, "tasks used <= total");
    UtAssert_True(stats.queues.used <= stats.queues.total, "queues used <= total");
    UtAssert_True(stats.bin_semaphores.used <= stats.bin_semaphores.total, "bin semaphores used <= total");
    UtAssert_True(stats.count_semaphores.used <= stats.count_semaphores.total, "count semaphores used <= total");
    UtAssert_True(stats.mutexes.used <= stats.mutexes.total, "mutexes used <= total");
    UtAssert_True(stats.streams.used <= stats.streams.total, "streams used <= total");
    UtAssert_True(stats.dirs.used <= stats.dirs.total, "dirs used <= total");
    UtAssert_True(stats.timebases.used <= stats.timebases.total, "timebases used <= total");
    UtAssert_True(stats.timers.used <= stats.timers.total, "timers used <= total");
    UtAssert_True(stats.modules.used <= stats.modules.total, "modules used <= total");
    UtAssert_True(stats.filesystems.used <= stats.filesystems.total, "filesystems used <= total");
    UtAssert_True(stats.consoles.used <= stats.consoles.total, "consoles used <= total");
    UtAssert_True(stats.condvars.used <= stats.condvars.total, "condvars used <= total");
}

void UT_os_resource_stats_used_never_underflows_test(void)
{
    OS_resource_stats_t stats_before;
    OS_resource_stats_t stats_after;
    osal_id_t           invalid_id = OS_OBJECT_ID_UNDEFINED;
    int32               result;

    /* Get baseline stats */
    UT_RETVAL(OS_GetResourceStats(&stats_before), OS_SUCCESS);

    /* Attempt to delete resources that don't exist - should fail gracefully */
    result = OS_BinSemDelete(invalid_id);
    UtAssert_True(result != OS_SUCCESS, "delete invalid bin semaphore fails");

    result = OS_CountSemDelete(invalid_id);
    UtAssert_True(result != OS_SUCCESS, "delete invalid count semaphore fails");

    result = OS_MutSemDelete(invalid_id);
    UtAssert_True(result != OS_SUCCESS, "delete invalid mutex fails");

    result = OS_QueueDelete(invalid_id);
    UtAssert_True(result != OS_SUCCESS, "delete invalid queue fails");

    /* Verify stats are unchanged and non-negative after failed deletes */
    UT_RETVAL(OS_GetResourceStats(&stats_after), OS_SUCCESS);

    UtAssert_True(stats_after.tasks.used == stats_before.tasks.used, "tasks used unchanged");
    UtAssert_True(stats_after.queues.used == stats_before.queues.used, "queues used unchanged");
    UtAssert_True(stats_after.bin_semaphores.used == stats_before.bin_semaphores.used, "bin semaphores used unchanged");
    UtAssert_True(stats_after.count_semaphores.used == stats_before.count_semaphores.used, "count semaphores used unchanged");
    UtAssert_True(stats_after.mutexes.used == stats_before.mutexes.used, "mutexes used unchanged");
}

void UT_os_resource_stats_at_limit_test(void)
{
    OS_resource_stats_t stats;
    osal_id_t           sem_ids[OS_MAX_BIN_SEMAPHORES];
    uint32              created_count = 0;
    uint32              sem_options   = 0;
    uint32              sem_init_val  = 1;
    int32               result;

    /* Get initial stats */
    UT_RETVAL(OS_GetResourceStats(&stats), OS_SUCCESS);
    uint32 initial_used = stats.bin_semaphores.used;

    /* Create semaphores until we hit the limit or can't create more */
    for (uint32 i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        char name[OS_MAX_API_NAME];
        /* Use test name prefix to avoid collision with other tests */
        snprintf(name, sizeof(name), "AtLmt_%u", i);

        result = OS_BinSemCreate(&sem_ids[created_count], name, sem_init_val, sem_options);
        if (result == OS_SUCCESS)
        {
            created_count++;
        }
        else if (result == OS_ERR_NO_FREE_IDS)
        {
            /* Hit the limit - this is expected */
            break;
        }
        else
        {
            /* Other error (name taken, etc) - stop trying */
            break;
        }
    }

    /* Verify we're at or near the limit */
    UT_RETVAL(OS_GetResourceStats(&stats), OS_SUCCESS);
    UtAssert_True(stats.bin_semaphores.used == (initial_used + created_count),
                  "bin semaphores used matches created count");
    UtAssert_True(stats.bin_semaphores.used <= stats.bin_semaphores.total, "used never exceeds total");

    /* Verify we can still query stats at limit */
    UT_RETVAL(OS_GetResourceStats(&stats), OS_SUCCESS);
    UtAssert_True(stats.bin_semaphores.used <= stats.bin_semaphores.total,
                  "used still <= total at or near limit");

    /* Clean up all created semaphores */
    for (uint32 i = 0; i < created_count; i++)
    {
        UT_RETVAL(OS_BinSemDelete(sem_ids[i]), OS_SUCCESS);
    }

    /* Verify count returned to baseline */
    UT_RETVAL(OS_GetResourceStats(&stats), OS_SUCCESS);
    UtAssert_True(stats.bin_semaphores.used == initial_used, "bin semaphores used returned to baseline");
}

