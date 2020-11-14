#ifndef _POSIX_MACOS_SEMAPHORE2_H_
#define _POSIX_MACOS_SEMAPHORE2_H_

#include <dispatch/dispatch.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef dispatch_semaphore_t mac_sem2_t;

    int mac_sem2_init(mac_sem2_t *psem, int flags, unsigned count);
    int mac_sem2_destroy(mac_sem2_t *psem);
    int mac_sem2_post(mac_sem2_t *psem);
    int mac_sem2_trywait(mac_sem2_t *psem);
    int mac_sem2_wait(mac_sem2_t *psem);
    int mac_sem2_timedwait(mac_sem2_t *psem, const struct timespec *abstim);
    int mac_sem2_getvalue(mac_sem2_t *sem, int *sval);

/// sem_* functions are available on macOS but they are deprecated.
/// Additionally, the sem_timedwait() is not implemented on macOS.
/// Redefining sem_* to mac_sem2_*.
#undef sem_t
#define sem_t mac_sem2_t

#undef sem_init
#define sem_init mac_sem2_init

#undef sem_destroy
#define sem_destroy mac_sem2_destroy

#undef sem_post
#define sem_post mac_sem2_post

#undef sem_wait
#define sem_wait mac_sem2_wait

#undef sem_trywait
#define sem_trywait mac_sem2_trywait

#undef sem_timedwait
#define sem_timedwait mac_sem2_timedwait

#undef sem_getvalue
#define sem_getvalue mac_sem2_getvalue

#ifdef __cplusplus
}
#endif

#endif /* _POSIX_MACOS_SEMAPHORE2_H_ */
