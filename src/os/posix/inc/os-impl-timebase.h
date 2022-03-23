/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_TIMEBASE_H
#define OS_IMPL_TIMEBASE_H

#include "osconfig.h"
#include <pthread.h>
#include <signal.h>

typedef struct
{
    pthread_t       handler_thread;
    pthread_mutex_t handler_mutex;
    timer_t         host_timerid;
    int             assigned_signal;
    sigset_t        sigset;
    sig_atomic_t    reset_flag;
    struct timespec softsleep;

} OS_impl_timebase_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern OS_impl_timebase_internal_record_t OS_impl_timebase_table[OS_MAX_TIMEBASES];

#endif /* OS_IMPL_TIMEBASE_H */
