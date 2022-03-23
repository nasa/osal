/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_MUTEX_H
#define OS_IMPL_MUTEX_H

#include "osconfig.h"
#include <semLib.h>

typedef struct
{
    VX_MUTEX_SEMAPHORE(mmem);
    SEM_ID vxid;
} OS_impl_mutsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_mutsem_internal_record_t OS_impl_mutex_table[OS_MAX_MUTEXES];

#endif /* OS_IMPL_MUTEX_H */
