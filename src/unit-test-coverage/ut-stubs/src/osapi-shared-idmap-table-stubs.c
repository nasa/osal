/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-idmap.h"

OS_common_record_t OS_stub_task_table[OS_MAX_TASKS];
OS_common_record_t OS_stub_queue_table[OS_MAX_QUEUES];
OS_common_record_t OS_stub_bin_sem_table[OS_MAX_BIN_SEMAPHORES];
OS_common_record_t OS_stub_count_sem_table[OS_MAX_COUNT_SEMAPHORES];
OS_common_record_t OS_stub_mutex_table[OS_MAX_MUTEXES];
OS_common_record_t OS_stub_console_table[OS_MAX_CONSOLES];
OS_common_record_t OS_stub_module_table[OS_MAX_MODULES];
OS_common_record_t OS_stub_filesys_table[OS_MAX_FILE_SYSTEMS];
OS_common_record_t OS_stub_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timecb_table[OS_MAX_TIMERS];
OS_common_record_t OS_stub_stream_table[OS_MAX_NUM_OPEN_FILES];
OS_common_record_t OS_stub_dir_table[OS_MAX_NUM_OPEN_DIRS];

OS_common_record_t *const OS_global_task_table      = OS_stub_task_table;
OS_common_record_t *const OS_global_queue_table     = OS_stub_queue_table;
OS_common_record_t *const OS_global_bin_sem_table   = OS_stub_bin_sem_table;
OS_common_record_t *const OS_global_count_sem_table = OS_stub_count_sem_table;
OS_common_record_t *const OS_global_mutex_table     = OS_stub_mutex_table;
OS_common_record_t *const OS_global_stream_table    = OS_stub_stream_table;
OS_common_record_t *const OS_global_dir_table       = OS_stub_dir_table;
OS_common_record_t *const OS_global_timebase_table  = OS_stub_timebase_table;
OS_common_record_t *const OS_global_timecb_table    = OS_stub_timecb_table;
OS_common_record_t *const OS_global_module_table    = OS_stub_module_table;
OS_common_record_t *const OS_global_filesys_table   = OS_stub_filesys_table;
OS_common_record_t *const OS_global_console_table   = OS_stub_console_table;
