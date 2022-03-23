/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_BINSEM_H
#define OS_IMPL_BINSEM_H

#include "osconfig.h"
#include <pthread.h>
#include <signal.h>

/* Binary Semaphores */
typedef struct
{
    pthread_mutex_t       id;
    pthread_cond_t        cv;
    volatile sig_atomic_t flush_request;
    volatile sig_atomic_t current_value;
} OS_impl_binsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

#endif /* OS_IMPL_BINSEM_H */
