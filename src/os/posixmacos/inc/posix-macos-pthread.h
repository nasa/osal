#ifndef _POSIX_MACOS_PTHREAD_H_
#define _POSIX_MACOS_PTHREAD_H_

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int pthread_setschedprio(pthread_t thread, int prio);
    int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout);

#ifdef __cplusplus
}
#endif

#endif /* _POSIX_MACOS_PTHREAD_H_ */
