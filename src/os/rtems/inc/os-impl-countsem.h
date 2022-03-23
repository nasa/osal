/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_COUNTSEM_H
#define OS_IMPL_COUNTSEM_H

#include "osconfig.h"
#include <rtems.h>

typedef struct
{
    rtems_id id;
} OS_impl_countsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_countsem_internal_record_t OS_impl_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

#endif /* OS_IMPL_COUNTSEM_H */
