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

