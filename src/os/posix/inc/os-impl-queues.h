/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_QUEUES_H
#define OS_IMPL_QUEUES_H

#include "osconfig.h"
#include <mqueue.h>

/* queues */
typedef struct
{
    mqd_t id;
} OS_impl_queue_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_queue_internal_record_t OS_impl_queue_table[OS_MAX_QUEUES];

#endif /* OS_IMPL_QUEUES_H */
