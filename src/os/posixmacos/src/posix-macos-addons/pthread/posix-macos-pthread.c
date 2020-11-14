#include "posix-macos-pthread.h"

#include <stdlib.h>
#include <errno.h>

int pthread_setschedprio(pthread_t thread, int prio)
{
    struct sched_param param;
    int                current_policy;

    int result = pthread_getschedparam(thread, &current_policy, &param);
    if (result != 0)
    {
        return result;
    }

    param.sched_priority = prio;
    result               = pthread_setschedparam(thread, current_policy, &param);

    return result;
}

/// https://www.mail-archive.com/dev@apr.apache.org/msg26846.html
int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
    int             rv;
    struct timespec remaining, slept, ts;

    remaining = *abs_timeout;
    while ((rv = pthread_mutex_trylock(mutex)) == EBUSY)
    {
        ts.tv_sec  = 0;
        ts.tv_nsec = (remaining.tv_sec > 0 ? 10000000 : (remaining.tv_nsec < 10000000 ? remaining.tv_nsec : 10000000));
        nanosleep(&ts, &slept);
        ts.tv_nsec -= slept.tv_nsec;
        if (ts.tv_nsec <= remaining.tv_nsec)
        {
            remaining.tv_nsec -= ts.tv_nsec;
        }
        else
        {
            remaining.tv_sec--;
            remaining.tv_nsec = (1000000 - (ts.tv_nsec - remaining.tv_nsec));
        }
        if (remaining.tv_sec < 0 || (!remaining.tv_sec && remaining.tv_nsec <= 0))
        {
            return ETIMEDOUT;
        }
    }

    return rv;
}
