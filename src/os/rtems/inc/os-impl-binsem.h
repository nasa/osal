/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_BINSEM_H
#define OS_IMPL_BINSEM_H

#include "osconfig.h"
#include <rtems.h>

typedef struct
{
    rtems_id id;
} OS_impl_binsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

#endif /* OS_IMPL_BINSEM_H */
