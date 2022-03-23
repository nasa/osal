/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_TASKS_H
#define OS_IMPL_TASKS_H

#include "osconfig.h"
#include <rtems.h>

typedef struct
{
    rtems_id id;
} OS_impl_task_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

#endif /* OS_IMPL_TASKS_H */
