/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_TASKS_H
#define OS_IMPL_TASKS_H

#include "os-shared-task.h"

#include "osconfig.h"
#include <pthread.h>

/*tasks */
typedef struct
{
    pthread_t id;
} OS_impl_task_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

int32 OS_Posix_InternalTaskCreate_Impl(pthread_t *pthr, osal_priority_t priority, size_t stacksz,
                                       PthreadFuncPtr_t entry, void *entry_arg);

#endif /* OS_IMPL_TASKS_H */
