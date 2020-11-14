#ifndef _POSIX_MACOS_SEMAPHORE_H_
#define _POSIX_MACOS_SEMAPHORE_H_

#include <pthread/pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        pthread_mutex_t count_lock;
        pthread_cond_t  count_bump;
        unsigned        count;
    } mac_sem_t;

    int mac_sem_init(mac_sem_t *psem, int flags, unsigned count);
    int mac_sem_destroy(mac_sem_t *psem);
    int mac_sem_post(mac_sem_t *psem);
    int mac_sem_trywait(mac_sem_t *psem);
    int mac_sem_wait(mac_sem_t *psem);
    int mac_sem_timedwait(mac_sem_t *psem, const struct timespec *abstim);
    int mac_sem_getvalue(mac_sem_t *sem, int *sval);

/// sem_* functions are available on macOS but they are deprecated.
/// Additionally, the sem_timedwait() is not implemented on macOS.
/// Redefining sem_* to mac_sem2_*.
#undef sem_t
#define sem_t mac_sem_t

#undef sem_init
#define sem_init mac_sem_init

#undef sem_destroy
#define sem_destroy mac_sem_destroy

#undef sem_post
#define sem_post mac_sem_post

#undef sem_wait
#define sem_wait mac_sem_wait

#undef sem_trywait
#define sem_trywait mac_sem_trywait

#undef sem_timedwait
#define sem_timedwait mac_sem_timedwait

#undef sem_getvalue
#define sem_getvalue mac_sem_getvalue

#ifdef __cplusplus
}
#endif

#endif /* _POSIX_MACOS_SEMAPHORE_H_ */
