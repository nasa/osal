/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_COUNTSEM_H
#define OS_IMPL_COUNTSEM_H

#include "osconfig.h"
#include <semLib.h>

/* Counting & Binary Semaphores */
typedef struct
{
    VX_COUNTING_SEMAPHORE(cmem);
    SEM_ID vxid;
} OS_impl_countsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_countsem_internal_record_t OS_impl_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

#endif /* OS_IMPL_COUNTSEM_H */
