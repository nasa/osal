/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_IDMAP_H
#define OS_IMPL_IDMAP_H

#include "osconfig.h"
#include "osapi-idmap.h"
#include <pthread.h>

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} OS_impl_objtype_lock_t;

/* Tables where the lock state information is stored */
extern OS_impl_objtype_lock_t *const OS_impl_objtype_lock_table[OS_OBJECT_TYPE_USER];

#endif /* OS_IMPL_IDMAP_H */
